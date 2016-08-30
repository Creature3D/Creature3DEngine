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
#ifndef CRGUI_CRDRAWTREEENVISITOR_H
#define CRGUI_CRDRAWTREEENVISITOR_H 1
#include <CRGUI\crElementNodeVisitor.h>
#include <CRGUI\crElementNode.h>

namespace CRGUI {

class CRGUI_EXPORT crDrawTreeENVisitor : public crElementNodeVisitor
{
    public:

        crDrawTreeENVisitor();
        virtual ~crDrawTreeENVisitor();

		virtual void apply(crElementNode& enode);

    protected:
        crDrawTreeENVisitor& operator = (const crDrawTreeENVisitor&) { return *this; }
		typedef std::stack< crElementNode::Mode > ModeStack;
		ModeStack m_modeStack;
};
}
#endif