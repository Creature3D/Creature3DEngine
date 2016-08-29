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
#ifndef CRCORE_CRFRAMEBUFFEROBJECT_H
#define CRCORE_CRFRAMEBUFFEROBJECT_H 1

#include <crgl/GL.h>
#include <CRCore/crTexture.h>
#include <CRCore/buffered_value.h>
#include <CRCore/crCameraNode.h>

#ifndef GL_EXT_framebuffer_object
#define GL_EXT_framebuffer_object 1
#define GL_FRAMEBUFFER_EXT                     0x8D40
#define GL_RENDERBUFFER_EXT                    0x8D41
// #define GL_STENCIL_INDEX_EXT                   0x8D45  // removed in rev. #114 of the spec
#define GL_STENCIL_INDEX1_EXT                  0x8D46
#define GL_STENCIL_INDEX4_EXT                  0x8D47
#define GL_STENCIL_INDEX8_EXT                  0x8D48
#define GL_STENCIL_INDEX16_EXT                 0x8D49
#define GL_RENDERBUFFER_WIDTH_EXT              0x8D42
#define GL_RENDERBUFFER_HEIGHT_EXT             0x8D43
#define GL_RENDERBUFFER_INTERNAL_FORMAT_EXT    0x8D44
#define GL_RENDERBUFFER_RED_SIZE_EXT           0x8D50
#define GL_RENDERBUFFER_GREEN_SIZE_EXT         0x8D51
#define GL_RENDERBUFFER_BLUE_SIZE_EXT          0x8D52
#define GL_RENDERBUFFER_ALPHA_SIZE_EXT         0x8D53
#define GL_RENDERBUFFER_DEPTH_SIZE_EXT         0x8D54
#define GL_RENDERBUFFER_STENCIL_SIZE_EXT       0x8D55
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE_EXT            0x8CD0
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME_EXT            0x8CD1
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL_EXT          0x8CD2
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE_EXT  0x8CD3
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_3D_ZOFFSET_EXT     0x8CD4
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER_EXT          0x8CD4
#define GL_COLOR_ATTACHMENT0_EXT                0x8CE0
#define GL_COLOR_ATTACHMENT1_EXT                0x8CE1
#define GL_COLOR_ATTACHMENT2_EXT                0x8CE2
#define GL_COLOR_ATTACHMENT3_EXT                0x8CE3
#define GL_COLOR_ATTACHMENT4_EXT                0x8CE4
#define GL_COLOR_ATTACHMENT5_EXT                0x8CE5
#define GL_COLOR_ATTACHMENT6_EXT                0x8CE6
#define GL_COLOR_ATTACHMENT7_EXT                0x8CE7
#define GL_COLOR_ATTACHMENT8_EXT                0x8CE8
#define GL_COLOR_ATTACHMENT9_EXT                0x8CE9
#define GL_COLOR_ATTACHMENT10_EXT               0x8CEA
#define GL_COLOR_ATTACHMENT11_EXT               0x8CEB
#define GL_COLOR_ATTACHMENT12_EXT               0x8CEC
#define GL_COLOR_ATTACHMENT13_EXT               0x8CED
#define GL_COLOR_ATTACHMENT14_EXT               0x8CEE
#define GL_COLOR_ATTACHMENT15_EXT               0x8CEF
#define GL_DEPTH_ATTACHMENT_EXT                 0x8D00
#define GL_STENCIL_ATTACHMENT_EXT               0x8D20
#define GL_FRAMEBUFFER_COMPLETE_EXT                          0x8CD5
#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT             0x8CD6
#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT     0x8CD7
#define GL_FRAMEBUFFER_INCOMPLETE_DUPLICATE_ATTACHMENT_EXT   0x8CD8
#define GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT             0x8CD9
#define GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT                0x8CDA
#define GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT            0x8CDB
#define GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT            0x8CDC
#define GL_FRAMEBUFFER_UNSUPPORTED_EXT                       0x8CDD
#define GL_FRAMEBUFFER_BINDING_EXT             0x8CA6
#define GL_RENDERBUFFER_BINDING_EXT            0x8CA7
#define GL_MAX_COLOR_ATTACHMENTS_EXT           0x8CDF
#define GL_MAX_RENDERBUFFER_SIZE_EXT           0x84E8
#define GL_INVALID_FRAMEBUFFER_OPERATION_EXT   0x0506
#endif

#ifndef GL_EXT_framebuffer_blit
#define GL_EXT_framebuffer_blit 1
#define GL_DRAW_FRAMEBUFFER_BINDING_EXT 0x8CA6
#define GL_READ_FRAMEBUFFER_EXT 0x8CA8
#define GL_DRAW_FRAMEBUFFER_EXT 0x8CA9
#define GL_READ_FRAMEBUFFER_BINDING_EXT 0x8CAA
#endif

#ifndef GL_EXT_framebuffer_multisample
#define GL_EXT_framebuffer_multisample 1
#define GL_RENDERBUFFER_SAMPLES_EXT                 0x8CAB
#define GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE_EXT   0x8D56
#define GL_MAX_SAMPLES_EXT                          0x8D57
#endif

#ifndef GL_MAX_SAMPLES_EXT
// Workaround for Centos 5 and other distros that define
// GL_EXT_framebuffer_multisample but not GL_MAX_SAMPLES_EXT
#define GL_MAX_SAMPLES_EXT 0x8D57
#endif

#ifndef GL_NV_framebuffer_multisample_coverage
#define GL_NV_framebuffer_multisample_coverage 1
#define GL_RENDERBUFFER_COVERAGE_SAMPLES_NV     0x8CAB
#define GL_RENDERBUFFER_COLOR_SAMPLES_NV        0x8E10
#define GL_MAX_MULTISAMPLE_COVERAGE_MODES_NV    0x8E11
#define GL_MULTISAMPLE_COVERAGE_MODES_NV        0x8E12
#endif

#ifndef GL_DEPTH_COMPONENT
#define GL_DEPTH_COMPONENT              0x1902
#endif

#ifndef GL_VERSION_1_4
#define GL_DEPTH_COMPONENT16              0x81A5
#define GL_DEPTH_COMPONENT24              0x81A6
#define GL_DEPTH_COMPONENT32              0x81A7
#endif

#ifndef GL_DEPTH_COMPONENT32F
#define GL_DEPTH_COMPONENT32F             0x8CAC
#endif

#ifndef GL_DEPTH_COMPONENT32F_NV
#define GL_DEPTH_COMPONENT32F_NV          0x8DAB
#endif

#ifndef GL_EXT_packed_depth_stencil
#define GL_EXT_packed_depth_stencil 1
#define GL_DEPTH_STENCIL_EXT 0x84F9
#define GL_UNSIGNED_INT_24_8_EXT 0x84FA
#define GL_DEPTH24_STENCIL8_EXT 0x88F0
#define GL_TEXTURE_STENCIL_SIZE_EXT 0x88F1
#endif

namespace CRCore
{

/**************************************************************************
 * crFBOExtensions
 **************************************************************************/

    class CR_EXPORT crFBOExtensions : public CRCore::Referenced
    {
    public:
        typedef void APIENTRY TglBindRenderbufferEXT(GLenum, GLuint);
        typedef void APIENTRY TglDeleteRenderbuffersEXT(GLsizei n, const GLuint *renderbuffers);
        typedef void APIENTRY TglGenRenderbuffersEXT(GLsizei, GLuint *);
        typedef void APIENTRY TglRenderbufferStorageEXT(GLenum, GLenum, GLsizei, GLsizei);
        typedef void APIENTRY TglRenderbufferStorageMultisampleEXT(GLenum, GLsizei, GLenum, GLsizei, GLsizei);
        typedef void APIENTRY TglRenderbufferStorageMultisampleCoverageNV(GLenum, GLsizei, GLsizei, GLenum, GLsizei, GLsizei);
        typedef void APIENTRY TglBindFramebufferEXT(GLenum, GLuint);
        typedef void APIENTRY TglDeleteFramebuffersEXT(GLsizei n, const GLuint *framebuffers);
        typedef void APIENTRY TglGenFramebuffersEXT(GLsizei, GLuint *);
        typedef GLenum APIENTRY TglCheckFramebufferStatusEXT(GLenum);
        typedef void APIENTRY TglFramebufferTexture1DEXT(GLenum, GLenum, GLenum, GLuint, GLint);
        typedef void APIENTRY TglFramebufferTexture2DEXT(GLenum, GLenum, GLenum, GLuint, GLint);
        typedef void APIENTRY TglFramebufferTexture3DEXT(GLenum, GLenum, GLenum, GLuint, GLint, GLint);
        typedef void APIENTRY TglFramebufferTextureLayerEXT(GLenum, GLenum, GLuint, GLint, GLint);
        typedef void APIENTRY TglFramebufferRenderbufferEXT(GLenum, GLenum, GLenum, GLuint);
        typedef void APIENTRY TglGenerateMipmapEXT(GLenum);
        typedef void APIENTRY TglBlitFramebufferEXT(GLint, GLint, GLint, GLint, GLint, GLint, GLint, GLint, GLbitfield, GLenum);

        TglBindRenderbufferEXT* glBindRenderbufferEXT;
        TglGenRenderbuffersEXT* glGenRenderbuffersEXT;
        TglDeleteRenderbuffersEXT* glDeleteRenderbuffersEXT; 
        TglRenderbufferStorageEXT* glRenderbufferStorageEXT;
        TglRenderbufferStorageMultisampleEXT* glRenderbufferStorageMultisampleEXT;
        TglRenderbufferStorageMultisampleCoverageNV* glRenderbufferStorageMultisampleCoverageNV;
        TglBindFramebufferEXT* glBindFramebufferEXT;
        TglDeleteFramebuffersEXT* glDeleteFramebuffersEXT;
        TglGenFramebuffersEXT* glGenFramebuffersEXT;
        TglCheckFramebufferStatusEXT* glCheckFramebufferStatusEXT;
        TglFramebufferTexture1DEXT* glFramebufferTexture1DEXT;
        TglFramebufferTexture2DEXT* glFramebufferTexture2DEXT;
        TglFramebufferTexture3DEXT* glFramebufferTexture3DEXT;
        TglFramebufferTextureLayerEXT* glFramebufferTextureLayerEXT;
        TglFramebufferRenderbufferEXT* glFramebufferRenderbufferEXT;
        TglGenerateMipmapEXT* glGenerateMipmapEXT;
        TglBlitFramebufferEXT* glBlitFramebufferEXT;

        static crFBOExtensions* instance(unsigned contextID, bool createIfNotInitalized);
		static void clearExtensions();
        bool isSupported() const { return m_supported; }
        bool isMultisampleSupported() const { return glRenderbufferStorageMultisampleEXT != 0; }
        bool isMultisampleCoverageSupported() const { return glRenderbufferStorageMultisampleCoverageNV != 0; }

    protected:
        crFBOExtensions(unsigned int contextID);

    private:        
        bool m_supported;
    };

/**************************************************************************
 * crRenderBuffer
 **************************************************************************/

    class CR_EXPORT crRenderBuffer: public crBase
    {
    public:
        crRenderBuffer();
        crRenderBuffer(int width, int height, GLenum internalFormat, int samples=0, int colorSamples=0);
        crRenderBuffer(const crRenderBuffer& copy, const crCopyOp& copyop = crCopyOp::SHALLOW_COPY);

        META_Base(CRCore, crRenderBuffer);

        inline int getWidth() const;
        inline int getHeight() const;
        inline void setWidth(int w);
        inline void setHeight(int h);
        inline void setSize(int w, int h);
        inline GLenum getInternalFormat() const;
        inline void setInternalFormat(GLenum format);
        inline int getSamples() const;
        inline int getColorSamples() const;
        inline void setSamples(int samples);
        inline void setColorSamples(int colorSamples);

        GLuint getObjectID(unsigned int contextID, const crFBOExtensions *ext) const;
        inline int compare(const crRenderBuffer &rb) const;

        /** Mark internal crRenderBuffer for deletion.
          * Deletion requests are queued until they can be executed
          * in the proper GL context. */
        static void deleteRenderBuffer(unsigned int contextID, GLuint rb);

        /** flush all the cached RenderBuffers which need to be deleted
          * in the OpenGL context related to contextID.*/
        static void flushDeletedRenderBuffers(unsigned int contextID,double currentTime, double& availableTime);
		static void flushAllDeletedRenderBuffers(unsigned int contextID);

        /** discard all the cached RenderBuffers which need to be deleted in the OpenGL context related to contextID.
          * Note, unlike flush no OpenGL calls are made, instead the handles are all removed.
          * this call is useful for when an OpenGL context has been destroyed. */
        static void discardDeletedRenderBuffers(unsigned int contextID);

        static int getMaxSamples(unsigned int contextID, const crFBOExtensions *ext);
		virtual void releaseObjects(crState *state);
    protected:
        virtual ~crRenderBuffer();
        crRenderBuffer &operator=(const crRenderBuffer &) { return *this; }

        inline void dirtyAll() const;

    private:
        mutable buffered_value<GLuint> m_objectID;
        mutable buffered_value<int> m_dirty;

        GLenum m_internalFormat;
        int m_width;
        int m_height;
        // "samples" in the framebuffer_multisample extension is equivalent to
        // "coverageSamples" in the framebuffer_multisample_coverage extension.
        int m_samples;
        int m_colorSamples;
    };

    // INLINE METHODS

    inline int crRenderBuffer::getWidth() const
    {
        return m_width;
    }

    inline int crRenderBuffer::getHeight() const
    {
        return m_height;
    }

    inline void crRenderBuffer::setWidth(int w)
    {
        m_width = w;
        dirtyAll();
    }

    inline void crRenderBuffer::setHeight(int h)
    {
        m_height = h;
        dirtyAll();
    }

    inline void crRenderBuffer::setSize(int w, int h)
    {
        m_width = w;
        m_height = h;
        dirtyAll();
    }

    inline GLenum crRenderBuffer::getInternalFormat() const
    {
        return m_internalFormat;
    }

    inline void crRenderBuffer::setInternalFormat(GLenum format)
    {
        m_internalFormat = format;
        dirtyAll();
    }

    inline int crRenderBuffer::getSamples() const
    {
        return m_samples;
    }

    inline int crRenderBuffer::getColorSamples() const
    {
        return m_colorSamples;
    }

    inline void crRenderBuffer::setSamples(int samples)
    {
        m_samples = samples;
        dirtyAll();
    }

    inline void crRenderBuffer::setColorSamples(int colorSamples)
    {
        m_colorSamples = colorSamples;
        dirtyAll();
    }

    inline void crRenderBuffer::dirtyAll() const
    {
        m_dirty.setAllElementsTo(1);
    }

    inline int crRenderBuffer::compare(const crRenderBuffer &rb) const
    {
        if (&rb == this) return 0;
        if (m_internalFormat < rb.m_internalFormat) return -1;
        if (m_internalFormat > rb.m_internalFormat) return 1;
        if (m_width < rb.m_width) return -1;
        if (m_width > rb.m_width) return 1;
        if (m_height < rb.m_height) return -1;
        if (m_height > rb.m_height) return 1;
        return 0;
    }

/**************************************************************************
 * FrameBufferAttachement
 **************************************************************************/
    class crTexture1D;
    class crTexture2D;
	class crTexture2DMultisample;
    class crTexture3D;
    class crTexture2DArray;
    class crTextureCubeMap;
    class crTextureRectangle;

    class CR_EXPORT crFrameBufferAttachment
    {
    public:
        crFrameBufferAttachment();
        crFrameBufferAttachment(const crFrameBufferAttachment& copy);

        explicit crFrameBufferAttachment(crRenderBuffer* target);
        explicit crFrameBufferAttachment(crTexture1D* target, int level = 0);
        explicit crFrameBufferAttachment(crTexture2D* target, int level = 0);
		explicit crFrameBufferAttachment(crTexture2DMultisample* target, unsigned int level = 0);
        explicit crFrameBufferAttachment(crTexture3D* target, int zoffset, int level = 0);
        explicit crFrameBufferAttachment(crTexture2DArray* target, int layer, int level = 0);
        explicit crFrameBufferAttachment(crTextureCubeMap* target, int face, int level = 0);
        explicit crFrameBufferAttachment(crTextureRectangle* target);
        explicit crFrameBufferAttachment(crCameraNode::Attachment& attachment);
        
        ~crFrameBufferAttachment();

        crFrameBufferAttachment&operator = (const crFrameBufferAttachment& copy);        

        bool isMultisample() const;
        void createRequiredTexturesAndApplyGenerateMipMap(crState& state, const crFBOExtensions* ext) const;
        void attach(crState &state, GLenum target, GLenum attachment_point, const crFBOExtensions* ext) const;
        int compare(const crFrameBufferAttachment &fa) const;
		void releaseObjects(CRCore::crState* state=0);
    private:
        // use the Pimpl idiom to avoid dependency from
        // all crTexture* headers
        struct Pimpl;
        Pimpl* m_ximpl;
    };

/**************************************************************************
 * crFrameBufferObject
 **************************************************************************/
    class CR_EXPORT crFrameBufferObject: public crStateAttribute
    {
    public:
        typedef std::map<crCameraNode::BufferComponent, crFrameBufferAttachment> AttachmentMap;
        typedef std::vector<GLenum> MultipleRenderingTargets;
        
        typedef crCameraNode::BufferComponent BufferComponent;

        crFrameBufferObject();
        crFrameBufferObject(const crFrameBufferObject& copy, const crCopyOp& copyop = crCopyOp::SHALLOW_COPY);

        META_StateAttribute(CRCore, crFrameBufferObject, FBO/*(crStateAttribute::Type)0x101010*//*crFrameBufferObject*/);

        inline const AttachmentMap& getAttachmentMap() const;
        

        void setAttachment(BufferComponent attachment_point, const crFrameBufferAttachment &attachment);
        inline const crFrameBufferAttachment& getAttachment(BufferComponent attachment_point) const;
        inline bool hasAttachment(BufferComponent attachment_point) const;
        
        inline bool hasMultipleRenderingTargets() const { return !m_drawBuffers.empty(); }
        inline const MultipleRenderingTargets& getMultipleRenderingTargets() const { return m_drawBuffers; }

        bool isMultisample() const;

        int compare(const crStateAttribute &sa) const;
		int compare(const crStateAttribute &sa);

        void apply(crState &state) const;

        enum BindTarget
        {
            READ_FRAMEBUFFER = GL_READ_FRAMEBUFFER_EXT,
            DRAW_FRAMEBUFFER = GL_DRAW_FRAMEBUFFER_EXT,
            READ_DRAW_FRAMEBUFFER = GL_FRAMEBUFFER_EXT
        };

        /** Bind the FBO as either the read or draw target, or both. */
        void apply(crState &state, BindTarget target) const;

		virtual void releaseObjects(CRCore::crState* state=0);

        /** Mark internal FBO for deletion.
          * Deletion requests are queued until they can be executed
          * in the proper GL context. */
        static void deleteFrameBufferObject(unsigned int contextID, GLuint program);

        /** flush all the cached FBOs which need to be deleted
          * in the OpenGL context related to contextID.*/
        static void flushDeletedFrameBufferObjects(unsigned int contextID,double currentTime, double& availableTime);
		static void flushAllDeletedFrameBufferObjects(unsigned int contextID);
        /** discard all the cached FBOs which need to be deleted
          * in the OpenGL context related to contextID.*/
        static void discardDeletedFrameBufferObjects(unsigned int contextID);

    protected:
        virtual ~crFrameBufferObject();
        crFrameBufferObject& operator = (const crFrameBufferObject&) { return *this; }

        void updateDrawBuffers();
        
        inline void dirtyAll();

        GLenum convertBufferComponentToGLenum(BufferComponent attachment_point) const;

    private:        
        mutable AttachmentMap               m_attachments;

        // Buffers passed to glDrawBuffers when using multiple render targets.
        MultipleRenderingTargets    m_drawBuffers;

        mutable buffered_value<int>         m_dirtyAttachmentList;
        mutable buffered_value<int>         m_unsupported;
        mutable buffered_value<GLuint>      m_fboID;
        
    };

    // INLINE METHODS

    inline const crFrameBufferObject::AttachmentMap &crFrameBufferObject::getAttachmentMap() const
    {
        return m_attachments;
    }

    inline bool crFrameBufferObject::hasAttachment(crFrameBufferObject::BufferComponent attachment_point) const
    {
        return m_attachments.find(attachment_point) != m_attachments.end();
    }

    inline const crFrameBufferAttachment &crFrameBufferObject::getAttachment(crFrameBufferObject::BufferComponent attachment_point) const
    {
        return m_attachments.find(attachment_point)->second;
    }

    inline void crFrameBufferObject::dirtyAll()
    {
        m_dirtyAttachmentList.setAllElementsTo(1);
    }


}

#endif

