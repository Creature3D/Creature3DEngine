// GroupRecord.cpp

#include <Plugin flt\flt.h>
#include <Plugin flt\Registry.h>
#include <Plugin flt\GroupRecord.h>

using namespace flt;

////////////////////////////////////////////////////////////////////
//
//                          GroupRecord
//
////////////////////////////////////////////////////////////////////

RegisterRecordProxy<GroupRecord> g_GroupProxy;

GroupRecord::GroupRecord()
{
}


// virtual
GroupRecord::~GroupRecord()
{
}


void GroupRecord::endian()
{
    SGroup *pSGroup = (SGroup*)getData();

    ENDIAN( pSGroup->iGroupRelPriority );
    ENDIAN( pSGroup->dwFlags );
    ENDIAN( pSGroup->iSpecialId_1 );
    ENDIAN( pSGroup->iSpecialId_2 );
    ENDIAN( pSGroup->iSignificance );

	if (getFltFile()->getFlightVersion() >= 1580 )
	{
		ENDIAN( pSGroup->iLoopCount );
		ENDIAN( pSGroup->fLoopDuration );
		ENDIAN( pSGroup->fLastFrameDuration );
	}
}
