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
#ifndef CRPRODUCER_GRAPHICSCONTEXTIMPLEMENTATION_H
#define CRPRODUCER_GRAPHICSCONTEXTIMPLEMENTATION_H 1


#include <CRCore/crGraphicsContext.h>

#include <CRProducer/Producer/RenderSurface>

#include <CRProducer/crExport.h>

namespace CRProducer {

class CRPRODUCER_EXPORT crGraphicsContextImplementation : public CRCore::crGraphicsContext
{
public :

    /** Construct a graphics context to specified traits.*/
    crGraphicsContextImplementation(Traits* traits);

    /** Construct a graphics context with specified RenderSurface.*/
    crGraphicsContextImplementation(Producer::RenderSurface* rs);

    /** Return true of graphics context is realized.*/

    /** Return the RenderSurface that implements the graphics context.*/
    Producer::RenderSurface* getRenderSurface() { return m_rs.get(); }
    
    /** Return the const RenderSurface that implements the graphics context.*/
    const Producer::RenderSurface* getRenderSurface() const { return m_rs.get(); }


    /** Realise the crGraphicsContext.*/
    virtual bool realizeImplementation();

    /** Return true if the graphics context has been realised and is ready to use.*/
    virtual bool isRealizedImplementation() const { return m_rs.valid() && m_rs->isRealized(); }

    /** Close the graphics context.*/
    virtual void closeImplementation();

    /** Make this graphics context current.*/
    virtual void makeCurrentImplementation();

    /** Make this graphics context current with specified read context.*/
    virtual void makeContextCurrentImplementation(CRCore::crGraphicsContext* readContext);
        
    /** Bind the graphics context to associated texture.*/
    virtual void bindPBufferToTextureImplementation(GLenum buffer);

    /** swap the front and back buffers.*/
    virtual void swapBuffersImplementation();


protected:

    virtual ~crGraphicsContextImplementation();
    
    bool                                  m_closeOnDestruction;
    CRCore::ref_ptr<Producer::RenderSurface> m_rs;
};

}

#endif
