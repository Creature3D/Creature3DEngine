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
#ifndef CRUTIL_TRANSFORMCALLBACK
#define CRUTIL_TRANSFORMCALLBACK 1

#include <CRCore/crNode.h>
#include <CRUtil/crExport.h>

namespace CRUtil
{

class CRUTIL_EXPORT crTransformCallback : public CRCore::crNodeCallback
{

    public:

        crTransformCallback(const CRCore::crVector3f& pivot,const CRCore::crVector3f& axis,float angularVelocity);

        void setPause(bool pause) { m_pause = pause; }

        /** implements the callback*/
        virtual void operator()(CRCore::crNode* node, CRCore::crNodeVisitor* nv);

    protected:
    
        float               m_angular_velocity;
        CRCore::crVector3f           m_pivot;
        CRCore::crVector3f           m_axis;

        int                 m_previousTraversalNumber;
        //double              m_previousTime;
        bool                m_pause;

};

}

#endif
