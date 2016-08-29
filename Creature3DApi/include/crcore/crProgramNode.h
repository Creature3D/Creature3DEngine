
#ifndef CRCORE_CRPROGRAMNODE
#define CRCORE_CRPROGRAMNODE 1
#include <CRCore/crGroup.h>
#include <CRCore/crLightSource.h>
#include <CRCore/crCameraNode.h>
#include <CRCore/crViewPort.h>

#include <map>

namespace CRCore {

class CR_EXPORT crProgramNode : public crGroup
{
    public :
		crProgramNode();

        crProgramNode(const crProgramNode&,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY);

        META_Node(CRCore, crProgramNode);
        
        virtual void traverse(crNodeVisitor& nv);

        

		enum LightRenderMode
		{
			None = 0x00,
			LightMapping = 0x01,
			ParallaxMapping = 0x02,
			ShadowMapping = 0x04,
			Fog = 0x08,
			SunLight = 0x10
		};

		typedef std::pair< CRCore::ref_ptr<CRCore::crLightSource>, CRCore::ref_ptr<CRCore::crCameraNode> > LS_CameraPair;
		typedef std::map< CRCore::ref_ptr<CRCore::crLightSource>, CRCore::ref_ptr<CRCore::crCameraNode> > LS_CameraMap;

		struct ActiveCameraParams
		{
			CRCore::ref_ptr<CRCore::crLightSource> m_lightSource;
            CRCore::ref_ptr<CRCore::crCameraNode> m_cameraNode;
			crMatrix m_lightMVP0;
			crMatrix m_lightMVP1;
		};
		typedef std::multimap< int, ActiveCameraParams, std::greater<int> > Active_LS_CameraMap;


		void insertLightSource( CRCore::crLightSource *ls );
		void createShader(const crVector4& fogColor = crVector4(1.0f,1.0f,1.0f,1.0f));

		inline int getNumLightSources(){ return m_ls_cameraMap.size(); }
        
        virtual void updateUniform(crViewPort *sceneViewPort);
		
		inline void setCurrentProjectMatrix(const crMatrix &prjMat){  m_currentProjectMatrix = prjMat; }
		inline void setCurrentViewMatrix(const crMatrix &viewMat){  m_currentViewMatrix = viewMat; }
		//inline void setCurrentHorizontalFov(float hfov){ m_hfov = hfov; }
		//inline void setCurrentVerticalFov(float vfov){ m_vfov = vfov; }

		inline void setIndoorFlag( bool indoor ){ m_indoor = indoor; }
		inline bool getIndoorFlag(){ return m_indoor; }

		Active_LS_CameraMap &getActive_LS_CameraMap(){ return m_active_ls_cameraMap; }
		Active_LS_CameraMap &getActive_LS_CameraMap_Buf(){ return m_active_ls_cameraMap_buf; }

		virtual void swapBuffers();

    protected :
        virtual ~crProgramNode() {}
		
		virtual bool computeBound() const;

		void updateUniformParams(const CRCore::crLightSource *light_source, CRCore::crCameraNode *shadow_camera, int nlight);

		LS_CameraMap m_ls_cameraMap;
		Active_LS_CameraMap m_active_ls_cameraMap;
		Active_LS_CameraMap m_active_ls_cameraMap_buf;
		//Active_LS_LightMVPMap m_active_ls_lightmvpMap;

		crMatrix                         m_currentProjectMatrix;
		crMatrix                         m_currentViewMatrix;
		//float                            m_hfov;
		//float                            m_vfov;
    private:
		bool m_indoor;
};

}

#endif
