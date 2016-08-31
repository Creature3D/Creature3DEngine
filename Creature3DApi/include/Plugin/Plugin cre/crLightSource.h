#ifndef CRE_LIGHTSOURCE
#define CRE_LIGHTSOURCE 1

#include <CRCore/crLightSource.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{
class crLightSource : public CRCore::crLightSource, public crReadWrite {
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
}

#endif
