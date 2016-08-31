#ifndef CRE_OCCLUDERNODE
#define CRE_OCCLUDERNODE 1

#include <CRCore/crOccluderNode.h>

#include <Plugin cre/crReadWrite.h>

namespace cre{
class crOccluderNode : public CRCore::crOccluderNode, public crReadWrite {
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
}

#endif
