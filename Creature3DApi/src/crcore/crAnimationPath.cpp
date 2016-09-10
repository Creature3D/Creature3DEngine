/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2006 Robert Osfield 
 *
 * This library is open source and may be redistributed and/or modified under  
 * the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or 
 * (at your option) any later version.  The full license is in LICENSE file
 * included with this distribution, and on the openscenegraph.org website.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * OpenSceneGraph Public License for more details.
*/
//Modified by Wucaihua
#include <CRCore/crAnimationPath.h>
#include <CRCore/crMatrixTransform.h>
#include <CRCore/crPositionAttitudeTransform.h>
#include <CRCore/crArgumentParser.h>
#include <CRCore/crWchMsg.h>

using namespace CRCore;

void crAnimationPath::insert(double time,const ControlPoint& controlPoint)
{
	CRCore::ScopedLock<crMutex> lock(m_mutex);
    m_timeControlPointMap[time] = controlPoint;
}

bool crAnimationPath::getInterpolatedControlPoint(double time,ControlPoint& controlPoint) const
{
    if (m_timeControlPointMap.empty()) return false;
    
    switch(m_loopMode)
    {
        case(SWING):
        {
            double modulated_time = (time - getFirstTime())/(getPeriod()*2.0);
            double fraction_part = modulated_time - floor(modulated_time);
            if (fraction_part>0.5) fraction_part = 1.0-fraction_part;
            
            time = getFirstTime()+(fraction_part*2.0) * getPeriod();
            break;
        }
        case(LOOP):
        {
            double modulated_time = (time - getFirstTime())/getPeriod();
            double fraction_part = modulated_time - floor(modulated_time);
            time = getFirstTime()+fraction_part * getPeriod();
            break;
        }
        case(NO_LOOPING):
            break;
    }
    
	CRCore::ScopedLock<crMutex> lock(m_mutex);
    TimeControlPointMap::const_iterator second = m_timeControlPointMap.lower_bound(time);
    if (second==m_timeControlPointMap.begin())
    {
        controlPoint = second->second;
    }
    else if (second!=m_timeControlPointMap.end())
    {
        TimeControlPointMap::const_iterator first = second;
        --first;        
        
        // deta_time = second.time - first.time
        double delta_time = second->first - first->first;

        if (delta_time==0.0)
		{
            controlPoint = first->second;
		}
        else
        {
            controlPoint.interpolate((time - first->first)/delta_time,
                            first->second,
                            second->second);
        }        
    }
    else // (second==m_timeControlPointMap.end())
    {
        controlPoint = m_timeControlPointMap.rbegin()->second;
    }
    return true;
}


void crAnimationPath::read(std::istream& in)
{
    while (!in.eof())
    {
        double time;
        CRCore::crVector3 position;
        CRCore::crQuat rotation;
        in >> time >> position.x() >> position.y() >> position.z() >> rotation.x() >> rotation.y() >> rotation.z() >> rotation.w();
        if(!in.eof())
            insert(time,CRCore::crAnimationPath::ControlPoint(position,rotation));
    }
}

void crAnimationPath::write(std::ostream& fout)
{
    const TimeControlPointMap& tcpm = getTimeControlPointMap();
    for(TimeControlPointMap::const_iterator tcpmitr=tcpm.begin();
        tcpmitr!=tcpm.end();
        ++tcpmitr)
    {
        const ControlPoint& cp = tcpmitr->second;
        fout<<tcpmitr->first<<" "<<cp.m_position<<" "<<cp.m_rotation<<std::endl;
    }
}

void crAnimationPath::ControlPoint::getMatrix(crMatrixf& matrix) const
{
	float x,y,z;
	m_rotation.getRadians(x,y,z);

	if(fabs(x) < 0.0001f && fabs(y) < 0.0001f && fabs(z) < 0.0001f) 
		matrix.makeScale(m_scale);
	else
	    matrix.postMult(CRCore::crMatrixf::rotate(m_rotation));
	matrix.postMult(CRCore::crMatrixf::translate(m_position));
}

void crAnimationPath::ControlPoint::getMatrix(crMatrixd& matrix) const
{
	float x,y,z;
	m_rotation.getRadians(x,y,z);

	if(fabs(x) < 0.0001f && fabs(y) < 0.0001f && fabs(z) < 0.0001f) 
		matrix.makeScale(m_scale);
	else
	    matrix.postMult(CRCore::crMatrixf::rotate(m_rotation));
	matrix.postMult(CRCore::crMatrixf::translate(m_position));
}

void crAnimationPath::ControlPoint::getInverse(crMatrixf& matrix) const
{
	matrix.makeScale(1.0f/m_scale.x(),1.0f/m_scale.y(),1.0f/m_scale.y());
	matrix.preMult(CRCore::crMatrixf::rotate(m_rotation.inverse()));
	matrix.preMult(CRCore::crMatrixf::translate(-m_position));
}

void crAnimationPath::ControlPoint::getInverse(crMatrixd& matrix) const
{
	matrix.makeScale(1.0f/m_scale.x(),1.0f/m_scale.y(),1.0f/m_scale.y());
	matrix.preMult(CRCore::crMatrixd::rotate(m_rotation.inverse()));
	matrix.preMult(CRCore::crMatrixd::translate(-m_position));
}

class AnimationPathCallbackVisitor : public crNodeVisitor
{
    public:

        AnimationPathCallbackVisitor(const crAnimationPath::ControlPoint& cp,const CRCore::crVector3d& pivotPoint,bool useInverseMatrix):
            m_cp(cp),
		    m_pivotPoint(pivotPoint),
            m_useInverseMatrix(useInverseMatrix) {}

        virtual void apply(crMatrixTransform& mt)
        {
            crMatrix matrix;
            if (m_useInverseMatrix)
                m_cp.getInverse(matrix);
            else
                m_cp.getMatrix(matrix);
                
			mt.setMatrix(matrix*crMatrix::translate(m_pivotPoint));
        }
        
        virtual void apply(crPositionAttitudeTransform& pat)
        {
            if (m_useInverseMatrix)
            {
                crMatrix matrix;
                m_cp.getInverse(matrix);
                pat.setPosition(matrix.getTrans());
                pat.setAttitude(m_cp.m_rotation.inverse());
				pat.setScale(CRCore::crVector3d(1.0f/m_cp.getScale().x(),1.0f/m_cp.getScale().y(),1.0f/m_cp.getScale().z()));
				pat.setPivotPoint(m_pivotPoint);
                
            }
            else
            {
                pat.setPosition(m_cp.m_position);
                pat.setAttitude(m_cp.m_rotation);
				pat.setScale(m_cp.getScale());
				pat.setPivotPoint(m_pivotPoint);
            }
        }
        
        crAnimationPath::ControlPoint m_cp;
		crVector3d m_pivotPoint;
        bool m_useInverseMatrix;      
};

void crAnimationPathCallback::operator()(crNode* node, crNodeVisitor* nv)
{
    if (m_animationPath.valid() && 
        nv->getVisitorType()==crNodeVisitor::UPDATE_VISITOR)
    {
        double time = crFrameStamp::getInstance()->getReferenceTime();
        m_latestTime = time;

        if (!m_pause)
        {
            if (m_firstTime==DBL_MAX) m_firstTime = time;
            update(*node);
        }
    }
    
    crNodeCallback::traverse(node,nv);
}

double crAnimationPathCallback::getAnimationTime() const
{
    return ((m_latestTime-m_firstTime)-m_timeOffset)*m_timeMultiplier;
}

void crAnimationPathCallback::update(CRCore::crNode& node)
{
    crAnimationPath::ControlPoint cp;
	double time = getAnimationTime();
    if (m_animationPath->getInterpolatedControlPoint(time,cp))
    {
        AnimationPathCallbackVisitor apcv(cp,m_pivotPoint,m_useInverseMatrix);
        node.accept(apcv);
		node.doEvent(WCH_AnimationEvent,MAKEINT64(this,&cp));
    }
}


void crAnimationPathCallback::reset()
{
    m_firstTime = m_latestTime;
    m_pauseTime = m_latestTime;
}

void crAnimationPathCallback::setPause(bool pause)
{
    if (m_pause==pause)
    {
        return;
    }
    
    m_pause = pause;
    if (m_pause)
    {
        m_pauseTime = m_latestTime;
    }
    else
    {
        m_firstTime += (m_latestTime-m_pauseTime);
    }
}

void crAnimationPath::addEventParam(int i, const std::string &str)
{
	int j = i%4;
	if(j == 0)
	{
		m_time_load = atof(str.c_str());
	}
	else if(j==1)
	{
		crArgumentParser::appAtoVec(str,m_position_load);
	}
	else if(j==2)
	{
		crVector4 rot;
		crArgumentParser::appAtoVec(str,rot);
		m_rotation_load.set(rot);
	}
	else if(j==3)
	{
		crArgumentParser::appAtoVec(str,m_scale_load);
		insert(m_time_load,crAnimationPath::ControlPoint(m_position_load,m_rotation_load,m_scale_load));
	}
}

void crAnimationPathCallback::addEventParam(int i, const std::string &str)
{
    if(!m_animationPath.valid())
		m_animationPath = new crAnimationPath;
    switch (i)
    {
    case 0:
		if(str.compare("SWING") == 0)
			m_animationPath->setLoopMode(crAnimationPath::SWING);
		else if(str.compare("LOOP") == 0)
			m_animationPath->setLoopMode(crAnimationPath::LOOP);
		else
			m_animationPath->setLoopMode(crAnimationPath::NO_LOOPING);
    	break;
	case 1:
		{
			crVector3 vec3;
            crArgumentParser::appAtoVec(str,vec3);
			m_pivotPoint = vec3;
		}
		break;
	case 2:
		m_useInverseMatrix = atoi(str.c_str());
		break;
	case 3:
		m_timeOffset = atof(str.c_str());
		break;
	case 4:
		m_timeMultiplier = atof(str.c_str());
		break;
	case 5:
		m_pause = atoi(str.c_str());
		break;
	default:
		m_animationPath->addEventParam(i-6,str);
    }

}