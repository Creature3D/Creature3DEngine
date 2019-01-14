// BSPRecord.cpp
//
// Author: Michael M. Morrison
//

#include <Plugin flt/flt.h>
#include <Plugin flt/Registry.h>
#include <Plugin flt/BSPRecord.h>

using namespace flt;

////////////////////////////////////////////////////////////////////
//
//                          BSPRecord
//
////////////////////////////////////////////////////////////////////

RegisterRecordProxy<BSPRecord> g_BSPProxy;

BSPRecord::BSPRecord()
{
}


// virtual
BSPRecord::~BSPRecord()
{
}


void BSPRecord::endian()
{
    SBSP *pSBSP = (SBSP*)getData();

    ENDIAN( pSBSP->planeA );
    ENDIAN( pSBSP->planeB );
    ENDIAN( pSBSP->planeC );
    ENDIAN( pSBSP->planeD );
}
