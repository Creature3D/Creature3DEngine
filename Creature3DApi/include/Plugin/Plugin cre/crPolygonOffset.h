#ifndef CRE_POLYGONOFFSET
#define CRE_POLYGONOFFSET 1

#include <CRCore/crPolygonOffset.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{
class crPolygonOffset : public CRCore::crPolygonOffset, public crReadWrite {
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
}

#endif
