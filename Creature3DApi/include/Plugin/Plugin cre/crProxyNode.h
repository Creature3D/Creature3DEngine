#ifndef CRE_PROXYNODE
#define CRE_PROXYNODE 1

#include <CRCore/crProxyNode.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{
class crProxyNode : public CRCore::crProxyNode, public crReadWrite {
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
class crExternNode : public CRCore::crExternNode, public crReadWrite {
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
}

#endif
