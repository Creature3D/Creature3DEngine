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
#ifndef CRCORE_CRLIGHTSOURCE_H
#define CRCORE_CRLIGHTSOURCE_H 1

#include <CRCore/crNodeVisitor.h>
#include <CRCore/crLight.h>
#include <CRCore/crGroup.h>
#include <CRCore/crObject.h>
#include <CRCore/thread/crScopedLock.h>
namespace CRCore {

/** Leaf Node for defining a light in the scene.*/
class CR_EXPORT crLightSource : public crGroup
{
    public:
    
        crLightSource();

        crLightSource(const crLightSource& ls, const crCopyOp& copyop=crCopyOp::SHALLOW_COPY):
            crGroup(ls,copyop),
			m_enable(ls.m_enable),
			m_stdModeEnable(ls.m_stdModeEnable),
			m_global(ls.m_global),
			m_sunLight(ls.m_sunLight),
            m_calcShadow(ls.m_calcShadow),
			m_calcShadow2(ls.m_calcShadow2),
			m_staticShadowMapChannel(ls.m_staticShadowMapChannel),
			m_referenceFrame(ls.m_referenceFrame),
			m_lightRangeScale(ls.m_lightRangeScale),
            m_invShadowMapSize(ls.m_invShadowMapSize),
            m_light(dynamic_cast<CRCore::crStateAttribute*>(copyop(ls.m_light.get()))),
			m_changeEnableInNextFrame(false),
			m_changeEnable(false)
			{
				//if(m_lightMap.valid())
				//	m_lightMap = dynamic_cast<CRCore::crStateAttribute*>(copyop(ls.m_lightMap.get()))
				m_lightParamStruct = new LightParamStruct;
			}

        META_Node(CRCore, crLightSource);

        enum ReferenceFrame
        {
            RELATIVE_TO_PARENTS,
            RELATIVE_TO_ABSOLUTE
        };
        
        /** Set the light sources's ReferenceFrame, either to be relative to its
          * parent reference frame, or relative to an absolute coordinate
          * frame. RELATIVE_TO_PARENTS is the default.
          * Note: setting the ReferenceFrame to be RELATIVE_TO_ABSOLUTE will
          * also set the CullingActive flag on the light source, and hence all
          * of its parents, to false, thereby disabling culling of it and
          * all its parents.  This is neccessary to prevent inappropriate
          * culling, but may impact cull times if the absolute light source is
          * deep in the scene graph.  It is therefore recommend to only use
          * absolute light source at the top of the scene. */
        void setReferenceFrame(ReferenceFrame rf);
        
        ReferenceFrame getReferenceFrame() const { return m_referenceFrame; }

        /** Set the attached light.*/
        void setLight(crStateAttribute* light);
		//void setLight( int lightid /*, crStateSet::StateData::LightMode lightMode =  crStateSet::StateData::ON */);
        void initLight();
        /** Get the attached light.*/
        inline crStateAttribute* getLight() { return m_light.get(); }

        /** Get the const attached light.*/
        inline const crStateAttribute* getLight() const { return m_light.get(); }

        ///** Set the GLModes on crStateSet associated with the crLightSource.*/
        //void setStateSetModes(crStateSet&,crStateAttribute::VRModeValue) const;

        ///** Set up the local crStateSet */
        //void setLocalStateSetModes(crStateAttribute::VRModeValue=crStateAttribute::ON);

		void setEnable( bool enable );
		inline bool getEnable()const{ return m_enable; }

		void setStdModeEnable( bool enable );
		inline bool getStdModeEnable()const{ return false/*m_stdModeEnable*/; }

		void setGlobal( bool global );
		inline bool getGlobal()const{ return m_global; }
        
		void setLightToBeSunLight(bool isSunlight);
		inline bool getLightIsSunLight()const { return m_sunLight; }
		
		void setCalcShadow(bool calcShadow);
		void setStaticShadowMapChannel(int channel);
		inline int getStaticShadowMapChannel() { return m_staticShadowMapChannel; }
	
		inline void setLightRangeScale(float scale){ m_lightRangeScale = scale; }
		inline float getLightRangeScale(){ return m_lightRangeScale; }

		inline void setLightMap(crTexture2D *lightmap){ m_lightMap = lightmap; }
		inline crTexture2D *getLightMap() { return m_lightMap.get(); }
		inline void setLightMap2(crTexture2D *lightmap){ m_lightMap2 = lightmap; }
		inline crTexture2D *getLightMap2() { return m_lightMap2.get(); }

		inline void setSpotLightMap(crTexture2D *lightmap){ m_spotLightMap = lightmap; }
		inline crTexture2D *getSpotLightMap() { return m_spotLightMap.get(); }

		inline void setInvShadowmapSize( double invShadowMapSize ) { m_invShadowMapSize = invShadowMapSize; }
		inline float getInvShadowmapSize(){ return m_invShadowMapSize; }

		////////culling 调用
		inline void setLightMapMVP(const crMatrixf &mvp){ m_lightParamStruct->m_lightMapMVP = mvp; }
		inline void setLightMapMVP2(const crMatrixf &mvp){ m_lightParamStruct->m_lightMapMVP2 = mvp; }
		inline void setCosLightMapFov(float cosfov){ m_lightParamStruct->m_cosfov = cosfov; }
		inline void setLightShadowCulled( bool culled ) { m_lightParamStruct->m_lightShadowCulled = culled; }
		inline void setLightMatrix(crMatrix &lightMat)
		{
			CRCore::ScopedLock<crMutex> lock(m_mutex);
			//CRCore::notify(CRCore::FATAL)<<getName()<<std::endl;
			const crVector4 &pos = (dynamic_cast<crLight *>(m_light.get()))->getPosition();
			m_lightParamStruct->m_lsPosition.set(pos[0],pos[1],pos[2]);
			//CRCore::notify(CRCore::FATAL)<<getName()<<"light pos :"<<pos<<std::endl;
			m_lightParamStruct->m_lsPosition = m_lightParamStruct->m_lsPosition * lightMat;
			//CRCore::notify(CRCore::FATAL)<<getName()<<"lspos = "<<m_lightParamStruct->m_lsPosition<<std::endl;
			lightMat.setTrans(0.0f,0.0f,0.0f);
			m_lightParamStruct->m_lsDirection = (dynamic_cast<crLight *>(m_light.get()))->getDirection() * lightMat;
			m_lightParamStruct->m_lsDirection.normalize();
			if(m_changeEnableInNextFrame) 
			{
				setEnable(m_changeEnable);
				m_changeEnableInNextFrame = false;
			}
		}
		inline void setLSDirection(const crVector3f &dir) 
		{ CRCore::ScopedLock<crMutex> lock(m_mutex); m_lightParamStruct->m_lsDirection = dir; }

		inline const crVector3f &getLSPosition() const { return m_lightParamStruct->m_lsPosition; }
		inline const crVector3f &getLSDirection() const { return m_lightParamStruct->m_lsDirection; }
		inline bool getCalcShadow()const{ return m_enable && m_calcShadow && !m_lightParamStruct->m_lightShadowCulled && crDisplaySettings::instance()->getShadowDetail()!=0; }
		inline bool getLightShadowCulled()const { return m_lightParamStruct->m_lightShadowCulled; }

		//inline const crVector4f &getLSColor() const { return m_lightParamStruct->m_lsColor; }
		//inline const crVector4f &getLSAttenuations() const { return m_lightParamStruct->m_lsAttenuations; }
		inline const crMatrixf &getLSParam()const { return m_lightParamStruct->m_lsParam; }
		
		inline const crMatrixf &getLightMapMVP()const { return m_lightParamStruct->m_lightMapMVP; }
		inline const crMatrixf &getLightMapMVP2()const { return m_lightParamStruct->m_lightMapMVP2; }
		inline float getCosLightMapFov()const { return m_lightParamStruct->m_cosfov; }        
		inline float getLoweastLum()const { return m_lightParamStruct->m_loweastLum; }
////////////////

		//typedef std::set<const crObject *> LightObjectSet;
		//void addAvoidLightObject(const crObject *obj);
		//inline bool isObjectAvoidLight(const crObject *obj) { return m_avoidLightObjectSet.find(obj) != m_avoidLightObjectSet.end(); }
		//void addNeedLightObject(const crObject *obj);
		//inline bool isObjectNeedLight(const crObject *obj) { return m_needLightObjectSet.find(obj) != m_needLightObjectSet.end(); }

		void initLightParamStruct();

		struct LightParamStruct :public Referenced
		{
			LightParamStruct():m_cosfov(0.0),
				m_lightShadowCulled(false){}

			bool                               m_lightShadowCulled;
			crMatrixf                          m_lightMapMVP;
			crMatrixf                          m_lightMapMVP2;
			float                              m_cosfov;
			float                              m_loweastLum;
			crVector3f                         m_lsPosition;
			crVector3f                         m_lsDirection;

			//crVector4f                         m_lsAmbient;
			//crVector4f                         m_lsDiffuse;
			//crVector4f                         m_lsSpecular;
			//crVector4f                         m_lsAttenuations;
			//crVector4f                       m_lsColor;
			crMatrixf                          m_lsParam;//LA,LD,LS,Atten
		};

		inline LightParamStruct *getLightParamStruct() { return m_lightParamStruct.get(); }

		virtual void setEffectPosition(const crVector3 &position);
		inline virtual bool isEffectIdle() const { return !m_enable;}
		inline virtual void startEffect(){ changeEnableInNextFrame(true); }
		inline virtual void stopEffect(){ setEnable(false); }

		inline void changeEnableInNextFrame(bool enable) { m_changeEnableInNextFrame = true; m_changeEnable = enable; }

		inline void setCalcShadow2(bool calcShadow2) { m_calcShadow2 = calcShadow2; }
		inline bool getCalcShadow2()const { return m_sunLight && m_calcShadow2 && getCalcShadow();  }
    protected:

        virtual ~crLightSource();

        virtual bool computeBound() const;

        ref_ptr<crStateAttribute>         m_light;
        bool                              m_enable;
		bool                              m_stdModeEnable;
		bool                              m_global;
		bool                              m_sunLight;
		bool                              m_calcShadow;
		bool                              m_calcShadow2;//计算远距离实时阴影，用于sunlight室外场景
        int                               m_staticShadowMapChannel;//-1表示不计算staticShadow(默认)，0=r,1=g,2=b

		ref_ptr<crTexture2D>              m_lightMap;
		ref_ptr<crTexture2D>              m_lightMap2;
		ref_ptr<crTexture2D>              m_spotLightMap;//聚光灯贴图,光源diffuse贴图

		float                             m_lightRangeScale;
		float                             m_invShadowMapSize;
		
		//crMatrix                          m_lightMatrix;
		ReferenceFrame                    m_referenceFrame;
		
		//LightObjectSet               m_avoidLightObjectSet;
		//LightObjectSet               m_needLightObjectSet;

		ref_ptr<LightParamStruct>    m_lightParamStruct;
        bool                         m_changeEnableInNextFrame;
		bool                         m_changeEnable;
};

}

#endif
