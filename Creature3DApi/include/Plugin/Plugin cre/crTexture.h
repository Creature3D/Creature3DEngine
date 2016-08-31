#ifndef CRE_TEXTURE
#define CRE_TEXTURE 1

#include <CRCore/crTexture.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{
class crTexture : public CRCore::crTexture, public crReadWrite {
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
}

#endif
