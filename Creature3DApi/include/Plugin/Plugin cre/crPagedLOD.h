#ifndef CRE_PAGEDLOD
#define CRE_PAGEDLOD 1

#include <CRCore/crPagedLOD.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{
class crPagedLOD : public CRCore::crPagedLOD, public crReadWrite {
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
}

#endif
