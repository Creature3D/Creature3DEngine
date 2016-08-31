#ifndef CRE_NODE
#define CRE_NODE 1

#include <CRCore/crNode.h>
#include <Plugin cre/crReadWrite.h>
#include <iostream>

namespace cre{
class crNode :  public CRCore::crNode, public crReadWrite{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
}

#endif
