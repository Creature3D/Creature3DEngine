#ifndef CRE_DOFTRANSFORM
#define CRE_DOFTRANSFORM 1

#include <CRCore/crDOFTransform.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{
class crDOFTransform : public CRCore::crDOFTransform, public crReadWrite {
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
}

#endif
