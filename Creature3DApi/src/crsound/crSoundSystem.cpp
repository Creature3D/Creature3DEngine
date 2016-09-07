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
#include <CRSound/crSoundSystem.h>
#include <CRCore/crArgumentParser.h>
#include <CRIOManager/crReadFile.h>
#include <CRCore/thread/crScopedLock.h>

using namespace CRSound;
using namespace CRCore;
using namespace CRIOManager;
ref_ptr<crSoundSystem> crSoundSystem::m_instance = NULL;
crSoundSystem::crSoundSystem():
	m_system(NULL),
	m_ambsoundVolume(1.0f)
{
}
crSoundSystem::~crSoundSystem()
{
}
void crSoundSystem::init()
{
	////////////////init fmod
	FMOD_RESULT      result;
	unsigned int     version;
	int              numdrivers;
	FMOD_SPEAKERMODE speakermode;
	FMOD_CAPS        caps;
	char             name[256];
	/*
        Create a System object and initialize.
    */
    result = FMOD::System_Create(&m_system);
    FMERRCHECK(result);
    
    result = m_system->getVersion(&version);
    FMERRCHECK(result);

    if (version < FMOD_VERSION)
    {
        printf("Error!  You are using an old version of FMOD %08x.  This program requires %08x\n", version, FMOD_VERSION);
        return;
    }
	result = m_system->getNumDrivers(&numdrivers);
	FMERRCHECK(result);

	if (numdrivers == 0)
	{
		result = m_system->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
		FMERRCHECK(result);
	}
	else
	{
		result = m_system->getDriverCaps(0, &caps, 0, 0, &speakermode);
		FMERRCHECK(result);

		result = m_system->setSpeakerMode(speakermode);       /* Set the user selected speaker mode. */
		FMERRCHECK(result);

		if (caps & FMOD_CAPS_HARDWARE_EMULATED)             /* The user has the 'Acceleration' slider set to off!  This is really bad for latency!. */
		{                                                   /* You might want to warn the user about this. */
			result = m_system->setDSPBufferSize(1024, 10);
			FMERRCHECK(result);
		}

		result = m_system->getDriverInfo(0, name, 256, 0);
		FMERRCHECK(result);

		if (strstr(name, "SigmaTel"))   /* Sigmatel sound devices crackle for some reason if the format is PCM 16bit.  PCM floating point output seems to solve it. */
		{
			result = m_system->setSoftwareFormat(48000, FMOD_SOUND_FORMAT_PCMFLOAT, 0,0, FMOD_DSP_RESAMPLER_LINEAR);
			FMERRCHECK(result);
		}
	}

	result = m_system->init(100, FMOD_INIT_NORMAL, 0);
	if (result == FMOD_ERR_OUTPUT_CREATEBUFFER)         /* Ok, the speaker mode selected isn't supported by this soundcard.  Switch it back to stereo... */
	{
		result = m_system->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);
		FMERRCHECK(result);

		result = m_system->init(100, FMOD_INIT_NORMAL, 0);/* ... and re-init. */
		FMERRCHECK(result);
	}
	/*
        Set the distance units. (meters/feet etc).
    */
	//const float DISTANCEFACTOR = 1.0f;          // Units per meter.  I.e feet would = 3.28.  centimeters would = 100.
    result = m_system->set3DSettings(1.0, 1.0f, 1.0f);
    FMERRCHECK(result);
	///////////////////end init fmod
}

void crSoundSystem::shutdown()
{
	////////////Fmod shutdow
	m_soundMap.clear();

	FMOD_RESULT result = m_system->close();
	FMERRCHECK(result);
	result = m_system->release();
	FMERRCHECK(result);
	m_system = NULL;
	/////////////////
}
crSoundSystem *crSoundSystem::getInstance()
{
	if (m_instance==NULL) 
		m_instance = new crSoundSystem;
	return m_instance.get();
}
void crSoundSystem::clear()
{
	m_soundMap.clear();
	m_ambSoundSet.clear();
	m_lastpos.set(0.0f,0.0f,0.0f);
}
crSound *crSoundSystem::createSound(const std::string& fileName)
{
	std::string name = crArgumentParser::getFileNameEliminateExt(crArgumentParser::getSimpleFileName(fileName));
	ref_ptr<crSound> sound = findSound(name);
	if(!sound.valid() && CRIOManager::fileExists(fileName))
	{
		sound = new crSound;
		sound->setName(name);
		sound->setFileName(fileName);
		addSound(sound.get());
	}
	return sound.get();
}
void crSoundSystem::addSound(crSound *sound)
{ 
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_soundMutex);
	m_soundMap[sound->getName()] = sound; 
}
void crSoundSystem::insertAmbSound(crSound *sound)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_soundMutex);
	m_ambSoundSet.insert(sound);
}
crSound* crSoundSystem::findSound(const std::string &name)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_soundMutex);
	SoundMap::iterator itr = m_soundMap.find(name);
	return itr == m_soundMap.end()?NULL:itr->second.get();
}
void crSoundSystem::update(const CRCore::crMatrixd &viewMat,float dt)
{
	bool ambchange = false;
	bool disableSound = crDisplaySettings::instance()->getDisableSound();
	float ambsoundVolume = 0.0f;
	if(!disableSound)
	{
		ambsoundVolume = crDisplaySettings::instance()->getAmbSoundVolume();
	}
	if(ambsoundVolume != m_ambsoundVolume)
	{
		m_ambsoundVolume = ambsoundVolume;
		ambchange = true;
	}
	if(ambchange)
	{
		ref_ptr<crSound> sound;
		for( AmbSoundSet::iterator itr = m_ambSoundSet.begin();
			itr != m_ambSoundSet.end();
			++itr )
		{
			sound = *itr;
			sound->getChannel()->setVolume(m_ambsoundVolume);
		}
	}

	crVector3 eye,center,up;
	viewMat.getLookAt(eye,center,up);
	crVector3 vel = (eye-m_lastpos) / dt;
	m_lastpos = eye;
	crVector3 dir = center - eye;
	dir.normalize();
	FMOD_VECTOR listener_pos = { eye[0],eye[1],eye[2] };
	FMOD_VECTOR listener_vel = { vel[0],vel[1],vel[2] };
	FMOD_VECTOR listener_forward = { dir[0],dir[1],dir[2] };
	FMOD_VECTOR listener_up = { up[0],up[1],up[2] };
	m_system->set3DListenerAttributes(0, &listener_pos, &listener_vel, &listener_forward, &listener_up);     // update 'ears'

	m_system->update();
}