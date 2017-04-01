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
#include <CRCore/crDisplaySettings.h>
#include <CRCore/crArgumentParser.h>
#include <CRCore/ref_ptr.h>
#include <CRCore/crBrain.h>

#include <algorithm>

using namespace CRCore;
using namespace std;

ref_ptr<crDisplaySettings> crDisplaySettings::m_instance = NULL;
float crDisplaySettings::s_fpsControl = 0.0f;
void crDisplaySettings::initFpsControl(float fps)
{
	s_fpsControl = fps;
}
void crDisplaySettings::restoreFpsControl()
{
	setFpsControl(s_fpsControl);
}
crDisplaySettings* crDisplaySettings::instance()
{
	if (m_instance==NULL) 
	{
		m_instance = new crDisplaySettings;
		//crBrain::getInstance()->pushInstance(m_instance.get());
	}
	return m_instance.get();
    //static ref_ptr<crDisplaySettings> s_displaySettings = new crDisplaySettings;
    //return s_displaySettings.get();
}

void crDisplaySettings::clear()
{
    //m_instance = NULL;
	//m_initRenderVecBuf.clear();
}

crDisplaySettings::crDisplaySettings(const crDisplaySettings& vs):Referenced()
{
    copy(vs);
	m_windowClosed = false;
}

crDisplaySettings::~crDisplaySettings()
{
}

 
 crDisplaySettings& crDisplaySettings::operator = (const crDisplaySettings& vs)
{
    if (this==&vs) return *this;
    copy(vs);
    return *this;
}

void crDisplaySettings::copy(const crDisplaySettings& vs)
{
    m_displayType = vs.m_displayType;
	m_stereo = vs.m_stereo;
    m_stereoMode = vs.m_stereoMode;
    m_eyeSeparation = vs.m_eyeSeparation;
    m_screenWidth = vs.m_screenWidth;
    m_screenHeight = vs.m_screenHeight;
    m_screenDistance = vs.m_screenDistance;

	m_fovy = vs.m_fovy;
	m_aspectRatio = vs.m_aspectRatio;
	//m_clip = vs.m_clip;
	//m_shear = vs.m_shear;

    m_splitStereoHorizontalEyeMapping = vs.m_splitStereoHorizontalEyeMapping;
    m_splitStereoHorizontalSeparation = vs.m_splitStereoHorizontalSeparation;

    m_splitStereoVerticalEyeMapping = vs.m_splitStereoVerticalEyeMapping;
    m_splitStereoVerticalSeparation = vs.m_splitStereoVerticalSeparation;
    
    m_splitStereoAutoAdjustAspectRatio = vs.m_splitStereoAutoAdjustAspectRatio;

    m_doubleBuffer = vs.m_doubleBuffer;
    m_RGB = vs.m_RGB;
    m_depthBuffer = vs.m_depthBuffer;
    m_minimumNumberAlphaBits = vs.m_minimumNumberAlphaBits;
    m_minimumNumberStencilBits = vs.m_minimumNumberStencilBits;
	m_minimumNumberAccumRedBits = vs.m_minimumNumberAccumRedBits;
	m_minimumNumberAccumGreenBits = vs.m_minimumNumberAccumGreenBits;
	m_minimumNumberAccumBlueBits = vs.m_minimumNumberAccumBlueBits;
	m_minimumNumberAccumAlphaBits = vs.m_minimumNumberAccumAlphaBits;

    m_maxNumOfGraphicsContexts = vs.m_maxNumOfGraphicsContexts;

	m_colorBits = vs.m_colorBits;
	m_depthBits = vs.m_depthBits;

	m_numMultiSamples = vs.m_numMultiSamples;
	m_maxAnisotropy = vs.m_maxAnisotropy;

	m_hdr = vs.m_hdr;
	m_glowSize = vs.m_glowSize;
	m_enableAlphaShadow = vs.m_enableAlphaShadow;

	//m_fog = vs.m_fog;// = 0:nofog, = 1:DepthFog, = 2:NoiseFog
	//m_fogColor = vs.m_fogColor;
 //   m_fogParams = vs.m_fogParams;
    
	//m_waveBlend = vs.m_waveBlend;
	//m_waveAmp = vs.m_waveAmp;
	//m_waveScaleSpeed = vs.m_waveScaleSpeed;

	m_needPixelDepth = vs.m_needPixelDepth;
    //m_maxDepth = vs.m_maxDepth;

	m_depthOfField = vs.m_depthOfField;
	//m_dofblur = vs.m_dofblur;

	m_motionBlur = vs.m_motionBlur;
	//m_motionBlurFactor = vs.m_motionBlurFactor;

    m_shadowDetail = vs.m_shadowDetail;
	m_textureDetail = vs.m_textureDetail;

	m_brightness = vs.m_brightness;
	//m_ambientIntensity = vs.m_ambientIntensity;

	//m_initRenderVecBuf = vs.m_initRenderVecBuf;

	//m_npr = vs.m_npr;
	//m_nprParam = vs.m_nprParam;

	m_vsync = vs.m_vsync;

	m_runMode = vs.m_runMode;

	m_needNormalDepthSample = vs.m_needNormalDepthSample;

	m_fullScreen = vs.m_fullScreen;
	m_windowMaxsize = vs.m_windowMaxsize;
	m_mouseSpeed = vs.m_mouseSpeed;

	m_useBoard = vs.m_useBoard;
	m_viewWidth = vs.m_viewWidth;
	m_viewHeight = vs.m_viewHeight;
	m_disableShader = vs.m_disableShader;
	m_useZperPass = vs.m_useZperPass;
	m_disableLOD = vs.m_disableLOD;
	m_uiWindowPixel = vs.m_uiWindowPixel;
	//m_viewScale = vs.m_viewScale;
	m_fpsControl = vs.m_fpsControl;
	m_outLine = vs.m_outLine;
	m_outLineColor = vs.m_outLineColor;
	m_outLineWidth = vs.m_outLineWidth;
	m_outLineThreshold = vs.m_outLineThreshold;
	m_outLineDispDistance = vs.m_outLineDispDistance;
	m_selectOutLineColor = vs.m_selectOutLineColor;
	m_characterDispDistance = vs.m_characterDispDistance;
	m_characterShadowDistance = vs.m_characterShadowDistance;
	m_characterUpdateFactor = vs.m_characterUpdateFactor;
	m_characterNearDistance = vs.m_characterNearDistance;
	m_characterFarDistance = vs.m_characterFarDistance;
	m_disableSound = vs.m_disableSound;
	m_ambSoundVolume = vs.m_ambSoundVolume;
	m_fxSoundVolume = vs.m_fxSoundVolume;
	m_uiscalemode = vs.m_uiscalemode;
	m_uiscaleFactor = vs.m_uiscaleFactor;
	m_ownWindow = vs.m_ownWindow;
	m_windowClosed = false;
}

void crDisplaySettings::merge(const crDisplaySettings& vs)
{
    if (m_stereo       || vs.m_stereo)        m_stereo = true;
    
    // need to think what to do about merging the stereo mode.
    
    if (m_doubleBuffer || vs.m_doubleBuffer)  m_doubleBuffer = true;
    if (m_RGB          || vs.m_RGB)           m_RGB = true;
    if (m_depthBuffer  || vs.m_depthBuffer)   m_depthBuffer = true;
    
    if (vs.m_minimumNumberAlphaBits>m_minimumNumberAlphaBits) m_minimumNumberAlphaBits = vs.m_minimumNumberAlphaBits;
    if (vs.m_minimumNumberStencilBits>m_minimumNumberStencilBits) m_minimumNumberStencilBits = vs.m_minimumNumberStencilBits;

	if (vs.m_minimumNumberAccumRedBits>m_minimumNumberAccumRedBits) m_minimumNumberAccumRedBits = vs.m_minimumNumberAccumRedBits;
	if (vs.m_minimumNumberAccumGreenBits>m_minimumNumberAccumGreenBits) m_minimumNumberAccumGreenBits = vs.m_minimumNumberAccumGreenBits;
	if (vs.m_minimumNumberAccumBlueBits>m_minimumNumberAccumBlueBits) m_minimumNumberAccumBlueBits = vs.m_minimumNumberAccumBlueBits;
	if (vs.m_minimumNumberAccumAlphaBits>m_minimumNumberAccumAlphaBits) m_minimumNumberAccumAlphaBits = vs.m_minimumNumberAccumAlphaBits;

	if (vs.m_colorBits>m_colorBits) m_colorBits = vs.m_colorBits;
	if (vs.m_depthBits>m_depthBits) m_depthBits = vs.m_depthBits;
	if (vs.m_numMultiSamples>m_numMultiSamples) m_numMultiSamples = vs.m_numMultiSamples;
	if (vs.m_maxAnisotropy>m_maxAnisotropy) m_maxAnisotropy = vs.m_maxAnisotropy;

	if (m_enableAlphaShadow       || vs.m_enableAlphaShadow)        m_enableAlphaShadow = true;

	//m_fog = CRCore::maximum(m_fog,vs.m_fog);
	//m_waveBlend = CRCore::maximum(m_waveBlend,vs.m_waveBlend);

	if (m_needPixelDepth || vs.m_needPixelDepth)        m_needPixelDepth = true;
	//m_maxDepth = CRCore::maximum(m_maxDepth,vs.m_maxDepth);

	m_glowSize = CRCore::maximum(m_glowSize,vs.m_glowSize);

	if (m_depthOfField || vs.m_depthOfField) m_depthOfField = true;
	//m_dofblur = CRCore::maximum(m_dofblur,vs.m_dofblur);

	if (m_motionBlur || vs.m_motionBlur) m_motionBlur = true;
	//m_motionBlurFactor = CRCore::maximum(m_motionBlur,vs.m_motionBlur);

	m_hdr = CRCore::maximum(m_hdr,vs.m_hdr);

	if (m_needNormalDepthSample || vs.m_needNormalDepthSample) m_needNormalDepthSample = true;
	//m_initRenderVecBuf.insert(m_initRenderVecBuf.end(),vs.m_initRenderVecBuf.begin(),vs.m_initRenderVecBuf.end());
	if (m_fullScreen || vs.m_fullScreen) m_fullScreen = true;
	if (m_windowMaxsize || vs.m_windowMaxsize) m_windowMaxsize = true;
	if (m_disableShader || vs.m_disableShader) m_disableShader = true;
	if (m_useZperPass || vs.m_useZperPass) m_useZperPass = true;
	if (!m_disableLOD || !vs.m_disableLOD) m_disableLOD = false;
	if (!m_uiscalemode || !vs.m_uiscalemode) m_uiscalemode = 0;
}

void crDisplaySettings::setDefaults()
{
    m_displayType = MONITOR;

    m_stereo = false;
    m_stereoMode = ANAGLYPHIC;
    m_eyeSeparation = 0.05f;
    m_screenWidth = 1024;//0.325f;
    m_screenHeight = 768;//0.26f;
    m_screenDistance = 0.5f;

	//m_fov.set(70,50);
	m_fovy = 50.0f;
	m_aspectRatio = 1.3333f;
	m_sightLimit = cos(CRCore::DegreesToRadians(m_fovy * 0.5f));
	//m_clip.set(1.0,1e7);

    m_splitStereoHorizontalEyeMapping = LEFT_EYE_LEFT_VIEWPORT;
    m_splitStereoHorizontalSeparation = 0;

    m_splitStereoVerticalEyeMapping = LEFT_EYE_TOP_VIEWPORT;
    m_splitStereoVerticalSeparation = 0;

    m_splitStereoAutoAdjustAspectRatio = true;

    m_doubleBuffer = true;
    m_RGB = true;
    m_depthBuffer = true;
    m_minimumNumberAlphaBits = 0;
    m_minimumNumberStencilBits = 0;
	m_minimumNumberAccumRedBits = 0;
	m_minimumNumberAccumGreenBits = 0;
	m_minimumNumberAccumBlueBits = 0;
	m_minimumNumberAccumAlphaBits = 0;

    m_maxNumOfGraphicsContexts = 1;

	m_colorBits = 32;
	m_depthBits = 24;

	m_numMultiSamples = 0;
	m_maxAnisotropy = 1;

	m_hdr = 0;
	m_glowSize = 8.0f;

	m_enableAlphaShadow = false;

	/*m_fog = 0;
	m_waveBlend = 0;
	m_waveAmp = 0.0f;
	m_needPixelDepth = false;
	m_maxDepth = 10000.0f;*/

	m_depthOfField = 0;
	//m_dofblur = 0.0375f;

	m_motionBlur = 0;
	//m_motionBlurFactor = 0.25f;

	m_shadowDetail = 0;
	m_textureDetail = 1;
	m_brightness = 0.0f;

	//m_npr = 0;
	//m_nprParam.set(0.0009765625,0.1,0.0049);
	//m_hdrParam.set(0.163029659,0.152222769,0.123913497,0.087939422);

	m_vsync = false;

	m_runMode = 1;

	m_needNormalDepthSample = false;
    m_needPixelDepth = false;

	m_fullScreen = false;
	m_windowMaxsize = false;

	m_mouseSpeed = 0.3f;
	m_useBoard = true;
	m_viewWidth = m_screenWidth;
	m_viewHeight = m_screenHeight;

	m_disableShader = false;
	m_useZperPass = false;
	m_disableLOD = false;
	m_uiWindowPixel = 2000;
	//m_viewScale.set(1.0f,1.0f);
	m_fpsControl = 0.0f;
	m_outLine = false;
	m_outLineWidth = 3.0f;
	m_outLineThreshold = 0.0049f;
	m_outLineDispDistance = 60.0f;
	m_selectOutLineColor.set(1.0f,0.0f,0.0f);
	m_characterDispDistance = 60.0f;
	m_characterShadowDistance = 30.0f;
	m_characterUpdateFactor = 0.1f;
	m_characterNearDistance = 15.0f;
	m_characterFarDistance = 5.0f;

	m_uiDesignWidth = 1440;
	m_uiDesignHeight = 900;

	m_disableSound = false;
	m_ambSoundVolume = 1.0f;
	m_fxSoundVolume = 1.0f;
	m_uiscalemode = 0;
	m_uiscaleFactor = 1.0f;
	m_ownWindow = true;
	m_windowClosed = false;
}

void crDisplaySettings::setMinimumNumAccumBits(unsigned int red, unsigned int green, unsigned int blue, unsigned int alpha)
{
	m_minimumNumberAccumRedBits = red;
	m_minimumNumberAccumGreenBits = green;
	m_minimumNumberAccumBlueBits = blue;
	m_minimumNumberAccumAlphaBits = alpha;
}

void crDisplaySettings::setGlowSize( float size )
{
    m_glowSize = size;
}

float crDisplaySettings::getGlowSize() const
{
    return m_glowSize;
}
//
//void crDisplaySettings::setFog( int fog )
//{
//    m_fog = fog;
//}
//
//int crDisplaySettings::getFog() const
//{
//	return m_fog;
//}
//
//void crDisplaySettings::setFogColor( const crVector3 &color )
//{
//    m_fogColor = color;
//}
//
//const crVector3& crDisplaySettings::getFogColor() const
//{
//	return m_fogColor;
//}
//
//void crDisplaySettings::setFogParams( const crVector3 &sed )
//{
//    m_fogParams = sed;
//}
//
//const crVector3& crDisplaySettings::getFogParams() const
//{
//    return m_fogParams;
//}

//void crDisplaySettings::setWaveBlend( int wave )
//{
//	m_waveBlend = wave;
//}
//
//int crDisplaySettings::getWaveBlend() const
//{
//	return m_waveBlend;
//}
//
//void crDisplaySettings::setWaveScaleSpeed( const crVector2 &ss )
//{
//    m_waveScaleSpeed = ss;
//}
//
//const crVector2& crDisplaySettings::getWaveScaleSpeed() const
//{
//	return m_waveScaleSpeed;
//}
//
//void crDisplaySettings::setWaveAmp( float amp )
//{
//	m_waveAmp = amp;
//}
//
//float crDisplaySettings::getWaveAmp() const
//{
//	return m_waveAmp;
//}
//
//void crDisplaySettings::setNoiseSample( const crVector4 &noise )
//{
//    m_noiseSample = noise;
//}
//
//const crVector4& crDisplaySettings::getNoiseSample() const
//{
//    return m_noiseSample;
//}

void crDisplaySettings::setNeedPixelDepth(bool bln) 
{ 
	m_needPixelDepth = bln; 
}

bool crDisplaySettings::getNeedPixelDepth() 
{ 
	return m_needPixelDepth; 
}

//void crDisplaySettings::setMaxDepth(float maxDepth) 
//{ 
//	m_maxDepth = maxDepth; 
//}
//
//float crDisplaySettings::getMaxDepth() 
//{ 
//	return m_maxDepth; 
//}

void crDisplaySettings::setDepthOfField(bool dof)
{
    m_depthOfField = dof;
	setNeedNormalDepthSample(true);
	setNeedPixelDepth(true);
}

bool crDisplaySettings::getDepthOfField()
{
    return m_depthOfField && m_hdr /*&& m_numMultiSamples<=1*/;
}

//void crDisplaySettings::setDofBlur( float blur )
//{
//    m_dofblur = blur;
//}
//
//float crDisplaySettings::getDofBlur() const
//{
//    return m_dofblur;
//}

void crDisplaySettings::setMotionBlur( bool blur )
{
	m_motionBlur = blur;
}

bool crDisplaySettings::getMotionBlur() const
{
	return m_motionBlur;
}

//void crDisplaySettings::setMotionBlurFactor( float blur )
//{
//	m_motionBlurFactor = blur;
//}
//
//float crDisplaySettings::getMotionBlurFactor() const
//{
//	return m_motionBlurFactor;
//}

void crDisplaySettings::setShadowDetail( int detail )
{
    m_shadowDetail = detail;
}

int crDisplaySettings::getShadowDetail() const
{
	return m_shadowDetail;
}

void crDisplaySettings::setTextureDetail( int detail )
{
    m_textureDetail = detail;
}

int crDisplaySettings::getTextureDetail() const
{
    return m_textureDetail;
}

//void crDisplaySettings::setAmbientIntensity(const crVector4& color)
//{
//    m_ambientIntensity = color;
//}
//
//const crVector4 &crDisplaySettings::getAmbientIntensity() const
//{
//    return m_ambientIntensity;
//}

void crDisplaySettings::setBrightness( float brightness )
{
    m_brightness = brightness;
}

float crDisplaySettings::getBrightness() const
{
    return m_brightness;
}

//void crDisplaySettings::setNPR( int npr )
//{
//	m_npr = npr;
//}
//
//int crDisplaySettings::getNPR() const
//{
//	return m_npr;
//}
//
//void crDisplaySettings::setNprParam(const crVector3& param)
//{
//	m_nprParam = param;
//}
//
//const crVector3 &crDisplaySettings::getNprParam() const
//{
//	return m_nprParam;
//}
void crDisplaySettings::setVsync( bool vsync )
{
    m_vsync = vsync;
}

bool crDisplaySettings::getVsync() const
{
    return m_vsync;
}

void crDisplaySettings::setRunMode(int runMode) 
{ 
	m_runMode = runMode; 
}

void crDisplaySettings::setNeedNormalDepthSample( bool bln )
{
	m_needNormalDepthSample = bln;
}

bool crDisplaySettings::getNeedNormalDepthSample() const
{
	return m_needNormalDepthSample && m_hdr /*&& m_numMultiSamples<=1*/;
}

void crDisplaySettings::setFullScreen( bool bln )
{
	m_fullScreen = bln;
}
bool crDisplaySettings::getFullScreen() const
{
	return m_fullScreen;
}
void crDisplaySettings::setWindowMaxsize( bool bln )
{
	m_windowMaxsize = bln;
}
bool crDisplaySettings::getWindowMaxsize() const
{
	return m_windowMaxsize;
}
void crDisplaySettings::setMouseSpeed( float mouseSpeed )
{
	m_mouseSpeed = mouseSpeed;
}
float crDisplaySettings::getMouseSpeed() const
{
	return m_mouseSpeed;
}
void crDisplaySettings::setUseBoard(bool useBoard)
{
    m_useBoard = useBoard;
}
bool crDisplaySettings::getUseBoard()const
{
	return m_useBoard;
}
void crDisplaySettings::setDisableShader(bool disableShader)
{
	m_disableShader = disableShader;
	if(m_disableShader)
	{
		setHDR(0);
		setShadowDetail(0);
		setDepthOfField(false);
	}
}
bool crDisplaySettings::getDisableShader()const
{
	return m_disableShader;
}
void crDisplaySettings::setDisableLOD(bool disable)
{
	m_disableLOD = disable;
}
void crDisplaySettings::setUIWindowPixel(int uipixel)
{
	m_uiWindowPixel = uipixel;
}
float crDisplaySettings::getUIPixelMeter() const
{
	return 2.0f/m_uiWindowPixel;
}
//void crDisplaySettings::calcViewScale()
//{
//	m_viewScale.set(m_standardWinSize[0]/m_viewWidth,m_standardWinSize[1]/m_viewHeight);
//	//if(m_viewScale[0]<1.0f)
//	//	m_viewScale[0] = 1.0f;
//}
//float crDisplaySettings::getFontScale()
//{
//	return 1.0f;//getWidthScale();
//}
//float crDisplaySettings::getWidthScale()
//{
//	return m_viewScale[0];
//}
//float crDisplaySettings::getHeightScale()
//{
//	return m_viewScale[1];
//}
float crDisplaySettings::getUIScale()
{
	if(m_uiscalemode == 0)//UI不随分辨缩放
		return m_uiWindowPixel/m_viewWidth*m_uiscaleFactor;
	else
		return m_uiWindowPixel/m_uiDesignWidth;//UI随分辨率缩放
}
void crDisplaySettings::setViewSize(int width,int height)
{
	if(width>0 && height>0)
	{
		m_viewWidth = width%2 == 0 ? width : width-1;
		m_viewHeight = height%2 == 0 ? height : height-1; 
	}
}
void crDisplaySettings::setFpsControl(float fps)
{
	if(fps<=0.0f)
		m_fpsControl = 0.0f;
	else
		m_fpsControl = CRCore::clampTo(1000.0f/fps,0.0f,100.0f);
}
void crDisplaySettings::setOutLine(bool outline)
{
	m_outLine = outline;
}
bool crDisplaySettings::getOutLine()
{
	return m_outLine;
}
void crDisplaySettings::setOutLineColor(const crVector3 &color)
{
	m_outLineColor = color;
}
const crVector3 &crDisplaySettings::getOutLineColor()
{
	return m_outLineColor;
}
void crDisplaySettings::setOutLineWidth(float width)
{
	m_outLineWidth = width;
}
float crDisplaySettings::getOutLineWidth()
{
	return m_outLineWidth;
}
void crDisplaySettings::setOutLineThreshold(float threshold)
{
	m_outLineThreshold = threshold;
}
float crDisplaySettings::getOutLineThreshold()
{
	return m_outLineThreshold;
}
void crDisplaySettings::setOutLineDispDistance(float dis)
{
	m_outLineDispDistance = dis;
}
void crDisplaySettings::setSelectOutLineColor(const crVector3 &color)
{
	m_selectOutLineColor = color;
}
const crVector3 &crDisplaySettings::getSelectOutLineColor()
{
	return m_selectOutLineColor;
}
void crDisplaySettings::setCharacterDispDistance(float dis)
{
	m_characterDispDistance = dis;
}
void crDisplaySettings::setCharacterShadowDistance(float dis)
{
	m_characterShadowDistance = dis;
}
void crDisplaySettings::setCharacterUpdateFactor(float factor)
{
	m_characterUpdateFactor = factor;
}
void crDisplaySettings::setCharacterNearDistance(float dist)
{
	m_characterNearDistance = dist;
}
void crDisplaySettings::setCharacterFarDistance(float dist)
{
	m_characterFarDistance = dist;
}
void crDisplaySettings::setupUIScaleMatrix()
{
	float uiScale = getUIScale();
	float heightScale = getUIHeightScale();
	m_uiscaleMatrix = crMatrix::scale(uiScale,heightScale,1.0f);
	m_uiscaleMatrix_inv = crMatrix::inverse(m_uiscaleMatrix);
}
const crMatrix& crDisplaySettings::getUIScaleMatrix()
{
	return m_uiscaleMatrix;
}
const crMatrix& crDisplaySettings::getInvUIScaleMatrix()
{
	return m_uiscaleMatrix_inv;
}
float crDisplaySettings::getUIHeightScale()
{
	return m_viewWidth/m_viewHeight * getUIScale();
	//return m_uiDesignWidth/m_uiDesignHeight*getUIScale();
}
void crDisplaySettings::setUIDesignWidth(float width)
{
	if(width>1.0f)
		m_uiDesignWidth = width;
}
float crDisplaySettings::getUIDesignWidth()
{
	return m_uiDesignWidth;
}
void crDisplaySettings::setUIDesignHeight(float height)
{
	if(height>1.0f)
		m_uiDesignHeight = height;
}
float crDisplaySettings::getUIDesignHeight()
{
	return m_uiDesignHeight;
}
crVector2 crDisplaySettings::getUIViewScale()
{
	////return crVector2(m_viewWidth/m_uiDesignWidth,m_viewHeight/m_uiDesignHeight);
	if(m_uiscalemode == 0)//UI不随分辨缩放
		return crVector2(m_uiscaleFactor,m_uiscaleFactor);
	else
		return crVector2(m_viewWidth/m_uiDesignWidth,m_viewWidth/m_uiDesignWidth);//UI随分辨率缩放
}