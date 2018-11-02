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
#ifndef CRCORE_CRBUFFEROBJECT
#define CRCORE_CRBUFFEROBJECT 1

#include <crgl/GL.h>
#include <CRCore/crBase.h>
#include <CRCore/buffered_value.h>

#ifndef GL_ARB_vertex_buffer_object

#define GL_ARB_vertex_buffer_object

// for compatibility with gl.h headers that don't support VBO,
#if defined(_WIN64)
typedef __int64 GLintptrARB;
typedef __int64 GLsizeiptrARB;
#elif defined(__ia64__) || defined(__x86_64__)
typedef long int GLintptrARB;
typedef long int GLsizeiptrARB;
#else
typedef int GLintptrARB;
typedef int GLsizeiptrARB;
#endif

#define GL_ARRAY_BUFFER_ARB               0x8892
#define GL_ELEMENT_ARRAY_BUFFER_ARB       0x8893
#define GL_ARRAY_BUFFER_BINDING_ARB       0x8894
#define GL_ELEMENT_ARRAY_BUFFER_BINDING_ARB 0x8895
#define GL_VERTEX_ARRAY_BUFFER_BINDING_ARB 0x8896
#define GL_NORMAL_ARRAY_BUFFER_BINDING_ARB 0x8897
#define GL_COLOR_ARRAY_BUFFER_BINDING_ARB 0x8898
#define GL_INDEX_ARRAY_BUFFER_BINDING_ARB 0x8899
#define GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING_ARB 0x889A
#define GL_EDGE_FLAG_ARRAY_BUFFER_BINDING_ARB 0x889B
#define GL_SECONDARY_COLOR_ARRAY_BUFFER_BINDING_ARB 0x889C
#define GL_FOG_COORDINATE_ARRAY_BUFFER_BINDING_ARB 0x889D
#define GL_WEIGHT_ARRAY_BUFFER_BINDING_ARB 0x889E
#define GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING_ARB 0x889F
#define GL_STREAM_DRAW_ARB                0x88E0
#define GL_STREAM_READ_ARB                0x88E1
#define GL_STREAM_COPY_ARB                0x88E2
#define GL_STATIC_DRAW_ARB                0x88E4
#define GL_STATIC_READ_ARB                0x88E5
#define GL_STATIC_COPY_ARB                0x88E6
#define GL_DYNAMIC_DRAW_ARB               0x88E8
#define GL_DYNAMIC_READ_ARB               0x88E9
#define GL_DYNAMIC_COPY_ARB               0x88EA
#define GL_READ_ONLY_ARB                  0x88B8
#define GL_WRITE_ONLY_ARB                 0x88B9
#define GL_READ_WRITE_ARB                 0x88BA
#define GL_BUFFER_SIZE_ARB                0x8764
#define GL_BUFFER_USAGE_ARB               0x8765
#define GL_BUFFER_ACCESS_ARB              0x88BB
#define GL_BUFFER_MAPPED_ARB              0x88BC
#define GL_BUFFER_MAP_POINTER_ARB         0x88BD

#endif

#ifndef GL_VERSION_1_5
#define GL_STREAM_DRAW                    0x88E0
#define GL_STREAM_READ                    0x88E1
#define GL_STREAM_COPY                    0x88E2
#define GL_STATIC_DRAW                    0x88E4
#define GL_STATIC_READ                    0x88E5
#define GL_STATIC_COPY                    0x88E6
#define GL_DYNAMIC_DRAW                   0x88E8
#define GL_DYNAMIC_READ                   0x88E9
#define GL_DYNAMIC_COPY                   0x88EA
#endif

#ifndef GL_VERSION_2_1
#define GL_PIXEL_PACK_BUFFER              0x88EB
#define GL_PIXEL_UNPACK_BUFFER            0x88EC
#define GL_PIXEL_PACK_BUFFER_BINDING      0x88ED
#define GL_PIXEL_UNPACK_BUFFER_BINDING    0x88EF
#endif


#ifndef GL_ARB_pixel_buffer_object
#define GL_PIXEL_PACK_BUFFER_ARB            0x88EB
#define GL_PIXEL_UNPACK_BUFFER_ARB          0x88EC
#define GL_PIXEL_PACK_BUFFER_BINDING_ARB    0x88ED
#define GL_PIXEL_UNPACK_BUFFER_BINDING_ARB  0x88EF
#endif

namespace CRCore 
{

class crState;

class CR_EXPORT crBufferObject : public crBase
{
    public:
		static unsigned int s_numberIBOs;
        crBufferObject();

        /** Copy constructor using crCopyOp to manage deep vs shallow copy.*/
        crBufferObject(const crBufferObject& bo,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY);

        virtual bool isSameKindAs(const crBase* obj) const { return dynamic_cast<const crBufferObject*>(obj)!=NULL; }
        virtual const char* libraryName() const { return "CRCore"; }
        virtual const char* className() const { return "crBufferObject"; }

        struct BufferEntry
        {
            BufferEntry(): dataSize(0),offset(0) {}
            BufferEntry(const BufferEntry& be): modifiedCount(be.modifiedCount),dataSize(be.dataSize),offset(be.offset) {}
        
            BufferEntry& operator = (const BufferEntry& be) { modifiedCount=be.modifiedCount; dataSize=be.dataSize; offset=be.offset; return *this; }

            mutable buffered_value<unsigned int>    modifiedCount;
            unsigned int                            dataSize;
            unsigned int                            offset;
        };

        inline bool isBufferObjectSupported(unsigned int contextID) const { return getExtensions(contextID,true)->isBufferObjectSupported(); }
		inline bool isPBOSupported(unsigned int contextID) const { return getExtensions(contextID,true)->isPBOSupported(); }

        inline GLuint& buffer(unsigned int contextID) const { return m_bufferObjectList[contextID]; }
        
        inline void bindBuffer(unsigned int contextID) const
        { 
            Extensions* extensions = getExtensions(contextID,true);
            extensions->glBindBuffer(m_target,m_bufferObjectList[contextID]);
        }

        inline void unbindBuffer(unsigned int contextID) const
        { 
            Extensions* extensions = getExtensions(contextID,true);
            extensions->glBindBuffer(m_target,0);
        }

        virtual bool needsCompile(unsigned int contextID) const = 0;

        virtual void compileBuffer(crState& state) const = 0;
        
		/** Resize any per context GLObject buffers to specified size. */
		//virtual void resizeGLObjectBuffers(unsigned int maxSize);

        void releaseBuffer(crState* state) const;


        /** Use deleteVertexBufferObject instead of glDeleteBuffers to allow
          * OpenGL buffer objects to be cached until they can be deleted
          * by the OpenGL context in which they were created, specified
          * by contextID.*/
        static void deleteBufferObject(unsigned int contextID,GLuint globj);

        /** flush all the cached display list which need to be deleted
          * in the OpenGL context related to contextID.*/
        static void flushDeletedBufferObjects(unsigned int contextID,double /*currentTime*/, double& availableTime);
		static void flushAllDeletedBufferObjects(unsigned int contextID);

		//static void discardDeletedBufferObjects(unsigned int contextID);

        /** Extensions class which encapsulates the querying of extensions and
        * associated function pointers, and provide convenience wrappers to
        * check for the extensions or use the associated functions.*/
        class Extensions : public CRCore::Referenced
        {
        public:
            Extensions(unsigned int contextID);

            Extensions(const Extensions& rhs);

            void lowestCommonDenominator(const Extensions& rhs);

            void setupGLExtenions(unsigned int contextID);

            bool isBufferObjectSupported() const { return m_glGenBuffers!=0; }
			bool isPBOSupported() const { return m_isPBOSupported; }

            void glGenBuffers (GLsizei n, GLuint *buffers) const;
            void glBindBuffer (GLenum target, GLuint buffer) const;
            void glBufferData (GLenum target, GLsizeiptrARB size, const GLvoid *data, GLenum usage) const;
            void glBufferSubData (GLenum target, GLintptrARB offset, GLsizeiptrARB size, const GLvoid *data) const;
            void glDeleteBuffers (GLsizei n, const GLuint *buffers) const;
            GLboolean glIsBuffer (GLuint buffer) const;
            void glGetBufferSubData (GLenum target, GLintptrARB offset, GLsizeiptrARB size, GLvoid *data) const;
            GLvoid* glMapBuffer (GLenum target, GLenum access) const;
            GLboolean glUnmapBuffer (GLenum target) const;
            void glGetBufferParameteriv (GLenum target, GLenum pname, GLint *params) const;
            void glGetBufferPointerv (GLenum target, GLenum pname, GLvoid* *params) const;
			static void clearExtensions();
        protected:

            typedef void (APIENTRY * GenBuffersProc) (GLsizei n, GLuint *buffers);
            typedef void (APIENTRY * BindBufferProc) (GLenum target, GLuint buffer);
            typedef void (APIENTRY * BufferDataProc) (GLenum target, GLsizeiptrARB size, const GLvoid *data, GLenum usage);
            typedef void (APIENTRY * BufferSubDataProc) (GLenum target, GLintptrARB offset, GLsizeiptrARB size, const GLvoid *data);
            typedef void (APIENTRY * DeleteBuffersProc) (GLsizei n, const GLuint *buffers);
            typedef GLboolean (APIENTRY * IsBufferProc) (GLuint buffer);
            typedef void (APIENTRY * GetBufferSubDataProc) (GLenum target, GLintptrARB offset, GLsizeiptrARB size, GLvoid *data);
            typedef GLvoid* (APIENTRY * MapBufferProc) (GLenum target, GLenum access);
            typedef GLboolean (APIENTRY * UnmapBufferProc) (GLenum target);
            typedef void (APIENTRY * GetBufferParameterivProc) (GLenum target, GLenum pname, GLint *params);
            typedef void (APIENTRY * GetBufferPointervProc) (GLenum target, GLenum pname, GLvoid* *params);

            GenBuffersProc          m_glGenBuffers;
            BindBufferProc          m_glBindBuffer;
            BufferDataProc          m_glBufferData;
            BufferSubDataProc       m_glBufferSubData;
            DeleteBuffersProc       m_glDeleteBuffers;
            IsBufferProc            m_glIsBuffer;
            GetBufferSubDataProc    m_glGetBufferSubData;
            MapBufferProc           m_glMapBuffer;
            UnmapBufferProc         m_glUnmapBuffer;
            GetBufferParameterivProc m_glGetBufferParameteriv;
            GetBufferPointervProc   m_glGetBufferPointerv;
			bool m_isPBOSupported;
        };

        /** Function to call to get the extension of a specified context.
        * If the Exentsion object for that context has not yet been created  
        * and the 'createIfNotInitalized' flag been set to false then returns NULL.
        * If 'createIfNotInitalized' is true then the Extensions object is 
        * automatically created.  However, in this case the extension object is
        * only created with the graphics context associated with ContextID..*/
        static Extensions* getExtensions(unsigned int contextID,bool createIfNotInitalized);

        /** setExtensions allows users to override the extensions across graphics contexts.
        * typically used when you have different extensions supported across graphics pipes
        * but need to ensure that they all use the same low common denominator extensions.*/
        static void setExtensions(unsigned int contextID,Extensions* extensions);

    protected:
    
        virtual ~crBufferObject();
        
        typedef CRCore::buffered_value<GLuint> GLObjectList;

        typedef std::pair< BufferEntry, ref_ptr<Array> >        BufferEntryArrayPair;
        typedef std::pair< BufferEntry, ref_ptr<crPrimitive> > BufferEntryPrimitiveSetPair;
        
        mutable GLObjectList    m_bufferObjectList;

        GLenum                  m_target;
        GLenum                  m_usage;
        mutable unsigned int    m_totalSize;
};
/*
class Array;
class CR_EXPORT crVertexBufferObject : public crBufferObject
{
public:

	crVertexBufferObject();

	crVertexBufferObject(const crVertexBufferObject& vbo,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY);

	META_Base(CRCore,crVertexBufferObject);

	typedef std::pair< BufferEntry, Array* > BufferEntryArrayPair;
	typedef std::vector< BufferEntryArrayPair > BufferEntryArrayPairs;

	unsigned int addArray(CRCore::Array* array);
	void removeArray(CRCore::Array* array);

	void setArray(unsigned int i, Array* array);
	Array* getArray(unsigned int i) { return m_bufferEntryArrayPairs[i].second; }
	const Array* getArray(unsigned int i) const { return m_bufferEntryArrayPairs[i].second; }

	const GLvoid* getOffset(unsigned int i) const { return (const GLvoid*)(((char *)0)+(m_bufferEntryArrayPairs[i].first.offset)); }

	virtual bool needsCompile(unsigned int contextID) const { return true; }
	virtual void compileBuffer(crState& state) const;

	virtual void resizeGLObjectBuffers(unsigned int maxSize);

protected:

	virtual ~crVertexBufferObject();

	BufferEntryArrayPairs m_bufferEntryArrayPairs;
};

class crPrimitive;
class CR_EXPORT crElementBufferObject : public crBufferObject
{
public:

	crElementBufferObject();

	crElementBufferObject(const crElementBufferObject& pbo,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY);

	META_Base(CRCore,crElementBufferObject);

	typedef std::pair< BufferEntry, crPrimitive* > BufferEntryDrawElementsPair;
	typedef std::vector< BufferEntryDrawElementsPair > BufferEntryDrawElementsPairs;

	unsigned int addDrawElements(CRCore::crPrimitive* PrimitiveSet);
	void removeDrawElements(CRCore::crPrimitive* PrimitiveSet);

	void setDrawElements(unsigned int i, crPrimitive* PrimitiveSet);
	crPrimitive* getDrawElements(unsigned int i) { return m_bufferEntryDrawElementsPairs[i].second; }
	const crPrimitive* getDrawElements(unsigned int i) const { return m_bufferEntryDrawElementsPairs[i].second; }

	const GLvoid* getOffset(unsigned int i) const { return (const GLvoid*)(((char *)0)+(m_bufferEntryDrawElementsPairs[i].first.offset)); }

	virtual bool needsCompile(unsigned int contextID) const { return true; }
	virtual void compileBuffer(crState& state) const;

	virtual void resizeGLObjectBuffers(unsigned int maxSize);

protected:

	virtual ~crElementBufferObject();

	BufferEntryDrawElementsPairs m_bufferEntryDrawElementsPairs;
};
*/
class crImage;
class CR_EXPORT crPixelBufferObject : public crBufferObject
{
    public:

        crPixelBufferObject(CRCore::crImage* image=0);

        /** Copy constructor using crCopyOp to manage deep vs shallow copy.*/
        crPixelBufferObject(const crPixelBufferObject& pbo,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY);

        META_Base(CRCore,crPixelBufferObject);
        
        typedef std::pair< BufferEntry, crImage* > BufferEntryImagePair;

        void setImage(CRCore::crImage* image);

        crImage* getImage() { return m_bufferEntryImagePair.second; }
        const crImage* getImage() const { return m_bufferEntryImagePair.second; }
        
        unsigned int offset() const { return m_bufferEntryImagePair.first.offset; }
        
        virtual bool needsCompile(unsigned int contextID) const;

        virtual void compileBuffer(crState& state) const;
		/** Resize any per context GLObject buffers to specified size. */
		//virtual void resizeGLObjectBuffers(unsigned int maxSize);

    protected:
    
        virtual ~crPixelBufferObject();
        
        BufferEntryImagePair m_bufferEntryImagePair;
};


}

#endif
