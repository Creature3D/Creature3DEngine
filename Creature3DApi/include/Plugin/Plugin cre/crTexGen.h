#ifndef CRE_TEXGEN
#define CRE_TEXGEN 1

#include <CRCore/crTexGen.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{
class crTexGen : public CRCore::crTexGen, public crReadWrite {
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
}

#endif
