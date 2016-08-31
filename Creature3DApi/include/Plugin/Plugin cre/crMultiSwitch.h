#ifndef CRE_MULTISWITCH
#define CRE_MULTISWITCH 1

#include <CRCore/crMultiSwitch.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{
class crMultiSwitch : public CRCore::crMultiSwitch, public crReadWrite {
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
}

#endif
