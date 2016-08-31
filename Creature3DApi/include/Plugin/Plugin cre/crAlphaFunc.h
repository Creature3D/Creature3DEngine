#ifndef CRE_ALPHAFUNC
#define CRE_ALPHAFUNC 1

#include <CRCore/crAlphaFunc.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{
  class crAlphaFunc : public CRCore::crAlphaFunc, public crReadWrite {
  public:
    void write(crDataOutputStream* out);
    void read(crDataInputStream* in);
  };
}

#endif
