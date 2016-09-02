/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2006 Robert Osfield 
 *
 * This library is open source and may be redistributed and/or modified under  
 * the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or 
 * (at your option) any later version.  The full license is in LICENSE file
 * included with this distribution, and on the openscenegraph.org website.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * OpenSceneGraph Public License for more details.
*/
//Modified by 吴财华
#include <CRCore/crLogicOp.h>

using namespace CRCore;

crLogicOp::crLogicOp():
    m_opcode(COPY)
{
}

crLogicOp::crLogicOp(Opcode opcode):
    m_opcode(opcode)
{
}

crLogicOp::~crLogicOp()
{
}

void crLogicOp::addAttributeParam(int i, const std::string &str)
{
	switch(i) 
	{
	case 0:
		if(str.compare("CLEAR") == 0)
			m_opcode = CLEAR;
		else if (str.compare("SET") == 0)
			m_opcode = SET;
		else if (str.compare("COPY") == 0)
			m_opcode = COPY;
		else if (str.compare("COPY_INVERTED") == 0)
			m_opcode = COPY_INVERTED;
		else if (str.compare("NOOP") == 0)
			m_opcode = NOOP;
		else if (str.compare("INVERT") == 0)
			m_opcode = INVERT;
		else if (str.compare("AND") == 0)
			m_opcode = AND;
		else if (str.compare("NAND") == 0)
			m_opcode = NAND;
		else if (str.compare("OR") == 0)
			m_opcode = OR;
		else if (str.compare("NOR") == 0)
			m_opcode = NOR;
		else if (str.compare("XOR") == 0)
			m_opcode = XOR;
		else if (str.compare("EQUIV") == 0)
			m_opcode = EQUIV;
		else if (str.compare("AND_REVERSE") == 0)
			m_opcode = AND_REVERSE;
		else if (str.compare("AND_INVERTED") == 0)
			m_opcode = AND_INVERTED;
		else if (str.compare("OR_REVERSE") == 0)
			m_opcode = OR_REVERSE;
		else if (str.compare("OR_INVERTED") == 0)
			m_opcode = OR_INVERTED;
		break;
	}

	return crStateAttribute::addAttributeParam(i-1,str);
}
#include <Driver/GLDrv/crLogicOpDrv.h>