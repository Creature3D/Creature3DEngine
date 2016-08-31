#ifndef CRE_DRAWELEMENTSUINT
#define CRE_DRAWELEMENTSUINT 1

#include <CRCore/crPrimitive.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{
class crDrawElementsUInt : public CRCore::DrawElementsUInt, public crReadWrite {
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
}

#endif
