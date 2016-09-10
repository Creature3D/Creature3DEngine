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
#include <Plugin cre/crException.h>
#include <Plugin cre/crLightSource.h>
#include <Plugin cre/crGroup.h>
#include <Plugin cre/crLight.h>
#include <CRCore/crLightSourceManager.h>
#include <CRCore/crDisplaySettings.h>
using namespace cre;

void crLightSource::write(crDataOutputStream* out){
    // Write crLightSource's identification.
    out->writeInt(CRELIGHTSOURCE);
    // If the CRCore class is inherited by any other class we should also write this to file.
    CRCore::crGroup*  group = dynamic_cast<CRCore::crGroup*>(this);
    if(group){
        ((cre::crGroup*)(group))->write(out);
    }
    else
        throw crException("crLightSource::write(): Could not cast this CRCore::crLightSource to an CRCore::crGroup.");
    // Write crLightSource's properties.

    // Write out light
    out->writeBool(getLight()!=0);
    if(getLight())
    {
		out->writeStateAttribute(getLight());
        //((cre::crLight*)(getLight()))->write(out);
    }

    // Write reference frame
    out->writeInt((int)getReferenceFrame());

	out->writeBool(getEnable());
	out->writeBool(getGlobal());
	out->writeBool(getLightIsSunLight());
	out->writeBool(getStdModeEnable());
	if ( out->getVersion() >= VERSION_0026 )
	{
		out->writeBool(m_calcShadow);
		out->writeBool(m_calcShadow2);
	}
	if ( out->getVersion() >= VERSION_0042 )
	{
		out->writeBool(m_spotLightMap.valid());
		if(m_spotLightMap.valid())
			out->writeStateAttribute(m_spotLightMap.get());
	}
}

void crLightSource::read(crDataInputStream* in){
    // Peek on crLightSource's identification.
    int id = in->peekInt();
    if(id == CRELIGHTSOURCE){
        // Read crLightSource's identification.
        id = in->readInt();
        // If the CRCore class is inherited by any other class we should also read this from file.
        CRCore::crGroup*  group = dynamic_cast<CRCore::crGroup*>(this);
        if(group){
            ((cre::crGroup*)(group))->read(in);
        }
        else
            throw crException("crLightSource::read(): Could not cast this CRCore::crLightSource to an CRCore::crBase.");
        // Read crLightSource's properties

        // Read light
        if(in->readBool()){
            //CRCore::crLight* light = new CRCore::crLight();
            //((cre::crLight*)(light))->read(in);
            //setLight(light);
			setLight(dynamic_cast<CRCore::crLight*>(in->readStateAttribute()));
        }

        // Read reference frame
        setReferenceFrame((CRCore::crLightSource::ReferenceFrame) in->readInt());

		setEnable(in->readBool());
		setGlobal(in->readBool());
		setLightToBeSunLight(in->readBool());
		setStdModeEnable(in->readBool());
		if ( in->getVersion() >= VERSION_0026 )
		{
			m_calcShadow = in->readBool();
			m_calcShadow2 = in->readBool();
		}
		if ( in->getVersion() >= VERSION_0042 )
		{
			if(in->readBool())
				setSpotLightMap(dynamic_cast<CRCore::crTexture2D *>(in->readStateAttribute()));
		}
		CRCore::crLight *light = dynamic_cast<CRCore::crLight *>(getLight());
		if(!light||light->getLightType()==crLight::LIGHT_INFINITE)//平行光不计算阴影
		{
			CRCore::notify(CRCore::WARN)<<"CREncapsulation::crLightSource::read: 引擎不支持平行光源"<<std::endl;
		}
		else
			CRCore::crLightSourceManager::getInstance()->insertLightSource(this);
		initLightParamStruct();
		//if(getLightIsSunLight())
		//{
		//	setCalcShadow(CRCore::crDisplaySettings::instance()->getShadowDetail()!=0);
		//}
    }
    else{
        throw crException("crLightSource::read(): Expected crLightSource identification.");
    }
}
