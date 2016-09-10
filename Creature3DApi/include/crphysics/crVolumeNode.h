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
#ifndef CRPHYSICS_CRVOLUME_H
#define CRPHYSICS_CRVOLUME_H

#include <CRCore/crNode.h>
#include <CRCore/crPolytope.h>
#include <CRCore/crVector3.h>
#include <CRCore/crFrameStamp.h>
#include <CRPhysics/crViewMatterObject.h>

#include <map>
	
namespace CRPhysics{
class CRPHYSICS_EXPORT crVolumeNode : public CRCore::crGroup
{
	friend class crVolumeNodeManager;
public:
	crVolumeNode();
	crVolumeNode(const crVolumeNode &volumeNode,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);

	META_Node(CRPhysics,crVolumeNode)
	virtual void releaseObjects(CRCore::crState* state);
	virtual void traverse(CRCore::crNodeVisitor& nv);
	void crVolumeNode::trigger(/*const CRCore::crFrameStamp* frameStamp*/);	
	virtual void testVolumeImplementation(crMatterObject *materObject);
	//virtual bool intersects(const CRCore::crBoundingBox &bbox);
	virtual bool intersects(const CRCore::crVector3 &pos);
	//inline virtual void setEffectPosition(const CRCore::crVector3 &position){}
	//inline virtual void setEffectMatrix(const CRCore::crMatrix& mat){}
	//inline virtual bool isEffectIdle() const {  return !getVisiable(); }
	//inline virtual void startEffect(){ setVisiable(true); }
	//inline virtual void stopEffect(){ setVisiable(false); }
	typedef std::set< CRCore::ref_ptr<crMatterObject> > InVolumeNodeSet;
	inline bool isInVolume(crMatterObject *materObject) { return m_inVolumeNodeSet.find(materObject) != m_inVolumeNodeSet.end(); }
	void inVolume(crMatterObject *materObject);
	void outVolume(crMatterObject *materObject);
    void tryEnableVolume(bool enable);
	void setEnable(bool enable);
	bool getEnable();
	void setMaxTriggerCount(int triggerCount);
	int getMaxTriggerCount();
	void setReTriggerDelay(float delay);
	float getReTriggerDelay();
	void setMainBody(CRCore::crNode *mainBody);
	CRCore::crNode *getMainBody();
protected:
	virtual ~crVolumeNode(){}
	bool doVolumeEvent(crMatterObject *materObject);
	void reset();
	void testVolumeEvent(crMatterObject *materObject,bool tested);
    bool m_enable;
	int m_maxTriggerCount;//-1表示无穷
	float m_reTriggerDelay;
	CRCore::ref_ptr<CRCore::crNode> m_mainBody;
	//运行时数据
	float m_interval;
	int m_triggerCount;
	InVolumeNodeSet m_inVolumeNodeSet;
};

class CRPHYSICS_EXPORT crSphereVolumeNode : public crVolumeNode
{
public:
	crSphereVolumeNode();
	crSphereVolumeNode(const crSphereVolumeNode &volumeNode,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);

	META_Node(CRPhysics,crSphereVolumeNode)
	virtual void testVolumeImplementation(crMatterObject *materObject);
	virtual bool intersects(const CRCore::crVector3 &pos);
protected:
	virtual ~crSphereVolumeNode(){}
};

class CRPHYSICS_EXPORT crPolytopeVolumeNode : public crVolumeNode
{
public:
	crPolytopeVolumeNode();
	crPolytopeVolumeNode(const crPolytopeVolumeNode &volumeNode,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);

	META_Node(CRPhysics,crPolytopeVolumeNode)
	virtual void testVolumeImplementation(crMatterObject *materObject);
	//virtual bool intersects(const CRCore::crBoundingBox &bbox);
	virtual bool intersects(const CRCore::crVector3 &pos);
protected:
	virtual ~crPolytopeVolumeNode(){}
	CRCore::crPolytope m_polytope;
};

class CRPHYSICS_EXPORT crMatrixVolumeNode : public crVolumeNode
{
public:
	crMatrixVolumeNode();
	crMatrixVolumeNode(const crMatrixVolumeNode &volumeNode,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);

	META_Node(CRPhysics,crMatrixVolumeNode)
	virtual void testVolumeImplementation(crMatterObject *materObject);
	//virtual bool intersects(const CRCore::crBoundingBox &bbox);
	virtual bool intersects(const CRCore::crVector3 &pos);

	inline virtual void setEffectPosition(const CRCore::crVector3 &position){m_matrix.setTrans(position);}
	inline virtual void setEffectMatrix(const CRCore::crMatrix& mat){m_matrix = mat;}
	inline virtual CRCore::crMatrix& getEffectMatrix(){ return m_matrix; }
protected:
	virtual ~crMatrixVolumeNode(){}
	CRCore::crMatrix m_matrix;
};

class CRPHYSICS_EXPORT crVolumeNodeManager : public CRCore::crNode
{
public :
	crVolumeNodeManager();
	static crVolumeNodeManager *getInstance();

	crVolumeNodeManager(const crVolumeNodeManager&,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY){}
	META_Node(CRPhysics, crVolumeNodeManager);
	virtual void traverse(CRCore::crNodeVisitor& nv);

	typedef std::multimap< std::string, CRCore::ref_ptr<crVolumeNode> > VolumeNodeMap;
	typedef std::list< CRCore::ref_ptr<crVolumeNode> > VolumeNodeList;
	void testVolume(crMatterObject *materObject);
	void removeVolumeNode(crVolumeNode *volumeNode);
	crVolumeNode *getVolumeNode(const std::string &name);
	//crVolumeNode *reuseOrgetOrCreateAnIdleEffectNodes(const std::string &name);
	void wantAddVolumeNode(crVolumeNode *volumeNode);
	virtual void clear();
protected:
	virtual ~crVolumeNodeManager(){}
	void addVolumeNode(crVolumeNode *volumeNode);
	static CRCore::ref_ptr<crVolumeNodeManager> m_instance;
	VolumeNodeMap m_volumeNodeMap;
	VolumeNodeList m_addVolumeNodeBuf;
	VolumeNodeList m_removeVolumeNodeBuf;
};
}
#endif

	