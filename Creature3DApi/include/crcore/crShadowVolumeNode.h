
#ifndef CRCORE_CRWATERREFLECTNODE
#define CRCORE_CRWATERREFLECTNODE 1
#include <CRCore/crMatrixTransform.h>

#include <map>

namespace CRCore {

class CR_EXPORT crShadowVolumeNode : public crMatrixTransform
{
    public :
		crShadowVolumeNode();

        crShadowVolumeNode(const crShadowVolumeNode&,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY);

        META_Node(CRCore, crShadowVolumeNode);
        
        virtual void traverse(crNodeVisitor& nv);

		crGroup *create(/*crPlane &plane*/float z);

    protected :
        virtual ~crShadowVolumeNode() {}
		
};

}

#endif
