#ifndef CRE_DEPTH
#define CRE_DEPTH 1

#include <CRCore/crDepth.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{
class crDepth : public CRCore::crDepth, public crReadWrite {
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
}

#endif
