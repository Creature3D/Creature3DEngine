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
#ifndef CRUTIL_CRDISPLAYREQUIREMENTSVISITOR_H
#define CRUTIL_CRDISPLAYREQUIREMENTSVISITOR_H 1

#include <CRUtil/crExport.h>
#include <CRCore/crNodeVisitor.h>
#include <CRCore/crObject.h>
#include <CRCore/crDisplaySettings.h>

namespace CRUtil {

/** A visitor for traversing a scene graph establishing the OpenGL visuals are required
  * to support rendering of that scene graph.  The results can then be used by
  * applications to set up there windows with the correct visuals.  Have a look at
  * src/osgGLUT/Viewer.cpp's Viewer::open() method for an example how to use it.
  */
class CRUTIL_EXPORT crDisplayRequirementsVisitor : public CRCore::crNodeVisitor
{
    public:

        /** Default to traversing all children, and reqiresDoubleBuffer,
          * requiresRGB and requiresDepthBuffer to true and with
          * alpha and stencil off.*/
        crDisplayRequirementsVisitor();
        

        /** Set the crDisplaySettings. */
        inline void setDisplaySettings(CRCore::crDisplaySettings* ds) { m_ds = ds; }
        
        /** Get the crDisplaySettings */
        inline const CRCore::crDisplaySettings* getDisplaySettings() const { return m_ds.get(); }
        
        virtual void applyStateSet(CRCore::crStateSet& stateset);

        virtual void apply(CRCore::crNode& node);

        virtual void apply(CRCore::crObject& object);

  //      virtual void apply(CRCore::Impostor& impostor);
        
    protected:
    
        CRCore::ref_ptr<CRCore::crDisplaySettings> m_ds;
    
};

}

#endif
