#ifndef CRE_TEXENV
#define CRE_TEXENV 1

#include <CRCore/crTexEnv.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{
class crTexEnv : public CRCore::crTexEnv, public crReadWrite {
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
}

#endif
