#ifndef CRE_DRAWARRAYS
#define CRE_DRAWARRAYS 1

#include <CRCore/crPrimitive.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{
class crDrawArrays : public crReadWrite, public CRCore::DrawArrays{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
}

#endif
