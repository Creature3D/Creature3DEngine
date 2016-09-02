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
#include <CRCore/crTexGenNode.h>

using namespace CRCore;

crTexGenNode::crTexGenNode():
    m_referenceFrame(RELATIVE_RF)
{
	setCullingActive(false);
	m_textureUnit = 0;
    m_value = crStateAttribute::ON;
    m_stateset = new crStateSet;
    m_texgen = new crTexGen;
}

crTexGenNode::crTexGenNode(const crTexGenNode& cn, const crCopyOp& copyop):
    crGroup(cn,copyop),
    m_textureUnit(cn.m_textureUnit),
    m_value(cn.m_value),
    m_texgen(static_cast<crTexGen*>(copyop(cn.m_texgen.get()))),
	m_referenceFrame(cn.m_referenceFrame)
{
}

crTexGenNode::crTexGenNode(crTexGen *texgen)
{
    m_textureUnit = 0;
    m_value = crStateAttribute::ON;
    m_stateset = new crStateSet;
    m_texgen = texgen;
}

crTexGenNode::~crTexGenNode()
{
}

void crTexGenNode::setTexGen(crTexGen* texgen)
{
    m_texgen = texgen;
}

void crTexGenNode::setReferenceFrame(ReferenceFrame rf)
{
	m_referenceFrame = rf;
}