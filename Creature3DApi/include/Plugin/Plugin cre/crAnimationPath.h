#ifndef CRE_ANIMATIONPATH
#define CRE_ANIMATIONPATH 1

#include <CRCore/crAnimationPath.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{
class crAnimationPath : public CRCore::crAnimationPath, public crReadWrite {
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
}

#endif
