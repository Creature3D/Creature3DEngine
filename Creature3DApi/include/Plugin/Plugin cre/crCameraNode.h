#ifndef CRE_CAMERANODE
#define CRE_CAMERANODE 1

#include <CRCore/crCameraNode.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{
class crCameraNode : public CRCore::crCameraNode, public crReadWrite {
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
}

#endif
