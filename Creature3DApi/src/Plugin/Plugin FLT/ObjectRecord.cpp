// ObjectRecord.cpp

#include <Plugin flt/flt.h>
#include <Plugin flt/Registry.h>
#include <Plugin flt/ObjectRecord.h>

using namespace flt;

////////////////////////////////////////////////////////////////////
//
//                          ObjectRecord
//
////////////////////////////////////////////////////////////////////

RegisterRecordProxy<ObjectRecord> g_ObjectProxy;

ObjectRecord::ObjectRecord()
{
}


// virtual
ObjectRecord::~ObjectRecord()
{
}


void ObjectRecord::endian()
{
    SObject *pSObject = (SObject*)getData();

    ENDIAN( pSObject->dwFlags );
    ENDIAN( pSObject->iObjectRelPriority );
    ENDIAN( pSObject->wTransparency );
    ENDIAN( pSObject->iSpecialId_1 );
    ENDIAN( pSObject->iSpecialId_2 );
    ENDIAN( pSObject->iSignificance );
}
