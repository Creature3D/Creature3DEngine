/* Creature3D - Online Game Engine, Copyright (C) 2005 Wucaihua(26756325@qq.com)
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
#include <CRGUI/crTextAttribute.h>
#include <CRCore/crNotify.h>
#include <CRIOManager/crLoadManager.h>
#include <sys/stat.h>
#include <stdio.h>
using namespace CRCore;
using namespace CRGUI;
using namespace CRIOManager;

crTextAttribute::TextAttributeFileMap crTextAttribute::m_textAttributeFileMap;
CRCore::crMutex crTextAttribute::m_textAttrMapMutex;

crTextAttribute::crTextAttribute()
{
	m_textColor.set(0,0,0);
	m_textAlign = TA_LEFT;
	m_linePen = CreatePen(PS_SOLID,0,RGB(0,0,0));
	m_font = CreateFont(
		15,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		PROOF_QUALITY  ,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		"ËÎÌו");                 // lpszFacename
	//HBRUSH              m_bkBrush;
	//HBRUSH              m_frontBrush;
	//HBRUSH              m_shadowBrush;
}
crTextAttribute::~crTextAttribute()
{
	if(m_linePen)
	{
		DeleteObject((HGDIOBJ) m_linePen);
		m_linePen = NULL;
	}
	if(m_font)
	{
		DeleteObject((HGDIOBJ) m_font);
		m_font = NULL;
	}
}
void crTextAttribute::loadTextAttrMap(const std::string &fileName)
{
	CRCore::ScopedLock<CRCore::crMutex> lock(m_textAttrMapMutex);
	rcfg::ConfigScript cfg_script;
	if(!crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,fileName))
	{
		CRCore::notify(CRCore::FATAL)<<"loadTextAttrMap(): file open error,"<<fileName<<std::endl;
		return;
	}

	CRCore::ref_ptr<crTextAttribute> textAttr;
	std::string str;
	int int1;
	std::vector<float> v_i;
	int ntextAttr = 1;
	int pen = 1;
	int font = 1;

	while (cfg_script.Push("TextAttribute", ntextAttr++)) 
	{
		if (cfg_script.Get("name", str)) 
		{
			if(m_textAttributeFileMap.find(str)!=m_textAttributeFileMap.end())
			{
                cfg_script.Pop();		
				continue;
			}
			else 
			{
                textAttr = new crTextAttribute;
				textAttr->m_name = str;
			}
		}
		else
		{
            CRCore::notify(CRCore::FATAL)<<"cfg load error: "<<fileName.c_str()<<"read textAttribute name error"<<std::endl;
			cfg_script.Pop();		
			continue;
		}

		if (cfg_script.Get("TextColor", v_i)) 
		{
			if (v_i.size() != 3)
				throw std::runtime_error("crTextAttribute::loadTextAttrMap(): " + cfg_script.CurrentScope() + "TextColor should be a vector with 3 elements");

			textAttr->setTextColor(v_i[0], v_i[1], v_i[2]);
		}

		if (cfg_script.Get("TextAlign", int1))
			textAttr->setTextAlign(int1);

		pen = 1;
		while (cfg_script.Push("Pen", pen)) 
		{
			int nPenStyle = 0;
			int nPenWidth = 0;
			CRCore::crVector3i nPenColor(0,0,0);

			if (cfg_script.Get("PenStyle", int1))
				nPenStyle = int1;
			if (cfg_script.Get("PenWidth", int1))
				nPenWidth = int1;

			if (cfg_script.Get("PenColor", v_i)) 
			{
				if (v_i.size() != 3)
					throw std::runtime_error("crTextAttribute::loadTextAttrMap(): " + cfg_script.CurrentScope() + "PenColor should be a vector with 3 elements");

				nPenColor.set(v_i[0], v_i[1], v_i[2]);
			}

			if (!cfg_script.Pop())
				CRCore::notify(CRCore::FATAL)<<"crTextAttribute::loadTextAttrMap(): "<<cfg_script.GetLastError()<<std::endl;

			pen++;    

			textAttr->createLinePen(nPenStyle,nPenWidth,nPenColor);
		}

		font = 1;
		while (cfg_script.Push("Font", font)) 
		{
			int nFontHeight  = 15;
			int nFontWidth  = 0;
			int nFontEscapement = 0;
			int nFontOrientation = 0;
			int nFontWeight = 400;
			int nFontItalic = 0;
			int nFontUnderline = 0;
			int nFontStrikeOut = 0;
			int nFontCharSet = 0;
			int nFontOutPrecision = 0;
			int nFontClipPrecision = 400;
			int nFontQuality = 0;
			int nFontPitchAndFamily = 0;
			std::string sFontFacename = "ËÎÌו";

			if (cfg_script.Get("FontHeight", int1))
				nFontHeight = int1;
			if (cfg_script.Get("FontWidth", int1))
				nFontWidth = int1;
			if (cfg_script.Get("FontEscapement", int1))
				nFontEscapement = int1;
			if (cfg_script.Get("FontOrientation", int1))
				nFontOrientation = int1;
			if (cfg_script.Get("FontWeight", int1))
				nFontWeight = int1;
			if (cfg_script.Get("FontItalic", int1))
				nFontItalic = int1;
			if (cfg_script.Get("FontUnderline", int1))
				nFontUnderline = int1;
			if (cfg_script.Get("FontStrikeOut", int1))
				nFontStrikeOut = int1;
			if (cfg_script.Get("FontCharSet", int1))
				nFontCharSet = int1;
			if (cfg_script.Get("FontOutPrecision", int1))
				nFontOutPrecision = int1;
			if (cfg_script.Get("FontClipPrecision", int1))
				nFontClipPrecision = int1;
			if (cfg_script.Get("FontQuality", int1))
				nFontQuality = int1;
			if (cfg_script.Get("FontPitchAndFamily", int1))
				nFontPitchAndFamily = int1;
			if (cfg_script.Get("FontFacename", str))
				sFontFacename = str;

			if (!cfg_script.Pop())
				CRCore::notify(CRCore::FATAL)<<"crTextAttribute::loadTextAttrMap(): "<<cfg_script.GetLastError()<<std::endl;

			font++;    

			textAttr->createFont( nFontHeight,
				nFontWidth,
				nFontEscapement,
				nFontOrientation,
				nFontWeight,
				nFontItalic,
				nFontUnderline,
				nFontStrikeOut,
				nFontCharSet,
				nFontOutPrecision,
				nFontClipPrecision,
				nFontQuality,
				nFontPitchAndFamily,
				sFontFacename );
		}

		
		if (!cfg_script.Pop())
			CRCore::notify(CRCore::FATAL)<<"crTextAttribute::loadTextAttrMap(): "<<cfg_script.GetLastError()<<std::endl;

		m_textAttributeFileMap[textAttr->m_name] = textAttr;
	}


	//if (ntextAttr == 1)
	//	CRCore::notify(CRCore::INFO) << "crTextAttribute::loadTextAttrMap(): Warning: No crTextAttribute loaded from config file" << std::endl;
}

crTextAttribute* crTextAttribute::getTextAttribute(const std::string& name)
{ 
	CRCore::ScopedLock<CRCore::crMutex> lock(m_textAttrMapMutex);
	TextAttributeFileMap::iterator itr = m_textAttributeFileMap.find(name); 
	return itr==m_textAttributeFileMap.end()?NULL:itr->second.get();
}

void crTextAttribute::createLinePen(int nPenStyle,int nWidth,const CRCore::crVector3i& color)
{ 
	if(m_linePen)
		DeleteObject((HGDIOBJ) m_linePen);
	   
	m_linePen = CreatePen(nPenStyle,nWidth,RGB(color[0],color[1],color[2])); 
}

void crTextAttribute::createFont(int nFontHeight,
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
				std::string& sFontFacename)
{
	if(m_font)
		DeleteObject((HGDIOBJ) m_font);

	m_font = CreateFont( nFontHeight,
						 nFontWidth,
						 nFontEscapement,
						 nFontOrientation,
						 nFontWeight,
						 nFontItalic,
						 nFontUnderline,
						 nFontStrikeOut,
						 nFontCharSet,
						 nFontOutPrecision,
						 nFontClipPrecision,
						 nFontQuality,
						 nFontPitchAndFamily,
						 sFontFacename.c_str());
}

void crTextAttribute::drawTextAttribute( Display hdc ) const
{
	SetTextColor(hdc,RGB(m_textColor[0],m_textColor[1],m_textColor[2]));
	SetTextAlign(hdc,m_textAlign);
	SelectObject(hdc,m_linePen);
	SelectObject(hdc,m_font);
	SetBkMode(hdc,TRANSPARENT);
}

void crTextAttribute::endDrawText( Display hdc )
{
	SetBkMode(hdc,OPAQUE);
}

void crText::addText(const CRCore::crVector2i &texCoor, const std::string &str )
{
    m_coordTextMap[texCoor] = str;
}

void crText::addText(const CRCore::crVector4i &texRect, const std::string &str )
{
    m_rectTextMap[texRect] = str;
}

void crText::drawText( Display hdc )
{
	for( CoordTextMap::iterator citr = m_coordTextMap.begin();
		 citr != m_coordTextMap.end();
		 ++citr )
	{
        TextOut(hdc,citr->first.x(),citr->first.y(),citr->second.c_str(),citr->second.length());
//		CRCore::notify(CRCore::FATAL)<<"crText::drawText:"<<citr->second.c_str()<<" length:"<<strlen(citr->second.c_str())<<std::endl;
	}

	RECT rect;
	for( RectTextMap::iterator ritr = m_rectTextMap.begin();
		ritr != m_rectTextMap.end();
		++ritr )
	{
		rect.left   = ritr->first.x();
		rect.top    = ritr->first.y();
		rect.right  = ritr->first.z();
		rect.bottom = ritr->first.w();

        ExtTextOut(hdc,rect.left,rect.top,ETO_CLIPPED,&rect,ritr->second.c_str(),ritr->second.length(),NULL);
	}
}
