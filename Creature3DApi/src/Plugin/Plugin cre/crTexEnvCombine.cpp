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
#include <Plugin cre/crTexEnvCombine.h>
#include <Plugin cre/crBase.h>

using namespace cre;

void crTexEnvCombine::write(crDataOutputStream* out){
	// Write crTexEnvCombine's identification.
	out->writeInt(CRETEXENVCOMBINE);
	// If the CRCore class is inherited by any other class we should also write this to file.
	CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
	if(obj){
		((cre::crBase*)(obj))->write(out);
	}
	else
		throw crException("crTexEnvCombine::write(): Could not cast this CRCore::crTexEnvCombine to an CRCore::crBase.");
	// Write crTexEnvCombine's properties.
	
	// Write combine method
    out->writeInt(getCombine_RGB());
    out->writeInt(getCombine_Alpha());
	// Write source params
	out->writeInt(getSource0_RGB());
	out->writeInt(getSource1_RGB());
	out->writeInt(getSource2_RGB());
	out->writeInt(getSource0_Alpha());
	out->writeInt(getSource1_Alpha());
	out->writeInt(getSource2_Alpha());
	// Write operand params
	out->writeInt(getOperand0_RGB());
	out->writeInt(getOperand1_RGB());
	out->writeInt(getOperand2_RGB());
	out->writeInt(getOperand0_Alpha());
	out->writeInt(getOperand1_Alpha());
	out->writeInt(getOperand2_Alpha());
	// Write scale and constant color
	out->writeFloat(getScale_RGB());
	out->writeFloat(getScale_Alpha());
	out->writeVec4(getConstantColor());

}

void crTexEnvCombine::read(crDataInputStream* in){
	// Peek on crTexEnvCombine's identification.
	int id = in->peekInt();
	if(id == CRETEXENVCOMBINE){
		// Read crTexEnvCombine's identification.
		id = in->readInt();
		// If the CRCore class is inherited by any other class we should also read this from file.
		CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
		if(obj){
			((cre::crBase*)(obj))->read(in);
		}
		else
			throw crException("crTexEnvCombine::read(): Could not cast this CRCore::crTexEnvCombine to an CRCore::crBase.");
		// Read crTexEnvCombine's properties

		// Read combine method
		setCombine_RGB((CombineParam)in->readInt());
        setCombine_Alpha((CombineParam)in->readInt());
		// Read combine source
		setSource0_RGB((SourceParam)in->readInt());
        setSource1_RGB((SourceParam)in->readInt());
        setSource2_RGB((SourceParam)in->readInt());
        setSource0_Alpha((SourceParam)in->readInt());
        setSource1_Alpha((SourceParam)in->readInt());
        setSource2_Alpha((SourceParam)in->readInt());
		// read combine params
        setOperand0_RGB((OperandParam)in->readInt());
        setOperand1_RGB((OperandParam)in->readInt());
        setOperand2_RGB((OperandParam)in->readInt());
        setOperand0_Alpha((OperandParam)in->readInt());
        setOperand1_Alpha((OperandParam)in->readInt());
        setOperand2_Alpha((OperandParam)in->readInt());
		// Read scale and constant color
        setScale_RGB(in->readFloat());
        setScale_Alpha(in->readFloat());
        setConstantColor(in->readVec4());

	}
	else{
		throw crException("crTexEnvCombine::read(): Expected crTexEnvCombine identification.");
	}
}
