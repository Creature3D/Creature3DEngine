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
#ifndef CRCORE_CRCLEARNODE_H
#define CRCORE_CRCLEARNODE_H 1

#include <CRCore/crGroup.h>
#include <CRCore/crVector4.h>

namespace CRCore {

/** crClearNode is a crGroup node which controls the clearing of the color and depth
  * buffers at the start of each frame.
  * The earth sky by default is empty and simply holds the clear color of
  * the background. However, if the uses wants to add their own clearing of
  * the color and depth buffers then the children can be added, and the
  * background clear turned off. The crClearNode by default has StateSet attached
  * to it which sets the default crClearNode bin number to -1, so that all drawables
  * below it are placed in a separate bin from the rest of the scene graph, and
  * are rendered prior to standard opaque and transparent drawables.
*/
class CR_EXPORT crClearNode : public crGroup
{
    public :
        
        crClearNode();

        crClearNode(const crClearNode& es, const crCopyOp& copyop=crCopyOp::SHALLOW_COPY):
            crGroup(es,copyop),
            m_requiresClear(es.m_requiresClear),
            m_clearColor(es.m_clearColor) {}  


        META_Node(CRCore, crClearNode);

	/** Sets the flag which control whether a glClear is required at the beginning of each frame. */
        inline void setRequiresClear(bool requiresClear) { m_requiresClear = requiresClear; }

	/** Gets the flag which control whether a glClear is required at the beginning of each frame. */
        inline bool getRequiresClear() const { return m_requiresClear; }

	/** Sets the clear color. */
        inline void setClearColor(const crVector4& color) { m_clearColor = color; }

	/** Returns the clear color. */
        inline const crVector4& getClearColor() const { return m_clearColor; }

		/** Set the clear mask used in glClear(..).
		* Defaults to GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT. */
		inline void setClearMask(GLbitfield mask) { m_clearMask = mask; }

		/** Get the clear mask.*/
		inline GLbitfield getClearMask() const { return m_clearMask; }

    protected :
    
        virtual ~crClearNode() {}

        bool m_requiresClear;
        crVector4 m_clearColor;  
		GLbitfield m_clearMask;
};

}

#endif
