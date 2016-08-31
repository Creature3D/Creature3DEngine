#ifndef CRE_COORDINATESYSTEMNODE
#define CRE_COORDINATESYSTEMNODE 1

#include <CRCore/crCoordinateSystemNode.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{
class crCoordinateSystemNode : public CRCore::crCoordinateSystemNode, public crReadWrite {
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
}

#endif
