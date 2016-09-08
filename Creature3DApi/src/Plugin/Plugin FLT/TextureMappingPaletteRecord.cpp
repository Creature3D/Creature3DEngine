// TextureMappingPaletteRecord.cpp

#include <Plugin flt\flt.h>
#include <Plugin flt\Registry.h>
#include <Plugin flt\TextureMappingPaletteRecord.h>

using namespace flt;

////////////////////////////////////////////////////////////////////
//
//                       TextureMappingPaletteRecord
//
////////////////////////////////////////////////////////////////////

RegisterRecordProxy<TextureMappingPaletteRecord> g_TextureMappingPaletteProxy;

TextureMappingPaletteRecord::TextureMappingPaletteRecord()
{
}


// virtual
TextureMappingPaletteRecord::~TextureMappingPaletteRecord()
{
}


// virtual
void TextureMappingPaletteRecord::endian()
{
    STextureMapping *pSMapping = (STextureMapping*)getData();

    if (pSMapping)
    {
        ENDIAN( pSMapping->diIndex );
        ENDIAN( pSMapping->diType );
        ENDIAN( pSMapping->diWarpFlag );

        for(int i=0;i<4;++i)
        {
            for(int j=0;j<4;++j)
            {
                ENDIAN( pSMapping->dfMat[i][j] );
            }
        }
    }
}
