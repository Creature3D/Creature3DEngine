#ifndef CRE_BILLBOARD
#define CRE_BILLBOARD 1

#include <CRCore/crBillboard.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{
class crBillboard : public CRCore::crBillboard, public crReadWrite {
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
}

#endif
