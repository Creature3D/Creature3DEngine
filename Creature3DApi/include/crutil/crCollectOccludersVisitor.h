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
//Modified by Îâ²Æ»ª
#ifndef CRUTIL_CRCOLLECTOCCLUDERSVISITOR_H
#define CRUTIL_CRCOLLECTOCCLUDERSVISITOR_H 1

#include <CRUtil/crExport.h>
#include <CRCore/crNodeVisitor.h>
#include <CRCore/crCullStack.h>

#include <set>

namespace CRUtil {

class CRUTIL_EXPORT crCollectOccludersVisitor : public CRCore::crNodeVisitor, public CRCore::crCullStack
{
    public:

		typedef std::set<CRCore::crShadowVolumeOccluder> ShadowVolumeOccluderSet;

        crCollectOccludersVisitor();
        virtual ~crCollectOccludersVisitor();

        virtual crCollectOccludersVisitor* cloneType() const { return new crCollectOccludersVisitor(); }

        virtual void reset();

        virtual float getDistanceToEyePoint(const CRCore::crVector3& pos, bool withLODScale) const;
        virtual float getDistanceFromEyePoint(const CRCore::crVector3& pos, bool withLODScale) const;

        virtual void apply(CRCore::crNode&);
		//virtual void apply(CRCore::crObject&);
		//virtual void apply(CRCore::crDrawable&);

        virtual void apply(CRCore::crTransform& node);
        virtual void apply(CRCore::crProjection& node);

        virtual void apply(CRCore::crSwitch& node);
        virtual void apply(CRCore::crLod& node);
        virtual void apply(CRCore::crOccluderNode& node);

        /** Set the minimum shadow occluder volume that an active occluder must have.
          * volume is units relative the clip space volume where 1.0 is the whole clip space.*/
        void setMinimumShadowOccluderVolume(float vol) { m_minShadowOccluderVolume = vol; }
        float getMinimumShadowOccluderVolume() const { return m_minShadowOccluderVolume; }

        /** Set the maximum number of occluders to have active for culling purposes.*/
        void setMaximumNumberOfActiveOccluders(unsigned int num) { m_maxNumberOfActiveOccluders = num; }
        unsigned int getMaximumNumberOfActiveOccluders() const { return m_maxNumberOfActiveOccluders; }
//		void setMaximumNumberOfCollectOccluders(unsigned int num) { m_maxNumberOfCollectOccluders = num; }

        void setCreateDrawablesOnOccludeNodes(bool flag) { m_createDrawables=flag; }
        bool getCreateDrawablesOnOccludeNodes() const { return m_createDrawables; }

        void setCollectedOcculderList(const ShadowVolumeOccluderSet& svol) { m_occluderSet = svol; }
        ShadowVolumeOccluderSet& getCollectedOccluderSet() { return m_occluderSet; }
        const ShadowVolumeOccluderSet& getCollectedOccluderSet() const { return m_occluderSet; }

        /** remove occluded occluders for the collected occluders list,
          * and then discard of all but MaximumNumberOfActiveOccluders of occluders,
          * discarding the occluders with the lowests shadow occluder volume .*/
        void removeOccludedOccluders();

		//void creatOccluder(const crVector3& v1,const crVector3& v2,const crVector3& v3,const crVector3& v4);

    protected:

        /** prevent unwanted copy construction.*/
        //crCollectOccludersVisitor(const crCollectOccludersVisitor&):CRCore::crNodeVisitor(),CRCore::crCullStack() {}

        /** prevent unwanted copy operator.*/
        crCollectOccludersVisitor& operator = (const crCollectOccludersVisitor&) { return *this; }
        
        inline void handle_cull_callbacks_and_traverse(CRCore::crNode& node)
        {
            /*CRCore::crNodeCallback* callback = node.getCullCallback();
            if (callback) (*callback)(&node,this);
            else*/ if (node.getNumChildrenWithOccluderNodes()>0) traverse(node);
        }

        inline void handle_cull_callbacks_and_accept(CRCore::crNode& node,CRCore::crNode* acceptNode)
        {
            /*CRCore::crNodeCallback* callback = node.getCullCallback();
            if (callback) (*callback)(&node,this);
            else*/ if (node.getNumChildrenWithOccluderNodes()>0) acceptNode->accept(*this);
        }

		//float                       m_minCalculateValue;
        float                       m_minShadowOccluderVolume;
        unsigned int                m_maxNumberOfActiveOccluders;
        //unsigned int                m_maxNumberOfCollectOccluders;
        bool                        m_createDrawables;
        ShadowVolumeOccluderSet     m_occluderSet;

};

}

#endif

