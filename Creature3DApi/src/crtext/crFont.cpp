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
#include <CRText/crFont.h>
#include <CRText/crText.h>

#include <CRCore/crState.h>
#include <CRCore/crNotify.h>

#include <CRIOManager/crReadFile.h>
#include <CRIOManager/crFileUtils.h>
#include <CRIOManager/crFileNameUtils.h>
#include <crgl/glu.h>

#include <string.h>

#include <CRText/crDefaultFont.h>

using namespace CRText;
using namespace std;

//CRCore::ref_ptr<crFont>& crDefaultFont::instance()
//{
//    static CRCore::crMutex s_DefaultFontMutex;
//    CRCore::ScopedLock<CRCore::crMutex> lock(s_DefaultFontMutex);
//
//    static CRCore::ref_ptr<crFont> s_defaultFont = new crDefaultFont;
//    return s_defaultFont;
//}

std::string CRText::findFontFile(const std::string& str)
{
	// try looking in OSGFILEPATH etc first for fonts.
	std::string filename = CRIOManager::findDataFile(str);
	if (!filename.empty()) return std::string(filename);


	static CRIOManager::FilePathList s_FontFilePath;
	static bool initialized = false;
	if (!initialized)
	{
		initialized = true;
#if defined(WIN32)
		CRIOManager::convertStringPathIntoFilePathList(
			".;C:/winnt/fonts;C:/windows/fonts",
			s_FontFilePath);

		char *ptr;
		if ((ptr = getenv( "windir" )))
		{
			s_FontFilePath.push_back(ptr);
		}
#else
		CRIOManager::convertStringPathIntoFilePathList(
			".:/usr/share/fonts/ttf:/usr/share/fonts/ttf/western:/usr/share/fonts/ttf/decoratives",
			s_FontFilePath);
#endif
	}

	filename = CRIOManager::findFileInPath(str,s_FontFilePath);
	if (!filename.empty()) return filename;
//#if defined(WIN32)
//	MessageBox(::GetDesktopWindow(),"缺少字体文件"+str,"错误",MB_OK);
//#endif
	CRCore::notify(CRCore::WARN)<<"Warning: font file \""<<str<<"\" not found."<<std::endl;    
	return std::string();
}

void CRText::readFontFile(CRCore::ref_ptr<CRText::crFont>&outFont, const std::string& filename, const CRIOManager::crReaderWriter::Options* userOptions)
{
    if (filename.empty()) return;

    std::string foundFile = findFontFile(filename);
    if (foundFile.empty())
        foundFile = filename;
    
    //CRCore::ScopedLock<CRCore::crMutex> lock(getFontFileMutex());

    CRCore::ref_ptr<CRIOManager::crReaderWriter::Options> localOptions;
    if (!userOptions)
    {
        localOptions = new CRIOManager::crReaderWriter::Options;
        localOptions->setObjectCacheHint(CRIOManager::crReaderWriter::Options::CACHE_OBJECTS);
    }

    outFont = dynamic_cast<CRText::crFont*>(CRIOManager::readObjectFile(foundFile, userOptions ? userOptions : localOptions.get()));

    // if the object is a font then return it.
    //CRText::crFont* font = dynamic_cast<CRText::crFont*>(object.get());
	if(outFont.valid())
	{
		outFont->setName(filename);
		crFont::FontImplementation* imp = outFont->getImplementation();
		if(imp) imp->m_facade = outFont.get();
		//return font;
	}
    // otherwise if the object has zero references then delete it by doing another unref().
    //if (object && object->referenceCount()==0) object->unref();
    //return 0;
}

//CRText::crFont* CRText::readFontStream(std::istream& stream, const CRIOManager::crReaderWriter::Options* userOptions)
//{
//    //CRCore::ScopedLock<CRCore::crMutex> lock(getFontFileMutex());
//
//    CRCore::ref_ptr<CRIOManager::crReaderWriter::Options> localOptions;
//    if (!userOptions)
//    {
//        localOptions = new CRIOManager::crReaderWriter::Options;
//        localOptions->setObjectCacheHint(CRIOManager::crReaderWriter::Options::CACHE_OBJECTS);
//    }
//
//    // there should be a better way to get the FreeType crReaderWriter by name...
//    CRIOManager::crReaderWriter *reader = CRIOManager::crRegistry::instance()->getReaderWriterForExtension("ttf");
//    if (reader == 0) return 0;
//    CRIOManager::crReaderWriter::ReadResult rr = reader->readObject(stream, userOptions ? userOptions : localOptions.get());
//    if (rr.error())
//    {
//        CRCore::notify(CRCore::WARN) << rr.message() << std::endl;
//        return 0;
//    }
//    if (!rr.validObject()) return 0;
//    
//    CRCore::crBase *object = rr.takeObject();
//
//    // if the object is a font then return it.
//    CRText::crFont* font = dynamic_cast<CRText::crFont*>(object);
//    if (font) return font;
//
//    // otherwise if the object has zero references then delete it by doing another unref().
//    if (object && object->referenceCount()==0) object->unref();
//    return 0;
//}
//
crFont::crFont(FontImplementation* implementation):
    CRCore::crBase(),
    m_margin(1),
    m_marginRatio(0.02),
    m_textureWidthHint(256),
    m_textureHeightHint(256),
    m_minFilterHint(CRCore::crTexture::LINEAR/*_MIPMAP_LINEAR*/),
    m_magFilterHint(CRCore::crTexture::LINEAR),
    m_depth(1),
    m_numCurveSamples(10)
{
    setImplementation(implementation);

    //m_texenv = new CRCore::crTexEnv;
    //m_stateset = new CRCore::crStateSet;
    //m_stateset->setRenderingHint(CRCore::crStateSet::TRANSPARENT_BIN);
}

crFont::~crFont()
{
    if (m_implementation.valid()) m_implementation->m_facade = 0;
	m_implementation=NULL;
}

void crFont::setImplementation(FontImplementation* implementation)
{
    if (m_implementation.valid()) m_implementation->m_facade = 0;
    m_implementation = implementation;
    if (m_implementation.valid()) m_implementation->m_facade = this;
}

crFont::FontImplementation* crFont::getImplementation()
{
    return m_implementation.get();
}

const crFont::FontImplementation* crFont::getImplementation() const
{
    return m_implementation.get();
}

std::string crFont::getFileName() const
{
    if (m_implementation.valid()) return m_implementation->getFileName();
    return std::string();
}

void crFont::setGlyphImageMargin(unsigned int margin)
{
    m_margin = margin;
}

unsigned int crFont::getGlyphImageMargin() const
{
    return m_margin;
}

void crFont::setGlyphImageMarginRatio(float ratio)
{
    m_marginRatio = ratio;
}

float crFont::getGlyphImageMarginRatio() const
{
    return m_marginRatio;
}

void crFont::setTextureSizeHint(unsigned int width,unsigned int height)
{
    m_textureWidthHint = width;
    m_textureHeightHint = height;
}

unsigned int crFont::getTextureWidthHint() const
{
    return m_textureWidthHint;
}

unsigned int crFont::getTextureHeightHint() const
{
    return m_textureHeightHint;
}    


void crFont::setMinFilterHint(CRCore::crTexture::FilterMode mode)
{
    m_minFilterHint = mode;
}

CRCore::crTexture::FilterMode crFont::getMinFilterHint() const
{
    return m_minFilterHint;
}

/** Set the magnification texture filter to use when creating the texture to store the glyph images when rendering.
  * Note, this doesn't affect already created crTexture Glhph's.*/
void crFont::setMagFilterHint(CRCore::crTexture::FilterMode mode)
{
    m_magFilterHint = mode;
}

CRCore::crTexture::FilterMode crFont::getMagFilterHint() const
{
    return m_magFilterHint;
}


crGlyph* crFont::getGlyph(const FontResolution& fontRes, unsigned int charcode)
{
    if (!m_implementation) return 0;

    FontResolution fontResUsed(0,0);
    if (m_implementation->supportsMultipleFontResolutions()) fontResUsed = fontRes;
        
    {
        CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_glyphMapMutex);
        FontSizeGlyphMap::iterator itr = m_sizeGlyphMap.find(fontResUsed);
        if (itr!=m_sizeGlyphMap.end())
        {
            GlyphMap& glyphmap = itr->second;
            GlyphMap::iterator gitr = glyphmap.find(charcode);
            if (gitr!=glyphmap.end()) return gitr->second.get();
        }
    }

    crGlyph* glyph = m_implementation->getGlyph(fontResUsed, charcode);
    if (glyph)
    {
        addGlyph(fontResUsed, charcode, glyph);
        return glyph;
    }
    else return 0;
}

crGlyph3D* crFont::getGlyph3D(unsigned int charcode)
{
    {
        CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_glyphMapMutex);
        Glyph3DMap::iterator itr = m_glyph3DMap.find(charcode);
        if (itr!=m_glyph3DMap.end()) return itr->second.get();
    }

    crGlyph3D* glyph = m_implementation.valid() ? m_implementation->getGlyph3D(charcode) : 0;
    if (glyph)
    {
        CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_glyphMapMutex);
        m_glyph3DMap[charcode] = glyph;
        return glyph;
    }
    return 0;
}

//void crFont::setThreadSafeRefUnref(bool threadSafe)
//{
//   CRCore::crBase::setThreadSafeRefUnref(threadSafe);
//    
//    if (m_texenv.valid()) m_texenv->setThreadSafeRefUnref(threadSafe);
//    if (m_stateset.valid()) m_stateset->setThreadSafeRefUnref(threadSafe);
//
//    for(GlyphTextureList::const_iterator itr=m_glyphTextureList.begin();
//        itr!=m_glyphTextureList.end();
//        ++itr)
//    {
//        (*itr)->setThreadSafeRefUnref(threadSafe);
//    }    
//}

//void crFont::resizeGLObjectBuffers(unsigned int maxSize)
//{
//    if (m_stateset.valid()) m_stateset->resizeGLObjectBuffers(maxSize);
//
//    for(GlyphTextureList::const_iterator itr=m_glyphTextureList.begin();
//        itr!=m_glyphTextureList.end();
//        ++itr)
//    {
//        (*itr)->resizeGLObjectBuffers(maxSize);
//    }    
//}

void crFont::releaseObjects(CRCore::crState* state)
{
	//CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_glyphMapMutex);
 //   ////if (m_stateset.valid()) m_stateset->releaseObjects(state);

 //   for(GlyphTextureList::iterator itr=m_glyphTextureList.begin();
 //       itr!=m_glyphTextureList.end();
 //       ++itr)
 //   {
 //       (*itr)->releaseObjects(state);
 //   }
	//m_glyphTextureList.clear();
	////for( SizeGlyphMap::iterator itr=m_sizeGlyphMap.begin();
	////	itr!=m_sizeGlyphMap.end();
	////	++itr )
	////{
	////	GlyphMap& glyphmap = itr->second;
	////	for( GlyphMap::iterator gitr = glyphmap.begin();
	////		gitr != glyphmap.end();
	////		++gitr )
	////	{
	////		gitr->second->releaseObjects(state);
	////	}
	////}
	////if (m_implementation.valid()) m_implementation->m_facade = 0;

 //   // const_cast<crFont*>(this)->m_glyphTextureList.clear();
 //   // const_cast<crFont*>(this)->m_sizeGlyphMap.clear();
}

CRCore::crVector2 crFont::getKerning(unsigned int leftcharcode,unsigned int rightcharcode, KerningType kerningType)
{
    if (m_implementation.valid()) return m_implementation->getKerning(leftcharcode,rightcharcode,kerningType);
    else return CRCore::crVector2(0.0f,0.0f);
}

bool crFont::hasVertical() const
{
    if (m_implementation.valid()) return m_implementation->hasVertical();
    else return false;
}



void crFont::addGlyph(const FontResolution& fontRes, unsigned int charcode, crGlyph* glyph)
{
    CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_glyphMapMutex);

    m_sizeGlyphMap[fontRes][charcode]=glyph;
    
    int posX=0,posY=0;
    
    crGlyphTexture* glyphTexture = 0;
    for(GlyphTextureList::iterator itr=m_glyphTextureList.begin();
        itr!=m_glyphTextureList.end() && !glyphTexture;
        ++itr)
    {
        if ((*itr)->getSpaceForGlyph(glyph,posX,posY)) glyphTexture = itr->get();
    }
    
    if (glyphTexture)
    {
        //cout << "    found space for texture "<<glyphTexture<<" posX="<<posX<<" posY="<<posY<<endl;
    }
    
    if (!glyphTexture)
    {
        
        glyphTexture = new crGlyphTexture;
        
        static int numberOfTexturesAllocated = 0;
        ++numberOfTexturesAllocated;

       // CRCore::notify(CRCore::INFO)<< "   crFont " << this<< ", numberOfTexturesAllocated "<<numberOfTexturesAllocated<<std::endl;

        // reserve enough space for the glyphs.
        glyphTexture->setGlyphImageMargin(m_margin);
        glyphTexture->setGlyphImageMarginRatio(m_marginRatio);
        glyphTexture->setTextureSize(m_textureWidthHint,m_textureHeightHint);
        glyphTexture->setFilter(CRCore::crTexture::MIN_FILTER,m_minFilterHint);
        glyphTexture->setFilter(CRCore::crTexture::MAG_FILTER,m_magFilterHint);
        glyphTexture->setMaxAnisotropy(16);
        
        m_glyphTextureList.push_back(glyphTexture);
        
        if (!glyphTexture->getSpaceForGlyph(glyph,posX,posY))
        {
            CRCore::notify(CRCore::WARN)<<"Warning: unable to allocate texture big enough for glyph"<<std::endl;
            return;
        }

    }    
    
    // add the glyph into the texture.
    glyphTexture->addGlyph(glyph,posX,posY);
    
}
