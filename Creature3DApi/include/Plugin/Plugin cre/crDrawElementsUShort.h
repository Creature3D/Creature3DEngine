#ifndef CRE_DRAWELEMENTSUSHORT
#define CRE_DRAWELEMENTSUSHORT 1

#include <CRCore/crPrimitive.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{
class crDrawElementsUShort : public CRCore::DrawElementsUShort, public crReadWrite {
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
}

#endif
