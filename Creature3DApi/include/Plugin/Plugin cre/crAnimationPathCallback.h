#ifndef CRE_ANIMATIONPATHCALLBACK
#define CRE_ANIMATIONPATHCALLBACK 1

#include <CRCore/crAnimationPath.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{
class crAnimationPathCallback : public CRCore::crAnimationPathCallback, public crReadWrite {
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
}

#endif
