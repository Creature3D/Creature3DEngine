#ifndef CRE_SWITCH
#define CRE_SWITCH 1

#include <CRCore/crSwitch.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{
class crSwitch : public CRCore::crSwitch, public crReadWrite {
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
}

#endif
