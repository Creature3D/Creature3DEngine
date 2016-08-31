#ifndef CRE_ClipPlane
#define CRE_ClipPlane 1

#include <CRCore/crClipPlane.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{
  class crClipPlane : public CRCore::crClipPlane, public crReadWrite {
  public:
    void write(crDataOutputStream* out);
    void read(crDataInputStream* in);
  };
}

#endif
