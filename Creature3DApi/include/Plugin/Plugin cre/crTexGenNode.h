#ifndef CRE_TEXGENNODE
#define CRE_TEXGENNODE 1

#include <CRCore/crTexGenNode.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{
class crTexGenNode : public CRCore::crTexGenNode, public crReadWrite {
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
}

#endif
