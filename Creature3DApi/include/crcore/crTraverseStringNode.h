/**********************************************************************
*
*	文件:	   crTraverseStringNode.h
*
*	描述:	   
*
*	作者:	   吴财华
*					
*
*	Copyright 2005 吴财华
**********************************************************************/
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
