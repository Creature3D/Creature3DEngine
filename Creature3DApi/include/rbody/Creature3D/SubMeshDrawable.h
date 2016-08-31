/* Creature3D - Online Game Engine, Copyright (C) 2005 吴财华(26756325@qq.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
*/
#ifndef __SUBMESHDRAWABLE_H__
#define __SUBMESHDRAWABLE_H__
#include <cal3d/cal3d.h>
#include "rbody/export.h"
#include <CRCore/crDrawable.h>
#include <CRCore/crGeometry.h>
#include <CRCore/ref_ptr.h>

namespace rbody {

    // Forward declarations
    class CreBodyNode;
    class CRCore::crState;


    /** crDrawable class for specific mesh/submesh of CRCore body */
    class REPLICANTBODY_API SubMeshDrawable : public CRCore::crDrawable
    {

        public:

            SubMeshDrawable() {};

            /** Constructor
             * \param body is body to get vertex data from
             * \param geometry is geometry with drawables
             * \param mesh_id is cal ID of mesh to get vertex data from
             * \param submesh_id is cal ID of submesh to get vertex data from
             **/
            SubMeshDrawable(CreBodyNode *body, CRCore::crGeometry *geometry, int mesh_id, int submesh_id/*, bool enableTangentSpace*/);

            // Set general settings for this class with macro
            META_Base(rbody, SubMeshDrawable);

            /** Copy Constructor
             * \param submesh_drawable is object to copy
             * \param copyop is copy operation to use
             **/
            SubMeshDrawable(const SubMeshDrawable& submesh_drawable, 
                    const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY) : 
                CRCore::crDrawable(submesh_drawable, copyop), m_body(submesh_drawable.m_body)/*,m_enableTangentSpace(submesh_drawable.m_enableTangentSpace)*/ {};

            /** Draw SubMeshDrawable directly
             * \param state is CRCore statistics
             **/
			virtual void draw(CRCore::crState& state) const;
            virtual void drawImplementation(CRCore::crState& state) const; 

            /** Compute bounding box (axis aligned) */
			//virtual CRCore::crBoundingBox computeBound() const;

            /** Accept primitive functor, enables collision detection */
            virtual void accept(PrimitiveFunctor &prim) const;
            
			virtual CRCore::crGeometry *getPuppetGeometry();
			virtual CRCore::crGeometry *getPuppetBufGeometry();
            /*void setNoNeedTangentSpaceFlag(bool noNeedTangentSpace)*/;

			virtual CRCore::crGeometry* asGeometry() { return m_geometry.get(); }
            
			virtual void releaseObjects(CRCore::crState* state=0);
			inline int getMeshID() { return m_mesh_id; }
			inline int getSubMeshID() { return m_submesh_id; }

			virtual void swapBuffers(int frameNumber);
			/** Handle what to do on update, that is, when body is in FOV */
			void update(/*int fn*/);

			CreBodyNode *getBodyNode();
        protected:
            /** Destructor */
            ~SubMeshDrawable();

			virtual bool computeBound() const;

        private:

            /** Osg body reference */
            CreBodyNode *m_body;

            /** crGeometry with allocated memory for shared vertices */
            mutable CRCore::ref_ptr<CRCore::crGeometry> m_geometry;
			mutable CRCore::ref_ptr<CRCore::crGeometry> m_geometry_buf;

            /** Cal ID of mesh with vertices */
            int m_mesh_id;

            /** Cal ID of submesh with vertices */
            int m_submesh_id;

            /** Dirty flag which tells if vertices need to be recalculated */
            //bool m_dirty_meshes;

			//bool m_enableTangentSpace;
			//bool m_noNeedTangentSpace;

			mutable CRCore::ref_ptr<CRCore::Vec4Array> m_tangentSpacesArray;
			int m_started;//0:未开始,1m_geometry已经好了,2表示m_geometry_buf可以渲染了
			//mutable int m_lastUpdateFrameNumber;
    };


} // namespace rbody
#endif // __SUBMESHDRAWABLE_H_

