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

#ifndef CRIOMANAGER_SHAREDSTATEMANAGER
#define CRIOMANAGER_SHAREDSTATEMANAGER 1
 

#include <CRCore/crNodeVisitor.h>
#include <CRCore/crObject.h>

#include <CRIOManager/crExport.h>

#include <CRCore/thread/crMutex.h>

#include <set>


namespace CRIOManager {

    class CRIOMANAGER_EXPORT crSharedStateManager : public CRCore::crNodeVisitor
    {
    public: 

        enum ShareMode
        {
            SHARE_NONE      = 0x00,
            SHARE_TEXTURES  = 0x01,
            SHARE_STATESETS = 0x02,
            SHARE_ALL       = SHARE_TEXTURES |
                              SHARE_STATESETS
        };

        crSharedStateManager();

        void setShareMode(unsigned int mode) { m_shareMode = mode; }
        unsigned int getShareMode() { return m_shareMode; }

        // Call right after each unload and before crRegistry cache prune.
        void prune();

        // Call right after each load
        void share(CRCore::crNode *node, CRCore::crMutex *mt=0);

        void apply(CRCore::crNode& node);
        void apply(CRCore::crObject& geode);
        
    protected:

        void process(CRCore::crStateSet* ss, CRCore::crBase* parent);
        CRCore::crStateAttribute *find(CRCore::crStateAttribute *sa);
        CRCore::crStateSet *find(CRCore::crStateSet *ss);
        void setStateSet(CRCore::crStateSet* ss, CRCore::crBase* object);
        void shareTextures(CRCore::crStateSet* ss);

        // Lists of shared objects
        typedef std::set< CRCore::ref_ptr<CRCore::crStateAttribute> > TextureSet;
        TextureSet m_sharedTextureList;
        typedef std::set< CRCore::ref_ptr<CRCore::crStateSet> > StateSetSet;
        StateSetSet m_sharedStateSetList;

        // Temporary lists just to avoid unnecesary find calls
        typedef std::pair<CRCore::crStateAttribute*, bool> TextureSharePair;
        typedef std::map<CRCore::crStateAttribute*, TextureSharePair> TextureTextureSharePairMap;
        TextureTextureSharePairMap tmpSharedTextureList;
        typedef std::pair<CRCore::crStateSet*, bool> StateSetSharePair;
        typedef std::map<CRCore::crStateSet*, StateSetSharePair> StateSetStateSetSharePairMap;
        StateSetStateSetSharePairMap tmpSharedStateSetList;

        // Share connection m_mutex 
        unsigned int m_shareMode;

        CRCore::crMutex *m_mutex;
    };

}

#endif
