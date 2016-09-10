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
#ifndef CRCORE_CRMATERIALSTATESETMANAGER
#define CRCORE_CRMATERIALSTATESETMANAGER 1

#include <CRCore/crBase.h>
#include <CRCore/crStateSet.h>
#include <map>

namespace CRCore {

class CR_EXPORT crMaterialStateSetManager : public CRCore::Referenced
{
public :
	crMaterialStateSetManager();
	static crMaterialStateSetManager *getInstance();
    
	typedef std::map< std::string, CRCore::ref_ptr<CRCore::crStateSet> > MaterialStateSetMap;
	void insertMaterial( crStateSet *stateset );
	crStateSet *getMaterial( const std::string& name );
	virtual void clear();
protected :
    virtual ~crMaterialStateSetManager() {}
	static ref_ptr<crMaterialStateSetManager> m_instance;

	MaterialStateSetMap m_materialStateSetMap;
};

}

#endif
