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
#ifndef CRCORE_CRTRAVERSESTRINGNODE
#define CRCORE_CRTRAVERSESTRINGNODE 1
#include <CRCore/crGroup.h>
//#include <CRCore/crLightSource.h>

namespace CRCore {

class CR_EXPORT crTraverseStringNode : public crGroup
{
    public :
		crTraverseStringNode();

        crTraverseStringNode(const crTraverseStringNode&,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY);

        META_Node(CRCore, crTraverseStringNode);
        
        inline virtual void traverse(crNodeVisitor& nv)
		{
			if(nv.getVisitorType() == crNodeVisitor::CULL_VISITOR/* || nv.getVisitorType() == crNodeVisitor::UPDATE_VISITOR*/)
			{
				//const GL2Extensions* extensions = GL2Extensions::Get(0,false);
				//if(extensions && extensions->isGlslSupported())
				//{
					std::string str = nv.getTraverseString();
					nv.setTraverseString(getName());
					crGroup::traverse(nv);
					nv.setTraverseString(str);
				//}
				//else
				//{
				//	crGroup::traverse(nv);
				//}
			}
			else
			{
				crGroup::traverse(nv);
			}
		}
        
		//void setGlobalLightSource(crLightSource *ls, crLightSource *ls1);
		//inline void getGlobalLightSource(ref_ptr<crLightSource> &ls, ref_ptr<crLightSource> &ls1){ ls = m_ls; ls1 = m_ls1; }
    protected :
        virtual ~crTraverseStringNode() {}
        //ref_ptr<crLightSource> m_ls;
        //ref_ptr<crLightSource> m_ls1;
};

}

#endif
