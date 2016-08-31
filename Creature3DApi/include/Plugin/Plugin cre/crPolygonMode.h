#ifndef CRE_POLYGONMODE
#define CRE_POLYGONMODE 1

#include <CRCore/crPolygonMode.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{
class crPolygonMode : public CRCore::crPolygonMode, public crReadWrite {
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
}

#endif
