#ifndef CRE_FOG
#define CRE_FOG 1

#include <CRCore/crFog.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{
class crFog : public CRCore::crFog, public crReadWrite {
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
}

#endif
