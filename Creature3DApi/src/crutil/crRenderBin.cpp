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
#include <CRUtil/crRenderBin.h>
#include <CRUtil/crRenderStage.h>
#include <CRCore/crStatistics.h>

//#include <CRCore/crImpostorSprite.h>
#include <CRCore/crMath.h>
#include <CRCore/crNotify.h>
#include <CRCore/crBrain.h>
#include <algorithm>

using namespace CRCore;
using namespace CRUtil;


class crRenderBinPrototypeList : public CRCore::Referenced, public std::map< std::string, CRCore::ref_ptr<crRenderBin> > 
{
public:
	crRenderBinPrototypeList() {}
	virtual void clear();
	static crRenderBinPrototypeList* renderBinPrototypeList();
protected:
	~crRenderBinPrototypeList() {}
	static ref_ptr<crRenderBinPrototypeList> m_instance;
};

// register a crRenderStage prototype with the crRenderBin prototype list.
//crRegisterRenderBinProxy s_registerRenderBinProxy("RenderBin",new crRenderBin(crRenderBin::getDefaultRenderBinSortMode()));
//crRegisterRenderBinProxy s_registerDepthSortedBinProxy("DepthSortedBin",new crRenderBin(crRenderBin::SORT_BACK_TO_FRONT));
//crRegisterRenderBinProxy s_registerStateSortedBinProxy("StateSortedBin",new crRenderBin(crRenderBin::SORT_BY_STATE));

ref_ptr<crRenderBinPrototypeList> crRenderBinPrototypeList::m_instance = NULL;

crRenderBinPrototypeList* crRenderBinPrototypeList::renderBinPrototypeList()
{
	if (m_instance==NULL) 
	{
		m_instance = new crRenderBinPrototypeList;
		m_instance->insert(std::make_pair("RenderBin",new crRenderBin(crRenderBin::getDefaultRenderBinSortMode())));
		m_instance->insert(std::make_pair("DepthSortedBin",new crRenderBin(crRenderBin::SORT_BACK_TO_FRONT)));
		m_instance->insert(std::make_pair("StateSortedBin",new crRenderBin(crRenderBin::SORT_BY_STATE)));
		m_instance->insert(std::make_pair("FrontBackSortedBin",new crRenderBin(crRenderBin::SORT_FRONT_TO_BACK)));
		m_instance->insert(std::make_pair("UIBin",new crRenderBin(crRenderBin::SORT_UI)));
		//在切换场景的时候这个不需要释放
		//crBrain::getInstance()->pushInstance(m_instance.get());
	}
	return m_instance.get();
}

void crRenderBinPrototypeList::clear()
{
	m_instance = NULL;
}

//crRenderBinPrototypeList* renderBinPrototypeList()
//{
//    static CRCore::ref_ptr<crRenderBinPrototypeList> s_renderBinPrototypeList = new  crRenderBinPrototypeList;
//    return s_renderBinPrototypeList.get();
//}

crRenderBin* crRenderBin::getRenderBinPrototype(const std::string& binName)
{
	crRenderBinPrototypeList* list = crRenderBinPrototypeList::renderBinPrototypeList();
    if (list)
    {
        crRenderBinPrototypeList::iterator itr = list->find(binName);
        if (itr != list->end()) return itr->second.get();
    }
    return NULL;
}

crRenderBin* crRenderBin::createRenderBin(const std::string& binName)
{
	crRenderBinPrototypeList* list = crRenderBinPrototypeList::renderBinPrototypeList();
    if (list)
    {
        crRenderBin* prototype = getRenderBinPrototype(binName);
        if (prototype) return dynamic_cast<crRenderBin*>(prototype->clone(CRCore::crCopyOp::DEEP_COPY_ALL));
    }
    
    //CRCore::notify(CRCore::WARN) <<"Warning: crRenderBin \""<<binName<<"\" implemention not found, using default crRenderBin as a fallback."<<std::endl;
    return new crRenderBin;
}

void crRenderBin::addRenderBinPrototype(const std::string& binName,crRenderBin* proto)
{
	crRenderBinPrototypeList* list = crRenderBinPrototypeList::renderBinPrototypeList();
    if (list && proto)
    {
        (*list)[binName] = proto;
    }
}

void crRenderBin::removeRenderBinPrototype(crRenderBin* proto)
{
	crRenderBinPrototypeList* list = crRenderBinPrototypeList::renderBinPrototypeList();
    if (list && proto)
    {
        crRenderBinPrototypeList::iterator itr = list->find(proto->className());
        if (itr != list->end()) list->erase(itr);
    }
}

static bool s_defaultBinSortModeInitialized = false;
static crRenderBin::SortMode s_defaultBinSortMode = crRenderBin::SORT_BY_STATE_THEN_FRONT_TO_BACK;//SORT_BY_STATE;

void crRenderBin::setDefaultRenderBinSortMode(crRenderBin::SortMode mode)
{
	s_defaultBinSortModeInitialized = true;
	s_defaultBinSortMode = mode;
}


crRenderBin::SortMode crRenderBin::getDefaultRenderBinSortMode()
{
	if (!s_defaultBinSortModeInitialized)
	{
		s_defaultBinSortModeInitialized = true;

		const char* str = getenv("CR_DEFAULT_BIN_SORT_MODE");
		if (str)
		{
			if (strcmp(str,"SORT_BY_STATE")==0) s_defaultBinSortMode = crRenderBin::SORT_BY_STATE;
			else if (strcmp(str,"SORT_BY_STATE_THEN_FRONT_TO_BACK")==0) s_defaultBinSortMode = crRenderBin::SORT_BY_STATE_THEN_FRONT_TO_BACK;
			else if (strcmp(str,"SORT_FRONT_TO_BACK")==0) s_defaultBinSortMode = crRenderBin::SORT_FRONT_TO_BACK;
			else if (strcmp(str,"SORT_BACK_TO_FRONT")==0) s_defaultBinSortMode = crRenderBin::SORT_BACK_TO_FRONT;
		}
	}

	return s_defaultBinSortMode;
}

crRenderBin::crRenderBin()
{
	m_binNum = 0;
	m_parent = NULL;
	m_stage = NULL;
	m_sortMode = getDefaultRenderBinSortMode();
}

crRenderBin::crRenderBin(SortMode mode)
{
    m_binNum = 0;
    m_parent = NULL;
    m_stage = NULL;
    m_sortMode = mode;
}

crRenderBin::crRenderBin(const crRenderBin& rhs,const crCopyOp& copyop):
        crBase(rhs,copyop),
        m_binNum(rhs.m_binNum),
        m_parent(rhs.m_parent),
        m_stage(rhs.m_stage),
        m_bins(rhs.m_bins),
        m_renderGraphList(rhs.m_renderGraphList),
        m_renderLeafList(rhs.m_renderLeafList),
        m_sortMode(rhs.m_sortMode),
        m_sortCallback(rhs.m_sortCallback),
        m_drawCallback(rhs.m_drawCallback)
{

}

crRenderBin::~crRenderBin()
{
	//CRCore::notify(CRCore::ALWAYS)<<"~crRenderBin()"<< std::endl;
}

void crRenderBin::reset()
{
	m_renderGraphList.resize(0);
	//m_bins.clear();
	//m_renderGraphList.clear();
	m_bins.clear();
}

void crRenderBin::prune()
{
	//std::vector<int> toEraseList;
	//RenderGraphList rgEraseList;

	// call prune on all children.
	for(RenderBinList::iterator citr=m_bins.begin();
		citr!=m_bins.end();
		)
	{
		citr->second->prune();

		for(RenderGraphList::iterator itr=m_renderGraphList.begin();
			itr!=m_renderGraphList.end();
			)
		{
			(*itr)->prune();
			if((*itr)->empty())
			{
			    //rgEraseList.push_back(*itr);
				itr = m_renderGraphList.erase(itr);
			}
			else
			{
				++itr;
			}
		}

		//for( RenderGraphList::iterator eitr = rgEraseList.begin();
		//	 eitr != rgEraseList.end();
		//	 ++eitr )
		//{
		//	m_renderGraphList.erase(eitr);
		//}

		if (citr->second->empty())
		{
			citr = m_bins.erase(citr);
			//toEraseList.push_back(citr->first);
		}
		else
		{
			++citr;
		}
	}

	//for(std::vector<int>::iterator eitr=toEraseList.begin();
	//	eitr!=toEraseList.end();
	//	++eitr)
	//{
	//	m_bins.erase(*eitr);
	//}

}

void crRenderBin::sort()
{
    for(RenderBinList::iterator itr = m_bins.begin();
        itr!=m_bins.end();
        ++itr)
    {
        itr->second->sort();
    }
    
    if (m_sortCallback.valid()) 
    {
        m_sortCallback->sortImplementation(this);
    }
    else sortImplementation();
}

void crRenderBin::setSortMode(SortMode mode)
{
    m_sortMode = mode;
}

void crRenderBin::sortImplementation()
{
    switch(m_sortMode)
    {
	case(SORT_BY_STATE):
		sortByState();
		break;
	case(SORT_BY_STATE_THEN_FRONT_TO_BACK):
		sortByStateThenFrontToBack();
		//sortFrontToBack();
		break;
	case(SORT_FRONT_TO_BACK):
		sortFrontToBack();
		break;
	case(SORT_BACK_TO_FRONT):
		sortBackToFront();
		break;
	default:
		break;
    }
}

struct SortByStateFunctor
{
    bool operator() (const crRenderGraph* lhs,const crRenderGraph* rhs) const
    {
        //return (*(lhs->m_stateset)<*(rhs->m_stateset));
		return (lhs->m_stateset<rhs->m_stateset);
    }
};

void crRenderBin::sortByState()
{
    // actually we'll do nothing right now, as fine grained sorting by state
    // appears to cost more to do than it saves in draw.  The contents of
    // the crRenderGraph leaves is already coasrse grained sorted, this
    // sorting is as a function of the cull traversal.
    // cout << "doing sortByState "<<this<<endl;
}

struct RenderGraphFrontToBackSortFunctor
{
	bool operator() (const ref_ptr<crRenderGraph> lhs,const ref_ptr<crRenderGraph> rhs) const
	{
		return (lhs->m_minimumDistance<rhs->m_minimumDistance);
	}
};

void crRenderBin::sortByStateThenFrontToBack()
{
	for(RenderGraphList::iterator itr=m_renderGraphList.begin();
		itr!=m_renderGraphList.end();
		++itr)
	{
		(*itr)->sortFrontToBack();
		(*itr)->getMinimumDistance();
	}
	std::sort(m_renderGraphList.begin(),m_renderGraphList.end(),RenderGraphFrontToBackSortFunctor());
}

struct FrontToBackSortFunctor
{
    bool operator() (const ref_ptr<crRenderLeaf> lhs,const ref_ptr<crRenderLeaf> rhs) const
    {
        return (lhs->m_depth<rhs->m_depth);
    }
};

    
void crRenderBin::sortFrontToBack()
{
    copyLeavesFromRenderGraphListToRenderLeafList();

    // now sort the list into acending depth order.
    std::sort(m_renderLeafList.begin(),m_renderLeafList.end(),FrontToBackSortFunctor());
    
//    cout << "sort front to back"<<endl;
}

struct BackToFrontSortFunctor
{
    bool operator() (const ref_ptr<crRenderLeaf> lhs,const ref_ptr<crRenderLeaf> rhs) const
    {
        return (rhs->m_depth<lhs->m_depth);
    }
};

void crRenderBin::sortBackToFront()
{
    copyLeavesFromRenderGraphListToRenderLeafList();

    // now sort the list into acending depth order.
    std::sort(m_renderLeafList.begin(),m_renderLeafList.end(),BackToFrontSortFunctor());

//    cout << "sort back to front"<<endl;
}

void crRenderBin::copyLeavesFromRenderGraphListToRenderLeafList()
{
    m_renderLeafList.clear();

    int totalsize=0;
    RenderGraphList::iterator itr;
    for(itr=m_renderGraphList.begin();
        itr!=m_renderGraphList.end();
        ++itr)
    {
        totalsize += (*itr)->m_leaves.size();
    }

    m_renderLeafList.reserve(totalsize);
    
    // first copy all the leaves from the render graphs into the leaf list.
    for(RenderGraphList::iterator itr=m_renderGraphList.begin();
        itr!=m_renderGraphList.end();
        ++itr)
    {
        for(crRenderGraph::LeafList::iterator dw_itr = (*itr)->m_leaves.begin();
            dw_itr != (*itr)->m_leaves.end();
            ++dw_itr)
        {
            m_renderLeafList.push_back(dw_itr->get());
        }
    }
    
    // empty the render graph list to prevent it being drawn along side the render leaf list (see drawImplementation.)
    //m_renderGraphList.clear();
	m_renderGraphList.resize(0);

	//int i = m_renderGraphList.size()+5;
	//if(i>0)
	//{
	//    m_renderGraphList.clear();
	//    m_renderGraphList.reserve(i);
	//}
}

crRenderBin* crRenderBin::find_or_insert(int binNum,const std::string& binName)
{
	// search for appropriate bin.
	RenderBinList::iterator itr = m_bins.find(binNum);
	if (itr!=m_bins.end()) return itr->second.get();

	// create a renderin bin and insert into bin list.
	crRenderBin* rb = crRenderBin::createRenderBin(binName);
	if (rb)
	{

		crRenderStage* rs = dynamic_cast<crRenderStage*>(rb);
		if (rs)
		{
			rs->m_binNum = binNum;
			rs->m_parent = NULL;
			rs->m_stage = rs;
			m_stage->addPreRenderStage(rs);
		}
		else
		{
			rb->m_binNum = binNum;
			rb->m_parent = this;
			rb->m_stage = m_stage;
			m_bins[binNum] = rb;
		}
	}
	return rb;

 //   // search for appropriate bin.
	//CRCore::ref_ptr<crRenderBin> &rb = m_bins[binNum];
 //   if(rb.valid()) return rb.get();

 ///*   RenderBinList::iterator itr = m_bins.find(binNum);
 //   if (itr!=m_bins.end()) return itr->second.get();*/

 //   // create a renderin bin and insert into bin list.
 //   crRenderBin* new_rb = crRenderBin::createRenderBin(binName);
 //   if (new_rb)
 //   {

 //       crRenderStage* rs = dynamic_cast<crRenderStage*>(new_rb);
 //       if (rs)
 //       {
 //           rs->m_binNum = binNum;
 //           rs->m_parent = NULL;
 //           rs->m_stage = rs;
 //           m_stage->addToDependencyList(rs);
 //       }
 //       else
 //       {
 //           new_rb->m_binNum = binNum;
 //           new_rb->m_parent = this;
 //           new_rb->m_stage = m_stage;
 //           rb = new_rb;
 //       }
 //   }
 //   return new_rb;
}

void crRenderBin::drawZprePassImplementation(CRCore::crState& state,crRenderLeaf*& previous)
{
	// draw first set of draw bins.
	RenderBinList::iterator rbitr;
	for(rbitr = m_bins.begin();
		rbitr!=m_bins.end() && rbitr->first<0;
		++rbitr)
	{
		rbitr->second->drawZprePassImplementation(state,previous);
	}

	//CRCore::notify(CRCore::FATAL)<<"crRenderBin::drawImplementation: binNum = "<<m_binNum<<std::endl;
	if(m_binNum == 0)
	{//只绘制实体的Depth
		// draw fine grained ordering.
		ref_ptr<crRenderLeaf> rl;
		for(RenderLeafList::iterator rlitr= m_renderLeafList.begin();
			rlitr!= m_renderLeafList.end();
			++rlitr)
		{
			rl = *rlitr;
			rl->renderZprePass(state,previous);
			previous = rl.get();
		}

		// draw coarse grained ordering.
		//try
		//{
		for(RenderGraphList::iterator oitr=m_renderGraphList.begin();
			oitr!=m_renderGraphList.end();
			++oitr)
		{
			for(crRenderGraph::LeafList::iterator dw_itr = (*oitr)->m_leaves.begin();
				dw_itr != (*oitr)->m_leaves.end();
				++dw_itr)
			{
				rl = dw_itr->get();

				if(rl.valid()) rl->renderZprePass(state,previous);

				previous = rl.get();

			}
		}
		//}
		//catch (...)
		//{
		//	CRCore::notify(CRCore::NOTICE)<<"crRenderBin::drawImplementation error: renderGraphList.size = "<<m_renderGraphList.size()<<std::endl;
		//}
	}
	// draw post bins.
	for(;
		rbitr!=m_bins.end();
		++rbitr)
	{
		rbitr->second->drawZprePassImplementation(state,previous);
	}
}

void crRenderBin::draw(CRCore::crState& state,crRenderLeaf*& previous)
{
    if (m_drawCallback.valid()) 
    {
        m_drawCallback->drawImplementation(this,state,previous);
    }
    else drawImplementation(state,previous);
}

void crRenderBin::drawImplementation(CRCore::crState& state,crRenderLeaf*& previous)
{
    // draw first set of draw bins.
    RenderBinList::iterator rbitr = m_bins.begin();
    for(;
        rbitr!=m_bins.end() && rbitr->first<0;
        ++rbitr)
    {
        rbitr->second->draw(state,previous);
    }

	//CRCore::notify(CRCore::FATAL)<<"crRenderBin::drawImplementation: binNum = "<<m_binNum<<std::endl;
	
    // draw fine grained ordering.
	ref_ptr<crRenderLeaf> rl;
    for(RenderLeafList::iterator rlitr= m_renderLeafList.begin();
        rlitr!= m_renderLeafList.end();
        ++rlitr)
    {
        rl = *rlitr;
        if(rl.valid()) rl->render(state,previous);
        previous = rl.get();
    }

    // draw coarse grained ordering.
//try
//{
	for(RenderGraphList::iterator oitr=m_renderGraphList.begin();
		oitr!=m_renderGraphList.end();
		++oitr)
	{
		if(oitr->valid())
		{
			for(crRenderGraph::LeafList::iterator dw_itr = (*oitr)->m_leaves.begin();
				dw_itr != (*oitr)->m_leaves.end();
				++dw_itr)
			{
				rl = dw_itr->get();

				if(rl.valid()) rl->render(state,previous);

				previous = rl.get();

			}
		}
	}
//}
//catch (...)
//{
//	CRCore::notify(CRCore::NOTICE)<<"crRenderBin::drawImplementation error: renderGraphList.size = "<<m_renderGraphList.size()<<std::endl;
//}
    // draw post bins.
    for(;
        rbitr!=m_bins.end();
        ++rbitr)
    {
        rbitr->second->draw(state,previous);
    }
}

void crRenderBin::insertRenderLeafintoDataList(  CRCore::crState& state, crRenderLeaf *rl, crRenderLeaf *previous )
{
	// don't draw this leaf if the abort rendering flag has been set.
	if (!state.getAbortRendering())
	{
		if (previous)
		{
			// apply state if required.
			crRenderGraph* prev_rg = previous->m_parent;
			crRenderGraph* prev_rg_parent = prev_rg->m_parent;
			crRenderGraph* rg = rl->m_parent;
			if (prev_rg_parent!=rg->m_parent)
			{
				crRenderGraph::moveRenderGraph(state,prev_rg_parent,rg->m_parent);
				// send state changes and matrix changes to OpenGL.
				m_renderDataList.push_back( RenderDataPair( rg, rl ) );
			}
			else if (rg!=prev_rg)
			{
				m_renderDataList.push_back( RenderDataPair( rg, rl ) );
			}
		}
		else
		{
			// apply state if required.
			crRenderGraph::moveRenderGraph(state,NULL,rl->m_parent->m_parent);

			m_renderDataList.push_back( RenderDataPair( rl->m_parent, rl ) );
		}
	}
}
void crRenderBin::createRenderDataList( CRCore::crState& state, crRenderLeaf *previous )
{
	m_renderDataList.clear();
	// draw first set of draw bins.
    RenderBinList::iterator rbitr;
    for(rbitr = m_bins.begin();
        rbitr!=m_bins.end() && rbitr->first<0;
        ++rbitr)
    {
        rbitr->second->createRenderDataList( state, previous );
    }
    

    // draw fine grained ordering.
	ref_ptr<crRenderLeaf> rl;
    for(RenderLeafList::iterator rlitr= m_renderLeafList.begin();
        rlitr!= m_renderLeafList.end();
        ++rlitr)
    {
        rl = *rlitr;
        insertRenderLeafintoDataList( state, rl.get(), previous );
		previous = rl.get();
	}

	// draw coarse grained ordering.
	for(RenderGraphList::iterator oitr=m_renderGraphList.begin();
		oitr!=m_renderGraphList.end();
		++oitr)
	{

		for(crRenderGraph::LeafList::iterator dw_itr = (*oitr)->m_leaves.begin();
			dw_itr != (*oitr)->m_leaves.end();
			++dw_itr)
		{
			rl = *dw_itr;
			insertRenderLeafintoDataList( state, rl.get(), previous );
			previous = rl.get();

		}
	}


	// draw post bins.
	for(;
		rbitr!=m_bins.end();
		++rbitr)
	{
		rbitr->second->createRenderDataList( state, previous );
	}

}


//// stats
//
//bool crRenderBin::getStats(crStatistics* primStats)
//{ // different by return type - collects the stats in this renderrBin
//    bool somestats=false;
//
//    // draw fine grained ordering.
//    for(RenderLeafList::iterator dw_itr = m_renderLeafList.begin();
//        dw_itr != m_renderLeafList.end();
//        ++dw_itr)
//    {
//      crRenderLeaf* rl = *dw_itr;
//      crDrawable* dw= rl->m_drawable.get();
//      primStats->addDrawable(); // number of geosets
//      if (rl->m_modelview.get()) primStats->addMatrix(); // number of matrices
//      if (dw)
//      {
//          // then tot up the primtive types and no vertices.
//          dw->accept(*primStats); // use sub-class to find the stats for each drawable
// //         if (typeid(*dw)==typeid(CRCore::crImpostorSprite)) primStats->addImpostor(1);
//      }
//      somestats = true;
//
//    }
//
//  for(RenderGraphList::iterator oitr=m_renderGraphList.begin();
//        oitr!=m_renderGraphList.end();
//        ++oitr)
//    {
//        
//        for(crRenderGraph::LeafList::iterator dw_itr = (*oitr)->m_leaves.begin();
//            dw_itr != (*oitr)->m_leaves.end();
//            ++dw_itr)
//        {
//            crRenderLeaf* rl = dw_itr->get();
//            crDrawable* dw= rl->m_drawable.get();
//            primStats->addDrawable(); // number of geosets
//            if (rl->m_modelview.get()) primStats->addMatrix(); // number of matrices
//            if (dw)
//            {
//                // then tot up the primtive types and no vertices.
//                dw->accept(*primStats); // use sub-class to find the stats for each drawable
//   //             if (typeid(*dw)==typeid(CRCore::crImpostorSprite)) primStats->addImpostor(1);
//            }
//        }
//        somestats=true;
//    }
//    return somestats;
//}
//
//void crRenderBin::getPrims(crStatistics* primStats)
//{
//    static int ndepth;
//    ndepth++;
//    for(RenderBinList::iterator itr = m_bins.begin();
//        itr!=m_bins.end();
//        ++itr)
//    {
//        primStats->addBins(1);
//        itr->second->getPrims(primStats);
//    }
//    getStats(primStats);
//    ndepth--;
//
//}
//
//bool crRenderBin::getPrims(crStatistics* primStats, int nbin)
//{ // collect stats for array of bins, maximum nbin 
//    // (which will be modified on next call if array of primStats is too small);
//    // return 1 for OK;
//    static int ndepth;
//    bool ok=false;
//    ndepth++;
//    int nb=primStats[0].getBins();
//    if (nb<nbin)
//    { // if statement to protect against writing to bins beyond the maximum seen before
//        primStats[nb].setBinNo(nb);
//        primStats[nb].setDepth(ndepth);
//        getStats(primStats+nb);
//    }
//    primStats[0].addBins(1);
//    for(RenderBinList::iterator itr = m_bins.begin();
//        itr!=m_bins.end();
//        ++itr)
//    {
//        if (itr->second->getPrims(primStats, nbin)) ok = true;
//    }
//    ok=true;
//    ndepth--;
//    return ok;
//}
