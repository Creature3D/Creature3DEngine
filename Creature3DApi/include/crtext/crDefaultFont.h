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

#ifndef CRTEXT_DEFAULTFONT_H
#define CRTEXT_DEFAULTFONT_H 1

#include <map>

#include <CRCore/ref_ptr.h>

#include <CRText/crFont.h>

namespace CRText {

class crDefaultFont : public crFont
{
public:

	static crDefaultFont* instance();

    virtual std::string getFileName() const { return ""; }

    virtual bool supportsMultipleFontResolutions() const { return false; }

    virtual CRText::crGlyph* getGlyph(const FontResolution& fontRes, unsigned int charcode);
    
    virtual CRText::crGlyph3D* getGlyph3D(unsigned int charcode) { return 0; }

    virtual CRCore::crVector2 getKerning(unsigned int leftcharcode,unsigned int rightcharcode, KerningType kerningType);
    
    virtual bool hasVertical() const;

    virtual float getScale() const { return 1.0; }
	virtual void clear();
protected:
	static CRCore::ref_ptr<crDefaultFont> m_instance;

	crDefaultFont();
    virtual ~crDefaultFont();
    
    void constructGlyphs();
    
    
};

}

#endif
