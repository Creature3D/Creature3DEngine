// UnknownRecord.cpp

#include <Plugin flt/flt.h>
#include <Plugin flt/Registry.h>
#include <Plugin flt/UnknownRecord.h>

using namespace flt;

////////////////////////////////////////////////////////////////////
//
//                         UnknownRecord
//
////////////////////////////////////////////////////////////////////

RegisterRecordProxy<UnknownRecord> g_UnknownProxy;

UnknownRecord::UnknownRecord()
{
}


// virtual
UnknownRecord::~UnknownRecord()
{
}
