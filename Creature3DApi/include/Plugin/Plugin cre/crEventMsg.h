/* Creature3D - Online Game Engine, Copyright (C) 2005 吴财华(26756325@qq.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
*/
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
