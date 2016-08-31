#ifndef CRE_CRTRAVERSESTRINGNODE
#define CRE_CRTRAVERSESTRINGNODE 1

#include <CRCore/crTraverseStringNode.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{
class crTraverseStringNode : public CRCore::crTraverseStringNode, public crReadWrite{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
}

#endif
