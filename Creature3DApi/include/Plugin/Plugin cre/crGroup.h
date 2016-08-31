#ifndef CRE_GROUP
#define CRE_GROUP 1

#include <CRCore/crGroup.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{
class crGroup : public CRCore::crGroup, public crReadWrite{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
}

#endif
