/* Creature3D - Online Game Engine, Copyright (C) 2005 Îâ²Æ»ª(26756325@qq.com)
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
#ifndef CRNETAPP_LOGINSTARTLOADER_H
#define CRNETAPP_LOGINSTARTLOADER_H 1

#include <CRNetApp/appExport.h>
#include <CREncapsulation/crStartHandler.h>
namespace CRNetApp{

class CRNETAPP_EXPORT crLoginStartLoader : public CREncapsulation::crStartLoader
{
public:
	virtual void loading(CRProducer::crViewer *viewer,Producer::Window parentWnd,Producer::Window renderWnd);
};

class CRNETAPP_EXPORT crLoginStartLoader2 : public CREncapsulation::crStartLoader
{
public:
	virtual void loading(CRProducer::crViewer *viewer,Producer::Window parentWnd,Producer::Window renderWnd);
};

}
#endif