#ifndef CRE_LIGHTMODEL
#define CRE_LIGHTMODEL 1

#include <CRCore/crLightModel.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{
  class crLightModel : public CRCore::crLightModel, public crReadWrite {
  public:
    void write(crDataOutputStream* out);
    void read(crDataInputStream* in);
  };
}

#endif
