#ifndef CRE_MATERIAL
#define CRE_MATERIAL 1

#include <CRCore/crMaterial.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{
class crMaterial : public CRCore::crMaterial, public crReadWrite {
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
}

#endif
