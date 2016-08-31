#ifndef CRE_SEQUENCE
#define CRE_SEQUENCE 1

#include <CRCore/crSequence.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{
class crSequence : public CRCore::crSequence, public crReadWrite {
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
class crMatrixSequence : public CRCore::crMatrixSequence, public crReadWrite {
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
}

#endif
