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
#ifndef CRCORE_COLORMATRIX
#define CRCORE_COLORMATRIX 1

#include <CRCore/crMatrix.h>
#include <CRCore/crStateAttribute.h>

namespace CRCore {

/** Texture crMatrix state class for encapsulating OpenGL texture matrix functionality.*/
class CR_EXPORT crColorMatrix : public crStateAttribute
{
    public :
    
        crColorMatrix();

        /** Copy constructor using crCopyOp to manage deep vs shallow copy.*/
        crColorMatrix(const crColorMatrix& cm,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY):
            crStateAttribute(cm,copyop),
            m_matrix(cm.m_matrix) {}

        META_StateAttribute(CRCore, crColorMatrix, COLORMATRIX);
        
        /** return -1 if *this < *rhs, 0 if *this==*rhs, 1 if *this>*rhs.*/
        virtual int compare(const crStateAttribute& sa) const
        {
            // check the types are equal and then create the rhs variable
            // used by the COMPARE_StateAttribute_Paramter macro's below.
            COMPARE_StateAttribute_Types(crColorMatrix,sa)

            // compare each paramter in turn against the rhs.
            COMPARE_StateAttribute_Parameter(m_matrix)

            return 0; // passed all the above comparison macro's, must be equal.
        }
		virtual int compare(const crStateAttribute& sa)
		{
			// check the types are equal and then create the rhs variable
			// used by the COMPARE_StateAttribute_Paramter macro's below.
			COMPARE_StateAttribute_Types(crColorMatrix,sa)

		    // compare each paramter in turn against the rhs.
			COMPARE_StateAttribute_Parameter(m_matrix)

			return 0; // passed all the above comparison macro's, must be equal.
		}
        /** Set the color matrix */
        inline void setMatrix(const crMatrix& matrix) { m_matrix = matrix; }

        /** Get the color matrix */
        inline crMatrix& getMatrix() { return m_matrix; }

        /** Get the const color matrix */
        inline const crMatrix& getMatrix() const { return m_matrix; }

        /** apply as OpenGL texture matrix.*/
        virtual void apply(crState& state) const;

    protected:
        virtual ~crColorMatrix( void );
        crMatrix m_matrix;
};

}


#endif
