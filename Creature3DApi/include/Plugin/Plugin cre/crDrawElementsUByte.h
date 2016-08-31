#ifndef CRE_DRAWELEMENTSUBYTE
#define CRE_DRAWELEMENTSUBYTE 1

#include <CRCore/crPrimitive.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{
class crDrawElementsUByte : public CRCore::DrawElementsUByte, public crReadWrite {
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
}

#endif
