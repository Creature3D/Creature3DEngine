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
#ifndef CRCORE_CULLSETTINGS_H
#define CRCORE_CULLSETTINGS_H 1

#include <CRCore/crExport.h>
#include <CRCore/crMatrix.h>
#include <CRCore/crClearNode.h>
#include <CRCore/crArgumentParser.h>

namespace CRCore {

class CR_EXPORT crCullSettings
{
    public:

        crCullSettings()
        {
            setDefaults();
            readEnvironmentalVariables();
        }

        crCullSettings(CRCore::crArgumentParser& arguments)
        {
            setDefaults();
            readEnvironmentalVariables();
        }

        crCullSettings(const crCullSettings& cs);

        virtual ~crCullSettings() {}
        
        crCullSettings& operator = (const crCullSettings& settings)
        {
            if (this==&settings) return *this;
            setCullSettings(settings);
            return *this;
        }
        
        void setDefaults();

		enum VariablesMask
		{
			COMPUTE_NEAR_FAR_MODE                   = 0x0001,
			CULLING_MODE                            = 0x0002,
			LOD_SCALE                               = 0x0004,
			SMALL_FEATURE_CULLING_PIXEL_SIZE        = 0x0008,
			CLAMP_PROJECTION_MATRIX_CALLBACK        = 0x0010,
			NEAR_FAR_RATIO                          = 0x0020,
			IMPOSTOR_ACTIVE                         = 0x0040,
			DEPTH_SORT_IMPOSTOR_SPRITES             = 0x0080,
			IMPOSTOR_PIXEL_ERROR_THRESHOLD          = 0x0100,
			NUM_FRAMES_TO_KEEP_IMPOSTORS_SPRITES    = 0x0200,
			CULL_MASK                               = 0x0400,
			CULL_MASK_LEFT                          = 0x0800,
			CULL_MASK_RIGHT                         = 0x1000,
			RENDER_MODE                             = 0x2000,

			NO_VARIABLES                            = 0x0000,
			ALL_VARIABLES                           = 0xFFFF
		};

		/** Set the inheritance mask used in inheritCullSettings to control which variables get overritten by the passed in crCullSettings object.*/
		void setInheritanceMask(unsigned int mask) { m_inheritanceMask = mask; }

		/** Get the inheritance mask used in inheritCullSettings to control which variables get overritten by the passed in crCullSettings object.*/
		unsigned int getInheritanceMask() const { return m_inheritanceMask; }

		/** Set the local cull settings values from specified CullSettings object.*/
		void setCullSettings(const crCullSettings& settings) { inheritCullSettings(settings, ALL_VARIABLES); }

		/** Inherit the local cull settings variable from specified CullSettings object, according to the inheritance mask.*/
		void inheritCullSettings(const crCullSettings& settings) { inheritCullSettings(settings, m_inheritanceMask); }

		/** Inherit the local cull settings variable from specified CullSettings object, according to the inheritance mask.*/
		void inheritCullSettings(const crCullSettings& settings, unsigned int inheritanceMask);

        /** read the environmental variables.*/
        void readEnvironmentalVariables();

        /** Switch the creation of Impostors on or off.
          * Setting active to false forces the CullVisitor to use the Impostor
          * LOD children for rendering. Setting active to true forces the
          * CullVisitor to create the appropriate pre-rendering stages which
          * render to the ImpostorSprite's texture.*/
        void setImpostorsActive(bool active) { m_impostorActive = active; }

        /** Get whether impostors are active or not. */
        bool getImpostorsActive() const { return m_impostorActive; }

        /** Set the impostor error threshold.
          * Used in calculation of whether impostors remain valid.*/
        void setImpostorPixelErrorThreshold(float numPixels) { m_impostorPixelErrorThreshold=numPixels; }

        /** Get the impostor error threshold.*/
        float getImpostorPixelErrorThreshold() const { return m_impostorPixelErrorThreshold; }

        /** Set whether ImpostorSprite's should be placed in a depth sorted bin for rendering.*/
        void setDepthSortImpostorSprites(bool doDepthSort) { m_depthSortImpostorSprites = doDepthSort; }

        /** Get whether ImpostorSprite's are depth sorted bin for rendering.*/
        bool getDepthSortImpostorSprites() const { return m_depthSortImpostorSprites; }

        /** Set the number of frames that an ImpostorSprite's is kept whilst not being beyond,
          * before being recycled.*/
        void setNumberOfFrameToKeepImpostorSprites(int numFrames) { m_numFramesToKeepImpostorSprites = numFrames; }

        /** Get the number of frames that an ImpostorSprite's is kept whilst not being beyond,
          * before being recycled.*/
        int getNumberOfFrameToKeepImpostorSprites() const { return m_numFramesToKeepImpostorSprites; }

        enum ComputeNearFarMode
        {
            DO_NOT_COMPUTE_NEAR_FAR = 0,
            COMPUTE_NEAR_FAR_USING_BOUNDING_VOLUMES,
            COMPUTE_NEAR_FAR_USING_PRIMITIVES
        };

        void setComputeNearFarMode(ComputeNearFarMode cnfm) { m_computeNearFar=cnfm; } 
        ComputeNearFarMode getComputeNearFarMode() const { return m_computeNearFar;} 

        void setNearFarRatio(double ratio) { m_nearFarRatio = ratio; }
        double getNearFarRatio() const { return m_nearFarRatio; }

		enum RenderMode
		{
			NormalRender,
			LightingRender,
			WaterReflectRender,
			CubeMapGainRender,
			StaitcShadowMapRender,
			ShadowMapRender,
			OnlyGeodeRender,
			ZprePassRender,
			CollideMapRender,
			RTTRender,
			NRPRender,
		};
		inline void setRenderMode( RenderMode mode ){ m_renderMode = mode; }
		inline RenderMode getRenderMode(){ return m_renderMode; }

        enum CullingModeValues
        {
            NO_CULLING                  = 0x0,
            VIEW_FRUSTUM_SIDES_CULLING  = 0x1,
            NEAR_PLANE_CULLING          = 0x2,
            FAR_PLANE_CULLING           = 0x4,
            VIEW_FRUSTUM_CULLING        = VIEW_FRUSTUM_SIDES_CULLING|
                                          NEAR_PLANE_CULLING|
                                          FAR_PLANE_CULLING,
            SMALL_FEATURE_CULLING       = 0x8,
            SHADOW_OCCLUSION_CULLING    = 0x10,
            CLUSTER_CULLING             = 0x20,
			EXTERN_FRUSTUM_CULLING      = 0x40,
			EXTERN_OCCLUSION_CULLING    = 0x80,
            DEFAULT_CULLING             = VIEW_FRUSTUM_SIDES_CULLING|
                                          SMALL_FEATURE_CULLING|
                                          SHADOW_OCCLUSION_CULLING|
                                          CLUSTER_CULLING,
            ENABLE_ALL_CULLING          = VIEW_FRUSTUM_CULLING|
                                          SMALL_FEATURE_CULLING|
                                          SHADOW_OCCLUSION_CULLING|
                                          CLUSTER_CULLING
        };
        
        typedef unsigned int CullingMode;

        /** Set the culling mode for the CullVisitor to use.*/
        void setCullingMode(CullingMode mode) { m_cullingMode = mode; }
        
        /** Returns the current CullingMode.*/
        CullingMode getCullingMode() const { return m_cullingMode; }


        void setCullMask(const CRCore::crNode::NodeMask nm) { m_cullMask = nm; }
        CRCore::crNode::NodeMask getCullMask() const { return m_cullMask; }

        void setCullMaskLeft(const CRCore::crNode::NodeMask nm) { m_cullMaskLeft = nm; }
        CRCore::crNode::NodeMask getCullMaskLeft() const { return m_cullMaskLeft; }

        void setCullMaskRight(const CRCore::crNode::NodeMask nm) { m_cullMaskRight = nm; }
        CRCore::crNode::NodeMask getCullMaskRight() const { return m_cullMaskRight; }

        /** Set the LOD bias for the CullVisitor to use.*/
        void setLODScale(float bias) { m_LODScale = bias; }
        
        /** Get the LOD bias.*/
        float getLODScale() const { return m_LODScale; }

        /** Set the Small Feature Culling Pixel Size.*/
        void setSmallFeatureCullingPixelSize(float value) { m_smallFeatureCullingPixelSize=value; }

        /** Get the Small Feature Culling Pixel Size.*/
        float getSmallFeatureCullingPixelSize() const { return m_smallFeatureCullingPixelSize; }


        /** Callback for overriding the CullVisitor's default clamping of the projection matrix to computed near and far values.
          * Note, both crMatrixf and crMatrixd versions of clampProjectionMatrixImplementation must be implemented as the CullVisitor
          * can target either crMatrix data type, configured at compile time.*/
        struct ClampProjectionMatrixCallback : public CRCore::Referenced
        {
            virtual bool clampProjectionMatrixImplementation(CRCore::crMatrixf& projection, double& znear, double& zfar) const = 0;
            virtual bool clampProjectionMatrixImplementation(CRCore::crMatrixd& projection, double& znear, double& zfar) const = 0;
        };
        
        /** set the ClampProjectionMatrixCallback.*/
        void setClampProjectionMatrixCallback(ClampProjectionMatrixCallback* cpmc) { m_clampProjectionMatrixCallback = cpmc; }
        /** get the non const ClampProjectionMatrixCallback.*/
        ClampProjectionMatrixCallback* getClampProjectionMatrixCallback() { return m_clampProjectionMatrixCallback.get(); }
        /** get the const ClampProjectionMatrixCallback.*/
        const ClampProjectionMatrixCallback* getClampProjectionMatrixCallback() const { return m_clampProjectionMatrixCallback.get(); }




    protected:

		unsigned int                                m_inheritanceMask;

        ComputeNearFarMode                          m_computeNearFar;
        CullingMode                                 m_cullingMode;
		RenderMode                                  m_renderMode;
        float                                       m_LODScale;
        float                                       m_smallFeatureCullingPixelSize;

		CRCore::ref_ptr<ClampProjectionMatrixCallback>      m_clampProjectionMatrixCallback;
        double                                      m_nearFarRatio;
        bool                                        m_impostorActive;
        bool                                        m_depthSortImpostorSprites;
        float                                       m_impostorPixelErrorThreshold;
        int                                         m_numFramesToKeepImpostorSprites;	

        CRCore::crNode::NodeMask                    m_cullMask;
        CRCore::crNode::NodeMask                    m_cullMaskLeft;
        CRCore::crNode::NodeMask                    m_cullMaskRight;
 

};


}

#endif
