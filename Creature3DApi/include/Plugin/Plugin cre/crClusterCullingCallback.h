#ifndef CRE_CLUSTERCULLINGCALLBACK
#define CRE_CLUSTERCULLINGCALLBACK 1

#include <CRCore/crClusterCullingCallback.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{
class crClusterCullingCallback : public CRCore::crClusterCullingCallback, public crReadWrite {
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
}

#endif
