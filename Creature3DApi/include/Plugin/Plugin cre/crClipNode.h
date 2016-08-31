#ifndef CRE_ClipNode
#define CRE_ClipNode 1

#include <CRCore/crClipNode.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{
class crClipNode : public CRCore::crClipNode, public crReadWrite {
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
}

#endif
