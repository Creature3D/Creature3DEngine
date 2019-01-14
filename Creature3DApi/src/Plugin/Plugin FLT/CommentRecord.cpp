// CommentRecord.cpp

#include <Plugin flt/flt.h>
#include <Plugin flt/Registry.h>
#include <Plugin flt/CommentRecord.h>

using namespace flt;

////////////////////////////////////////////////////////////////////
//
//                          CommentRecord
//
////////////////////////////////////////////////////////////////////

RegisterRecordProxy<CommentRecord> g_CommentRecordProxy;

CommentRecord::CommentRecord()
{
}


// virtual
CommentRecord::~CommentRecord()
{
}


// virtual
void CommentRecord::endian()
{
}
