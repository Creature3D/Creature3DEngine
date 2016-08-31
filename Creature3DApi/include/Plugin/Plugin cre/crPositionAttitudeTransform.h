#ifndef CRE_POSITIONATTITUDETRANSFORM
#define CRE_POSITIONATTITUDETRANSFORM 1

#include <CRCore/crPositionAttitudeTransform.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{
class crPositionAttitudeTransform : public CRCore::crPositionAttitudeTransform, public crReadWrite {
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
}

#endif
