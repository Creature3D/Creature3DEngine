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
#ifndef CRUTIL_CRRENDERBIN_H
#define CRUTIL_CRRENDERBIN_H 1

#include <CRUtil/crExport.h>
#include <CRUtil/crRenderGraph.h>

#include <map>
#include <vector>
#include <string>

namespace CRUtil {

class crRenderStage;
//class crStatistics;
/**
 * crRenderBin base class.
 */
class CRUTIL_EXPORT crRenderBin : public CRCore::crBase
{
    public:
    
        typedef std::vector< CRCore::ref_ptr<crRenderLeaf> >                    RenderLeafList; 
        typedef std::vector< CRCore::ref_ptr<crRenderGraph> >                   RenderGraphList;
		typedef std::map< int, CRCore::ref_ptr<crRenderBin>, std::less<int> >    RenderBinList; 

		typedef std::pair< CRCore::ref_ptr<crRenderGraph>, CRCore::ref_ptr<crRenderLeaf> >    RenderDataPair; 
		typedef std::vector< RenderDataPair >    RenderDataList; 

		enum SortMode
		{
			SORT_BY_STATE,
			SORT_BY_STATE_THEN_FRONT_TO_BACK,
			SORT_FRONT_TO_BACK,
			SORT_BACK_TO_FRONT,
			SORT_UI,
		};

        // static methods.
        static crRenderBin* createRenderBin(const std::string& binName);
        static crRenderBin* getRenderBinPrototype(const std::string& binName);
        static void addRenderBinPrototype(const std::string& binName,crRenderBin* proto);
        static void removeRenderBinPrototype(crRenderBin* proto);

		static void setDefaultRenderBinSortMode(SortMode mode);
		static SortMode getDefaultRenderBinSortMode();

        crRenderBin();
		crRenderBin(SortMode mode);

        /** Copy constructor using crCopyOp to manage deep vs shallow copy.*/
        crRenderBin(const crRenderBin& rhs,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);
		
        virtual CRCore::crBase* cloneType() const { return new crRenderBin(); }
        virtual CRCore::crBase* clone(const CRCore::crCopyOp& copyop) const { return new crRenderBin(*this,copyop); } // note only implements a clone of type.
        virtual bool isSameKindAs(const CRCore::crBase* obj) const { return dynamic_cast<const crRenderBin*>(obj)!=0L; }
        virtual const char* libraryName() const { return "CRUtil"; }
        virtual const char* className() const { return "crRenderBin"; }

        virtual void reset();
        
		crRenderBin* getParent() { return m_parent; }
		const crRenderBin* getParent() const { return m_parent; }

		crRenderStage* getStage() { return m_stage; }
		const crRenderStage* getStage() const { return m_stage; }

		int getBinNum() const { return m_binNum; }

		//RenderGraphList& getRenderGraphList() { return m_renderGraphList; }
		//const RenderGraphList& getRenderGraphList() const { return m_renderGraphList; }

		RenderBinList& getRenderBinList() { return m_bins; }
		const RenderBinList& getRenderBinList() const { return m_bins; }

		RenderLeafList& getRenderLeafList() { return m_renderLeafList; }
		const RenderLeafList& getRenderLeafList() const { return m_renderLeafList; }

        crRenderBin* find_or_insert(int binNum,const std::string& binName);

        void addRenderGraph(crRenderGraph* rg)
        {
            m_renderGraphList.push_back(rg);
        }

		/** return true if all of drawables, lights and children are empty.*/
		inline bool empty() const
		{
			return m_renderGraphList.empty() && m_bins.empty();
		}

        void sort();

        virtual void sortImplementation();
	
        void setSortMode(SortMode mode);
        SortMode getSortMode() const { return m_sortMode; }

        virtual void sortByState();
		virtual void sortByStateThenFrontToBack();
        virtual void sortFrontToBack();
        virtual void sortBackToFront();
        
        struct SortCallback : public CRCore::Referenced    
        {
            virtual void sortImplementation(crRenderBin*) = 0;
        };

        void setSortCallback(SortCallback* sortCallback) { m_sortCallback = sortCallback; }
        SortCallback* getSortCallback() { return m_sortCallback.get(); }
        const SortCallback* getSortCallback() const { return m_sortCallback.get(); }



        virtual void draw(CRCore::crState& state,crRenderLeaf*& previous);

        virtual void drawImplementation(CRCore::crState& state,crRenderLeaf*& previous);

        struct DrawCallback : public CRCore::Referenced    
        {
            virtual void drawImplementation(crRenderBin* bin,CRCore::crState& state,crRenderLeaf*& previous) = 0;
        };

        void setDrawCallback(DrawCallback* drawCallback) { m_drawCallback = drawCallback; }
        DrawCallback* getDrawCallback() { return m_drawCallback.get(); }
        const DrawCallback* getDrawCallback() const { return m_drawCallback.get(); }

		void insertRenderLeafintoDataList( CRCore::crState& state, crRenderLeaf *r1, crRenderLeaf *previous );
        void createRenderDataList(CRCore::crState& state,crRenderLeaf* previous);
		const RenderDataList &getRenderDataList() const { return m_renderDataList; };
		RenderDataList &getRenderDataList() { return m_renderDataList; };

		/** prune the empty crRenderGraph */
		void prune();


        ///** extract stats for current draw list. */
        //bool getStats(crStatistics* primStats);
        //void getPrims(crStatistics* primStats);
        //bool getPrims(crStatistics* primStats, int nbin);

		void copyLeavesFromRenderGraphListToRenderLeafList();
        void drawZprePassImplementation(CRCore::crState& state,crRenderLeaf*& previous);
    public:
   
        int                               m_binNum;
        crRenderBin*                      m_parent;
        crRenderStage*                    m_stage;
        RenderBinList                     m_bins;
        RenderGraphList                   m_renderGraphList;
        RenderLeafList                    m_renderLeafList;
           
        RenderDataList                    m_renderDataList;

        SortMode                           m_sortMode;
        CRCore::ref_ptr<SortCallback>      m_sortCallback;

        CRCore::ref_ptr<DrawCallback>      m_drawCallback;

    protected:
    
        virtual ~crRenderBin();

};

/** Proxy class for automatic registration of renderbins with the crRenderBin prototypelist.*/
class crRegisterRenderBinProxy
{
    public:
        crRegisterRenderBinProxy(const std::string& binName,crRenderBin* proto)
        {
            m_rb = proto;
            crRenderBin::addRenderBinPrototype(binName,m_rb.get());
        }

        ~crRegisterRenderBinProxy()
        {
            crRenderBin::removeRenderBinPrototype(m_rb.get());
        }
        
    protected:
        CRCore::ref_ptr<crRenderBin> m_rb;
};


}

#endif

    
