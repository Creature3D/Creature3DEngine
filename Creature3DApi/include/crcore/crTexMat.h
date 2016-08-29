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
#ifndef CRCORE_TEXMAT_H
#define CRCORE_TEXMAT_H 1

#include <CRCore/crStateAttribute.h>
#include <CRCore/crMatrix.h>

namespace CRCore {

/** Texture crMatrix state class for encapsulating OpenGL texture matrix functionality.*/
class CR_EXPORT crTexMat : public crStateAttribute
{
    public :

        crTexMat();

        /** Copy constructor using crCopyOp to manage deep vs shallow copy.*/
        crTexMat(const crTexMat& texmat,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY):
            crStateAttribute(texmat,copyop),
            m_matrix(texmat.m_matrix) {}

        META_StateAttribute(CRCore, crTexMat, TEXMAT);
        
        virtual bool isTextureAttribute() const { return true; }

        /** return -1 if *this < *rhs, 0 if *this==*rhs, 1 if *this>*rhs.*/
        virtual int compare(const crStateAttribute& sa) const
        {
            // check the types are equal and then create the rhs variable
            // used by the COMPARE_StateAttribute_Paramter macro's below.
            COMPARE_StateAttribute_Types(crTexMat,sa)

            // compare each paramter in turn against the rhs.
            COMPARE_StateAttribute_Parameter(m_matrix)

            return 0; // passed all the above comparison macro's, must be equal.
        }
		virtual int compare(const crStateAttribute& sa)
		{
			// check the types are equal and then create the rhs variable
			// used by the COMPARE_StateAttribute_Paramter macro's below.
			COMPARE_StateAttribute_Types(crTexMat,sa)

			// compare each paramter in turn against the rhs.
			COMPARE_StateAttribute_Parameter(m_matrix)

			return 0; // passed all the above comparison macro's, must be equal.
		}

        /** Set the texture matrix */
        inline void setMatrix(const crMatrix& matrix) { m_matrix = matrix; }

        /** Get the texture matrix */
        inline crMatrix& getMatrix() { return m_matrix; }

        /** Get the const texture matrix */
        inline const crMatrix& getMatrix() const { return m_matrix; }

        /** apply as OpenGL texture matrix.*/
        virtual void apply(crState& state) const;

    protected:

        virtual ~crTexMat( void );

        crMatrix m_matrix;

};

}


#endif
