#ifndef CRE_EVENTGROUP
#define CRE_EVENTGROUP 1

#include <CRCore/crEventGroup.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{
class crEventGroup : public CRCore::crEventGroup, public crReadWrite{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
}

#endif
