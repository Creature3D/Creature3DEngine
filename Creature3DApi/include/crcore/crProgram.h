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
#ifndef CRCORE_CRPROGRAM
#define CRCORE_CRPROGRAM 1

#include <list>
#include <string>
#include <vector>
#include <map>

#include <CRCore/buffered_value.h>
#include <CRCore/ref_ptr.h>
#include <CRCore/crUniform.h>
#include <CRCore/crShader.h>
#include <CRCore/crStateAttribute.h>
#include <CRCore/thread/crMutex.h>

namespace CRCore {

class crState;

class CR_EXPORT crProgram : public CRCore::crStateAttribute
{
    public:
        crProgram();

        /** Copy constructor using crCopyOp to manage deep vs shallow copy.*/
        crProgram(const crProgram& rhs, const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);

        META_StateAttribute(CRCore, crProgram, PROGRAM);

        /** return -1 if *this < *rhs, 0 if *this==*rhs, 1 if *this>*rhs.*/
        virtual int compare(const CRCore::crStateAttribute& sa) const;
		virtual int compare(const CRCore::crStateAttribute& sa);

        /** If enabled, activate our program in the GL pipeline,
          * performing any rebuild operations that might be pending. */
        virtual void apply(CRCore::crState& state) const;

        /** Set whether to use a mutex to ensure ref() and unref() are thread safe.*/
        //virtual void setThreadSafeRefUnref(bool threadSafe);

        /** Compile program and associated shaders.*/
        void compile(CRCore::crState& state) const;

        /** Resize any per context GLObject buffers to specified size. */
        //virtual void resizeGLObjectBuffers(unsigned int maxSize);

        /** release OpenGL objects in specified graphics context if crState
            object is passed, otherwise release OpenGL objects for all graphics context if
            crState object pointer NULL.*/
        virtual void releaseObjects(CRCore::crState* state=0);

        /** Mark our PCSOs as needing relink */
        void dirtyProgram();

        /** Attach an CRCore::crShader to this CRCore::crProgram.
          * Mark crProgram as needing relink.  Return true for success */
		bool addShader( crShader* shader );

		unsigned int getNumShaders() const { return m_shaderList.size(); }

        crShader* getShader( unsigned int i ) { return m_shaderList[i].get(); }
        const crShader* getShader( unsigned int i ) const { return m_shaderList[i].get(); }

        /** Remove CRCore::crShader from this CRCore::crProgram.
          * Mark crProgram as needing relink.  Return true for success */
        bool removeShader( crShader* shader );

        /** Set/get GL program parameters */
        void setParameter( GLenum pname, GLint value );
        GLint getParameter( GLenum pname ) const;

        /** Add an attribute location binding. */
        void addBindAttribLocation( const std::string& name, GLuint index );
        
        /** Remove an attribute location binding. */
        void removeBindAttribLocation( const std::string& name );

        /** Add an frag data location binding. See EXT_gpu_shader4 for BindFragDataLocationEXT */
        void addBindFragDataLocation( const std::string& name, GLuint index );

        /** Remove an frag data location binding. */
        void removeBindFragDataLocation( const std::string& name );

        typedef std::map<std::string,GLuint> AttribBindingList;
        typedef std::map<std::string,GLuint> FragDataBindingList;

        const AttribBindingList& getAttribBindingList() const { return m_attribBindingList; }
        const FragDataBindingList& getFragDataBindingList() const { return m_fragDataBindingList; }

        /** Return true if this crProgram represents "fixed-functionality" rendering */
        bool isFixedFunction() const;

        /** Query InfoLog from a glProgram */
        bool getGlProgramInfoLog(unsigned int contextID, std::string& log) const;

        /** Mark internal glProgram for deletion.
          * Deletion requests are queued until they can be executed
          * in the proper GL context. */
        static void deleteGlProgram(unsigned int contextID, GLuint program);

        /** flush all the cached glPrograms which need to be deleted
          * in the OpenGL context related to contextID.*/
        static void flushDeletedGlPrograms(unsigned int contextID,double currentTime, double& availableTime);
		static void flushAllDeletedGlPrograms(unsigned int contextID);

        /** discard all the cached glPrograms which need to be deleted
          * in the OpenGL context related to contextID.
          * Note, unlike flush no OpenGL calls are made, instead the handles are all removed.
          * this call is useful for when an OpenGL context has been destroyed. */
        static void discardDeletedGlPrograms(unsigned int contextID);

        struct ActiveVarInfo {
            ActiveVarInfo() : m_location(-1), m_type(crUniform::UNDEFINED), m_size(-1) {}
            ActiveVarInfo( GLint loc, GLenum type, GLint size ) : m_location(loc), m_type(type), m_size(size) {}
            GLint m_location;
            GLenum m_type;
            GLint m_size;
        };
        typedef std::map< std::string, ActiveVarInfo > ActiveVarInfoMap;
        const ActiveVarInfoMap& getActiveUniforms(unsigned int contextID) const;
        const ActiveVarInfoMap& getActiveAttribs(unsigned int contextID) const;

		typedef std::list<unsigned int> GlProgramHandleList;
		typedef std::map<unsigned int, GlProgramHandleList> DeletedGlProgramCache;

    public:

        // make PerContextProgram a friend to allow it access crProgram's protected
        // methods and member variables.
        class PerContextProgram;
        friend class PerContextProgram;

        /** PerContextProgram (PCP) is an OSG-internal encapsulation of glPrograms per-GL context.  */
        class CR_EXPORT PerContextProgram : public CRCore::Referenced
        {
            public:
                PerContextProgram(const crProgram* program, unsigned int contextID);

                GLuint getHandle() const {return m_glProgramHandle;}

                void requestLink();
                void linkProgram();
                bool validateProgram();
                bool needsLink() const {return m_needsLink;}
                bool isLinked() const {return m_isLinked;}
                bool getInfoLog( std::string& infoLog ) const;

                void useProgram() const;

                void resetAppliedUniforms() const
                {
                    for(LastAppliedUniformMap::iterator itr=m_lastAppliedUniformMap.begin();
                        itr!=m_lastAppliedUniformMap.end();
                        ++itr)
                    {
                        itr->second.first = 0;
                        itr->second.second = 0;
                    }
                }
                
                
                inline void apply(const crUniform& uniform) const
                {
                    GLint location = getUniformLocation(uniform.getName());
                    if (location>=0)
                    {
						uniform.apply(m_extensions.get(),location);
						m_lastAppliedUniformMap[location].first = &uniform;
                        ////if ((unsigned int)location>=m_lastAppliedUniformList.size()) m_lastAppliedUniformList.resize(location+1);
                        //const crUniform* lastAppliedUniform = m_lastAppliedUniformMap[location].first.get();
                        //if (lastAppliedUniform != &uniform)
                        //{
                        //    // new attribute
                        //    uniform.apply(m_extensions.get(),location);
                        //    m_lastAppliedUniformMap[location].first = &uniform;
                        //    m_lastAppliedUniformMap[location].second = uniform.getModifiedCount();
                        //}
                        //else if (m_lastAppliedUniformMap[location].second != uniform.getModifiedCount())
                        //{
                        //    // existing attribute has been modified
                        //    uniform.apply(m_extensions.get(),location);
                        //    m_lastAppliedUniformMap[location].first = &uniform;
                        //    m_lastAppliedUniformMap[location].second = uniform.getModifiedCount();
                        //}
                    }
                }

                const ActiveVarInfoMap& getActiveUniforms() const {return m_uniformInfoMap;}
                const ActiveVarInfoMap& getActiveAttribs() const {return m_attribInfoMap;}

                inline GLint getUniformLocation( const std::string& name ) const { ActiveVarInfoMap::const_iterator itr = m_uniformInfoMap.find(name); return (itr!=m_uniformInfoMap.end()) ? itr->second.m_location : -1; }
                inline GLint getAttribLocation( const std::string& name ) const { ActiveVarInfoMap::const_iterator itr = m_attribInfoMap.find(name); return (itr!=m_attribInfoMap.end()) ? itr->second.m_location : -1; }

                inline void addShaderToAttach(crShader *shader)
                {
                    m_shadersToAttach.push_back(shader);
                }

                inline void addShaderToDetach(crShader *shader)
                {
                    m_shadersToDetach.push_back(shader);
                }
      
            protected:        /*methods*/
                ~PerContextProgram();

            protected:        /*data*/
                /** Pointer to our parent crProgram */
                const crProgram* m_program;
                /** Pointer to this context's extension functions */
                CRCore::ref_ptr<GL2Extensions> m_extensions;
                /** Handle to the actual OpenGL glProgram */
                GLuint m_glProgramHandle;
                /** Does our glProgram need to be linked? */
                bool m_needsLink;
                /** Is our glProgram successfully linked? */
                bool m_isLinked;
                const unsigned int m_contextID;

                ActiveVarInfoMap m_uniformInfoMap;
                ActiveVarInfoMap m_attribInfoMap;
                
                typedef std::pair<CRCore::ref_ptr<const CRCore::crUniform>, unsigned int> UniformModifiedCountPair;
                typedef std::map<GLint,UniformModifiedCountPair> LastAppliedUniformMap;
                mutable LastAppliedUniformMap m_lastAppliedUniformMap;
      
                typedef std::vector< ref_ptr<crShader> > ShaderList;
                ShaderList m_shadersToDetach;
                ShaderList m_shadersToAttach;

            private:
                PerContextProgram();        // disallowed
                PerContextProgram(const PerContextProgram&);        // disallowed
                PerContextProgram& operator=(const PerContextProgram&);        // disallowed
        };

        /** Get the PCP for a particular GL context */
        PerContextProgram* getPCP(unsigned int contextID) const;

    protected:        /*methods*/
        virtual ~crProgram();

    protected:        /*data*/

        mutable CRCore::buffered_value< CRCore::ref_ptr<PerContextProgram> > m_pcpList;
        AttribBindingList m_attribBindingList;
        FragDataBindingList m_fragDataBindingList;

        typedef std::vector< ref_ptr<crShader> > ShaderList;
        mutable ShaderList m_shaderList;

		static crMutex    s_mutex_deletedGlProgramCache;
		static DeletedGlProgramCache s_deletedGlProgramCache;

        /** Parameters maintained with glProgramParameteriEXT */
        GLint m_geometryVerticesOut;
        GLint m_geometryInputType;
        GLint m_geometryOutputType;

    private:
        crProgram& operator=(const crProgram&);        // disallowed
};

}

#endif
