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
#ifndef CRGUI_CRIELEMENTNODEMANAGER_H
#define CRGUI_CRIELEMENTNODEMANAGER_H 1

#include <CRGUI/crExport.h>
#include <CRGUI\crTypes.h>
#include <CRGUI/crElementNode.h>
#include <CRCore/crEventGroup.h>
#include <CRCore/crVector2.h>
#include <CRCore/crVector3.h>
#include <CRCore/crVector4.h>
#include <CRCore/crCopyOp.h>
#include "rcfg/ConfigScriptOO.h"
#include <map>
#include <set>
#include <string>

namespace CRGUI{

class CRGUI_EXPORT crElementNodeManager : public CRCore::Referenced
{
public:
	crElementNodeManager();

	static crElementNodeManager *getInstance();
	void reset();

	typedef std::vector< CRCore::ref_ptr<crElementNode> > SelectedNodeList;
	SelectedNodeList *getSelectedNodes() { return m_selectedNodes; }
	SelectedNodeList *getLastSelectedNodes() { return m_lastSelectedNodes; }
	inline int getNumSelectedNodes(){ return m_selectedNodes->size(); }
	bool validToReSelect(const CRCore::crVector2i& point);
	void initSelect();
	void resetSelect();
	void select(crElementNode* enode);

	bool isNodeBeSelected(crElementNode *enode);

	void copySelectedNodes();
	SelectedNodeList& getCopiedNodes(){ return m_copyedNodes; }

	void modify( bool modifyFlag );
	bool isModify();

	virtual void clear();
protected:
	virtual ~crElementNodeManager();
	static CRCore::ref_ptr<crElementNodeManager> m_instance;
	void clearSelectBox(  SelectedNodeList *selectedNodes );
	void showSelectBox( crElementNode* enode );

	SelectedNodeList *m_selectedNodes;
	SelectedNodeList *m_lastSelectedNodes;
	SelectedNodeList m_copyedNodes;
	bool m_modifyFlag;
};
}
#endif