#ifndef CRE_CULLFACE
#define CRE_CULLFACE 1

#include <CRCore/crCullFace.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{
class crCullFace : public CRCore::crCullFace, public crReadWrite {
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
}

#endif
