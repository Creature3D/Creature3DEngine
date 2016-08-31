#ifndef CRE_IMAGE
#define CRE_IMAGE 1

#include <CRCore/crImage.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{
class crImage : public CRCore::crImage, public crReadWrite {
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
}

#endif
