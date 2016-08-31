#ifndef CRE_TEXMAT
#define CRE_TEXMAT 1

#include <CRCore/crTexMat.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{
class crTexMat : public CRCore::crTexMat, public crReadWrite {
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
}

#endif
