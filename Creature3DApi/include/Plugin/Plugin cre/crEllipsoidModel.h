#ifndef CRE_ELLIPSOIDMODEL
#define CRE_ELLIPSOIDMODEL 1

#include <CRCore/crCoordinateSystemNode.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{
class crEllipsoidModel : public CRCore::crEllipsoidModel, public crReadWrite {
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
}

#endif
