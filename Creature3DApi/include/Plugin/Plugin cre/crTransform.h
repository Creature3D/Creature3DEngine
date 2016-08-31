#ifndef CRE_TRANSFORM
#define CRE_TRANSFORM 1

#include <CRCore/crTransform.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{
class crTransform : public CRCore::crTransform, public crReadWrite {
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
}

#endif
