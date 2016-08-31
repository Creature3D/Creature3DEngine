#ifndef CRE_GEOMETRY
#define CRE_GEOMETRY 1

#include <CRCore/crGeometry.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{
class crGeometry : public crReadWrite, public CRCore::crGeometry{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
}

#endif
