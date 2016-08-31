#ifndef CRE_TEXTURE3D
#define CRE_TEXTURE3D 1

#include <CRCore/crTexture3D.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{
class crTexture3D : public CRCore::crTexture3D, public crReadWrite {
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
}

#endif
