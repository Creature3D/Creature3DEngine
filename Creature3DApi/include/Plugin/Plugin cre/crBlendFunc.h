#ifndef CRE_BLENDFUNC
#define CRE_BLENDFUNC 1

#include <CRCore/crBlendFunc.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{
class crBlendFunc : public CRCore::crBlendFunc, public crReadWrite {
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
}

#endif
