/**********************************************************************
*
*	文件:	   crMaterialStateSetManager.h
*
*	描述:	   
*
*	作者:	   吴财华
*					
*
*	Copyright 2005 吴财华
**********************************************************************/
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
