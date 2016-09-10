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
#ifndef CRCORE_CRCAMERANODE
#define CRCORE_CRCAMERANODE 1

#include <CRCore/crTransform.h>
#include <CRCore/crViewPort.h>
#include <CRCore/crColorMask.h>
#include <CRCore/crCullSettings.h>
#include <CRCore/crTexture.h>
#include <CRCore/crImage.h>
#include <CRCore/crGraphicsContext.h>
#include <CRCore/crCullingSet.h>

#include <CRCore/thread/crMutex.h>

namespace CRCore {

/** crCameraNode - is a subclass of crTransform which represents encapsulates the settings of a crCameraNode.
*/
class CR_EXPORT crCameraNode : public crTransform, public crCullSettings
{
    public :


        crCameraNode();

        /** Copy constructor using crCopyOp to manage deep vs shallow copy.*/
        crCameraNode(const crCameraNode&,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY);

        META_Node(CRCore, crCameraNode);

		/** Set the clear color used in glClearColor(..). 
		* glClearColor is only called if mask & GL_COLOR_BUFFER_BIT is true*/
		void setClearColor(const CRCore::crVector4& color) { m_clearColor=color; }

		/** Get the clear color.*/
		const CRCore::crVector4& getClearColor() const { return m_clearColor; }

        /** Set the clear mask used in glClear(..).
          * Defaults to GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT. */
        inline void setClearMask(GLbitfield mask) { m_clearMask = mask; }

        /** Get the clear mask.*/
        inline GLbitfield getClearMask() const { return m_clearMask; }
        
        /** Set the clear accum used in glClearAccum(..). 
          * glClearAcumm is only called if mask & GL_ACCUM_BUFFER_BIT is true. */
        void setClearAccum(const CRCore::crVector4& color) { m_clearAccum=color; }
        
        /** Get the clear accum value.*/
        const CRCore::crVector4& getClearAccum() const { return m_clearAccum; }
        
        /** Set the clear depth used in glClearDepth(..). Defaults to 1.0
          * glClearDepth is only called if mask & GL_DEPTH_BUFFER_BIT is true. */
        void setClearDepth(double depth) { m_clearDepth=depth; }
        
        /** Get the clear depth value.*/
        double getClearDepth() const { return m_clearDepth; }

        /** Set the clear stencil value used in glClearStencil(). Defaults to 0;
          * glClearStencil is only called if mask & GL_STENCIL_BUFFER_BIT is true*/
        void setClearStencil(int stencil) { m_clearStencil=stencil; }
        
        /** Get the clear stencil value.*/
        int getClearStencil() const { return m_clearStencil; }
        
        
        /** Set the color mask of the camera to use specified CRCore::crColorMask. */
        void setColorMask(CRCore::crColorMask* colorMask);


        /** Set the color mask of the camera to specified values. */
        void setColorMask(bool red, bool green, bool blue, bool alpha);
        
        /** Get the const crColorMask. */
        const crColorMask* getColorMask() const { return m_colorMask.get(); }

        /** Get the crColorMask. */
        crColorMask* getColorMask() { return m_colorMask.get(); }


        /** Set the viewport of the camera to use specified CRCore::crViewPort. */
        void setViewport(CRCore::crViewPort* viewport);

        /** Set the viewport of the camera to specified dimensions. */
        void setViewport(int x,int y,int width,int height);
        
        /** Get the const viewport. */
        const crViewPort* getViewport() const { return m_viewport.get(); }

        /** Get the viewport. */
        crViewPort* getViewport() { return m_viewport.get(); }


        enum TransformOrder
        {
            PRE_MULTIPLE,
            POST_MULTIPLE  
        };
        
        /** Set the transformation order for world-to-local and local-to-world transformation.*/
        void setTransformOrder(TransformOrder order) { m_transformOrder = order; }

        /** Get the transformation order.*/
        TransformOrder getTransformOrder() const { return m_transformOrder; }

		/** Set the projection matrix. Can be thought of as setting the lens of a camera. */
		inline void setProjectionMatrix(const CRCore::crMatrixf& matrix) 
		{ 			
			if(!m_projectionMatrix) m_projectionMatrix = new crMatrixd;
			m_projectionMatrix->set(matrix); 
		}

		/** Set the projection matrix. Can be thought of as setting the lens of a camera. */
		inline void setProjectionMatrix(const CRCore::crMatrixd& matrix) 
		{ 
			if(!m_projectionMatrix) m_projectionMatrix = new crMatrixd;
			m_projectionMatrix->set(matrix); 
		}

		inline void setBufProjectionMatrix(const CRCore::crMatrixf& matrix) 
		{ 			
			if(!m_projectionMatrix_buf) m_projectionMatrix_buf = new crMatrixd;
			m_projectionMatrix_buf->set(matrix); 
		}

		/** Set the projection matrix. Can be thought of as setting the lens of a camera. */
		inline void setBufProjectionMatrix(const CRCore::crMatrixd& matrix) 
		{ 
			if(!m_projectionMatrix_buf) m_projectionMatrix_buf = new crMatrixd;
			m_projectionMatrix_buf->set(matrix); 
		}

        /** Set to an orthographic projection. See OpenGL glOrtho for documentation further details.*/
        void setProjectionMatrixAsOrtho(double left, double right,
                                        double bottom, double top,
                                        double zNear, double zFar);

        /** Set to a 2D orthographic projection. See OpenGL glOrtho2D documentation for further details.*/
        void setProjectionMatrixAsOrtho2D(double left, double right,
                                          double bottom, double top);

        /** Set to a perspective projection. See OpenGL glFrustum documentation for further details.*/
        void setProjectionMatrixAsFrustum(double left, double right,
                                          double bottom, double top,
                                          double zNear, double zFar);

        /** Create a symmetrical perspective projection, See OpenGL gluPerspective documentation for further details.
          * Aspect ratio is defined as width/height.*/
        void setProjectionMatrixAsPerspective(double fovy,double aspectRatio,
                                              double zNear, double zFar);

		/** Get the projection matrix.*/
		CRCore::crMatrixd* getProjectionMatrix() 
		{   
			if(!m_projectionMatrix) m_projectionMatrix = new crMatrixd;
			return m_projectionMatrix.get(); 
		}

		/** Get the const projection matrix.*/
		const CRCore::crMatrixd* getProjectionMatrix() const 
		{   
			if(!m_projectionMatrix) m_projectionMatrix = new crMatrixd;
			return m_projectionMatrix.get(); 
		}

		/** Get the projection matrix.*/
		CRCore::crMatrixd* getBufProjectionMatrix() 
		{   
			if(!m_projectionMatrix_buf) m_projectionMatrix_buf = new crMatrixd;
			return m_projectionMatrix_buf.get(); 
		}

		/** Get the const projection matrix.*/
		const CRCore::crMatrixd* getBufProjectionMatrix() const 
		{   
			if(!m_projectionMatrix_buf) m_projectionMatrix_buf = new crMatrixd;
			return m_projectionMatrix_buf.get(); 
		}

        /** Get the orthographic settings of the orthographic projection matrix. 
          * Returns false if matrix is not an orthographic matrix, where parameter values are undefined.*/
        bool getProjectionMatrixAsOrtho(double& left, double& right,
                                        double& bottom, double& top,
                                        double& zNear, double& zFar);

        /** Get the frustum setting of a perspective projection matrix.
          * Returns false if matrix is not a perspective matrix, where parameter values are undefined.*/
        bool getProjectionMatrixAsFrustum(double& left, double& right,
                                          double& bottom, double& top,
                                          double& zNear, double& zFar);

        /** Get the frustum setting of a symmetric perspective projection matrix.
          * Returns false if matrix is not a perspective matrix, where parameter values are undefined. 
          * Note, if matrix is not a symmetric perspective matrix then the shear will be lost.
          * Asymmetric matrices occur when stereo, power walls, caves and reality center display are used.
          * In these configurations one should use the 'getProjectionMatrixAsFrustum' method instead.*/
        bool getProjectionMatrixAsPerspective(double& fovy,double& aspectRatio,
                                              double& zNear, double& zFar);



		/** Set the view matrix. Can be thought of as setting the position of the world relative to the camera in camera coordinates. */
		inline void setViewMatrix(const CRCore::crMatrixf& matrix) 
		{
			if(!m_viewMatrix) m_viewMatrix = new crMatrixd;
			m_viewMatrix->set(matrix);
			//setViewInverseMatrix(CRCore::crMatrixd::inverse(matrix));
			dirtyBound();
		}

		/** Set the view matrix. Can be thought of as setting the position of the world relative to the camera in camera coordinates. */
		inline void setViewMatrix(const CRCore::crMatrixd& matrix) 
		{ 
			if(!m_viewMatrix) m_viewMatrix = new crMatrixd;
			m_viewMatrix->set(matrix);
			//setViewInverseMatrix(CRCore::crMatrixd::inverse(matrix));
			dirtyBound();
		}

		inline void setBufViewMatrix(const CRCore::crMatrixf& matrix) 
		{
			if(!m_viewMatrix_buf) m_viewMatrix_buf = new crMatrixd;
			m_viewMatrix_buf->set(matrix);  
			//setBufViewInverseMatrix(CRCore::crMatrixd::inverse(matrix));
			dirtyBound();
		}

		inline void setBufViewMatrix(const CRCore::crMatrixd& matrix) 
		{
			if(!m_viewMatrix_buf) m_viewMatrix_buf = new crMatrixd;
			m_viewMatrix_buf->set(matrix);  
			//setBufViewInverseMatrix(CRCore::crMatrixd::inverse(matrix));
			dirtyBound();
		}
		
		inline void computerViewInverseMatrix()
		{
			setViewInverseMatrix(CRCore::crMatrixd::inverse(*getViewMatrix()));
			//setBufViewInverseMatrix(CRCore::crMatrixd::inverse(*getBufViewMatrix()));
		}
		/** Set to the position and orientation of view matrix, using the same convention as gluLookAt. */
		void setViewMatrixAsLookAt(const CRCore::crVector3& eye,const CRCore::crVector3& center,const CRCore::crVector3& up);

		/** Get the view matrix. */
		inline CRCore::crMatrixd* getViewMatrix() 
		{   
			if(!m_viewMatrix) m_viewMatrix = new crMatrixd;
			return m_viewMatrix.get(); 
		}
		/** Get the const view matrix. */
		inline const CRCore::crMatrixd* getViewMatrix() const
		{   
			if(!m_viewMatrix) m_viewMatrix = new crMatrixd;
			return m_viewMatrix.get(); 
		}

		/** Get the view matrix. */
		inline CRCore::crMatrixd* getBufViewMatrix()
		{   
			if(!m_viewMatrix_buf) m_viewMatrix_buf = new crMatrixd;
			return m_viewMatrix_buf.get(); 
		}

		/** Get the const view matrix. */
		inline const CRCore::crMatrixd* getBufViewMatrix() const
		{   
			if(!m_viewMatrix_buf) m_viewMatrix_buf = new crMatrixd;
			return m_viewMatrix_buf.get(); 
		}

		/** Get the view matrix. */
		inline CRCore::crMatrixd* getViewInverseMatrix() 
		{   
			if(!m_viewInverseMatrix) m_viewInverseMatrix = new crMatrixd;
			return m_viewInverseMatrix.get(); 
		}
		/** Get the const view matrix. */
		inline const CRCore::crMatrixd* getViewInverseMatrix() const
		{   
			if(!m_viewInverseMatrix) m_viewInverseMatrix = new crMatrixd;
			return m_viewInverseMatrix.get(); 
		}

		/** Get the view matrix. */
		inline CRCore::crMatrixd* getBufViewInverseMatrix()
		{   
			if(!m_viewInverseMatrix_buf) m_viewInverseMatrix_buf = new crMatrixd;
			return m_viewInverseMatrix_buf.get(); 
		}

		/** Get the const view matrix. */
		inline const CRCore::crMatrixd* getBufViewInverseMatrix() const
		{   
			if(!m_viewInverseMatrix_buf) m_viewInverseMatrix_buf = new crMatrixd;
			return m_viewInverseMatrix_buf.get(); 
		}

		virtual void swapBuffers(int frameNumber);

		/** Get to the position and orientation of a modelview matrix, using the same convention as gluLookAt. */
		void getViewMatrixAsLookAt(CRCore::crVector3d& eye,CRCore::crVector3d& center,CRCore::crVector3d& up,float lookDistance=1.0f);
        
		/** Get the inverse view matrix.*/
		//crMatrixd getInverseViewMatrix() const;

        enum RenderOrder
        {
            PRE_RENDER,
            NESTED_RENDER,
            POST_RENDER
        };
        
        /** Set the rendering order of this camera's subgraph relative to any camera that this subgraph is nested within.
          * For rendering to a texture, one typically uses PRE_RENDER.
          * For Head Up Displays, one would typically use POST_RENDER.*/
		void setRenderOrder(RenderOrder order) { m_renderOrder = order; }
        
        /** Get the rendering order of this camera's subgraph relative to any camera that this subgraph is nested within.*/
		RenderOrder getRenderOrder() const { return m_renderOrder; }

        /** Return true if this crCameraNode is set up as a render to texture camera, i.e. it has textures assigned to it.*/
        bool isRenderToTextureCamera() const;

        enum RenderTargetImplementation
        {
            FRAME_BUFFER_OBJECT,
            PIXEL_BUFFER_RTT,
            PIXEL_BUFFER,
            FRAME_BUFFER,
            SEPERATE_WINDOW
        };

        /** Set the render target.*/
        void setRenderTargetImplementation(RenderTargetImplementation impl);

        /** Set the render target and fall-back that's used if the former isn't available.*/
        void setRenderTargetImplementation(RenderTargetImplementation impl, RenderTargetImplementation fallback);

        /** Get the render target.*/
        RenderTargetImplementation getRenderTargetImplementation() const { return m_renderTargetImplementation; }

        /** Get the render target fallback.*/
        RenderTargetImplementation getRenderTargetFallback() const { return m_renderTargetFallback; }


        /** Set the draw buffer used at the start of each frame draw. 
          * Note, a buffer value of GL_NONE is used to sepecify that the rendering back-end should choose the most appropriate buffer.*/
        void setDrawBuffer(GLenum buffer) { m_drawBuffer = buffer; }

        /** Get the draw buffer used at the start of each frame draw. */
        GLenum getDrawBuffer() const { return m_drawBuffer; }

        /** Set the read buffer for any required copy operations to use.  
          * Note, a buffer value of GL_NONE is used to sepecify that the rendering back-end should choose the most appropriate buffer.*/
        void setReadBuffer(GLenum buffer) { m_readBuffer = buffer; }

        /** Get the read buffer for any required copy operations to use. */
        GLenum getReadBuffer() const { return m_readBuffer; }

        enum BufferComponent
        {
            DEPTH_BUFFER,
            STENCIL_BUFFER,
			PACKED_DEPTH_STENCIL_BUFFER,
            COLOR_BUFFER,
            COLOR_BUFFER0,
            COLOR_BUFFER1 = COLOR_BUFFER0+1,
            COLOR_BUFFER2 = COLOR_BUFFER0+2,
            COLOR_BUFFER3 = COLOR_BUFFER0+3,
            COLOR_BUFFER4 = COLOR_BUFFER0+4,
            COLOR_BUFFER5 = COLOR_BUFFER0+5,
            COLOR_BUFFER6 = COLOR_BUFFER0+6,
            COLOR_BUFFER7 = COLOR_BUFFER0+7,
            COLOR_BUFFER8 = COLOR_BUFFER0+8,
            COLOR_BUFFER9 = COLOR_BUFFER0+9,
            COLOR_BUFFER10 = COLOR_BUFFER0+10,
            COLOR_BUFFER11 = COLOR_BUFFER0+11,
            COLOR_BUFFER12 = COLOR_BUFFER0+12,
            COLOR_BUFFER13 = COLOR_BUFFER0+13,
            COLOR_BUFFER14 = COLOR_BUFFER0+14,
            COLOR_BUFFER15 = COLOR_BUFFER0+15
        };

        /** Attach a buffer with specified OpenGL internal format.*/
        void attach(BufferComponent buffer, GLenum internalFormat);

        /** Attach a crTexture to specified buffer component. 
          * The level parameter controls the mip map level of the texture that is attached. 
          * The face parameter controls the face of texture cube map or z level of 3d texture. 
          * The mipMapGeneration flag controls whether mipmap generation should be done for texture.*/
        void attach(BufferComponent buffer, CRCore::crTexture* texture, unsigned int level = 0, unsigned int face=0, bool mipMapGeneration=false,
            unsigned int multisampleSamples = 0,
            unsigned int multisampleColorSamples = 0);

        /** Attach a crImage to specified buffer component.*/
        void attach(BufferComponent buffer, CRCore::crImage* image,
            unsigned int multisampleSamples = 0,
            unsigned int multisampleColorSamples = 0);

        /** Detach specified buffer component.*/
        void detach(BufferComponent buffer);
        
        struct Attachment
        {
            Attachment():
                m_internalFormat(GL_NONE),
                m_level(0),
                m_face(0),
                m_mipMapGeneration(false),
                m_multisampleSamples(0),
                m_multisampleColorSamples(0) {}
        
            int width() const
            {
                if (m_texture.valid()) return m_texture->getTextureWidth(); 
                if (m_image.valid()) return m_image->s();
                return 0;
            };

            int height() const
            {
                if (m_texture.valid()) return m_texture->getTextureHeight(); 
                if (m_image.valid()) return m_image->t();
                return 0;
            };
        
            int depth() const
            {
                if (m_texture.valid()) return m_texture->getTextureDepth(); 
                if (m_image.valid()) return m_image->r();
                return 0;
            };

            GLenum              m_internalFormat;
            ref_ptr<crImage>      m_image;
            ref_ptr<crTexture>    m_texture;
            unsigned int        m_level;
            unsigned int        m_face;
            bool                m_mipMapGeneration;
            unsigned int        m_multisampleSamples;
            unsigned int        m_multisampleColorSamples;
        };
        
        typedef std::map< BufferComponent, Attachment> BufferAttachmentMap;
        
        /** Get the BufferAttachmentMap, used to configure frame buffer objects, pbuffers and texture reads.*/
        BufferAttachmentMap& getBufferAttachmentMap() { return m_bufferAttachmentMap; }

        /** Get the const BufferAttachmentMap, used to configure frame buffer objects, pbuffers and texture reads.*/
        const BufferAttachmentMap& getBufferAttachmentMap() const { return m_bufferAttachmentMap; }

        /** Set the crGraphicsContext that provides the mechansim for managing the OpenGL graphics context associated with this camera.*/
        void setGraphicsContext(crGraphicsContext* context);

        /** Get the crGraphicsContext.*/
        crGraphicsContext* getGraphicsContext() { return m_graphicsContext.get(); }

        /** Get the const crGraphicsContext.*/
        const crGraphicsContext* getGraphicsContext() const { return m_graphicsContext.get(); }
       
		/** Set the Rendering object that is used to implement rendering of the subgraph.*/
		void setRenderingCache(unsigned int contextID, CRCore::crBase* rc) { (*m_renderingCache)[contextID] = rc; }

		/** Get the Rendering object that is used to implement rendering of the subgraph.*/
		CRCore::crBase* getRenderingCache(unsigned int contextID) { return (*m_renderingCache)[contextID].get(); }

		/** Get the const Rendering object that is used to implement rendering of the subgraph.*/
		const CRCore::crBase* getRenderingCache(unsigned int contextID) const { return (*m_renderingCache)[contextID].get(); }

		CRCore::crBase* getBufRenderingCache(unsigned int contextID) { return (*m_renderingCache_buf)[contextID].get(); }
		const CRCore::crBase* getBufRenderingCache(unsigned int contextID) const { return (*m_renderingCache_buf)[contextID].get(); }
 
        /** Draw callback for custom operations.*/
		struct DrawCallback : public crBase
		{
			DrawCallback() {}

			DrawCallback(const DrawCallback&,const crCopyOp&) {}

			META_Base(CRCore,DrawCallback)

		    virtual void operator () (CRCore::crCameraNode& /*camera*/){}
		};

		/** Set the initial draw callback for custom operations to be done before the drawing of the camera's subgraph and pre render stages.*/
		void setInitialDrawCallback(DrawCallback* cb) { m_initialDrawCallback = cb; }

		/** Get the initial draw callback.*/
		DrawCallback* getInitialDrawCallback() { return m_initialDrawCallback.get(); }

		/** Get the const initial draw callback.*/
		const DrawCallback* getInitialDrawCallback() const { return m_initialDrawCallback.get(); }


		/** Set the pre draw callback for custom operations to be done before the drawing of the camera's subgraph but after any pre render stages have been completed.*/
		void setPreDrawCallback(DrawCallback* cb) { m_preDrawCallback = cb; }

		/** Get the pre draw callback.*/
		DrawCallback* getPreDrawCallback() { return m_preDrawCallback.get(); }

		/** Get the const pre draw callback.*/
		const DrawCallback* getPreDrawCallback() const { return m_preDrawCallback.get(); }


		/** Set the post draw callback for custom operations to be done after the drawing of the camera's subgraph but before the any post render stages have been completed.*/
		void setPostDrawCallback(DrawCallback* cb) { m_postDrawCallback = cb; }

		/** Get the post draw callback.*/
		DrawCallback* getPostDrawCallback() { return m_postDrawCallback.get(); }

		/** Get the const post draw callback.*/
		const DrawCallback* getPostDrawCallback() const { return m_postDrawCallback.get(); }


		/** Set the final draw callback for custom operations to be done after the drawing of the camera's subgraph and all of the post render stages has been completed.*/
		void setFinalDrawCallback(DrawCallback* cb) { m_finalDrawCallback = cb; }

		/** Get the final draw callback.*/
		DrawCallback* getFinalDrawCallback() { return m_finalDrawCallback.get(); }

		/** Get the const final draw callback.*/
		const DrawCallback* getFinalDrawCallback() const { return m_finalDrawCallback.get(); }


		crMutex* getDataChangeMutex() const { return &m_dataChangeMutex; }

        /** If crState is non-zero, this function releases any associated OpenGL objects for
           * the specified graphics context. Otherwise, releases OpenGL objexts
           * for all graphics contexts. */
        virtual void releaseObjects(CRCore::crState* state);

		inline void setShallRender(bool shallrender){ m_shallRender = shallrender; }
		inline bool getShallRender(){ return m_shallRender; }

		inline void addExternFrustum(const crPolytope& polytope) { m_externFrustumList.push_back(polytope); }
		inline void setExternFrustumList(const crCullingSet::FrustumList& efl) { m_externFrustumList = efl; }
		inline crCullingSet::FrustumList& getExternFrustumList() { return m_externFrustumList; }
		inline bool hasExternFrustum(){ return !m_externFrustumList.empty(); }
		inline void clearExternFrustum(){ m_externFrustumList.clear(); }

		inline void addExternOccluderFrustum(const crPolytope& polytope) { m_externOccluderFrustumList.push_back(polytope); }
		inline void setExternOccluderFrustumList(const crCullingSet::FrustumList& efl) { m_externOccluderFrustumList = efl; }
		inline crCullingSet::FrustumList& getExternOccluderFrustumList() { return m_externOccluderFrustumList; }
		inline bool hasExternOccluderFrustum(){ return !m_externOccluderFrustumList.empty(); }
		inline void clearExternOccluderFrustum(){ m_externOccluderFrustumList.clear(); }
		void exportImage(char progress);
		void exportImageBuf(char progress);
		inline char getExportImage() { return m_exportImage; }
		inline char getExportImageBuf() { return m_exportImage_buf; }
    public:

        /** crTransform method that must be defined to provide generic interface for scene graph traversals.*/
        virtual bool computeLocalToWorldMatrix(crMatrix& matrix,crNodeVisitor*) const;

        /** crTransform method that must be defined to provide generic interface for scene graph traversals.*/
        virtual bool computeWorldToLocalMatrix(crMatrix& matrix,crNodeVisitor*) const;


    protected :
    
        virtual ~crCameraNode();
		/** Set the view matrix. Can be thought of as setting the position of the world relative to the camera in camera coordinates. */
		inline void setViewInverseMatrix(const CRCore::crMatrixd& matrix) 
		{ 
			if(!m_viewInverseMatrix) m_viewInverseMatrix = new crMatrixd;
			m_viewInverseMatrix->set(matrix);
		}

		inline void setBufViewInverseMatrix(const CRCore::crMatrixd& matrix) 
		{
			if(!m_viewInverseMatrix_buf) m_viewInverseMatrix_buf = new crMatrixd;
			m_viewInverseMatrix_buf->set(matrix);
		}

        mutable crMutex          m_dataChangeMutex;

        GLbitfield                          m_clearMask;
        CRCore::crVector4                   m_clearColor;
        CRCore::crVector4                   m_clearAccum;
        double                              m_clearDepth;
        int                                 m_clearStencil;
        
        ref_ptr<crColorMask>                  m_colorMask;
        ref_ptr<crViewPort>                   m_viewport;

        TransformOrder                      m_transformOrder;

		mutable CRCore::ref_ptr<CRCore::crMatrixd>                     m_projectionMatrix;
		mutable CRCore::ref_ptr<CRCore::crMatrixd>                     m_viewMatrix;
		mutable CRCore::ref_ptr<CRCore::crMatrixd>                     m_projectionMatrix_buf;
		mutable CRCore::ref_ptr<CRCore::crMatrixd>                     m_viewMatrix_buf;
		mutable CRCore::ref_ptr<CRCore::crMatrixd>                     m_viewInverseMatrix;
		mutable CRCore::ref_ptr<CRCore::crMatrixd>                     m_viewInverseMatrix_buf;
        
        RenderOrder                         m_renderOrder;
        
        GLenum                              m_drawBuffer;
        GLenum                              m_readBuffer;
        
        RenderTargetImplementation          m_renderTargetImplementation;
        RenderTargetImplementation          m_renderTargetFallback;
        BufferAttachmentMap                 m_bufferAttachmentMap;
        
        ref_ptr<crGraphicsContext>            m_graphicsContext;
		ref_ptr<buffered_ptrobject< ref_ptr<crBase> > > m_renderingCache;
		ref_ptr<buffered_ptrobject< ref_ptr<crBase> > > m_renderingCache_buf;
        
		ref_ptr<DrawCallback>               m_initialDrawCallback;
		ref_ptr<DrawCallback>               m_preDrawCallback;
		ref_ptr<DrawCallback>               m_postDrawCallback;
		ref_ptr<DrawCallback>               m_finalDrawCallback;

		bool                         m_shallRender;
		crCullingSet::FrustumList    m_externFrustumList;
		crCullingSet::FrustumList    m_externOccluderFrustumList;
		char m_exportImage;
		char m_exportImage_buf;
};

}

#endif
