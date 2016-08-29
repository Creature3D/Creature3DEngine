
#ifndef CRCORE_CRDOUBLESTATENODE
#define CRCORE_CRDOUBLESTATENODE 1
#include <CRCore/crGroup.h>
#include <CRCore/crStateSet.h>

namespace CRCore {

class CR_EXPORT crDoubleStateNode : public crGroup
{
    public :
		crDoubleStateNode(){}

        crDoubleStateNode(const crDoubleStateNode&,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY);

        META_Node(CRCore, crDoubleStateNode);
                
		void setSunLightShaderStateSet(crStateSet *stateset);
		void setSpotLightShaderStateSet(crStateSet *stateset);
		crStateSet *getSunLightShaderStateSet();
		crStateSet *getSpotLightShaderStateSet();

		virtual void releaseObjects(crState* = 0) const;

    protected :
        virtual ~crDoubleStateNode() {}
		
        ref_ptr<crStateSet> m_sunLightShaderStateSet;
		ref_ptr<crStateSet> m_spotLightShaderStateSet;
};

}

#endif
