#ifndef CRE_CRCOLLIDECALLBACK
#define CRE_CRCOLLIDECALLBACK 1

#include <CRCore/crCollideCallback.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{

class crCollideCallback : public CRCore::crCollideCallback, public crReadWrite
{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};

}

#endif
