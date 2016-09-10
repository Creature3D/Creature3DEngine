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
//Modified by Wucaihua
#include <CRCore/crCullSettings.h>
#include <CRCore/crArgumentParser.h>

#include <CRCore/crNotify.h>

using namespace CRCore;
#undef _CRT_SECURE_NO_DEPRECATE
crCullSettings::crCullSettings(const crCullSettings& cs)
{
    setCullSettings(cs);
}

void crCullSettings::setDefaults()
{
	m_inheritanceMask = ALL_VARIABLES;
    m_cullingMode = ENABLE_ALL_CULLING;//DEFAULT_CULLING;
    m_LODScale = 1.0f;
    m_smallFeatureCullingPixelSize = 2.0f;

    m_computeNearFar = DO_NOT_COMPUTE_NEAR_FAR;//COMPUTE_NEAR_FAR_USING_BOUNDING_VOLUMES;//;
	m_renderMode = NormalRender;
    m_nearFarRatio = 0.0005f;
    m_impostorActive = true;
    m_depthSortImpostorSprites = false;
    m_impostorPixelErrorThreshold = 4.0f;
    m_numFramesToKeepImpostorSprites = 10;
    m_cullMask = 0xffffffff;
    m_cullMaskLeft = 0xffffffff;
    m_cullMaskRight = 0xffffffff;

    // override during testing
    //m_computeNearFar = COMPUTE_NEAR_FAR_USING_PRIMITIVES;
    //m_nearFarRatio = 0.00005f;
}

void crCullSettings::inheritCullSettings(const crCullSettings& settings, unsigned int inheritanceMask)
{
	if (inheritanceMask & COMPUTE_NEAR_FAR_MODE) m_computeNearFar = settings.m_computeNearFar;
	if (inheritanceMask & NEAR_FAR_RATIO) m_nearFarRatio = settings.m_nearFarRatio;
	if (inheritanceMask & IMPOSTOR_ACTIVE) m_impostorActive = settings.m_impostorActive;
	if (inheritanceMask & DEPTH_SORT_IMPOSTOR_SPRITES) m_depthSortImpostorSprites = settings.m_depthSortImpostorSprites;
	if (inheritanceMask & IMPOSTOR_PIXEL_ERROR_THRESHOLD) m_impostorPixelErrorThreshold = settings.m_impostorPixelErrorThreshold;
	if (inheritanceMask & NUM_FRAMES_TO_KEEP_IMPOSTORS_SPRITES) m_numFramesToKeepImpostorSprites = settings.m_numFramesToKeepImpostorSprites;
	if (inheritanceMask & CULL_MASK) m_cullMask = settings.m_cullMask;
	if (inheritanceMask & CULL_MASK_LEFT) m_cullMaskLeft = settings.m_cullMaskLeft;
	if (inheritanceMask & CULL_MASK_RIGHT) m_cullMaskRight = settings.m_cullMaskRight;
	if (inheritanceMask & CULLING_MODE) m_cullingMode = settings.m_cullingMode;
	if (inheritanceMask & LOD_SCALE) m_LODScale = settings.m_LODScale;
	if (inheritanceMask & SMALL_FEATURE_CULLING_PIXEL_SIZE) m_smallFeatureCullingPixelSize = settings.m_smallFeatureCullingPixelSize;
	if (inheritanceMask & CLAMP_PROJECTION_MATRIX_CALLBACK) m_clampProjectionMatrixCallback = settings.m_clampProjectionMatrixCallback;
	if (inheritanceMask & RENDER_MODE) m_renderMode = settings.m_renderMode;
	//CRCore::notify(CRCore::ALWAYS)<<"m_computeNearFar = "<<m_computeNearFar<<std::endl;
	//CRCore::notify(CRCore::ALWAYS)<<"m_nearFarRatio = "<<m_nearFarRatio<<std::endl;

}

void crCullSettings::readEnvironmentalVariables()
{
    //CRCore::notify(CRCore::INFO)<<"crCullSettings::readEnvironmentalVariables()"<<std::endl;

    char *ptr;
    if ((ptr = getenv("CR_COMPUTE_NEAR_FAR_MODE")) != 0)
    {
        if (strcmp(ptr,"DO_NOT_COMPUTE_NEAR_FAR")==0) m_computeNearFar = DO_NOT_COMPUTE_NEAR_FAR;
        else if (strcmp(ptr,"COMPUTE_NEAR_FAR_USING_BOUNDING_VOLUMES")==0) m_computeNearFar = COMPUTE_NEAR_FAR_USING_BOUNDING_VOLUMES;
        else if (strcmp(ptr,"COMPUTE_NEAR_FAR_USING_PRIMITIVES")==0) m_computeNearFar = COMPUTE_NEAR_FAR_USING_PRIMITIVES;

       // CRCore::notify(CRCore::INFO)<<"Set compute near far mode to "<<m_computeNearFar<<std::endl;
        
    }
    
    if ((ptr = getenv("CR_NEAR_FAR_RATIO")) != 0)
    {
        m_nearFarRatio = atof(ptr);

       // CRCore::notify(CRCore::INFO)<<"Set near/far ratio to "<<m_nearFarRatio<<std::endl;
    }
    
}
