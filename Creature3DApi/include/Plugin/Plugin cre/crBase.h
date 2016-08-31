#ifndef CRE_OBJECT
#define CRE_OBJECT 1


#include <CRCore/crBase.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{

class crBase : public crReadWrite, public CRCore::crBase{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* out);
};

}

#endif
