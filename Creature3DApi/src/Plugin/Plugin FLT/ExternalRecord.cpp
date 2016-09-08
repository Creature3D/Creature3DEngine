// ExternalRecord.cpp

#include <Plugin flt\flt.h>
#include <Plugin flt\Registry.h>
#include <Plugin flt\ExternalRecord.h>

using namespace flt;

////////////////////////////////////////////////////////////////////
//
//                          ExternalRecord
//
////////////////////////////////////////////////////////////////////

RegisterRecordProxy<ExternalRecord> g_ExternalProxy;

ExternalRecord::ExternalRecord()
{
}


// virtual
ExternalRecord::~ExternalRecord()
{
}


void ExternalRecord::setExternal(FltFile* pExternal)
{
    m_fltfile = pExternal;
}


void ExternalRecord::endian()
{
    SExternalReference *pSExternal = (SExternalReference*)getData();

    if (getFlightVersion() > 13)
    {
        ENDIAN( pSExternal->dwFlags );
    }
}
