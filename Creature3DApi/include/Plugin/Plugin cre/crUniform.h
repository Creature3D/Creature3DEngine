#ifndef CRE_UNIFORM
#define CRE_UNIFORM 1

#include <CRCore/crUniform.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{
class crUniform : public CRCore::crUniform, public crReadWrite {
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
}

#endif
