#ifndef CRE_MATTERGROUP
#define CRE_MATTERGROUP 1

#include <CRPhysics/crMatterGroup.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{
class crMatterGroup : public CRPhysics::crMatterGroup, public crReadWrite{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
}

#endif
