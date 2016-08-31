#ifndef CRE_FRONTFACE
#define CRE_FRONTFACE 1

#include <CRCore/crFrontFace.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{
class crFrontFace : public CRCore::crFrontFace, public crReadWrite {
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
}

#endif

