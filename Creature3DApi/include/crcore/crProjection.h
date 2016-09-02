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
#ifndef CRCORE_PROJECTION
#define CRCORE_PROJECTION 1

#include <CRCore/crGroup.h>
#include <CRCore/crMatrix.h>

namespace CRCore {

/** crProjection nodes set up the frustum/orthographic projection used when rendering the scene .
*/
class CR_EXPORT crProjection : public crGroup
{
    public :


        crProjection();

        /** Copy constructor using crCopyOp to manage deep vs shallow copy.*/
        crProjection(const crProjection&,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY);

        crProjection(const crMatrix& matix);

        META_Node(CRCore, crProjection);

        /** Set the transform's matrix.*/
        void setMatrix(const crMatrix& mat) { m_matrix = mat; }
        
        /** Get the transform's matrix. */
        inline const crMatrix& getMatrix() const { return m_matrix; }

        /** preMult transform.*/
        void preMult(const crMatrix& mat) { m_matrix.preMult(mat); }
        
        /** postMult transform.*/
        void postMult(const crMatrix& mat)  { m_matrix.postMult(mat); }
    

    protected :
    
        virtual ~crProjection();
                       
        crMatrix  m_matrix;

};

}

#endif
