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
//Modified by Îâ²Æ»ª
#include <CRCore/crMatrixTransform.h>
#include <CRCore/crFrameStamp.h>
#include <CRUtil/crTransformCallback.h>

using namespace CRCore;
using namespace CRUtil;

crTransformCallback::crTransformCallback(const CRCore::crVector3f& pivot,const CRCore::crVector3f& axis,float angularVelocity)
{
    m_pivot = pivot;
    m_axis = axis;
    m_angular_velocity = angularVelocity;
    
    m_previousTraversalNumber = -1;
    //m_previousTime = -1.0;
    
    m_pause = false;
}

void crTransformCallback::operator() (CRCore::crNode* node, CRCore::crNodeVisitor* nv)
{
    CRCore::crMatrixTransform* transform = dynamic_cast<CRCore::crMatrixTransform*>(node);
    if (nv && transform)
    {
        
        //const CRCore::crFrameStamp* fs = nv->getFrameStamp();
        //if (!fs) return; // not frame stamp, no handle on the time so can't move.
        
        //double newTime = fs->getReferenceTime();

        // ensure that we do not operate on this node more than
        // once during this traversal.  This is an issue since node
        // can be shared between multiple parents.
        if (!m_pause && nv->getTraversalNumber()!=m_previousTraversalNumber)
        {
            //float delta_angle = m_angular_velocity*(newTime-m_previousTime);
			float delta_angle = m_angular_velocity*(crFrameStamp::getInstance()->getFrameInterval());

            CRCore::crMatrix mat = CRCore::crMatrix::translate(-m_pivot)*
                              CRCore::crMatrix::rotate(delta_angle,m_axis)*
                              CRCore::crMatrix::translate(m_pivot);


            // update the specified transform
            transform->preMult(mat);
            
            m_previousTraversalNumber = nv->getTraversalNumber();
        }

        //m_previousTime = newTime; 

    }

    // must call any nested node callbacks and continue subgraph traversal.
    traverse(node,nv);

}
