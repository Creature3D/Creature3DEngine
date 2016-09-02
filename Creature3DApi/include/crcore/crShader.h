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
//Modified by 吴财华
#ifndef CRCORE_CRSHADER
#define CRCORE_CRSHADER 1

#include <crgl/GL2Extensions.h>
#include <CRCore/crBase.h>
#include <CRCore/buffered_value.h>
#include <CRCore/thread/crMutex.h>

#include <set>
#include <list>
#include <map>
namespace CRCore {

class crProgram;

///////////////////////////////////////////////////////////////////////////
/** CRCore::crShader is an application-level abstraction of an OpenGL glShader.
  * It is a container to load the shader source code text and manage its
  * compilation.
  * An CRCore::crShader may be attached to more than one CRCore::crProgram.
  * crShader will automatically manage per-context instancing of the
  * internal objects, if that is necessary for a particular display
  * configuration.
  */

class CR_EXPORT crShader : public CRCore::crBase
{
    public:

        enum Type {
            VERTEX = GL_VERTEX_SHADER,
            FRAGMENT = GL_FRAGMENT_SHADER,
            GEOMETRY = GL_GEOMETRY_SHADER_EXT,
            UNDEFINED = -1
        };

        crShader( Type type = UNDEFINED);
        crShader( Type type, const std::string& source );

        /** Copy constructor using crCopyOp to manage deep vs shallow copy.*/
        crShader(const crShader& rhs, const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);

        META_Base(CRCore, crShader);

        int compare(const crShader& rhs) const;

        bool setType( Type t );
        

        /** Load the crShader's source code text from a string. */
        void setShaderSource( const std::string& sourceText );

        /** Query the shader's source code text */
        inline const std::string& getShaderSource() const { return m_shaderSource; }

        /** Get the crShader type as an enum. */
        inline Type getType() const { return m_type; }

        /** Get the crShader type as a descriptive string. */
        const char* getTypename() const;

        /** Mark our PCSs as needing recompilation.
          * Also mark Programs that depend on us as needing relink */
        void dirtyShader();        

        /** If needed, compile the PCS's glShader */
        void compileShader(unsigned int contextID) const;

        /** For a given GL context, attach a glShader to a glProgram */
        void attachShader(unsigned int contextID, GLuint program) const;

        /** For a given GL context, detach a glShader to a glProgram */
        void detachShader(unsigned int contextID, GLuint program) const;

        /** Query InfoLog from a glShader */
        bool getGlShaderInfoLog(unsigned int contextID, std::string& log) const;

        /** Mark internal glShader for deletion.
          * Deletion requests are queued until they can be executed
          * in the proper GL context. */
        static void deleteGlShader(unsigned int contextID, GLuint shader);

        /** flush all the cached glShaders which need to be deleted
          * in the OpenGL context related to contextID.*/
        static void flushDeletedGlShaders(unsigned int contextID,double currentTime, double& availableTime);
		static void flushAllDeletedGlShaders(unsigned int contextID);

        /** discard all the cached glShaders which need to be deleted in the OpenGL context related to contextID.
          * Note, unlike flush no OpenGL calls are made, instead the handles are all removed.
          * this call is useful for when an OpenGL context has been destroyed. */
        static void discardDeletedGlShaders(unsigned int contextID);

        static crShader::Type getTypeId( const std::string& tname );

		typedef std::list<GLuint> GlShaderHandleList;
		typedef std::map<unsigned int, GlShaderHandleList> DeletedGlShaderCache;

    protected:
        /** PerContextShader (PCS) is an OSG-internal encapsulation of glShader per-GL context. */
        class PerContextShader : public CRCore::Referenced
        {
            public:
                PerContextShader(const crShader* shader, unsigned int contextID);

                GLuint getHandle() const {return m_glShaderHandle;}

                void requestCompile();
                void compileShader();
                bool needsCompile() const {return m_needsCompile;}
                bool isCompiled() const {return m_isCompiled;}
                bool getInfoLog( std::string& infoLog ) const;

                /** Attach our glShader to a glProgram */
                void attachShader(GLuint program) const;

                /** Detach our glShader from a glProgram */
                void detachShader(GLuint program) const;

            protected:        /*methods*/
                ~PerContextShader();

            protected:        /*data*/
                /** Pointer to our parent CRCore::crShader */
                const crShader* m_shader;
                /** Pointer to this context's extension functions. */
                CRCore::ref_ptr<CRCore::GL2Extensions> m_extensions;
                /** Handle to the actual glShader. */
                GLuint m_glShaderHandle;
                /** Does our glShader need to be recompiled? */
                bool m_needsCompile;
                /** Is our glShader successfully compiled? */
                bool m_isCompiled;
                const unsigned int m_contextID;

            private:
                PerContextShader();        // disallowed
                PerContextShader(const PerContextShader&);        // disallowed
                PerContextShader& operator=(const PerContextShader&);        // disallowed
        };

    protected:        /*methods*/
        virtual ~crShader();

        PerContextShader* getPCS(unsigned int contextID) const;

        friend class CRCore::crProgram;
        bool addProgramRef( CRCore::crProgram* program );
        bool removeProgramRef( CRCore::crProgram* program );

    protected:        /*data*/
        Type m_type;
        std::string m_shaderSource;
        //std::string m_shaderFileName;
        /** CRCore::Programs that this CRCore::crShader is attached to */
        typedef std::set< CRCore::crProgram* > ProgramSet;
        ProgramSet m_programSet;
        mutable CRCore::buffered_value< CRCore::ref_ptr<PerContextShader> > m_pcsList;

		static crMutex    s_mutex_deletedGlShaderCache;
		static DeletedGlShaderCache  s_deletedGlShaderCache;

    private:
        crShader& operator=(const crShader&);        // disallowed
};

}

#endif
