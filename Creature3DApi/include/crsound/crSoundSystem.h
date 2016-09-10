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
#ifndef CRSOUND_CRSOUNDSYSTEM_H
#define CRSOUND_CRSOUNDSYSTEM_H

#include <CRCore/ref_ptr.h>
#include <CRCore/Referenced.h>
#include <CRSound/crExport.h>
#include <CRSound/crSound.h>
#include <CRCore/thread/crCriticalMutex.h>
#include <CRCore/crMatrixd.h>
#include <fmod.hpp>
#include <fmod_errors.h>
#include <string>
#include <map>
namespace CRSound
{
#define FMERRCHECK(result) \
		if (result != FMOD_OK)\
		{\
			printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));\
		}
class CRSOUND_EXPORT crSoundSystem : public CRCore::Referenced
{
public:
	static crSoundSystem* getInstance();
	virtual void clear();
  
	void init();
	void shutdown();
	crSound *createSound(const std::string& fileName);
	void addSound(crSound *sound);
	void insertAmbSound(crSound *sound);
	crSound* findSound(const std::string &name);
	FMOD::System *getSystem(){ return m_system; }
	void update(const CRCore::crMatrixd &viewMat,float dt);
protected:
	FMOD::System    *m_system;
	typedef std::map<std::string, CRCore::ref_ptr<crSound> > SoundMap;
	SoundMap m_soundMap;
	CRCore::crCriticalMutex m_soundMutex;
	typedef std::set<CRCore::ref_ptr<crSound> > AmbSoundSet;
	AmbSoundSet m_ambSoundSet;
	crSoundSystem();
	~crSoundSystem();
	static CRCore::ref_ptr<crSoundSystem> m_instance;
	CRCore::crVector3 m_lastpos;
	float m_ambsoundVolume;
};

}
#endif

