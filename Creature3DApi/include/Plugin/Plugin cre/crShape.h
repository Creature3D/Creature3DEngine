#ifndef CRE_HIEGHTFIELD
#define CRE_HIEGHTFIELD 1

#include <CRCore/crShape.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{

class crSphere : public CRCore::crSphere, public crReadWrite {
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};

class crBox : public CRCore::crBox, public crReadWrite {
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};

class crCone : public CRCore::crCone, public crReadWrite {
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};

class crCylinder : public CRCore::crCylinder, public crReadWrite {
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};

class crCapsule : public CRCore::crCapsule, public crReadWrite {
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};

class crHeightField : public CRCore::crHeightField, public crReadWrite {
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};

}

#endif
