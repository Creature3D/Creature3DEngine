// SwitchRecord.cpp

#include <Plugin flt/flt.h>
#include <Plugin flt/Registry.h>
#include <Plugin flt/SwitchRecord.h>

using namespace flt;

////////////////////////////////////////////////////////////////////
//
//                          SwitchRecord
//
////////////////////////////////////////////////////////////////////

RegisterRecordProxy<SwitchRecord> g_SwitchProxy;

SwitchRecord::SwitchRecord()
{
}


// virtual
SwitchRecord::~SwitchRecord()
{
}


void SwitchRecord::endian()
{
    SSwitch *pSSwitch = (SSwitch*)getData();

    ENDIAN( pSSwitch->nCurrentMask );
    ENDIAN( pSSwitch->nMasks );
    ENDIAN( pSSwitch->nWordsInMask );

    for (int m=0; m < pSSwitch->nMasks*pSSwitch->nWordsInMask; m++)
    {
        ENDIAN( pSSwitch->aMask[m] );
    }
}
