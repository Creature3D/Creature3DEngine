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
#ifndef CRUTIL_CRRENDERSTAGE_H
#define CRUTIL_CRRENDERSTAGE_H 1

#include <CRCore/crColorMask.h>
#include <CRCore/crViewPort.h>
#include <CRCore/crTexture.h>
#include <CRCore/crFrameBufferObject.h>
#include <CRCore/crCameraNode.h>

#include <CRUtil/crRenderBin.h>
#include <CRUtil/crPositionalStateContainer.h>


namespace CRUtil {

/**
 * crRenderStage base class. Used for encapsulate a complete stage in
 * rendering - setting up of viewport, the projection and model
 * matrices and rendering the crRenderBin's enclosed with this crRenderStage.
 * crRenderStage also has a dependency list of other RenderStages, each
 * of which must be called before the rendering of this stage.  These
 * 'pre' rendering stages are used for advanced rendering techniques
 * like multistage pixel shading or impostors.
 */
class CRUTIL_EXPORT crRenderStage : public crRenderBin
{
    public:
    

        crRenderStage();
        crRenderStage(SortMode mode);
        
        crRenderStage(const crRenderStage& rhs,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);
        
        virtual CRCore::crBase* cloneType() const { return new crRenderStage(); }
        virtual CRCore::crBase* clone(const CRCore::crCopyOp& copyop) const { return new crRenderStage(*this,copyop); } // note only implements a clone of type.
        virtual bool isSameKindAs(const CRCore::crBase* obj) const { return dynamic_cast<const crRenderStage*>(obj)!=0L; }
        virtual const char* className() const { return "crRenderStage"; }

        virtual void reset();
        
        
        /** Set the draw buffer used at the start of each frame draw. */
        void setDrawBuffer(GLenum buffer) { m_drawBuffer = buffer; }

        /** Get the draw buffer used at the start of each frame draw. */
        GLenum getDrawBuffer() const { return m_drawBuffer; }

        /** Set the read buffer for any required copy operations to use. */
        void setReadBuffer(GLenum buffer) { m_readBuffer = buffer; }

        /** Get the read buffer for any required copy operations to use. */
        GLenum getReadBuffer() const { return m_readBuffer; }


        /** Set the viewport.*/
        void setViewport(CRCore::crViewPort* viewport) { m_viewport = viewport; }

        /** Get the const viewport. */
        const CRCore::crViewPort* getViewport() const { return m_viewport.get(); }

        /** Get the viewport. */
        CRCore::crViewPort* getViewport() { return m_viewport.get(); }

        /** Set the clear mask used in glClear(..).
          * Defaults to GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT. */
        void setClearMask(GLbitfield mask) { m_clearMask = mask; }

        /** Get the clear mask.*/
        GLbitfield getClearMask() const { return m_clearMask; }


        void setColorMask(CRCore::crColorMask* cm) { m_colorMask = cm; }
        CRCore::crColorMask* getColorMask() { return m_colorMask.get(); }
        const CRCore::crColorMask* getColorMask() const { return m_colorMask.get(); }


        /** Set the clear color used in glClearColor(..). 
          * glClearColor is only called if mask & GL_COLOR_BUFFER_BIT is true*/
        void setClearColor(const CRCore::crVector4& color) { m_clearColor=color; }
        
        /** Get the clear color.*/
        const CRCore::crVector4& getClearColor() const { return m_clearColor; }
        
        /** Set the clear accum used in glClearAccum(..). 
          * glClearAcumm is only called if mask & GL_ACCUM_BUFFER_BIT is true. */
        void setClearAccum(const CRCore::crVector4& color) { m_clearAccum=color; }
        
        /** Get the clear accum.*/
        const CRCore::crVector4& getClearAccum() const { return m_clearAccum; }
        
        /** Set the clear depth used in glClearDepth(..). Defaults to 1.0
          * glClearDepth is only called if mask & GL_DEPTH_BUFFER_BIT is true. */
        void setClearDepth(double depth) { m_clearDepth=depth; }
        
        /** Get the clear depth.*/
        double getClearDepth() const { return m_clearDepth; }

        /** Set the clear stencil value used in glClearStencil(). Defaults to 0;
          * glClearStencil is only called if mask & GL_STENCIL_BUFFER_BIT is true*/
        void setClearStencil(int stencil) { m_clearStencil=stencil; }
        
        /** Get the clear color.*/
        int getClearStencil() const { return m_clearStencil; }
        

        void setCameraNode(CRCore::crCameraNode* camera);
        CRCore::crCameraNode* getCamera() { return m_camera; }
        const CRCore::crCameraNode* getCamera() const { return m_camera; }

        void setCameraRequiresSetUp(bool flag) { m_cameraRequiresSetUp = flag; }
        bool getCameraRequiresSetUp() const { return m_cameraRequiresSetUp; }
        
        /** Attempt the set the crRenderStage from the crCameraNode settings.*/
        void runCameraSetUp(CRCore::crState& state);
        
        void setTexture(CRCore::crTexture* texture, unsigned int level = 0, unsigned int face=0) { m_texture = texture; m_level = level; m_face = face; }
        CRCore::crTexture* getTexture() { return m_texture.get(); }
        
        void setImage(CRCore::crImage* image) { m_image = image; }
        CRCore::crImage* getImage() { return m_image.get(); }
        
        void setImageReadPixelFormat(GLenum format) { m_imageReadPixelFormat = format; }
        GLenum getImageReadPixelFormat() const { return m_imageReadPixelFormat; }

        void setImageReadPixelDataType(GLenum type) { m_imageReadPixelDataType = type; }
        GLenum getImageReadPixelDataType() const { return m_imageReadPixelDataType; }

        /** Set a framebuffer object to render into. It is permissible for the
          * framebuffer object to be multisampled, in which case you should also
          * set a resolve framebuffer object - see setMultisampleResolveFramebufferObject(). */
        void setFrameBufferObject(CRCore::crFrameBufferObject* fbo) { m_fbo = fbo; }
        CRCore::crFrameBufferObject* getFrameBufferObject() { return m_fbo.get(); }
        const CRCore::crFrameBufferObject* getFrameBufferObject() const { return m_fbo.get(); }

        /** Sets the destination framebuffer object for glBlitFramebufferEXT to
          * resolve a multisampled framebuffer object after the crRenderStage is
          * drawn. The resolve framebuffer object must not be multisampled. The
          * resolve framebuffer object is only necessary if the primary framebuffer
          * object is multisampled, if not then leave it set to null. */
        void setMultisampleResolveFramebufferObject(CRCore::crFrameBufferObject* fbo);
        CRCore::crFrameBufferObject* getMultisampleResolveFramebufferObject() { return m_resolveFbo.get(); }
        const CRCore::crFrameBufferObject* getMultisampleResolveFramebufferObject() const { return m_resolveFbo.get(); }

        /** Set whether the framebuffer object should be unbound after
          * rendering. By default this is set to true. Set it to false if the
          * unbinding is not required. */
        void setDisableFboAfterRender(bool disable) {m_disableFboAfterRender = disable;}
        bool getDisableFboAfterRender() const {return m_disableFboAfterRender;}

        void setGraphicsContext(CRCore::crGraphicsContext* context) { m_graphicsContext = context; }
        CRCore::crGraphicsContext* getGraphicsContext() { return m_graphicsContext.get(); }
        const CRCore::crGraphicsContext* getGraphicsContext() const { return m_graphicsContext.get(); }




        void setInheritedPositionalStateContainerMatrix(const CRCore::crMatrix& matrix) { m_inheritedPositionalStateContainerMatrix = matrix; }
        const CRCore::crMatrix& getInheritedPositionalStateContainerMatrix() const { return m_inheritedPositionalStateContainerMatrix; }

        void setInheritedPositionalStateContainer(crPositionalStateContainer* rsl) { m_inheritedPositionalStateContainer = rsl; }
        crPositionalStateContainer* getInheritedPositionalStateContainer() { return m_inheritedPositionalStateContainer.get(); }

        void setPositionalStateContainer(crPositionalStateContainer* rsl) { m_renderStageLighting = rsl; }

        crPositionalStateContainer* getPositionalStateContainer() const
        {
            if (!m_renderStageLighting.valid()) m_renderStageLighting = new crPositionalStateContainer;
            return m_renderStageLighting.get();
        }

        virtual void addPositionedAttribute(CRCore::RefMatrix* matrix,const CRCore::crStateAttribute* attr)
        {
            getPositionalStateContainer()->addPositionedAttribute(matrix,attr);
        }
        
        virtual void addPositionedTextureAttribute(unsigned int textureUnit, CRCore::RefMatrix* matrix,const CRCore::crStateAttribute* attr)
        {
            getPositionalStateContainer()->addPositionedTextureAttribute(textureUnit, matrix,attr);
        }

        void copyTexture(CRCore::crState& state);

        virtual void sort();
        
        virtual void drawPreRenderStages(CRCore::crState& state,crRenderLeaf*& previous);

        virtual void draw(CRCore::crState& state,crRenderLeaf*& previous);

        virtual void drawInner(CRCore::crState& state,crRenderLeaf*& previous, bool& doCopyTexture);
    
        virtual void drawPostRenderStages(CRCore::crState& state,crRenderLeaf*& previous);

        virtual void drawImplementation(CRCore::crState& state,crRenderLeaf*& previous);


        void addToDependencyList(crRenderStage* rs) { addPreRenderStage(rs); }

        void addPreRenderStage(crRenderStage* rs, int order = 0);
        
        void addPostRenderStage(crRenderStage* rs, int order = 0);

        /** Extract stats for current draw list. */
        //bool getStats(crStatistics& stats) const; 
		virtual void releaseObjects(CRCore::crState* state);

        /** Compute the number of dynamic RenderLeaves.*/
        //virtual unsigned int computeNumberOfDynamicRenderLeaves() const;

        struct Attachment
        {
            CRCore::ref_ptr<CRCore::crImage>                m_image;
            GLenum                                  m_imageReadPixelFormat;
            GLenum                                  m_imageReadPixelDataType;
        };
       
        void attach(CRCore::crCameraNode::BufferComponent buffer, CRCore::crImage* image);
        
		void drawZprePass(CRCore::crState& state,crRenderLeaf*& previous);
    protected:
        
        virtual ~crRenderStage();

		typedef std::vector< CRCore::ref_ptr<crRenderStage> > RenderStageList;

        bool                                m_stageDrawnThisFrame;
        RenderStageList                     m_preRenderList;
        RenderStageList                     m_postRenderList;

        // viewport x,y,width,height.
        CRCore::ref_ptr<CRCore::crViewPort>         m_viewport;
        
        GLenum                              m_drawBuffer;
        GLenum                              m_readBuffer;
        GLbitfield                          m_clearMask;
        CRCore::ref_ptr<CRCore::crColorMask>        m_colorMask;
        CRCore::crVector4                           m_clearColor;
        CRCore::crVector4                           m_clearAccum;
        double                              m_clearDepth;
        int                                 m_clearStencil;

        bool                                m_cameraRequiresSetUp;
        CRCore::crCameraNode*                        m_camera;
        
        CRCore::ref_ptr<CRCore::crTexture>              m_texture;
        unsigned int                            m_level;
        unsigned int                            m_face;

        CRCore::ref_ptr<CRCore::crImage>                m_image;
        GLenum                                  m_imageReadPixelFormat;
        GLenum                                  m_imageReadPixelDataType;
        
        std::map< CRCore::crCameraNode::BufferComponent, Attachment> m_bufferAttachmentMap;
        
        CRCore::ref_ptr<CRCore::crFrameBufferObject>    m_fbo;
        CRCore::ref_ptr<CRCore::crFrameBufferObject>    m_resolveFbo;
        CRCore::ref_ptr<CRCore::crGraphicsContext>      m_graphicsContext;
        bool                                    m_disableFboAfterRender;

        mutable CRCore::crMatrix                         m_inheritedPositionalStateContainerMatrix;
        mutable CRCore::ref_ptr<crPositionalStateContainer>   m_inheritedPositionalStateContainer;
        mutable CRCore::ref_ptr<crPositionalStateContainer>   m_renderStageLighting;
        
    

};

}

#endif

