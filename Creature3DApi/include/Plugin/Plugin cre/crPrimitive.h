#ifndef CRE_PRIMITCRESET
#define CRE_PRIMITCRESET 1

#include <CRCore/crPrimitive.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{
class crPrimitive : public crReadWrite, public CRCore::crPrimitive{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
}

#endif
