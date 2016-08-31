#ifndef CRE_PROGRAM
#define CRE_PROGRAM 1

#include <CRCore/crProgram.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{
class crProgram : public CRCore::crProgram, public crReadWrite {
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
}

#endif
