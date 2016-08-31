#ifndef CRE_JOINT
#define CRE_JOINT 1

#include <CRPhysics/crJoint.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{
class crJointParameter : public CRPhysics::crJointParameter, public crReadWrite{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};

class crJoint : public CRPhysics::crJoint, public crReadWrite{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};

class crJointBall : public CRPhysics::crJointBall, public crReadWrite{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};

class crJointHinge : public CRPhysics::crJointHinge, public crReadWrite{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};

class crJointSlider : public CRPhysics::crJointSlider, public crReadWrite{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};

class crJointUniversal : public CRPhysics::crJointUniversal, public crReadWrite{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};

class crJointContact : public CRPhysics::crJointContact, public crReadWrite{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};

class crJointHinge2 : public CRPhysics::crJointHinge2, public crReadWrite{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};

class crJointFixed : public CRPhysics::crJointFixed, public crReadWrite{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};

class crJointAMotor : public CRPhysics::crJointAMotor, public crReadWrite{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};

class crJointNull : public CRPhysics::crJointNull, public crReadWrite{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
}

#endif
