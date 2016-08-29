/**********************************************************************
*
*	文件:	   crEnvironmentManager.h
*
*	描述:	   
*
*	作者:	   吴财华
*					
*
*	Copyright 2005 吴财华
**********************************************************************/
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
