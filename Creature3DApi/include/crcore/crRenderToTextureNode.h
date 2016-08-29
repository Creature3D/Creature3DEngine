
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
