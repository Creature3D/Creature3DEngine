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
#ifndef CRSOUND_CRSOUNDSTATE_H
#define CRSOUND_CRSOUNDSTATE_H


#include <CRSound/crExport.h>
#include <limits>
#include <string>
#include <map>
#include <cassert>
#include <vector>

#include <CRCore/Referenced.h>
#include <CRCore/crVector3.h>
#include <fmod.hpp>
#include <fmod_errors.h>


namespace CRSound {

class CRSOUND_EXPORT crSound : public CRCore::Referenced
{
public:
	crSound();
	void setName(const std::string& name) { m_name = name; }
	std::string getName() const { return m_name; }
	void setFileName(const std::string& file) { m_fileName = file; }
	std::string getFileName() const { return m_fileName; }

	void createSound(FMOD_MODE mode, FMOD_CREATESOUNDEXINFO *exinfo = NULL);
	void playSound(FMOD_CHANNELINDEX channelid, bool paused);
	FMOD::Sound *getSound(){ return m_sound; }
	FMOD::Channel *getChannel(){ return m_channel; }
protected:
	~crSound(); 
	std::string m_name;
	std::string m_fileName;
	FMOD::Sound *m_sound;
	FMOD::Channel *m_channel;
};

}
#endif
