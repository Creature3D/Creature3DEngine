#ifndef CRE_STATESET
#define CRE_STATESET 1

#include <CRCore/crStateSet.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{
class crStateSet : public crReadWrite, public CRCore::crStateSet{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
}

#endif
