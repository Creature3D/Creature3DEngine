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
#include <CRSound/crSound.h>
#include <CRSound/crSoundSystem.h>
#include <CRIOManager/crFileNameUtils.h>
using namespace CRSound;

crSound::crSound():
	m_sound(NULL),
	m_channel(NULL)
{ 
}

crSound::~crSound()
{
	if(m_sound)
	{
		FMOD_RESULT result = m_sound->release();
		FMERRCHECK(result);
		m_sound = NULL;
	}
}
void crSound::createSound(FMOD_MODE mode, FMOD_CREATESOUNDEXINFO *exinfo)
{
	if(!m_sound)
	{
		std::string ext = CRIOManager::getFileExtension(m_fileName);
		if( CRIOManager::equalCaseInsensitive(ext,"ogg") ||
			CRIOManager::equalCaseInsensitive(ext,"mp3") )
			mode |= FMOD_CREATECOMPRESSEDSAMPLE;
		FMOD_RESULT result = crSoundSystem::getInstance()->getSystem()->createSound(m_fileName.c_str(), mode|FMOD_VIRTUAL_PLAYFROMSTART, exinfo, &m_sound);
		FMERRCHECK(result);
	}
}
void crSound::playSound(FMOD_CHANNELINDEX channelid, bool paused)
{
	FMOD_RESULT result = crSoundSystem::getInstance()->getSystem()->playSound(channelid, m_sound, paused, &m_channel);
	FMERRCHECK(result);
}