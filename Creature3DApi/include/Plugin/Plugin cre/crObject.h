#ifndef CRE_GEODE
#define CRE_GEODE 1

#include <CRCore/crObject.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{
class crObject : public crReadWrite, public CRCore::crObject{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
}

#endif
