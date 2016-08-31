#ifndef CRE_SHADEMODEL
#define CRE_SHADEMODEL 1

#include <CRCore/crShadeModel.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{
class crShadeModel : public CRCore::crShadeModel, public crReadWrite {
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
}

#endif
