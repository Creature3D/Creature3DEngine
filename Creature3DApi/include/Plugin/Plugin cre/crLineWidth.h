#ifndef CRE_LINEWIDTH
#define CRE_LINEWIDTH 1

#include <CRCore/crLineWidth.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{
class crLineWidth : public CRCore::crLineWidth, public crReadWrite {
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
}

#endif
