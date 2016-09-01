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
#include <CRCore/crSequence.h>
#include <CRCore/crBillboard.h>
//#include <CRCore/Notify>

using namespace CRCore;

/**
* crSequence constructor.
*/
crSequence::crSequence() :
crGroup(),
m_nValue(-1),
m_fLast(-1.0f),
m_nStep(1),
m_loopMode(LOOP),
m_nBegin(0),
m_nEnd(-1),
m_fSpeed(0),
m_nNreps(0),
m_nNrepsremain(0),
m_mode(STOP),
m_vanishWhenStop(true),
m_frameCount(0)
{
	setNumChildrenRequiringUpdateTraversal(1);
	m_dataVariance = DYNAMIC;
}

crSequence::crSequence(const crSequence& seq, const crCopyOp& copyop) :
crGroup(seq, copyop),
m_nValue(seq.m_nValue),
m_fLast(seq.m_fLast),
m_vecFrameTime(seq.m_vecFrameTime),
m_nStep(seq.m_nStep),
m_loopMode(seq.m_loopMode),
m_nBegin(seq.m_nBegin),
m_nEnd(seq.m_nEnd),
m_fSpeed(seq.m_fSpeed),
m_nNreps(seq.m_nNreps),
m_nNrepsremain(seq.m_nNrepsremain),
m_mode(seq.m_mode),
m_vanishWhenStop(seq.m_vanishWhenStop),
m_frameCount(seq.m_frameCount)
{
	setNumChildrenRequiringUpdateTraversal(getNumChildrenRequiringUpdateTraversal()+1);            
}

void crSequence::setTime(int frame, float t)
{
	int sz = m_vecFrameTime.size();
	//CRCore::notify(CRCore::NOTICE)<<" name = "<<getName()<< "sz=" << sz << " frame=" << frame<<" t=" << t << std::endl;
	if (frame < sz)
		m_vecFrameTime[frame] = t;
	else
		for (int i = sz; i < (frame+1); i++) 
		{
			m_vecFrameTime.push_back(t);
		}
}

float crSequence::getTime(int frame) const
{
	if (frame >= 0 && frame < (int) m_vecFrameTime.size())
		return m_vecFrameTime[frame];
	else
		return -1.0f;
}

void crSequence::setInterval(LoopMode mode, int begin, int end)
{
	m_loopMode = mode;
	m_nBegin = begin;
	m_nEnd = end;

	// switch to beginning of interval
	unsigned int nch = getNumChildren();
	m_frameCount = nch;
	begin = (m_nBegin < 0 ? nch + m_nBegin : m_nBegin);
	end = (m_nEnd < 0 ? nch + m_nEnd : m_nEnd);

	setValue(begin);
	m_nStep = (begin < end ? 1 : -1);
}

void crSequence::setInterval(LoopMode mode, int begin, int end, int frameCount)
{
	m_loopMode = mode;
	m_nBegin = begin;
	m_nEnd = end;
	m_frameCount = frameCount;

	if(frameCount>0)
	{
		begin = (m_nBegin < 0 ? frameCount + m_nBegin : m_nBegin);
		end = (m_nEnd < 0 ? frameCount + m_nEnd : m_nEnd);

		setValue(begin);
		m_nStep = (begin < end ? 1 : -1);
	}
}

void crSequence::setDuration(float speed, int nreps)
{
	//if(nreps>0) nreps *= getNumChildren();
	//CRCore::notify(CRCore::ALWAYS)<<"crSequence::setDuration speed = "<<speed<<" nreps = "<<nreps<< std::endl;
	m_fSpeed = (speed <= 0.0f ? 0.0f : speed);
	m_nNreps = (nreps < 0 ? -1 : nreps);
	m_nNrepsremain = m_nNreps;
}

void crSequence::setMode(SequenceMode mode)
{
	switch (mode) 
	{
	case START:
		// restarts sequence in 'traverse'
		//setValue(-1);
		{
			unsigned int nch = m_frameCount;
			int begin = (m_nBegin < 0 ? nch + m_nBegin : m_nBegin);
			int end = (m_nEnd < 0 ? nch + m_nEnd : m_nEnd);
			setValue(begin);
			m_nStep = (begin < end ? 1 : -1);
			m_mode = mode;
			m_nNrepsremain = m_nNreps;
			setVisiable(true);
		}
		break;
	case STOP:
		if(m_vanishWhenStop) setVisiable(false);
		m_mode = mode;
		break;
	case PAUSE:
		if (m_mode == START)
			m_mode = PAUSE;
		break;
	case RESUME:
		if (m_mode == PAUSE)
			m_mode = START;
		break;
	}
}

void crSequence::setVanishWhenStop(bool vanish)
{
    m_vanishWhenStop = vanish;
}

bool crSequence::getVanishWhenStop()
{
	return m_vanishWhenStop;
}
void crSequence::update(/*const crFrameStamp* framestamp*/)
{
    if(m_mode == START && m_nNrepsremain)
	{
		double t = crFrameStamp::getInstance()->getReferenceTime();
		if (m_fLast == -1.0)
			m_fLast = t;

		// first and last frame of interval
		unsigned int nch = m_frameCount;
		int begin = (m_nBegin < 0 ? nch + m_nBegin : m_nBegin);
		int end = (m_nEnd < 0 ? nch + m_nEnd : m_nEnd);

		int sw = getValue();
		if (sw<0)
		{
			sw = begin;
			m_nStep = (begin < end ? 1 : -1);
		}

		// default timeout for unset values
		if (sw >= (int) m_vecFrameTime.size())
		{
			setTime(sw, 1.0f);
		}

		// frame time-out?
		float dur = m_vecFrameTime[sw] * m_fSpeed;
		if ((t - m_fLast) > dur) 
		{
			sw += m_nStep;

			// check interval
			int ibegin = (begin < end ? begin : end);
			int iend = (end > begin ? end : begin);
			//cerr << this << " interval " << ibegin << "," << iend << endl;

			if (sw < ibegin || sw > iend) 
			{
				// stop at last frame
				if (sw < ibegin)
					sw = ibegin;
				else
					sw = iend;

				// repeat counter
				if (crDisplaySettings::instance()->getRunMode() != 0 && m_nNrepsremain > 0)
					m_nNrepsremain--;

				if (m_nNrepsremain == 0)
				{
					// stop
					setMode(STOP);
				}
				else 
				{
					// wrap around
					switch (m_loopMode) 
					{
					case LOOP:
						//cerr << this << " loop" << endl;
						sw = begin;
						break;
					case SWING:
						//cerr << this << " swing" << endl;
						m_nStep = -m_nStep;
						break;
					}
				}
			}

			m_fLast = t;
		}
		setValue(sw);
	}
}
void crSequence::traverse(crNodeVisitor& nv)
{
	CRCore::ScopedLock<crMutex> lock(m_effectMutex);
	// if app traversal update the frame count.
	if (nv.getVisitorType()==crNodeVisitor::UPDATE_VISITOR)
	{
		update(/*nv.getFrameStamp()*/);
	}
	// now do the traversal
	if (nv.getTraversalMode()==crNodeVisitor::TRAVERSE_ACTIVE_CHILDREN)
	{
		int i = getValue();
		if (i>=0 && i<(int)m_childArray.size()) 
			m_childArray[i]->accept(nv);
	}
	else
	{
		crGroup::traverse(nv);
	}
}

void crSequence::setEffectPosition(const crVector3 &position)
{
	crBillboard *child;
	crBillboard::PositionList::iterator bpItr;
	for( NodeArray::iterator itr = m_childArray.begin();
		 itr != m_childArray.end();
		 ++itr )
	{
		 child = dynamic_cast<crBillboard *>(itr->get());
		 if(child) 
		 {
			 crBillboard::PositionList& pl = child->getPositionList();
             for( bpItr = pl.begin(); 
				  bpItr != pl.end();
				  ++bpItr )
			 {
                 *bpItr = position;
			 }

			 child->dirtyBound();//billboard 计算Bound有问题
		 }
	}
	//dirtyBound();
}
void crSequence::lockEffect()
{
	m_effectMutex.lock();
}
void crSequence::unlockEffect()
{
    m_effectMutex.unlock();
}
void crSequence::addParam(int i, const std::string &str)
{
	switch(i) 
	{
	case 0:
		if(str.compare("LOOP") == 0)
			m_loopMode = LOOP;
		else if(str.compare("SWING") == 0)
			m_loopMode = SWING;
		break;
	case 1:
		m_nBegin = atoi(str.c_str());
		break;
	case 2:
		m_nEnd = atoi(str.c_str());
		break;
	case 3:
		m_frameCount = atoi(str.c_str());
		setInterval(m_loopMode,m_nBegin,m_nEnd,m_frameCount);
		break;
	case 4:
		{
			float speed = atof(str.c_str());
			m_fSpeed = speed==0.0f?0.0f:1.0f/speed;
		}
		break;
	case 5:
		m_nNreps = atoi(str.c_str());
		setDuration(m_fSpeed,m_nNreps);
		break;
	case 6:
		if(str.compare("START") == 0)
			m_mode = START;
		else if(str.compare("STOP") == 0)
			m_mode = STOP;
		else if(str.compare("PAUSE") == 0)
			m_mode = PAUSE;
		else if(str.compare("RESUME") == 0)
			m_mode = RESUME;
		setMode(m_mode);
		break;
	case 7:
		m_vanishWhenStop = atoi(str.c_str());
		break;
	}
	if(i>7)
	{
        m_vecFrameTime.push_back(atof(str.c_str()));
	}
}
void crSequence::getInterval(LoopMode& mode, int& begin, int& end, int& frameCount) const
{
	mode = m_loopMode;
	begin = m_nBegin;
	end = m_nEnd;
	frameCount = m_frameCount;
}
//////////////////////////////////////////
//
//crMatrixSequence
//
///////////////////////////////////////////
crMatrixSequence::crMatrixSequence():
m_inverseDirty(false)
{
	m_referenceFrame = RELATIVE_TO_PARENTS;
	m_dataVariance = DYNAMIC;
}

crMatrixSequence::crMatrixSequence(const crMatrixSequence& sequence,const crCopyOp& copyop):
crSequence(sequence,copyop),
m_matrix(sequence.m_matrix),
m_inverse(sequence.m_inverse),
m_inverseDirty(sequence.m_inverseDirty),
m_referenceFrame(sequence.m_referenceFrame)
{    
}

crMatrixSequence::~crMatrixSequence()
{
}

void crMatrixSequence::setReferenceFrame(ReferenceFrame rf)
{
	if (m_referenceFrame == rf) return;

	m_referenceFrame = rf;

	// switch off culling if crTransform is absolute.
	if (m_referenceFrame==RELATIVE_TO_ABSOLUTE) setCullingActive(false);
	else setCullingActive(true);
}

bool crMatrixSequence::computeLocalToWorldMatrix(crMatrix& matrix,crNodeVisitor*) const
{
	if (m_referenceFrame==RELATIVE_TO_PARENTS)
	{
		matrix.preMult(m_matrix);
	}
	else // absolute
	{
		matrix = m_matrix;
	}
	return true;
}

bool crMatrixSequence::computeWorldToLocalMatrix(crMatrix& matrix,crNodeVisitor*) const
{
	const crMatrix& inverse = getInverseMatrix();

	if (m_referenceFrame==RELATIVE_TO_PARENTS)
	{
		matrix.postMult(inverse);
	}
	else // absolute
	{
		matrix = inverse;
	}
	return true;
}

bool crMatrixSequence::computeBound() const
{
	if (!crGroup::computeBound()) return false;

	// note, NULL pointer for NodeVisitor, so compute's need
	// to handle this case gracefully, normally this should not be a problem.
	crMatrix l2w;

	computeLocalToWorldMatrix(l2w,NULL);

	m_bbox.m_max = m_bbox.m_max * l2w;
	m_bbox.m_min = m_bbox.m_min * l2w;

	m_bbox.correct();

	crVector3 xdash = m_boundSphere.m_center;
	xdash.x() += m_boundSphere.m_fRadius;
	xdash = xdash*l2w;

	crVector3 ydash = m_boundSphere.m_center;
	ydash.y() += m_boundSphere.m_fRadius;
	ydash = ydash*l2w;

	crVector3 zdash = m_boundSphere.m_center;
	zdash.z() += m_boundSphere.m_fRadius;
	zdash = zdash*l2w;

	m_boundSphere.m_center = m_boundSphere.m_center*l2w;

	xdash -= m_boundSphere.m_center;
	float len_xdash = xdash.length();

	ydash -= m_boundSphere.m_center;
	float len_ydash = ydash.length();

	zdash -= m_boundSphere.m_center;
	float len_zdash = zdash.length();

	m_boundSphere.m_fRadius = len_xdash;
	if (m_boundSphere.m_fRadius<len_ydash) m_boundSphere.m_fRadius = len_ydash;
	if (m_boundSphere.m_fRadius<len_zdash) m_boundSphere.m_fRadius = len_zdash;

	return true;

}
