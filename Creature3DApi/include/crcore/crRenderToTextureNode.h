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
#ifndef CRCORE_CRRENDERTOTEXTURENODE
#define CRCORE_CRRENDERTOTEXTURENODE 1
#include <CRCore/crGroup.h>
#include <CRCore/crLightSource.h>
#include <CRCore/crCameraNode.h>
#include <CRCore/crViewPort.h>

#include <map>

namespace CRCore {

class CR_EXPORT crRenderToTextureNode : public crGroup
{
    public :
		crRenderToTextureNode();

        crRenderToTextureNode(const crRenderToTextureNode&,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY);

        META_Node(CRCore, crRenderToTextureNode);
        
        virtual void traverse(crNodeVisitor& nv);
        		
		inline void setCurrentProjectMatrix(const crMatrix &prjMat){  m_currentProjectMatrix = prjMat; }
		inline void setCurrentViewMatrix(const crMatrix &viewMat){  m_currentViewMatrix = viewMat; }
		//inline void setCurrentHorizontalFov(float hfov){ m_hfov = hfov; }
		//inline void setCurrentVerticalFov(float vfov){ m_vfov = vfov; }

		virtual void releaseObjects(crState* = 0) const;

		crTexture *getTexture();
		crImage *getImage();
        void setTextureSize(int width, int height);
		
    protected :
        virtual ~crRenderToTextureNode() {}
		
		crMatrix                        m_currentProjectMatrix;
		crMatrix                        m_currentViewMatrix;
		//float                            m_hfov;
		//float                            m_vfov;
    private:
		crVector2i                      m_textureSize;
		bool                            m_needRender;

};



}

#endif
