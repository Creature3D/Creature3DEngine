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
#include <CRCore/crDOFTransform.h>
#include <CRCore/crArgumentParser.h>
#include <CRCore/crNotify.h>

using namespace CRCore;

//static const unsigned int XTran  = 0x80000000u >> 0;
//static const unsigned int YTran  = 0x80000000u >> 1;
//static const unsigned int ZTran  = 0x80000000u >> 2;
//static const unsigned int YRot = 0x80000000u >> 3;
//static const unsigned int ZRot  = 0x80000000u >> 4;
//static const unsigned int XRot   = 0x80000000u >> 5;
//static const unsigned int XScale        = 0x80000000u >> 6;
//static const unsigned int YScale        = 0x80000000u >> 7;
//static const unsigned int ZScale        = 0x80000000u >> 8;


crDOFTransform::crDOFTransform():
    m_previousTraversalNumber(-1),
    //m_previousTime(0.0),
    m_limitationFlags(ALL), 
    m_animationOn(true), 
    m_increasingFlags(ALL),
    m_multOrder(PRH),
	m_surgeFlags(0),
	//m_endStop(false),
	m_endless(true),
	m_lifeTime(0.0),
	m_startTime(0.0),
	m_currentTime(0.0),
	m_resetTime(0.0),
	m_dt(0.0f)
{
	setCurrentScale(crVector3(1.0f,1.0f,1.0f));
	m_xAxis.set(1.0f,0.0f,0.0f);
	m_xyPlane.set(0.0f,1.0f,0.0f);
    setNumChildrenRequiringUpdateTraversal(1);
}

crDOFTransform::crDOFTransform(const crDOFTransform& dof, const CRCore::crCopyOp& copyop):
    CRCore::crTransform(dof, copyop),
    m_previousTraversalNumber(dof.m_previousTraversalNumber),
    //m_previousTime(dof.m_previousTime),
    m_minHPR(dof.m_minHPR),
    m_maxHPR(dof.m_maxHPR),
    m_currentHPR(dof.m_currentHPR),
    m_incrementHPR(dof.m_incrementHPR),
    m_minTranslate(dof.m_minTranslate),
    m_maxTranslate(dof.m_maxTranslate),
    m_currentTranslate(dof.m_currentTranslate),
    m_incrementTranslate(dof.m_incrementTranslate),
    m_minScale(dof.m_minScale),
    m_maxScale(dof.m_maxScale),
    m_currentScale(dof.m_currentScale),
    m_incrementScale(dof.m_incrementScale),
    m_Put(dof.m_Put),
    m_inversePut(dof.m_inversePut),
    m_limitationFlags(dof.m_limitationFlags),
    m_animationOn(dof.m_animationOn),
    m_increasingFlags(dof.m_increasingFlags),
    m_multOrder(dof.m_multOrder),
	m_surgeFlags(dof.m_surgeFlags),
	m_endStopHPR(dof.m_endStopHPR),
	m_endStopTrans(dof.m_endStopTrans),
	m_endStopScale(dof.m_endStopScale),
	m_endless(dof.m_endless),
	m_lifeTime(dof.m_lifeTime),
	m_startTime(dof.m_startTime),
	m_currentTime(dof.m_currentTime),
	m_resetTime(dof.m_resetTime),
	m_O(dof.m_O),
	m_xAxis(dof.m_xAxis),
	m_xyPlane(dof.m_xyPlane),
	m_dt(0.0f)
{
    setNumChildrenRequiringUpdateTraversal(getNumChildrenRequiringUpdateTraversal()+1);            
}

void crDOFTransform::traverse(CRCore::crNodeVisitor& nv)
{
    if (nv.getVisitorType()==CRCore::crNodeVisitor::UPDATE_VISITOR)
    {
        // ensure that we do not operate on this node more than
        // once during this traversal.  This is an issue since node
        // can be shared between multiple parents.
        if ((nv.getTraversalNumber()!=m_previousTraversalNumber)/* && nv.getFrameStamp()*/)
        {
            //double newTime = nv.getFrameStamp()->getReferenceTime();

            //animate((float)(newTime-m_previousTime));
			animate(crFrameStamp::getInstance()->getFrameInterval());
            m_previousTraversalNumber = nv.getTraversalNumber();
           // m_previousTime = newTime; 
        }
    }

    crTransform::traverse(nv);
}

//void crDOFTransform::animate(CRCore::crNodeVisitor& nv)
//{
//	if (nv.getVisitorType()==CRCore::crNodeVisitor::UPDATE_VISITOR||nv.getVisitorType()==CRCore::crNodeVisitor::CULL_VISITOR)
//	{
//		if ((nv.getTraversalNumber()!=m_previousTraversalNumber) && nv.getFrameStamp())
//		{
//			//double newTime = nv.getFrameStamp()->getReferenceTime();
//
//			//animate((float)(newTime-m_previousTime));
//			animate(nv.getFrameStamp()->getFrameInterval());
//
//			m_previousTraversalNumber = nv.getTraversalNumber();
//			//m_previousTime = newTime; 
//		}
//	}
//}

bool crDOFTransform::computeLocalToWorldMatrix(CRCore::crMatrix& matrix,CRCore::crNodeVisitor*) const
{
    //put the PUT matrix first:
    CRCore::crMatrix l2w(getPutMatrix());

    //now the current matrix:
    CRCore::crMatrix current; 
    current.makeTranslate(getCurrentTranslate());

    //now create the local rotation:
    if(m_multOrder == PRH)
    {
        current.preMult(CRCore::crMatrix::rotate(getCurrentHPR()[1], 1.0, 0.0, 0.0));//pitch
        current.preMult(CRCore::crMatrix::rotate(getCurrentHPR()[2], 0.0, 1.0, 0.0));//roll
        current.preMult(CRCore::crMatrix::rotate(getCurrentHPR()[0], 0.0, 0.0, 1.0));//heading
    }
    else if(m_multOrder == PHR)
    {
        current.preMult(CRCore::crMatrix::rotate(getCurrentHPR()[1], 1.0, 0.0, 0.0));//pitch
        current.preMult(CRCore::crMatrix::rotate(getCurrentHPR()[0], 0.0, 0.0, 1.0));//heading
        current.preMult(CRCore::crMatrix::rotate(getCurrentHPR()[2], 0.0, 1.0, 0.0));//roll
    }
    else if(m_multOrder == HPR)
    {
        current.preMult(CRCore::crMatrix::rotate(getCurrentHPR()[0], 0.0, 0.0, 1.0));//heading
        current.preMult(CRCore::crMatrix::rotate(getCurrentHPR()[1], 1.0, 0.0, 0.0));//pitch
        current.preMult(CRCore::crMatrix::rotate(getCurrentHPR()[2], 0.0, 1.0, 0.0));//roll
    }
    else if(m_multOrder == HRP)
    {
        current.preMult(CRCore::crMatrix::rotate(getCurrentHPR()[0], 0.0, 0.0, 1.0));//heading
        current.preMult(CRCore::crMatrix::rotate(getCurrentHPR()[2], 0.0, 1.0, 0.0));//roll
        current.preMult(CRCore::crMatrix::rotate(getCurrentHPR()[1], 1.0, 0.0, 0.0));//pitch
    }
    else if(m_multOrder == RHP)
    {
        current.preMult(CRCore::crMatrix::rotate(getCurrentHPR()[2], 0.0, 1.0, 0.0));//roll
        current.preMult(CRCore::crMatrix::rotate(getCurrentHPR()[0], 0.0, 0.0, 1.0));//heading
        current.preMult(CRCore::crMatrix::rotate(getCurrentHPR()[1], 1.0, 0.0, 0.0));//pitch
    }
    else // m_multOrder == RPH
    {
        current.preMult(CRCore::crMatrix::rotate(getCurrentHPR()[2], 0.0, 1.0, 0.0));//roll
        current.preMult(CRCore::crMatrix::rotate(getCurrentHPR()[1], 1.0, 0.0, 0.0));//pitch
        current.preMult(CRCore::crMatrix::rotate(getCurrentHPR()[0], 0.0, 0.0, 1.0));//heading
    }

    //and scale:
    current.preMult(CRCore::crMatrix::scale(getCurrentScale()));

    l2w.postMult(current);

    //and impose inverse put:
    l2w.postMult(getInversePutMatrix());

    // finally.
    if (m_referenceFrame==RELATIVE_TO_PARENTS)
    {
        matrix.preMult(l2w);
    }
    else
    {
        matrix = l2w;    
    }

    return true;
}


bool crDOFTransform::computeWorldToLocalMatrix(CRCore::crMatrix& matrix,CRCore::crNodeVisitor*) const
{
    //put the PUT matrix first:
    CRCore::crMatrix w2l(getInversePutMatrix());

    //now the current matrix:
    CRCore::crMatrix current; 
    current.makeTranslate(-getCurrentTranslate());

    //now create the local rotation:
    if(m_multOrder == PRH)
    {
        current.postMult(CRCore::crMatrix::rotate(-getCurrentHPR()[0], 0.0, 0.0, 1.0));//heading
        current.postMult(CRCore::crMatrix::rotate(-getCurrentHPR()[2], 0.0, 1.0, 0.0));//roll
        current.postMult(CRCore::crMatrix::rotate(-getCurrentHPR()[1], 1.0, 0.0, 0.0));//pitch
    }
    else if(m_multOrder == PHR)
    {
        current.postMult(CRCore::crMatrix::rotate(-getCurrentHPR()[2], 0.0, 1.0, 0.0));//roll
        current.postMult(CRCore::crMatrix::rotate(-getCurrentHPR()[0], 0.0, 0.0, 1.0));//heading
        current.postMult(CRCore::crMatrix::rotate(-getCurrentHPR()[1], 1.0, 0.0, 0.0));//pitch
    }
    else if(m_multOrder == HPR)
    {
        current.postMult(CRCore::crMatrix::rotate(-getCurrentHPR()[2], 0.0, 1.0, 0.0));//roll
        current.postMult(CRCore::crMatrix::rotate(-getCurrentHPR()[1], 1.0, 0.0, 0.0));//pitch
        current.postMult(CRCore::crMatrix::rotate(-getCurrentHPR()[0], 0.0, 0.0, 1.0));//heading
    }
    else if(m_multOrder == HRP)
    {
        current.postMult(CRCore::crMatrix::rotate(-getCurrentHPR()[1], 1.0, 0.0, 0.0));//pitch
        current.postMult(CRCore::crMatrix::rotate(-getCurrentHPR()[2], 0.0, 1.0, 0.0));//roll
        current.postMult(CRCore::crMatrix::rotate(-getCurrentHPR()[0], 0.0, 0.0, 1.0));//heading
    }
    else if(m_multOrder == RHP)
    {
        current.postMult(CRCore::crMatrix::rotate(-getCurrentHPR()[1], 1.0, 0.0, 0.0));//pitch
        current.postMult(CRCore::crMatrix::rotate(-getCurrentHPR()[0], 0.0, 0.0, 1.0));//heading
        current.postMult(CRCore::crMatrix::rotate(-getCurrentHPR()[2], 0.0, 1.0, 0.0));//roll
    }
    else // m_multOrder == MultOrder::RPH
    {
        current.postMult(CRCore::crMatrix::rotate(-getCurrentHPR()[0], 0.0, 0.0, 1.0));//heading
        current.postMult(CRCore::crMatrix::rotate(-getCurrentHPR()[1], 1.0, 0.0, 0.0));//pitch
        current.postMult(CRCore::crMatrix::rotate(-getCurrentHPR()[2], 0.0, 1.0, 0.0));//roll
    }

    //and scale:
    current.postMult(CRCore::crMatrix::scale(1./getCurrentScale()[0], 1./getCurrentScale()[1], 1./getCurrentScale()[2]));

    w2l.postMult(current);

    //and impose inverse put:
    w2l.postMult(getPutMatrix());

    if (m_referenceFrame==RELATIVE_TO_PARENTS)
    {
        //finally:
        matrix.postMult(w2l);
    }
    else // absolute
    {
        matrix = w2l;
    }
    return true;
}


void crDOFTransform::updateCurrentHPR(const CRCore::crVector3& hpr)
{
    //if there is constrain on animation
    if (m_limitationFlags & XRot)
    {
        //if we have min == max, it is efective constrain, so don't change 
        if(m_minHPR[2] != m_maxHPR[2])
        {
            m_currentHPR[2] = hpr[2];
			if(m_endStopHPR[2])
			{
				if(m_currentHPR[2] < m_minHPR[2])
				{
					m_currentHPR[2] = m_minHPR[2];
					//m_animationOn = false;
					m_incrementHPR[2] = 0.0f;
				}
				else if(m_currentHPR[2] > m_maxHPR[2])
				{
					m_currentHPR[2] = m_maxHPR[2];
					//m_animationOn = false;
					m_incrementHPR[2] = 0.0f;
				}
			}
			else
			{
				unsigned short this_flag = XRot;//roll

				if(m_surgeFlags & this_flag)
				{
					if(m_currentHPR[2] < m_minHPR[2]) 
					{
						m_currentHPR[2] = m_minHPR[2];
						m_increasingFlags |= this_flag;
					}
					else if(m_currentHPR[2] > m_maxHPR[2]) 
					{
						m_currentHPR[2] = m_maxHPR[2];
						m_increasingFlags &= ~this_flag;
					}
				}
				else
				{
					m_increasingFlags |= this_flag;
					if(m_currentHPR[2] < m_minHPR[2])
					{
						m_currentHPR[2] = m_maxHPR[2] + (m_currentHPR[2] - m_minHPR[2]);
					}
					else if(m_currentHPR[2] > m_maxHPR[2]) 
					{
						m_currentHPR[2] = m_minHPR[2] + (m_currentHPR[2] - m_maxHPR[2]);
					}
				}
			}
        }
    }
    else
    {
        m_currentHPR[2] = hpr[2];
    }

    if (m_limitationFlags & YRot)
    {
        if(m_minHPR[1] != m_maxHPR[1])
        {
            m_currentHPR[1] = hpr[1];
			if(m_endStopHPR[1])
			{
				if(m_currentHPR[1] < m_minHPR[1])
				{
					m_currentHPR[1] = m_minHPR[1];
					//m_animationOn = false;
					m_incrementHPR[1] = 0.0f;
				}
				else if(m_currentHPR[1] > m_maxHPR[1])
				{
					m_currentHPR[1] = m_maxHPR[1];
					//m_animationOn = false;
					m_incrementHPR[1] = 0.0f;
				}
			}
			else
			{
                unsigned short this_flag = YRot;//pitch
				if(m_surgeFlags & this_flag)
				{

					if(m_currentHPR[1] < m_minHPR[1])
					{
						m_currentHPR[1] = m_minHPR[1];
						m_increasingFlags |= this_flag;
					}
					else if(m_currentHPR[1] > m_maxHPR[1])
					{
						m_currentHPR[1] = m_maxHPR[1];
						m_increasingFlags &= ~this_flag;
					}
				}
				else
				{
					m_increasingFlags |= this_flag;
					if(m_currentHPR[1] < m_minHPR[1])
					{
						m_currentHPR[1] = m_maxHPR[1] + (m_currentHPR[1] - m_minHPR[1]);
					}
					else if(m_currentHPR[1] > m_maxHPR[1]) 
					{
						m_currentHPR[1] = m_minHPR[1] + (m_currentHPR[1] - m_maxHPR[1]);
					}
				}
			}
        }
    }
    else
    {
        m_currentHPR[1] = hpr[1];
    }

    if (m_limitationFlags & ZRot)
    {
        if(m_minHPR[0] != m_maxHPR[0])
        {
            m_currentHPR[0] = hpr[0];
			if(m_endStopHPR[0])
			{
				if(m_currentHPR[0] < m_minHPR[0])
				{
					m_currentHPR[0] = m_minHPR[0];
					//m_animationOn = false;
					m_incrementHPR[0] = 0.0f;
				}
				else if(m_currentHPR[0] > m_maxHPR[0])
				{
					m_currentHPR[0] = m_maxHPR[0];
					//m_animationOn = false;
					m_incrementHPR[0] = 0.0f;
				}
			}
			else
			{
				unsigned short this_flag = ZRot;//heading

				if(m_surgeFlags & this_flag)
				{
					if(m_currentHPR[0] < m_minHPR[0]) 
					{
						m_currentHPR[0] = m_minHPR[0];
						m_increasingFlags |= this_flag;
					}
					else if(m_currentHPR[0] > m_maxHPR[0]) 
					{
						m_currentHPR[0] = m_maxHPR[0];
						m_increasingFlags &= ~this_flag;
					}
				}
				else
				{
					m_increasingFlags |= this_flag;
					if(m_currentHPR[0] < m_minHPR[0])
					{
						m_currentHPR[0] = m_maxHPR[0] + (m_currentHPR[0] - m_minHPR[0]);
					}
					else if(m_currentHPR[0] > m_maxHPR[0]) 
					{
						m_currentHPR[0] = m_minHPR[0] + (m_currentHPR[0] - m_maxHPR[0]);
					}
				}
			}
        }
    }
    else
    {
        m_currentHPR[0] = hpr[0];
    }

    dirtyBound();
}


void crDOFTransform::updateCurrentTranslate(const CRCore::crVector3& translate)
{
    if (m_limitationFlags & ZTran)
    {
        if(m_minTranslate[2] != m_maxTranslate[2])
        {
            m_currentTranslate[2] = translate[2];
			if(m_endStopTrans[2])
			{
				if(m_currentTranslate[2] < m_minTranslate[2])
				{
					m_currentTranslate[2] = m_minTranslate[2];
					//m_animationOn = false;
					m_incrementTranslate[2] = 0.0f;
				}
				else if(m_currentTranslate[2] > m_maxTranslate[2])
				{
					m_currentTranslate[2] = m_maxTranslate[2];
					//m_animationOn = false;
					m_incrementTranslate[2] = 0.0f;
				}
			}
			else
			{
				unsigned short this_flag = ZTran;

				if(m_surgeFlags & this_flag)
				{
					if(m_currentTranslate[2] < m_minTranslate[2])
					{
						m_currentTranslate[2] = m_minTranslate[2];
						m_increasingFlags |= this_flag;
					}
					else if(m_currentTranslate[2] > m_maxTranslate[2]) 
					{
						m_currentTranslate[2] = m_maxTranslate[2];
						m_increasingFlags &= ~this_flag;
					}
				}
				else
				{
					m_increasingFlags |= this_flag;

					if(m_currentTranslate[2] < m_minTranslate[2])
					{
						m_currentTranslate[2] = m_maxTranslate[2] + (m_currentTranslate[2] - m_minTranslate[2]);
					}
					else if(m_currentTranslate[2] > m_maxTranslate[2]) 
					{
						m_currentTranslate[2] = m_minTranslate[2] + (m_currentTranslate[2] - m_maxTranslate[2]);
					}
				}
			}
        }
    }
    else
    {
        m_currentTranslate[2] = translate[2];
    }

    if (m_limitationFlags & YTran)
    {
        if(m_minTranslate[1] != m_maxTranslate[1])
        {
            m_currentTranslate[1] = translate[1];
			if(m_endStopTrans[1])
			{
				if(m_currentTranslate[1] < m_minTranslate[1])
				{
					m_currentTranslate[1] = m_minTranslate[1];
					//m_animationOn = false;
					m_incrementTranslate[1] = 0.0f;
				}
				else if(m_currentTranslate[1] > m_maxTranslate[1])
				{
					m_currentTranslate[1] = m_maxTranslate[1];
					//m_animationOn = false;
					m_incrementTranslate[1] = 0.0f;
				}
			}
			else
			{
				unsigned short this_flag = YTran;

				if(m_surgeFlags & this_flag)
				{
					if(m_currentTranslate[1] < m_minTranslate[1])
					{
						m_currentTranslate[1] = m_minTranslate[1];
						m_increasingFlags |= this_flag;
					}
					else if(m_currentTranslate[1] > m_maxTranslate[1]) 
					{
						m_currentTranslate[1] = m_maxTranslate[1];
						m_increasingFlags &= ~this_flag;
					}
				}
				else
				{
					m_increasingFlags |= this_flag;

					if(m_currentTranslate[1] < m_minTranslate[1])
					{
						m_currentTranslate[1] = m_maxTranslate[1] + (m_currentTranslate[1] - m_minTranslate[1]);
					}
					else if(m_currentTranslate[1] > m_maxTranslate[1]) 
					{
						m_currentTranslate[1] = m_minTranslate[1] + (m_currentTranslate[1] - m_maxTranslate[1]);
					}
				}
			}
        }
    }
    else
    {
        m_currentTranslate[1] = translate[1];
    }

    if (m_limitationFlags & XTran)
    {
        if(m_minTranslate[0] != m_maxTranslate[0])
        {
            m_currentTranslate[0] = translate[0];
			if(m_endStopTrans[0])
			{
				if(m_currentTranslate[0] < m_minTranslate[0])
				{
					m_currentTranslate[0] = m_minTranslate[0];
					//m_animationOn = false;
					m_incrementTranslate[0] = 0.0f;
				}
				else if(m_currentTranslate[0] > m_maxTranslate[0])
				{
					m_currentTranslate[0] = m_maxTranslate[0];
					//m_animationOn = false;
					m_incrementTranslate[0] = 0.0f;
				}
			}
			else
			{
				unsigned short this_flag = XTran;

				if(m_surgeFlags & this_flag)
				{
					if(m_currentTranslate[0] < m_minTranslate[0]) 
					{
						m_currentTranslate[0] = m_minTranslate[0];
						m_increasingFlags |= this_flag;
					}
					else if(m_currentTranslate[0] > m_maxTranslate[0]) 
					{
						m_currentTranslate[0] = m_maxTranslate[0];
						m_increasingFlags &= ~this_flag;
					}
				}
				else
				{
					m_increasingFlags |= this_flag;

					if(m_currentTranslate[0] < m_minTranslate[0])
					{
						m_currentTranslate[0] = m_maxTranslate[0] + (m_currentTranslate[0] - m_minTranslate[0]);
					}
					else if(m_currentTranslate[0] > m_maxTranslate[0]) 
					{
						m_currentTranslate[0] = m_minTranslate[0] + (m_currentTranslate[0] - m_maxTranslate[0]);
					}
				}
			}
        }
    }    
    else
    {
        m_currentTranslate[0] = translate[0];
    }

    dirtyBound();
}


void crDOFTransform::updateCurrentScale(const CRCore::crVector3& scale)
{
    if (m_limitationFlags & ZScale)
    {
        if(m_minScale[2] != m_maxScale[2])
        {            
            m_currentScale[2] = scale[2];
			if(m_endStopScale[2])
			{
				if(m_currentScale[2] < m_minScale[2])
				{
					m_currentScale[2] = m_minScale[2];
					//m_animationOn = false;
					m_incrementScale[2] = 0.0f;
				}
				else if(m_currentScale[2] > m_maxScale[2])
				{
					m_currentScale[2] = m_maxScale[2];
					//m_animationOn = false;
					m_incrementScale[2] = 0.0f;
				}
			}
			else
			{
				unsigned short this_flag = ZScale;

				if(m_surgeFlags & this_flag)
				{
					if(m_currentScale[2] < m_minScale[2]) 
					{
						m_currentScale[2] = m_minScale[2];
						m_increasingFlags |= this_flag;
					}
					else if(m_currentScale[2] > m_maxScale[2]) 
					{
						m_currentScale[2] = m_maxScale[2];
						m_increasingFlags &= ~this_flag;
					}
				}
				else
				{
					m_increasingFlags |= this_flag;

					if(m_currentScale[2] < m_minScale[2]) 
					{
						m_currentScale[2] = m_maxScale[2] + (m_currentScale[2] - m_minScale[2]);
					}
					else if(m_currentScale[2] > m_maxScale[2]) 
					{
						m_currentScale[2] = m_minScale[2] + (m_currentScale[2] - m_maxScale[2]);
					}
				}
			}
        }
    }
    else
    {
        m_currentScale[2] = scale[2];
    }

    if (m_limitationFlags & YScale)
    {
        if(m_minScale[1] != m_maxScale[1])
        {
            m_currentScale[1] = scale[1];
			if(m_endStopScale[1])
			{
				if(m_currentScale[1] < m_minScale[1])
				{
					m_currentScale[1] = m_minScale[1];
					//m_animationOn = false;
					m_incrementScale[1] = 0.0f;
				}
				else if(m_currentScale[1] > m_maxScale[1])
				{
					m_currentScale[1] = m_maxScale[1];
					//m_animationOn = false;
					m_incrementScale[1] = 0.0f;
				}
			}
			else
			{
				unsigned short this_flag = YScale;

				if(m_surgeFlags & this_flag)
				{
					if(m_currentScale[1] < m_minScale[1]) 
					{
						m_currentScale[1] = m_minScale[1];
						m_increasingFlags |= this_flag;
					}
					else if(m_currentScale[1] > m_maxScale[1]) 
					{
						m_currentScale[1] = m_maxScale[1];
						m_increasingFlags &= ~this_flag;
					}
				}
				else
				{
					m_increasingFlags |= this_flag;

					if(m_currentScale[1] < m_minScale[1]) 
					{
						m_currentScale[1] = m_maxScale[1] + (m_currentScale[1] - m_minScale[1]);
					}
					else if(m_currentScale[1] > m_maxScale[1]) 
					{
						m_currentScale[1] = m_minScale[1] + (m_currentScale[1] - m_maxScale[1]);
					}
				}
			}
        }
    }
    else
    {
        m_currentScale[1] = scale[1];
    }

    if (m_limitationFlags & XScale)
    {
        if(m_minScale[0] != m_maxScale[0])
        {
            m_currentScale[0] = scale[0];
			if(m_endStopScale[0])
			{
				if(m_currentScale[0] < m_minScale[0])
				{
					m_currentScale[0] = m_minScale[0];
					//m_animationOn = false;
					m_incrementScale[0] = 0.0f;
				}
				else if(m_currentScale[0] > m_maxScale[0])
				{
					m_currentScale[0] = m_maxScale[0];
					//m_animationOn = false;
					m_incrementScale[0] = 0.0f;
				}
			}
			else
			{
				unsigned short this_flag = XScale;

				if(m_surgeFlags & this_flag)
				{
					if(m_currentScale[0] < m_minScale[0]) 
					{
						m_currentScale[0] = m_minScale[0];
						m_increasingFlags |= this_flag;
					}
					else if(m_currentScale[0] > m_maxScale[0]) 
					{
						m_currentScale[0] = m_maxScale[0];
						m_increasingFlags &= ~this_flag;
					}
				}
				else
				{
					m_increasingFlags |= this_flag;

					if(m_currentScale[0] < m_minScale[0]) 
					{
						m_currentScale[0] = m_maxScale[0] + (m_currentScale[0] - m_minScale[0]);
					}
					else if(m_currentScale[0] > m_maxScale[0]) 
					{
						m_currentScale[0] = m_minScale[0] + (m_currentScale[0] - m_maxScale[0]);
					}
				}
			}
        }
    }
    else
    {
        m_currentScale[0] = scale[0];
    }

    dirtyBound();
}

void crDOFTransform::animate(float deltaTime)
{
	m_dt += deltaTime;
	if(m_dt<0.04f)
	{
		return;
	}
	deltaTime = m_dt;
	m_dt = 0.0f;

	if ((m_currentTime >= m_resetTime) && (m_resetTime > 0))
	{
		m_currentTime = 0;
	}
	bool alive = false;
	if (m_currentTime >= m_startTime)
	{
		if (m_endless || (m_currentTime < (m_startTime + m_lifeTime)))
			alive = true;
	}
	m_currentTime += deltaTime;
	if (alive && m_animationOn)
	{
		//CRCore::notify(CRCore::ALWAYS)<<"crDOFTransform::animate deltaTime = "<<deltaTime<<std::endl;

		CRCore::crVector3 new_value = m_currentTranslate;

		if(m_increasingFlags & XTran)
			new_value[0] += m_incrementTranslate[0]*deltaTime;
		else
			new_value[0] -= m_incrementTranslate[0]*deltaTime;

		if(m_increasingFlags & YTran)
			new_value[1] += m_incrementTranslate[1]*deltaTime;
		else
			new_value[1] -= m_incrementTranslate[1]*deltaTime;

		if(m_increasingFlags & ZTran)
			new_value[2] += m_incrementTranslate[2]*deltaTime;
		else
			new_value[2] -= m_incrementTranslate[2]*deltaTime;

		updateCurrentTranslate(new_value);

		new_value = m_currentHPR;

		if(m_increasingFlags & (ZRot))
			new_value[0] += m_incrementHPR[0]*deltaTime;
		else
			new_value[0] -= m_incrementHPR[0]*deltaTime;

		if(m_increasingFlags & (YRot))
			new_value[1] += m_incrementHPR[1]*deltaTime;
		else
			new_value[1] -= m_incrementHPR[1]*deltaTime;

		if(m_increasingFlags & (XRot))
			new_value[2] += m_incrementHPR[2]*deltaTime;
		else
			new_value[2] -= m_incrementHPR[2]*deltaTime;

		updateCurrentHPR(new_value);

		new_value = m_currentScale;

		if(m_increasingFlags & XScale)
			new_value[0] += m_incrementScale[0]*deltaTime;
		else
			new_value[0] -= m_incrementScale[0]*deltaTime;

		if(m_increasingFlags & YScale)
			new_value[1] += m_incrementScale[1]*deltaTime;
		else
			new_value[1] -= m_incrementScale[1]*deltaTime;

		if(m_increasingFlags & ZScale)
			new_value[2] += m_incrementScale[2]*deltaTime;
		else
			new_value[2] -= m_incrementScale[2]*deltaTime;

		updateCurrentScale(new_value);
	} 

	//bool needResetEndStop = false;
 //   if(!m_animationOn) 
	//{
	//	if(m_resetTime>0.0f && deltaTime > m_resetTime)
	//	{
 //           m_animationOn = true;
	//		if(m_endStop)
	//		{
	//			m_endStop = false;
	//			needResetEndStop = true;
	//		}
	//	}
	//	else return;
	//}
    //this will increment or decrement all allowed values


	//if(needResetEndStop)
	//{
	//	m_endStop = true;
	//}
}

void crDOFTransform::setSurgeFlags(unsigned short flags)
{
    m_surgeFlags = flags;
}

//void crDOFTransform::setEndStop(bool endStop) {m_endStop = endStop;}
//bool crDOFTransform::getEndStop() const {return m_endStop;}
void crDOFTransform::setEndStopHPR(const crVector3b& endStop) { m_endStopHPR = endStop; }
const CRCore::crVector3b& crDOFTransform::getEndStopHPR() const { return m_endStopHPR; }
void crDOFTransform::setEndStopTrans(const crVector3b& endStop){ m_endStopTrans = endStop; }
const CRCore::crVector3b& crDOFTransform::getEndStopTrans() const{ return m_endStopTrans; }
void crDOFTransform::setEndStopScale(const crVector3b& endStop){ m_endStopScale = endStop;}
const CRCore::crVector3b& crDOFTransform::getEndStopScale() const{ return m_endStopScale;}

void crDOFTransform::setEndless(bool endless) {m_endless = endless;}
bool crDOFTransform::getEndless() const {return m_endless;}

double crDOFTransform::getCurrentTime() const{return m_currentTime;}

void crDOFTransform::setResetTime(double resetTime){ m_resetTime = resetTime; }
double crDOFTransform::getResetTime() const{ return m_resetTime; }

void crDOFTransform::setLifeTime(double lifeTime){ m_lifeTime = lifeTime; }
double crDOFTransform::getLifeTime() const{ return m_lifeTime; }

void crDOFTransform::setStartTime(double startTime){ m_startTime = startTime; }
double crDOFTransform::getStartTime() const{ return m_startTime; }

void crDOFTransform::computePutMatrix()
{
	// compute the put matrix.
	crVector3 xAxis = m_xAxis - m_O;
	xAxis.normalize();

	crVector3 xyPlane = m_xyPlane - m_O;
	xyPlane.normalize();

	CRCore::crVector3 normalz = xAxis ^ xyPlane;
	normalz.normalize();

	// get X, Y, Z axis of the DOF in terms of global coordinates
	CRCore::crVector3 Rz = normalz;
	if (Rz == CRCore::crVector3(0,0,0)) Rz[2] = 1;
	CRCore::crVector3 Rx = xAxis;
	if (Rx == CRCore::crVector3(0,0,0)) Rx[0] = 1;
	CRCore::crVector3 Ry = Rz ^ Rx;
	// create the putmatrix
	CRCore::crMatrix inv_putmat( Rx.x(), Rx.y(), Rx.z(), 0, 
		Ry.x(), Ry.y(), Ry.z(), 0,
		Rz.x(), Rz.y(), Rz.z(), 0,
		m_O.x(), m_O.y(), m_O.z(), 1);

	setInversePutMatrix(inv_putmat);
	setPutMatrix(CRCore::crMatrix::inverse(inv_putmat));
}
void crDOFTransform::addParam(int i, const std::string &str)
{
	switch(i) 
	{
	case 0:
		crArgumentParser::appAtoVec(str,m_minHPR);
		m_minHPR.set(inDegrees(m_minHPR[0]),inDegrees(m_minHPR[1]),inDegrees(m_minHPR[2]));
		break;
	case 1:
		crArgumentParser::appAtoVec(str,m_maxHPR);
		m_maxHPR.set(inDegrees(m_maxHPR[0]),inDegrees(m_maxHPR[1]),inDegrees(m_maxHPR[2]));
		break;
	case 2:
		crArgumentParser::appAtoVec(str,m_currentHPR);
		m_currentHPR.set(inDegrees(m_currentHPR[0]),inDegrees(m_currentHPR[1]),inDegrees(m_currentHPR[2]));
		break;
	case 3:
		crArgumentParser::appAtoVec(str,m_incrementHPR);
		m_incrementHPR.set(inDegrees(m_incrementHPR[0]),inDegrees(m_incrementHPR[1]),inDegrees(m_incrementHPR[2]));
		break;
	case 4:
		{
		    crVector3i endStop;
			crArgumentParser::appAtoVec(str,endStop);
			m_endStopHPR = endStop;
		}
		break;
	case 5:
		crArgumentParser::appAtoVec(str,m_minTranslate);
		break;
	case 6:
		crArgumentParser::appAtoVec(str,m_maxTranslate);
		break;
	case 7:
		crArgumentParser::appAtoVec(str,m_currentTranslate);
		break;
	case 8:
		crArgumentParser::appAtoVec(str,m_incrementTranslate);
		break;
	case 9:
		{
			crVector3i endStop;
			crArgumentParser::appAtoVec(str,endStop);
			m_endStopTrans = endStop;
		}
		break;
	case 10:
		crArgumentParser::appAtoVec(str,m_minScale);
		break;
	case 11:
		crArgumentParser::appAtoVec(str,m_maxScale);
		break;
	case 12:
		crArgumentParser::appAtoVec(str,m_currentScale);
		break;
	case 13:
		crArgumentParser::appAtoVec(str,m_incrementScale);
		break;
	case 14:
		{
			crVector3i endStop;
			crArgumentParser::appAtoVec(str,endStop);
			m_endStopScale = endStop;
		}
		break;
	case 15:
		crArgumentParser::appAtoVec(str,m_O);
		break;
	case 16:
		crArgumentParser::appAtoVec(str,m_xAxis);
		break;
	case 17:
		crArgumentParser::appAtoVec(str,m_xyPlane);
        computePutMatrix();
		break;
	case 18:
		//m_limitationFlags = atol(str.c_str());
        m_limitationFlags = 0;
		if(str.find("ALL") !=-1)
			m_limitationFlags = ALL;
		else
		{
			if(str.find("XTran")!=-1)
				m_limitationFlags |= XTran;
			if(str.find("YTran")!=-1)
				m_limitationFlags |= YTran;
			if(str.find("ZTran")!=-1)
				m_limitationFlags |= ZTran;
			if(str.find("XRot")!=-1)
				m_limitationFlags |= XRot;
			if(str.find("YRot")!=-1)
				m_limitationFlags |= YRot;
			if(str.find("ZRot")!=-1)
				m_limitationFlags |= ZRot;
			if(str.find("XScale")!=-1)
				m_limitationFlags |= XScale;
			if(str.find("YScale")!=-1)
				m_limitationFlags |= YScale;
			if(str.find("ZScale")!=-1)
				m_limitationFlags |= ZScale;
		}
		break;
	case 19:
		m_animationOn = atoi(str.c_str());
		break;
	case 20:
	    {
			//m_surgeFlags = atoi(str.c_str());
			m_surgeFlags = 0;
			if(str.find("ALL")!=-1)
				m_surgeFlags = ALL;
			else
			{
				if(str.find("XYZTran")!=-1)
				{
					m_surgeFlags |= XTran;
					m_surgeFlags |= YTran;
					m_surgeFlags |= ZTran;
				}
				else
				{
					if(str.find("XTran")!=-1)
						m_surgeFlags |= XTran;
					if(str.find("YTran")!=-1)
						m_surgeFlags |= YTran;
					if(str.find("ZTran")!=-1)
						m_surgeFlags |= ZTran;
				}
				if(str.find("XYZRot")!=-1)
				{
					m_surgeFlags |= XRot;
					m_surgeFlags |= YRot;
					m_surgeFlags |= ZRot;
				}
				else
				{
					if(str.find("XRot")!=-1)
						m_surgeFlags |= XRot;
					if(str.find("YRot")!=-1)
						m_surgeFlags |= YRot;
					if(str.find("ZRot")!=-1)
						m_surgeFlags |= ZRot;
				}
				if(str.find("XYZScale")!=-1)
				{
					m_surgeFlags |= XScale;
					m_surgeFlags |= YScale;
					m_surgeFlags |= ZScale;
				}
				else
				{
					if(str.find("XScale")!=-1)
						m_surgeFlags |= XScale;
					if(str.find("YScale")!=-1)
						m_surgeFlags |= YScale;
					if(str.find("ZScale")!=-1)
						m_surgeFlags |= ZScale;
				}
			}
			//m_surgeFlags = ALL;
			//if(str.compare("ALL") == 0)
			//	m_surgeFlags = ALL;
			//else
			//{
			//	unsigned short this_flag;
			//	this_flag = XTran;
			//	if(str.find("XTran")==std::string::npos)
			//		m_surgeFlags &= ~this_flag;
			//	this_flag = YTran;
			//	if(str.find("YTran")==std::string::npos)				
			//		m_surgeFlags &= ~this_flag;			
			//	this_flag = ZTran;
			//	if(str.find("ZTran")==std::string::npos)
			//		m_surgeFlags &= ~this_flag;	
			//	this_flag = XRot;
			//	if(str.find("XRot")==std::string::npos)
			//		m_surgeFlags &= ~this_flag;		
			//	this_flag = YRot;
			//	if(str.find("YRot")==std::string::npos)
			//		m_surgeFlags &= ~this_flag;
			//	this_flag = ZRot;
			//	if(str.find("ZRot")==std::string::npos)
			//		m_surgeFlags &= ~this_flag;			
			//	this_flag = XScale;
			//	if(str.find("XScale")==std::string::npos)
			//		m_surgeFlags &= ~this_flag;			
			//	this_flag = YScale;
			//	if(str.find("YScale")==std::string::npos)
			//		m_surgeFlags &= ~this_flag;			
			//	this_flag = ZScale;
			//	if(str.find("ZScale")==std::string::npos)
			//		m_surgeFlags &= ~this_flag;
			//}
		}
		break;
	//case 18:
	//	//m_endStop = atoi(str.c_str());
	//	{
	//		crVector3i endStop;
	//		crArgumentParser::readKeyValue(str,"VEC3",endStop);
	//		m_endStop = endStop;
	//	}
	//	break;
	case 21:
		m_endless = atoi(str.c_str());
		break;
	case 22:
		m_lifeTime = atof(str.c_str());
		break;
	case 23:
		m_startTime = atof(str.c_str());
		break;
	case 24:
		m_resetTime = atof(str.c_str());
		break;
	}
	crTransform::addParam(i-25,str);
}