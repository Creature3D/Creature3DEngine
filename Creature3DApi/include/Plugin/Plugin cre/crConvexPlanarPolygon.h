#ifndef CRE_CONVEXPLANARPOLYGON
#define CRE_CONVEXPLANARPOLYGON 1

#include <CRCore/crConvexPlanarPolygon.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{
class crConvexPlanarPolygon : public CRCore::crConvexPlanarPolygon, public crReadWrite {
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
        
        virtual ~crConvexPlanarPolygon() {}
};
}

#endif
