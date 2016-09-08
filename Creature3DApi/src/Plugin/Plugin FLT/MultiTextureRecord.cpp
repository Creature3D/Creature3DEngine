// MultiTextureRecord.cpp

#include <Plugin flt\flt.h>
#include <Plugin flt\Registry.h>
#include <Plugin flt\MultiTextureRecord.h>

using namespace flt;

////////////////////////////////////////////////////////////////////
//
//                       MultiTextureRecord
//
////////////////////////////////////////////////////////////////////

RegisterRecordProxy<MultiTextureRecord> g_MultiTextureProxy;

MultiTextureRecord::MultiTextureRecord()
{
    //CERR << "MultiTextureRecord created\n";
}


// virtual
MultiTextureRecord::~MultiTextureRecord()
{
}


// virtual
void MultiTextureRecord::endian()
{
    SMultiTexture *pSMultiTexture = (SMultiTexture*)getData();

    ENDIAN( pSMultiTexture->layers );
}
