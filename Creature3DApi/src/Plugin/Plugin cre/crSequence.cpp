/* Creature3D - Online Game Engine, Copyright (C) 2005 吴财华(26756325@qq.com)
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
#include <Plugin cre/crSequence.h>
#include <Plugin cre/crGroup.h>

using namespace cre;

void crSequence::write(crDataOutputStream* out){
	// Write crSequence's identification.
	out->writeInt(CRESEQUENCE);
	// If the CRCore class is inherited by any other class we should also write this to file.
	CRCore::crGroup*  group = dynamic_cast<CRCore::crGroup*>(this);
	if(group){
		((cre::crGroup*)(group))->write(out);
	}
	else
		throw crException("crSequence::write(): Could not cast this CRCore::crSequence to an CRCore::crGroup.");
	// Write crSequence's properties.

    // Write frame times.
	int size = getNumChildren();
	out->writeInt(size);
	for(int i=0;i<size;i++){
		out->writeFloat(getTime(i));
	}
    // Write loop mode & interval
	CRCore::crSequence::LoopMode mode;
    int begin, end;
    getInterval(mode, begin, end);
	out->writeInt(mode);
	out->writeInt(begin);
	out->writeInt(end);
	// Write duration
    float speed;
    int nreps;
    getDuration(speed, nreps);
	out->writeFloat(speed);
	out->writeInt(nreps);
    // Write sequence mode
    out->writeInt(getMode());
	out->writeBool(m_vanishWhenStop);
}

void crSequence::read(crDataInputStream* in){
	// Peek on crSequence's identification.
	int id = in->peekInt();
	if(id == CRESEQUENCE){
		// Read crSequence's identification.
		id = in->readInt();
		// If the CRCore class is inherited by any other class we should also read this from file.
		CRCore::crGroup*  group = dynamic_cast<CRCore::crGroup*>(this);
		if(group){
			((cre::crGroup*)(group))->read(in);
		}
		else
			throw crException("crSequence::read(): Could not cast this CRCore::crSequence to an CRCore::crGroup.");
		// Read crSequence's properties
		// Read frame times.
		int size = in->readInt();
		for(int i=0;i<size;i++){
			setTime(i, in->readFloat());
		}
		// Read loop mode & interval
		int mode = in->readInt();
		int begin = in->readInt();
		int end = in->readInt();
	    setInterval((CRCore::crSequence::LoopMode)mode, begin, end);
		// Read duration
		float speed = in->readFloat();
		int nreps = in->readInt();
		setDuration(speed, nreps);
	    // Read sequence mode
		setMode((CRCore::crSequence::SequenceMode)in->readInt());
		m_vanishWhenStop = in->readBool();
	}
	else{
		throw crException("crSequence::read(): Expected crSequence identification.");
	}
}
/////////////////////////////////
//
//crMatrixSequence
//
////////////////////////////////////
void crMatrixSequence::write(crDataOutputStream* out){
	// Write crSequence's identification.
	out->writeInt(CREMATRIXSEQUENCE);
	// If the CRCore class is inherited by any other class we should also write this to file.
	CRCore::crSequence*  group = dynamic_cast<CRCore::crSequence*>(this);
	if(group){
		((cre::crSequence*)(group))->write(out);
	}
	if ( out->getVersion() >= VERSION_0017 )
	    out->writeMatrixf(m_matrix);
	else
		throw crException("crMatrixSequence::write(): Could not cast this CRCore::crMatrixSequence to an CRCore::crSequence.");
}

void crMatrixSequence::read(crDataInputStream* in){
	// Peek on crSequence's identification.
	int id = in->peekInt();
	if(id == CREMATRIXSEQUENCE){
		// Read crSequence's identification.
		id = in->readInt();
		// If the CRCore class is inherited by any other class we should also read this from file.
		CRCore::crSequence*  group = dynamic_cast<CRCore::crSequence*>(this);
		if(group){
			((cre::crSequence*)(group))->read(in);
		}
        if ( in->getVersion() >= VERSION_0017 )
		    setEffectMatrix(in->readMatrixf());
		else
			throw crException("crMatrixSequence::read(): Could not cast this CRCore::crMatrixSequence to an CRCore::crSequence.");

	}
	else{
		throw crException("crMatrixSequence::read(): Expected crMatrixSequence identification.");
	}
}
