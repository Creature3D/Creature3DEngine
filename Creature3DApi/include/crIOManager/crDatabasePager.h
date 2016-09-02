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
#ifndef CRIOMANAGER_DATABASEPAGER
#define CRIOMANAGER_DATABASEPAGER 1

#include <CRCore/crNodeVisitor.h>
#include <CRCore/crGroup.h>
#include <CRCore/crPagedLOD.h>
#include <CRCore/crDrawable.h>
#include <CRCore/crGraphicsThread.h>

#include <CRCore/thread/crThread.h>
#include <CRCore/thread/crMutex.h>
#include <CRCore/thread/crScopedLock.h>
#include <CRCore/thread/crCondition.h>

#include <CRIOManager/crSharedStateManager.h>
#include <CRIOManager/crExport.h>

#include <map>
#include <list>

namespace CRIOManager {

/** Database paging class which manages the loading of files in a background thread, 
  * and syncronizing of loaded models with the main scene graph.*/
class CRIOMANAGER_EXPORT crDatabasePager : public CRCore::crNodeVisitor::DatabaseRequestHandler, public CRCore::crThread
{
    public :

        typedef CRCore::crThread::ThreadPriority ThreadPriority;

        crDatabasePager();

        /** Add a request to load a node file to end the the database request list.*/
        virtual void requestNodeFile(const std::string& fileName,CRCore::crGroup* group, float priority/*, const CRCore::crFrameStamp* framestamp*/);

		void requestAddNode(CRCore::crGroup* parent,CRCore::crNode* loadedNode,bool needCompile);
		void requestRemoveNode(CRCore::crGroup* parent,CRCore::crNode* loadedNode);

		void requestCompile(CRCore::crNode* loadedNode);

        /** Run does the database paging.*/        
        virtual void run();
        
        /** Cancel the database pager thread.*/        
        virtual int cancel();
        
        /** Clear all internally cached structures.*/
        virtual void clear();
        
        /** Set whether the database pager thread should be paused or not.*/
        void setDatabasePagerThreadPause(bool pause);
        
        /** Get whether the database pager thread should is paused or not.*/
        bool getDatabasePagerThreadPause() const { return m_databasePagerThreadPaused; }
        
        /** Set whether new database request calls are accepted or ignored.*/
        void setAcceptNewDatabaseRequests(bool acceptNewRequests) { m_acceptNewRequests = acceptNewRequests; }
        
        /** Get whether new database request calls are accepted or ignored.*/
        bool getAcceptNewDatabaseRequests() const { return m_acceptNewRequests; }
        
        /** Set the use of the frame block which, if enabled, blocks the crDatabasePager 
          * from executing which the current frame is being drawn.  
          * When a single processor machine is being used it can be useful to block on
          * frame to help prevent the database paging thread from slowing the cull and draw
          * traversals which in turn can cause frame drops.*/
        void setUseFrameBlock(bool useFrameBlock) { m_useFrameBlock = useFrameBlock; }
        
        /** Get the whether UseFrameBlock is on or off.*/
        bool getUseFrameBlock() const { return m_useFrameBlock; }
        
		CRCore::crBlock* getFrameBlock() { return m_frameBlock.get(); }

        /** Set the priority of the database pager thread during the frame (i.e. while cull and draw are running.)*/
        void setThreadPriorityDuringFrame(ThreadPriority duringFrame) { m_threadPriorityDuringFrame = duringFrame; }

        /** Get the priority of the database pager thread during the frame*/
        ThreadPriority getThreadPriorityDuringFrame() const { return m_threadPriorityDuringFrame; }
        
        /** Set the priority of the database pager thread when the frame is not being exectuted (i.e. before or after cull and draw have run.)*/
        void setThreadPriorityOutwithFrame(ThreadPriority outwithFrame)  { m_threadPriorityOutwithFrame = outwithFrame; }

        /** Get the priority of the database pager thread when the frame is not being exectuted.*/
        ThreadPriority getThreadPriorityOutwithFrame() const { return m_threadPriorityOutwithFrame; }
        
        
        /** Get the number of frames that are currently active.*/
        int getNumFramesActive() const { return m_numFramesActive; }

        /** Signal the database thread that the update, cull and draw has begun for a new frame.
          * Note, this is called by the application so that the database pager can go to sleep while the CPU is busy on the main rendering threads. */
        virtual void signalBeginFrame(/*const CRCore::crFrameStamp* framestamp*/);
        
        /** Signal the database thread that the update, cull and draw dispatch has completed.
          * Note, this is called by the application so that the database pager can go to wake back up now the main rendering threads are iddle waiting for the next frame.*/
        virtual void signalEndFrame();
        

        /** Find all crPagedLOD nodes in a subgraph and register them with 
          * the crDatabasePager so it can keep track of expired nodes.
          * note, should be only be called from the update thread. */
        virtual void registerPagedLODs(CRCore::crNode* subgraph);

		/** Set whether the database pager should pre compile OpenGL objects before allowing
		* them to be merged into the scene graph.
		* Pre compilation helps reduce the chances of frame drops, but also slows the
		* speed at which tiles are merged as they have to be compiled first.*/
		void setDoPreCompile(bool flag) { m_doPreCompile = flag; }

		/** Get whether the database pager should pre compile OpenGL objects before allowing
		* them to be merged into the scene graph.*/
		bool getDoPreCompile() const { return m_doPreCompile; }

        /** Set the target frame rate that the crDatabasePager should assume.
          * Typically one would set this to the value refresh rate of your display system i.e. 60Hz.
          * Default value is 100.
          * Usage notes.  The TargetFrameRate and the MinimumTimeAvailableForGLCompileAndDeletePerFrame
          * parameters are not directly used by crDatabasePager, but are should be used as a guide for how
          * long to set aside per frame for compiling and deleting OpenGL objects - ie. the value to use 
          * when calling crDatabasePager::compileGLObjectgs(state,availableTime,). The longer amount of
          * time to set aside  cthe faster databases will be paged in but with increased chance of frame drops,
          * the lower the amount of time the set aside the slower databases will paged it but with better
          * chance of avoid any frame drops.  The default values are chosen to achieve the later when running
          * on a modern mid to high end  PC. 
          * The way to compute the amount of available time use a scheme such as :
          *    availableTime = maximum(1.0/targetFrameRate - timeTakenDuringUpdateCullAndDraw, minimumTimeAvailableForGLCompileAndDeletePerFrame). 
          */
        void setTargetFrameRate(double tfr) { m_targetFrameRate = tfr; }

        /** Get the target frame rate that the crDatabasePager should assume.*/
        double getTargetFrameRate() const { return m_targetFrameRate; }
        
        /** Set the minimum amount of time (in seconds) that should be made available for compiling and delete OpenGL objects per frame.
          * Default value is 0.001 (1 millisecond). 
          * For usage see notes in setTargetFrameRate.*/
        void setMinimumTimeAvailableForGLCompileAndDeletePerFrame(double ta) { m_minimumTimeAvailableForGLCompileAndDeletePerFrame = ta; }

        /** Get the minimum amount of time that should be made available for compiling and delete OpenGL objects per frame.
          * For usage see notes in setTargetFrameRate.*/
        double getMinimumTimeAvailableForGLCompileAndDeletePerFrame() const { return m_minimumTimeAvailableForGLCompileAndDeletePerFrame; }

        /** Set the maximum number of OpenGL objects that the page should attempt to compile per frame.
          * Note, Lower values reduces chances of a frame drop but lower the rate that database will be paged in at.
          * Default value is 8. */
        void setMaximumNumOfObjectsToCompilePerFrame(unsigned int num) { m_maximumNumOfObjectsToCompilePerFrame = num; }

        /** Get the maximum number of OpenGL objects that the page should attempt to compile per frame.*/
        unsigned int getMaximumNumOfObjectsToCompilePerFrame() const { return m_maximumNumOfObjectsToCompilePerFrame; }


        /** Set the amount of time that a subgraph will be kept without being visited in the cull traversal
          * before being removed.*/
        void setExpiryDelay(double expiryDelay) { m_expiryDelay = expiryDelay; }
        
        /** Get the amount of time that a subgraph will be kept without being visited in the cull traversal
          * before being removed.*/
        double getExpiryDelay() const { return m_expiryDelay; }

        /** set whether the removed subgraphs should be deleted in the database thread or not.*/
        void setDeleteRemovedSubgraphsInDatabaseThread(bool flag) { m_deleteRemovedSubgraphsInDatabaseThread = flag; }
        
        /** get whether the removed subgraphs should be deleted in the database thread or not.*/
        bool getDeleteRemovedSubgraphsInDatabaseThread() const { return m_deleteRemovedSubgraphsInDatabaseThread; }

		enum DrawablePolicy
		{
			DO_NOT_MODIFY_DRAWABLE_SETTINGS,
			USE_DISPLAY_LISTS,
			USE_VERTEX_BUFFER_OBJECTS,
			USE_VERTEX_ARRAYS
		};

		/** Set how loaded drawables should be handled w.r.t their display list/vertex buffer object/vertex array settings.*/
		void setDrawablePolicy(DrawablePolicy policy) { m_drawablePolicy = policy; }

		/** Get how loaded drawables should be handled w.r.t their display list/vertex buffer object/vertex array settings.*/
		DrawablePolicy getDrawablePolicy() const { return m_drawablePolicy; }

        /** set whether newly loaded textures should have their UnrefImageDataAfterApply set to a specified value.*/
        void setUnrefImageDataAfterApplyPolicy(bool changeAutoUnRef, bool valueAutoUnRef) { m_changeAutoUnRef = changeAutoUnRef; m_valueAutoUnRef = valueAutoUnRef; }

        /** get whether newly loaded textures should have their UnrefImageDataAfterApply set to a specified value.*/
        void getUnrefImageDataAfterApplyPolicy(bool& changeAutoUnRef, bool& valueAutoUnRef) const { changeAutoUnRef = m_changeAutoUnRef; valueAutoUnRef = m_valueAutoUnRef; }


        /** set whether newly loaded textures should have their MaxAnisotopy set to a specified value.*/
        void setMaxAnisotropyPolicy(bool changeAnisotropy, float valueAnisotropy) { m_changeAnisotropy = changeAnisotropy; m_valueAnisotropy = valueAnisotropy; }

        /** set whether newly loaded textures should have their MaxAnisotopy set to a specified value.*/
        void getMaxAnisotropyPolicy(bool& changeAnisotropy, float& valueAnisotropy) const { changeAnisotropy = m_changeAnisotropy; valueAnisotropy = m_valueAnisotropy; }


        /** Return true if there are pending updates to the scene graph that require a call to updateSceneGraph(double). */
        bool requiresUpdateSceneGraph() const;
        
        /** Merge the changes to the scene graph by calling calling removeExpiredSubgraphs then addLoadedDataToSceneGraph.
          * Note, must only be called from single thread update phase. */
		virtual void updateSceneGraph(double currentFrameTime)
        {
			addLoadedDataToSceneGraph(currentFrameTime);
            removeExpiredSubgraphs(currentFrameTime);
        }
        void flushDeletedObjects(CRCore::crState *state);//Draw thread 调用
        /** Turn the compilation of rendering objects for specfied graphics context on (true) or off(false). */
        void setCompileForContextID(unsigned int contextID, bool on);
        
        /** Get whether the compilation of rendering objects for specfied graphics context on (true) or off(false). */
        bool getCompileForContextID(unsigned int contextID);

        /** Return true if there are pending compile operations that are required.
          * If requiresCompile() return true the application should call compile() .*/
        bool requiresCompile() const;

        /** Compile the rendering objects (display lists,texture objects, VBO's) on loaded subgraph.
          * note, should only be called from the draw thread.
          * Note, must only be called from a valid graphics context. */
        virtual void compile(CRCore::crState& state,double& availableTime);

        typedef std::set< CRCore::ref_ptr<CRCore::crPagedLOD> >   PagedLODList;
        typedef std::set< CRCore::ref_ptr<CRCore::crStateSet> >    StateSetList;
        typedef std::vector< CRCore::ref_ptr<CRCore::crDrawable> > DrawableList;
        typedef std::pair<StateSetList,DrawableList>       DataToCompile;
        typedef std::map< unsigned int, DataToCompile >    DataToCompileMap; 
        typedef std::set<unsigned int>                     ActiveGraphicsContexts;

        void registerOptimizerData(CRCore::crNode* subgraph);

		void setPhysicsInitVisitor(CRCore::crNodeVisitor* pv);
		void setRenderInitVisitor(CRCore::crNodeVisitor* pv);
		CRCore::crNodeVisitor* getRenderInitVisitor();
		typedef std::map< std::string, CRCore::ref_ptr<CRCore::crNode> > LoadedMap;
        LoadedMap &getLoadedMap();
		void inited(){ m_inited = true; }
		inline bool getInited(){ return m_inited; }
    protected:

        virtual ~crDatabasePager();

		/** Iterate through the active crPagedLOD nodes children removing 
		* children which havn't been visited since specified expiryTime.
		* note, should be only be called from the update thread. */
		void removeExpiredSubgraphs(double currentFrameTime);

		/** Add the loaded data to the scene graph.*/
		void addLoadedDataToSceneGraph(double currentFrameTime);

        friend struct DatabaseRequest;
        
        struct DatabaseRequest : public CRCore::Referenced
        {
            DatabaseRequest():
                m_numOfRequests(0),
				m_frameNumberFirstRequest(0),
                m_timestampFirstRequest(0.0f),
                m_priorityFirstRequest(0.0f),
                m_frameNumberLastRequest(0),
                m_timestampLastRequest(0),
                m_priorityLastRequest(0)
            {}
            
            std::string                 m_fileName;
            int                         m_frameNumberFirstRequest;
            double                      m_timestampFirstRequest;
            float                       m_priorityFirstRequest;
            int                         m_frameNumberLastRequest;
            double                      m_timestampLastRequest;
            float                       m_priorityLastRequest;
            unsigned int                m_numOfRequests;
			typedef std::vector< CRCore::ref_ptr<CRCore::crGroup> > GroupVector;
			GroupVector                 m_groupsForAddingLoadedSubgraph;
            //CRCore::ref_ptr<CRCore::crGroup>    m_groupForAddingLoadedSubgraph;
            CRCore::ref_ptr<CRCore::crNode>     m_loadedModel;
            DataToCompileMap            m_dataToCompileMap;
            
        };
        
        typedef std::vector< CRCore::ref_ptr<DatabaseRequest> > DatabaseRequestList;
        typedef std::vector< CRCore::ref_ptr<CRCore::crBase> > ObjectList;
		typedef std::multimap< CRCore::ref_ptr<CRCore::crNode>, CRCore::ref_ptr<CRCore::crGroup> > ChildRemovedMap;
		ChildRemovedMap m_childRemovedMap;
		CRCore::crMutex              m_childremovedmap_mutex;

		LoadedMap m_loadedMap;

        //// forward declare inner helper classes
        class FindCompileableGLObjectsVisitor;
        friend class FindCompileableGLObjectsVisitor;
        
        class FindPagedLODsVisitor;
        friend class FindPagedLODsVisitor;

        struct SortFileRequestFunctor;
        friend struct SortFileRequestFunctor;

        
        CRCore::crMutex              m_run_mutex;
        bool                            m_startThreadCalled;

        
		CRCore::ref_ptr<CRCore::crBlock>             m_databasePagerThreadBlock;

        inline void updateDatabasePagerThreadBlock()
        {
            m_databasePagerThreadBlock->set(
                (!m_fileRequestList.empty() || !m_childrenToDeleteList.empty()) && !m_databasePagerThreadPaused);
        }
        
        inline void updateFrameBlock(int delta)
        {
            CRCore::ScopedLock<CRCore::crMutex> lock(m_numFramesActiveMutex);
            m_numFramesActive += delta;
            m_frameBlock->set(m_numFramesActive==0);
        }

        bool                            m_done;
        bool                            m_acceptNewRequests;
        bool                            m_databasePagerThreadPaused;
    
        bool                            m_useFrameBlock;
        int                             m_numFramesActive;
        mutable CRCore::crMutex      m_numFramesActiveMutex;
		CRCore::ref_ptr<CRCore::crBlock>             m_frameBlock;
        int                             m_frameNumber;

        ThreadPriority                  m_threadPriorityDuringFrame;
        ThreadPriority                  m_threadPriorityOutwithFrame;

        DatabaseRequestList             m_fileRequestList;
        mutable CRCore::crMutex      m_fileRequestListMutex;

		//std::set<std::string>           m_fileLoadedList;

        DatabaseRequestList             m_dataToCompileList;
        mutable CRCore::crMutex      m_dataToCompileListMutex;

		DrawablePolicy                  m_drawablePolicy;

        bool                            m_changeAutoUnRef;
        bool                            m_valueAutoUnRef;
        bool                            m_changeAnisotropy;
        float                           m_valueAnisotropy;

        bool                            m_deleteRemovedSubgraphsInDatabaseThread;
        ObjectList                      m_childrenToDeleteList;
		ObjectList                      m_safeDeleteList;
        mutable CRCore::crMutex      m_childrenToDeleteListMutex;
		mutable CRCore::crMutex      m_safeDeleteListMutex;

        DatabaseRequestList             m_dataToMergeList;
        mutable CRCore::crMutex      m_dataToMergeListMutex;
        DatabaseRequestList             m_dataToMerge2List;
        
        PagedLODList                    m_activePagedLODList;
        PagedLODList                    m_inactivePagedLODList;
        
        double                          m_expiryDelay;

        ActiveGraphicsContexts          m_activeGraphicsContexts;
        
		bool                            m_doPreCompile;
        double                          m_targetFrameRate;
        double                          m_minimumTimeAvailableForGLCompileAndDeletePerFrame;
        unsigned int                    m_maximumNumOfObjectsToCompilePerFrame;
		CRCore::ref_ptr<CRCore::crNodeVisitor> m_initVisitor;
		CRCore::ref_ptr<CRCore::crNodeVisitor> m_physicsInitVisitor;
		bool                            m_inited;
		//typedef std::set< CRCore::ref_ptr<CRCore::crNodeVisitor> > NodeVisitorList;
		//NodeVisitorList                 m_physicsInitVisitors;

		CRCore::ref_ptr<CRCore::crGroup>  m_optimizer_decorator;

};

}

#endif
