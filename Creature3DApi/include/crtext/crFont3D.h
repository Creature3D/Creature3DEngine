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


#ifndef CRTEXT_CRFONT3D
#define CRTEXT_CRFONT3D 1

#include <CRText/crFont.h>

namespace CRText {

typedef crFont crFont3D;

/** deprecated, use readFontFile() instead.*/
inline void readFont3DFile(CRCore::ref_ptr<crFont> &outFont,const std::string& filename, const CRIOManager::crReaderWriter::Options* userOptions = 0)
{
    readFontFile(outFont,filename,userOptions); 
}

/** deprecated, use readFontStream() instead.*/
//inline crFont* readFont3DStream(std::istream& stream, const CRIOManager::crReaderWriter::Options* userOptions = 0)
//{
//    return readFontStream(stream, userOptions);
//}

/** deprecated, use readRefFontFile() instead.*/
//inline CRCore::ref_ptr<crFont> readRefFont3DFile(const std::string& filename, const CRIOManager::crReaderWriter::Options* userOptions = 0)
//{
//    return readRefFontFile(filename, userOptions);
//}

/** deprecated, use readRefFontStream() instead.*/
//inline CRCore::ref_ptr<crFont> readRefFont3DStream(std::istream& stream, const CRIOManager::crReaderWriter::Options* userOptions = 0)
//{
//    return readRefFontStream(stream, userOptions);
//}

/** deprecated, use findFontFile() instead.*/
inline std::string findFont3DFile(const std::string& str)
{
    return findFontFile(str);
}

}

#endif
