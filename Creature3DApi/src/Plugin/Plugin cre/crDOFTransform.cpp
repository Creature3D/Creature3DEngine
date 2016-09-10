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
#include <Plugin cre/crDOFTransform.h>
#include <Plugin cre/crTransform.h>

using namespace cre;

void crDOFTransform::write(crDataOutputStream* out){
    // Write crDOFTransform's identification.
    out->writeInt(CREDOFTRANSFORM);
    // If the CRCore class is inherited by any other class we should also write this to file.
    CRCore::crTransform*  trans = dynamic_cast<CRCore::crTransform*>(this);
    if(trans){
        ((cre::crTransform*)(trans))->write(out);
    }
    else
        throw crException("crDOFTransform::write(): Could not cast this CRCore::crDOFTransform to an CRCore::crTransform.");

    // Write crDOFTransform's properties.
    out->writeMatrixd(getPutMatrix());

    out->writeVec3(getMinHPR());
    out->writeVec3(getMaxHPR());
    out->writeVec3(getIncrementHPR());
    out->writeVec3(getCurrentHPR());
    out->writeVec3b(getEndStopHPR());

    out->writeVec3(getMinTranslate());
    out->writeVec3(getMaxTranslate());
    out->writeVec3(getIncrementTranslate());
    out->writeVec3(getCurrentTranslate());
    out->writeVec3b(getEndStopTrans());

    out->writeVec3(getMinScale());
    out->writeVec3(getMaxScale());
    out->writeVec3(getIncrementScale());
    out->writeVec3(getCurrentScale());
    out->writeVec3b(getEndStopScale());
    //if ( out->getVersion() >= VERSION_0011 )
    //{
    //    out->writeChar((char)(getHPRMultOrder()));
    //}
    
	if ( out->getVersion() >= VERSION_0019 )
	    out->writeUShort(getLimitationFlags());
	else
        out->writeULong(getLimitationFlags());
    out->writeBool(getAnimationOn());

	out->writeUShort(m_surgeFlags);
	//out->writeBool(m_endStop);

	out->writeBool(m_endless);
	out->writeFloat(m_lifeTime);
	out->writeFloat(m_startTime);
	out->writeFloat(m_resetTime);

	if ( out->getVersion() >= VERSION_0019 )
	{
	    out->writeVec3(m_O);
		out->writeVec3(m_xAxis);
		out->writeVec3(m_xyPlane);
	}
}

void crDOFTransform::read(crDataInputStream* in){
    // Peek on crDOFTransform's identification.
    int id = in->peekInt();
    if(id == CREDOFTRANSFORM){
        // Read crDOFTransform's identification.
        id = in->readInt();
        // If the CRCore class is inherited by any other class we should also read this from file.
        CRCore::crTransform*  trans = dynamic_cast<CRCore::crTransform*>(this);
        if(trans){
            ((cre::crTransform*)(trans))->read(in);
        }
        else
            throw crException("crDOFTransform::read(): Could not cast this CRCore::crDOFTransform to an CRCore::crTransform.");

        // Read crDOFTransform's properties
        setPutMatrix(in->readMatrixd());
        setInversePutMatrix( CRCore::crMatrixd::inverse( getPutMatrix() ) );

        setMinHPR(in->readVec3());
        setMaxHPR(in->readVec3());
        setIncrementHPR(in->readVec3());
        setCurrentHPR(in->readVec3());
		setEndStopHPR(in->readVec3b());

        setMinTranslate(in->readVec3());
        setMaxTranslate(in->readVec3());
        setIncrementTranslate(in->readVec3());
        setCurrentTranslate(in->readVec3());
		setEndStopTrans(in->readVec3b());

        setMinScale(in->readVec3());
        setMaxScale(in->readVec3());
        setIncrementScale(in->readVec3());
        setCurrentScale(in->readVec3());
        setEndStopScale(in->readVec3b());
        //if ( in->getVersion() >= VERSION_0011 )
        //{
        //    setHPRMultOrder((MultOrder)(in->readChar()));
        //}
        if ( in->getVersion() >= VERSION_0019 )
            setLimitationFlags(in->readUShort());
		else
            setLimitationFlags(in->readULong());
        setAnimationOn(in->readBool());

		setSurgeFlags(in->readUShort());
		//setEndStop(in->readBool());

		m_endless = in->readBool();
		m_lifeTime = in->readFloat();
		m_startTime = in->readFloat();
		m_resetTime = in->readFloat();

		if ( in->getVersion() >= VERSION_0019 )
		{
			m_O = in->readVec3();
			m_xAxis = in->readVec3();
			m_xyPlane = in->readVec3();
		}
    }
    else{
        throw crException("crDOFTransform::read(): Expected crDOFTransform identification.");
    }
}
