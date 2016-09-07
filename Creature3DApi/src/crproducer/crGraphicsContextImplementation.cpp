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
#include <CRProducer/crGraphicsContextImplementation.h>
#include <CRCore/crTextureRectangle.h>
#include <CRCore/crTextureCubeMap.h>
#include <CRCore/crNotify.h>

using namespace CRProducer;
namespace CRProducer
{
    struct MyCreateGraphicContexCallback : public CRCore::crGraphicsContext::CreateGraphicContextCallback
    {
        virtual CRCore::crGraphicsContext* createGraphicsContext(CRCore::crGraphicsContext::Traits* traits)
        {
            return new crGraphicsContextImplementation(traits);
        }
    };

    struct RegisterCreateGraphicsContextCallbackProxy
    {
        RegisterCreateGraphicsContextCallbackProxy()
        {
            CRCore::crGraphicsContext::setCreateGraphicsContextCallback(new MyCreateGraphicContexCallback);
        }
        
        ~RegisterCreateGraphicsContextCallbackProxy()
        {
            CRCore::crGraphicsContext::setCreateGraphicsContextCallback(0);
        }
    };
    
    RegisterCreateGraphicsContextCallbackProxy createGraphicsContextCallbackProxy;
};


crGraphicsContextImplementation::crGraphicsContextImplementation(Traits* traits)
{
    m_traits = traits;

    m_rs = new Producer::RenderSurface;
    m_rs->setWindowName(traits->m_windowName);
    m_rs->setWindowRectangle(traits->m_x, traits->m_y, traits->m_width, traits->m_height);
    m_rs->useBorder(traits->m_windowDecoration);

    // set the visual chooser
    Producer::VisualChooser* rs_vc = m_rs->getVisualChooser();
    if (!rs_vc)
    {
        rs_vc = new Producer::VisualChooser;
        m_rs->setVisualChooser(rs_vc);
    }
    
    rs_vc->setRedSize(m_traits->m_red);
    rs_vc->setGreenSize(m_traits->m_green);
    rs_vc->setBlueSize(m_traits->m_blue);
    rs_vc->setAlphaSize(m_traits->m_alpha);
    
    rs_vc->setDepthSize(m_traits->m_depth);
    rs_vc->setStencilSize(m_traits->m_stencil);
    
    if (m_traits->m_doubleBuffer) rs_vc->useDoubleBuffer();

    rs_vc->addAttribute( Producer::VisualChooser::RGBA );

    // Always use UseGL
    rs_vc->addAttribute( Producer::VisualChooser::UseGL );
 
    if (traits->m_pbuffer)
    {
        m_rs->setDrawableType(Producer::RenderSurface::DrawableType_PBuffer);

        if (traits->m_target)
        {

            m_rs->setRenderToTextureOptions(traits->m_mipMapGeneration ? Producer::RenderSurface::RequestSpaceForMipMaps :
                                                                       Producer::RenderSurface::RenderToTextureOptions_Default);
            m_rs->setRenderToTextureMipMapLevel(traits->m_level);
            m_rs->setRenderToTextureMode(traits->m_alpha>0 ? Producer::RenderSurface::RenderToRGBATexture : 
                                                           Producer::RenderSurface::RenderToRGBTexture);

            switch(traits->m_target)
            {
                case(GL_TEXTURE_1D) : 
                    m_rs->setRenderToTextureTarget(Producer::RenderSurface::Texture1D);
                    break;
                case(GL_TEXTURE_2D) : 
                    m_rs->setRenderToTextureTarget(Producer::RenderSurface::Texture2D);
                    break;
                case(GL_TEXTURE_3D) :
                    CRCore::notify(CRCore::NOTICE)<<"PBuffer render to Texture3D not supported."<<std::endl;
                    // not supported. 
                    // m_rs->setRenderToTextureTarget(Producer::RenderSurface::Texture3D);
                    break;
                case(GL_TEXTURE_RECTANGLE_NV) : 
                    CRCore::notify(CRCore::NOTICE)<<"PBuffer render to crTextureRectangle not supported."<<std::endl;
                    // not supported.
                    // m_rs->setRenderToTextureTarget(Producer::RenderSurface::crTextureRectangle);
                    break;
                case(GL_TEXTURE_CUBE_MAP_POSITIVE_X) : 
                case(GL_TEXTURE_CUBE_MAP_NEGATIVE_X) : 
                case(GL_TEXTURE_CUBE_MAP_POSITIVE_Y) : 
                case(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y) : 
                case(GL_TEXTURE_CUBE_MAP_POSITIVE_Z) : 
                case(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z) : 
                    m_rs->setRenderToTextureTarget(Producer::RenderSurface::TextureCUBE);
                    m_rs->setRenderToTextureFace( Producer::RenderSurface::CubeMapFace(traits->m_target - GL_TEXTURE_CUBE_MAP_POSITIVE_X));
                    break;
            }

        }
        
    }
    
    crGraphicsContextImplementation* sharedContext = dynamic_cast<crGraphicsContextImplementation*>(traits->m_sharedContext);

    if (sharedContext)
    {
        // different graphics context so we have our own state.
        setState(new CRCore::crState);
        
        if (sharedContext->getState())
        {
            getState()->setContextID( sharedContext->getState()->getContextID() );
            incrementContextIDUsageCount( sharedContext->getState()->getContextID() );   
        }
        else
        {
            getState()->setContextID( CRCore::crGraphicsContext::createNewContextID() );
        }
        
        // but we share texture objects etc. so we also share the same contextID
        //m_rs->realize( 0, sharedContext->m_rs->getGLContext() );
        
    }
    else
    {
    
        // need to do something here....    
        setState( new CRCore::crState );
        getState()->setContextID( CRCore::crGraphicsContext::createNewContextID() );

        //m_rs->realize();
    }
    
    // m_rs->useConfigEventThread(false);

    m_closeOnDestruction = true;
}

crGraphicsContextImplementation::crGraphicsContextImplementation(Producer::RenderSurface* rs)
{
    m_rs = rs;
    m_closeOnDestruction = false;
}

crGraphicsContextImplementation::~crGraphicsContextImplementation()
{
    if (m_closeOnDestruction) close();
}

bool crGraphicsContextImplementation::realizeImplementation()
{
    if (m_rs.valid()) 
    {
        crGraphicsContextImplementation* sharedContext = dynamic_cast<crGraphicsContextImplementation*>(m_traits->m_sharedContext);

        if (sharedContext)
        {
            m_rs->realize( 0, sharedContext->m_rs->getGLContext() );
        }
        else
        {
            m_rs->realize();
        }
        return m_rs->isRealized();
    }
    else
    {
        return false;
    }
}

void crGraphicsContextImplementation::makeCurrentImplementation()
{
    if (!m_rs)
    {
        CRCore::notify(CRCore::NOTICE)<<"Error: crGraphicsContextImplementation::makeCurrentImplementation() no RenderSurface."<<std::endl;
        return;
    }

    if (!isRealized())
    {
        CRCore::notify(CRCore::NOTICE)<<"Error: crGraphicsContextImplementation::makeCurrentImplementation() not Realized."<<std::endl;
        return;
    }

//    CRCore::notify(CRCore::INFO)<<"crGraphicsContextImplementation::makeCurrentImplementation()"<<std::endl;

    m_rs->setReadDrawable( 0 );

    // comment out right now, as Producer's setReadDrawable() is doing a call for us.
    // m_rs->makeCurrent();
}

void crGraphicsContextImplementation::makeContextCurrentImplementation(CRCore::crGraphicsContext* readContext)
{
    if (!m_rs) return;

    crGraphicsContextImplementation* readContextImplemention = dynamic_cast<crGraphicsContextImplementation*>(readContext);

    if (readContextImplemention)
    {
        m_rs->setReadDrawable( readContextImplemention->getRenderSurface() );
    }
    else
    {
        m_rs->setReadDrawable( 0 );
    }

    // comment out right now, as Producer's setReadDrawable() is doing a call for us.
    // m_rs->makeCurrent();
}

void crGraphicsContextImplementation::closeImplementation()
{
    if (!m_rs) return;
    
    // close render surface by deleting it 
    m_rs = 0;
}

void crGraphicsContextImplementation::bindPBufferToTextureImplementation(GLenum buffer)
{
    if (!m_rs) return;
 
    Producer::RenderSurface::BufferType bufferType = Producer::RenderSurface::FrontBuffer;
    switch(buffer)
    {
        case(GL_BACK): bufferType = Producer::RenderSurface::BackBuffer; break;
        case(GL_FRONT): bufferType = Producer::RenderSurface::FrontBuffer; break;
        default: bufferType = Producer::RenderSurface::FrontBuffer; break;
    }

    m_rs->bindPBufferToTexture(bufferType);
}

void crGraphicsContextImplementation::swapBuffersImplementation()
{
    m_rs->swapBuffers();
}
