#ifndef CRE_TEXT
#define CRE_TEXT 1

#include <CRText/crText.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{
class crText : public CRText::crText, public crReadWrite {
public:
    void write(crDataOutputStream* out);
    void read(crDataInputStream* in);
};
}

#endif
