#ifndef CRE_BRAIN
#define CRE_BRAIN 1

#include <CRCore/crBrain.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{

//class crAITreeHead : public CRCore::crAITreeHead, public crReadWrite{
//public:
//	void write(crDataOutputStream* out);
//	void read(crDataInputStream* in);
//};

class crBrain : public CRCore::crBrain, public crReadWrite{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};

}

#endif
