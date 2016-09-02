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
#ifndef CRCORE_FRAGMENTPROGRAM_H
#define CRCORE_FRAGMENTPROGRAM_H 1

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

/** crFragmentProgram - encapsulates the OpenGL ARB fragment program state.*/
class CR_EXPORT crFragmentProgram : public crStateAttribute
{
    public:

        crFragmentProgram();

        /** Copy constructor using crCopyOp to manage deep vs shallow copy.*/
        crFragmentProgram(const crFragmentProgram& vp,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY);

        META_StateAttribute(CRCore, crFragmentProgram, FRAGMENTPROGRAM);

        /** return -1 if *this < *rhs, 0 if *this==*rhs, 1 if *this>*rhs.*/
        virtual int compare(const CRCore::crStateAttribute& sa) const
        {
            // check the types are equal and then create the rhs variable
            // used by the COMPARE_StateAttribute_Paramter macro's below.
            COMPARE_StateAttribute_Types(crFragmentProgram,sa)

            // compare each paramter in turn against the rhs.
            COMPARE_StateAttribute_Parameter(m_fragmentProgram)

            return 0; // passed all the above comparison macro's, must be equal.
        }
		virtual int compare(const CRCore::crStateAttribute& sa)
		{
			// check the types are equal and then create the rhs variable
			// used by the COMPARE_StateAttribute_Paramter macro's below.
			COMPARE_StateAttribute_Types(crFragmentProgram,sa)

			// compare each paramter in turn against the rhs.
			COMPARE_StateAttribute_Parameter(m_fragmentProgram)

			return 0; // passed all the above comparison macro's, must be equal.
		}

		virtual void getAssociatedModes(std::vector<VRMode>& modes) const 
		{
			modes.push_back(GL_FRAGMENT_PROGRAM_ARB);
		}
        // data access methods.

        /** Get the handle to the fragment program id for the current context.*/
        inline unsigned int& getFragmentProgramID(unsigned int contextID) const
        {
            return m_fragmentProgramIDList[contextID];
        }

        /** Set the fragment program using C++ style string.*/
        inline void setFragmentProgram( const std::string& program )
        {
            m_fragmentProgram = program;
            dirtyFragmentProgramObject();
        }
        
        /** Set the fragment program using a C style string.*/
        inline void setFragmentProgram( const char* program ) 
        { 
            m_fragmentProgram = program; 
            dirtyFragmentProgramObject();
        }
        /** Get the fragment program.*/
        inline const std::string& getFragmentProgram() const { return m_fragmentProgram; }

        /** Set Program Parameters */
        inline void setProgramLocalParameter(const unsigned int index, const crVector4& p)
        {
            m_programLocalParameters[index] = p;
        }

        typedef std::map<unsigned int,crVector4> LocalParamList;

        /** Get list of Program Parameters */
        inline LocalParamList& getLocalParamList() { return m_programLocalParameters; }

		  /** Get const list of Program Parameters */
        inline const LocalParamList& getLocalParamList() const { return m_programLocalParameters; }

        /** crMatrix */
        inline void setMatrix(const unsigned int mode, const crMatrix& matrix)
        {
            m_matrixList[mode] = matrix;
        }
		typedef std::map<unsigned int,crMatrix> MatrixList;
		inline MatrixList &getMatrixList(){ return m_matrixList; }
		inline const MatrixList &getMatrixList()const{ return m_matrixList; }

        /** Force a recompile on next apply() of associated OpenGL vertex program objects.*/
        void dirtyFragmentProgramObject();        

        /** use deleteFragmentProgramObject instead of glDeletePrograms to allow
          * OpenGL Fragment Program objects to cached until they can be deleted
          * by the OpenGL context in which they were created, specified
          * by contextID.*/
        static void deleteFragmentProgramObject(unsigned int contextID,unsigned int handle);

        /** flush all the cached fragment programs which need to be deleted
          * in the OpenGL context related to contextID.*/
        //static void flushDeletedFragmentProgramObjects(unsigned int contextID,double currentTime, double& availableTime);

        //virtual void apply(crState& state) const;

        virtual void compile(crState& state) const { apply(state); }

        /** release an OpenGL objects in specified graphics context if crState
            object is passed, otherwise release OpenGL objexts for all graphics context if
            crState object pointer NULL.*/
        virtual void releaseGLObjects(crState* state=0) const;

		// static cache of deleted fragment programs which can only 
		// by completely deleted once the appropriate OpenGL context
		// is set.
		typedef std::list<unsigned int> FragmentProgramObjectList;
		typedef std::map<unsigned int,FragmentProgramObjectList> DeletedFragmentProgramObjectCache;
		static DeletedFragmentProgramObjectCache s_deletedFragmentProgramObjectCache;
		static OpenThreads::Mutex                s_mutex_deletedFragmentProgramObjectCache;

    protected:


        virtual ~crFragmentProgram();

        typedef buffered_value<unsigned int> FragmentProgramIDList;
        mutable FragmentProgramIDList m_fragmentProgramIDList;

        std::string     m_fragmentProgram;

        LocalParamList  m_programLocalParameters;

        MatrixList  m_matrixList;
};

}

#endif

