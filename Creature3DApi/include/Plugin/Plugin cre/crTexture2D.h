#ifndef CRE_TEXTURE2D
#define CRE_TEXTURE2D 1

#include <CRCore/crTexture2D.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{
class crTexture2D : public CRCore::crTexture2D, public crReadWrite {
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
}

#endif
