#ifndef CRE_POINT
#define CRE_POINT 1

#include <CRCore/crPoint.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{
class crPoint : public CRCore::crPoint, public crReadWrite {
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
}

#endif
