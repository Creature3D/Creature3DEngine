#ifndef CRE_LIGHT
#define CRE_LIGHT 1

#include <CRCore/crLight.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{
class crLight : public CRCore::crLight, public crReadWrite {
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
}

#endif
