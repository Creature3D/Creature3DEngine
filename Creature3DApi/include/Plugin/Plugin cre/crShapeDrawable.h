#ifndef CRE_SHAPEDRAWABLE
#define CRE_SHAPEDRAWABLE 1

#include <CRCore/crShapeDrawable.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{
class crShapeDrawable : public CRCore::crShapeDrawable, public crReadWrite {
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
}

#endif
