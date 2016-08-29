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
#ifndef CRCORE_CRTRANSFORM_H
#define CRCORE_CRTRANSFORM_H

#include <CRCore/crGroup.h>
#include <CRCore/crMatrix.h>

namespace CRCore{
/** compute the crMatrix which crTransforms objects in local coords to world coords,
  * by accumulating the crTransform local to world matrices along the specified node path.*/
extern CR_EXPORT crMatrix computeLocalToWorld(NodePath& nodePath);

/** compute the crMatrix which crTransforms objects in world coords to local coords,
  * by accumulating the crTransform world to local matrices along the specified node path.*/
extern CR_EXPORT crMatrix computeWorldToLocal(NodePath& nodePath);

/** compute the crMatrix which crTransforms objects in local coords to world coords,
  * by accumulating the crTransform local to world matrices along the specified node path 
  * the supplied initialial camera modelview .*/
extern CR_EXPORT crMatrix computeLocalToEye(const crMatrix& modelview, NodePath& nodePath);

/** compute the crMatrix which crTransforms objects in world coords to local coords,
  * by accumulating the crTransform world to local matrices along the specified node path 
  * the inverse of the supplied initialial camera modelview.*/
extern CR_EXPORT crMatrix computeEyeToLocal(const crMatrix& modelview, NodePath& nodePath);


/** A crTransform is a crGroup node for which all children are crTransformed by
  * a 4x4 crMatrix.  It is often used for positioning objects within a scene,
  * producing trackball functionality or for animation.
  *
  * crTransform itself does not provide set/get functions, only the interface
  * for defining what the 4x4 crTransformation is.  Subclasses, such as
  * crMatrixcrTransform and PositionAttitudecrTransform support the use of an
  * CRCore::crMatrix or a CRCore::Vec3/CRCore::Quat resprectively.
  * The crTransform node can be customized via the ComputeTransfromCallback
  * which can be attached to the node.  This might be used to convert from
  * internal representations of the crTransformation into generic CRCore::crMatrix
  * objects which are used during scene grpah traversal, such as
  * CullTraversal and IntersectionTraversal.
  *
  * Note: if the crTransformation crMatrix scales the subgraph then the normals
  * of the underlying geometry will need to be renormalized to be unit
  * vectors once more.  This can be done transparently through OpenGL's 
  * use of either GL_NORMALIZE and GL_SCALE_NORMALIZE modes.  For further
  * background reading see the glNormalize documentation in the OpenGL
  * Reference Guide (the blue book). To enable it in the OSG, you simply
  * need to attach a local CRCore::StateSet to the CRCore::crTransform, and set
  * the appropriate mode to ON via
  *   stateset->setMode(GL_NORMALIZE, CRCore::StateAttribute::ON);
*/

class crMatrixTransform;
class crPositionAttitudeTransform;

class CR_EXPORT crTransform : public crGroup
{
public :

    crTransform();

    /** Copy constructor using crCopyOp to manage deep vs shallow copy.*/
    crTransform(const crTransform&,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY);

    META_Node(CRCore, crTransform);

    virtual crTransform* asTransform() { return this; }
    virtual const crTransform* asTransform() const { return this; }

    virtual crMatrixTransform* asMatrixTransform() { return 0; }
    virtual const crMatrixTransform* asMatrixTransform() const { return 0; }

    virtual crPositionAttitudeTransform* asPositionAttitudeTransform() { return 0; }
    virtual const crPositionAttitudeTransform* asPositionAttitudeTransform() const { return 0; }

    enum ReferenceFrame
    {
        RELATIVE_TO_PARENTS,
        RELATIVE_TO_ABSOLUTE
    };
    
    /** Set the crTransform's ReferenceFrame, either to be relative to its
        * parent reference frame, or relative to an absolute coordinate
        * frame. RELATIVE_TO_PARENTS is the default.
        * Note: setting the ReferenceFrame to be RELATIVE_TO_ABSOLUTE will
        * also set the CullingActive flag on the crTransform, and hence all
        * of its parents, to false, thereby disabling culling of it and
        * all its parents.  This is neccessary to prevent inappropriate
        * culling, but may impact cull times if the absolute crTransform is
        * deep in the scene graph.  It is therefore recommend to only use
        * absolute crTransforms at the top of the scene, for such things as
        * heads up displays. */
    void setReferenceFrame(ReferenceFrame rf);
    
    ReferenceFrame getReferenceFrame() const { return m_referenceFrame; }

    virtual bool computeLocalToWorldMatrix(crMatrix& crMatrix,crNodeVisitor*) const
    {
        if (m_referenceFrame==RELATIVE_TO_PARENTS)
        {
            return false;
        }
        else // absolute
        {
            crMatrix.makeIdentity();
            return true;
        }
    }
    
    virtual bool computeWorldToLocalMatrix(crMatrix& crMatrix,crNodeVisitor*) const
    {
        if (m_referenceFrame == RELATIVE_TO_PARENTS)
        {
            return false;
        }
        else // absolute
        {
            crMatrix.makeIdentity();
            return true;
        }
    }

protected :

    virtual ~crTransform();
    
    /** Overrides crGroup's computeBound. 
        * There is no need to override in subclasses from CRCore::crTransform
        * since this computeBound() uses the underlying crMatrix (calling
        * computecrMatrix if required.) */
    virtual bool computeBound() const;
    
    ReferenceFrame                      m_referenceFrame;

};

}

#endif