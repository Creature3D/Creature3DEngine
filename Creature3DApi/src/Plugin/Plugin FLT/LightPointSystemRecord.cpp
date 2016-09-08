#include <Plugin flt\flt.h>
#include <Plugin flt\Registry.h>
#include <Plugin flt\LightPointSystemRecord.h>

using namespace flt;

////////////////////////////////////////////////////////////////////
//
//                     LightPointSystemRecord
//
////////////////////////////////////////////////////////////////////

RegisterRecordProxy<LightPointSystemRecord> g_LightPointSystemProxy;

LightPointSystemRecord::LightPointSystemRecord()
{
}


// virtual
LightPointSystemRecord::~LightPointSystemRecord()
{
}


// virtual
void LightPointSystemRecord::endian()
{
    SLightPointSystem* ltPtSys = (SLightPointSystem*)getData();

    ENDIAN( ltPtSys->intensity );
    ENDIAN( ltPtSys->animationState );
    ENDIAN( ltPtSys->flags );
}

