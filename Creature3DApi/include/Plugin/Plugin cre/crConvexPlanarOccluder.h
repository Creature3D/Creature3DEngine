#ifndef CRE_CONVEXPLANAROCCLUDER
#define CRE_CONVEXPLANAROCCLUDER 1

#include <CRCore/crConvexPlanarOccluder.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{
class crConvexPlanarOccluder : public CRCore::crConvexPlanarOccluder, public crReadWrite {
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
}

#endif
