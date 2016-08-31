#ifndef CRE_MATTEROBJECT
#define CRE_MATTEROBJECT 1

#include <CRPhysics/crMatterObject.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{
class crMatterObject : public CRPhysics::crMatterObject, public crReadWrite {
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};

class crBoxMatterObject : public CRPhysics::crBoxMatterObject, public crReadWrite {
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};

class crSphereMatterObject : public CRPhysics::crSphereMatterObject, public crReadWrite {
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};

class crCCylinderMatterObject : public CRPhysics::crCCylinderMatterObject, public crReadWrite {
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};

class crMeshMatterObject : public CRPhysics::crMeshMatterObject, public crReadWrite {
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};

//class crViewMatterObject : public CRPhysics::crViewMatterObject, public crReadWrite {
//public:
//	void write(crDataOutputStream* out);
//	void read(crDataInputStream* in);
//};
//
//class crCarMatterObject : public CRPhysics::crCarMatterObject, public crReadWrite {
//public:
//	void write(crDataOutputStream* out);
//	void read(crDataInputStream* in);
//};

//class crRayMatterObject : public CRPhysics::crRayMatterObject, public crReadWrite {
//public:
//	void write(crDataOutputStream* out);
//	void read(crDataInputStream* in);
//};

}

#endif
