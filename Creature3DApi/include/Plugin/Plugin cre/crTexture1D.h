#ifndef CRE_TEXTURE1D
#define CRE_TEXTURE1D 1

#include <CRCore/crTexture1D.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{
class crTexture1D : public CRCore::crTexture1D, public crReadWrite {
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
}

#endif
