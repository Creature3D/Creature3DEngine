#ifndef CRE_DRAWABLE
#define CRE_DRAWABLE 1

#include <CRCore/crDrawable.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{
class crDrawable : public crReadWrite, public CRCore::crDrawable{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
}

#endif
