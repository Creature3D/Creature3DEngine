#ifndef CRE_DRAWARRAYLENGTHS
#define CRE_DRAWARRAYLENGTHS 1

#include <CRCore/crPrimitive.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{
class crDrawArrayLengths : public CRCore::DrawArrayLengths, public crReadWrite {
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
}

#endif
