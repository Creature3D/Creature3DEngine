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
#ifndef CRGUI_CRCOMPUTEPOSENVISITOR_H
#define CRGUI_CRCOMPUTEPOSENVISITOR_H 1

#include <CRGUI/crElementNodeVisitor.h>
#include <CRGUI/crElementNode.h>
#include <CRCore/crVector2.h>

namespace CRGUI {

class CRGUI_EXPORT crComputePosENVisitor : public crElementNodeVisitor
{
    public:

        crComputePosENVisitor();
        virtual ~crComputePosENVisitor();
		
		virtual void reset() { m_posX = TREEPOS_X; }

		virtual void apply(crElementNode& enode);

        typedef std::stack<crElementNode*> ENodeStack;
        
		const CRCore::crVector2i getTreeSize();
		const CRCore::crVector2i &getRealSize();

		enum Mode
		{
			ComputeStaticPos,
			ComputeMovingPos,
			ComputeMovingSize,
			ResetMovingPos
		};
        
		inline void setMode( Mode mode ) { m_mode = mode; }
		inline Mode getMode() { return m_mode; }

    protected:

        /** prevent unwanted copy operator.*/
        crComputePosENVisitor& operator = (const crComputePosENVisitor&) { return *this; }

		virtual void computeStaticPos(crElementNode& enode);
		virtual void computeMovingPos(crElementNode& enode);
		virtual void computeMovingSize(crElementNode& enode);
		virtual void resetMovingPos(crElementNode& enode);

		Mode m_mode;
		int m_posX;
		CRCore::crVector2i m_size;

		ENodeStack m_nodeStack;

};
}
#endif