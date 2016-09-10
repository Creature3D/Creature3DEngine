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
#include <Plugin cre/crJoint.h>

#include <CRPhysics/crMatterObject.h>

using namespace cre;

void crJointParameter::write(crDataOutputStream* out){

	out->writeInt(CREJOINTPARAMETER);

	out->writeFloat(m_dParamLoStop);	
	out->writeFloat(m_dParamHiStop);
	out->writeFloat(m_dParamVel);
	out->writeFloat(m_dParamFMax);
	out->writeFloat(m_dParamFudgeFactor);
	out->writeFloat(m_dParamBounce);
	out->writeFloat(m_dParamCFM);
	out->writeFloat(m_dParamStopERP);
	out->writeFloat(m_dParamStopCFM);

	out->writeFloat(m_dParamSuspensionERP);
	out->writeFloat(m_dParamSuspensionCFM);
}

void crJointParameter::read(crDataInputStream* in){

	int id = in->peekInt();
	if(id == CREJOINTPARAMETER)
	{
		id = in->readInt();

		m_dParamLoStop = in->readFloat();	
		m_dParamHiStop = in->readFloat();
		m_dParamVel = in->readFloat();
		m_dParamFMax = in->readFloat();
		m_dParamFudgeFactor = in->readFloat();
		m_dParamBounce = in->readFloat();
		m_dParamCFM = in->readFloat();
		m_dParamStopERP = in->readFloat();
		m_dParamStopCFM = in->readFloat();

		m_dParamSuspensionERP = in->readFloat();
		m_dParamSuspensionCFM = in->readFloat();
	}
	else
	{
		throw crException("crJointParameter::read(): Expected crJointParameter identification");
	}
}

void crJoint::write(crDataOutputStream* out){

	out->writeInt(CREJOINT);

	out->writeBool(m_jointObject[0].valid());
	if(m_jointObject[0].valid())
	{
		out->writeNode(m_jointObject[0].get());
	}
	out->writeBool(m_jointObject[1].valid());
	if(m_jointObject[1].valid())
	{
		out->writeNode(m_jointObject[1].get());
	}
}

void crJoint::read(crDataInputStream* in){

	int id = in->peekInt();
	if(id == CREJOINTPARAMETER)
	{
		id = in->readInt();
        
		if(in->readBool())
		{
			m_jointObject[0] = dynamic_cast<CRPhysics::crMatterObject*>(in->readNode());
		}
		if(in->readBool())
		{
			m_jointObject[1] = dynamic_cast<CRPhysics::crMatterObject*>(in->readNode());
		}
	}
	else
	{
		throw crException("crJoint::read(): Expected crJoint identification");
	}
}

void crJointBall::write(crDataOutputStream* out){

	out->writeInt(CREJOINTBALL);

	CRPhysics::crJoint* joint = dynamic_cast<CRPhysics::crJoint*>(this);
	if(joint)
	{
		static_cast<cre::crJoint*>(joint)->write(out);
	}
	else
		throw crException("crJointBall::write(): Could not cast this CRPhysics::crJointBall to an CRPhysics::crJoint.");

	out->writeVec3(m_anchor1);
    out->writeVec3(m_anchor2);
}

void crJointBall::read(crDataInputStream* in){

	int id = in->peekInt();
	if(id == CREJOINTBALL)
	{
		id = in->readInt();

	    CRPhysics::crJoint* joint = dynamic_cast<CRPhysics::crJoint*>(this);
		if(joint)
		{
			((cre::crJoint*)(joint))->read(in);
		}
		else
			throw crException("crJointBall::read(): Could not cast this CRPhysics::crJointBall to an CRPhysics::crJoint.");

	    m_anchor1 = in->readVec3();
	    m_anchor2 = in->readVec3();
	}
	else
	{
		throw crException("crJointBall::read(): Expected crJointBall identification");
	}
}

void crJointHinge::write(crDataOutputStream* out){

	out->writeInt(CREJOINTHINGE);

	CRPhysics::crJoint* joint = dynamic_cast<CRPhysics::crJoint*>(this);
	if(joint)
	{
		static_cast<cre::crJoint*>(joint)->write(out);
	}
	else
		throw crException("crJointHinge::write(): Could not cast this CRPhysics::crJointHinge to an CRPhysics::crJoint.");

	out->writeVec3(m_anchor1);
    out->writeVec3(m_anchor2);
	out->writeVec3(m_axis1);
	out->writeVec3(m_axis2);
	out->writeQuat(m_qrel);
	out->writeBool(m_parameter1.valid());
	if(m_parameter1.valid())
	{
		out->writeJointParam(m_parameter1.get());
	}
}

void crJointHinge::read(crDataInputStream* in){

	int id = in->peekInt();
	if(id == CREJOINTHINGE)
	{
		id = in->readInt();

	    CRPhysics::crJoint* joint = dynamic_cast<CRPhysics::crJoint*>(this);
		if(joint)
		{
			((cre::crJoint*)(joint))->read(in);
		}
		else
			throw crException("crJointHinge::read(): Could not cast this CRPhysics::crJointHinge to an CRPhysics::crJoint.");

		m_anchor1 = in->readVec3();
		m_anchor2 = in->readVec3();
		m_axis1 = in->readVec3();
		m_axis2 = in->readVec3();
        m_qrel = in->readQuat();
		
		if(in->readBool())
		{
            m_parameter1 = in->readJointParam();
		}

	}
	else
	{
		throw crException("crJointHinge::read(): Expected crJointHinge identification");
	}
}

void crJointSlider::write(crDataOutputStream* out){

	out->writeInt(CREJOINTSLIDER);

	CRPhysics::crJoint* joint = dynamic_cast<CRPhysics::crJoint*>(this);
	if(joint)
	{
		static_cast<cre::crJoint*>(joint)->write(out);
	}
	else
		throw crException("crJointSlider::write(): Could not cast this CRPhysics::crJointSlider to an CRPhysics::crJoint.");

	out->writeVec3(m_axis1);
    out->writeVec3(m_offset);
	out->writeQuat(m_qrel);
	out->writeBool(m_parameter1.valid());
	if(m_parameter1.valid())
	{
		out->writeJointParam(m_parameter1.get());
	}
}

void crJointSlider::read(crDataInputStream* in){

	int id = in->peekInt();
	if(id == CREJOINTSLIDER)
	{
		id = in->readInt();

	    CRPhysics::crJoint* joint = dynamic_cast<CRPhysics::crJoint*>(this);
		if(joint)
		{
			((cre::crJoint*)(joint))->read(in);
		}
		else
			throw crException("crJointSlider::read(): Could not cast this CRPhysics::crJointSlider to an CRPhysics::crJoint.");

		m_axis1 = in->readVec3();
		m_offset = in->readVec3();
        m_qrel = in->readQuat();
		
		if(in->readBool())
		{
            m_parameter1 = in->readJointParam();
		}

	}
	else
	{
		throw crException("crJointSlider::read(): Expected crJointSlider identification");
	}
}

void crJointUniversal::write(crDataOutputStream* out){

	out->writeInt(CREJOINTUNIVERSAL);

	CRPhysics::crJoint* joint = dynamic_cast<CRPhysics::crJoint*>(this);
	if(joint)
	{
		static_cast<cre::crJoint*>(joint)->write(out);
	}
	else
		throw crException("crJointUniversal::write(): Could not cast this CRPhysics::crJointUniversal to an CRPhysics::crJoint.");

	out->writeVec3(m_anchor1);
	out->writeVec3(m_anchor2);
	out->writeVec3(m_axis1);
	out->writeVec3(m_axis2);
	out->writeQuat(m_qrel1);
	out->writeQuat(m_qrel2);

	out->writeBool(m_parameter1.valid());
	if(m_parameter1.valid())
	{
		out->writeJointParam(m_parameter1.get());
	}

	out->writeBool(m_parameter2.valid());
	if(m_parameter2.valid())
	{
		out->writeJointParam(m_parameter2.get());
	}
}

void crJointUniversal::read(crDataInputStream* in){

	int id = in->peekInt();
	if(id == CREJOINTUNIVERSAL)
	{
		id = in->readInt();

		CRPhysics::crJoint* joint = dynamic_cast<CRPhysics::crJoint*>(this);
		if(joint)
		{
			((cre::crJoint*)(joint))->read(in);
		}
		else
			throw crException("crJointUniversal::read(): Could not cast this CRPhysics::crJointUniversal to an CRPhysics::crJoint.");

		m_anchor1 = in->readVec3();
		m_anchor2 = in->readVec3();
		m_axis1 = in->readVec3();
		m_axis2 = in->readVec3();
		m_qrel1 = in->readQuat();
		m_qrel2 = in->readQuat();

		if(in->readBool())
		{
			m_parameter1 = in->readJointParam();
		}

		if(in->readBool())
		{
			m_parameter2 = in->readJointParam();
		}
	}
	else
	{
		throw crException("crJointUniversal::read(): Expected crJointUniversal identification");
	}
}

void crJointContact::write(crDataOutputStream* out){

	out->writeInt(CREJOINTSLIDER);

	CRPhysics::crJoint* joint = dynamic_cast<CRPhysics::crJoint*>(this);
	if(joint)
	{
		static_cast<cre::crJoint*>(joint)->write(out);
	}
	else
		throw crException("crJointContact::write(): Could not cast this CRPhysics::crJointContact to an CRPhysics::crJoint.");

	//int the_m;
	//dContact m_contact;
	///crJointContact只是在碰撞的时候被设置，其生命周期只有在碰撞的一瞬间，没有读写的必要
}

void crJointContact::read(crDataInputStream* in){

	int id = in->peekInt();
	if(id == CREJOINTSLIDER)
	{
		id = in->readInt();

	    CRPhysics::crJoint* joint = dynamic_cast<CRPhysics::crJoint*>(this);
		if(joint)
		{
			((cre::crJoint*)(joint))->read(in);
		}
		else
			throw crException("crJointContact::read(): Could not cast this CRPhysics::crJointContact to an CRPhysics::crJoint.");

	}
	else
	{
		throw crException("crJointContact::read(): Expected crJointContact identification");
	}
}

void crJointHinge2::write(crDataOutputStream* out){

	out->writeInt(CREJOINTHINGE2);

	CRPhysics::crJoint* joint = dynamic_cast<CRPhysics::crJoint*>(this);
	if(joint)
	{
		static_cast<cre::crJoint*>(joint)->write(out);
	}
	else
		throw crException("crJointHinge2::write(): Could not cast this CRPhysics::crJointHinge2 to an CRPhysics::crJoint.");

	out->writeVec3(m_anchor1);
	out->writeVec3(m_anchor2);
	out->writeVec3(m_axis1);
	out->writeVec3(m_axis2);
	out->writeVec3(m_v1);
	out->writeVec3(m_v2);
    out->writeFloat(m_c0);
    out->writeFloat(m_s0);

	out->writeBool(m_parameter1.valid());
	if(m_parameter1.valid())
	{
		out->writeJointParam(m_parameter1.get());
	}

	out->writeBool(m_parameter2.valid());
	if(m_parameter2.valid())
	{
		out->writeJointParam(m_parameter2.get());
	}
}

void crJointHinge2::read(crDataInputStream* in){

	int id = in->peekInt();
	if(id == CREJOINTHINGE2)
	{
		id = in->readInt();

		CRPhysics::crJoint* joint = dynamic_cast<CRPhysics::crJoint*>(this);
		if(joint)
		{
			((cre::crJoint*)(joint))->read(in);
		}
		else
			throw crException("crJointHinge2::read(): Could not cast this CRPhysics::crJointHinge2 to an CRPhysics::crJoint.");

		m_anchor1 = in->readVec3();
		m_anchor2 = in->readVec3();
		m_axis1 = in->readVec3();
		m_axis2 = in->readVec3();
		m_v1 = in->readVec3();
		m_v2 = in->readVec3();
		m_c0 = in->readFloat();
		m_s0 = in->readFloat();

		if(in->readBool())
		{
			m_parameter1 = in->readJointParam();
		}

		if(in->readBool())
		{
			m_parameter2 = in->readJointParam();
		}
	}
	else
	{
		throw crException("crJointHinge2::read(): Expected crJointHinge2 identification");
	}
}

void crJointFixed::write(crDataOutputStream* out){

	out->writeInt(CREJOINTFIXED);

	CRPhysics::crJoint* joint = dynamic_cast<CRPhysics::crJoint*>(this);
	if(joint)
	{
		static_cast<cre::crJoint*>(joint)->write(out);
	}
	else
		throw crException("crJointFixed::write(): Could not cast this CRPhysics::crJointFixed to an CRPhysics::crJoint.");

	out->writeVec3(m_offset);
	out->writeQuat(m_qrel);
}

void crJointFixed::read(crDataInputStream* in){

	int id = in->peekInt();
	if(id == CREJOINTFIXED)
	{
		id = in->readInt();

		CRPhysics::crJoint* joint = dynamic_cast<CRPhysics::crJoint*>(this);
		if(joint)
		{
			((cre::crJoint*)(joint))->read(in);
		}
		else
			throw crException("crJointFixed::read(): Could not cast this CRPhysics::crJointFixed to an CRPhysics::crJoint.");

		m_qrel = in->readQuat();
		m_offset = in->readVec3();

	}
	else
	{
		throw crException("crJointFixed::read(): Expected crJointFixed identification");
	}
}

void crJointAMotor::write(crDataOutputStream* out){

	out->writeInt(CREJOINTAMOTOR);

	CRPhysics::crJoint* joint = dynamic_cast<CRPhysics::crJoint*>(this);
	if(joint)
	{
		static_cast<cre::crJoint*>(joint)->write(out);
	}
	else
		throw crException("crJointAMotor::write(): Could not cast this CRPhysics::crJointAMotor to an CRPhysics::crJoint.");
    
	out->writeInt(m_aMotorMode);
	out->writeInt(m_num);
	out->writeInt(m_rel[0]);
	out->writeInt(m_rel[1]);
	out->writeInt(m_rel[2]);
	out->writeFloat(m_angle[0]);
	out->writeFloat(m_angle[1]);
	out->writeFloat(m_angle[2]);
	out->writeVec3(m_axis[0]);
	out->writeVec3(m_axis[1]);
	out->writeVec3(m_axis[2]);
	out->writeVec3(m_reference1);
	out->writeVec3(m_reference2);

	out->writeBool(m_parameter1.valid());
	if(m_parameter1.valid())
	{
		out->writeJointParam(m_parameter1.get());
	}

	out->writeBool(m_parameter2.valid());
	if(m_parameter2.valid())
	{
		out->writeJointParam(m_parameter2.get());
	}

	out->writeBool(m_parameter3.valid());
	if(m_parameter3.valid())
	{
		out->writeJointParam(m_parameter3.get());
	}
}

void crJointAMotor::read(crDataInputStream* in){

	int id = in->peekInt();
	if(id == CREJOINTAMOTOR)
	{
		id = in->readInt();

		CRPhysics::crJoint* joint = dynamic_cast<CRPhysics::crJoint*>(this);
		if(joint)
		{
			((cre::crJoint*)(joint))->read(in);
		}
		else
			throw crException("crJointAMotor::read(): Could not cast this CRPhysics::crJointAMotor to an CRPhysics::crJoint.");

        m_aMotorMode = (AMotorMode)(in->readInt());
		m_num = in->readInt();
        m_rel[0] = in->readInt();
		m_rel[1] = in->readInt();
		m_rel[2] = in->readInt();
		m_angle[0] = in->readFloat();
		m_angle[1] = in->readFloat();
		m_angle[2] = in->readFloat();
		m_axis[0] = in->readVec3();
		m_axis[1] = in->readVec3();
		m_axis[2] = in->readVec3();
		m_reference1 = in->readVec3();
		m_reference2 = in->readVec3();

		if(in->readBool())
		{
			m_parameter1 = in->readJointParam();
		}

		if(in->readBool())
		{
			m_parameter2 = in->readJointParam();
		}

		if(in->readBool())
		{
			m_parameter3 = in->readJointParam();
		}
	}
	else
	{
		throw crException("crJointAMotor::read(): Expected crJointAMotor identification");
	}
}

void crJointNull::write(crDataOutputStream* out){

	out->writeInt(CREJOINTNULL);

	CRPhysics::crJoint* joint = dynamic_cast<CRPhysics::crJoint*>(this);
	if(joint)
	{
		static_cast<cre::crJoint*>(joint)->write(out);
	}
	else
		throw crException("crJointNull::write(): Could not cast this CRPhysics::crJointNull to an CRPhysics::crJoint.");

}

void crJointNull::read(crDataInputStream* in){

	int id = in->peekInt();
	if(id == CREJOINTNULL)
	{
		id = in->readInt();

		CRPhysics::crJoint* joint = dynamic_cast<CRPhysics::crJoint*>(this);
		if(joint)
		{
			((cre::crJoint*)(joint))->read(in);
		}
		else
			throw crException("crJointNull::read(): Could not cast this CRPhysics::crJointNull to an CRPhysics::crJoint.");

	}
	else
	{
		throw crException("crJointNull::read(): Expected crJointNull identification");
	}
}
