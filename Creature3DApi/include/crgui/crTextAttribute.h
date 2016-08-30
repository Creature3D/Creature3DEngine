/* Creature3D - Online Game Engine, Copyright (C) 2005 Îâ²Æ»ª(26756325@qq.com)
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
#ifndef CRGUI_CRTextAttribute_H
#define CRGUI_CRTextAttribute_H 1

#include <CRGUI/crExport.h>
#include <CRGUI\crTypes.h>
#include <CRGUI/crWin32.h>
#include <CRCore/crVector2.h>
#include <CRCore/crVector3.h>
#include <CRCore/crVector4.h>
#include <CRCore/crCopyOp.h>
#include <CRCore/thread/crMutex.h>
#include "rcfg/ConfigScriptOO.h"
#include <vector>
#include <map>
#include <string>

namespace CRGUI{

class CRGUI_EXPORT crTextAttribute : public CRCore::Referenced
{
public:
	crTextAttribute();
	void setTextColor(const CRCore::crVector3i& color){ m_textColor = color; }
	void setTextColor(int r, int g, int b){ m_textColor.set(r,g,b); }
    void setTextAlign(unsigned int align){ m_textAlign = align; }
	void createLinePen(int nPenStyle,int nWidth,const CRCore::crVector3i& color);
	void createFont(int nFontHeight,
					int nFontWidth,
					int nFontEscapement,
					int nFontOrientation,
					int nFontWeight,
					int nFontItalic,
					int nFontUnderline,
					int nFontStrikeOut,
					int nFontCharSet,
					int nFontOutPrecision,
					int nFontClipPrecision,
					int nFontQuality,
					int nFontPitchAndFamily,
					std::string& sFontFacename);

    void setLinePen(HPEN pen){ m_linePen = pen; }
	void setFont( HFONT font ){ m_font = font; }
 //   void setBKBrush( HBRUSH bkbrush ){ m_bkBrush = bkbrush; }
 //   void setFrontBrush( HBRUSH frontBrush ){ m_frontBrush = frontBrush; }
	//void setShadowBrush( HBRUSH ShadowBrush ){ m_shadowBrush = ShadowBrush; }

	inline bool operator == (const crTextAttribute& textAttr) const 
	{ 
		return m_name.compare(textAttr.m_name);
	}
	inline bool operator != (const crTextAttribute& textAttr) const 
	{
		return !(*this==textAttr);
	}
	inline bool operator < (const crTextAttribute& textAttr) const 
	{
		return  (m_name.compare(textAttr.m_name) < 0);
	}
	virtual void drawTextAttribute( Display hdc ) const;
	static void endDrawText( Display hdc );

	typedef std::map<std::string,CRCore::ref_ptr<crTextAttribute> > TextAttributeFileMap;
	static void loadTextAttrMap(const std::string &fileName);
	static crTextAttribute* getTextAttribute(const std::string& name);
protected:
	static TextAttributeFileMap m_textAttributeFileMap;
	static CRCore::crMutex m_textAttrMapMutex;
	virtual ~crTextAttribute();
	std::string m_name;
	CRCore::crVector3i  m_textColor;
	unsigned int        m_textAlign;
	HPEN                m_linePen;
	HFONT               m_font;
	//HBRUSH              m_bkBrush;
	//HBRUSH              m_frontBrush;
	//HBRUSH              m_shadowBrush;
};

class CRGUI_EXPORT crText : public CRCore::Referenced
{
public:
	crText(){}
	typedef std::map<CRCore::crVector2i,std::string> CoordTextMap;
	typedef std::map<CRCore::crVector4i,std::string> RectTextMap;//texRect x:left y:top z:right w:bottom

	void addText(const CRCore::crVector2i &texCoor, const std::string &str );
	void addText(const CRCore::crVector4i &texRect, const std::string &str );
    virtual void drawText( Display hdc );
	void setTextSound(const std::string& textSound){ m_textSound = textSound; }
	const std::string& getTextSound()const { return m_textSound; }

	inline bool operator == (const crText& text) const 
	{
		if(m_textSound.compare(text.m_textSound)!=0)
			return false;

		if(m_coordTextMap.size() != text.m_coordTextMap.size())
			return false;
		if(m_rectTextMap.size() != text.m_rectTextMap.size() )
			return false;
		
		CoordTextMap::const_iterator ctmapiter;
		for( CoordTextMap::const_iterator citr = m_coordTextMap.begin();
			 citr != m_coordTextMap.end();
			 ++citr )
		{
            ctmapiter = text.m_coordTextMap.find(citr->first);
			if(ctmapiter == text.m_coordTextMap.end())
				return false;
			if(ctmapiter->second.compare(citr->second)!=0)
				return false;
		}

		RectTextMap::const_iterator rtmapiter;
		for( RectTextMap::const_iterator ritr = m_rectTextMap.begin();
			ritr != m_rectTextMap.end();
			++ritr )
		{
			rtmapiter = text.m_rectTextMap.find(ritr->first);
			if(rtmapiter == text.m_rectTextMap.end())
				return false;
			if(rtmapiter->second.compare(ritr->second)!=0)
				return false;
		}

		return true;
        
	}
	inline bool operator != (const crText& text) const 
	{
		return !(*this==text);
	}
	inline bool operator < (const crText& text) const 
	{
		if(m_textSound.compare(text.m_textSound)>0)
			return false;
        if(*this == text)
			return false;
		return true;
	}
protected:
    CoordTextMap m_coordTextMap;
	RectTextMap m_rectTextMap;
	std::string m_textSound;
};

}
#endif