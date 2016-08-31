#ifndef CRE_LOD
#define CRE_LOD 1

#include <CRCore/crLod.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{
class crLod : public CRCore::crLod, public crReadWrite {
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
}

#endif
