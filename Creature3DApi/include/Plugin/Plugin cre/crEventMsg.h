#ifndef CRE_EVENTMSG
#define CRE_EVENTMSG 1

#include <CRCore/crEventMsg.h>
#include <Plugin cre/crReadWrite.h>

namespace cre{

class Condition : public CRCore::Condition, public crReadWrite{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};

class SendMsg : public CRCore::SendMsg, public crReadWrite{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};

class ECondition : public CRCore::ECondition, public crReadWrite{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
}; 

class crEvent : public CRCore::crEvent, public crReadWrite{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};

class crEventMsg : public CRCore::crEventMsg, public crReadWrite{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};

class crIF_ELSE_EventMsg : public CRCore::crIF_ELSE_EventMsg, public crReadWrite{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};

class crSwitch_EventMsg : public CRCore::crSwitch_EventMsg, public crReadWrite{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};

class crWhile_EventMsg : public CRCore::crWhile_EventMsg, public crReadWrite{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};

}

#endif
