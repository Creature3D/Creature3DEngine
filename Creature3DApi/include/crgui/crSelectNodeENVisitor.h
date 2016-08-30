/* Creature3D - Online Game Engine, Copyright (C) 2005 吴财华(26756325@qq.com)
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
#include <CRCore\ref_ptr.h>
#include <CRGUI\crElementNodeVisitor.h>
#include <CRGUI\crElementNode.h>
#include <CRCore\crVector2.h>
#include <CRCore\crVector4.h>

#include <vector>

namespace CRGUI {

class CRGUI_EXPORT crSelectNodeENVisitor : public crElementNodeVisitor
{
    public:

        crSelectNodeENVisitor(int x, int y);
		crSelectNodeENVisitor(CRCore::crVector2i &point);
		crSelectNodeENVisitor(int left, int right, int top, int bottom);
		crSelectNodeENVisitor(CRCore::crVector4i &rect);
        virtual ~crSelectNodeENVisitor();
		
		virtual void apply(crElementNode& enode);
                        
		enum Flag
		{
            POINTSEL,
			RECTSEL,
			RECTINTERSECT
		};
        
		void setFlag(Flag flg){ m_flag = flg; }

    protected:

        /** prevent unwanted copy operator.*/
        crSelectNodeENVisitor& operator = (const crSelectNodeENVisitor&) { return *this; }

        CRCore::crVector2i m_point;//点选
		CRCore::crVector4i m_rect;//框选left top right buttom
        Flag m_flag;
};

}
#endif