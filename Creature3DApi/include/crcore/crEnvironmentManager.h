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
#ifndef CRCORE_CRENVIRONMENTMANAGER
#define CRCORE_CRENVIRONMENTMANAGER 1

#include <CRCore/crBase.h>
#include <CRCore/crTextureCubeMap.h>
#include <CRCore/crVector3.h>
#include <map>

namespace CRCore {

class CR_EXPORT crEnvironmentManager : public CRCore::Referenced
{
public :
	crEnvironmentManager();
	static crEnvironmentManager *getInstance();
    
	typedef std::map< crVector3f, CRCore::ref_ptr<crTextureCubeMap> > EnvironmentMap;
	void insertEnvironmentMap( const crVector3f &local, crTextureCubeMap *env );
	crTextureCubeMap *getSuitableEnvMap(const crVector3f &local);
	EnvironmentMap &getEnvMap();
	void releaseObjects(crState* state);
	//need renderinit
	virtual void clear();
protected :
    virtual ~crEnvironmentManager() {}
	static ref_ptr<crEnvironmentManager> m_instance;
	EnvironmentMap m_envMap;
};

}

#endif
