/* Creature3D - Online Game Engine, Copyright (C) 2005 Wucaihua(26756325@qq.com)
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
#ifndef CRNETAPP_NETLOADER_H
#define CRNETAPP_NETLOADER_H 1

#include <CRNetApp/appExport.h>

namespace CRNetApp{
	extern CRNETAPP_EXPORT void registerDataClass();
	extern CRNETAPP_EXPORT void registerHandleClass();
	extern CRNETAPP_EXPORT void registerNetPacket();
	extern CRNETAPP_EXPORT void registerNetCallbacks();
	extern CRNETAPP_EXPORT void registerServerDataClass();
	extern CRNETAPP_EXPORT void registerServerHandleClass();

	extern CRNETAPP_EXPORT void registerVRHandleClass();

	extern CRNETAPP_EXPORT void registerEventCallback();
}

#endif