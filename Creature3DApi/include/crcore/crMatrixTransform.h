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
#ifndef CRCORE_CRMATRIXTRANSFORM
#define CRCORE_CRMATRIXTRANSFORM 1

#include <CRCore/crTransform.h>
#include <CRCore/crAnimationPath.h>

namespace CRCore {

class CR_EXPORT crMatrixTransform : public crTransform
{
    public :


        crMatrixTransform();

        /** Copy constructor using crCopyOp to manage deep vs shallow copy.*/
        crMatrixTransform(const crMatrixTransform&,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY);

        crMatrixTransform(const crMatrix& matix);

        META_Node(CRCore, crMatrixTransform);

        virtual crMatrixTransform* asMatrixTransform() { return this; }
        virtual const crMatrixTransform* asMatrixTransform() const { return this; }

		inline virtual void dirtyBound(){ m_inverseDirty=true; crTransform::dirtyBound(); }

		inline void setPosition( const CRCore::crVector3f& pos ){ m_matrix.setTrans(pos); dirtyBound(); }
        /** Set the transform's matrix.*/
        virtual void setMatrix(const crMatrix& mat) { m_matrix = mat; dirtyBound(); }
        
        /** Get the matrix. */
        inline const crMatrix& getMatrix() const { return m_matrix; }
		//inline crMatrix* getMatrixPtr() { return &m_matrix; }

        /** pre multiply the transforms matrix.*/
        void preMult(const crMatrix& mat) { setMatrix(mat * m_matrix); }
        
        /** post multiply the transforms matrix.*/
        void postMult(const crMatrix& mat)  { setMatrix(m_matrix * mat); }
    
        /** Get the inverse matrix. */
        inline const crMatrix& getInverseMatrix() const
        {
            if (m_inverseDirty)
            {
                m_inverse.invert(m_matrix);
                m_inverseDirty = false;
            }
            return m_inverse;
        }

        virtual bool computeLocalToWorldMatrix(crMatrix& matrix, crNodeVisitor*) const;

        virtual bool computeWorldToLocalMatrix(crMatrix& matrix, crNodeVisitor*) const;

		inline CRCore::crVector3 getTrans() const { return m_matrix.getTrans(); }

    protected :
    
        virtual ~crMatrixTransform();
        
        crMatrix                              m_matrix;
        mutable crMatrix                      m_inverse;
        mutable bool                          m_inverseDirty;

};

}

#endif
