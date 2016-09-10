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
#ifndef CRUTIL_CRPREPAREVISITOR_H
#define CRUTIL_CRPREPAREVISITOR_H 1

#include <CRUtil/crExport.h>
#include <CRCore/crNodeVisitor.h>
#include <CRCore/crGeometry.h>
#include <CRCore/crStateSet.h>
#include <CRCore/crBillboard.h>
#include <CRCore/crObject.h>
#include <stack>
namespace CRUtil{

class CRUTIL_EXPORT crPrepareVisitor : public CRCore::crNodeVisitor
{
public:
	crPrepareVisitor():
		CRCore::crNodeVisitor(NODE_VISITOR,TRAVERSE_ALL_CHILDREN){}
	virtual ~crPrepareVisitor(){}

	virtual void apply(CRCore::crNode& node);
	virtual void apply(CRCore::crObject &node);
	virtual void apply(CRCore::crBillboard &node);
	enum PrepareMode
	{
		PrepareNone = 0x0,
		PrepareTexture = 0x01,
        PrepareTBN = 0x02,
		PrepareMesh = 0x04,
		Default = PrepareTexture |
		          PrepareTBN
	};
	void prepareMesh(CRCore::crGeometry *geo);
	void prepareTBN(CRCore::crGeometry *geo);
	void prepareStateSet(CRCore::crStateSet *stateset);
protected:
	typedef std::stack<int> PrepareModeStack;
	PrepareModeStack m_prepareModeStack;
};
class CRUTIL_EXPORT crRemoveTransStateVisitor : public CRCore::crNodeVisitor
{
public:
	crRemoveTransStateVisitor():
	  CRCore::crNodeVisitor(NODE_VISITOR,TRAVERSE_ALL_CHILDREN){}
	  virtual ~crRemoveTransStateVisitor(){}

	  virtual void apply(CRCore::crNode& node);
	  virtual void apply(CRCore::crObject &node);
	  void apply(CRCore::crStateSet& stateset);
};
}
#endif
