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
#ifndef CRCORE_VERTEXPROGRAM_H
#define CRCORE_VERTEXPROGRAM_H 1

#include <CRCore/crStateAttribute.h>
#include <CRCore/crVector4.h>
#include <CRCore/crMatrix.h>
#include <CRCore/buffered_value.h>
#include <OpenThreads/ScopedLock>
#include <OpenThreads/Mutex>
#include <list>
#include <map>
#include <string>

namespace CRCore {

/** crVertexProgram - encapsulates the OpenGL ARB vertex program state.*/
class CR_EXPORT crVertexProgram : public crStateAttribute
{
    public:

        crVertexProgram();

        /** Copy constructor using crCopyOp to manage deep vs shallow copy.*/
        crVertexProgram(const crVertexProgram& vp,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY);

        META_StateAttribute(CRCore, crVertexProgram, VERTEXPROGRAM);

        /** return -1 if *this < *rhs, 0 if *this==*rhs, 1 if *this>*rhs.*/
        virtual int compare(const CRCore::crStateAttribute& sa) const
        {
            // check the types are equal and then create the rhs variable
            // used by the COMPARE_StateAttribute_Paramter macro's below.
            COMPARE_StateAttribute_Types(crVertexProgram,sa)

            // compare each paramter in turn against the rhs.
            COMPARE_StateAttribute_Parameter(m_vertexProgram)

            return 0; // passed all the above comparison macro's, must be equal.
        }
		virtual int compare(const CRCore::crStateAttribute& sa)
		{
			// check the types are equal and then create the rhs variable
			// used by the COMPARE_StateAttribute_Paramter macro's below.
			COMPARE_StateAttribute_Types(crVertexProgram,sa)

			// compare each paramter in turn against the rhs.
			COMPARE_StateAttribute_Parameter(m_vertexProgram)

			return 0; // passed all the above comparison macro's, must be equal.
		}
        //virtual bool getModeUsage(ModeUsage& usage) const
        //{
        //    usage.usesMode(GL_VERTEX_PROGRAM_ARB);
        //    return true;
        //}
		virtual void getAssociatedModes(std::vector<VRMode>& modes) const 
		{
			modes.push_back(GL_VERTEX_PROGRAM_ARB);
		}

        // data access methods.

        /** Get the handle to the vertex program id for the current context.*/
        inline unsigned int& getVertexProgramID(unsigned int contextID) const
        {
            return m_vertexProgramIDList[contextID];
        }

        /** Set the vertex program using C++ style string.*/
        inline void setVertexProgram( const std::string& program )
        {
            m_vertexProgram = program;
            dirtyVertexProgramObject();
        }
        
        /** Set the vertex program using a C style string.*/
        inline void setVertexProgram( const char* program ) 
        { 
            m_vertexProgram = program; 
            dirtyVertexProgramObject();
        }
        /** Get the vertex program.*/
        inline const std::string& getVertexProgram() const { return m_vertexProgram; }

        /** Program Parameters */
		typedef std::map<unsigned int,crVector4> LocalParamList;
		inline void setProgramLocalParameter(const unsigned int index, const crVector4& p)
        {
            m_programLocalParameters[index] = p;
        }
       /** Get list of Program Parameters */
        inline LocalParamList& getLocalParamList() { return m_programLocalParameters; }

		  /** Get const list of Program Parameters */
        inline const LocalParamList& getLocalParamList() const { return m_programLocalParameters; }

		/** crMatrix */
		typedef std::map<unsigned int,crMatrix> MatrixList;
		inline void setMatrix(const unsigned int mode, const crMatrix& matrix)
        {
            m_matrixList[mode] = matrix;
        }
		inline MatrixList &getMatrixList(){ return m_matrixList; }
		inline const MatrixList &getMatrixList()const{ return m_matrixList; }

        /** Force a recompile on next apply() of associated OpenGL vertex program objects.*/
        void dirtyVertexProgramObject();        

        /** use deleteVertexProgramObject instead of glDeletePrograms to allow
          * OpenGL Vertex Program objects to cached until they can be deleted
          * by the OpenGL context in which they were created, specified
          * by contextID.*/
        static void deleteVertexProgramObject(unsigned int contextID,unsigned int handle);

        //virtual void apply(crState& state) const;

        virtual void compile(crState& state) const { apply(state); }

        /** release an OpenGL objects in specified graphics context if crState
            object is passed, otherwise release OpenGL objexts for all graphics context if
            crState object pointer NULL.*/
        virtual void releaseGLObjects(crState* state=0) const;

		typedef std::list<unsigned int> VertexProgramObjectList;
		typedef std::map<unsigned int,VertexProgramObjectList> DeletedVertexProgramObjectCache;
		static DeletedVertexProgramObjectCache s_deletedVertexProgramObjectCache;
		static OpenThreads::Mutex              s_mutex_deletedVertexProgramObjectCache;

    protected:

        virtual ~crVertexProgram();

        typedef buffered_value<unsigned int> VertexProgramIDList;
        mutable VertexProgramIDList m_vertexProgramIDList;

        std::string     m_vertexProgram;

        LocalParamList  m_programLocalParameters;

        MatrixList  m_matrixList;
};



}

#endif

