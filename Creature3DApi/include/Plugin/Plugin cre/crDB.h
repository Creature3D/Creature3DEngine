#ifndef CRE_DB
#define CRE_DB 1

#include <CRCore/crDB.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{
class crDB : public CRCore::crDB, public crReadWrite{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
}

#endif
