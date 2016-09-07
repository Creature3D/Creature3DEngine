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
