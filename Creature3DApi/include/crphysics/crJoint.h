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
#ifndef CRPHYSICS_CRJOINT_H
#define CRPHYSICS_CRJOINT_H 1

#include <CRPhysics\crExport.h>
#include <CRPhysics\crMatterObject.h>
#include <CRCore\crVector3.h>
#include <CRCore\crQuat.h>
#include <CRCore\Referenced.h>
#include <CRCore\ref_ptr.h>

#include <ode/ode.h>

#include <vector>

#define DEFAULTJOINTPARAM   FLT_MIN

namespace CRPhysics{

class CRPHYSICS_EXPORT crJointParameter : public CRCore::Referenced
{
public:
	crJointParameter():
      m_dParamLoStop(DEFAULTJOINTPARAM),
	  m_dParamHiStop(DEFAULTJOINTPARAM),
	  m_dParamVel(DEFAULTJOINTPARAM),
	  m_dParamFMax(DEFAULTJOINTPARAM),
	  m_dParamFudgeFactor(DEFAULTJOINTPARAM),
	  m_dParamBounce(DEFAULTJOINTPARAM),
	  m_dParamCFM(DEFAULTJOINTPARAM),
	  m_dParamStopERP(DEFAULTJOINTPARAM),
	  m_dParamStopCFM(DEFAULTJOINTPARAM),
	  m_dParamSuspensionERP(DEFAULTJOINTPARAM),
	  m_dParamSuspensionCFM(DEFAULTJOINTPARAM) {}

    void setParamLoStop( float param ) { m_dParamLoStop = param; }
    void setParamHiStop( float param ) { m_dParamHiStop = param; }
	void setParamVel( float param ) { m_dParamVel = param; }
	void setParamFMax( float param ) { m_dParamFMax = param; }
	void setParamFudgeFactor( float param ) { m_dParamFudgeFactor = param; }
	void setParamBounce( float param ) { m_dParamBounce = param; }
	void setParamCFM( float param ) { m_dParamCFM = param; }
	void setParamStopERP( float param ) { m_dParamStopERP = param; }
	void setParamStopCFM( float param ) { m_dParamStopCFM = param; }
	void setParamSuspensionERP( float param ) { m_dParamSuspensionERP = param; }
	void setParamSuspensionCFM( float param ) { m_dParamSuspensionCFM = param; }

	float    m_dParamLoStop;
	float    m_dParamHiStop;
	float    m_dParamVel;
	float    m_dParamFMax;
	float    m_dParamFudgeFactor;
	float    m_dParamBounce;
	float    m_dParamCFM;
	float    m_dParamStopERP;
	float    m_dParamStopCFM;

	float    m_dParamSuspensionERP;
	float    m_dParamSuspensionCFM;

};

class CRPHYSICS_EXPORT crJoint : public CRCore::Referenced
{
public:
	crJoint():m_data(0){};
	void setJointMatterObject(crMatterObject *jointObject1,crMatterObject *jointObject2)
	{ 
		m_jointObject[0] = jointObject1;
	    m_jointObject[1] = jointObject2;
	}
	crMatterObject *getJointMatterObject(int i)
	{ 
		if(i>0&&i<2) 
			return m_jointObject[i].get();
		return NULL;
	}
    virtual void create(dWorldID worldID, dJointGroupID jointGroupID) = 0;
	virtual void registJointParam(){}

	//virtual void setJointParameters(crJointParameter *p){}
	//virtual void setJointParameters(crJointParameter *p1,crJointParameter *p2){}
	virtual void setJointParameters(crJointParameter *p1,crJointParameter *p2 = NULL,crJointParameter *p3 = NULL){}
	virtual crJointParameter* getJointParameter1() { return NULL; }
	virtual crJointParameter* getJointParameter2() { return NULL; }
	virtual crJointParameter* getJointParameter3() { return NULL; }
    
	virtual void release()const { if(m_jointID) dJointDestroy(m_jointID); m_jointID = NULL; }
protected:
	virtual ~crJoint(){ /*if( m_jointID ) dJointDestroy(m_jointID);*/ };
	CRCore::ref_ptr<crMatterObject>  m_jointObject[2];
	void*                   m_data;
	mutable dJointID  m_jointID;
};

class CRPHYSICS_EXPORT crJointBall : public crJoint
{
public:
	crJointBall(){};
	virtual void create(dWorldID worldID, dJointGroupID jointGroupID);

	void setAnchor1( const CRCore::crVector3 &anchor ) { m_anchor1 = anchor; }
protected:
	virtual ~crJointBall(){};

	CRCore::crVector3     m_anchor1;
	CRCore::crVector3     m_anchor2;

};

class CRPHYSICS_EXPORT crJointHinge : public crJoint
{
public:
	crJointHinge(){ m_parameter1 = new crJointParameter; }

	virtual void create(dWorldID worldID, dJointGroupID jointGroupID);
	virtual void registJointParam();

	virtual void setJointParameters(crJointParameter *p1,crJointParameter *p2 = NULL,crJointParameter *p3 = NULL)
	{ if(p1) m_parameter1 = p1; }

	virtual crJointParameter* getJointParameter1() { return m_parameter1.get(); }

	void setAnchor1( const CRCore::crVector3 &anchor ) { m_anchor1 = anchor; }
	void setAxis1( const CRCore::crVector3 &axis ) { m_axis1 = axis; }
	void setAxis2( const CRCore::crVector3 &axis ) { m_axis2 = axis; }
protected:
	virtual ~crJointHinge(){};

	CRCore::crVector3 m_anchor1;
	CRCore::crVector3 m_anchor2;
	CRCore::crVector3 m_axis1;
	CRCore::crVector3 m_axis2;
	CRCore::crQuat    m_qrel;
	CRCore::ref_ptr<crJointParameter>  m_parameter1;
};

class CRPHYSICS_EXPORT crJointSlider : public crJoint
{
public:
	crJointSlider(){ m_parameter1 = new crJointParameter; }

	virtual void create(dWorldID worldID, dJointGroupID jointGroupID);
	virtual void registJointParam();

	virtual void setJointParameters(crJointParameter *p1,crJointParameter *p2 = NULL,crJointParameter *p3 = NULL)
	{ if(p1) m_parameter1 = p1; }

	virtual crJointParameter* getJointParameter1() { return m_parameter1.get(); }
protected:

	CRCore::crVector3 m_axis1;
	CRCore::crVector3 m_offset;
	CRCore::crQuat    m_qrel;
	CRCore::ref_ptr<crJointParameter> m_parameter1;
};

class CRPHYSICS_EXPORT crJointUniversal : public crJoint
{
public:
	crJointUniversal(){ m_parameter1 = new crJointParameter; m_parameter2 = new crJointParameter; }

	virtual void create(dWorldID worldID, dJointGroupID jointGroupID);
	virtual void registJointParam();

	virtual void setJointParameters(crJointParameter *p1,crJointParameter *p2 = NULL,crJointParameter *p3 = NULL)
	{ if(p1) m_parameter1 = p1; if(p2) m_parameter2 = p2; }

	virtual crJointParameter* getJointParameter1() { return m_parameter1.get(); }
	virtual crJointParameter* getJointParameter2() { return m_parameter2.get(); }

	void setAnchor1( const CRCore::crVector3 &anchor ) { m_anchor1 = anchor; }
	void setAxis1( const CRCore::crVector3 &axis ) { m_axis1 = axis; }
	void setAxis2( const CRCore::crVector3 &axis ) { m_axis2 = axis; }
protected:
	CRCore::crVector3 m_anchor1;
	CRCore::crVector3 m_anchor2;
	CRCore::crVector3 m_axis1;
	CRCore::crVector3 m_axis2;
	CRCore::crQuat m_qrel1;
	CRCore::crQuat m_qrel2;
	CRCore::ref_ptr<crJointParameter> m_parameter1;
	CRCore::ref_ptr<crJointParameter> m_parameter2;

};

class CRPHYSICS_EXPORT crJointContact : public crJoint
{
public:
	virtual void create(dWorldID worldID, dJointGroupID jointGroupID);
	virtual void registJointParam();
protected:
	int the_m;
	dContact m_contact;
};

class CRPHYSICS_EXPORT crJointHinge2 : public crJoint
{
public:
	crJointHinge2(){ m_parameter1 = new crJointParameter; m_parameter2 = new crJointParameter; }
	
	virtual void create(dWorldID worldID, dJointGroupID jointGroupID);
	virtual void registJointParam();
	virtual void setJointParameters(crJointParameter *p1,crJointParameter *p2 = NULL,crJointParameter *p3 = NULL)
	{ if(p1) m_parameter1 = p1; if(p2) m_parameter2 = p2; }

	virtual crJointParameter* getJointParameter1() { return m_parameter1.get(); }
	virtual crJointParameter* getJointParameter2() { return m_parameter2.get(); }

	void setAnchor1( const CRCore::crVector3 &anchor ) { m_anchor1 = anchor; }
	void setAxis1( const CRCore::crVector3 &axis ) { m_axis1 = axis; }
	void setAxis2( const CRCore::crVector3 &axis ) { m_axis2 = axis; }

	float jointGetHinge2Angle1();
    float jointGetHinge2Angle2Rate();
	
protected:
	CRCore::crVector3 m_anchor1;
	CRCore::crVector3 m_anchor2;
	CRCore::crVector3 m_axis1;
	CRCore::crVector3 m_axis2;
	CRCore::crVector3 m_v1;
	CRCore::crVector3 m_v2;
	float m_c0;
	float m_s0;
	CRCore::ref_ptr<crJointParameter> m_parameter1;
	CRCore::ref_ptr<crJointParameter> m_parameter2;

};

class CRPHYSICS_EXPORT crJointFixed : public crJoint
{
public:
	virtual void create(dWorldID worldID, dJointGroupID jointGroupID);
	virtual void registJointParam();
protected:
	CRCore::crQuat m_qrel;
	CRCore::crVector3 m_offset;
};

class CRPHYSICS_EXPORT crJointAMotor : public crJoint
{
public:
	crJointAMotor();

	virtual void create(dWorldID worldID, dJointGroupID jointGroupID);
	virtual void registJointParam();
	enum AMotorMode
	{
		AMotorUser = 0,
		AMotorEuler = 1
	};

	virtual void setJointParameters(crJointParameter *p1,crJointParameter *p2 = NULL,crJointParameter *p3 = NULL)
	{ if(p1) m_parameter1 = p1; if(p2) m_parameter2 = p2; if(p3) m_parameter3 = p3; }
	virtual crJointParameter* getJointParameter1() { return m_parameter1.get(); }
	virtual crJointParameter* getJointParameter2() { return m_parameter2.get(); }
	virtual crJointParameter* getJointParameter3() { return m_parameter3.get(); }

	void setAMotorMode( AMotorMode mode );
	void setAMotorNumAxes( int num );
	inline void setAxis( int anum, const CRCore::crVector3 &axis ) { m_axis[anum] = axis; }
	inline void setRels( int anum, int rel ) { m_rel[anum] = rel; }
	inline void setAMotorAngle( int anum, float angle ) { m_angle[anum] = angle; }

protected:
	AMotorMode m_aMotorMode;
	int m_num;
	int m_rel[3];
	float m_angle[3];
	CRCore::crVector3 m_axis[3];
	CRCore::crVector3 m_reference1;
	CRCore::crVector3 m_reference2;
	CRCore::ref_ptr<crJointParameter> m_parameter1;
	CRCore::ref_ptr<crJointParameter> m_parameter2;
	CRCore::ref_ptr<crJointParameter> m_parameter3;
};

class CRPHYSICS_EXPORT crJointNull : public crJoint
{
public:
	virtual void create(dWorldID worldID, dJointGroupID jointGroupID){};
	virtual void registJointParam(){}

};

class CRPHYSICS_EXPORT crJointMsg : public crJoint
{
public:
	virtual void create(dWorldID worldID, dJointGroupID jointGroupID){};
	virtual void registJointParam(){}

	virtual void setJointParameters(crJointParameter *p1,crJointParameter *p2 = NULL,crJointParameter *p3 = NULL)
	{ m_parameter1 = p1; m_parameter2 = p2; m_parameter3 = p3; }

	virtual crJointParameter* getJointParameter1() { return m_parameter1.get(); }
	virtual crJointParameter* getJointParameter2() { return m_parameter2.get(); }
	virtual crJointParameter* getJointParameter3() { return m_parameter3.get(); }

protected:
    CRCore::ref_ptr<crJointParameter> m_parameter1;
    CRCore::ref_ptr<crJointParameter> m_parameter2;
    CRCore::ref_ptr<crJointParameter> m_parameter3;
};


}

#endif