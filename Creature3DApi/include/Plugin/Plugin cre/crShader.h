#ifndef CRE_SHADER
#define CRE_SHADER 1

#include <CRCore/crShader.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{
class crShader : public CRCore::crShader, public crReadWrite {
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
}

#endif
