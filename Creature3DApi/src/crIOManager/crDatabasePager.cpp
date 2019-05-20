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
//Modified by Wucaihua
#include <CRIOManager/crDatabasePager.h>
#include <CRIOManager/crReadFile.h>
#include <CRIOManager/crPreRemoveNodeVisitor.h>
#include <CRCore/crBrain.h>
#include <CRCore/crObject.h>
#include <CRCore/crTimer.h>
#include <CRCore/crTexture.h>
#include <CRCore/crNotify.h>
#include <CRCore/crGeometry.h>
#include <CRCore/crSearchNodeBYNameVisitor.h>
//#include <CRUtil/crRenderInitVisitor.h>
#include <CRPhysics/crPhysicsInitVisitor.h>
#include <CRPhysics/crVolumeNode.h>
#include <CRCore/thread/crScopedLock.h>

#include <stack>
#include <algorithm>

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <unistd.h>
#endif

using namespace CRIOManager;
using namespace CRCore;
using namespace CRPhysics;

crDatabasePager::crDatabasePager()
{
    //CRCore::notify(CRCore::INFO)<<"Constructing crDatabasePager()"<<std::endl;
    
	m_inited = false;
    m_startThreadCalled = false;

    m_done = false;
    m_acceptNewRequests = true;
    m_databasePagerThreadPaused = false;
    
    m_useFrameBlock = false;
    m_numFramesActive = 0;
    m_frameNumber = 0;
    m_frameBlock = new crBlock;
    m_databasePagerThreadBlock = new crBlock;

    m_threadPriorityDuringFrame = THREAD_PRIORITY_NOMINAL;
    m_threadPriorityOutwithFrame = THREAD_PRIORITY_NOMINAL;

#if __APPLE__
	// OSX really doesn't like compiling display lists, and performs poorly when they are used,
	// so apply this hack to make up for its short comings.
	m_drawablePolicy = USE_VERTEX_ARRAYS;
#else
	m_drawablePolicy = USE_VERTEX_BUFFER_OBJECTS;//DO_NOT_MODIFY_DRAWABLE_SETTINGS;
#endif   

	//const char* str = getenv("CRE_DATABASE_PAGER_GEOMETRY");
	//if (!str) str = getenv("CRE_DATABASE_PAGER_DRAWABLE");
	//if (str)
	//{
	//	if (strcmp(str,"DoNotModify")==0)
	//	{
	//		m_drawablePolicy = DO_NOT_MODIFY_DRAWABLE_SETTINGS;
	//	}
	//	else if (strcmp(str,"DisplayList")==0 || strcmp(str,"DL")==0)
	//	{
	//		m_drawablePolicy = USE_DISPLAY_LISTS;
	//	}
	//	else if (strcmp(str,"VBO")==0)
	//	{
	//		m_drawablePolicy = USE_VERTEX_BUFFER_OBJECTS;
	//	}
	//	else if (strcmp(str,"VertexArrays")==0 || strcmp(str,"VA")==0 )
	//	{
	//		m_drawablePolicy = USE_VERTEX_ARRAYS;
	//	} 
	//}
    m_drawablePolicy = DO_NOT_MODIFY_DRAWABLE_SETTINGS;

    m_changeAutoUnRef = true;
    m_valueAutoUnRef = true;
    m_changeAnisotropy = true;
    m_valueAnisotropy = 1.0f;

#if 1
    m_deleteRemovedSubgraphsInDatabaseThread = true;
#else
    m_deleteRemovedSubgraphsInDatabaseThread = false;
#endif
    
    m_expiryDelay = 10.0;

	//const char* ptr=0;
	m_doPreCompile = false;
	//if( (ptr = getenv("CRE_DO_PRE_COMPILE")) != 0)
	//{
	//	m_doPreCompile = strcmp(ptr,"yes")==0 || strcmp(ptr,"YES")==0 ||
	//		strcmp(ptr,"on")==0 || strcmp(ptr,"ON")==0;
	//}

    m_targetFrameRate = 100.0;
    m_minimumTimeAvailableForGLCompileAndDeletePerFrame = 0.001; // 1ms.
    m_maximumNumOfObjectsToCompilePerFrame = 4;//16;//4
	//if( (ptr = getenv("CRE_MINIMUM_COMPILE_TIME_PER_FRAME")) != 0)
	//{
	//	m_minimumTimeAvailableForGLCompileAndDeletePerFrame = atof(ptr);
	//}

	//if( (ptr = getenv("CRE_MAXIMUM_OBJECTS_TO_COMPILE_PER_FRAME")) != 0)
	//{
	//	m_maximumNumOfObjectsToCompilePerFrame = atoi(ptr);
	//}
    // make sure a crSharedStateManager exists.
    //CRIOManager::crRegistry::instance()->getOrCreateSharedStateManager();
    //
    //if (CRIOManager::crRegistry::instance()->getSharedStateManager())
    //    CRIOManager::crRegistry::instance()->setUseObjectCacheHint(true);

	//m_initVisitor = new CRRender::crRenderInitVisitor(CRRender::crRenderInitVisitor::NONE);
    m_optimizer_decorator = new crGroup;
}

void crDatabasePager::setPhysicsInitVisitor(CRCore::crNodeVisitor* pv)
{
	m_physicsInitVisitor = pv; 
}

void crDatabasePager::setRenderInitVisitor(CRCore::crNodeVisitor* pv)
{
    m_initVisitor = pv;
}
CRCore::crNodeVisitor* crDatabasePager::getRenderInitVisitor()
{
	return m_initVisitor.get();
}

crDatabasePager::~crDatabasePager()
{
	//CRCore::notify(CRCore::ALWAYS)<<"crDatabasePager::~crDatabasePager()"<< std::endl;
    cancel();
}

void crDatabasePager::registerOptimizerData(CRCore::crNode* subgraph)
{
	m_optimizer_decorator->addChild(subgraph);
}

int crDatabasePager::cancel()
{
    int result = 0;
    if( isRunning() )
    {
        m_done = true;

        // cancel the thread..
        //result = Thread::cancel();
        //join();

        // release the frameBlock and m_databasePagerThreadBlock incase its holding up thread cancelation.
        m_frameBlock->release();
        m_databasePagerThreadBlock->release();

        // then wait for the the thread to stop running.
        while(isRunning())
        {
			CRCore::crThread::sleep(10);
        }
        
        m_startThreadCalled = false;
    }
    return result;
}

void crDatabasePager::clear()
{
    {
        CRCore::ScopedLock<CRCore::crMutex> lock(m_fileRequestListMutex);
        m_fileRequestList.clear();
    }

    {
        CRCore::ScopedLock<CRCore::crMutex> lock(m_dataToCompileListMutex);
        m_dataToCompileList.clear();
    }

    {
        CRCore::ScopedLock<CRCore::crMutex> lock(m_childrenToDeleteListMutex);
        m_childrenToDeleteList.clear();
    }
    
    {
        CRCore::ScopedLock<CRCore::crMutex> lock(m_dataToMergeListMutex);
        m_dataToMergeList.clear();
    }

	{
		CRCore::ScopedLock<CRCore::crMutex> lock(m_safeDeleteListMutex);
		m_safeDeleteList.clear();
	}
	{
		CRCore::ScopedLock<CRCore::crMutex> lock(m_loadedMapMutex);
		m_loadedMap.clear();
	}
    // no mutex??
    m_activePagedLODList.clear();
    m_inactivePagedLODList.clear();
    
	m_childRemovedMap.clear();
    // ??
    // m_activeGraphicsContexts
}

crDatabasePager::LoadedMap &crDatabasePager::getLoadedMap()
{
    return m_loadedMap;
}

void crDatabasePager::requestNodeFile(const std::string& fileName,CRCore::crGroup* group, float priority/*, const CRCore::crFrameStamp* framestamp*/)
{
    if (!m_acceptNewRequests) return;
   
    double timestamp = crFrameStamp::getInstance()->getReferenceTime();
    int frameNumber = crFrameStamp::getInstance()->getFrameNumber();
   
    // search to see if filename already exist in the file loaded list.
    bool foundEntry = false;
	{
		CRCore::crPagedLOD* plod;
        //CRCore::ScopedLock<CRCore::crMutex> lock1(m_childrenToDeleteListMutex);
		CRCore::ScopedLock<CRCore::crMutex> lock1(m_loadedMapMutex);
		for(LoadedMap::iterator litr = m_loadedMap.begin();
			litr != m_loadedMap.end() && !foundEntry;
			++litr)
		{
            if (litr->first.compare(fileName)==0)
			{
				foundEntry = true;
				group->addChild(litr->second.get());
				plod = dynamic_cast<CRCore::crPagedLOD*>(group);
				if (plod)
				{
					plod->setTimeStamp(plod->getChildIndex(litr->second.get()),timestamp);
					//CRCore::notify(CRCore::NOTICE)<<"crDatabasePager::requestNodeFile "<<fileName<<" referenceCount= "<<litr->second->referenceCount()<<std::endl;
				}
				break;
			}
		}
		CRCore::ScopedLock<CRCore::crMutex> lock2(m_dataToMergeListMutex);
		for(DatabaseRequestList::iterator litr = m_dataToMergeList.begin();
			litr != m_dataToMergeList.end() && !foundEntry;
			++litr)
		{
			if ((*litr)->m_fileName.compare(fileName)==0)
			{
				foundEntry = true;
				(*litr)->m_frameNumberLastRequest = frameNumber;
				(*litr)->m_timestampLastRequest = timestamp;
				(*litr)->m_priorityLastRequest = priority;
				++((*litr)->m_numOfRequests);
				(*litr)->m_groupsForAddingLoadedSubgraph.push_back(group);
				break;
			}
		}
	}
	if (!foundEntry)
    {
        CRCore::ScopedLock<CRCore::crMutex> lock(m_dataToCompileListMutex);
    
        for(DatabaseRequestList::iterator litr = m_dataToCompileList.begin();
            litr != m_dataToCompileList.end() && !foundEntry;
            ++litr)
        {
            if ((*litr)->m_fileName.compare(fileName)==0)
            {
                foundEntry = true;
                (*litr)->m_frameNumberLastRequest = frameNumber;
                (*litr)->m_timestampLastRequest = timestamp;
                (*litr)->m_priorityLastRequest = priority;
                ++((*litr)->m_numOfRequests);
				(*litr)->m_groupsForAddingLoadedSubgraph.push_back(group);
				break;
            }
        }        

    }
    if (!foundEntry)
    {
    
        CRCore::ScopedLock<CRCore::crMutex> lock(m_fileRequestListMutex);

        // search to see if entry already  in file request list.
        bool foundEntry = false;
        for(DatabaseRequestList::iterator ritr = m_fileRequestList.begin();
            ritr != m_fileRequestList.end() && !foundEntry;
            ++ritr)
        {
            if ((*ritr)->m_fileName.compare(fileName)==0)
            {
                foundEntry = true;
                (*ritr)->m_timestampLastRequest = timestamp;
                (*ritr)->m_priorityLastRequest = priority;
                (*ritr)->m_frameNumberLastRequest = frameNumber;
                ++((*ritr)->m_numOfRequests);
				(*ritr)->m_groupsForAddingLoadedSubgraph.push_back(group);
				break;
            }
        }        
 /*       if(!foundEntry)
		{
			foundEntry = m_fileLoadedList.find(fileName) != m_fileLoadedList.end();
		}*/
        if (!foundEntry)
        {
			//CRCore::notify(CRCore::ALWAYS)<<"In crDatabasePager::fileRquest("<<fileName<<")"<<std::endl;

            CRCore::ref_ptr<DatabaseRequest> databaseRequest = new DatabaseRequest;

            databaseRequest->m_fileName = fileName;
            databaseRequest->m_frameNumberFirstRequest = frameNumber;
            databaseRequest->m_timestampFirstRequest = timestamp;
            databaseRequest->m_priorityFirstRequest = priority;
            databaseRequest->m_frameNumberLastRequest = frameNumber;
            databaseRequest->m_timestampLastRequest = timestamp;
            databaseRequest->m_priorityLastRequest = priority;
            //databaseRequest->m_groupForAddingLoadedSubgraph = group;
			databaseRequest->m_groupsForAddingLoadedSubgraph.push_back(group);

            m_fileRequestList.push_back(databaseRequest);

            updateDatabasePagerThreadBlock();
        }
		//CRCore::notify(CRCore::ALWAYS)<<"In crDatabasePager:: find("<<fileName<<")"<<std::endl;
    }
    
    if (!isRunning())
    {
        CRCore::ScopedLock<CRCore::crMutex> lock(m_run_mutex);
        
        if (!m_startThreadCalled)
        {
            m_startThreadCalled = true;
            m_done = false;
            //CRCore::notify(CRCore::DEBUG_INFO)<<"crDatabasePager::startThread()"<<std::endl;
            setSchedulePriority(m_threadPriorityDuringFrame);
            startThread();
        }
                
    }
}

void crDatabasePager::signalBeginFrame(/*const CRCore::crFrameStamp* framestamp*/)
{

    //if (framestamp)
    //{
        //CRCore::notify(CRCore::INFO) << "signalBeginFrame "<<framestamp->getFrameNumber()<<">>>>>>>>>>>>>>>>"<<std::endl;
        m_frameNumber = crFrameStamp::getInstance()->getFrameNumber();
        
    //} //else CRCore::notify(CRCore::INFO) << "signalBeginFrame >>>>>>>>>>>>>>>>"<<std::endl;

    updateFrameBlock(1);

    if (m_numFramesActive>0 && m_threadPriorityDuringFrame!=getSchedulePriority())
        setSchedulePriority(m_threadPriorityDuringFrame);
}

void crDatabasePager::signalEndFrame()
{
    //CRCore::notify(CRCore::INFO) << "signalEndFrame <<<<<<<<<<<<<<<<<<<< "<<std::endl;
    updateFrameBlock(-1);

    if (m_numFramesActive<=0 && m_threadPriorityOutwithFrame!=getSchedulePriority())
        setSchedulePriority(m_threadPriorityOutwithFrame);

}

class crDatabasePager::FindCompileableGLObjectsVisitor : public CRCore::crNodeVisitor
{
public:
    FindCompileableGLObjectsVisitor(crDatabasePager::DataToCompile& dataToCompile, 
                               bool changeAutoUnRef, bool valueAutoUnRef,
                               bool changeAnisotropy, float valueAnisotropy,
							   crDatabasePager::DrawablePolicy drawablePolicy):
                        CRCore::crNodeVisitor(CRCore::crNodeVisitor::TRAVERSE_ALL_CHILDREN),
                        m_dataToCompile(dataToCompile),
                        m_changeAutoUnRef(changeAutoUnRef), m_valueAutoUnRef(valueAutoUnRef),
                        m_changeAnisotropy(changeAnisotropy), m_valueAnisotropy(valueAnisotropy),
						m_drawablePolicy(drawablePolicy)
    {
    }
    
    inline virtual void apply(CRCore::crNode& node)
    {
        apply(node.getStateSet());
		traverse(node);
    }

    inline virtual void apply(CRCore::crObject& geode)
    {
		apply(geode.getStateSet());
		for(unsigned int i=0;i<geode.getNumDrawables();++i)
		{
			apply(geode.getDrawable(i));
		}
    }
    
    inline void apply(CRCore::crStateSet* stateset)
    {
        if (stateset && stateset->getNeedComplie())
        {
            //for(unsigned int i=0;i<stateset->getTextureAttributeList().size();++i)
            //{
            //    CRCore::crTexture* texture = dynamic_cast<CRCore::crTexture*>(stateset->getTextureAttribute(i,CRCore::crStateAttribute::TEXTURE));
            //    if (texture)
            //    {
            //        if (m_changeAutoUnRef) texture->setUnRefImageDataAfterApply(m_valueAutoUnRef);
            //        if (m_changeAnisotropy) texture->setMaxAnisotropy(/*m_valueAnisotropy*/CRCore::crDisplaySettings::instance()->getMaxAnisotropy());
            //    }
            //}
            m_dataToCompile.first.insert(stateset);
        }
    }
    
    inline void apply(CRCore::crDrawable* drawable)
    {
        apply(drawable->getStateSet());
        
		//switch(m_drawablePolicy)
		//{
		//case crDatabasePager::DO_NOT_MODIFY_DRAWABLE_SETTINGS: 
		//	// do nothing, leave settings as they came in from loaded database.
		//	// CRCore::notify(CRCore::NOTICE)<<"DO_NOT_MODIFY_DRAWABLE_SETTINGS"<<std::endl;
		//	break;
		//case crDatabasePager::USE_DISPLAY_LISTS: 
		//	drawable->setUseDisplayList(true);
		//	drawable->setUseVertexBufferObjects(false);
		//	break;
		//case crDatabasePager::USE_VERTEX_BUFFER_OBJECTS:
		//	drawable->setUseDisplayList(false);
		//	drawable->setUseVertexBufferObjects(true);
		//	// CRCore::notify(CRCore::NOTICE)<<"USE_VERTEX_BUFFER_OBJECTS"<<std::endl;
		//	break;
		//case crDatabasePager::USE_VERTEX_ARRAYS:
		//	drawable->setUseDisplayList(false);
		//	drawable->setUseVertexBufferObjects(false);
		//	// CRCore::notify(CRCore::NOTICE)<<"USE_VERTEX_ARRAYS"<<std::endl;
		//	break;
		//}
        //不需要对drawable进行预编译
        //if (drawable->getUseDisplayList() || drawable->getUseVertexBufferObjects())
        //{
        //    //CRCore::notify(CRCore::INFO)<<"Found compilable drawable"<<std::endl;
        //    m_dataToCompile.second.push_back(drawable);
        //}
    }
    
    crDatabasePager::DataToCompile&   m_dataToCompile;
    bool                            m_changeAutoUnRef;
    bool                            m_valueAutoUnRef;
    bool                            m_changeAnisotropy;
    float                           m_valueAnisotropy;
	crDatabasePager::DrawablePolicy   m_drawablePolicy;
};


struct crDatabasePager::SortFileRequestFunctor
{
    bool operator() (const CRCore::ref_ptr<crDatabasePager::DatabaseRequest>& lhs,const CRCore::ref_ptr<crDatabasePager::DatabaseRequest>& rhs) const
    {
        if (lhs->m_timestampLastRequest>rhs->m_timestampLastRequest) return true;
        else if (lhs->m_timestampLastRequest<rhs->m_timestampLastRequest) return false;
        else return (lhs->m_priorityLastRequest>rhs->m_priorityLastRequest);
    }
};


void crDatabasePager::setDatabasePagerThreadPause(bool pause)
{
    m_databasePagerThreadPaused = pause;
    updateDatabasePagerThreadBlock();
}

void crDatabasePager::run()
{
	//try{
    CRCore::notify(CRCore::NOTICE)<<"crDatabasePager::run()"<<std::endl;

    // need to set the texture object manager to be able to reuse textures
    CRCore::crTexture::setMinimumNumberOfTextureObjectsToRetainInCache(0);//100
    
    // need to set the texture object manager to be able to reuse textures
    CRCore::crDrawable::setMinimumNumberOfDisplayListsToRetainInCache(0);//500

    bool firstTime = true;
    
    do
    {
        m_databasePagerThreadBlock->block();

        if (m_useFrameBlock)
        {
            m_frameBlock->block();
        }

        //
        // delete any children if required.
        //
    //    if (m_deleteRemovedSubgraphsInDatabaseThread)
    //    {
    //        //CRCore::ref_ptr<CRCore::crBase> obj = 0;
    //        //{
    //            CRCore::ScopedLock<CRCore::crMutex> lock(m_childrenToDeleteListMutex);
				////CRCore::ref_ptr<CRPhysics::crPhysicsReleaseVisitor> physicsRelease = new CRPhysics::crPhysicsReleaseVisitor;
    //            if (!m_childrenToDeleteList.empty())
    //            {
    //                //CRCore::notify(CRCore::NOTICE)<<"In crDatabasePager thread deleting "<<m_childrenToDeleteList.size()<<" objects"<<std::endl;
    //                //CRCore::Timer_t before = CRCore::Timer::instance()->tick();
    //                //obj = m_childrenToDeleteList.back();
    //                m_childrenToDeleteList.pop_back();
    //                //CRCore::notify(CRCore::NOTICE)<<"Done crDatabasePager thread deleted in "<<CRCore::Timer::instance()->delta_m(before,CRCore::Timer::instance()->tick())<<" ms"<<" objects"<<std::endl;
    //                //obj->releaseObjects();
				//	//(dynamic_cast<crNode *>(obj.get()))->accept(*physicsRelease);
				//	//obj->releaseObjects(0);
    //                updateDatabasePagerThreadBlock();
    //            }
    //        //}
    //    }
        //
        // load any subgraphs that are required.
        //
        CRCore::ref_ptr<DatabaseRequest> databaseRequest;
    
        // get the front of the file request list.
        {
            CRCore::ScopedLock<CRCore::crMutex> lock(m_fileRequestListMutex);
            if (!m_fileRequestList.empty())
            {
                std::sort(m_fileRequestList.begin(),m_fileRequestList.end(),SortFileRequestFunctor());
                databaseRequest = m_fileRequestList.front();
            }
        }
        if (databaseRequest.valid())
        {
            // check if databaseRequest is still relevant
            if (m_frameNumber-databaseRequest->m_frameNumberLastRequest<=2)
            {
                       
                // load the data, note safe to write to the databaseRequest since once 
                // it is created this thread is the only one to write to the m_loadedModel pointer.
                // CRCore::notify(CRCore::NOTICE)<<"In crDatabasePager thread readNodeFile("<<databaseRequest->m_fileName<<")"<<std::endl;
                //CRCore::Timer_t before = CRCore::Timer::instance()->tick();
                          
                bool serialize_readNodeFile = true;
				//CRCore::notify(CRCore::NOTICE)<<"NeedLoadPage: "<<databaseRequest->m_fileName<<std::endl;
				//CRCore::crVector4i _size;
				//_size[0] = CRCore::crBrain::getInstance()->getStateSetSet().size();
    //            _size[1] = CRCore::crBrain::getInstance()->getTexture2DMap().size();
    //            _size[2] = CRCore::crBrain::getInstance()->getTextureCubeMapMap().size();
				//_size[3] = CRCore::crBrain::getInstance()->getProgramMapMap().size();
				//CRCore::notify(CRCore::NOTICE)<<"_size: "<<_size<<std::endl;
                if (serialize_readNodeFile)
                {
                    // do *not* assume that we only have one crDatabasePager, or that reaNodeFile is thread safe...
                    static CRCore::crMutex s_serialize_readNodeFile_mutex;
                    CRCore::ScopedLock<CRCore::crMutex> lock(s_serialize_readNodeFile_mutex);
                    databaseRequest->m_loadedModel = CRIOManager::readNodeFile(databaseRequest->m_fileName);
                }
                else
                {
                    // assume that we only have one crDatabasePager, or that reaNodeFile is thread safe...
                    databaseRequest->m_loadedModel = CRIOManager::readNodeFile(databaseRequest->m_fileName);
                }
				//CRCore::notify(CRCore::NOTICE)<<databaseRequest->m_fileName<<" Loaded"<<std::endl;
				//_size;
				//_size[0] = CRCore::crBrain::getInstance()->getStateSetSet().size();
				//_size[1] = CRCore::crBrain::getInstance()->getTexture2DMap().size();
				//_size[2] = CRCore::crBrain::getInstance()->getTextureCubeMapMap().size();
				//_size[3] = CRCore::crBrain::getInstance()->getProgramMapMap().size();
				//CRCore::notify(CRCore::NOTICE)<<"_size: "<<_size<<std::endl;

				if(databaseRequest->m_loadedModel.valid())
				{
					if(m_initVisitor.valid())
					    databaseRequest->m_loadedModel->accept(*m_initVisitor);
					//if(m_physicsInitVisitor.valid())
					//	databaseRequest->m_loadedModel->accept(*m_physicsInitVisitor);
				}

                //CRCore::notify(CRCore::NOTICE)<<"     node read in "<<CRCore::Timer::instance()->delta_m(before,CRCore::Timer::instance()->tick())<<" ms"<<std::endl;
                
                bool loadedObjectsNeedToBeCompiled = false;

                if (m_doPreCompile && databaseRequest->m_loadedModel.valid() && !m_activeGraphicsContexts.empty())
                {
                    // force a compute of the loaded model's bounding volume, so that when the subgraph
                    // merged with the main scene graph and large computeBound() isn't incurred.
                    databaseRequest->m_loadedModel->getBound();


                    ActiveGraphicsContexts::iterator itr = m_activeGraphicsContexts.begin();

                    DataToCompile& dtc = databaseRequest->m_dataToCompileMap[*itr];
                    ++itr;                

                    // find all the compileable rendering objects
                    FindCompileableGLObjectsVisitor frov(dtc, 
                                                         m_changeAutoUnRef, m_valueAutoUnRef,
                                                         m_changeAnisotropy, m_valueAnisotropy,
														 m_drawablePolicy);

                    databaseRequest->m_loadedModel->accept(frov);

                    if (!dtc.first.empty() || !dtc.second.empty())
                    {
                        loadedObjectsNeedToBeCompiled = true;                

                        // copy the objects from the compile list to the other graphics context list.
                        for(;
                            itr != m_activeGraphicsContexts.end();
                            ++itr)
                        {
                            databaseRequest->m_dataToCompileMap[*itr] = dtc;
                        }
                    }
                }            

                // move the databaseRequest from the front of the fileRequest to the end of
                // dataLoad list.
                
                CRCore::ScopedLock<CRCore::crMutex> lock(m_fileRequestListMutex);

                if (databaseRequest->m_loadedModel.valid())
                {
                    if (loadedObjectsNeedToBeCompiled)
                    {
                        CRCore::ScopedLock<CRCore::crMutex> lock(m_dataToCompileListMutex);
                        m_dataToCompileList.push_back(databaseRequest);
                    }
                    else
                    {
                        CRCore::ScopedLock<CRCore::crMutex> lock(m_dataToMergeListMutex);
                        m_dataToMergeList.push_back(databaseRequest);
                    }

					//m_fileLoadedList.insert(databaseRequest->m_fileName);
                }        

                if (!m_fileRequestList.empty()) m_fileRequestList.erase(m_fileRequestList.begin());

                updateDatabasePagerThreadBlock();

            }
            else
            {
                CRCore::ScopedLock<CRCore::crMutex> lock(m_fileRequestListMutex);

                if (!m_fileRequestList.empty()) m_fileRequestList.erase(m_fileRequestList.begin());

                updateDatabasePagerThreadBlock();

            }
        }

        // go to sleep till our the next time our thread gets scheduled.

        if (firstTime)
        {
            // do a yield to get round a peculiar thread hang when testCancel() is called 
            // in certain cirumstances - of which there is no particular pattern.
			CRCore::crThread::sleep(10);
            firstTime = false;
        }
		//crBrain::getInstance()->dataThreadPaused(true);
		//crBrain::getInstance()->gameLoop();
		//crBrain::getInstance()->dataThreadPaused(false);

    } while (!testCancel() && !m_done);
#ifdef _DEBUG
	CRCore::notify(CRCore::ALWAYS)<<"crDatabasePager::run() end"<<std::endl;
#endif
//}
//catch (...) {
//	CRCore::notify(CRCore::NOTICE)<<"crDatabasePager run error"<<std::endl;
//}
}


bool crDatabasePager::requiresUpdateSceneGraph() const
{
    CRCore::ScopedLock<CRCore::crMutex> lock(m_dataToMergeListMutex);
    if (!m_dataToMergeList.empty()) return true;
   
	return false;
}
 

void crDatabasePager::addLoadedDataToSceneGraph(double timeStamp)
{
    // CRCore::Timer_t before = CRCore::Timer::instance()->tick();

    DatabaseRequestList localFileLoadedList;

    // get the dat for the m_dataToCompileList, leaving it empty via a std::vector<>.swap.
    {
        CRCore::ScopedLock<CRCore::crMutex> lock(m_dataToMergeListMutex);
		localFileLoadedList.swap(m_dataToMerge2List);
		m_dataToMerge2List.swap(m_dataToMergeList);
		//localFileLoadedList.swap(m_dataToMergeList);
    }
        
    // add the loaded data into the scene graph.
	//bool added = false;
	DatabaseRequest* databaseRequest = NULL;
	CRCore::crGroup* group = NULL;
	CRCore::crPagedLOD* plod = NULL;
    for(DatabaseRequestList::iterator itr=localFileLoadedList.begin();
        itr!=localFileLoadedList.end();
        ++itr)
    {
        databaseRequest = itr->get();

        // CRCore::notify(CRCore::NOTICE)<<"Merging "<<m_frameNumber-(*itr)->m_frameNumberLastRequest<<std::endl;

		ref_ptr<crGroup> dummyGroup;
		for( DatabaseRequest::GroupVector::iterator gitr = databaseRequest->m_groupsForAddingLoadedSubgraph.begin();
			 gitr != databaseRequest->m_groupsForAddingLoadedSubgraph.end();
			 ++gitr )
		{
            group = gitr->get();
			dummyGroup = dynamic_cast<crGroup *>(databaseRequest->m_loadedModel.get());
			if(dummyGroup.valid()&&dummyGroup->getName().compare("LoadNodeDummy")==0)
			{
				databaseRequest->m_loadedModel = dummyGroup->getChild(0);
			}
			group->addChild(databaseRequest->m_loadedModel.get());
			plod = dynamic_cast<CRCore::crPagedLOD*>(group);
			if (plod)
			{
				plod->setTimeStamp(plod->getChildIndex(databaseRequest->m_loadedModel.get()),timeStamp);
			}
		}
		//CRCore::crGroup* group = databaseRequest->m_groupForAddingLoadedSubgraph.get();
		//CRCore::crPagedLOD* plod = dynamic_cast<CRCore::crPagedLOD*>(group);
		//if (plod)
		//{
		//	plod->setTimeStamp(plod->getNumChildren(),timeStamp);
		//} 
		//group->addChild(databaseRequest->m_loadedModel.get());
		//m_fileLoadedList.erase(databaseRequest->m_fileName);

        //if (CRIOManager::crRegistry::instance()->getSharedStateManager()) 
        //    CRIOManager::crRegistry::instance()->getSharedStateManager()->share(databaseRequest->m_loadedModel.get());
        
        registerPagedLODs(databaseRequest->m_loadedModel.get());

		if(m_physicsInitVisitor.valid())
		{
			//CRCore::notify(CRCore::FATAL)<<"crDatabasePager::addLoadedDataToSceneGraph() m_physicsInitVisitor "<<group->getName()<<std::endl;
			databaseRequest->m_loadedModel/*group*/->accept(*m_physicsInitVisitor);
		}

		if(!databaseRequest->m_fileName.empty() && databaseRequest->m_fileName.find("tmp.cre") == std::string::npos )
		{
			//CRCore::crSearchNodeBYClassNameVisitor searchByClassNameVisitor;
			//searchByClassNameVisitor.setTraversalMode(crNodeVisitor::TRAVERSE_ALL_CHILDREN);
			//searchByClassNameVisitor.insertClassNameId("CreBodyNode");
			//databaseRequest->m_loadedModel->accept(searchByClassNameVisitor);
			//if(!searchByClassNameVisitor.getResult())
			//{
				m_loadedMapMutex.lock();
		        m_loadedMap[databaseRequest->m_fileName] = databaseRequest->m_loadedModel.get();
				m_loadedMapMutex.unlock();
			//}
		}

		//added = true;
		//CRCore::notify(CRCore::NOTICE)<<"merged subgraph"<<databaseRequest->m_fileName<<" after "<<databaseRequest->m_numOfRequests<<" requests. "<<databaseRequest->m_loadedModel->referenceCount()<<std::endl;
    }
 //   if(added)
	//{
	//	//if (CRIOManager::crRegistry::instance()->getSharedStateManager()) 
	//	//	CRIOManager::crRegistry::instance()->getSharedStateManager()->share(CRCore::crBrain::getInstance()->getCurrentActiveDB());
	//}

    // CRCore::notify(CRCore::NOTICE)<<"Done crDatabasePager::addLoadedDataToSceneGraph"<<CRCore::Timer::instance()->delta_m(before,CRCore::Timer::instance()->tick())<<" ms  objects"<<localFileLoadedList.size()<<std::endl;
}
void crDatabasePager::flushDeletedObjects(CRCore::crState *state)
{
	if(!m_safeDeleteList.empty())
	{
		CRCore::ScopedLock<CRCore::crMutex> lock(m_safeDeleteListMutex);
		//CRCore::ref_ptr<CRPhysics::crPhysicsReleaseVisitor> physicsRelease = CRPhysics::crPhysicsReleaseVisitor::getInstance();
		for( ObjectList::iterator itr = m_safeDeleteList.begin();
			itr != m_safeDeleteList.end();
			++itr )
		{
			//(dynamic_cast<crNode *>(itr->get()))->accept(*physicsRelease);
			(*itr)->releaseObjects(state);
		}
		m_safeDeleteList.clear();
	}
}
void crDatabasePager::removeExpiredSubgraphs(double currentFrameTime)
{
    //CRCore::notify(CRCore::NOTICE)<<"crDatabasePager::removeExpiredSubgraphs()"<<std::endl;

    // CRCore::Timer_t before = CRCore::Timer::instance()->tick();

    double expiryTime = currentFrameTime - m_expiryDelay;

    CRCore::NodeArray childrenRemoved;

    for(PagedLODList::iterator active_itr = m_activePagedLODList.begin();
        active_itr!=m_activePagedLODList.end();
        )
    {
        const CRCore::crPagedLOD* plod = active_itr->get();
        bool remove_plod = false;
        if (plod->referenceCount()<=1)
        {
            // prune PageLOD's that are no longer externally referenced
            childrenRemoved.push_back(const_cast<CRCore::crPagedLOD*>(plod));
            //CRCore::notify(CRCore::NOTICE)<<"m_activePagedLODList : pruning no longer externally referenced"<<std::endl;
            remove_plod = true;
        }
        else if (plod->getFrameNumberOfLastTraversal()<m_frameNumber-1)
        {                
            //CRCore::notify(CRCore::NOTICE)<<"m_activePagedLODList : moving PageLOD to inactive list"<<std::endl;
            m_inactivePagedLODList.insert(*active_itr);
            remove_plod = true;
        }
        
        if (remove_plod)
        {
            PagedLODList::iterator itr_to_erase = active_itr;
            ++active_itr;
            
            m_activePagedLODList.erase(itr_to_erase);            
        }
        else
        {
			//const_cast<CRCore::crPagedLOD*>(plod)->removeExpiredChildren(expiryTime,childrenRemoved);
            ++active_itr;
        }
    }
    
    unsigned int i = 0;
    // unsigned int numberOfPagedLODToTest = m_inactivePagedLODList.size();
    unsigned int targetNumOfInActivePagedLODs = 100;//100;
    unsigned int targetNumOfRemovedChildPagedLODs = 0;
    if (m_inactivePagedLODList.size()>targetNumOfInActivePagedLODs) targetNumOfRemovedChildPagedLODs = m_inactivePagedLODList.size()-targetNumOfInActivePagedLODs;

    if (targetNumOfRemovedChildPagedLODs>1) targetNumOfRemovedChildPagedLODs=1;
    

    // filter out singly referenced crPagedLOD and move reactivated crPagedLOD into the active list
	{
		CRCore::ScopedLock<CRCore::crMutex> lock(m_loadedMapMutex);
		for (PagedLODList::iterator inactive_itr = m_inactivePagedLODList.begin();
			inactive_itr != m_inactivePagedLODList.end();
			)
		{
			const CRCore::crPagedLOD* plod = inactive_itr->get();
			bool remove_plod = false;
			if (plod->referenceCount() <= 1)
			{
				// prune PageLOD's that are no longer externally referenced
				childrenRemoved.push_back(const_cast<CRCore::crPagedLOD*>(plod));
				//CRCore::notify(CRCore::NOTICE)<<"m_activePagedLODList : pruning no longer externally referenced"<<std::endl;
				remove_plod = true;
			}
			else if (plod->getFrameNumberOfLastTraversal() >= m_frameNumber - 1)
			{
				//CRCore::notify(CRCore::NOTICE)<<"m_inactivePagedLODList : moving PageLOD to active list"<<std::endl;
				// found a reactivated pagedLOD's need to put it back in the active list.                
				m_activePagedLODList.insert(*inactive_itr);
				remove_plod = true;
			}

			if (remove_plod)
			{
				PagedLODList::iterator itr_to_erase = inactive_itr;
				++inactive_itr;

				m_inactivePagedLODList.erase(itr_to_erase);
			}
			else
			{
				//            if (i<numberOfPagedLODToTest)
				if (childrenRemoved.size() < targetNumOfRemovedChildPagedLODs)
				{
					// check for removing expired children.
					if (const_cast<CRCore::crPagedLOD*>(plod)->removeExpiredChildren(expiryTime, childrenRemoved, m_loadedMap))
					{
						//CRCore::notify(CRCore::NOTICE)<<"Some children removed from PLod "<<plod->getName()<<std::endl;
					}
					else
					{
						//CRCore::notify(CRCore::NOTICE)<<"no children removed from PLod"<<std::endl;
					}
				}
				//const_cast<CRCore::crPagedLOD*>(plod)->removeExpiredChildren(expiryTime,childrenRemoved);
				++inactive_itr;
				++i;
			}
		}
	}
    //CRCore::notify(CRCore::NOTICE)<<"m_activePagedLODList.size()="<<m_activePagedLODList.size()<<"\t_inactivePagedLODList.size()="<<m_inactivePagedLODList.size()<<std::endl;


    //CRCore::notify(CRCore::NOTICE)<<"   number stale "<<numberStale<<"  number active "<<m_pagedLODList.size()-numberStale<<std::endl;

    //double t = CRCore::Timer::instance()->delta_m(before,CRCore::Timer::instance()->tick());
    //CRCore::notify(CRCore::NOTICE)<<"   time 1 "<<t<<" ms "<<m_pagedLODList.size()<<" pagedLOD's"<<std::endl;
    //CRCore::notify(CRCore::NOTICE)<<"   average time = "<<t/(double)m_pagedLODList.size()<<" ms/per crPagedLOD"<<std::endl;
	crNode *node;
	if (!m_childrenToDeleteList.empty())
	{
		CRCore::ScopedLock<CRCore::crMutex> lock(m_safeDeleteListMutex);
		CRCore::ScopedLock<CRCore::crMutex> lock2(m_childrenToDeleteListMutex);
		CRPhysics::crPhysicsReleaseVisitor* physicsRelease = CRPhysics::crPhysicsReleaseVisitor::getInstance();
		for (ObjectList::iterator oitr = m_childrenToDeleteList.begin();
			oitr!=m_childrenToDeleteList.end();
			++oitr)
		{
            node = dynamic_cast<crNode *>(oitr->get());
			if(node)
			{
                node->accept(*physicsRelease);
			}
		}
        m_safeDeleteList.swap(m_childrenToDeleteList);
	    //CRCore::notify(CRCore::NOTICE)<<"  m_childrenToDeleteList = "<<m_childrenToDeleteList.size()<<std::endl;
	}
    if (!childrenRemoved.empty())
    { 
        // pass the objects across to the database pager delete list
        CRCore::ScopedLock<CRCore::crMutex> lock(m_childrenToDeleteListMutex);
        for (CRCore::NodeArray::iterator critr = childrenRemoved.begin();
                critr!=childrenRemoved.end();
                ++critr)
        {
            m_childrenToDeleteList.push_back(critr->get());
			//这里的节点在调用crPreRemoveNodeVisitor之前已经从场景里移除了，所以没有必要调用，如果节点内存在粒子系统，则会出现安全隐患
			//node = dynamic_cast<crNode *>(critr->get());
			//if(node)
			//{//
			//	node->accept(*preRemove);
			//}
        }
        //updateDatabasePagerThreadBlock();

        childrenRemoved.clear();
    }

    // CRCore::notify(CRCore::NOTICE)<<"   time 2 "<<CRCore::Timer::instance()->delta_m(before,CRCore::Timer::instance()->tick())<<" ms "<<std::endl;

    
    //if (CRIOManager::crRegistry::instance()->getSharedStateManager()) 
    //    CRIOManager::crRegistry::instance()->getSharedStateManager()->prune();

    // update the crRegistry object cache.
    CRIOManager::crRegistry::instance()->updateTimeStampOfObjectsInCacheWithExtenalReferences(currentFrameTime);
    CRIOManager::crRegistry::instance()->removeExpiredObjectsInCache(expiryTime);
	
	if(!m_childRemovedMap.empty())
	{
		CRCore::ScopedLock<CRCore::crMutex> lock(m_childremovedmap_mutex);
		CRCore::ScopedLock<CRCore::crMutex> lock2(m_childrenToDeleteListMutex);
		ref_ptr<crNode> childRemove;
		ref_ptr<crGroup> parent;
		//crVolumeNode *volumeNode;
		crPreRemoveNodeVisitor *preRemove = crPreRemoveNodeVisitor::getInstance();
		for( ChildRemovedMap::iterator itr = m_childRemovedMap.begin();
			itr != m_childRemovedMap.end();
			++itr )
		{
			childRemove = const_cast<crNode *>(itr->first.get());
			childRemove->accept(*preRemove);
			parent = const_cast<crGroup *>(itr->second.get());
			parent->removeChild(childRemove.get());
			//LoadedMap中的模型，保留在内存中
			//if(childRemove->getNumParents()==0)
			//    m_childrenToDeleteList.push_back(childRemove.get());
		}
		//updateDatabasePagerThreadBlock();
		m_childRemovedMap.clear();
	}
    // CRCore::notify(CRCore::NOTICE)<<"Done crDatabasePager::removeExpiredSubgraphs() "<<CRCore::Timer::instance()->delta_m(before,CRCore::Timer::instance()->tick())<<" ms "<<std::endl;
}


class crDatabasePager::FindPagedLODsVisitor : public CRCore::crNodeVisitor
{
public:
    FindPagedLODsVisitor(crDatabasePager::PagedLODList& pagedLODList):
        CRCore::crNodeVisitor(CRCore::crNodeVisitor::TRAVERSE_ALL_CHILDREN),
        m_pagedLODList(pagedLODList)
    {
    }

    virtual void apply(CRCore::crPagedLOD& plod)
    {
        m_pagedLODList.insert(&plod);
		//CRCore::notify(CRCore::WARN)<<"crDatabasePager::FindPagedLODsVisitor() "<<plod.getFileName(0)<<std::endl;
        traverse(plod);
    }
    
    crDatabasePager::PagedLODList& m_pagedLODList;
};

void crDatabasePager::registerPagedLODs(CRCore::crNode* subgraph)
{
    FindPagedLODsVisitor fplv(m_activePagedLODList);
    if (subgraph) subgraph->accept(fplv);
}

bool crDatabasePager::requiresCompile() const
{
    CRCore::ScopedLock<CRCore::crMutex> lock(m_dataToCompileListMutex);
    return !m_dataToCompileList.empty();
}

void crDatabasePager::setCompileForContextID(unsigned int contextID, bool on)
{
    if (on)
    {
        m_activeGraphicsContexts.insert(contextID);
    }
    else
    {
        m_activeGraphicsContexts.erase(contextID);
    }
}

//void crDatabasePager::setPhysicsInitVisitor(CRCore::crNodeVisitor* pv)
//{ 
//	m_physicsInitVisitors.insert(pv); 
//}

bool crDatabasePager::getCompileForContextID(unsigned int contextID)
{
    return m_activeGraphicsContexts.count(contextID)!=0;
}


void crDatabasePager::compile(CRCore::crState& state, double& availableTime)
{
	//try{
    //CRCore::notify(CRCore::NOTICE)<<"crDatabasePager::compile "<<m_frameNumber<<std::endl;
    if (availableTime>0.0)
    {

        const CRCore::Timer& timer = *CRCore::Timer::instance();
        CRCore::Timer_t start_tick = timer.tick();
        double elapsedTime = 0.0;
        double estimatedTextureDuration = 0.0001;
        double estimatedDrawableDuration = 0.0001;

        CRCore::ref_ptr<DatabaseRequest> databaseRequest;

        // get the first compileable entry.
        //{
        //    CRCore::ScopedLock<CRCore::crMutex> lock(m_dataToCompileListMutex);
        //    if (!m_dataToCompileList.empty())
        //    {
        //        std::sort(m_dataToCompileList.begin(),m_dataToCompileList.end(),SortFileRequestFunctor());

        //        // prune all the old entries.
        //        DatabaseRequestList::iterator litr;
        //        int i=0;
        //        for(litr = m_dataToCompileList.begin();
        //            (litr != m_dataToCompileList.end()) && (m_frameNumber-(*litr)->m_frameNumberLastRequest)<=1;
        //            ++litr,i++)
        //        {
        //            //CRCore::notify(CRCore::NOTICE)<<"Compile "<<m_frameNumber-(*litr)->m_frameNumberLastRequest<<std::endl;
        //        }
        //        if (litr != m_dataToCompileList.end())
        //        {
        //            if (m_deleteRemovedSubgraphsInDatabaseThread)
        //            {
        //                CRCore::ScopedLock<CRCore::crMutex> lock(m_childrenToDeleteListMutex);

        //                for(DatabaseRequestList::iterator ditr=litr;
        //                    ditr!=m_dataToCompileList.end();
        //                    ++ditr)
        //                {
        //                    m_childrenToDeleteList.push_back((*ditr)->m_loadedModel.get());
        //                }
        //            }
        //            //CRCore::notify(CRCore::NOTICE)<<"Pruning "<<m_dataToCompileList.size()-i<<std::endl;
        //            m_dataToCompileList.erase(litr,m_dataToCompileList.end());
        //        }

        //        // advance to the next entry to compile if one is available.
        //        databaseRequest = m_dataToCompileList.empty() ? 0 : m_dataToCompileList.front();
        //    
        //    }
        //};

        databaseRequest = m_dataToCompileList.empty() ? 0 : m_dataToCompileList.front();

        unsigned int numObjectsCompiled = 0;
        //unsigned int maxNumObjectsToCompile = 4;//4

        // while there are valid databaseRequest's in the to compile list and there is
        // sufficient time left compile each databaseRequest's stateset and drawables.
		double startCompileTime;
		//GLint p;
        while (databaseRequest.valid() && elapsedTime<availableTime && numObjectsCompiled<m_maximumNumOfObjectsToCompilePerFrame) 
        {
            DataToCompileMap& dcm = databaseRequest->m_dataToCompileMap;
            DataToCompile& dtc = dcm[state.getContextID()];
            if (!dtc.first.empty() && (elapsedTime+estimatedTextureDuration)<availableTime && numObjectsCompiled<m_maximumNumOfObjectsToCompilePerFrame)
            {

                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_PRIORITY, 1.0);

                // we have crStateSet's to compile
                StateSetList& sslist = dtc.first;
                //CRCore::notify(CRCore::INFO)<<"Compiling statesets"<<std::endl;
                StateSetList::iterator itr=sslist.begin();
                for(;
                    itr!=sslist.end() && (elapsedTime+estimatedTextureDuration)<availableTime && numObjectsCompiled<m_maximumNumOfObjectsToCompilePerFrame;
                    ++itr)
                {
                    //CRCore::notify(CRCore::INFO)<<"    Compiling stateset "<<(*itr).get()<<std::endl;

                    startCompileTime = timer.delta_s(start_tick,timer.tick());

                    (*itr)->compile(state);

                    //glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_RESIDENT, &p);

                    elapsedTime = timer.delta_s(start_tick,timer.tick());


                    // estimate the duration of the compile based on current compile duration.
                    estimatedTextureDuration = (elapsedTime-startCompileTime);

                    ++numObjectsCompiled;
                }
                // remove the compiled stateset from the list.
                sslist.erase(sslist.begin(),itr);
            }

            if (!dtc.second.empty() && (elapsedTime+estimatedDrawableDuration)<availableTime && numObjectsCompiled<m_maximumNumOfObjectsToCompilePerFrame)
            {
                // we have crDrawable's to compile
                //CRCore::notify(CRCore::INFO)<<"Compiling drawables"<<std::endl;
                DrawableList& dwlist = dtc.second;
                DrawableList::iterator itr=dwlist.begin();
                for(;
                    itr!=dwlist.end() && (elapsedTime+estimatedDrawableDuration)<availableTime && numObjectsCompiled<m_maximumNumOfObjectsToCompilePerFrame;
                    ++itr)
                {
                    //CRCore::notify(CRCore::INFO)<<"    Compiling drawable "<<(*itr).get()<<std::endl;
                    double startCompileTime = timer.delta_s(start_tick,timer.tick());
                    (*itr)->compile(state);
                    elapsedTime = timer.delta_s(start_tick,timer.tick());

                    // estimate the duration of the compile based on current compile duration.
                    estimatedDrawableDuration = (elapsedTime-startCompileTime);

                    ++numObjectsCompiled;

                }
                // remove the compiled drawables from the list.
                dwlist.erase(dwlist.begin(),itr);
            }

            //CRCore::notify(CRCore::INFO)<<"Checking if compiled"<<std::endl;

            // now check the to compile entries for all active graphics contexts
            // to make sure that all have been compiled.
            bool allCompiled = true;
            for(DataToCompileMap::iterator itr=dcm.begin();
                itr!=dcm.end() && allCompiled;
                ++itr)
            {
                if (!(itr->second.first.empty())) allCompiled=false;
                if (!(itr->second.second.empty())) allCompiled=false;
            }


            if (allCompiled)
            {
                // we've compile all of the current databaseRequest so we can now pop it off the
                // to compile list and place it on the merge list.
                CRCore::ScopedLock<CRCore::crMutex> lock(m_dataToCompileListMutex);

                if(!databaseRequest->m_groupsForAddingLoadedSubgraph.empty())
                {
                    CRCore::ScopedLock<CRCore::crMutex> lock(m_dataToMergeListMutex);
                    m_dataToMergeList.push_back(databaseRequest);
                }

                if (!m_dataToCompileList.empty()) m_dataToCompileList.erase(m_dataToCompileList.begin());

                if (!m_dataToCompileList.empty())
                {
                    std::sort(m_dataToCompileList.begin(),m_dataToCompileList.end(),SortFileRequestFunctor());
                    databaseRequest = m_dataToCompileList.front();
                }
                else databaseRequest = 0;

            }
            else 
            {
                //CRCore::notify(CRCore::INFO)<<"Not all compiled"<<std::endl;
                databaseRequest = 0;
            }

            elapsedTime = timer.delta_s(start_tick,timer.tick());
        }

        availableTime -= elapsedTime;

        //CRCore::notify(CRCore::NOTICE)<<"elapsedTime="<<elapsedTime<<"\ttime remaining ="<<availableTime<<"\tnumObjectsCompiled = "<<numObjectsCompiled<<std::endl;
        //CRCore::notify(CRCore::NOTICE)<<"estimatedTextureDuration="<<estimatedTextureDuration;
        //CRCore::notify(CRCore::NOTICE)<<"\testimatedDrawableDuration="<<estimatedDrawableDuration<<std::endl;
    }
    else
    {
        availableTime = 0.0f;
    }
	//CRCore::notify(CRCore::NOTICE)<<"crDatabasePager::compile end "<<m_frameNumber<<std::endl;
//}
//catch (...) {
//	int i = 0;
//}
}

void crDatabasePager::requestCompile(CRCore::crNode* loadedNode)
{
    if(!loadedNode) return;

	CRCore::ref_ptr<DatabaseRequest> databaseRequest = new DatabaseRequest;

	databaseRequest->m_loadedModel = loadedNode;

	if(m_initVisitor.valid())
		loadedNode->accept(*m_initVisitor);

	//if(m_physicsInitVisitor.valid())
	//{
	//	//CRCore::notify(CRCore::FATAL)<<"crDatabasePager::addLoadedDataToSceneGraph() m_physicsInitVisitor "<<group->getName()<<std::endl;
	//	loadedNode->accept(*m_physicsInitVisitor);
	//}

	bool loadedObjectsNeedToBeCompiled = false;

	if (m_doPreCompile && !m_activeGraphicsContexts.empty())
	{
		// force a compute of the loaded model's bounding volume, so that when the subgraph
		// merged with the main scene graph and large computeBound() isn't incurred.
		loadedNode->getBound();

		ActiveGraphicsContexts::iterator itr = m_activeGraphicsContexts.begin();

		DataToCompile& dtc = databaseRequest->m_dataToCompileMap[*itr];
		++itr;                

		//// find all the compileable rendering objects
		FindCompileableGLObjectsVisitor frov(dtc, 
			m_changeAutoUnRef, m_valueAutoUnRef,
			m_changeAnisotropy, m_valueAnisotropy,
			m_drawablePolicy);

		loadedNode->accept(frov);

		if (!dtc.first.empty() || !dtc.second.empty())
		{
			loadedObjectsNeedToBeCompiled = true;                

			// copy the objects from the compile list to the other graphics context list.
			for(;
				itr != m_activeGraphicsContexts.end();
				++itr)
			{
				databaseRequest->m_dataToCompileMap[*itr] = dtc;
			}
		}
	}            

	if (loadedObjectsNeedToBeCompiled)
	{
		CRCore::ScopedLock<CRCore::crMutex> lock(m_dataToCompileListMutex);
		m_dataToCompileList.push_back(databaseRequest);
	}
	else
	{
		CRCore::ScopedLock<CRCore::crMutex> lock(m_dataToMergeListMutex);
		m_dataToMergeList.push_back(databaseRequest);
	}
}

void crDatabasePager::requestAddNode(CRCore::crGroup* parent,CRCore::crNode* loadedNode,bool needCompile)
{
	if(!parent || !loadedNode) return;

	CRCore::ref_ptr<DatabaseRequest> databaseRequest = new DatabaseRequest;

	databaseRequest->m_groupsForAddingLoadedSubgraph.push_back(parent);
	databaseRequest->m_loadedModel = loadedNode;

	if(m_initVisitor.valid())
		loadedNode->accept(*m_initVisitor);

	bool loadedObjectsNeedToBeCompiled = false;

	if (needCompile && m_doPreCompile && !m_activeGraphicsContexts.empty())
	{
		// force a compute of the loaded model's bounding volume, so that when the subgraph
		// merged with the main scene graph and large computeBound() isn't incurred.
		loadedNode->getBound();

		ActiveGraphicsContexts::iterator itr = m_activeGraphicsContexts.begin();

		DataToCompile& dtc = databaseRequest->m_dataToCompileMap[*itr];
		++itr;                

		//// find all the compileable rendering objects
		FindCompileableGLObjectsVisitor frov(dtc, 
			m_changeAutoUnRef, m_valueAutoUnRef,
			m_changeAnisotropy, m_valueAnisotropy,
			m_drawablePolicy);

		loadedNode->accept(frov);

		if (!dtc.first.empty() || !dtc.second.empty())
		{
			loadedObjectsNeedToBeCompiled = true;                

			// copy the objects from the compile list to the other graphics context list.
			for(;
				itr != m_activeGraphicsContexts.end();
				++itr)
			{
				databaseRequest->m_dataToCompileMap[*itr] = dtc;
			}
		}

		//registerPagedLODs(loadedNode);

		//if(m_physicsInitVisitor.valid())
		//{
		//	loadedNode->accept(*m_physicsInitVisitor);
		//}
	}            

	if (loadedObjectsNeedToBeCompiled)
	{
		CRCore::ScopedLock<CRCore::crMutex> lock(m_dataToCompileListMutex);
		m_dataToCompileList.push_back(databaseRequest);
	}
	else
	{
		CRCore::ScopedLock<CRCore::crMutex> lock(m_dataToMergeListMutex);
		m_dataToMergeList.push_back(databaseRequest);
	}
}

void crDatabasePager::requestRemoveNode(CRCore::crGroup* parent,CRCore::crNode* loadedNode)
{
	if(!parent || !loadedNode) return;

	CRCore::ScopedLock<CRCore::crMutex> lock(m_childremovedmap_mutex);
	m_childRemovedMap.insert(std::make_pair(loadedNode,parent));
}