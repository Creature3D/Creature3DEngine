#ifndef CRE_TEXENVCOMBINE
#define CRE_TEXENVCOMBINE 1

#include <CRCore/crTexEnvCombine.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{
class crTexEnvCombine : public CRCore::crTexEnvCombine, public crReadWrite {
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
}

#endif
