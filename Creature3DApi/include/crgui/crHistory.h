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
#ifndef CRGUI_CRHISTORY_H
#define CRGUI_CRHISTORY_H 1

#include <CRCore/ref_ptr.h>
#include <CRCore/referenced.h>
#include <CRCore/crVector2.h>
#include <CRGUI/crElementNode.h>

#include <string>
#include <stack>
#include <vector>

namespace CRGUI {

class CRGUI_EXPORT crHistory : public CRCore::Referenced
{
public:
	crHistory():
	  m_modifyFlag(MOVE){}


	typedef std::stack< CRCore::ref_ptr<crHistory> > HistoryStack;

	enum ModifyFlag
	{
		MOVE,
		ATTRMODIFY,
		//CONVERT,       注：被记录在MOVE里
		//POSITIONMODIFY 注：被记录在MOVE里
	};

	struct MoveParam : public CRCore::Referenced
	{
		MoveParam():m_srcIndex(0){}
		CRCore::ref_ptr<crElementNode> m_srcParentNode;
		int m_srcIndex;
		CRCore::ref_ptr<crElementNode> m_modifiedNode;
	};

	struct AttrParam : public CRCore::Referenced
	{
		CRCore::ref_ptr<crElementNode> m_srcAttrs;//修改前的这些节点的属性，必须为同类型节点才能一起修改
		CRCore::ref_ptr<crElementNode> m_modifiedNode;
	};
    
	typedef std::vector< CRCore::ref_ptr<MoveParam> > MoveParamList;
	typedef std::vector< CRCore::ref_ptr<AttrParam> > AttrParamList;

	void setModifyFlag( ModifyFlag flag ){ m_modifyFlag = flag; }
	ModifyFlag getModifyFlag()const { return m_modifyFlag; }

	void pushMoveParam(MoveParam *param){ m_moveParams.push_back(param); }
	void pushAttrParam(AttrParam *param){ m_attrParams.push_back(param); }

	static HistoryStack &getHistoryStack(){ return m_historyStack; }
	static void pushHistory( crHistory *history );
    static void popHistory();
	static crHistory *topHistory(){ return m_historyStack.top().get(); }
	static int sizeHistory(){ return m_historyStack.size(); }
    static void undo();
protected:
	virtual ~crHistory(){}
    static HistoryStack m_historyStack;

	crElementNode::ElementNodeArray   m_modifiedNodes;
    ModifyFlag m_modifyFlag;
	MoveParamList m_moveParams;
	AttrParamList m_attrParams;
};

}
#endif
