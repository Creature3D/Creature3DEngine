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
#ifndef CRENCAPSULATION_CRNODEVISITORS
#define CRENCAPSULATION_CRNODEVISITORS 1

#include <CREncapsulation/crExport.h>
#include <CRCore/crNodeVisitor.h>
#include <CRCore/crObject.h>
namespace CREncapsulation {
class CRENCAPSULATION_EXPORT crDisconnectDrawableVisitor : public CRCore::crNodeVisitor
{
public:
	crDisconnectDrawableVisitor():
	  crNodeVisitor(NODE_VISITOR,TRAVERSE_ALL_CHILDREN) {}

	  virtual ~crDisconnectDrawableVisitor(){}
	  virtual void apply(CRCore::crObject& node);
};
class CRENCAPSULATION_EXPORT crRectSelectNodeVisitor : public CRCore::crNodeVisitor
{
public:
	crRectSelectNodeVisitor():
		crNodeVisitor(NODE_VISITOR,TRAVERSE_ALL_CHILDREN) {}

	virtual ~crRectSelectNodeVisitor(){}
	void setSelectType(int _type);
	void setSelectClassName(const std::string &classname);
	void setRect(CRCore::crVector4i &rect);
	virtual void apply(CRCore::crNode& node);
	virtual void apply(CRCore::crGroup& node);
	virtual void apply(CRCore::crObject& node);
	virtual void apply(CRCore::crTransform& node);
	typedef std::vector< CRCore::ref_ptr<CRCore::crNode> >SelectNodeVec;
	SelectNodeVec &getResult() { return m_selectNodeVec; }

	typedef std::vector<CRCore::ref_ptr<CRCore::RefMatrix> > MatrixList;
	inline void pushMatrix( CRCore::RefMatrix* matrix)
	{
		m_matrixList.push_back(matrix);
	}
	inline void popMatrix()
	{
		m_matrixList.pop_back();
	}
	inline CRCore::crMatrix topMatrix()
	{
		if(m_matrixList.empty())
			return CRCore::crMatrix::identity();
		else
			return *(m_matrixList.back().get());
	}
protected:
	void doSelect(CRCore::crNode &node);
	MatrixList                              m_matrixList;
	int m_selectType;//0:ProxyNode, 1:PickObject, 2:PickMesh, >=3:ClassName, 21:PickEditorNode
	std::string m_classname;
	CRCore::crBoundingBox m_rectBB;
	SelectNodeVec m_selectNodeVec;
};
/////////////////////////////////////////
//
//crLoadUISceneVisitor
//
/////////////////////////////////////////
class CRENCAPSULATION_EXPORT crLoadUISceneVisitor : public CRCore::crNodeVisitor
{
public:
	crLoadUISceneVisitor():
		crNodeVisitor(NODE_VISITOR,TRAVERSE_ALL_CHILDREN) {}

	virtual ~crLoadUISceneVisitor(){}

	void init(CRCore::crGroup *root) { m_root= root; }
	virtual void apply(CRCore::crGroup& node);
protected:
	CRCore::crGroup *m_root;
};
}
#endif