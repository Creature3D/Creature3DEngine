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
#include <CRCore/crTimer.h>
#include <CRIOManager/crSharedStateManager.h>

using namespace CRIOManager;

crSharedStateManager::crSharedStateManager():
    CRCore::crNodeVisitor(CRCore::crNodeVisitor::TRAVERSE_ALL_CHILDREN) 
{
    m_shareMode = SHARE_ALL;
    //m_shareMode = SHARE_STATESETS;
    m_mutex=0;
}

//----------------------------------------------------------------
// crSharedStateManager::prune
//----------------------------------------------------------------
void crSharedStateManager::prune()
{
    StateSetSet::iterator sitr/*, sitr1*/;
    for(sitr=m_sharedStateSetList.begin(); sitr!=m_sharedStateSetList.end();/* sitr=sitr1*/)
    {
        //sitr1=sitr; ++sitr1;
        if((*sitr)->referenceCount()<=1) sitr = m_sharedStateSetList.erase(sitr);
		else ++sitr;
    }

    TextureSet::iterator titr/*, titr1*/;
    for(titr=m_sharedTextureList.begin(); titr!=m_sharedTextureList.end();/* titr=titr1*/)
    {
        //titr1=titr; ++titr1;
        if((*titr)->referenceCount()<=1) titr = m_sharedTextureList.erase(titr);
		else ++titr;
    }

} 


//----------------------------------------------------------------
// crSharedStateManager::share
//----------------------------------------------------------------
void crSharedStateManager::share(CRCore::crNode *node, CRCore::crMutex *mt)
{
//    const CRCore::Timer& timer = *CRCore::Timer::instance();
//    CRCore::Timer_t start_tick = timer.tick();
    
    m_mutex = mt;
    apply(*node);
    tmpSharedTextureList.clear();
    tmpSharedStateSetList.clear();
    m_mutex = 0;
}


//----------------------------------------------------------------
// crSharedStateManager::apply
//----------------------------------------------------------------
void crSharedStateManager::apply(CRCore::crNode& node)
{
    CRCore::crStateSet* ss = node.getStateSet();
    if(ss) process(ss, &node);
    traverse(node);
}
void crSharedStateManager::apply(CRCore::crObject& geode)
{
    CRCore::crStateSet* ss = geode.getStateSet();
    if(ss) process(ss, &geode);
    for(unsigned int i=0;i<geode.getNumDrawables();++i)
    {
        CRCore::crDrawable* drawable = geode.getDrawable(i);
        if(drawable)
        {
            ss = drawable->getStateSet();
            if(ss) process(ss, drawable);
        }
    }
}
 

//----------------------------------------------------------------
// crSharedStateManager::find
//----------------------------------------------------------------
CRCore::crStateSet *crSharedStateManager::find(CRCore::crStateSet *ss)
{
    for(StateSetSet::iterator itr=m_sharedStateSetList.begin(); 
        itr!=m_sharedStateSetList.end(); 
        ++itr)
    {
        if(ss->compare(*(itr->get()),true)==0) 
            return (CRCore::crStateSet *)itr->get();
    }
    return NULL;
}
CRCore::crStateAttribute *crSharedStateManager::find(CRCore::crStateAttribute *sa)
{
    for(TextureSet::iterator itr=m_sharedTextureList.begin(); 
    itr!=m_sharedTextureList.end(); 
    ++itr)
    {
        if(sa->compare(*(itr->get()))==0) 
            return (CRCore::crStateAttribute *)itr->get();
    }
    return NULL;
}
   

//----------------------------------------------------------------
// crSharedStateManager::setStateSet
//----------------------------------------------------------------
void crSharedStateManager::setStateSet(CRCore::crStateSet* ss, CRCore::crBase* object)
{
    CRCore::crDrawable* drawable = dynamic_cast<CRCore::crDrawable*>(object);
    if (drawable)
    {
        drawable->setStateSet(ss);
    }
    else
    {
        CRCore::crNode* node = dynamic_cast<CRCore::crNode*>(object);
        if (node)
        {
            node->setStateSet(ss);
        }
    }
}


//----------------------------------------------------------------
// crSharedStateManager::shareTextures
//----------------------------------------------------------------
void crSharedStateManager::shareTextures(CRCore::crStateSet* ss)
{
    CRCore::crStateSet::TextureAttributeList& texAttributes = ss->getTextureAttributeList();
    for(unsigned int unit=0;unit<texAttributes.size();++unit)
    {
        CRCore::crStateAttribute *texture = ss->getTextureAttribute(unit, CRCore::crStateAttribute::TEXTURE);

        // Valid crTexture to be shared
        if(texture && texture->getDataVariance()==CRCore::crBase::STATIC)
        {
            TextureTextureSharePairMap::iterator titr = tmpSharedTextureList.find(texture);
            if(titr==tmpSharedTextureList.end())
            {
                // crTexture is not in tmp list: 
                // First time it appears in this file, search crTexture in sharedAttributeList
                CRCore::crStateAttribute *textureFromSharedList = find(texture);
                if(textureFromSharedList)
                {
                    // crTexture is in sharedAttributeList: 
                    // Share now. Required to be shared all next times
                    if(m_mutex) m_mutex->lock();
                    ss->setTextureAttributeAndModes(unit, textureFromSharedList, CRCore::crStateAttribute::ON);
                    if(m_mutex) m_mutex->unlock();
                    tmpSharedTextureList[texture] = TextureSharePair(textureFromSharedList, true);
                }
                else
                {
                    // crTexture is not in _sharedAttributeList: 
                    // Add to _sharedAttributeList. Not needed to be shared all next times.
                    m_sharedTextureList.insert(texture); 
                    tmpSharedTextureList[texture] = TextureSharePair(texture, false);            
                }
            }
            else if(titr->second.second)
            {
                // crTexture is in tmpSharedAttributeList and share flag is on:
                // It should be shared
                if(m_mutex) m_mutex->lock();
                ss->setTextureAttributeAndModes(unit, titr->second.first, CRCore::crStateAttribute::ON);
                if(m_mutex) m_mutex->unlock();
            }
        }
    }
}


//----------------------------------------------------------------
// crSharedStateManager::process
//----------------------------------------------------------------
void crSharedStateManager::process(CRCore::crStateSet* ss, CRCore::crBase* parent)
{
    if(m_shareMode & SHARE_STATESETS)
    {
        // Valid crStateSet to be shared
        if(ss->getDataVariance()==CRCore::crBase::STATIC)
        {
            StateSetStateSetSharePairMap::iterator sitr = tmpSharedStateSetList.find(ss);
            if(sitr==tmpSharedStateSetList.end())
            {
                // crStateSet is not in tmp list: 
                // First time it appears in this file, search crStateSet in sharedObjectList
                CRCore::crStateSet *ssFromSharedList = find(ss);
                if(ssFromSharedList)
                {
                    // crStateSet is in sharedStateSetList: 
                    // Share now. Required to be shared all next times
                    if(m_mutex) m_mutex->lock();
                    setStateSet(ssFromSharedList, parent);
                    if(m_mutex) m_mutex->unlock();
                    tmpSharedStateSetList[ss] = StateSetSharePair(ssFromSharedList, true);
                }
                else
                {
                    // crStateSet is not in sharedStateSetList: 
                    // Add to sharedStateSetList. Not needed to be shared all next times.
                    m_sharedStateSetList.insert(ss); 
                    tmpSharedStateSetList[ss] = StateSetSharePair(ss, false);            

                    // Only in this case sharing textures is also required
                    if(m_shareMode & SHARE_TEXTURES)
                    {
                        shareTextures(ss);
                    }
                }
            }
            else if(sitr->second.second)
            {
                // crStateSet is in tmpSharedStateSetList and share flag is on:
                // It should be shared
                if(m_mutex) m_mutex->lock();
                setStateSet(sitr->second.first, parent);
                if(m_mutex) m_mutex->unlock();
            }
        }
    }

    else if(m_shareMode & SHARE_TEXTURES)
    {
        shareTextures(ss);
    }
}
