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
#ifndef CRGUI_CRELEMENTNODE_H
#define CRGUI_CRELEMENTNODE_H 1

#include <CRGUI/crExport.h>
#include <CRGUI/crTypes.h>
#include <CRGUI/crElement.h>
#include <CRCore/crNode.h>

#include "rcfg/ConfigScriptOO.h"
#include <stack>
#include <string>

#define		TREEPOS_X		20
#define		TREEPOS_Y		20
#define		NODESIZE_X		50
#define		NODESIZE_Y		26
#define		SPACEPERNODE	8
#define		SPACEPERLEVEL	14

namespace CRGUI{
class crElementNodeVisitor;
class CRGUI_EXPORT crElementNode : public crElement
{
public:
	crElementNode();
	crElementNode(const crElementNode& element);
	META_Element(CRGUI,crElementNode)
	virtual void load(rcfg::ConfigScript& cfg_script, float widthScale, float heightScale);
	virtual void initWindow();
	enum Mode
	{
		CONTRACT,
		EXPAND
	};
	
	void setMode( Mode mode );
	inline Mode getMode( ){ return m_mode; }

	void setTextAttribute(crTextAttribute *textAttr) { m_textAttribute = textAttr; }
	virtual bool testButtons(int mx, int my, int mouseButtonMsg);

	inline void setPos2( int x, int y){ m_pos2.set(x,y); }
	inline void initPos2(){ setPos2( m_rect.x(), m_rect.y() ); }
	inline CRCore::crVector2i getPos2()const { return m_pos2; }

	typedef std::vector< CRCore::ref_ptr<crElementNode> > ElementNodeArray;
	void addChild(crElementNode *nodeElement);
	inline unsigned int getNumChildren() const { return m_childs.size(); }
	inline unsigned int getNumChildren() { return m_childs.size(); }

	void setNode(CRCore::crNode *node);
	void setCaption(const std::string &caption);

	virtual void accept(crElementNodeVisitor& nv);
	virtual void ascend(crElementNodeVisitor& nv);
	virtual void traverse(crElementNodeVisitor& nv);

	inline bool isSelected(){ return m_bSelected; }
	inline void select(bool select){ m_bSelected = select; }

	bool checkMoveable_Child()const;
	bool checkMoveable_Brother()const;
	bool checkMoveable( const crElementNode *peNode )const;

	bool _insert(crElementNode *insertNode, int insertAt = 0);
	static bool _delete();
	bool _replace(CRCore::crNode *sampleNode, bool isAll = true);

	static void recToHistory( bool rec ){ m_rectoHistory = rec; }
protected:
	virtual ~crElementNode();
	void addParent(crElementNode *nodeElement);

	//crTextAttribute *getTextAttribute(CRCore::crNode &node);
	//crTextAttribute *getTextAttribute(CRCore::crGroup &node);
	//crTextAttribute *getTextAttribute(CRCore::crLod &node);
	//crTextAttribute *getTextAttribute(CRCore::crSequence &node);
	//crTextAttribute *getTextAttribute(CRCore::crSwitch &node);
	//crTextAttribute *getTextAttribute(CRCore::crPagedLOD &node);
	//crTextAttribute *getTextAttribute(CRCore::crObject &node);

	virtual int drawStageCopy(Display dc, Display down_dc);
	virtual int drawStageMask(Display dc, Display down_dc);
	virtual int drawStageTransparent(Display dc, Display down_dc){ return 1; }

	CRCore::ref_ptr<crTextAttribute> m_textAttribute;
	std::string m_caption;
	//std::string m_nodeName;

	ElementNodeArray m_parents;
    ElementNodeArray m_childs;

	CRCore::ref_ptr<CRCore::crNode> m_node;
	Mode  m_mode;

	CRCore::crVector2i m_pos2;//for move
	bool m_bSelected;
	static bool m_rectoHistory;

	static HPEN   m_whitePen0;
	static HPEN   m_whitePen2;
	std::string m_hScrollBarName;
	std::string m_vScrollBarName;
	CRCore::ref_ptr<crScrollBarElement> m_hScrollBar;
	CRCore::ref_ptr<crScrollBarElement> m_vScrollBar;
};

typedef std::stack<crElementNode*> ENodePath;

}
#endif