#ifndef CRE_MATRIXTRANSFORM
#define CRE_MATRIXTRANSFORM 1

#include <CRCore/crMatrixTransform.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{
class crMatrixTransform : public CRCore::crMatrixTransform, public crReadWrite {
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
}

#endif
