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
#ifndef CRCORE_CRSEARCHNODEBYNAMEVISITOR_H
#define CRCORE_CRSEARCHNODEBYNAMEVISITOR_H 1

#include <CRCore/crNodeVisitor.h>
#include <CRCore/crNode.h>
#include <CRCore/crEventGroup.h>
#include <CRCore/crLightSource.h>
#include <CRCore/crDB.h>
#include <CRCore/crObject.h>
#include <CRCore/crArgumentParser.h>
#include <CRCore/crMatrixTransform.h>
#include <CRCore/crExport.h>
#include <CRCore/crMultiSwitch.h>
#include <CRCore/crLod.h>
#include <CRCore/crPagedLOD.h>
#include <CRCore/crSequence.h>
#include <CRCore/crWidgetNode.h>
#include <string>

namespace CRCore {
enum SearchNodeType
{
	ALLNODE,
	GROUP,
	LIGHTSOURCE,
	EVENTGROUP,
	TRANSFORM,
	MATRIXTRANSFORM,
	OBJECT,
	NODE,
	MULTISWITCH,
	LOD,
	PAGEDLOD,
	SEQUENCE,
	WIDGETNODE,
};
class CR_EXPORT crSearchNodeBYNameVisitor : public CRCore::crNodeVisitor
{
public:
	crSearchNodeBYNameVisitor():crNodeVisitor(NODE_VISITOR,TRAVERSE_ALL_CHILDREN){}
	crSearchNodeBYNameVisitor(std::string &nameid, SearchNodeType stype = ALLNODE):
	    crNodeVisitor(NODE_VISITOR,TRAVERSE_ALL_CHILDREN),
		m_nameid(nameid),m_searchNodeType(stype){}
	virtual ~crSearchNodeBYNameVisitor(){}

    virtual void apply(crNode& node);
	virtual void apply(crGroup& node);
	virtual void apply(crObject& node);
	virtual void apply(crEventGroup& node);
	virtual void apply(crLightSource& node);
	virtual void apply(crTransform& node);
	virtual void apply(crMatrixTransform& node);
	virtual void apply(crMultiSwitch& node);
    virtual void apply(crLod& node);
    virtual void apply(crPagedLOD& node);
	virtual void apply(crSequence& node);
	virtual void apply(crWidgetNode& node);

	inline void setNameId( const std::string &nameid ){ m_nameid = nameid; }
	inline void setSearchNodeType( SearchNodeType stype ) { m_searchNodeType = stype; }
	inline void reset(){ m_result = NULL; m_searchNodeType = ALLNODE; setTraversalMode(crNodeVisitor::TRAVERSE_ALL_CHILDREN); }
	inline void clearResult(){ m_result = NULL; }
	inline crNode *getResult(){ return m_result.get(); }

protected:

	inline void handle(CRCore::crNode& node)
	{
		std::string str;
		if( node.getName().compare(m_nameid)==0 )
		{
			m_result = &node;
		}
	}

	SearchNodeType m_searchNodeType;
	std::string m_nameid;
	ref_ptr<crNode> m_result;
};

class CR_EXPORT crCollectNodeBYNameVisitor : public CRCore::crNodeVisitor
{
public:
	crCollectNodeBYNameVisitor(SearchNodeType stype = ALLNODE):
		crNodeVisitor(NODE_VISITOR,TRAVERSE_ALL_CHILDREN),
		m_searchNodeType(stype){}
	virtual ~crCollectNodeBYNameVisitor(){}

	virtual void apply(crNode& node);
	virtual void apply(crGroup& node);
	virtual void apply(crObject& node);
	virtual void apply(crEventGroup& node);
	virtual void apply(crLightSource& node);
	virtual void apply(crTransform& node);
	virtual void apply(crMatrixTransform& node);
	virtual void apply(crMultiSwitch& node);
	virtual void apply(crLod& node);
	virtual void apply(crPagedLOD& node);
	virtual void apply(crSequence& node);
	virtual void apply(crWidgetNode& node);

	typedef std::set<std::string> NameSet;
	inline void insertNodeNameId( const std::string &name ){ m_nodeNameSet.insert(name); }
	inline void setSearchNodeType( SearchNodeType stype ) { m_searchNodeType = stype; }
	inline void reset(){ m_result.clear(); m_nodeNameSet.clear(); m_searchNodeType = ALLNODE; setTraversalMode(crNodeVisitor::TRAVERSE_ALL_CHILDREN); }
	inline void clearResult(){ m_result.clear(); }
	inline NodeArray &getResult(){ return m_result; }
protected:
	SearchNodeType m_searchNodeType;
	NameSet m_nodeNameSet;
	NodeArray m_result;
	inline void handle(CRCore::crNode& node)
	{
		std::string str;
		if( m_nodeNameSet.find(node.getName()) != m_nodeNameSet.end() )
		{
			m_result.push_back(&node);
		}
	}
};

class CR_EXPORT crSearchNodeBYClassNameVisitor : public CRCore::crNodeVisitor
{
public:
	crSearchNodeBYClassNameVisitor():crNodeVisitor(NODE_VISITOR,TRAVERSE_ALL_CHILDREN){}
	virtual ~crSearchNodeBYClassNameVisitor(){}

	virtual void apply(crNode& node);

	typedef std::set<std::string> NameSet;
	inline void insertClassNameId( const std::string &classname ){ m_classNameSet.insert(classname); }
	inline void reset(){ m_result = NULL; m_classNameSet.clear(); setTraversalMode(crNodeVisitor::TRAVERSE_ALL_CHILDREN); }
	inline void clearResult(){ m_result=NULL; }
	inline crNode *getResult(){ return m_result.get(); }
protected:
	NameSet m_classNameSet;
	ref_ptr<crNode> m_result;
};

class CR_EXPORT crCollectNodeBYClassNameVisitor : public CRCore::crNodeVisitor
{
public:
	crCollectNodeBYClassNameVisitor():crNodeVisitor(NODE_VISITOR,TRAVERSE_ALL_CHILDREN){}
	virtual ~crCollectNodeBYClassNameVisitor(){}

	virtual void apply(crNode& node);

	typedef std::set<std::string> NameSet;
	inline void insertClassNameId( const std::string &classname ){ m_classNameSet.insert(classname); }
	inline void reset(){ m_result.clear(); m_classNameSet.clear(); setTraversalMode(crNodeVisitor::TRAVERSE_ALL_CHILDREN); }
	inline void clearResult(){ m_result.clear(); }
	inline NodeArray &getResult(){ return m_result; }
protected:
	NameSet m_classNameSet;
	NodeArray m_result;
};

class CR_EXPORT crSearchNodeHasDataEventVisitor : public CRCore::crNodeVisitor
{
public:
	crSearchNodeHasDataEventVisitor():crNodeVisitor(NODE_VISITOR,TRAVERSE_PARENTS){}
	virtual ~crSearchNodeHasDataEventVisitor(){}

	virtual void apply(crNode& node);

	inline void reset(){ m_result = NULL;setTraversalMode(crNodeVisitor::TRAVERSE_PARENTS); }
	inline void clearResult(){ m_result=NULL; }
	inline crNode *getResult(){ return m_result.get(); }
protected:
	ref_ptr<crNode> m_result;
};

class CR_EXPORT crCollectNodeBYCallbackVisitor : public CRCore::crNodeVisitor
{
public:
	crCollectNodeBYCallbackVisitor():crNodeVisitor(NODE_VISITOR,TRAVERSE_ALL_CHILDREN){}
	virtual ~crCollectNodeBYCallbackVisitor(){}

	virtual void apply(crNode& node);

	inline void setCallback( const std::string &callback ){ m_callback = callback; }
	inline void reset(){ m_result.clear(); m_callback.clear(); setTraversalMode(crNodeVisitor::TRAVERSE_ALL_CHILDREN); }
	inline void clearResult(){ m_result.clear(); }
	inline NodeArray &getResult(){ return m_result; }
protected:
	std::string m_callback;
	NodeArray m_result;
};

class CR_EXPORT crContainNodeVisitor : public CRCore::crNodeVisitor
{
public:
	crContainNodeVisitor():crNodeVisitor(NODE_VISITOR,TRAVERSE_ALL_CHILDREN),m_node(NULL),m_result(false){}
	virtual ~crContainNodeVisitor(){}

	virtual void apply(crNode& node);

	inline void setNode( crNode *node ){ m_node = node; }
	inline void reset(){ m_result = false; m_node = NULL; setTraversalMode(crNodeVisitor::TRAVERSE_ALL_CHILDREN); }
	inline void clearResult(){ m_result = false; }
	inline bool getResult(){ return m_result; }
protected:
	crNode *m_node;
	bool m_result;
};
}

#endif

