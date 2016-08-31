#ifndef CRE_CAMERAVIEW
#define CRE_CAMERAVIEW 1

#include <CRCore/crCameraView.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{
class crCameraView : public CRCore::crCameraView, public crReadWrite {
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
}

#endif
