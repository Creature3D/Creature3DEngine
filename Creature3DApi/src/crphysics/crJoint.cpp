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
#include <CRPhysics\crJoint.h>
#include <CRPhysics\crMatterObject.h>

using namespace CRCore;
using namespace CRPhysics;

#define dParamLoStop1          dParamLoStop
#define dParamHiStop1          dParamHiStop
#define dParamVel1             dParamVel
#define dParamFMax1            dParamFMax
#define dParamFudgeFactor1     dParamFudgeFactor
#define dParamBounce1          dParamBounce
#define dParamCFM1             dParamCFM
#define dParamStopERP1         dParamStopERP
#define dParamStopCFM1         dParamStopCFM
#define dParamSuspensionERP1   dParamSuspensionERP
#define dParamSuspensionCFM1   dParamSuspensionCFM

#define RegistJointParam(JointSetParam,i) \
	if(m_parameter##i->m_dParamLoStop != DEFAULTJOINTPARAM)\
	    JointSetParam(m_jointID,dParamLoStop##i,m_parameter##i->m_dParamLoStop);\
	if(m_parameter##i->m_dParamHiStop != DEFAULTJOINTPARAM)\
	    JointSetParam(m_jointID,dParamHiStop##i,m_parameter##i->m_dParamHiStop);\
	if(m_parameter##i->m_dParamVel != DEFAULTJOINTPARAM)\
	    JointSetParam(m_jointID,dParamVel##i,m_parameter##i->m_dParamVel);\
	if(m_parameter##i->m_dParamFMax != DEFAULTJOINTPARAM)\
	    JointSetParam(m_jointID,dParamFMax##i,m_parameter##i->m_dParamFMax);\
	if(m_parameter##i->m_dParamFudgeFactor != DEFAULTJOINTPARAM)\
	    JointSetParam(m_jointID,dParamFudgeFactor##i,m_parameter##i->m_dParamFudgeFactor);\
	if(m_parameter##i->m_dParamBounce != DEFAULTJOINTPARAM)\
	    JointSetParam(m_jointID,dParamBounce##i,m_parameter##i->m_dParamBounce);\
	if(m_parameter##i->m_dParamCFM != DEFAULTJOINTPARAM)\
	    JointSetParam(m_jointID,dParamCFM##i,m_parameter##i->m_dParamCFM);\
	if(m_parameter##i->m_dParamStopERP != DEFAULTJOINTPARAM)\
	    JointSetParam(m_jointID,dParamStopERP##i,m_parameter##i->m_dParamStopERP);\
	if(m_parameter##i->m_dParamStopCFM != DEFAULTJOINTPARAM)\
	    JointSetParam(m_jointID,dParamStopCFM##i,m_parameter##i->m_dParamStopCFM);\
	if(m_parameter##i->m_dParamSuspensionERP != DEFAULTJOINTPARAM)\
	    JointSetParam(m_jointID,dParamSuspensionERP##i,m_parameter##i->m_dParamSuspensionERP);\
	if(m_parameter##i->m_dParamSuspensionCFM != DEFAULTJOINTPARAM)\
	    JointSetParam(m_jointID,dParamSuspensionCFM##i,m_parameter##i->m_dParamSuspensionCFM);

void crJointBall::create(dWorldID worldID, dJointGroupID jointGroupID)
{
	m_jointID = dJointCreateBall(worldID,jointGroupID);
	dJointAttach (m_jointID, m_jointObject[0]->getBodyID(), m_jointObject[1]->getBodyID());
	dJointSetData(m_jointID,m_data);
	
	dJointSetBallAnchor(m_jointID,m_anchor1.x(),m_anchor1.y(),m_anchor1.z());
 
	registJointParam();
}

void crJointHinge::create(dWorldID worldID, dJointGroupID jointGroupID)
{
	m_jointID = dJointCreateHinge(worldID,jointGroupID);
	dJointAttach (m_jointID, m_jointObject[0]->getBodyID(), m_jointObject[1]->getBodyID());
	dJointSetData(m_jointID,m_data);

	dJointSetHingeAnchor(m_jointID,m_anchor1.x(),m_anchor1.y(),m_anchor1.z());
    dJointSetHingeAxis(m_jointID,m_axis1.x(),m_axis1.y(),m_axis1.z());
	
	registJointParam();
}

void crJointHinge::registJointParam()
{
 /*   dJointSetHingeParam(m_jointID,dParamLoStop,m_parameter->m_dParamLoStop);
	dJointSetHingeParam(m_jointID,dParamHiStop,m_parameter->m_dParamHiStop);
	dJointSetHingeParam(m_jointID,dParamVel,m_parameter->m_dParamVel);
	dJointSetHingeParam(m_jointID,dParamFMax,m_parameter->m_dParamFMax);
	dJointSetHingeParam(m_jointID,dParamFudgeFactor,m_parameter->m_dParamFudgeFactor);
	dJointSetHingeParam(m_jointID,dParamBounce,m_parameter->m_dParamBounce);
	dJointSetHingeParam(m_jointID,dParamCFM,m_parameter->m_dParamCFM);
	dJointSetHingeParam(m_jointID,dParamStopERP,m_parameter->m_dParamStopERP);
	dJointSetHingeParam(m_jointID,dParamStopCFM,m_parameter->m_dParamStopCFM);*/

	RegistJointParam(dJointSetHingeParam,1)
}

void crJointSlider::create(dWorldID worldID, dJointGroupID jointGroupID)
{
	m_jointID = dJointCreateSlider(worldID,jointGroupID);
	dJointAttach (m_jointID, m_jointObject[0]->getBodyID(), m_jointObject[1]->getBodyID());
	dJointSetData(m_jointID,m_data);

	dJointSetSliderAxis(m_jointID,m_axis1.x(),m_axis1.y(),m_axis1.z());
	
    registJointParam();
}

void crJointSlider::registJointParam()
{
	//dJointSetSliderParam(m_jointID,dParamLoStop,m_parameter->m_dParamLoStop);
	//dJointSetSliderParam(m_jointID,dParamHiStop,m_parameter->m_dParamHiStop);
	//dJointSetSliderParam(m_jointID,dParamVel,m_parameter->m_dParamVel);
	//dJointSetSliderParam(m_jointID,dParamFMax,m_parameter->m_dParamFMax);
	//dJointSetSliderParam(m_jointID,dParamFudgeFactor,m_parameter->m_dParamFudgeFactor);
	//dJointSetSliderParam(m_jointID,dParamBounce,m_parameter->m_dParamBounce);
	//dJointSetSliderParam(m_jointID,dParamCFM,m_parameter->m_dParamCFM);
	//dJointSetSliderParam(m_jointID,dParamStopERP,m_parameter->m_dParamStopERP);
	//dJointSetSliderParam(m_jointID,dParamStopCFM,m_parameter->m_dParamStopCFM);
	RegistJointParam(dJointSetSliderParam,1)
}

void crJointUniversal::create(dWorldID worldID, dJointGroupID jointGroupID)
{
	m_jointID = dJointCreateUniversal(worldID,jointGroupID);
	dJointAttach (m_jointID, m_jointObject[0]->getBodyID(), m_jointObject[1]->getBodyID());
	dJointSetData(m_jointID,m_data);

	dJointSetUniversalAnchor(m_jointID,m_anchor1.x(),m_anchor1.y(),m_anchor1.z());
	dJointSetUniversalAxis1(m_jointID,m_axis1.x(),m_axis1.y(),m_axis1.z());
	dJointSetUniversalAxis2(m_jointID,m_axis2.x(),m_axis2.y(),m_axis2.z());

    registJointParam();
}

void crJointUniversal::registJointParam()
{
	//dJointSetUniversalParam(m_jointID,dParamLoStop,m_parameter1->m_dParamLoStop);
	//dJointSetUniversalParam(m_jointID,dParamHiStop,m_parameter1->m_dParamHiStop);
	//dJointSetUniversalParam(m_jointID,dParamVel,m_parameter1->m_dParamVel);
	//dJointSetUniversalParam(m_jointID,dParamFMax,m_parameter1->m_dParamFMax);
	//dJointSetUniversalParam(m_jointID,dParamFudgeFactor,m_parameter1->m_dParamFudgeFactor);
	//dJointSetUniversalParam(m_jointID,dParamBounce,m_parameter1->m_dParamBounce);
	//dJointSetUniversalParam(m_jointID,dParamCFM,m_parameter1->m_dParamCFM);
	//dJointSetUniversalParam(m_jointID,dParamStopERP,m_parameter1->m_dParamStopERP);
	//dJointSetUniversalParam(m_jointID,dParamStopCFM,m_parameter1->m_dParamStopCFM);

	//dJointSetUniversalParam(m_jointID,dParamLoStop2,m_parameter2->m_dParamLoStop);
	//dJointSetUniversalParam(m_jointID,dParamHiStop2,m_parameter2->m_dParamHiStop);
	//dJointSetUniversalParam(m_jointID,dParamVel2,m_parameter2->m_dParamVel);
	//dJointSetUniversalParam(m_jointID,dParamFMax2,m_parameter2->m_dParamFMax);
	//dJointSetUniversalParam(m_jointID,dParamFudgeFactor2,m_parameter2->m_dParamFudgeFactor);
	//dJointSetUniversalParam(m_jointID,dParamBounce2,m_parameter2->m_dParamBounce);
	//dJointSetUniversalParam(m_jointID,dParamCFM2,m_parameter2->m_dParamCFM);
	//dJointSetUniversalParam(m_jointID,dParamStopERP2,m_parameter2->m_dParamStopERP);
	//dJointSetUniversalParam(m_jointID,dParamStopCFM2,m_parameter2->m_dParamStopCFM);

	RegistJointParam(dJointSetUniversalParam,1)
	RegistJointParam(dJointSetUniversalParam,2)
}

void crJointContact::create(dWorldID worldID, dJointGroupID jointGroupID)
{
	m_jointID = dJointCreateContact(worldID,jointGroupID,&m_contact);
	dJointAttach (m_jointID, m_jointObject[0]->getBodyID(), m_jointObject[1]->getBodyID());
	dJointSetData(m_jointID,m_data);

	registJointParam();
}

void crJointContact::registJointParam()
{

}

void crJointHinge2::create(dWorldID worldID, dJointGroupID jointGroupID)
{
	m_jointID = dJointCreateHinge2(worldID,jointGroupID);
	dJointAttach (m_jointID, m_jointObject[0]->getBodyID(), m_jointObject[1]->getBodyID());
	dJointSetData(m_jointID,m_data);

	dJointSetHinge2Anchor(m_jointID,m_anchor1.x(),m_anchor1.y(),m_anchor1.z());
	dJointSetHinge2Axis1(m_jointID,m_axis1.x(),m_axis1.y(),m_axis1.z());
	dJointSetHinge2Axis2(m_jointID,m_axis2.x(),m_axis2.y(),m_axis2.z());

    registJointParam();
}

void crJointHinge2::registJointParam()
{
	//if(m_parameter1->m_dParamLoStop != DEFAULTJOINTPARAM)
	//    dJointSetHinge2Param(m_jointID,dParamLoStop,m_parameter1->m_dParamLoStop);
	//if(m_parameter1->m_dParamHiStop != DEFAULTJOINTPARAM)
	//    dJointSetHinge2Param(m_jointID,dParamHiStop,m_parameter1->m_dParamHiStop);
	//if(m_parameter1->m_dParamVel != DEFAULTJOINTPARAM)
	//    dJointSetHinge2Param(m_jointID,dParamVel,m_parameter1->m_dParamVel);
	//if(m_parameter1->m_dParamFMax != DEFAULTJOINTPARAM)
	//    dJointSetHinge2Param(m_jointID,dParamFMax,m_parameter1->m_dParamFMax);
	//if(m_parameter1->m_dParamFudgeFactor != DEFAULTJOINTPARAM)
 //   	dJointSetHinge2Param(m_jointID,dParamFudgeFactor,m_parameter1->m_dParamFudgeFactor);
	//if(m_parameter1->m_dParamBounce != DEFAULTJOINTPARAM)
	//    dJointSetHinge2Param(m_jointID,dParamBounce,m_parameter1->m_dParamBounce);
	//if(m_parameter1->m_dParamCFM != DEFAULTJOINTPARAM)
	//    dJointSetHinge2Param(m_jointID,dParamCFM,m_parameter1->m_dParamCFM);
	//if(m_parameter1->m_dParamStopERP != DEFAULTJOINTPARAM)
	//    dJointSetHinge2Param(m_jointID,dParamStopERP,m_parameter1->m_dParamStopERP);
	//if(m_parameter1->m_dParamStopCFM != DEFAULTJOINTPARAM)
	//    dJointSetHinge2Param(m_jointID,dParamStopCFM,m_parameter1->m_dParamStopCFM);
 //	if(m_parameter1->m_dParamSuspensionERP != DEFAULTJOINTPARAM)
	//    dJointSetHinge2Param(m_jointID,dParamSuspensionERP,m_parameter1->m_dParamSuspensionERP);
	//if(m_parameter1->m_dParamSuspensionCFM != DEFAULTJOINTPARAM)
	//    dJointSetHinge2Param(m_jointID,dParamSuspensionCFM,m_parameter1->m_dParamSuspensionCFM);

    RegistJointParam(dJointSetHinge2Param,1)
    RegistJointParam(dJointSetHinge2Param,2)
	     
	//if(m_parameter2->m_dParamLoStop != DEFAULTJOINTPARAM)
	//    dJointSetHinge2Param(m_jointID,dParamLoStop2,m_parameter2->m_dParamLoStop);
	//if(m_parameter2->m_dParamHiStop != DEFAULTJOINTPARAM)
	//    dJointSetHinge2Param(m_jointID,dParamHiStop2,m_parameter2->m_dParamHiStop);
	//if(m_parameter2->m_dParamVel != DEFAULTJOINTPARAM)
	//	dJointSetHinge2Param(m_jointID,dParamVel2,m_parameter2->m_dParamVel);
	//if(m_parameter2->m_dParamFMax != DEFAULTJOINTPARAM)
	//	dJointSetHinge2Param(m_jointID,dParamFMax2,m_parameter2->m_dParamFMax);
	//if(m_parameter2->m_dParamFudgeFactor != DEFAULTJOINTPARAM)
	//	dJointSetHinge2Param(m_jointID,dParamFudgeFactor2,m_parameter2->m_dParamFudgeFactor);

	////dJointSetHinge2Param(m_jointID,dParamBounce2,m_parameter2->m_dParamBounce);
	////dJointSetHinge2Param(m_jointID,dParamCFM2,m_parameter2->m_dParamCFM);
	////dJointSetHinge2Param(m_jointID,dParamStopERP2,m_parameter2->m_dParamStopERP);
	////dJointSetHinge2Param(m_jointID,dParamStopCFM2,m_parameter2->m_dParamStopCFM);
	//if(m_parameter2->m_dParamSuspensionERP != DEFAULTJOINTPARAM)
	//	dJointSetHinge2Param(m_jointID,dParamSuspensionERP2,m_parameter2->m_dParamSuspensionERP);
	//if(m_parameter2->m_dParamSuspensionCFM != DEFAULTJOINTPARAM)
	//	dJointSetHinge2Param(m_jointID,dParamSuspensionCFM2,m_parameter2->m_dParamSuspensionCFM);

	////dJointSetHinge2Param(m_jointID,dParamSuspensionERP2,m_parameter2->m_dParamSuspensionERP);
	////dJointSetHinge2Param(m_jointID,dParamSuspensionCFM2,m_parameter2->m_dParamSuspensionCFM);
}

float crJointHinge2::jointGetHinge2Angle1()
{
	if(!m_jointID)
		return 0;
    return dJointGetHinge2Angle1(m_jointID);
}

float crJointHinge2::jointGetHinge2Angle2Rate()
{
	if(!m_jointID)
		return 0;
	return dJointGetHinge2Angle2Rate(m_jointID);
}

void crJointFixed::create(dWorldID worldID, dJointGroupID jointGroupID)
{
	m_jointID = dJointCreateFixed(worldID,jointGroupID);
	dJointAttach (m_jointID, m_jointObject[0]->getBodyID(), m_jointObject[1]->getBodyID());
	dJointSetData(m_jointID,m_data);
	
	dJointSetFixed (m_jointID);

	registJointParam();
}

void crJointFixed::registJointParam()
{

}

crJointAMotor::crJointAMotor():
  m_aMotorMode(AMotorUser),m_num(3)
{ 
	m_parameter1 = new crJointParameter; 
	m_parameter2 = new crJointParameter; 
	m_parameter3 = new crJointParameter; 
    
	for(int i = 0; i<3; i++ )
	{
		m_rel[i] = 1;
		m_angle[i] = 0;
	}

}
void crJointAMotor::setAMotorMode( AMotorMode mode )
{
    m_aMotorMode = mode;
}

void crJointAMotor::setAMotorNumAxes( int num )
{
    m_num = num;
}

void crJointAMotor::create(dWorldID worldID, dJointGroupID jointGroupID)
{
	m_jointID = dJointCreateAMotor(worldID,jointGroupID);
	dJointAttach (m_jointID, m_jointObject[0]->getBodyID(), m_jointObject[1]->getBodyID());
	dJointSetData(m_jointID,m_data);

	dJointSetAMotorMode(m_jointID,m_aMotorMode);
	dJointSetAMotorNumAxes(m_jointID,m_num);
	for(int i = 0; i<m_num; i++ )
	{
	    dJointSetAMotorAxis(m_jointID,i,m_rel[i],m_axis[i][0],m_axis[i][1],m_axis[i][2]);
		dJointSetAMotorAngle (m_jointID, i, m_angle[i]);
	}

    registJointParam();
}

void crJointAMotor::registJointParam()
{
	//dJointSetAMotorParam(m_jointID,dParamLoStop,m_parameter1->m_dParamLoStop);
	//dJointSetAMotorParam(m_jointID,dParamHiStop,m_parameter1->m_dParamHiStop);
	//dJointSetAMotorParam(m_jointID,dParamVel,m_parameter1->m_dParamVel);
	//dJointSetAMotorParam(m_jointID,dParamFMax,m_parameter1->m_dParamFMax);
	//dJointSetAMotorParam(m_jointID,dParamFudgeFactor,m_parameter1->m_dParamFudgeFactor);
	//dJointSetAMotorParam(m_jointID,dParamBounce,m_parameter1->m_dParamBounce);
	//dJointSetAMotorParam(m_jointID,dParamCFM,m_parameter1->m_dParamCFM);
	//dJointSetAMotorParam(m_jointID,dParamStopERP,m_parameter1->m_dParamStopERP);
	//dJointSetAMotorParam(m_jointID,dParamStopCFM,m_parameter1->m_dParamStopCFM);

	//dJointSetAMotorParam(m_jointID,dParamLoStop2,m_parameter2->m_dParamLoStop);
	//dJointSetAMotorParam(m_jointID,dParamHiStop2,m_parameter2->m_dParamHiStop);
	//dJointSetAMotorParam(m_jointID,dParamVel2,m_parameter2->m_dParamVel);
	//dJointSetAMotorParam(m_jointID,dParamFMax2,m_parameter2->m_dParamFMax);
	//dJointSetAMotorParam(m_jointID,dParamFudgeFactor2,m_parameter2->m_dParamFudgeFactor);
	//dJointSetAMotorParam(m_jointID,dParamBounce2,m_parameter2->m_dParamBounce);
	//dJointSetAMotorParam(m_jointID,dParamCFM2,m_parameter2->m_dParamCFM);
	//dJointSetAMotorParam(m_jointID,dParamStopERP2,m_parameter2->m_dParamStopERP);
	//dJointSetAMotorParam(m_jointID,dParamStopCFM2,m_parameter2->m_dParamStopCFM);

	//dJointSetAMotorParam(m_jointID,dParamLoStop3,m_parameter3->m_dParamLoStop);
	//dJointSetAMotorParam(m_jointID,dParamHiStop3,m_parameter3->m_dParamHiStop);
	//dJointSetAMotorParam(m_jointID,dParamVel3,m_parameter3->m_dParamVel);
	//dJointSetAMotorParam(m_jointID,dParamFMax3,m_parameter3->m_dParamFMax);
	//dJointSetAMotorParam(m_jointID,dParamFudgeFactor3,m_parameter3->m_dParamFudgeFactor);
	//dJointSetAMotorParam(m_jointID,dParamBounce3,m_parameter3->m_dParamBounce);
	//dJointSetAMotorParam(m_jointID,dParamCFM3,m_parameter3->m_dParamCFM);
	//dJointSetAMotorParam(m_jointID,dParamStopERP3,m_parameter3->m_dParamStopERP);
	//dJointSetAMotorParam(m_jointID,dParamStopCFM3,m_parameter3->m_dParamStopCFM);

	RegistJointParam(dJointSetAMotorParam,1)
	RegistJointParam(dJointSetAMotorParam,2)
	RegistJointParam(dJointSetAMotorParam,2)
}