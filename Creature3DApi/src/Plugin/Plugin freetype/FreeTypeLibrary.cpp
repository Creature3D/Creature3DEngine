#include <Plugin Freetype/FreeTypeLibrary.h>
#include <CRCore/crNotify.h>
//#include <ft2build.h>

//#define PRINT_OUT_FONT_DETAILS
#ifdef PRINT_OUT_FONT_DETAILS
    #include <freetype/ftsnames.h>
#endif

#include FT_TRUETYPE_IDS_H

FreeTypeLibrary::FreeTypeLibrary()
{
    //CRCore::notify(CRCore::INFO) << "FreeTypeLibrary::FreeTypeLibrary()" << std::endl; 
    FT_Error error = FT_Init_FreeType( &m_ftlibrary );
    if (error)
    {
        CRCore::notify(CRCore::WARN)<<"Warning: an error occurred during FT_Init_FreeType(..) initialisation, error code = "<<std::hex<<error<<std::dec<<std::endl;
    }

}

FreeTypeLibrary::~FreeTypeLibrary()
{
    // need to remove the implementations from the Fonts here
    // just in case the Fonts continue to have external references
    // to them, otherwise they will try to point to an object thats
    // definition has been unloaded along with the unload of the FreeType
    // plugin.
    while(!m_fontImplementationSet.empty())
    {
        FreeTypeFont* fontImplementation = *m_fontImplementationSet.begin();
        m_fontImplementationSet.erase(m_fontImplementationSet.begin());
        CRText::crFont* font = fontImplementation->m_facade;
        if (font) font->setImplementation(0);
        else fontImplementation->m_facade = 0;
    }
    
    FT_Done_FreeType( m_ftlibrary);
}

FreeTypeLibrary* FreeTypeLibrary::instance()
{
    static CRCore::ref_ptr<FreeTypeLibrary> s_library = new FreeTypeLibrary;
    return s_library.get();
}

bool FreeTypeLibrary::getFace(const std::string& fontfile,unsigned int index, FT_Face & face)
{
    CRCore::ScopedLock<CRCore::crMutex> lock(getMutex());

    FT_Error error = FT_New_Face( m_ftlibrary, fontfile.c_str(), index, &face );
    if (error == FT_Err_Unknown_File_Format)
    {
        CRCore::notify(CRCore::WARN)<<" .... the font file could be opened and read, but it appears"<<std::endl;
        CRCore::notify(CRCore::WARN)<<" .... that its font format is unsupported"<<std::endl;
        return false;
    }
    else if (error)
    {
        CRCore::notify(CRCore::WARN)<<" .... another error code means that the font file could not"<<std::endl;
        CRCore::notify(CRCore::WARN)<<" .... be opened, read or simply that it is broken.."<<std::endl;
        return false;
    }
    
#ifdef PRINT_OUT_FONT_DETAILS

    CRCore::notify(CRCore::NOTICE)<<"Face"<<face<<std::endl;
    unsigned int count = FT_Get_Sfnt_Name_Count(face);
    for(unsigned int i=0; i<count; ++i)
    {
        FT_SfntName names;
        FT_Error error = FT_Get_Sfnt_Name(face, i, &names);
        
        std::string name((char*)names.string, (char*)names.string + names.string_len);
        
        CRCore::notify(CRCore::NOTICE)<<"names "<<name<<std::endl;
    }

    CRCore::notify(CRCore::NOTICE)<<std::endl;
#endif

    //
    // GT: Fix to handle symbol fonts in MS Windows
    //
    verifyCharacterMap(face);
    
    return true;
}

FT_Byte* FreeTypeLibrary::getFace(std::istream& fontstream, unsigned int index, FT_Face & face)
{
    CRCore::ScopedLock<CRCore::crMutex> lock(getMutex());

    FT_Open_Args args;

    std::streampos start = fontstream.tellg();
    fontstream.seekg(0, std::ios::end);
    std::streampos end = fontstream.tellg();
    fontstream.seekg(start, std::ios::beg);
    std::streampos length = end - start;

    /* empty stream into memory, open that, and keep the pointer in a FreeTypeFont for cleanup */
    FT_Byte *buffer = new FT_Byte[length];
    fontstream.read(reinterpret_cast<char*>(buffer), length);
    if (!fontstream || (static_cast<std::streampos>(fontstream.gcount()) != length))
    {
        CRCore::notify(CRCore::WARN)<<" .... the font file could not be read from its stream"<<std::endl;
        if (buffer) delete [] buffer;
        return 0;
    }
    args.flags = FT_OPEN_MEMORY;
    args.memory_base = buffer;
    args.memory_size = length;

    FT_Error error = FT_Open_Face( m_ftlibrary, &args, index, &face );

    if (error == FT_Err_Unknown_File_Format)
    {
        CRCore::notify(CRCore::WARN)<<" .... the font file could be opened and read, but it appears"<<std::endl;
        CRCore::notify(CRCore::WARN)<<" .... that its font format is unsupported"<<std::endl;
        return 0;
    }
    else if (error)
    {
        CRCore::notify(CRCore::WARN)<<" .... another error code means that the font file could not"<<std::endl;
        CRCore::notify(CRCore::WARN)<<" .... be opened, read or simply that it is broken..."<<std::endl;
        return 0;
    }

    //
    // GT: Fix to handle symbol fonts in MS Windows
    //
    verifyCharacterMap(face);
    
    return buffer;
}


CRText::crFont* FreeTypeLibrary::getFont(const std::string& fontfile, unsigned int index, unsigned int flags)
{
    FT_Face face;
    if (getFace(fontfile, index, face) == false) return (0);

    CRCore::ScopedLock<CRCore::crMutex> lock(getMutex());
    
    FreeTypeFont* fontImp = new FreeTypeFont(fontfile,face,flags);
    CRText::crFont* font = new CRText::crFont(fontImp);

    m_fontImplementationSet.insert(fontImp);
    
    return font;
}
CRText::crFont* FreeTypeLibrary::getFont(std::istream& fontstream, unsigned int index, unsigned int flags)
{
    FT_Face face = 0;
    FT_Byte * buffer = getFace(fontstream, index, face);
    if (face == 0) return (0);

    
    CRCore::ScopedLock<CRCore::crMutex> lock(getMutex());

    FreeTypeFont* fontImp = new FreeTypeFont(buffer,face,flags);
    CRText::crFont* font = new CRText::crFont(fontImp);
    
    m_fontImplementationSet.insert(fontImp);

    return font;
}

void FreeTypeLibrary::verifyCharacterMap(FT_Face face)
{
    //
    // GT: Verify the correct character mapping for MS windows 
    // as symbol fonts were being returned incorrectly
    //
    FT_CharMap charmap;
    if (face->charmap == NULL)
    {
        for (int n = 0; n < face->num_charmaps; n++) 
        { 
            charmap = face->charmaps[n]; 
            if (charmap->platform_id == TT_PLATFORM_MICROSOFT) 
            { 
                FT_Set_Charmap(face, charmap);
                break; 
            } 
        }
    }
}
