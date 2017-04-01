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
#ifndef CRCORE_CRDISPLAYSETTINGS_H
#define CRCORE_CRDISPLAYSETTINGS_H 1

#include <CRCore/Referenced.h>
#include <CRCore/ref_ptr.h>
#include <CRCore/crVector2.h>
#include <CRCore/crVector3.h>
#include <CRCore/crVector4.h>
#include <CRCore/crMath.h>
#include <CRCore/crMatrix.h>
#include <CRCore/crMath.h>
#include <string>
#include <vector>

namespace CRCore {

// forward declare
class crArgumentParser;

/** crDisplaySettings class for encapsulating what visuals are required and
  * have been set up, and the status of stereo viewing.*/
class CR_EXPORT crDisplaySettings : public CRCore::Referenced
{

    public:

        /** Maintain a crDisplaySettings singleton for objects to querry at runtime.*/
        static crDisplaySettings* instance();


        crDisplaySettings()
        {
            setDefaults();
        }
        
        crDisplaySettings(crArgumentParser& arguments)
        {
            setDefaults();
        }

        crDisplaySettings(const crDisplaySettings& vs);
        

        crDisplaySettings& operator = (const crDisplaySettings& vs);
 
        
        void merge(const crDisplaySettings& vs);

        void setDefaults();
       
        enum DisplayType
        {
            MONITOR,
            POWERWALL,
            REALITY_CENTER,
            HEAD_MOUNTED_DISPLAY
        };

        void setDisplayType(DisplayType type) { m_displayType = type; }
        
        DisplayType getDisplayType() const { return m_displayType; }


        void setStereo(bool on) { m_stereo = on; }
        bool getStereo() const { return m_stereo; }

        enum StereoMode
        {
			QUAD_BUFFER,
			ANAGLYPHIC,
			HORIZONTAL_SPLIT,
			VERTICAL_SPLIT,
			LEFT_EYE,
			RIGHT_EYE,
			HORIZONTAL_INTERLACE,
			VERTICAL_INTERLACE
        };
        
        void setStereoMode(StereoMode mode) { m_stereoMode = mode; }
        StereoMode getStereoMode() const { return m_stereoMode; }

        void setEyeSeparation(float eyeSeparation) { m_eyeSeparation = eyeSeparation; }
        float getEyeSeparation() const { return m_eyeSeparation; }

        enum SplitStereoHorizontalEyeMapping
        {
            LEFT_EYE_LEFT_VIEWPORT,
            LEFT_EYE_RIGHT_VIEWPORT
        };
        
        void setSplitStereoHorizontalEyeMapping(SplitStereoHorizontalEyeMapping m) { m_splitStereoHorizontalEyeMapping = m; }
        SplitStereoHorizontalEyeMapping getSplitStereoHorizontalEyeMapping() const { return m_splitStereoHorizontalEyeMapping; }

        void setSplitStereoHorizontalSeparation(int s) { m_splitStereoHorizontalSeparation = s; }
        int getSplitStereoHorizontalSeparation() const { return m_splitStereoHorizontalSeparation; }

        enum SplitStereoVerticalEyeMapping
        {
            LEFT_EYE_TOP_VIEWPORT,
            LEFT_EYE_BOTTOM_VIEWPORT
        };

        void setSplitStereoVerticalEyeMapping(SplitStereoVerticalEyeMapping m) { m_splitStereoVerticalEyeMapping = m; }
        SplitStereoVerticalEyeMapping getSplitStereoVerticalEyeMapping() const { return m_splitStereoVerticalEyeMapping; }

        void setSplitStereoVerticalSeparation(int s) { m_splitStereoVerticalSeparation = s; }
        int getSplitStereoVerticalSeparation() const { return m_splitStereoVerticalSeparation; }

        void setSplitStereoAutoAjustAspectRatio(bool flag) { m_splitStereoAutoAdjustAspectRatio=flag; }
        bool getSplitStereoAutoAjustAspectRatio() const { return m_splitStereoAutoAdjustAspectRatio; }


        void setScreenWidth(float width) { if(width>1.0f) m_screenWidth = width; }
        float getScreenWidth() const { return m_screenWidth; }

        void setScreenHeight(float height) { if(height>1.0f) m_screenHeight = height; }
        float getScreenHeight() const { return m_screenHeight; }

        void setScreenDistance(float distance) { m_screenDistance = distance; }
        float getScreenDistance() const { return m_screenDistance; }

		//void setFov(const crVector2 &fov) { m_fov = fov; m_sightLimit = cos(CRCore::DegreesToRadians(m_fov[0] * 0.5f)); }
		//const crVector2 &getFov() const { return m_fov; }

		void setFovy(float fovy) { m_fovy = fovy; m_sightLimit = cos(CRCore::DegreesToRadians(m_fovy * 0.5f)); }
		float getFovy() const { return m_fovy; }

		void setAspectRatio(float ratio) { m_aspectRatio = ratio;}
		float getAspectRatio() const { return m_viewWidth/m_viewHeight; }

		inline float getSightLimit() { return m_sightLimit; }

		//void setShear(const crVector2 &shear) { m_shear = shear; }
		//const crVector2 &getShear() const { return m_shear; }

		//void setClip(const crVector2 &clip) { m_clip = clip; }
		//const crVector2 &getClip() const { return m_clip; }

        void setDoubleBuffer(bool flag) { m_doubleBuffer = flag; }
        bool getDoubleBuffer() const { return m_doubleBuffer; }

        void setRGB(bool flag) { m_RGB = flag; }
        bool getRGB() const { return m_RGB; }

        void setDepthBuffer(bool flag) { m_depthBuffer = flag; }
        bool getDepthBuffer() const { return m_depthBuffer; }

        void setMinimumNumAlphaBits(unsigned int bits) { m_minimumNumberAlphaBits = bits; }
        unsigned int getMinimumNumAlphaBits() const { return m_minimumNumberAlphaBits; }
        bool getAlphaBuffer() const { return m_minimumNumberAlphaBits!=0; }

        void setMinimumNumStencilBits(unsigned int bits) { m_minimumNumberStencilBits = bits; }
        unsigned int getMinimumNumStencilBits() const { return m_minimumNumberStencilBits; }
        bool getStencilBuffer() const { return m_minimumNumberStencilBits!=0; }

		void setMinimumNumAccumBits(unsigned int red, unsigned int green, unsigned int blue, unsigned int alpha);
		unsigned int getMinimumNumAccumRedBits() const { return m_minimumNumberAccumRedBits; }
		unsigned int getMinimumNumAccumGreenBits() const { return m_minimumNumberAccumGreenBits; }
		unsigned int getMinimumNumAccumBlueBits() const { return m_minimumNumberAccumBlueBits; }
		unsigned int getMinimumNumAccumAlphaBits() const { return m_minimumNumberAccumAlphaBits; }
		bool getAccumBuffer() const { return (m_minimumNumberAccumRedBits+m_minimumNumberAccumGreenBits+m_minimumNumberAccumBlueBits+m_minimumNumberAccumAlphaBits)!=0; }

        void setMaxNumberOfGraphicsContexts(unsigned int num) { m_maxNumOfGraphicsContexts = num; }
        unsigned int getMaxNumberOfGraphicsContexts() const { return m_maxNumOfGraphicsContexts; }
        
		void setColorBits(unsigned int bits ) { m_colorBits = bits; }
		unsigned int getColorBits() const { return m_colorBits; }

		void setDepthBits(unsigned int bits ) { m_depthBits = bits; }
		unsigned int getDepthBits() const { return m_depthBits; }

		void setNumMultiSamples(unsigned int samples ) { m_numMultiSamples = samples; }
		unsigned int getNumMultiSamples() const { return m_numMultiSamples; }
		
		inline bool useHardWareAA() { return !m_hdr && m_numMultiSamples>1; }
		inline bool useFXAA() { return m_hdr && m_numMultiSamples>1; }

		void setMaxAnisotropy(unsigned int anisotry ) { m_maxAnisotropy = maximum((unsigned int)2,anisotry); }
		unsigned int getMaxAnisotropy() const { return maximum((unsigned int)2,m_maxAnisotropy); }

		//void setAmbientIntensity(const crVector4& color);
  //      const crVector4 &getAmbientIntensity() const;

		void setHDR(int hdr) { m_hdr = hdr; }
		inline int getHDR() { return m_hdr; }

		void setGlowSize( float size );
		float getGlowSize() const;

		void setEnableAlphaShadow(bool enableAlphaShadow) { m_enableAlphaShadow = enableAlphaShadow; }
		inline bool getEnableAlphaShadow() { return m_enableAlphaShadow; }

		//void setFog( int fog );
		//int getFog() const;

		//void setFogColor( const crVector3 &color );
  //      const crVector3& getFogColor() const;

		//void setFogParams( const crVector3 &sed );
		//const crVector3& getFogParams() const;

		//void setWaveBlend( int wave );
		//int getWaveBlend() const;

		//void setWaveAmp( float waveAmp );
		//float getWaveAmp() const;

		//void setWaveScaleSpeed( const crVector2 &ss );
		//const crVector2& getWaveScaleSpeed() const;

		//void setNoiseSample( const crVector4 &noise );
		//const crVector4& getNoiseSample() const;

		void setNeedPixelDepth(bool bln);
		bool getNeedPixelDepth();

		//void setMaxDepth(float maxDepth);
		//float getMaxDepth();

		void setDepthOfField(bool dof);
		bool getDepthOfField();

		//void setDofBlur( float blur );
		//float getDofBlur() const;

		void setMotionBlur(bool blur);
		bool getMotionBlur()const;

		//void setMotionBlurFactor( float blur );
		//float getMotionBlurFactor() const;

		void setShadowDetail( int detail );
		int getShadowDetail() const;
		
		void setTextureDetail( int detail );
		int getTextureDetail() const;
        
		void setBrightness( float brightness );
		float getBrightness() const;

		virtual void clear();

		//void setNPR( int npr );
		//int getNPR() const;
		//void setNprParam( const crVector3 &param );
		//const crVector3& getNprParam() const;

		void setVsync( bool vsync );
		bool getVsync() const;

		enum RunMode
		{
			Editor,
			NetGame,
			SingleGame,
			WebGame,
			Robot,
			RobotNoTexture
		};
		void setRunMode(int runMode);//0:Editor, 1:NetGame,2:SingleGame,3WebGame,4Robot,5RobotNoTexture
		inline int getRunMode()const { return m_runMode; }
		inline void setOwnWindow(bool own){ m_ownWindow = own; }
		inline bool isOwnWindow()const { return m_ownWindow; }
		inline void close(){ m_windowClosed = true; }
		inline bool isWindowClosed()const { return m_windowClosed; }
		void setNeedNormalDepthSample( bool bln );
		bool getNeedNormalDepthSample() const;
		void setFullScreen( bool bln );
		bool getFullScreen() const;
		void setWindowMaxsize( bool bln );
		bool getWindowMaxsize() const;

		void setMouseSpeed( float mouseSpeed );
		float getMouseSpeed() const;

		void setUseBoard(bool useBoard);
		bool getUseBoard()const;
		void setViewSize(int width,int height);
		inline float getViewWidth(){ return m_viewWidth; }
		inline float getViewHeight(){ return m_viewHeight; }

		void setDisableShader(bool disableShader);
		bool getDisableShader() const;
		void setUseZperPass(bool useZperPass){ m_useZperPass = useZperPass; }
		inline bool getUseZperPass()const { return m_useZperPass; }

		void setDisableLOD(bool disable);
		inline bool getDisableLOD() const { return m_disableLOD; }
		void setUIWindowPixel(int uipixel);
		float getUIScale();
		float getUIPixelMeter() const;//获得UI 1像素占多少米
		//void calcViewScale();
		//float getFontScale();
		//float getWidthScale();
		//float getHeightScale();
		void setFpsControl(float fps);
		inline float getFpsControl() { return m_fpsControl; }//返回ms fps = 1000.0f/ms
		void setOutLine(bool outline);
		bool getOutLine();
		void setOutLineColor(const crVector3 &color);
		const crVector3 &getOutLineColor();
		void setOutLineWidth(float width);
		float getOutLineWidth();
		void setOutLineThreshold(float threshold);// shader outline
		float getOutLineThreshold();
		void setOutLineDispDistance(float dis);
		inline float getOutLineDispDistance() { return m_outLineDispDistance; }
		void setSelectOutLineColor(const crVector3 &color);
		const crVector3 &getSelectOutLineColor();
		void setCharacterDispDistance(float dis);
		inline float getCharacterDispDistance() { return m_characterDispDistance; }
		void setCharacterShadowDistance(float dis);
		inline float getCharacterShadowDistance() { return m_characterShadowDistance; }
		void setCharacterUpdateFactor(float factor);
		inline float getCharacterUpdateFactor() { return m_characterUpdateFactor; }
		void setCharacterNearDistance(float dis);
		inline float getCharacterNearDistance() { return m_characterNearDistance; }
		void setCharacterFarDistance(float dis);
		inline float getCharacterFarDistance() { return m_characterFarDistance; }

		void setupUIScaleMatrix();
		const crMatrix& getUIScaleMatrix();
		const crMatrix& getInvUIScaleMatrix();
		float getUIHeightScale();
		void setUIDesignWidth(float width);
		float getUIDesignWidth();
		void setUIDesignHeight(float height);
		float getUIDesignHeight();
		crVector2 getUIViewScale();
		///声音设置
		void setDisableSound(bool bln){ m_disableSound = bln; }
		bool getDisableSound() { return m_disableSound; }
		void setAmbSoundVolume(float volume) { m_ambSoundVolume = volume; }
		float getAmbSoundVolume() { return m_ambSoundVolume; }
		void setFxSoundVolume(float volume) { m_fxSoundVolume = volume; }
		float getFxSoundVolume() { return m_fxSoundVolume; }
		static void initFpsControl(float fps);
		void restoreFpsControl();
		inline void setUIScaleMode(unsigned char mode){ m_uiscalemode = mode; }
		inline unsigned char getUIScaleMode(){ return m_uiscalemode; }
		inline void setUIScaleFactor(float factor){ m_uiscaleFactor = factor; }
		inline float getUIScaleFactor(){ return m_uiscaleFactor; }
    protected:
    
        virtual ~crDisplaySettings();
		static ref_ptr<crDisplaySettings> m_instance;

        void copy(const crDisplaySettings& vs);

        DisplayType                     m_displayType;
        bool                            m_stereo;
        StereoMode                      m_stereoMode;
        float                           m_eyeSeparation;
        float                           m_screenWidth;
        float                           m_screenHeight;
        float                           m_screenDistance;
        float                           m_sightLimit;
		//crVector2                       m_fov;//xfov,yfov
		float                           m_fovy;
		float                           m_aspectRatio;
		//crVector2                       m_clip;
        //crVector2                       m_shear;//xshear,yshear

        SplitStereoHorizontalEyeMapping m_splitStereoHorizontalEyeMapping;
        int                             m_splitStereoHorizontalSeparation;
        SplitStereoVerticalEyeMapping   m_splitStereoVerticalEyeMapping;
        int                             m_splitStereoVerticalSeparation;
        bool                            m_splitStereoAutoAdjustAspectRatio;
    
        bool                            m_doubleBuffer;
        bool                            m_RGB;
        bool                            m_depthBuffer;
        unsigned int                    m_minimumNumberAlphaBits;
        unsigned int                    m_minimumNumberStencilBits;
		unsigned int                    m_minimumNumberAccumRedBits;
		unsigned int                    m_minimumNumberAccumGreenBits;
		unsigned int                    m_minimumNumberAccumBlueBits;
		unsigned int                    m_minimumNumberAccumAlphaBits;

        unsigned int                    m_maxNumOfGraphicsContexts;

        unsigned int                    m_colorBits;
		unsigned int                    m_depthBits;

		unsigned int                    m_numMultiSamples;
		unsigned int                    m_maxAnisotropy;

		int                             m_hdr;
		float                           m_glowSize;

		bool                            m_enableAlphaShadow;

		//int                             m_fog;// = 0:nofog, = 1:DepthFog, = 2:NoiseFog
		//crVector3                       m_fogColor;
		//crVector3                       m_fogParams;//(Start,End,Density)

		//int                             m_waveBlend;
		//float                           m_waveAmp;
		//crVector2                       m_waveScaleSpeed;
		//crVector4                       m_noiseSample;//texSize: [16,128] startFrequency [2, 16] numOctaves[2, 6] amp

		//crVector4                       m_ambientIntensity;//LightModel AmbientIntensity

		bool                            m_needPixelDepth;
        //float                           m_maxDepth;
        
		bool                            m_depthOfField;
		bool                            m_outLine;
		crVector3                       m_outLineColor;//
		float                           m_outLineWidth;
		float                           m_outLineThreshold;
		float                           m_outLineDispDistance;
		crVector3                       m_selectOutLineColor;//
		//float                           m_dofblur;

		bool                            m_motionBlur;
		//float                           m_motionBlurFactor;

		int                             m_shadowDetail;//0(关闭),1(Low),2(Mid),3(Height)
		int                             m_textureDetail;//0(Low),1,2(Height)
        float                           m_brightness;

		//int                             m_npr;
		//crVector3                       m_nprParam;

		bool                            m_vsync;

		int                             m_runMode;//0:Editor, 1:Game,2:SingleGame,3WebGame
		bool                            m_needNormalDepthSample;
		bool                            m_fullScreen;
		bool                            m_windowMaxsize;//窗口最大化
		/////////////非显示设置
        float                           m_mouseSpeed;

		bool                            m_useBoard;
		float                           m_viewWidth;
		float                           m_viewHeight;
		bool                            m_disableShader;
		bool                            m_useZperPass;
		bool                            m_disableLOD;
		//crVector2f						m_standardWinSize;
		//crVector2f						m_viewScale;
		float                           m_fpsControl;//帧率控制0：全速, 每帧的毫秒上限。
		static float                    s_fpsControl;
		float                           m_uiWindowPixel;//UI窗口宽2米占多少像素
		float                           m_characterDispDistance;
		float                           m_characterShadowDistance;
		float                           m_characterUpdateFactor;
		float                           m_characterNearDistance;
		float                           m_characterFarDistance;
                 
		crMatrix                        m_uiscaleMatrix;
		crMatrix                        m_uiscaleMatrix_inv;
		float                           m_uiDesignWidth;//ui设计分辨率
		float                           m_uiDesignHeight;
		////////音量设置
		bool                            m_disableSound;
		float                           m_ambSoundVolume;
		float                           m_fxSoundVolume;
		unsigned char                   m_uiscalemode;//0：不随分辨缩放，1：UI随分辨率缩放
		float                           m_uiscaleFactor;//UI缩放因子
		bool                            m_ownWindow;
		bool                            m_windowClosed;
};

}

# endif
