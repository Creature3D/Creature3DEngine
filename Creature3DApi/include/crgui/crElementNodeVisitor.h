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
#ifndef CRGUI_CRELEMENTNODEVISITOR_H
#define CRGUI_CRELEMENTNODEVISITOR_H 1

#include <CRGUI\crExport.h>
#include <CRCore\ref_ptr.h>
#include <CRCore\referenced.h>
#include <CRGUI\crElementNode.h>

namespace CRGUI {

class CRGUI_EXPORT crElementNodeVisitor : public virtual CRCore::Referenced
{
    public:
		enum TraversalMode
		{
			TRAVERSE_NONE,
			TRAVERSE_PARENTS,
			TRAVERSE_ALL_CHILDREN,
			TRAVERSE_ACTIVE_CHILDREN,
			TRAVERSE_EXPAND_CHILDREN
		};

		enum VisitorType
		{
			ENODE_VISITOR = 0
		};

		crElementNodeVisitor(TraversalMode tm=TRAVERSE_NONE);

		crElementNodeVisitor(VisitorType type,TraversalMode tm=TRAVERSE_NONE);

		virtual ~crElementNodeVisitor();

		virtual void reset() {}

		inline void setVisitorType(VisitorType type) { m_VisitorType = type; }
		inline VisitorType getVisitorType() const { return m_VisitorType; }
		
		inline void setTraversalMode(TraversalMode mode) { m_traversalMode = mode; }
		inline TraversalMode getTraversalMode() const { return m_traversalMode; }

		inline void traverse(crElementNode& enode)
		{
			if (m_traversalMode==TRAVERSE_PARENTS) enode.ascend(*this);
			else if (m_traversalMode!=TRAVERSE_NONE) enode.traverse(*this);
		}

		inline void pushOntoNodePath(crElementNode* enode) { m_nodePath.push(enode); }
		inline void popFromNodePath()            { m_nodePath.pop(); }
		ENodePath& getNodePath() { return m_nodePath; }
		const ENodePath& getNodePath() const { return m_nodePath; }
		
		virtual void apply(crElementNode& enode)   { traverse(enode); }

    protected:
        VisitorType                     m_VisitorType;
		TraversalMode                   m_traversalMode;
		ENodePath                       m_nodePath;
};

struct crElementNodeAcceptOp
{
	crElementNodeVisitor& m_nv;
	crElementNodeAcceptOp(crElementNodeVisitor& nv):m_nv(nv) {}
	void operator () (crElementNode* enode) { enode->accept(m_nv); }
	void operator () (CRCore::ref_ptr<crElementNode> enode) { enode->accept(m_nv); }
};

}
#endif