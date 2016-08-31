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
#ifndef FREETYPE_LIBRARY
#define FREETYPE_LIBRARY

#include <Plugin Freetype/FreeTypeFont.h>

#include <set>
#include <istream>

#include <CRText/crFont.h>
#include <CRText/crFont3D.h>

#include <ft2build.h>

class FreeTypeLibrary : public CRCore::Referenced
{
public:

    /** protected destrcutor to prevent inappropriate deletion.*/
    virtual ~FreeTypeLibrary();

    /** get the singleton instance.*/
    static FreeTypeLibrary* instance();

    CRCore::crMutex& getMutex() { return m_mutex; }

    CRText::crFont* getFont(const std::string& fontfile,unsigned int index=0, unsigned int flags=0);
    CRText::crFont* getFont(std::istream& fontstream, unsigned int index=0, unsigned int flags=0);
    
    void removeFontImplmentation(FreeTypeFont* fontImpl) { m_fontImplementationSet.erase(fontImpl); }

protected:

    /** common method to load a FT_Face from a file*/
    bool getFace(const std::string& fontfile,unsigned int index, FT_Face & face);
    /** common method to load a FT_Face from a stream */
    FT_Byte* getFace(std::istream& fontstream, unsigned int index, FT_Face & face);
    
    /** Verify the correct character mapping for MS windows */
    void  verifyCharacterMap(FT_Face face);

    /** protected constructor to ensure the only way to create the 
      * library is via the singleton instance method.*/
    FreeTypeLibrary();

    typedef std::set< FreeTypeFont* > FontImplementationSet;

    mutable CRCore::crMutex  m_mutex;
    FT_Library                  m_ftlibrary;
    FontImplementationSet       m_fontImplementationSet;

};


#endif
