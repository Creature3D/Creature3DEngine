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
#include <CRUtil/crOptimizer.h>
#include <CRCore/crBrain.h>
#include <CRCore/crTransform.h>
#include <CRCore/crMatrixTransform.h>
#include <CRCore/crPositionAttitudeTransform.h>
#include <CRCore/crLod.h>
//#include <CRCore/crImpostor.h>
#include <CRCore/crBillboard.h>
#include <CRCore/crGeometry.h>
#include <CRCore/crNotify.h>
#include <CRCore/crOccluderNode.h>
#include <CRCore/crSequence.h>
#include <CRCore/crSwitch.h>
#include <CRCore/crClearNode.h>
#include <CRCore/crLightSource.h>
#include <CRCore/crShapeDrawable.h>
#include <CRCore/crArgumentParser.h>
#include <CRCore/crTextureCubeMap.h>
#include <CRCore/crTexGen.h>
#include <CRCore/crTexEnv.h>
#include <CRCore/crTexEnvCombine.h>
#include <CRCore/crFog.h>
#include <CRCore/crPolygonOffset.h>
#include <CRCore/crClipPlane.h>
#include <CRUtil/crCullVisitor.h>
#include <CRUtil/crTransformAttributeFunctor.h>
#include <CRUtil/crTriStripVisitor.h>
#include <CRUtil/crHighlightMapGenerator.h>
#include <CRUtil/crAcceptGIVisitor.h>
//#include <CRIOManager/crReadFile.h>
#include <CRCore/crDisplaySettings.h>
#include <CRCore/crTimer.h>
#include <CRCore/crLightModel.h>
#include <rbody/Creature3D/CreBody.h>

#include <stdlib.h>
#include <typeinfo>
#include <algorithm>
#include <numeric>
#include <iterator>
using namespace CRCore;
using namespace CRUtil;


////////////////////////////////////////////////////////////////////////////
// Overall Optimization function.
////////////////////////////////////////////////////////////////////////////

static bool isNodeEmpty(const CRCore::crNode& node)
{
	if(node.getName() != "ProxyNode" && node.getName().find("@") != 0)
	{
		if (node.getDataClass()) return false;
		if (node.getUpdateCallback()) return false;
		if (node.getCullCallback()) return false;
		//if (node.getEventCallback()) return false;
		if (node.getNumDescriptions()>0) return false;
		if (node.getStateSet()) return false;
		if (!node.getEnableIntersect()) return false;
		if (node.getNodeMask()!=0xffffffff) return false;
	}
	else if(((const crGroup&)node).getNumChildren() != 0)
		return false;
    return true;
}

static bool isDrawableEmpty(const CRCore::crGeometry& node)
{
	if (!node.getVertexArray()) return true;
	if (!node.getVertexArray()->getNumElements()) return true;
	return false;
}

void crOptimizer::reset()
{
    m_permissableOptimizationsMap.clear();
}

void crOptimizer::optimize(CRCore::crNode* node)
{
    unsigned int options = 0;
    options = DEFAULT_OPTIMIZATIONS;
    optimize(node,options);
}

void crOptimizer::optimize(CRCore::crNode* node, unsigned int options)
{
    CRCore::Timer_t start_tick=0;
	CRCore::Timer_t end_tick=0;
	{
		if (options & OBJECTCOMMANDBUILD)
		{
			//CRCore::notify(CRCore::DEBUG_INFO)<<"Optimizer::optimize() doing OBJECTCOMMANDBUILD"<<std::endl;

			crObjectCommandBuildVisitor ocb;
			node->accept(ocb);
		}

		start_tick = CRCore::Timer::instance()->tick();
		///////////3角化优化
		if (options & TESSELATE_GEOMETRY)
		{
			//CRCore::notify(CRCore::DEBUG_INFO)<<"crOptimizer::optimize() doing TESSELATE_GEOMETRY"<<std::endl;

			crTesselateVisitor tsv;
			node->accept(tsv);        
		}
		end_tick = CRCore::Timer::instance()->tick();
		//CRCore::notify(CRCore::DEBUG_INFO)<<"Time to crTesselateVisitor = "<<CRCore::Timer::instance()->delta_s(start_tick,end_tick)<<std::endl;
		////////////////////////////
		
		start_tick = CRCore::Timer::instance()->tick();
		if (options & INITIAL_CHECK)
		{
			//CRCore::notify(CRCore::DEBUG_INFO)<<"crOptimizer::optimize() doing INITIAL_CHECK"<<std::endl;

			////最初的检查
			crInitialCheckVisitor init;
			node->accept(init);     
			//////////////////    
		}
		end_tick = CRCore::Timer::instance()->tick();
		//CRCore::notify(CRCore::DEBUG_INFO)<<"Time to crInitialCheckVisitor = "<<CRCore::Timer::instance()->delta_s(start_tick,end_tick)<<std::endl;

   
		start_tick = CRCore::Timer::instance()->tick();
		if (options & SHARE_DUPLICATE_STATE)
		{
			//CRCore::notify(CRCore::DEBUG_INFO)<<"crOptimizer::optimize() doing SHARE_DUPLICATE_STATE"<<std::endl;

			crStateVisitor osv(this);
			node->accept(osv);
			osv.optimize();
		}
		end_tick = CRCore::Timer::instance()->tick();
		//CRCore::notify(CRCore::DEBUG_INFO)<<"Time to SHARE_DUPLICATE_STATE = "<<CRCore::Timer::instance()->delta_s(start_tick,end_tick)<<std::endl;
		
		//start_tick = CRCore::Timer::instance()->tick();
		//if (options & COPY_SHARED_NODES)
		//{
		//	CRCore::notify(CRCore::INFO)<<"crOptimizer::optimize() doing COPY_SHARED_NODES"<<std::endl;

		//	crCopySharedSubgraphsVisitor cssv(this);
		//	node->accept(cssv);
		//	cssv.copySharedNodes();
		//}
		//end_tick = CRCore::Timer::instance()->tick();
		//CRCore::notify(CRCore::DEBUG_INFO)<<"Time to COPY_SHARED_NODES = "<<CRCore::Timer::instance()->delta_s(start_tick,end_tick)<<std::endl;

		start_tick = CRCore::Timer::instance()->tick();
		if (options & CHECK_PHYSICS)
		{
			//CRCore::notify(CRCore::DEBUG_INFO)<<"crOptimizer::optimize() doing CHECK_PHYSICS"<<std::endl;

			crCheckPhysicsVisitor cpv;
			node->accept(cpv);
		}
		end_tick = CRCore::Timer::instance()->tick();
		//CRCore::notify(CRCore::DEBUG_INFO)<<"Time to CHECK_PHYSICS = "<<CRCore::Timer::instance()->delta_s(start_tick,end_tick)<<std::endl;

		start_tick = CRCore::Timer::instance()->tick();
		if (options & FLATTEN_STATIC_TRANSFORMS)
		{
			//CRCore::notify(CRCore::DEBUG_INFO)<<"crOptimizer::optimize() doing FLATTEN_STATIC_TRANSFORMS"<<std::endl;

			int i=0;
			bool result = false;
			do
			{
				//CRCore::notify(CRCore::DEBUG_INFO) << "** RemoveStaticTransformsVisitor *** Pass "<<i<<std::endl;
				crFlattenStaticTransformsVisitor fstv(this);
				node->accept(fstv);
				result = fstv.removeTransforms(node);
				++i;
			} while (result);

			// no combine any adjacent static transforms.
			crCombineStaticTransformsVisitor cstv(this);
			node->accept(cstv);
			cstv.removeTransforms(node);

		}
		end_tick = CRCore::Timer::instance()->tick();
		//CRCore::notify(CRCore::DEBUG_INFO)<<"Time to FLATTEN_STATIC_TRANSFORMS = "<<CRCore::Timer::instance()->delta_s(start_tick,end_tick)<<std::endl;
		
		start_tick = CRCore::Timer::instance()->tick();
		if (options & REMOVE_REDUNDANT_NODES)
		{
			//CRCore::notify(CRCore::DEBUG_INFO)<<"crOptimizer::optimize() doing REMOVE_REDUNDANT_NODES"<<std::endl;

			crRemoveEmptyNodesVisitor renv(this);
			node->accept(renv);
			renv.removeEmptyNodes();

			crRemoveRedundantNodesVisitor rrnv(this);
			node->accept(rrnv);
			rrnv.removeRedundantNodes();

		}
	    end_tick = CRCore::Timer::instance()->tick();
	    //CRCore::notify(CRCore::DEBUG_INFO)<<"Time to crRemoveRedundantNodesVisitor = "<<CRCore::Timer::instance()->delta_s(start_tick,end_tick)<<std::endl;

		start_tick = CRCore::Timer::instance()->tick();		
		///////////unite nodes
		if (options & UNITE_NODES)
		{
			//CRCore::notify(CRCore::DEBUG_INFO)<<"crOptimizer::optimize() doing UNITE_NODES"<<std::endl;

			crUniteNodesVisitor ouv(this);
			node->accept(ouv);
		}
		end_tick = CRCore::Timer::instance()->tick();
		//CRCore::notify(CRCore::DEBUG_INFO)<<"Time to crUniteNodesVisitor = "<<CRCore::Timer::instance()->delta_s(start_tick,end_tick)<<std::endl;

		start_tick = CRCore::Timer::instance()->tick();
		if (options & COMBINE_ADJACENT_LODS)
		{
			//CRCore::notify(CRCore::DEBUG_INFO)<<"crOptimizer::optimize() doing COMBINE_ADJACENT_LODS"<<std::endl;

			crCombineLODsVisitor clv(this);
			node->accept(clv);        
			clv.combineLODs();
		}
		end_tick = CRCore::Timer::instance()->tick();
		//CRCore::notify(CRCore::DEBUG_INFO)<<"Time to crCombineLODsVisitor = "<<CRCore::Timer::instance()->delta_s(start_tick,end_tick)<<std::endl;
		
		if(crDisplaySettings::instance()->getRunMode() != 0)
		{
			start_tick = CRCore::Timer::instance()->tick();
			if (options & FREEZE_TRANSFORMS)
			{
				//CRCore::notify(CRCore::DEBUG_INFO)<<"Optimizer::optimize() doing FREEZE_TRANSFORMS"<<std::endl;

				crFreezeTransformsVisitor freezeTransformsVisitor;
				node->accept(freezeTransformsVisitor);
				freezeTransformsVisitor.freeze();
			}
			end_tick = CRCore::Timer::instance()->tick();
			//CRCore::notify(CRCore::DEBUG_INFO)<<"Time to crFreezeTransformsVisitor = "<<CRCore::Timer::instance()->delta_s(start_tick,end_tick)<<std::endl;
		}

		start_tick = CRCore::Timer::instance()->tick();		
		if (options & CHECK_GEOMETRY)
		{
			//CRCore::notify(CRCore::DEBUG_INFO)<<"crOptimizer::optimize() doing CHECK_GEOMETRY"<<std::endl;

			crCheckGeometryVisitor cgv(this);
			node->accept(cgv);
		}
		end_tick = CRCore::Timer::instance()->tick();
		//CRCore::notify(CRCore::DEBUG_INFO)<<"Time to crCheckGeometryVisitor = "<<CRCore::Timer::instance()->delta_s(start_tick,end_tick)<<std::endl;

		start_tick = CRCore::Timer::instance()->tick();		
		if (options & MERGE_GEOMETRY)
		{
			//CRCore::notify(CRCore::DEBUG_INFO)<<"crOptimizer::optimize() doing MERGE_GEOMETRY"<<std::endl;

			crMergeGeometryVisitor mgv(this);
			node->accept(mgv);
		}
		end_tick = CRCore::Timer::instance()->tick();
		//CRCore::notify(CRCore::DEBUG_INFO)<<"Time to crMergeGeometryVisitor = "<<CRCore::Timer::instance()->delta_s(start_tick,end_tick)<<std::endl;
		
		start_tick = CRCore::Timer::instance()->tick();
		if (options & MERGE_VERTEX)
		{
			//CRCore::notify(CRCore::DEBUG_INFO)<<"crOptimizer::optimize() doing MERGE_VERTEX"<<std::endl;

			crMergeVertexVisitor mvv;
			node->accept(mvv);
		}
		end_tick = CRCore::Timer::instance()->tick();
		//CRCore::notify(CRCore::DEBUG_INFO)<<"Time to crMergeVertexVisitor = "<<CRCore::Timer::instance()->delta_s(start_tick,end_tick)<<std::endl;

		start_tick = CRCore::Timer::instance()->tick();		
		if (options & TRISTRIP_GEOMETRY)
		{
			//CRCore::notify(CRCore::DEBUG_INFO)<<"crOptimizer::optimize() doing TRISTRIP_GEOMETRY"<<std::endl;

			crTriStripVisitor tsv(this);
			node->accept(tsv);
			tsv.stripify();
		}
		end_tick = CRCore::Timer::instance()->tick();
		//CRCore::notify(CRCore::DEBUG_INFO)<<"Time to crTriStripVisitor = "<<CRCore::Timer::instance()->delta_s(start_tick,end_tick)<<std::endl;
		
		if(crDisplaySettings::instance()->getRunMode() != 0)
		{
			start_tick = CRCore::Timer::instance()->tick();		
			if (options & SPATIALIZE_GROUPS)
			{
				//CRCore::notify(CRCore::DEBUG_INFO)<<"crOptimizer::optimize() doing SPATIALIZE_GROUPS"<<std::endl;

				crSpatializeGroupsVisitor sv(this);
				node->accept(sv);
				sv.divide();
			}
			end_tick = CRCore::Timer::instance()->tick();
			//CRCore::notify(CRCore::DEBUG_INFO)<<"Time to crSpatializeGroupsVisitor = "<<CRCore::Timer::instance()->delta_s(start_tick,end_tick)<<std::endl;
		}

		if (options & TEXTURE_SETTINGS)
		{
			//CRCore::notify(CRCore::DEBUG_INFO)<<"Optimizer::optimize() doing OPTIMIZE_TEXTURE_SETTINGS"<<std::endl;

			crTextureVisitor tv(true,true, // unref image 
				false,false, // client storage
				true,crDisplaySettings::instance()->getMaxAnisotropy(), // anisotropic filtering
				this );
			node->accept(tv);
		}

		if (options & PRERENDEROPT)
		{
			//CRCore::notify(CRCore::DEBUG_INFO)<<"Optimizer::optimize() doing OPTIMIZE_PRERENDER"<<std::endl;

			crBeforehandRenderVisitor before;
			node->accept(before);
		}

		if (options & NODECOMMANDPARSE)
		{
			//CRCore::notify(CRCore::DEBUG_INFO)<<"Optimizer::optimize() doing NODECOMMANDPARSE"<<std::endl;

			crNodeCommandParseVisitor nodeCommandParse;
			node->accept(nodeCommandParse);
		}
	}
	//外部附加优化
	if( m_optimizeFunctorArray.size() > 0 )
	{	
		OptimizeFunctorArray::iterator iter;
		for(iter=m_optimizeFunctorArray.begin();iter!=m_optimizeFunctorArray.end();iter++)
		{
			(*(iter->get()))(node);
		}
	}
}


////////////////////////////////////////////////////////////////////////////
// Tesselate geometry - eg break complex POLYGONS into triangles, strips, fans..
////////////////////////////////////////////////////////////////////////////
void crOptimizer::crTesselateVisitor::apply(CRCore::crFilterRenderManager& filter)
{

}
void crOptimizer::crTesselateVisitor::apply(CRCore::crObject& object)
{
    for(unsigned int i=0;i<object.getNumDrawables();++i)
    {
        CRCore::crGeometry* geom = dynamic_cast<CRCore::crGeometry*>(object.getDrawable(i));
        if (geom) 
		{
			CRUtil::crTesselator tesselator;
            //tesselator.retesselate(*geom);
			tesselator.retesselatePolygons(*geom);
        }
    }
//    traverse(object);
}


////////////////////////////////////////////////////////////////////////////
// Optimize crState Visitor(StateData)
////////////////////////////////////////////////////////////////////////////

//struct LessAttributeFunctor
//{
//    bool operator () (const CRCore::crStateAttribute* lhs,const CRCore::crStateAttribute* rhs) const
//    {
//        return (*lhs<*rhs); 
//    }
//};

void crOptimizer::crStateVisitor::reset()
{
    m_statesets.clear();
}

void crOptimizer::crStateVisitor::addStateSet(CRCore::crStateSet* stateset,CRCore::crBase* base)
{
    m_statesets[stateset].insert(base);
}

void crOptimizer::crStateVisitor::apply(CRCore::crNode& node)
{
    CRCore::crStateSet* ss = node.getStateSet();
    if (ss && ss->getDataVariance()==CRCore::crBase::STATIC) 
    {
        if (isOperationPermissableForObject(&node) &&
            isOperationPermissableForObject(ss))
        {
            addStateSet(ss,&node);
        }
    }

    traverse(node);
}

void crOptimizer::crStateVisitor::apply(CRCore::crObject& object)
{
    if (!isOperationPermissableForObject(&object)) return;

    CRCore::crStateSet* ss = object.getStateSet();
    
    if (ss && ss->getDataVariance()==CRCore::crBase::STATIC)
    {
        if (isOperationPermissableForObject(ss))
        {
            addStateSet(ss,&object);
        }
    }
    for(unsigned int i=0;i<object.getNumDrawables();++i)
    {
        CRCore::crDrawable* drawable = object.getDrawable(i);
        if (drawable)
        {
            ss = drawable->getStateSet();
            if (ss && ss->getDataVariance()==CRCore::crBase::STATIC)
            {
                if (isOperationPermissableForObject(drawable) &&
                    isOperationPermissableForObject(ss))
                {
                    addStateSet(ss,drawable);
                }
            }
        }
    }
}

void crOptimizer::crStateVisitor::optimize()
{
	//CRCore::notify(CRCore::NOTICE) << "Num of crStateSet="<<m_statesets.size()<< std::endl;

    // duplicate state attributes removed.
    // now need to look at duplicate state sets.
	crNode *pNode=NULL;
	CRCore::ref_ptr<CRCore::crBase> setItr;
	for(StateSetMap::iterator itr=m_statesets.begin();
		itr!=m_statesets.end();
		++itr)
    {
		if (itr->second.size()>1)
		{
			//CRCore::notify(CRCore::NOTICE) << "itr->second.size()="<<itr->second.size()<< std::endl;

			BaseSet::iterator bsitr=itr->second.begin();
			for(bsitr;bsitr!=itr->second.end();bsitr++)
			{
				setItr = *bsitr;
			    if(setItr->referenceCount()>1)
				    setItr->setStateSet(itr->first);
			}
        }
    } 
}

//void crOptimizer::crStateVisitor::optimize()
//{
//    CRCore::notify(CRCore::INFO) << "Num of crStateSet="<<m_statesets.size()<< std::endl;
//
//	{
//		// create map from state attributes to stateset which contain them.
//		typedef std::pair<CRCore::crStateSet*,unsigned int>      StateSetUnitPair;
//		typedef std::set<StateSetUnitPair>                  StateSetList;
//		typedef std::map<CRCore::crStateAttribute*,StateSetList> AttributeToStateSetMap;
//		AttributeToStateSetMap attributeToStateSetMap;
//
//		// create map from uniforms to stateset when contain them.
//		typedef std::set<CRCore::crStateSet*>                    StateSetSet;
//		typedef std::map<CRCore::crUniform*,StateSetSet>         UniformToStateSetMap;
//
//		const unsigned int NON_TEXTURE_ATTRIBUTE = 0xffffffff;
//
//		UniformToStateSetMap  uniformToStateSetMap;
//
//		// NOTE - TODO will need to track state attribute override value too.
//
//		for(StateSetMap::iterator sitr=m_statesets.begin();
//			sitr!=m_statesets.end();
//			++sitr)
//		{
//			CRCore::crStateSet::AttributeList& attributes = sitr->first->getAttributeList();
//			for(CRCore::crStateSet::AttributeList::iterator aitr= attributes.begin();
//				aitr!=attributes.end();
//				++aitr)
//			{
//				if (aitr->second.first->getDataVariance()==CRCore::crBase::STATIC)
//				{
//					attributeToStateSetMap[aitr->second.first.get()].insert(StateSetUnitPair(sitr->first,NON_TEXTURE_ATTRIBUTE));
//				}
//			}
//
//
//			CRCore::crStateSet::TextureAttributeList& texAttributes = sitr->first->getTextureAttributeList();
//			for(unsigned int unit=0;unit<texAttributes.size();++unit)
//			{
//				CRCore::crStateSet::AttributeList& attributes = texAttributes[unit];
//				for(CRCore::crStateSet::AttributeList::iterator aitr= attributes.begin();
//					aitr!=attributes.end();
//					++aitr)
//				{
//					if (aitr->second.first->getDataVariance()==CRCore::crBase::STATIC)
//					{
//						attributeToStateSetMap[aitr->second.first.get()].insert(StateSetUnitPair(sitr->first,unit));
//					}
//				}
//			}
//
//
//			CRCore::crStateSet::UniformList& uniforms = sitr->first->getUniformList();
//			for(CRCore::crStateSet::UniformList::iterator uitr= uniforms.begin();
//				uitr!=uniforms.end();
//				++uitr)
//			{
//				if (uitr->second.first->getDataVariance()==CRCore::crBase::STATIC)
//				{
//					uniformToStateSetMap[uitr->second.first.get()].insert(sitr->first);
//				}
//			}
//
//		}
//
//		if (attributeToStateSetMap.size()>=2)
//		{
//			// create unique set of state attribute pointers.
//			typedef std::vector<CRCore::crStateAttribute*> AttributeList;
//			AttributeList attributeList;
//
//			for(AttributeToStateSetMap::iterator aitr=attributeToStateSetMap.begin();
//				aitr!=attributeToStateSetMap.end();
//				++aitr)
//			{
//				attributeList.push_back(aitr->first);
//			}
//
//			// sort the attributes so that equal attributes sit along side each
//			// other.
//			std::sort(attributeList.begin(),attributeList.end(),LessDerefFunctor<CRCore::crStateAttribute>());
//
//			CRCore::notify(CRCore::INFO) << "state attribute list"<< std::endl;
//			for(AttributeList::iterator aaitr = attributeList.begin();
//				aaitr!=attributeList.end();
//				++aaitr)
//			{
//				CRCore::notify(CRCore::INFO) << "    "<<*aaitr << "  "<<(*aaitr)->className()<< std::endl;
//			}
//
//			CRCore::notify(CRCore::INFO) << "searching for duplicate attributes"<< std::endl;
//			// find the duplicates.
//			AttributeList::iterator first_unique = attributeList.begin();
//			AttributeList::iterator current = first_unique;
//			++current;
//			for(; current!=attributeList.end();++current)
//			{
//				if (**current==**first_unique)
//				{
//					CRCore::notify(CRCore::INFO) << "    found duplicate "<<(*current)->className()<<"  first="<<*first_unique<<"  current="<<*current<< std::endl;
//					StateSetList& statesetlist = attributeToStateSetMap[*current];
//					for(StateSetList::iterator sitr=statesetlist.begin();
//						sitr!=statesetlist.end();
//						++sitr)
//					{
//						CRCore::notify(CRCore::INFO) << "       replace duplicate "<<*current<<" with "<<*first_unique<< std::endl;
//						CRCore::crStateSet* stateset = sitr->first;
//						unsigned int unit = sitr->second;
//						if (unit==NON_TEXTURE_ATTRIBUTE) stateset->setAttribute(*first_unique);
//						else stateset->setTextureAttribute(unit,*first_unique);
//					}
//				}
//				else first_unique = current;
//			}
//		}
//
//
//		if (uniformToStateSetMap.size()>=2)
//		{
//			// create unique set of uniform pointers.
//			typedef std::vector<CRCore::crUniform*> UniformList;
//			UniformList uniformList;
//
//			for(UniformToStateSetMap::iterator aitr=uniformToStateSetMap.begin();
//				aitr!=uniformToStateSetMap.end();
//				++aitr)
//			{
//				uniformList.push_back(aitr->first);
//			}
//
//			// sort the uniforms so that equal uniforms sit along side each
//			// other.
//			std::sort(uniformList.begin(),uniformList.end(),LessDerefFunctor<CRCore::crUniform>());
//
//			CRCore::notify(CRCore::INFO) << "state uniform list"<< std::endl;
//			for(UniformList::iterator uuitr = uniformList.begin();
//				uuitr!=uniformList.end();
//				++uuitr)
//			{
//				CRCore::notify(CRCore::INFO) << "    "<<*uuitr << "  "<<(*uuitr)->getName()<< std::endl;
//			}
//
//			CRCore::notify(CRCore::INFO) << "searching for duplicate uniforms"<< std::endl;
//			// find the duplicates.
//			UniformList::iterator first_unique_uniform = uniformList.begin();
//			UniformList::iterator current_uniform = first_unique_uniform; 
//			++current_uniform;
//			for(; current_uniform!=uniformList.end();++current_uniform)
//			{
//				if ((**current_uniform)==(**first_unique_uniform))
//				{
//					CRCore::notify(CRCore::INFO) << "    found duplicate uniform "<<(*current_uniform)->getName()<<"  first_unique_uniform="<<*first_unique_uniform<<"  current_uniform="<<*current_uniform<< std::endl;
//					StateSetSet& statesetset = uniformToStateSetMap[*current_uniform];
//					for(StateSetSet::iterator sitr=statesetset.begin();
//						sitr!=statesetset.end();
//						++sitr)
//					{
//						CRCore::notify(CRCore::INFO) << "       replace duplicate "<<*current_uniform<<" with "<<*first_unique_uniform<< std::endl;
//						CRCore::crStateSet* stateset = *sitr;
//						stateset->addUniform(*first_unique_uniform);
//					}
//				}
//				else first_unique_uniform = current_uniform;
//			}
//		}
//
//	}
//
//	// duplicate state attributes removed.
//	// now need to look at duplicate state sets.
//	if (m_statesets.size()>=2)
//	{
//		// create the list of stateset's.
//		typedef std::vector<CRCore::crStateSet*> StateSetSortList;
//		StateSetSortList statesetSortList;
//		for(StateSetMap::iterator ssitr=m_statesets.begin();
//			ssitr!=m_statesets.end();
//			++ssitr)
//		{
//			statesetSortList.push_back(ssitr->first);
//		}
//
//
//		// sort the StateSet's so that equal StateSet's sit along side each
//		// other.
//		std::sort(statesetSortList.begin(),statesetSortList.end(),LessDerefFunctor<CRCore::crStateSet>());
//
//		CRCore::notify(CRCore::INFO) << "searching for duplicate attributes"<< std::endl;
//		// find the duplicates.
//		StateSetSortList::iterator first_unique = statesetSortList.begin();
//		StateSetSortList::iterator current = first_unique; ++current;
//		for(; current!=statesetSortList.end();++current)
//		{
//			if (**current==**first_unique)
//			{
//				CRCore::notify(CRCore::INFO) << "    found duplicate "<<(*current)->className()<<"  first="<<*first_unique<<"  current="<<*current<< std::endl;
//				BaseSet& objSet = m_statesets[*current];
//				for(BaseSet::iterator sitr=objSet.begin();
//					sitr!=objSet.end();
//					++sitr)
//				{
//					CRCore::notify(CRCore::INFO) << "       replace duplicate "<<*current<<" with "<<*first_unique<< std::endl;
//					CRCore::crBase* obj = sitr->get();
//					CRCore::crDrawable* drawable = dynamic_cast<CRCore::crDrawable*>(obj);
//					if (drawable)
//					{
//						drawable->setStateSet(*first_unique);
//					}
//					else 
//					{
//						CRCore::crNode* node = dynamic_cast<CRCore::crNode*>(obj);
//						if (node)
//						{
//							node->setStateSet(*first_unique);
//						}
//					}
//				}
//			}
//			else first_unique = current;
//		}
//	}
//
//}
//
//
////////////////////////////////////////////////////////////////////////////
// Optimize crState Visitor
////////////////////////////////////////////////////////////////////////////

/*
struct LessAttributeFunctor
{
	bool operator () (const CRCore::crStateAttribute* lhs,const CRCore::crStateAttribute* rhs) const
	{
		return (*lhs<*rhs); 
	}
};

struct LessStateSetFunctor
{
	bool operator () (const CRCore::crStateSet* lhs,const CRCore::crStateSet* rhs) const
	{
		return (*lhs<*rhs);
	}
};



void crOptimizer::crStateVisitor::reset()
{
	m_statesets.clear();
}

void crOptimizer::crStateVisitor::addStateSet(CRCore::crStateSet* stateset,CRCore::crBase* base)
{
	m_statesets[stateset].insert(base);
}

void crOptimizer::crStateVisitor::apply(CRCore::crNode& node)
{

	CRCore::crStateSet* ss = node.getStateSet();
	if (ss && ss->getDataVariance()==CRCore::crBase::STATIC) 
	{
		if (isOperationPermissableForObject(&node) &&
			isOperationPermissableForObject(ss))
		{
			addStateSet(ss,&node);
		}
	}

	traverse(node);
}

void crOptimizer::crStateVisitor::apply(CRCore::crObject& object)
{
	if (!isOperationPermissableForObject(&object)) return;

	CRCore::crStateSet* ss = object.getStateSet();


	if (ss && ss->getDataVariance()==CRCore::crBase::STATIC)
	{
		if (isOperationPermissableForObject(ss))
		{
			addStateSet(ss,&object);
		}
	}
	for(unsigned int i=0;i<object.getNumDrawables();++i)
	{
		CRCore::crDrawable* drawable = object.getDrawable(i);
		if (drawable)
		{
			ss = drawable->getStateSet();
			if (ss && ss->getDataVariance()==CRCore::crBase::STATIC)
			{
				if (isOperationPermissableForObject(drawable) &&
					isOperationPermissableForObject(ss))
				{
					addStateSet(ss,drawable);
				}
			}
		}
	}
}

void crOptimizer::crStateVisitor::optimize()
{
	CRCore::notify(CRCore::INFO) << "Num of crStateSet="<<m_statesets.size()<< std::endl;

	{
		// create map from state attributes to stateset which contain them.
		typedef std::pair<CRCore::crStateSet*,unsigned int>      StateSetUnitPair;
		typedef std::set<StateSetUnitPair>                  StateSetList;
		typedef std::map<CRCore::crStateAttribute*,StateSetList> AttributeToStateSetMap;

		const unsigned int NON_TEXTURE_ATTRIBUTE = 0xffffffff;

		AttributeToStateSetMap m_attributeToStateSetMap;

		// NOTE will need to track state attribute override value too.

		for(StateSetMap::iterator sitr=m_statesets.begin();
			sitr!=m_statesets.end();
			++sitr)
		{
			CRCore::crStateSet::AttributeList& attributes = sitr->first->getAttributeList();
			for(CRCore::crStateSet::AttributeList::iterator aitr= attributes.begin();
				aitr!=attributes.end();
				++aitr)
			{
				if (aitr->second.first->getDataVariance()==CRCore::crBase::STATIC)
				{
					m_attributeToStateSetMap[aitr->second.first.get()].insert(StateSetUnitPair(sitr->first,NON_TEXTURE_ATTRIBUTE));
				}
			}


			CRCore::crStateSet::TextureAttributeList& texAttributes = sitr->first->getTextureAttributeList();
			for(unsigned int unit=0;unit<texAttributes.size();++unit)
			{
				CRCore::crStateSet::AttributeList& attributes = texAttributes[unit];
				for(CRCore::crStateSet::AttributeList::iterator aitr= attributes.begin();
					aitr!=attributes.end();
					++aitr)
				{
					if (aitr->second.first->getDataVariance()==CRCore::crBase::STATIC)
					{
						m_attributeToStateSetMap[aitr->second.first.get()].insert(StateSetUnitPair(sitr->first,unit));
					}
				}
			}

		}

		if (m_attributeToStateSetMap.size()>2)
		{
			CRCore::notify(CRCore::INFO) << "Too few state attributes to optimize."<< std::endl;
			return;
		}

		// create unique set of state attribute pointers.
		typedef std::vector<CRCore::crStateAttribute*> AttributeList;
		AttributeList m_attributeList;

		for(AttributeToStateSetMap::iterator aitr=m_attributeToStateSetMap.begin();
			aitr!=m_attributeToStateSetMap.end();
			++aitr)
		{
			m_attributeList.push_back(aitr->first);
		}

		// sort the attributes so that equal attributes sit along side each
		// other.
		std::sort(m_attributeList.begin(),m_attributeList.end(),LessAttributeFunctor());


		CRCore::notify(CRCore::INFO) << "state attribute list"<< std::endl;
		for(AttributeList::iterator aaitr = m_attributeList.begin();
			aaitr!=m_attributeList.end();
			++aaitr)
		{
			CRCore::notify(CRCore::INFO) << "    "<<*aaitr << "  "<<(*aaitr)->className()<< std::endl;
		}

		CRCore::notify(CRCore::INFO) << "searching for duplicate attributes"<< std::endl;
		// find the duplicates.
		AttributeList::iterator first_unique = m_attributeList.begin();
		AttributeList::iterator current = first_unique; ++current;
		for(; current!=m_attributeList.end();++current)
		{
			if (**current==**first_unique)
			{
				CRCore::notify(CRCore::INFO) << "    found duplicate "<<(*current)->className()<<"  first="<<*first_unique<<"  current="<<*current<< std::endl;
				StateSetList& statesetlist = m_attributeToStateSetMap[*current];
				for(StateSetList::iterator sitr=statesetlist.begin();
					sitr!=statesetlist.end();
					++sitr)
				{
					CRCore::notify(CRCore::INFO) << "       replace duplicate "<<*current<<" with "<<*first_unique<< std::endl;
					CRCore::crStateSet* stateset = sitr->first;
					unsigned int unit = sitr->second;
					if (unit==NON_TEXTURE_ATTRIBUTE) stateset->setAttribute(*first_unique);
					else stateset->setTextureAttribute(unit,*first_unique);
				}
			}
			else first_unique = current;
		}

	}
	// duplicate state attributes removed.
	// now need to look at duplicate state sets.

	{
		// create the list of stateset's.
		typedef std::vector<CRCore::crStateSet*> StateSetSortList;
		StateSetSortList m_statesetSortList;
		for(StateSetMap::iterator ssitr=m_statesets.begin();
			ssitr!=m_statesets.end();
			++ssitr)
		{
			m_statesetSortList.push_back(ssitr->first);
		}


		// sort the crStateSet's so that equal crStateSet's sit along side each
		// other.
		std::sort(m_statesetSortList.begin(),m_statesetSortList.end(),LessStateSetFunctor());

		CRCore::notify(CRCore::INFO) << "searching for duplicate attributes"<< std::endl;
		// find the duplicates.
		StateSetSortList::iterator first_unique = m_statesetSortList.begin();
		StateSetSortList::iterator current = first_unique; ++current;
		for(; current!=m_statesetSortList.end();++current)
		{
			if (**current==**first_unique)
			{
				CRCore::notify(CRCore::INFO) << "    found duplicate "<<(*current)->className()<<"  first="<<*first_unique<<"  current="<<*current<< std::endl;
				BaseSet& baseSet = m_statesets[*current];
				for(BaseSet::iterator sitr=baseSet.begin();
					sitr!=baseSet.end();
					++sitr)
				{
					CRCore::notify(CRCore::INFO) << "       replace duplicate "<<*current<<" with "<<*first_unique<< std::endl;
					CRCore::crBase* base = *sitr;
					CRCore::crDrawable* drawable = dynamic_cast<CRCore::crDrawable*>(base);
					if (drawable)
					{
						drawable->setStateSet(*first_unique);
					}
					else 
					{
						CRCore::crNode* node = dynamic_cast<CRCore::crNode*>(base);
						if (node)
						{
							node->setStateSet(*first_unique);
						}
					}
				}
			}
			else first_unique = current;
		}
	}

}
*/
////////////////////////////////////////////////////////////////////////////
// Flatten static transforms
////////////////////////////////////////////////////////////////////////////

class crCollectLowestTransformsVisitor : public CRCore::crNodeVisitor
{
    public:


        crCollectLowestTransformsVisitor(crOptimizer* optimizer=0):
                    CRCore::crNodeVisitor(CRCore::crNodeVisitor::TRAVERSE_PARENTS),
                    m_optimizer(optimizer) {}

        virtual void apply(CRCore::crNode& node)
        {
            if (node.getNumParents())
            {
                traverse(node);
            }
            else
            {
                // for all current objects mark a NULL transform for them.
                registerWithCurrentObjects(0);
            }
        }

        virtual void apply(CRCore::crLod& lod)
        {
            m_currentObjectList.push_back(&lod);

            traverse(lod);

            m_currentObjectList.pop_back();
        }

        virtual void apply(CRCore::crTransform& transform)
        {
            // for all current objects associated this transform with them.
            registerWithCurrentObjects(&transform);
        }
        
        virtual void apply(CRCore::crObject& object)
        {
            traverse(object);
        }
        
        virtual void apply(CRCore::crBillboard& object)
        {
            traverse(object);
        }
        

        void collectDataFor(CRCore::crBillboard* billboard)
        {
            m_currentObjectList.push_back(billboard);
            
            billboard->accept(*this);
            
            m_currentObjectList.pop_back();
        }
        
        void collectDataFor(CRCore::crDrawable* drawable)
        {
			m_currentObjectList.push_back(drawable);
            CRCore::crDrawable::ParentList& parents = drawable->getParents();
			CRCore::crDrawable::ParentList::iterator itr = parents.begin();
            for( itr; itr!=parents.end(); itr++ )
            {
                (*itr)->accept(*this);
            }

            m_currentObjectList.pop_back();
        }

        void setUpMaps();
        void disableTransform(CRCore::crTransform* transform);
        bool removeTransforms(CRCore::crNode* nodeWeCannotRemove);

        inline bool isOperationPermissableForObject(const CRCore::crBase* object) const
        {
            return m_optimizer ? m_optimizer->isOperationPermissableForObject(object,crOptimizer::FLATTEN_STATIC_TRANSFORMS) :  true; 
        }

    protected:        

        struct TransformStruct
        {
            typedef std::set<CRCore::crBase*> BaseSet;

            TransformStruct():m_canBeApplied(true) {}

            void add(CRCore::crBase* base)
            {
                m_objectSet.insert(base);
            }

            bool        m_canBeApplied;
            BaseSet   m_objectSet;
        };

        struct ObjectStruct
        {
            typedef std::set<CRCore::crTransform*> TransformSet;

            ObjectStruct():m_canBeApplied(true),m_moreThanOneMatrixRequired(false) {}

            void add(CRCore::crTransform* transform)
            {
                if (transform)
                {
                    if (transform->getDataVariance()==CRCore::crTransform::DYNAMIC) m_moreThanOneMatrixRequired=true;
                    else if (transform->getReferenceFrame()==CRCore::crTransform::RELATIVE_TO_ABSOLUTE) m_moreThanOneMatrixRequired=true;
                    else
                    {
                        if (m_transformSet.empty()) transform->computeLocalToWorldMatrix(m_firstMatrix,0);
                        else
                        {
                            CRCore::crMatrix matrix;
                            transform->computeLocalToWorldMatrix(m_firstMatrix,0);
                            if (m_firstMatrix!=matrix) m_moreThanOneMatrixRequired=true;
                        }
                    }
                }
                else
                {
                    if (!m_transformSet.empty()) 
                    {
                        if (m_firstMatrix!=CRCore::crMatrix::identity()) m_moreThanOneMatrixRequired=true;
                    }

                }
                m_transformSet.insert(transform);
            }

            bool            m_canBeApplied;
            bool            m_moreThanOneMatrixRequired;
            CRCore::crMatrix     m_firstMatrix;
            TransformSet    m_transformSet;

        };    


        void registerWithCurrentObjects(CRCore::crTransform* transform)
        {
            for(ObjectList::iterator itr=m_currentObjectList.begin();
                itr!=m_currentObjectList.end();
                ++itr)
            {
                m_objectMap[*itr].add(transform);
            }
        }

        typedef std::map<CRCore::crTransform*,TransformStruct>   TransformMap;
        typedef std::map<CRCore::crBase*,ObjectStruct>         ObjectMap;
        typedef std::vector<CRCore::crBase*>                   ObjectList;

        void disableObject(CRCore::crBase* object)
        {
            disableObject(m_objectMap.find(object));
        }

        void disableObject(ObjectMap::iterator itr);
        void doTransform(CRCore::crBase* base,CRCore::crMatrix& matrix);
        

        crOptimizer*      m_optimizer;
        TransformMap    m_transformMap;
        ObjectMap       m_objectMap;
        ObjectList      m_currentObjectList;

};


void crCollectLowestTransformsVisitor::doTransform(CRCore::crBase* base,CRCore::crMatrix& matrix)
{
    CRCore::crDrawable* drawable = dynamic_cast<CRCore::crDrawable*>(base);
    if (drawable)
    {
        CRUtil::crTransformAttributeFunctor tf(matrix);
        drawable->accept(tf);
        drawable->dirtyBound();
        return;
    }

    CRCore::crLod* lod = dynamic_cast<CRCore::crLod*>(base);
    if (lod)
    {
        CRCore::crMatrix matrix_no_trans = matrix;
        matrix_no_trans.setTrans(0.0f,0.0f,0.0f);
        
        CRCore::crVector3 v111(1.0f,1.0f,1.0f);
        CRCore::crVector3 new_v111 = v111*matrix_no_trans;
        float ratio = new_v111.length()/v111.length();

        // move center point.
        lod->setCenter(lod->getCenter()*matrix);
        
        // adjust ranges to new scale.
        for(unsigned int i=0;i<lod->getNumRanges();++i)
        {
            lod->setRange(i,lod->getMinRange(i)*ratio,lod->getMaxRange(i)*ratio);
        }
        
        lod->dirtyBound();
        return;
    }

    CRCore::crBillboard* billboard = dynamic_cast<CRCore::crBillboard*>(base);
    if (billboard)
    {
        CRCore::crMatrix matrix_no_trans = matrix;
        matrix_no_trans.setTrans(0.0f,0.0f,0.0f);
  
        CRUtil::crTransformAttributeFunctor tf(matrix_no_trans);

        CRCore::crVector3 axis = CRCore::crMatrix::transform3x3(tf.m_im,billboard->getAxis());
        axis.normalize();
        billboard->setAxis(axis);

        CRCore::crVector3 normal = CRCore::crMatrix::transform3x3(tf.m_im,billboard->getNormal());
        normal.normalize();
        billboard->setNormal(normal);


        for(unsigned int i=0;i<billboard->getNumDrawables();++i)
        {
            billboard->setPos(i,billboard->getPos(i)*matrix);
            billboard->getDrawable(i)->accept(tf);
        }
        
        billboard->dirtyBound();

        return;
    }
}

void crCollectLowestTransformsVisitor::disableObject(ObjectMap::iterator itr)
{
    if (itr==m_objectMap.end())
    {
        return;
    }    

    if (itr->second.m_canBeApplied)
    {
        // we havn't been disabled yet so we need to disable,
        itr->second.m_canBeApplied = false;

        // and then inform everybody we have been disabled.
        for(ObjectStruct::TransformSet::iterator titr = itr->second.m_transformSet.begin();
            titr != itr->second.m_transformSet.end();
            ++titr)
        {
            disableTransform(*titr);
        }
    }
}

void crCollectLowestTransformsVisitor::disableTransform(CRCore::crTransform* transform)
{
    TransformMap::iterator itr=m_transformMap.find(transform);
    if (itr==m_transformMap.end())
    {
        return;
    }    

    if (itr->second.m_canBeApplied)
    {
    
        // we havn't been disabled yet so we need to disable,
        itr->second.m_canBeApplied = false;
        // and then inform everybody we have been disabled.
        for(TransformStruct::BaseSet::iterator oitr = itr->second.m_objectSet.begin();
            oitr != itr->second.m_objectSet.end();
            ++oitr)
        {
            disableObject(*oitr);
        }
    }
}

void crCollectLowestTransformsVisitor::setUpMaps()
{

    // create the TransformMap from the ObjectMap
    ObjectMap::iterator oitr;
    for(oitr=m_objectMap.begin();
        oitr!=m_objectMap.end();
        ++oitr)
    {
        CRCore::crBase* object = oitr->first;
        ObjectStruct& os = oitr->second;
        
        for(ObjectStruct::TransformSet::iterator titr = os.m_transformSet.begin();
            titr != os.m_transformSet.end();
            ++titr)
        {
            m_transformMap[*titr].add(object);
        }
    }
    

    // disable all the objects which have more than one matrix associated 
    // with them, and then disable all transforms which have an object associated 
    // them that can't be applied, and then disable all objects which have
    // disabled transforms associated, recursing until all disabled 
    // associativity.
    // and disable all objects that the operation is not permisable for)
    for(oitr=m_objectMap.begin();
        oitr!=m_objectMap.end();
        ++oitr)
    {
        CRCore::crBase* object = oitr->first;
        ObjectStruct& os = oitr->second;
        if (os.m_canBeApplied)
        {
            if (os.m_moreThanOneMatrixRequired || !isOperationPermissableForObject(object))
            {
                disableObject(oitr);
            }
        }
    }
}

bool crCollectLowestTransformsVisitor::removeTransforms(CRCore::crNode* nodeWeCannotRemove)
{
    // transform the objects that can be applied.
    for(ObjectMap::iterator oitr=m_objectMap.begin();
        oitr!=m_objectMap.end();
        ++oitr)
    {
        CRCore::crBase* object = oitr->first;
        ObjectStruct& os = oitr->second;
        if (os.m_canBeApplied)
        {
            doTransform(object,os.m_firstMatrix);
        }
    }


    bool transformRemoved = false;

    // clean up the transforms.
    for(TransformMap::iterator titr=m_transformMap.begin();
        titr!=m_transformMap.end();
        ++titr)
    {
        if (titr->second.m_canBeApplied)
        {
        
            if (titr->first!=nodeWeCannotRemove)
            {
                transformRemoved = true;

                CRCore::ref_ptr<CRCore::crTransform> transform = titr->first;
                CRCore::ref_ptr<CRCore::crGroup>     group = new CRCore::crGroup;
                group->setName( transform->getName() );
                group->setDataVariance(CRCore::crBase::STATIC);
                for(unsigned int i=0;i<transform->getNumChildren();++i)
                {
                    for(unsigned int j=0;j<transform->getNumParents();++j)
                    {
                        group->addChild(transform->getChild(i));
                    }
                }

                for(int i2=transform->getNumParents()-1;i2>=0;--i2)
                {
                    transform->getParent(i2)->replaceChild(transform.get(),group.get());
                }
            }
            else
            {
                CRCore::crMatrixTransform* mt = dynamic_cast<CRCore::crMatrixTransform*>(titr->first);
                if (mt) mt->setMatrix(CRCore::crMatrix::identity());
                else
                {
                    CRCore::crPositionAttitudeTransform* pat = dynamic_cast<CRCore::crPositionAttitudeTransform*>(titr->first);
                    if (pat)
                    {
                        pat->setPosition(CRCore::crVector3(0.0f,0.0f,0.0f));
                        pat->setAttitude(CRCore::crQuat());
                        pat->setPivotPoint(CRCore::crVector3(0.0f,0.0f,0.0f));
                    }
                    else
                    {
                        CRCore::notify(CRCore::WARN)<<"Warning:: during Optimize::crCollectLowestTransformsVisitor::removeTransforms(crNode*)"<<std::endl;
                        CRCore::notify(CRCore::WARN)<<"          unhandled of setting of indentity matrix on "<< titr->first->className()<<std::endl;
                        CRCore::notify(CRCore::WARN)<<"          model will appear in the incorrect position."<<std::endl;
                    }
                }
                
            }                
        }
    }
    m_objectMap.clear();
    m_transformMap.clear();
    
    return transformRemoved;
}

void crOptimizer::crFlattenStaticTransformsVisitor::apply(CRCore::crObject& object)
{

    if (!m_transformStack.empty())
    {
        for(unsigned int i=0;i<object.getNumDrawables();++i)
        {
            m_drawableSet.insert(object.getDrawable(i));
        }
    }
}

void crOptimizer::crFlattenStaticTransformsVisitor::apply(CRCore::crBillboard& billboard)
{
    if (!m_transformStack.empty())
    {
        m_billboardSet.insert(&billboard);
    }
}

void crOptimizer::crFlattenStaticTransformsVisitor::apply(CRCore::crTransform& transform)
{
    if (!m_transformStack.empty())
    {
        // we need to disable any transform higher in the list.
        m_transformSet.insert(m_transformStack.back());
    }

    m_transformStack.push_back(&transform);

    

    // simple traverse the children as if this crTransform didn't exist.
    traverse(transform);

    m_transformStack.pop_back();
}

bool crOptimizer::crFlattenStaticTransformsVisitor::removeTransforms(CRCore::crNode* nodeWeCannotRemove)
{
    crCollectLowestTransformsVisitor cltv(m_optimizer);

    for(DrawableSet::iterator ditr=m_drawableSet.begin();
        ditr!=m_drawableSet.end();
        ++ditr)
    {
        cltv.collectDataFor(*ditr);
    }

    //for(BillboardSet::iterator bitr=m_billboardSet.begin();
    //    bitr!=m_billboardSet.end();
    //    ++bitr)
    //{
    //    cltv.collectDataFor(*bitr);
    //}
    
    cltv.setUpMaps();
    
    for(TransformSet::iterator titr=m_transformSet.begin();
        titr!=m_transformSet.end();
        ++titr)
    {
        cltv.disableTransform(*titr);
    }
    

    return cltv.removeTransforms(nodeWeCannotRemove);
}

////////////////////////////////////////////////////////////////////////////
// RemoveEmptyNodes.
////////////////////////////////////////////////////////////////////////////

void crOptimizer::crCombineStaticTransformsVisitor::apply(CRCore::crMatrixTransform& transform)
{
    if (transform.getDataVariance()==CRCore::crBase::STATIC &&
        transform.getNumChildren()==1 &&
        transform.getChild(0)->asTransform()!=0 &&
        transform.getChild(0)->asTransform()->asMatrixTransform()!=0 &&
        transform.getChild(0)->asTransform()->getDataVariance()==CRCore::crBase::STATIC)
    {
        m_transformSet.insert(&transform);
    }
    
    traverse(transform);
}

bool crOptimizer::crCombineStaticTransformsVisitor::removeTransforms(CRCore::crNode* nodeWeCannotRemove)
{
    if (nodeWeCannotRemove && nodeWeCannotRemove->asTransform()!=0 && nodeWeCannotRemove->asTransform()->asMatrixTransform()!=0)
    {
        // remove topmost node if it exists in the transform set.
        TransformSet::iterator itr = m_transformSet.find(nodeWeCannotRemove->asTransform()->asMatrixTransform());
        if (itr!=m_transformSet.end()) m_transformSet.erase(itr);
    }
    
    //CRCore::notify(CRCore::INFO)<<"Have found "<<m_transformSet.size()<<" static crTransform pairs to flatten"<<std::endl;
    
    bool transformRemoved = false;

    while (!m_transformSet.empty())
    {
        // get the first available transform to combine.
        CRCore::ref_ptr<CRCore::crMatrixTransform> transform = *m_transformSet.begin();
        m_transformSet.erase(m_transformSet.begin());
        
        if (transform->getNumChildren()==1 &&
            transform->getChild(0)->asTransform()!=0 &&
            transform->getChild(0)->asTransform()->asMatrixTransform()!=0 &&
            transform->getChild(0)->asTransform()->getDataVariance()==CRCore::crBase::STATIC)
        {
            // now combine with its child.
            CRCore::crMatrixTransform* child = transform->getChild(0)->asTransform()->asMatrixTransform();
            
            CRCore::crMatrix newMatrix = child->getMatrix()*transform->getMatrix();
            child->setMatrix(newMatrix);
            
            transformRemoved = true;

			CRCore::crNode::ParentArray parents = transform->getParents();
            for(CRCore::crNode::ParentArray::iterator pitr=parents.begin();
                pitr!=parents.end();
                ++pitr)
            {
                (*pitr)->replaceChild(transform.get(),child);
            }
            
        }
        
    }
    return transformRemoved;
}

////////////////////////////////////////////////////////////////////////////
// RemoveEmptyNodes.
////////////////////////////////////////////////////////////////////////////

void crOptimizer::crRemoveEmptyNodesVisitor::apply(CRCore::crObject& object)
{
    if (object.getNumParents()>0)
    {
		crGeometry *geometry;
		for(unsigned int i=0;i<object.getNumDrawables();++i)
		{
			geometry = object.getDrawable(i)->asGeometry();
            if(geometry && isDrawableEmpty(*geometry))
				object.removeDrawable(i);
		}
        if (object.getNumDrawables()==0 && (m_onlyCheckChild||isNodeEmpty(object))) m_redundantNodeList.insert(&object);
    }
}

void crOptimizer::crRemoveEmptyNodesVisitor::apply(CRCore::crGroup& group)
{
    if (group.getNumParents()>0)
    {
        // only remove empty groups, but not empty occluders.
        if (group.getNumChildren()==0 && (m_onlyCheckChild||(isNodeEmpty(group) && 
            (typeid(group)==typeid(CRCore::crGroup) || dynamic_cast<CRCore::crTransform*>(&group)))))
        {
            m_redundantNodeList.insert(&group);
        }
    }
    traverse(group);
}

void crOptimizer::crRemoveEmptyNodesVisitor::removeEmptyNodes()
{

    NodeList newEmptyGroups;

    // keep iterator through until scene graph is cleaned of empty nodes.
    while (!m_redundantNodeList.empty())
    {
        for(NodeList::iterator itr=m_redundantNodeList.begin();
            itr!=m_redundantNodeList.end();
            ++itr)
        {
            
            CRCore::ref_ptr<CRCore::crNode> nodeToRemove = (*itr);
            
            // take a copy of parents list since subsequent removes will modify the original one.
			CRCore::crNode::ParentArray parents = nodeToRemove->getParents();

            for(CRCore::crNode::ParentArray::iterator pitr=parents.begin();
                pitr!=parents.end();
                ++pitr)
            {
                CRCore::crGroup* parent = *pitr;
                //if (m_onlyCheckChild||(!dynamic_cast<CRCore::crSequence*>(parent) &&
                //    !dynamic_cast<CRCore::crSwitch*>(parent) && 
                //    !dynamic_cast<CRCore::crMultiSwitch*>(parent)))
                //{
                    parent->removeChild(nodeToRemove.get());
                    if (parent->getNumChildren()==0) newEmptyGroups.insert(*pitr);
                //}
            }                
        }

        m_redundantNodeList.clear();
        m_redundantNodeList.swap(newEmptyGroups);
    }
}


////////////////////////////////////////////////////////////////////////////
// RemoveRedundantNodes.
////////////////////////////////////////////////////////////////////////////

void crOptimizer::crRemoveRedundantNodesVisitor::apply(CRCore::crGroup& group)
{
    if (group.getNumParents()>0)
    {
		if(group.getDataVariance()==CRCore::crBase::STATIC && typeid(group)==typeid(CRCore::crGroup))
        //if (group.getNumChildren()==1 && typeid(group)==typeid(CRCore::crGroup))
        {
            if (group.getNumParents()>0 /*&& group.getNumChildren()<=1*/)
            {
                if (isNodeEmpty(group) && isOperationPermissableForObject(&group))
                {
                    m_redundantNodeList.insert(&group);
                }
            }
        }
    }
    traverse(group);
}

void crOptimizer::crRemoveRedundantNodesVisitor::apply(CRCore::crTransform& transform)
{
    if (transform.getNumParents()>0 && 
        transform.getDataVariance()==CRCore::crBase::STATIC &&
        isOperationPermissableForObject(&transform))
    {
        static CRCore::crMatrix identity;
        CRCore::crMatrix matrix;
        transform.computeWorldToLocalMatrix(matrix,NULL);
        if (matrix==identity)
        {
            m_redundantNodeList.insert(&transform);
        }
    }
    traverse(transform);
}


void crOptimizer::crRemoveRedundantNodesVisitor::removeRedundantNodes()
{

    for(NodeList::iterator itr=m_redundantNodeList.begin();
        itr!=m_redundantNodeList.end();
        ++itr)
    {
        CRCore::ref_ptr<CRCore::crGroup> group = dynamic_cast<CRCore::crGroup*>(*itr);
        if (group.valid())
        {
            // take a copy of parents list since subsequent removes will modify the original one.
			CRCore::crNode::ParentArray parents = group->getParents();
            
            if (group->getNumChildren()==1)
            {
                CRCore::crNode* child = group->getChild(0);
                for(CRCore::crNode::ParentArray::iterator pitr=parents.begin();
                    pitr!=parents.end();
                    ++pitr)
                {
                    (*pitr)->replaceChild(group.get(),child);
                }

            }
			CRCore::NodeArray &childArray = group->getChildArray();
			for(CRCore::crNode::ParentArray::iterator pitr=parents.begin();
				pitr!=parents.end();
				++pitr)
			{
				for(CRCore::NodeArray::iterator childItr=childArray.begin();
					childItr!=childArray.end();
					++childItr)
				{
				    (*pitr)->addChild((*childItr).get());
				}
				(*pitr)->removeChild(group.get());
			}
        }
        else
        {
            CRCore::notify(CRCore::WARN)<<"crOptimizer::crRemoveRedundantNodesVisitor::removeRedundantNodes() - failed dynamic_cast"<<std::endl;
        }                                
    }
    m_redundantNodeList.clear();
}



////////////////////////////////////////////////////////////////////////////
// combine crLod's.
////////////////////////////////////////////////////////////////////////////
void crOptimizer::crCombineLODsVisitor::apply(CRCore::crLod& lod)
{
    for(unsigned int i=0;i<lod.getNumParents();++i)
    {
        if (typeid(*lod.getParent(i))==typeid(CRCore::crGroup))
        {
            if (isOperationPermissableForObject(&lod))
            {
                m_groupList.insert(lod.getParent(i));
            }
        }
    }
    traverse(lod);
}

void crOptimizer::crCombineLODsVisitor::apply(CRCore::crPagedLOD& lod)
{
	traverse((crGroup&)lod);
}


void crOptimizer::crCombineLODsVisitor::combineLODs()
{
    for(GroupList::iterator itr=m_groupList.begin();
        itr!=m_groupList.end();
        ++itr)
    {
        CRCore::crGroup* group = *itr;

        typedef std::set<CRCore::crLod*>                 LODSet;

        LODSet    lodChildren;

        for(unsigned int i=0;i<group->getNumChildren();++i)
        {
            CRCore::crNode* child = group->getChild(i);
            CRCore::crLod* lod = dynamic_cast<CRCore::crLod*>(child);
            if (lod)
            {
                lodChildren.insert(lod);
            }
        }

        if (lodChildren.size()>=2)
        {
            CRCore::crBoundingBox bb;
            LODSet::iterator lod_itr;
            float smallestRadius=FLT_MAX;
            for(lod_itr=lodChildren.begin();
                lod_itr!=lodChildren.end();
                ++lod_itr)
            {
                float r = (*lod_itr)->getBound().radius();
                if (r>=0 && r<smallestRadius) smallestRadius = r;
                bb.expandBy((*lod_itr)->getCenter());
            }
            if (bb.radius()<smallestRadius*0.1f)
            {
                typedef std::pair<float,float> RangePair;
                typedef std::multimap<RangePair,CRCore::crNode*> RangeMap;
                RangeMap rangeMap;
                for(lod_itr=lodChildren.begin();
                    lod_itr!=lodChildren.end();
                    ++lod_itr)
                {

                    CRCore::crLod* lod = *lod_itr;
                    for(unsigned int i=0;i<lod->getNumRanges();++i)
                    {
                        rangeMap.insert(RangeMap::value_type(RangePair(lod->getMinRange(i),lod->getMaxRange(i)),lod->getChild(i)));
                    }

                }

                // create new crLod containing all other crLod's children.
                CRCore::crLod* newLOD = new CRCore::crLod;
                newLOD->setName("newLOD");
                newLOD->setCenter(bb.center());

                int i=0;
                for(RangeMap::iterator c_itr=rangeMap.begin();
                    c_itr!=rangeMap.end();
                    ++c_itr,++i)
                {
                    newLOD->setRange(i,c_itr->first.first,c_itr->first.second);
                    newLOD->addChild(c_itr->second);
                }

                // add crLod into parent.
                group->addChild(newLOD);

                // remove all the old crLod's from group.
                for(lod_itr=lodChildren.begin();
                    lod_itr!=lodChildren.end();
                    ++lod_itr)
                {
                    group->removeChild(*lod_itr);
                }

            }

        }
    }
    m_groupList.clear();
}

////////////////////////////////////////////////////////////////////////////
// code to merge geometry object which share, state, and attribute bindings.
////////////////////////////////////////////////////////////////////////////

struct LessGeometry
{
    bool operator() (const CRCore::crGeometry* lhs,const CRCore::crGeometry* rhs) const
    {		
		//if(lhs->getVertexBindMode()<rhs->getVertexBindMode()) return true;
		//if(rhs->getVertexBindMode()<lhs->getVertexBindMode()) return false;

		if(lhs->getStateSet()<rhs->getStateSet()) return true;
		if(rhs->getStateSet()<lhs->getStateSet()) return false;

		//return false;

        if (rhs->getVertexIndices()) { if (!lhs->getVertexIndices()) return true; }
        else if (lhs->getVertexIndices()) return false;
        
        if (lhs->getNormalBinding()<rhs->getNormalBinding()) return true;
        if (rhs->getNormalBinding()<lhs->getNormalBinding()) return false;

        if (rhs->getNormalIndices()) { if (!lhs->getNormalIndices()) return true; }
        else if (lhs->getNormalIndices()) return false;

        if (lhs->getColorBinding()<rhs->getColorBinding()) return true;
        if (rhs->getColorBinding()<lhs->getColorBinding()) return false;
        
        if (rhs->getColorIndices()) { if (!lhs->getColorIndices()) return true; }
        else if (lhs->getColorIndices()) return false;

        if (lhs->getSecondaryColorBinding()<rhs->getSecondaryColorBinding()) return true;
        if (rhs->getSecondaryColorBinding()<lhs->getSecondaryColorBinding()) return false;

        if (rhs->getSecondaryColorIndices()) { if (!lhs->getSecondaryColorIndices()) return true; }
        else if (lhs->getSecondaryColorIndices()) return false;

        if (lhs->getFogCoordBinding()<rhs->getFogCoordBinding()) return true;
        if (rhs->getFogCoordBinding()<lhs->getFogCoordBinding()) return false;

        if (rhs->getFogCoordIndices()) { if (!lhs->getFogCoordIndices()) return true; }
        else if (lhs->getFogCoordIndices()) return false;

        if (lhs->getNumTexCoordArrays()<rhs->getNumTexCoordArrays()) return true;
        if (rhs->getNumTexCoordArrays()<lhs->getNumTexCoordArrays()) return false;
    
        // therefore lhs->getNumTexCoordArrays()==rhs->getNumTexCoordArrays()
        
        unsigned int i;
        for(i=0;i<lhs->getNumTexCoordArrays();++i)
        {
            if (rhs->getTexCoordArray(i)) 
            {
                if (!lhs->getTexCoordArray(i)) return true;
            }
            else if (lhs->getTexCoordArray(i)) return false;

            if (rhs->getTexCoordIndices(i)) { if (!lhs->getTexCoordIndices(i)) return true; }
            else if (lhs->getTexCoordIndices(i)) return false;
        }
        
        for(i=0;i<lhs->getNumVertexAttribArrays();++i)
        {
            if (rhs->getVertexAttribArray(i)) 
            {
                if (!lhs->getVertexAttribArray(i)) return true;
            }
            else if (lhs->getVertexAttribArray(i)) return false;

            if (rhs->getVertexAttribIndices(i)) { if (!lhs->getVertexAttribIndices(i)) return true; }
            else if (lhs->getVertexAttribIndices(i)) return false;
        }
        
        
        if (lhs->getNormalBinding()==CRCore::crGeometry::BIND_OVERALL)
        {
            // assumes that the bindings and arrays are set up correctly, this
            // should be the case after running computeCorrectBindingsAndArraySizes();
            const CRCore::crVector3& lhs_normal = (*(lhs->getNormalArray()))[0];
            const CRCore::crVector3& rhs_normal = (*(rhs->getNormalArray()))[0];
            if (lhs_normal<rhs_normal) return true;            
            if (rhs_normal<lhs_normal) return false;            
        }
        
        if (lhs->getColorBinding()==CRCore::crGeometry::BIND_OVERALL)
        {
            const CRCore::Array* lhs_colorArray = lhs->getColorArray();
            const CRCore::Array* rhs_colorArray = rhs->getColorArray();
            if (lhs_colorArray->getType()<rhs_colorArray->getType()) return true;
            if (rhs_colorArray->getType()<lhs_colorArray->getType()) return false;
            switch(lhs_colorArray->getType())
            {
                case(CRCore::Array::Vec4ubArrayType):
                    if ((*static_cast<const CRCore::Vec4ubArray*>(lhs_colorArray))[0]<(*static_cast<const CRCore::Vec4ubArray*>(rhs_colorArray))[0]) return true;
                    if ((*static_cast<const CRCore::Vec4ubArray*>(rhs_colorArray))[0]<(*static_cast<const CRCore::Vec4ubArray*>(lhs_colorArray))[0]) return false;
                case(CRCore::Array::Vec3ArrayType):
                    if ((*static_cast<const CRCore::Vec3Array*>(lhs_colorArray))[0]<(*static_cast<const CRCore::Vec3Array*>(rhs_colorArray))[0]) return true;
                    if ((*static_cast<const CRCore::Vec3Array*>(rhs_colorArray))[0]<(*static_cast<const CRCore::Vec3Array*>(lhs_colorArray))[0]) return false;
                case(CRCore::Array::Vec4ArrayType):
                    if ((*static_cast<const CRCore::Vec4Array*>(lhs_colorArray))[0]<(*static_cast<const CRCore::Vec4Array*>(rhs_colorArray))[0]) return true;
                    if ((*static_cast<const CRCore::Vec4Array*>(rhs_colorArray))[0]<(*static_cast<const CRCore::Vec4Array*>(lhs_colorArray))[0]) return false;
                    break;
                default:
                    break;
            }
            
        }

        return false;

    }
};

struct LessGeometryPrimitiveType
{
    bool operator() (const CRCore::crGeometry* lhs,const CRCore::crGeometry* rhs) const
    {
        for(unsigned int i=0;
            i<lhs->getNumPrimitives() && i<rhs->getNumPrimitives();
            ++i)
        {
            if (lhs->getPrimitive(i)->getType()<rhs->getPrimitive(i)->getType()) return true;
            else if (rhs->getPrimitive(i)->getType()<lhs->getPrimitive(i)->getType()) return false;

            if (lhs->getPrimitive(i)->getMode()<rhs->getPrimitive(i)->getMode()) return true;
            else if (rhs->getPrimitive(i)->getMode()<lhs->getPrimitive(i)->getMode()) return false;

        }
        return lhs->getNumPrimitives()<rhs->getNumPrimitives();
    }
};

void crOptimizer::crCheckGeometryVisitor::checkObject(CRCore::crObject& object)
{
    if (isOperationPermissableForObject(&object))
    {
        for(unsigned int i=0;i<object.getNumDrawables();++i)
        {
            CRCore::crGeometry* geom = object.getDrawable(i)->asGeometry();
            if (geom && isOperationPermissableForObject(geom))
            {
                geom->computeCorrectBindingsAndArraySizes();
            }
        }
    }
}

bool crOptimizer::crMergeGeometryVisitor::mergeObject(CRCore::crObject& object)
{
    if (!isOperationPermissableForObject(&object)) return false;

    if (object.getNumDrawables()>=2)
    {
    
        typedef std::vector<CRCore::crGeometry*>                         DuplicateList;
        typedef std::map<CRCore::crGeometry*,DuplicateList,LessGeometry> GeometryDuplicateMap;

        GeometryDuplicateMap geometryDuplicateMap;
        unsigned int i;
        for(i=0;i<object.getNumDrawables();++i)
        {
            CRCore::crGeometry* geom = object.getDrawable(i)->asGeometry();
            if (geom)
            {
                if (!geometryContainsSharedArrays(*geom))
                {
                    geometryDuplicateMap[geom].push_back(geom);
                }
#if _DEBUG
				else
				{
					CRCore::notify(WARN)<<"Warning: in CRUtil::crOptimizer::crMergeGeometryVisitor::mergeObject "<<std::endl
						<<"         geometryContainsSharedArrays "<<std::endl;
				}
#endif
            }
        }

        // don't merge geometry if its above a maximum number of vertices.
        unsigned int _maximumNumber = 65536;//
        for(GeometryDuplicateMap::iterator itr=geometryDuplicateMap.begin();
            itr!=geometryDuplicateMap.end();
            ++itr)
        {
			if (itr->second.size()>1)
            {
                std::sort(itr->second.begin(),itr->second.end(),LessGeometryPrimitiveType());
                CRCore::crGeometry* lhs = itr->second[0];
                for(DuplicateList::iterator dupItr=itr->second.begin()+1;
                    dupItr!=itr->second.end();
                    ++dupItr)
                {
                
                    CRCore::crGeometry* rhs = *dupItr;
                    
                    if (lhs->getVertexArray() && lhs->getVertexArray()->getNumElements()>=_maximumNumber)
                    {
                        lhs = rhs;
                        continue;
                    }

                    if (rhs->getVertexArray() && rhs->getVertexArray()->getNumElements()>=_maximumNumber)
                    {
                        continue;
                    }
                    
                    if (lhs->getVertexArray() && rhs->getVertexArray() && 
                        (lhs->getVertexArray()->getNumElements()+rhs->getVertexArray()->getNumElements())>=_maximumNumber)
                    {
						lhs = rhs;
						continue;
                    }
                    
                    mergeGeometry(*lhs,*rhs);
                    {
						object.removeDrawable(rhs);
                        //static int co = 0;
                        //CRCore::notify(CRCore::INFO)<<"merged and removed crGeometry "<<++co<<std::endl;
                    }
                }
            }
        }
    }

    // convert all polygon primitives which has 3 indices into PT_TRIANGLES, 4 indices into PT_QUADS.
    unsigned int i;
    for(i=0;i<object.getNumDrawables();++i)
    {
        CRCore::crGeometry* geom = dynamic_cast<CRCore::crGeometry*>(object.getDrawable(i));
        if (geom)
        {
			CRCore::crGeometry::PrimitiveList& primitives = geom->getPrimitiveList();
            for(CRCore::crGeometry::PrimitiveList::iterator itr=primitives.begin();
                itr!=primitives.end();
                ++itr)
            {
                CRCore::crPrimitive* prim = itr->get();
                if (prim->getMode()==CRCore::crPrimitive::PT_POLYGON)
                {
                    if (prim->getNumIndices()==3)
                    {
                        prim->setMode(CRCore::crPrimitive::PT_TRIANGLES);
                    }
                    else if (prim->getNumIndices()==4)
                    {
                        prim->setMode(CRCore::crPrimitive::PT_QUADS);
                    }
                }
            }
        }
    }
    // now merge any compatible primtives.
    for(i=0;i<object.getNumDrawables();++i)
    {
        CRCore::crGeometry* geom = dynamic_cast<CRCore::crGeometry*>(object.getDrawable(i));
        if (geom)
        {
            if (geom->getNumPrimitives()>0 &&
                geom->getNormalBinding()!=CRCore::crGeometry::BIND_PER_PRIMITIVE_SET &&
                geom->getColorBinding()!=CRCore::crGeometry::BIND_PER_PRIMITIVE_SET &&
                geom->getSecondaryColorBinding()!=CRCore::crGeometry::BIND_PER_PRIMITIVE_SET &&
                geom->getFogCoordBinding()!=CRCore::crGeometry::BIND_PER_PRIMITIVE_SET)
            {
                CRCore::crGeometry::PrimitiveList& primitives = geom->getPrimitiveList();
                unsigned int primNo=0;
                while(primNo+1<primitives.size())
                {
                    CRCore::crPrimitive* lhs = primitives[primNo].get();
                    CRCore::crPrimitive* rhs = primitives[primNo+1].get();
 
                    bool combine = false;

                    if (lhs->getType()==rhs->getType() &&
                        lhs->getMode()==rhs->getMode())
                    {

                        switch(lhs->getMode())
                        {
                        case(CRCore::crPrimitive::PT_POINTS):
                        case(CRCore::crPrimitive::PT_LINES):
                        case(CRCore::crPrimitive::PT_TRIANGLES):
                        case(CRCore::crPrimitive::PT_QUADS):
                            combine = true;       
                            break;
                        }
                        
                    }
                    if (combine)
                    {
                        switch(lhs->getType())
                        {
                        case(CRCore::crPrimitive::DrawArraysPrimitiveType):
                            combine = mergePrimitive(*(static_cast<CRCore::DrawArrays*>(lhs)),*(static_cast<CRCore::DrawArrays*>(rhs)));
                            break;
                        case(CRCore::crPrimitive::DrawArrayLengthsPrimitiveType):
                            combine = mergePrimitive(*(static_cast<CRCore::DrawArrayLengths*>(lhs)),*(static_cast<CRCore::DrawArrayLengths*>(rhs)));
                            break;
                        case(CRCore::crPrimitive::DrawElementsUBytePrimitiveType):
                            combine = mergePrimitive(*(static_cast<CRCore::DrawElementsUByte*>(lhs)),*(static_cast<CRCore::DrawElementsUByte*>(rhs)));
                            break;
                        case(CRCore::crPrimitive::DrawElementsUShortPrimitiveType):
                            combine = mergePrimitive(*(static_cast<CRCore::DrawElementsUShort*>(lhs)),*(static_cast<CRCore::DrawElementsUShort*>(rhs)));
							break;
                        case(CRCore::crPrimitive::DrawElementsUIntPrimitiveType):
                            combine = mergePrimitive(*(static_cast<CRCore::DrawElementsUInt*>(lhs)),*(static_cast<CRCore::DrawElementsUInt*>(rhs)));
                            break;
                        default:
                            break;
                        }
                    }
                    if (combine)
                    {
                        primitives.erase(primitives.begin()+primNo+1);
                    }

                    if (!combine)
                    {
                        primNo++;
                    }
                }
            }
        }
    }

//    object.dirtyBound();
    return false;
}

bool crOptimizer::crMergeGeometryVisitor::geometryContainsSharedArrays(CRCore::crGeometry& geom)
{
    if (geom.getVertexArray() && geom.getVertexArray()->referenceCount()>1) return true;
    if (geom.getNormalArray() && geom.getNormalArray()->referenceCount()>1) return true;
    if (geom.getColorArray() && geom.getColorArray()->referenceCount()>1) return true;
    if (geom.getSecondaryColorArray() && geom.getSecondaryColorArray()->referenceCount()>1) return true;
    if (geom.getFogCoordArray() && geom.getFogCoordArray()->referenceCount()>1) return true;
    

    for(unsigned int unit=0;unit<geom.getNumTexCoordArrays();++unit)
    {
        CRCore::Array* tex = geom.getTexCoordArray(unit);
        if (tex && tex->referenceCount()>1) return true;
    }
    
    // shift the indices of the incomming primitives to account for the pre exisiting geometry.
    for(CRCore::crGeometry::PrimitiveList::iterator primItr=geom.getPrimitiveList().begin();
        primItr!=geom.getPrimitiveList().end();
        ++primItr)
    {
        if ((*primItr)->referenceCount()>1) return true;
    }
    
    
    return false;
}


class MergeArrayVisitor : public CRCore::ArrayVisitor
{
    public:
    
        CRCore::Array* m_lhs;
    
        MergeArrayVisitor() :
            m_lhs(0) {}
            
            
        void merge(CRCore::Array* lhs,CRCore::Array* rhs)
        {
            if (lhs==0 || rhs==0) return;
            if (lhs->getType()!=rhs->getType()) return;
            
            m_lhs = lhs;
            
            rhs->accept(*this);
        }
        
        template<typename T>
        void m_merge(T& rhs)
        {
            T* lhs = static_cast<T*>(m_lhs);
			//std::copy(rhs.begin(),rhs.end(),std::back_inserter(*lhs));
            lhs->insert(lhs->end(),rhs.begin(),rhs.end());
        }
        
            
        virtual void apply(CRCore::Array&) { CRCore::notify(CRCore::WARN) << "Warning: crOptimizer's MergeArrayVisitor cannot merge Array type." << std::endl; }
        virtual void apply(CRCore::ByteArray& rhs) { m_merge(rhs); }
        virtual void apply(CRCore::ShortArray& rhs) { m_merge(rhs); }
        virtual void apply(CRCore::IntArray& rhs) { m_merge(rhs); }
        virtual void apply(CRCore::UByteArray& rhs) { m_merge(rhs); }
        virtual void apply(CRCore::UShortArray& rhs) { m_merge(rhs); }
        virtual void apply(CRCore::UIntArray& rhs) { m_merge(rhs); }
        virtual void apply(CRCore::Vec4ubArray& rhs) { m_merge(rhs); }
        virtual void apply(CRCore::FloatArray& rhs) { m_merge(rhs); }
        virtual void apply(CRCore::Vec2Array& rhs) { m_merge(rhs); }
        virtual void apply(CRCore::Vec3Array& rhs) { m_merge(rhs); }
        virtual void apply(CRCore::Vec4Array& rhs) { m_merge(rhs); }
};

bool crOptimizer::crMergeGeometryVisitor::mergeGeometry(CRCore::crGeometry& lhs,CRCore::crGeometry& rhs)
{

    MergeArrayVisitor merger;
 
    unsigned int base = 0;
    if (lhs.getVertexArray() && rhs.getVertexArray())
    {

        base = lhs.getVertexArray()->getNumElements();

        merger.merge(lhs.getVertexArray(),rhs.getVertexArray());

    }
    else if (rhs.getVertexArray())
    {
        base = 0;
        lhs.setVertexArray(rhs.getVertexArray());
    }
    
    if (lhs.getVertexIndices() && rhs.getVertexIndices())
    {

        base = lhs.getVertexIndices()->getNumElements();
        merger.merge(lhs.getVertexIndices(),rhs.getVertexIndices());

    }
    else if (rhs.getVertexIndices())
    {
        base = 0;
        lhs.setVertexIndices(rhs.getVertexIndices());
    }
    

    if (lhs.getNormalArray() && rhs.getNormalArray() && lhs.getNormalBinding()!=CRCore::crGeometry::BIND_OVERALL)
    {
        merger.merge(lhs.getNormalArray(),rhs.getNormalArray());
    }
    else if (rhs.getNormalArray())
    {
        lhs.setNormalArray(rhs.getNormalArray());
    }

    if (lhs.getNormalIndices() && rhs.getNormalIndices() && lhs.getNormalBinding()!=CRCore::crGeometry::BIND_OVERALL)
    {
        merger.merge(lhs.getNormalIndices(),rhs.getNormalIndices());
    }
    else if (rhs.getNormalIndices())
    {
        lhs.setNormalIndices(rhs.getNormalIndices());
    }


    if (lhs.getColorArray() && rhs.getColorArray() && lhs.getColorBinding()!=CRCore::crGeometry::BIND_OVERALL)
    {
        merger.merge(lhs.getColorArray(),rhs.getColorArray());
    }
    else if (rhs.getColorArray())
    {
        lhs.setColorArray(rhs.getColorArray());
    }
    
    if (lhs.getColorIndices() && rhs.getColorIndices() && lhs.getColorBinding()!=CRCore::crGeometry::BIND_OVERALL)
    {
        merger.merge(lhs.getColorIndices(),rhs.getColorIndices());
    }
    else if (rhs.getColorIndices())
    {
        lhs.setColorIndices(rhs.getColorIndices());
    }

    if (lhs.getSecondaryColorArray() && rhs.getSecondaryColorArray() && lhs.getSecondaryColorBinding()!=CRCore::crGeometry::BIND_OVERALL)
    {
        merger.merge(lhs.getSecondaryColorArray(),rhs.getSecondaryColorArray());
    }
    else if (rhs.getSecondaryColorArray())
    {
        lhs.setSecondaryColorArray(rhs.getSecondaryColorArray());
    }
    
    if (lhs.getSecondaryColorIndices() && rhs.getSecondaryColorIndices() && lhs.getSecondaryColorBinding()!=CRCore::crGeometry::BIND_OVERALL)
    {
        merger.merge(lhs.getSecondaryColorIndices(),rhs.getSecondaryColorIndices());
    }
    else if (rhs.getSecondaryColorIndices())
    {
        lhs.setSecondaryColorIndices(rhs.getSecondaryColorIndices());
    }

    if (lhs.getFogCoordArray() && rhs.getFogCoordArray() && lhs.getFogCoordBinding()!=CRCore::crGeometry::BIND_OVERALL)
    {
        merger.merge(lhs.getFogCoordArray(),rhs.getFogCoordArray());
    }
    else if (rhs.getFogCoordArray())
    {
        lhs.setFogCoordArray(rhs.getFogCoordArray());
    }

    if (lhs.getFogCoordIndices() && rhs.getFogCoordIndices() && lhs.getFogCoordBinding()!=CRCore::crGeometry::BIND_OVERALL)
    {
        merger.merge(lhs.getFogCoordIndices(),rhs.getFogCoordIndices());
    }
    else if (rhs.getFogCoordIndices())
    {
        lhs.setFogCoordIndices(rhs.getFogCoordIndices());
    }


    unsigned int unit;
    for(unit=0;unit<lhs.getNumTexCoordArrays();++unit)
    {
        merger.merge(lhs.getTexCoordArray(unit),rhs.getTexCoordArray(unit));
        if (lhs.getTexCoordIndices(unit) && rhs.getTexCoordIndices(unit))
        {
            merger.merge(lhs.getTexCoordIndices(unit),rhs.getTexCoordIndices(unit));
        }
    }
    
    for(unit=0;unit<lhs.getNumVertexAttribArrays();++unit)
    {
        merger.merge(lhs.getVertexAttribArray(unit),rhs.getVertexAttribArray(unit));
        if (lhs.getVertexAttribIndices(unit) && rhs.getVertexAttribIndices(unit))
        {
            merger.merge(lhs.getVertexAttribIndices(unit),rhs.getVertexAttribIndices(unit));
        }
    }


    // shift the indices of the incomming primitives to account for the pre exisiting geometry.
    for(CRCore::crGeometry::PrimitiveList::iterator primItr=rhs.getPrimitiveList().begin();
        primItr!=rhs.getPrimitiveList().end();
        ++primItr)
    {
        CRCore::crPrimitive* primitive = primItr->get();
        switch(primitive->getType())
        {
        case(CRCore::crPrimitive::DrawElementsUBytePrimitiveType):
            {
                CRCore::DrawElementsUByte* primitiveUByte = static_cast<CRCore::DrawElementsUByte*>(primitive);
                unsigned int currentMaximum = 0;
				for(CRCore::DrawElementsUByte::PT_IndexArray::iterator eitr=primitiveUByte->getIndexArray().begin();
                    eitr!=primitiveUByte->getIndexArray().end();
                    ++eitr)
                {
                    currentMaximum = CRCore::maximum(currentMaximum,(unsigned int)*eitr);
                }
                if ((base+currentMaximum)>=65536)
                {
                    // must promote to a DrawElementsUInt
                    DrawElementsUInt* new_primitive = new DrawElementsUInt(primitive->getMode());
                    std::copy(primitiveUByte->getIndexArray().begin(),primitiveUByte->getIndexArray().end(),
						      std::back_inserter(new_primitive->getIndexArray()));
                    new_primitive->offsetIndices(base);
                    (*primItr) = new_primitive;
                } 
				else if ((base+currentMaximum)>=256)
                {
                    // must promote to a DrawElementsUShort
                    DrawElementsUShort* new_primitive = new DrawElementsUShort(primitive->getMode(),primitiveUByte->getNumIndices());
					DrawElementsUShort::PT_IndexArray &lhs = new_primitive->getIndexArray();
                    std::copy(primitiveUByte->getIndexArray().begin(),primitiveUByte->getIndexArray().end(),std::back_inserter(lhs));
                    new_primitive->offsetIndices(base);
                    (*primItr) = new_primitive;
                }
                else
                {
                    primitive->offsetIndices(base);
                }
            }
            break;

        case(CRCore::crPrimitive::DrawElementsUShortPrimitiveType):
            {
                DrawElementsUShort* primitiveUShort = static_cast<DrawElementsUShort*>(primitive);
                unsigned int currentMaximum = 0;
                for(DrawElementsUShort::PT_IndexArray::iterator eitr=primitiveUShort->getIndexArray().begin();
                    eitr!=primitiveUShort->getIndexArray().end();
                    ++eitr)
                {
                    currentMaximum = CRCore::maximum(currentMaximum,(unsigned int)*eitr);
                }
                if ((base+currentMaximum)>=65536)
                {
                    // must promote to a DrawElementsUInt
                    DrawElementsUInt* new_primitive = new DrawElementsUInt(primitive->getMode(),primitiveUShort->getNumIndices());
					DrawElementsUInt::PT_IndexArray &lhs = new_primitive->getIndexArray();
					std::copy(primitiveUShort->getIndexArray().begin(),primitiveUShort->getIndexArray().end(),std::back_inserter(lhs));
                    new_primitive->offsetIndices(base);
                    (*primItr) = new_primitive;
                }
                else
                {
                    primitive->offsetIndices(base);
                }
            }
            break;
        case(CRCore::crPrimitive::DrawElementsUIntPrimitiveType):
        case(CRCore::crPrimitive::DrawArraysPrimitiveType):
        case(CRCore::crPrimitive::DrawArrayLengthsPrimitiveType):
        default:
            primitive->offsetIndices(base);
            break;
        }

        
    }
    
    lhs.getPrimitiveList().insert(lhs.getPrimitiveList().end(),
                                     rhs.getPrimitiveList().begin(),rhs.getPrimitiveList().end());
	//crGeometry::PrimitiveList::iterator iter;
	//for(iter= rhs.getPrimitiveList().begin();iter!=rhs.getPrimitiveList().end();iter++)
	//{
	//	lhs.addPrimitive(iter->get());
	//}
	////
	//std::copy(rhs.getPrimitiveList().begin(),rhs.getPrimitiveList().end(),std::back_inserter(lhs.getPrimitiveList()));

    lhs.dirtyBound();
    lhs.dirtyDisplayList();
    
    return true;
}

bool crOptimizer::crMergeGeometryVisitor::mergePrimitive(DrawArrays& lhs,DrawArrays& rhs)
{
    if (lhs.getFirst()+lhs.getCount()==rhs.getFirst())
    {
        lhs.setCount(lhs.getCount()+rhs.getCount());
        return true;
    }
    return false;
}

bool crOptimizer::crMergeGeometryVisitor::mergePrimitive(DrawArrayLengths& lhs,DrawArrayLengths& rhs)
{
	DrawArrayLengths::PT_IndexArray &lhs_ia = lhs.getIndexArray();
	DrawArrayLengths::PT_IndexArray &rhs_ia = rhs.getIndexArray();
	int lhs_count = std::accumulate(lhs_ia.begin(),lhs_ia.end(),0);

    if (lhs.getFirst()+lhs_count==rhs.getFirst())
    {
        lhs_ia.insert(lhs_ia.end(),rhs_ia.begin(),rhs_ia.end());
		//std::copy(rhs_ia.begin(),rhs_ia.end(),std::back_inserter(lhs_ia));
        return true;
    }
    return false;
}

bool crOptimizer::crMergeGeometryVisitor::mergePrimitive(CRCore::DrawElementsUByte& lhs,CRCore::DrawElementsUByte& rhs)
{
	DrawElementsUByte::PT_IndexArray &lhs_ia = lhs.getIndexArray();
	DrawElementsUByte::PT_IndexArray &rhs_ia = rhs.getIndexArray(); 
	lhs_ia.insert(lhs_ia.end(),rhs_ia.begin(),rhs_ia.end());
	//std::copy(rhs_ia.begin(),rhs_ia.end(),std::back_inserter(lhs_ia));
    return true;
}

bool crOptimizer::crMergeGeometryVisitor::mergePrimitive(DrawElementsUShort& lhs,DrawElementsUShort& rhs)
{
	DrawElementsUShort::PT_IndexArray &lhs_ia = lhs.getIndexArray();
	DrawElementsUShort::PT_IndexArray &rhs_ia = rhs.getIndexArray();
	lhs_ia.insert(lhs_ia.end(),rhs_ia.begin(),rhs_ia.end());

    return true;
}

bool crOptimizer::crMergeGeometryVisitor::mergePrimitive(DrawElementsUInt& lhs,DrawElementsUInt& rhs)
{
	DrawElementsUInt::PT_IndexArray &lhs_ia = lhs.getIndexArray();
	DrawElementsUInt::PT_IndexArray &rhs_ia = rhs.getIndexArray();   
	lhs_ia.insert(lhs_ia.end(),rhs_ia.begin(),rhs_ia.end());    
	//std::copy(rhs_ia.begin(),rhs_ia.end(),std::back_inserter(lhs_ia));
    return true;
}

void crOptimizer::crMergeGeometryVisitor::deuceObject(CRCore::crObject& object)
{
	crObject::DrawableList& drawableList = object.getDrawableList();

	CRCore::crGeometry* geom;
	//unsigned int maximumNumber = 65536;//
	unsigned int minimumNumber = 64;//
    unsigned int size;
	//float multiple;
	//int deuce;
	//int i;
	for( crObject::DrawableList::iterator itr = drawableList.begin();
		 itr != drawableList.end();
		 ++itr )
	{
        geom = (*itr)->asGeometry();
		if(geom)
		{
            size = geom->getVertexArray()->getNumElements();
			if(size<minimumNumber)
			{
				geom->setUseVertexBufferObjects(false);
				geom->setUseIndexBufferObjects(false);
				geom->setUseDisplayList(false);
			}
			else
			{
				geom->setUseVertexBufferObjects(true);
				geom->setUseIndexBufferObjects(true);
			}
			//else if(size>maximumNumber)
			//{
   //             multiple = float(size) / float(65536);
			//	deuce = int(multiple);
			//	if(multiple>float(deuce))
			//	{
   //                 deuce++;
			//	}
			//	for( i = 0; i<deuce; i++)
			//	{
   //                 newgeom = geom->clone();
			//	}
			//}
		}
	}
	//将所有的DrawElementsUInt转换成DrawElementsUShort
}



////////////////////////////////////////////////////////////////////////////////////////////
//
//  Spatialize the scene to accelerate culling
//
void crOptimizer::crSpatializeGroupsVisitor::apply(CRCore::crFilterRenderManager& group)
{
}
void crOptimizer::crSpatializeGroupsVisitor::apply(CRCore::crCanvasNode& group)
{

}
void crOptimizer::crSpatializeGroupsVisitor::apply(CRCore::crWidgetNode& group)
{

}
void crOptimizer::crSpatializeGroupsVisitor::apply(CRCore::crGroup& group)
{
	std::string command = group.getDescriptionsInString();
	if(command.find("#Canvas")!=-1)
		return;
    if (typeid(group)==typeid(CRCore::crGroup) || group.asTransform())
    {
        if (isOperationPermissableForObject(&group))
        {
            _groupsToDivideList.insert(&group);
        }
    }

	traverse(group);
}

bool crOptimizer::crSpatializeGroupsVisitor::divide(unsigned int maxNumTreesPerCell)
{
    bool divided = false;
    for(GroupsToDivideList::iterator itr=_groupsToDivideList.begin();
        itr!=_groupsToDivideList.end();
        ++itr)
    {
        if (divide(*itr,maxNumTreesPerCell)) divided = true;
    }
    return divided;
}

bool crOptimizer::crSpatializeGroupsVisitor::divide(CRCore::crGroup* group, unsigned int maxNumTreesPerCell)
{
    if (group->getNumChildren()<=maxNumTreesPerCell) return false;
    
    // create the original box.
    CRCore::crBoundingBox bb;
    unsigned int i;
    for(i=0;i<group->getNumChildren();++i)
    {
        bb.expandBy(group->getChild(i)->getBound().center());
    }
    if(!bb.valid()) return false;
    float radius = bb.radius();
	if(radius==0) return false;
    float divide_distance = radius*0.7f;
    bool xAxis = (bb.xMax()-bb.xMin())>divide_distance;
    bool yAxis = (bb.yMax()-bb.yMin())>divide_distance;
    bool zAxis = (bb.zMax()-bb.zMin())>divide_distance;

   // CRCore::notify(CRCore::INFO)<<"Dividing "<<group->className()<<"  num children = "<<group->getNumChildren()<<"  xAxis="<<xAxis<<"  yAxis="<<yAxis<<"   zAxis="<<zAxis<<std::endl;
    
    unsigned int numChildrenOnEntry = group->getNumChildren();

    typedef std::pair< CRCore::crBoundingBox, CRCore::ref_ptr<CRCore::crGroup> > BoxGroupPair;
    typedef std::vector< BoxGroupPair > Boxes;
    Boxes boxes;
	crGroup *newGroup = new CRCore::crGroup;
	newGroup->setName("SpatializeNewGroups");
	boxes.push_back( BoxGroupPair(bb,newGroup) );

    // divide up on each axis    
    if (xAxis)
    {
        unsigned int numCellsToDivide=boxes.size();
        for(unsigned int i=0;i<numCellsToDivide;++i)
        {
            CRCore::crBoundingBox& orig_cell = boxes[i].first;
            CRCore::crBoundingBox new_cell = orig_cell;

            float xCenter = (orig_cell.xMin()+orig_cell.xMax())*0.5f;
            orig_cell.xMax() = xCenter;
            new_cell.xMin() = xCenter;

			newGroup = new CRCore::crGroup;
			newGroup->setName("SpatializeNewGroups");
            boxes.push_back(BoxGroupPair(new_cell,newGroup));
        }
    }

    if (yAxis)
    {
        unsigned int numCellsToDivide=boxes.size();
        for(unsigned int i=0;i<numCellsToDivide;++i)
        {
            CRCore::crBoundingBox& orig_cell = boxes[i].first;
            CRCore::crBoundingBox new_cell = orig_cell;

            float yCenter = (orig_cell.yMin()+orig_cell.yMax())*0.5f;
            orig_cell.yMax() = yCenter;
            new_cell.yMin() = yCenter;

			newGroup = new CRCore::crGroup;
			newGroup->setName("SpatializeNewGroups");
            boxes.push_back(BoxGroupPair(new_cell,newGroup));
        }
    }

    if (zAxis)
    {
        unsigned int numCellsToDivide=boxes.size();
        for(unsigned int i=0;i<numCellsToDivide;++i)
        {
            CRCore::crBoundingBox& orig_cell = boxes[i].first;
            CRCore::crBoundingBox new_cell = orig_cell;

            float zCenter = (orig_cell.zMin()+orig_cell.zMax())*0.5f;
            orig_cell.zMax() = zCenter;
            new_cell.zMin() = zCenter;

			newGroup = new CRCore::crGroup;
			newGroup->setName("SpatializeNewGroups");
            boxes.push_back(BoxGroupPair(new_cell,newGroup));
        }
    }


    // create the groups to drop the children into
    

    // bin each child into associated bb group
    typedef std::vector< CRCore::ref_ptr<CRCore::crNode> > NodeList;
    NodeList unassignedList;
    for(i=0;i<group->getNumChildren();++i)
    {
        bool assigned = false;
        CRCore::crVector3 center = group->getChild(i)->getBound().center();
        for(Boxes::iterator itr=boxes.begin();
            itr!=boxes.end() && !assigned;
            ++itr)
        {
            if (itr->first.contains(center))
            {
                // move child from main group into bb group.
                (itr->second)->addChild(group->getChild(i));
                assigned = true;
            }
        }
        if (!assigned)
        {
            unassignedList.push_back(group->getChild(i));
        }
    }


    // now transfer nodes across, by :
    //      first removing from the original group,
    //      add in the bb groups
    //      add then the unassigned children.
    
    
    // first removing from the original group,
    group->removeChild(0,group->getNumChildren());
    
    // add in the bb groups
    typedef std::vector< CRCore::ref_ptr<CRCore::crGroup> > GroupList;
    GroupList groupsToDivideList;
    for(Boxes::iterator itr=boxes.begin();
        itr!=boxes.end();
        ++itr)
    {
        // move child from main group into bb group.
        CRCore::crGroup* bb_group = (itr->second).get();
        if (bb_group->getNumChildren()>0)
        {
            if (bb_group->getNumChildren()==1)
            {
                group->addChild(bb_group->getChild(0));
            }
            else
            {
                group->addChild(bb_group);
                if (bb_group->getNumChildren()>maxNumTreesPerCell)
                {
                    groupsToDivideList.push_back(bb_group);
                }
            }
        }
    }
    
    
    // add then the unassigned children.
    for(NodeList::iterator nitr=unassignedList.begin();
        nitr!=unassignedList.end();
        ++nitr)
    {
        group->addChild(nitr->get());
    }

    // now call divide on all groups that require it.
    for(GroupList::iterator gitr=groupsToDivideList.begin();
        gitr!=groupsToDivideList.end();
        ++gitr)
    {
        divide(gitr->get(),maxNumTreesPerCell);
    }

    return (numChildrenOnEntry<group->getNumChildren());   
}


////////////////////////////////////////////////////////////////////////////////////////////
//
//  Spatialize the scene to accelerate culling
//

void crOptimizer::crCopySharedSubgraphsVisitor::apply(CRCore::crNode& node)
{
    if (node.getNumParents()>1 && !isOperationPermissableForObject(&node))
    {
        m_sharedNodeList.insert(&node);
    }
    traverse(node);
}

void crOptimizer::crCopySharedSubgraphsVisitor::copySharedNodes()
{
    //CRCore::notify(CRCore::INFO)<<"Shared node "<<m_sharedNodeList.size()<<std::endl;
    for(SharedNodeList::iterator itr=m_sharedNodeList.begin();
        itr!=m_sharedNodeList.end();
        ++itr)
    {
        //CRCore::notify(CRCore::INFO)<<"   No parents "<<(*itr)->getNumParents()<<std::endl;
        CRCore::crNode* node = *itr;
        for(unsigned int i=node->getNumParents()-1;i>0;--i)
        {
            // create a clone.
            CRCore::ref_ptr<CRCore::crBase> new_object = node->clone(CRCore::crCopyOp::DEEP_COPY_NODES |
                                                          CRCore::crCopyOp::DEEP_COPY_DRAWABLES);
            // cast it to node.                                              
            CRCore::crNode* new_node = dynamic_cast<CRCore::crNode*>(new_object.get());

            // replace the node by new_new 
            if (new_node) node->getParent(i)->replaceChild(node,new_node);
        }
    } 
}

////////////////////////////////////////////////////////////////////////////
// Unite Nodes.
////////////////////////////////////////////////////////////////////////////
void crOptimizer::crUniteNodesVisitor::reset()
{
	m_groups.clear();
	m_objects.clear();
	m_billboards.clear();
}
void crOptimizer::crUniteNodesVisitor::apply(CRCore::crGroup& group)
{
//	if(typeid(*&group) != typeid(CRCore::crGroup)) traverse(group);
//    if (group.getDataVariance()!=CRCore::crBase::STATIC) traverse(group);

	crGroup *pGroup = NULL;
    crObject *pObj = NULL;
	crBillboard *pBillboard = NULL;
	CRCore::NodeArray &childArray = group.getChildArray();
	//ref_ptr<crGroup> tempGroup = new crGroup;
	for(CRCore::NodeArray::iterator itr=childArray.begin();
		itr!=childArray.end();
		++itr)
	{
		//if ((*itr)->getDataVariance()==CRCore::crBase::STATIC) 
		{   
			//if (isOperationPermissableForObject(itr->get()))
			{
				if((*itr)->asGroup())
				{
					if(typeid(**itr)!=typeid(CRCore::crGroup)/*!(*itr)->isSameKindAs(tempGroup.get())*/) 
						continue;
					pGroup = dynamic_cast<crGroup*>(itr->get());
					if(pGroup)
					    m_groups[pGroup].push_back(pGroup);  //group级的合并
				}
				else
				{
					pObj = dynamic_cast<crObject *>(itr->get());
					if(!pObj) continue;
                    pBillboard = dynamic_cast<crBillboard *>(pObj);
					if(!pBillboard) m_objects[pObj].push_back(pObj);
					else m_billboards[pBillboard].push_back(pBillboard);
				}
			}
		}
	}
    optimize();
    reset();
    traverse(group);
}

void crOptimizer::crUniteNodesVisitor::apply(CRCore::crSwitch& node)
{//孩子不可合并，但可以合并其孩子的孩子
	//for(NodeArray::iterator itr=node.getChildArray().begin();
	//	itr!=node.getChildArray().end();
	//	++itr)
	//{
	//	traverse(**itr);
	//}
}
void crOptimizer::crUniteNodesVisitor::apply(CRCore::crSequence& node)
{
	//for(NodeArray::iterator itr=node.getChildArray().begin();
	//	itr!=node.getChildArray().end();
	//	++itr)
	//{
	//	traverse(**itr);
	//}
}
void crOptimizer::crUniteNodesVisitor::apply(CRCore::crLod& node)
{
	//for(NodeArray::iterator itr=node.getChildArray().begin();
	//	itr!=node.getChildArray().end();
	//	++itr)
	//{
	//	traverse(**itr);
	//}
	apply((crGroup&)node);
}
//void crOptimizer::crUniteNodesVisitor::apply(CRCore::crEventGroup& node)
//{
	//for(NodeArray::iterator itr=node.getChildArray().begin();
	//	itr!=node.getChildArray().end();
	//	++itr)
	//{
	//	traverse(**itr);
	//}
//}

void crOptimizer::crUniteNodesVisitor::optimize()
{
	CRCore::crNode::ParentArray::iterator pitr;

	if(!m_groups.empty())
	{
		GroupDuplicateMap::iterator gitr=m_groups.begin();
		CRCore::crNode::ParentArray &gParents = gitr->first->getParents();
		GroupDuplicateList::iterator gditr; 

		for( ; gitr!=m_groups.end(); gitr++ )
		{
			if (gitr->second.size()>1)
			{
				gditr=gitr->second.begin();
				for(gditr++;gditr!=gitr->second.end();gditr++)
				{//合并group
					for( int i=0; i<(*gditr)->getNumChildren(); i++)
					{
						gitr->first->addChild((*gditr)->getChild(i));
					}
					for( pitr=gParents.begin(); pitr!=gParents.end(); pitr++ )
					{
						(*pitr)->removeChild(*gditr);

						//if(isNodeEmpty(*pitr))

					}
				}
			}
		} 
	}
	if(!m_objects.empty())
	{
		ObjectDuplicateMap::iterator oitr=m_objects.begin();
		CRCore::crNode::ParentArray &oParents = oitr->first->getParents();
		ObjectDuplicateList::iterator oditr; 

		for( ; oitr!=m_objects.end(); oitr++ )
		{
			if (oitr->second.size()>1)
			{
				oditr=oitr->second.begin();
				for(oditr++;oditr!=oitr->second.end();oditr++)
				{//合并object
					for( int i=0; i<(*oditr)->getNumDrawables(); i++)
					{
						oitr->first->addDrawable((*oditr)->getDrawable(i));
					}
					for( pitr=oParents.begin(); pitr!=oParents.end(); pitr++ )
					{
						(*pitr)->removeChild(*oditr);
					}
				}
			}
		} 
	}
	if(!m_billboards.empty())
	{
		BillboardDuplicateMap::iterator bitr=m_billboards.begin();
		CRCore::crNode::ParentArray &bParents = bitr->first->getParents();
		BillboardDuplicateList::iterator bditr; 

		for( ; bitr!=m_billboards.end(); bitr++ )
		{
			if (bitr->second.size()>1)
			{
				bditr=bitr->second.begin();
				for(bditr++;bditr!=bitr->second.end();bditr++)
				{//合并Billboard
			/*		for( int i=0; i<(*bditr)->getNumDrawables(); i++)
					{
						bitr->first->addDrawable((*bditr)->getDrawable(i));
					}*/
					if(bitr->first->expand(**bditr))
					{
						for( pitr=bParents.begin(); pitr!=bParents.end(); pitr++ )
						{
							(*pitr)->removeChild(*bditr);
						}
					}
				}
			}
		} 
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
//
//  Set the attributes of textures up.
//


void crOptimizer::crTextureVisitor::apply(CRCore::crNode& node)
{
    
    CRCore::crStateSet* ss = node.getStateSet();
    if (ss)
    {
        apply(*ss);
    }

    traverse(node);
}

void crOptimizer::crTextureVisitor::apply(CRCore::crObject& geode)
{
    CRCore::crStateSet* ss = geode.getStateSet();
    
    if (ss)
    {
        apply(*ss);
    }

    for(unsigned int i=0;i<geode.getNumDrawables();++i)
    {
        CRCore::crDrawable* drawable = geode.getDrawable(i);
        if (drawable)
        {
            ss = drawable->getStateSet();
            if (ss)
            {
                apply(*ss);
            }
        }
    }
}

void crOptimizer::crTextureVisitor::apply(CRCore::crTraverseStringNode& node)
{
	std::string str = getTraverseString();
	setTraverseString(node.getName());
	const std::string &ts = getTraverseString();
	CRCore::crShaderManager::Callback *callback = 0;
	if(!ts.empty())
	{
		callback = CRCore::crShaderManager::getInstance()->getCallback(ts);
	}
	m_removeTexEnv = callback!=0;
	apply((crNode&)node);
	setTraverseString(str);
	callback = 0;
	if(!str.empty())
	{
		callback = CRCore::crShaderManager::getInstance()->getCallback(str);
	}
	m_removeTexEnv = callback!=0;
}

void crOptimizer::crTextureVisitor::apply(CRCore::crStateSet& stateset)
{
    for(unsigned int i=0;i<stateset.getTextureAttributeList().size();++i)
    {
		if (m_removeTexEnv)
		{
			stateset.removeTextureAttribute(i,crStateAttribute::TEXENV);
			stateset.removeTextureAttribute(i,crStateAttribute::TEXENVFILTER);
		}
        CRCore::crStateAttribute* sa = stateset.getTextureAttribute(i,CRCore::crStateAttribute::TEXTURE);
        CRCore::crTexture* texture = dynamic_cast<CRCore::crTexture*>(sa);
        if (texture)
        {
            apply(*texture);
        }
    }
}
void crOptimizer::crTextureVisitor::setChangeFilter(unsigned short minfilter,unsigned short magfilter)
{
	m_min_filter = minfilter;
	m_mag_filter = magfilter;
	m_changeFilter = true;
}
void crOptimizer::crTextureVisitor::setChangeWrap(CRCore::crTexture::WrapMode mode)
{
	m_wrapMode = mode;
	m_changeWrap = true;
}
void crOptimizer::crTextureVisitor::apply(CRCore::crTexture& texture)
{
    if (m_changeAutoUnRef)
    {
        texture.setUnRefImageDataAfterApply(m_valueAutoUnRef);
    }
    
    if (m_changeClientImageStorage)
    {
        texture.setClientStorageHint(m_valueClientImageStorage);
    }
    
    if (m_changeAnisotropy)
    {
        texture.setMaxAnisotropy(m_valueAnisotropy);
    }

	if(m_changeFilter)
	{
		texture.setFilter(crTexture2D::MIN_FILTER, (crTexture::FilterMode)m_min_filter);
		texture.setFilter(crTexture2D::MAG_FILTER, (crTexture::FilterMode)m_mag_filter);
	}
	if(m_changeWrap)
	{
		texture.setWrap(crTexture::WRAP_S,m_wrapMode);
		texture.setWrap(crTexture::WRAP_T,m_wrapMode);
		texture.setWrap(crTexture::WRAP_R,m_wrapMode);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
//
//  Check Physics
//
/////////////////////////////////////////////////////////////////////////

void crOptimizer::crCheckPhysicsVisitor::apply(CRPhysics::crMatterObject& node)
{
	int int1;
	if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"StaticMatterObject",int1))
	{
		node.setStaticMatterObject(int1);
	}

	const CRCore::crBoundingBox &bbox = node.getBoundBox();
	crVector3f center = bbox.center();
	crVector3f origin(0,0,0);

	if(center!=origin)
	{
	    CRCore::ref_ptr<RefMatrix> matrix = new RefMatrix;
		crMatrix mat = crMatrix::translate(-center[0],-center[1],-center[2]);;
		matrix->preMult(mat);

		if(!m_matrixList.empty())
			matrix->postMult(*(m_matrixList.back().get()));

		pushMatrix(matrix.get());

		traverse(node); 

		node.setInitPosition(center);

		popMatrix();
	}
	else
		traverse(node); 
}

void crOptimizer::crCheckPhysicsVisitor::apply(CRCore::crGroup& node)
{
	int needTranslate = 0;
	if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"NeedTranslateToCenter",needTranslate) && needTranslate)
	{
		const CRCore::crBoundingBox &bbox = node.getBoundBox();
		crVector3f center = bbox.center();
		crVector3f origin(0,0,0);

		if(center!=origin)
		{
			CRCore::ref_ptr<RefMatrix> matrix = new RefMatrix;
			crMatrix mat = crMatrix::translate(-center[0],-center[1],-center[2]);;
			matrix->preMult(mat);

			if(!m_matrixList.empty())
				matrix->postMult(*(m_matrixList.back().get()));

			pushMatrix(matrix.get());

			traverse(node); 

			popMatrix();
		}
	}
	else
		traverse(node); 
}

void crOptimizer::crCheckPhysicsVisitor::apply(CRCore::crObject& node)
{//将模型居中
	if( m_matrixList.empty() ) return;
	
	crMatrix &mat = *(m_matrixList.back().get());

	Vec3Array *vertexArray = NULL;
	for(CRCore::crObject::DrawableList::iterator itr = node.getDrawableList().begin();
		itr != node.getDrawableList().end();
		++itr)
	{
		if(dynamic_cast<CRCore::crGeometry *>(itr->get()))
		{
			crGeometry *geometry = dynamic_cast<CRCore::crGeometry *>(itr->get());
			vertexArray = (Vec3Array*)(geometry->getVertexArray());
            
			for( Vec3Array::iterator vitr = vertexArray->begin();
				 vitr != vertexArray->end();
				 ++vitr )
			{
				*vitr = *vitr * mat;
			}
		}
		else if(dynamic_cast<CRCore::crShapeDrawable *>(itr->get()))
		{

		}

		(*itr)->dirtyBound();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
//
//  最初的检查矫正
//

void crOptimizer::crInitialCheckVisitor::apply(CRCore::crObject& node)
{
	int int1;
	if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"EnableCalcShadow",int1))
	{
		node.setCalcShadow(int1);
	}

	if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"EnableVisiable",int1))
	{
		node.setVisiable(int1);
	}
	if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"EnableIntersect",int1))
	{
		node.setEnableIntersect(int1);
	}
	if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"EnableRenderable",int1))
	{
		node.setRenderable(int1);
	}
	if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"EnableCulling",int1))
	{
		node.setCullingActive(int1);
	}
	if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"AvoidIntersect",int1))
	{
		node.setAvoidIntersect(int1);
	}
	//node.setEnableIntersect(m_enableIntersect);
}

void crOptimizer::crInitialCheckVisitor::apply(CRCore::crGroup& node)
{
   int int1;
   if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"EnableCalcShadow",int1))
   {
        node.setCalcShadow(int1);
   }

   if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"EnableVisiable",int1))
   {
	   node.setVisiable(int1);
   }

   if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"EnableIntersect",int1))
   {
	   node.setEnableIntersect(int1);
   }
   if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"EnableRenderable",int1))
   {
	   node.setRenderable(int1);
   }
   if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"EnableCulling",int1))
   {
	   node.setCullingActive(int1);
   }
   if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"AvoidIntersect",int1))
   {
	   node.setAvoidIntersect(int1);
   }
	//m_enableIntersect = node.getEnableIntersect();
	//if(!m_enableIntersect)
	//{
	//	for( NodeArray::iterator itr = node.getChildArray().begin();
	//		 itr != node.getChildArray().end();
	//		 ++itr )
	//	{
	//		(*itr)->setEnableIntersect(false);
	//	}
	//}
    
	traverse(node);
	//m_enableIntersect = true;
}

//CupMap特效渲染，预渲染处理
//crBeforehandRenderVisitor

void crAutoTextureMatrix::apply(CRCore::crState &state) const
{
	glMatrixMode(GL_TEXTURE);

	if (m_active) 
	{
		CRCore::crMatrix M = state.getInitialViewMatrix();
		M(3, 0) = 0; M(3, 1) = 0; M(3, 2) = 0;
		M(3, 3) = 1; M(0, 3) = 0; M(1, 3) = 0;
		M(2, 3) = 0;

		CRCore::crVector4 lightvec;
		glGetLightfv(GL_LIGHT0+m_lightnum, GL_POSITION, lightvec._v);

		CRCore::crVector3 eye_light_ref = CRCore::crVector3(0, 0, 1) * M;

		CRCore::crMatrix LM = CRCore::crMatrix::rotate(
			CRCore::crVector3(lightvec.x(), lightvec.y(), lightvec.z()),
			eye_light_ref);

		glLoadMatrix((LM * CRCore::crMatrix::inverse(M)).ptr());

	} 
	else 
	{
		glLoadIdentity();
	}

	glMatrixMode(GL_MODELVIEW);
}

void crOptimizer::crBeforehandRenderVisitor::apply(CRCore::crDB& node)
{
	m_currentDB = &node;
	traverse(node);
}

void crOptimizer::crBeforehandRenderVisitor::apply(CRCore::crNode& node)
{
	std::string fxstr;

	if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"BeforehandRender",fxstr))
	{
		if(fxstr.compare("cubemap")==0)
		{
			createCubeMap(node);
		}
		else if(fxstr.compare("fog")==0)
		{
			if(!crDisplaySettings::instance()->getHDR())
			    createFog(node);
		}
		else if(fxstr.compare("SpecularHighlights")==0)
		{
			createSpecularHighlights(node);
		}	
	}

	traverse(node);
}

void crOptimizer::crBeforehandRenderVisitor::createCubeMap(CRCore::crNode& node)
{//2005.3.8 全面去除crResource
	////crResource *resource = CRCore::crBrain::getInstance()->getResource();
	std::string strbuf;
	crStateSet *ss;
	int cubemapUnit = 3;
	CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"cubemapUnit",cubemapUnit);
	ss = node.getOrCreateStateSet();
	crTextureCubeMap *cubemap = 
		dynamic_cast<crTextureCubeMap *>(ss->getTextureAttribute(cubemapUnit,crStateAttribute::TEXTURE));
	if(!cubemap)
	{
		if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"cubemapImageName",strbuf))
		{
			cubemap = new CRCore::crTextureCubeMap();
			cubemap->setImageNameID(strbuf);
			CRCore::crTextureCubeMap *texCubeMap = CRCore::crBrain::getInstance()->getTextureCubeMap(strbuf);
			bool needReloadImage =false;
			if(texCubeMap)
			{
				if(CRCore::crBrain::getInstance()->getNeedLoadImageCubeMap(texCubeMap))
				{
					for(int face = 0; face < 6; face++)
					{
						if(cubemap->getImage(face)&&cubemap->getImage(face)->data())
							texCubeMap->setImage(face,cubemap->getImage(face));
						else 
						{
							needReloadImage = true;
							break;
						}
					}
					if(needReloadImage)
						texCubeMap->getImageDataRequestHandler()->requestCubeImageFile(strbuf,texCubeMap);
				}

				cubemap = texCubeMap;
			}
			else
			{
				for(int face = 0; face < 6; face++)
				{
					if(!(cubemap->getImage(face)&&cubemap->getImage(face)->data()))
					{
						needReloadImage = true;
						break;
					}
				}
				if(needReloadImage)
					cubemap->getImageDataRequestHandler()->requestCubeImageFile(strbuf,cubemap);

				CRCore::crBrain::getInstance()->insertTextureCubeMap(strbuf,cubemap);
			}
		}
		ss->setTextureAttributeAndModes(cubemapUnit, cubemap,CRCore::crStateAttribute::ON);
	}
	crTexGen *texgen = 
		dynamic_cast<crTexGen *>(ss->getTextureAttribute(cubemapUnit,crStateAttribute::TEXGEN));
	if(!texgen)
	{
		texgen = new crTexGen;
		CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"TexGen",strbuf);
		if(strbuf.compare("REFLECTION_MAP")==0)
		{
			texgen->setMode(crTexGen::REFLECTION_MAP);
		}
		else if(strbuf.compare("NORMAL_MAP")==0)
		{
			texgen->setMode(crTexGen::NORMAL_MAP);
		}
		else if(strbuf.compare("SPHERE_MAP")==0)
		{
			texgen->setMode(crTexGen::SPHERE_MAP);
		}
		ss->setTextureAttributeAndModes(cubemapUnit,texgen,CRCore::crStateAttribute::ON);
	}
	crTexEnv *texenv = 
		dynamic_cast<crTexEnv *>(ss->getTextureAttribute(cubemapUnit,crStateAttribute::TEXENV));
	if(!texenv)
	{
		texenv = new crTexEnv;
		CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"TexEnv",strbuf);
		if(strbuf.compare("ADD")==0)
		{
			texenv->setMode(crTexEnv::ADD);
		}
		else if(strbuf.compare("MODULATE")==0)
		{
			texenv->setMode(crTexEnv::MODULATE);
		}
		else if(strbuf.compare("BLEND")==0)
		{
			texenv->setMode(crTexEnv::BLEND);
		}
		else if(strbuf.compare("REPLACE")==0)
		{
			texenv->setMode(crTexEnv::REPLACE);
		}
		else if(strbuf.compare("DECAL")==0)
		{
			texenv->setMode(crTexEnv::DECAL);
		}
		ss->setTextureAttributeAndModes(cubemapUnit,texenv,CRCore::crStateAttribute::ON);
	}
}

void crOptimizer::crBeforehandRenderVisitor::createFog(CRCore::crNode& node)
{
	std::string strbuf;
    float flt1;
	crVector4f vc4;
	crStateSet *ss = node.getOrCreateStateSet();
	crFog *fog = new crFog;
    if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"mode",strbuf))
	{
		if(strbuf.compare("LINEAR")==0)
		{
			fog->setMode(crFog::LINEAR);
		}
		else if(strbuf.compare("EXP")==0)
		{
			fog->setMode(crFog::EXP);
		}
		else if(strbuf.compare("EXP2")==0)
		{
			fog->setMode(crFog::EXP2);
		}
	}

    if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"density",flt1))
	{
		fog->setDensity(flt1);
	}

	if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"start",flt1))
	{
		fog->setStart(flt1);
	}

	if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"end",flt1))
	{
		fog->setEnd(flt1);
	}

	if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"color",vc4))
	{
		fog->setColor(vc4);
	}

	if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"fogCoordinateSource",strbuf))
	{
		if(strbuf.compare("FRAGMENT_DEPTH")==0)
			fog->setFogCoordinateSource(crFog::FRAGMENT_DEPTH);
		else if(strbuf.compare("FOG_COORDINATE")==0)
			fog->setFogCoordinateSource(crFog::FOG_COORDINATE);
	}

	ss->setAttributeAndModes(fog,CRCore::crStateAttribute::ON);
}

void crOptimizer::crBeforehandRenderVisitor::createSpecularHighlights(CRCore::crNode& node)
{
	int light_num,texture_unit,specular_exponent;
	crVector4 specular_color;
	CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"light_num",light_num);
	CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"texture_unit",texture_unit);
	CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"specular_exponent",specular_exponent);
	CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"specular_color",specular_color);
    
	CRCore::ref_ptr<CRCore::crStateSet> ss = node.getOrCreateStateSet();

	ss->setTextureAttributeAndModes(texture_unit, new crAutoTextureMatrix(light_num),CRCore::crStateAttribute::ON);

	CRCore::ref_ptr<CRUtil::crHighlightMapGenerator> hmg = new CRUtil::crHighlightMapGenerator(CRCore::crVector3(0, 0, -1), specular_color, specular_exponent);
	hmg->generateMap(false);

	CRCore::ref_ptr<CRCore::crTextureCubeMap> texture = new CRCore::crTextureCubeMap;
	texture->setImage(CRCore::crTextureCubeMap::POSITIVE_X, hmg->getImage(CRCore::crTextureCubeMap::POSITIVE_X));
	texture->setImage(CRCore::crTextureCubeMap::POSITIVE_Y, hmg->getImage(CRCore::crTextureCubeMap::POSITIVE_Y));
	texture->setImage(CRCore::crTextureCubeMap::POSITIVE_Z, hmg->getImage(CRCore::crTextureCubeMap::POSITIVE_Z));
	texture->setImage(CRCore::crTextureCubeMap::NEGATIVE_X, hmg->getImage(CRCore::crTextureCubeMap::NEGATIVE_X));
	texture->setImage(CRCore::crTextureCubeMap::NEGATIVE_Y, hmg->getImage(CRCore::crTextureCubeMap::NEGATIVE_Y));
	texture->setImage(CRCore::crTextureCubeMap::NEGATIVE_Z, hmg->getImage(CRCore::crTextureCubeMap::NEGATIVE_Z));
	texture->setWrap(CRCore::crTexture::WRAP_S, CRCore::crTexture::CLAMP_TO_EDGE);
	texture->setWrap(CRCore::crTexture::WRAP_T, CRCore::crTexture::CLAMP_TO_EDGE);
	texture->setWrap(CRCore::crTexture::WRAP_R, CRCore::crTexture::CLAMP_TO_EDGE);
	ss->setTextureAttributeAndModes(texture_unit, texture.get(), CRCore::crStateAttribute::ON);

	CRCore::ref_ptr<CRCore::crTexGen> texgen = new CRCore::crTexGen;
	texgen->setMode(CRCore::crTexGen::REFLECTION_MAP);
	ss->setTextureAttributeAndModes(texture_unit, texgen.get(), CRCore::crStateAttribute::ON);

	CRCore::ref_ptr<CRCore::crTexEnv> texenv = new CRCore::crTexEnv;
	texenv->setMode(CRCore::crTexEnv::ADD);
	ss->setTextureAttributeAndModes(texture_unit, texenv.get(), CRCore::crStateAttribute::ON);
}

////crObjectCommandBuildVisitor
void crOptimizer::crObjectCommandBuildVisitor::apply(CRCore::crObject& node)
{
	for( StringList::iterator itr = m_currentObjCommandList.begin();
        itr != m_currentObjCommandList.end();
		++itr )
	{
		node.addDescription(*itr);
	}

	traverse(node);
}

void crOptimizer::crObjectCommandBuildVisitor::apply(CRCore::crGroup& node)
{
	std::string str = node.getDescriptionsInString();
	std::string objStr = str;
	if(str.empty())
	{
		traverse(node);
		return;
	}

	int i = str.find("#ObjectCommandString");
	if(i<0) 
	{
		traverse(node);
		return;
	}
	int keylen = strlen("#ObjectCommandString");
	objStr.erase(0,i+keylen+1);
	int j = objStr.find("#ObjectCommandStringEnd");
	if(j>-1)
		objStr.erase(j,objStr.size());
	else
	{
        traverse(node);
		return;
	}

    keylen = strlen("#ObjectCommandStringEnd");
	str.erase(i,j+keylen+1-i);
    node.setDescriptionsInString(str);

	m_currentObjCommandList.push_back(objStr);

	traverse(node);
    
	m_currentObjCommandList.pop_back();
}
///////////////////////////////////////////
//crNodeCommandParseVisitor
///////////////////////////////////////////
void crOptimizer::crNodeCommandParseVisitor::apply(CRPhysics::crMatterObject& node)
{
	int int1;
	if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"StaticMatterObject",int1))
	{
		node.setStaticMatterObject(int1);
	}

	apply((CRCore::crGroup&) node);
}

void crOptimizer::crNodeCommandParseVisitor::apply(CRCore::crSequence& node)
{////SequenceStateSet的要求是模型相同,StateSet可以不相同
	//CRCore::notify(CRCore::ALWAYS)<<"crNodeCommandParseVisitor "<<node.className()<< std::endl;
	if(node.getDescriptionsInString().find("SequenceStateSet") != -1)
	{
		ref_ptr<crObject> childObject;
		ref_ptr<crDrawable> drawable0;
		int i = 0;
		for( CRCore::NodeArray::iterator itr = node.getChildArray().begin();
			 itr != node.getChildArray().end();
			 ++itr,i++ )
		{
			childObject = dynamic_cast<crObject *>(itr->get());
			if(!childObject.valid()) 
			{
				CRCore::notify(CRCore::ALWAYS)<<"crOptimizer::crNodeCommandParseVisitor::apply(CRCore::crSequence& node) SequenceStateSet error "<<node.getName()<< std::endl;
			    break;
			}
			if(i==0)
			{
				drawable0 = childObject->getDrawable(0);
				if(!drawable0->getStateSet())
					break;
                childObject->setStateSet(drawable0->getStateSet());
				drawable0->setStateSet(0);
			}
			else
			{
				childObject->setStateSet(childObject->getDrawable(0)->getStateSet());
				childObject->getDrawableList().clear();
				childObject->getDrawableList().push_back(drawable0);
			}
		}
	}
    apply((CRCore::crGroup&) node);
}

void crOptimizer::crNodeCommandParseVisitor::apply(CRCore::crGroup& node)
{
	std::string command = node.getDescriptionsInString();
	if(command.empty())
	{
		traverse(node);
		return;
	}
	std::string str;
	bool hasKeyWords = false;

	if(crArgumentParser::readKeyValue(command,"[NameID]",str))
		node.setName(str);
	//if( node.getName().compare("water") == 0/* ||
	//	(crArgumentParser::readKeyValue(command,"[NameID]",str) && str.compare("water")==0)*/)
	//{
	//	m_parentKeyWordsStack.push("water");
 //       hasKeyWords = true;
	//}
	//else if(node.getName().compare("Ambient") == 0)
	//{
 //       node.getOrCreateStateSet()->setRenderBinDetails(1,"StateSortedBin");
	//}
	
	if(CRCore::crArgumentParser::readKeyValue(command,"CollideMode",str))
	{
		if(str.compare("CollideNone")==0)
		{
			node.setCollideMode(crNode::CollideNone);
		}
		else if(str.compare("CollideWithBoundBox")==0)
		{
			node.setCollideMode(crNode::CollideWithBoundBox);
		}
		else if(str.compare("CollideWithBoundSphere")==0)
		{
			node.setCollideMode(crNode::CollideWithBoundSphere);
		}
		else if(str.compare("PrecisionCollide")==0)
		{
			node.setCollideMode(crNode::PrecisionCollide);
		}
		else if(str.compare("AvoidCollide")==0)
		{
			node.setCollideMode(crNode::AvoidCollide);
		}
	}
	crVector2 vec2;

	if(crArgumentParser::readKeyValue(command,"PolygonOffset",vec2))
	{
		//CRCore::notify(CRCore::WARN)<<"PolygonOffset = "<<vec2<<std::endl;
        crPolygonOffset *polygonOffset = new crPolygonOffset(vec2[0],vec2[1]);
		node.getOrCreateStateSet()->setAttribute(polygonOffset);
		node.getOrCreateStateSet()->setMode(GL_POLYGON_OFFSET_FILL,crStateAttribute::ON);
	}

	int int1 = 0;
	if(crArgumentParser::readKeyValue(command,"EnableLightStdMode",int1))
	{
		if(int1)
		    node.getOrCreateStateSet()->setMode(GL_LIGHTING,crStateAttribute::ON);
		else
			node.getOrCreateStateSet()->setMode(GL_LIGHTING,crStateAttribute::OFF);
	}

	if(CRCore::crArgumentParser::readKeyValue(command,"StateSetMode",str))
	{
		crStateSet *ss = node.getOrCreateStateSet();
		if(str.compare("ShadowMapRender")==0)
		{
			ss->setStateSetMode(crStateSet::SS_ShadowMapRender);
		}
		else if(str.compare("CollideMapRender")==0)
		{
			ss->setStateSetMode(crStateSet::SS_CollideMapRender);
		}
		else
		{
			ss->setStateSetMode(crStateSet::SS_Normal);
		}
	}
	//if(crArgumentParser::readKeyValue(command,"ShadowStateSet",int1))
	//{
	//	node.getOrCreateStateSet()->setStateSetMode(int1?crStateSet::SS_ShadowMapRender:crStateSet::SS_Normal);
	//}
	

	crVector3 vec3;
	crVector4 vec4;
	if(crArgumentParser::readKeyValue(command,"Gravity",vec3))
	{
		CRCore::crBrain::getInstance()->getCurrentActiveDB()->setGravity(vec3);
	}
	if(crArgumentParser::readKeyValue(command,"GroundPlane",vec4))
	{
		CRCore::crBrain::getInstance()->getCurrentActiveDB()->setGroundPlane(crPlane(vec4[0],vec4[1],vec4[2],vec4[3]));
	}
	if(crArgumentParser::readKeyValue(command,"MaxIterations",int1))
	{
		CRCore::crBrain::getInstance()->getCurrentActiveDB()->setMaxIterations(int1);
	}

	float flt1 = 0.0f;
	if(CRCore::crArgumentParser::readKeyValue(command,"MapArea",flt1))
	{//MapArea 0表示不能通过
	 //MapArea 0.01到0.19系统保留
	 //MapArea 0.2到0.29表示出生区域
	 //MapArea 0.3到1.0表示不同可行走区域
		crStateSet *ss = node.getOrCreateStateSet();
		ss->setStateSetMode(crStateSet::SS_CollideMapRender);
		crUniform *uniform = ss->getOrCreateUniform("walkable",CRCore::crUniform::FLOAT);
		uniform->setDataVariance(crBase::STATIC);
		uniform->set(flt1);
		node.setMapCollide(true);
	}
	traverse(node);

	//if(hasKeyWords) m_parentKeyWordsStack.pop();
}

void crOptimizer::crNodeCommandParseVisitor::apply(CRCore::crObject& node)
{			
	std::string command = node.getDescriptionsInString();
	if(command.empty())
	{
		traverse(node);
		return;
	}
	std::string str;
	bool hasKeyWords = false;

	if(crArgumentParser::readKeyValue(command,"[NameID]",str))
		node.setName(str);

	int int1 = 1;
	if(crArgumentParser::readKeyValue(node.getDescriptions(),"MapCollide",int1))
		node.setMapCollide(int1);
	if(crArgumentParser::readKeyValue(node.getDescriptions(),"EffectByShadow",int1))
		node.setEffectByShadow(int1);
	if(crArgumentParser::readKeyValue(node.getDescriptions(),"NeedSelectLight",int1))
		node.setNeedSelectLight(int1);

	//if( node.getName().compare("water") == 0 /*||
	//	(crArgumentParser::readKeyValue(command,"[NameID]",str) && str.compare("water")==0)*/)
	//{
	//	m_parentKeyWordsStack.push("water");
	//	hasKeyWords = true;
	//}

	if(CRCore::crArgumentParser::readKeyValue(command,"CollideMode",str))
	{
		if(str.compare("CollideNone")==0)
		{
			node.setCollideMode(crNode::CollideNone);
		}
		else if(str.compare("CollideWithBoundBox")==0)
		{
			node.setCollideMode(crNode::CollideWithBoundBox);
		}
		else if(str.compare("CollideWithBoundSphere")==0)
		{
			node.setCollideMode(crNode::CollideWithBoundSphere);
		}
		else if(str.compare("PrecisionCollide")==0)
		{
			node.setCollideMode(crNode::PrecisionCollide);
		}
		else if(str.compare("AvoidCollide")==0)
		{
			node.setCollideMode(crNode::AvoidCollide);
		}
	}

	if(CRCore::crArgumentParser::readKeyValue(command,"StateSetMode",str))
	{
		crStateSet *ss = node.getOrCreateStateSet();
		if(str.compare("ShadowMapRender")==0)
		{
			ss->setStateSetMode(crStateSet::SS_ShadowMapRender);
		}
		else if(str.compare("CollideMapRender")==0)
		{
			ss->setStateSetMode(crStateSet::SS_CollideMapRender);
		}
		else
		{
			ss->setStateSetMode(crStateSet::SS_Normal);
		}
	}

	//for(unsigned int i=0;i<node.getNumDrawables();++i)
	//{
	//	crDrawable* drawable = node.getDrawable(i);
	//	if (drawable)
	//	{
	//		apply(*drawable);
	//	}
	//}

	//if(hasKeyWords) m_parentKeyWordsStack.pop();

	crVector2 vec2;

	if(crArgumentParser::readKeyValue(command,"PolygonOffset",vec2))
	{
		//CRCore::notify(CRCore::WARN)<<"crObject PolygonOffset = "<<vec2<<std::endl;

		crPolygonOffset *polygonOffset = new crPolygonOffset(vec2[0],vec2[1]);
		node.getOrCreateStateSet()->setAttribute(polygonOffset);
		node.getOrCreateStateSet()->setMode(GL_POLYGON_OFFSET_FILL,crStateAttribute::ON);
	}
	float flt1 = 0.0f;
	if(CRCore::crArgumentParser::readKeyValue(command,"MapArea",flt1))
	{//MapArea 0表示不能通过
	 //MapArea 0.01到0.19系统保留
	 //MapArea 0.2到0.29表示出生区域
	 //MapArea 0.3到1.0表示不同可行走区域
		crStateSet *ss = node.getOrCreateStateSet();
		ss->setStateSetMode(crStateSet::SS_CollideMapRender);
		crUniform *uniform = ss->getOrCreateUniform("walkable",CRCore::crUniform::FLOAT);
		uniform->setDataVariance(crBase::STATIC);
		uniform->set(flt1);
		node.setMapCollide(true);
	}
}

//void crOptimizer::crNodeCommandParseVisitor::apply(CRCore::crDrawable& drawable)
//{
//	if(!m_parentKeyWordsStack.empty())
//	{
//		const std::string &parentKeyWords = m_parentKeyWordsStack.top();
//		if(parentKeyWords.compare("water") == 0)
//			drawable.getStateSet()->setBinNumber(9);
//	}
//}

////////////////////////////////////////////////////////////////////////////////////////////
//
//  MergeVertex
//

void crOptimizer::crMergeVertexVisitor::apply(CRCore::crObject& node)
{
	for(unsigned int i=0;i<node.getNumDrawables();++i)
	{
		crGeometry* geometry = node.getDrawable(i)->asGeometry();
		if (geometry)
		{
			apply(*geometry);
		}
	}
}

//void crOptimizer::crMergeVertexVisitor::apply(CRCore::crBillboard& node)
//{
//    apply((CRCore::crObject&) node);
//	node.init();
//}
//
//void crOptimizer::crMergeVertexVisitor::apply(CRCore::crDepthSortedObject& node)
//{
//    apply((CRCore::crObject&) node);
//    node.init();
//	crGeometry* geometry = node.getComposeDrawable()->asGeometry();
//	if(geometry)
//		apply(*geometry)
//}

void crOptimizer::crMergeVertexVisitor::initVtxIndexMap(int size)
{
    m_vtxIndexMap.clear();
	for(int i = 0; i<size; i++)
		m_vtxIndexMap[i] = i;
	//CRCore::notify(CRCore::FATAL)<<"crOptimizer::crMergeVertexVisitor(): initVtxIndexMap first = "<<m_vtxIndexMap.begin()->first<<std::endl;
}

void crOptimizer::crMergeVertexVisitor::vtxiRemoved(int i)
{
	for( VtxIndexMap::iterator itr = m_vtxIndexMap.begin();
		 itr != m_vtxIndexMap.end();
		 ++itr )
	{
        if(itr->second>i)
            itr->second--;
	}
}

void crOptimizer::crMergeVertexVisitor::apply(CRCore::crGeometry& geometry)
{
	if(geometry.getColorBinding()==crGeometry::BIND_PER_VERTEX)
		return;
	Vec3Array *vtxArray = (Vec3Array*)(geometry.getVertexArray());
	if(!vtxArray)
	{
		CRCore::notify(CRCore::FATAL)<<"crOptimizer::crMergeVertexVisitor(): Geometry has no vertex"<<std::endl;
		return;
	}
	Vec3Array *normalArray = NULL;
	if(geometry.getNormalBinding()==crGeometry::BIND_PER_VERTEX)
		normalArray = geometry.getNormalArray();
	//Vec4Array *colorArray = NULL;
	//if(geometry.getColorBinding()==crGeometry::BIND_PER_VERTEX)
	//	colorArray = dynamic_cast<CRCore::Vec4Array*>(geometry.getColorArray());
	CRCore::ref_ptr<Vec2Array> texArray[8];
	for(int i = 0; i<8; i++)
	{
		if(geometry.getTexCoordBinding(i)==crGeometry::BIND_PER_VERTEX)
			texArray[i] = (Vec2Array *)(geometry.getTexCoordArray(i));
		else
			texArray[i] = NULL;
	}
	
	int size = vtxArray->size();
	initVtxIndexMap(size);
	m_toEraseList.clear();
	bool texCheck = true;
	for(int i = 0; i<size - 1; ++i)
	{
		for(int j = i+1; j<size;j++)
		{
			if(m_toEraseList.find(j) != m_toEraseList.end())
				continue;
			if((*vtxArray)[i] == (*vtxArray)[j])
			{
				if(normalArray && (*normalArray)[i] != (*normalArray)[j])
				{
					continue;
				}
				//if(colorArray && (*colorArray)[i] != (*colorArray)[j])
				//{
				//	continue;
				//}
                texCheck = true;
				for(int texi = 0; texi<8;/* ++texi*/)
				{
					if(texArray[texi].valid() && (*texArray[texi])[i] != (*texArray[texi])[j])
					{
						texCheck = false;
						break;
					}
                    texi++;

					if(texArray[texi].valid() && (*texArray[texi])[i] != (*texArray[texi])[j])
					{
						texCheck = false;
						break;
					}
					texi++;

					if(texArray[texi].valid() && (*texArray[texi])[i] != (*texArray[texi])[j])
					{
						texCheck = false;
						break;
					}
					texi++;

					if(texArray[texi].valid() && (*texArray[texi])[i] != (*texArray[texi])[j])
					{
						texCheck = false;
						break;
					}
					texi++;
				}
				if(!texCheck)
				{
					continue;
				}

				m_vtxIndexMap[j] = i;
				m_toEraseList.insert(j);
			}
		}
	}
	if(m_toEraseList.size()>0)
	{
		int removed = 0;
		int off = 0;
		for( ToEraseList::iterator itr = m_toEraseList.begin();
			 itr != m_toEraseList.end();
			 ++itr )
		{
			off = *itr - removed;
			vtxiRemoved(off);
			vtxArray->erase(vtxArray->begin() + off);
			if(normalArray)
				normalArray->erase(normalArray->begin() + off);
			for(int texi = 0; texi<8; /*++texi*/)
			{
				if(texArray[texi].valid())
				{
					texArray[texi]->erase(texArray[texi]->begin() + off);
				}
				texi++;

				if(texArray[texi].valid())
				{
					texArray[texi]->erase(texArray[texi]->begin() + off);
				}
				texi++;

				if(texArray[texi].valid())
				{
					texArray[texi]->erase(texArray[texi]->begin() + off);
				}
				texi++;

				if(texArray[texi].valid())
				{
					texArray[texi]->erase(texArray[texi]->begin() + off);
				}
				texi++;
			}
			removed++;
		}

		crPrimitive *prim;
		for( crGeometry::PrimitiveList::iterator itr = geometry.getPrimitiveList().begin();
			itr != geometry.getPrimitiveList().end(); 
			++itr )
		{
			prim = itr->get();
			for( VtxIndexMap::iterator vtxItr = m_vtxIndexMap.begin();
				vtxItr != m_vtxIndexMap.end();
				++vtxItr )
			{
				if(vtxItr->first != vtxItr->second)
				{
					prim->replaceIndex(vtxItr->first,vtxItr->second);
				}
			}
		}

	    //CRCore::notify(CRCore::FATAL)<<"crOptimizer::crMergeVertexVisitor(): merge vertex count = "<<m_toEraseList.size()<<std::endl;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
//
//  OPT_DRAWABLESTATESET
//
//void crOptimizer::crSplitStateSetVisitor::apply(CRCore::crObject& node)
//{
//	for(unsigned int i=0;i<node.getNumDrawables();++i)
//	{
//		crDrawable* drawable = node.getDrawable(i);
//		if (drawable)
//		{
//			apply(*drawable);
//		}
//	}
//}
//
//void crOptimizer::crSplitStateSetVisitor::apply(CRCore::crDrawable& drawable)
//{
//	//crStateSet *stateset = drawable.getStateSet();
//    //if(stateset && !drawable.getStateSetOptimizerd()) apply(*stateset);
//}
//
//void crOptimizer::crSplitStateSetVisitor::apply(CRCore::crStateSet& stateset)
//{//SplitStateSet
//    
//	//drawable.setStateSetOptimizerd(true);
//}
//

////////////////////////////////////////////////////////////////////////////////////////////
//
// crFreezeTransformsVisitor
//
/////////////////////////////////////////////////////////////////////////
void crOptimizer::crFreezeTransformsVisitor::apply(CRPhysics::crMatterObject& node)
{
	if(!m_ignorePhysics)
	{
		CRCore::ref_ptr<RefMatrix> matrix = new RefMatrix;
		node.computeLocalToWorldMatrix(*matrix,this);

		if( node.getReferenceFrame() == crTransform::RELATIVE_TO_PARENTS )
		{
			if(!m_matrixList.empty())
				matrix->preMult(*(m_matrixList.back().get()));
		}

		pushMatrix(matrix.get());

		traverse(node); 

		popMatrix();
	}
	else
	{
		traverse(node);
	}
}
void crOptimizer::crFreezeTransformsVisitor::apply(rbody::CreBodyNode& node)
{
	m_sucess = false;
}
void crOptimizer::crFreezeTransformsVisitor::apply(CRCore::crTransform& node)
{
	if(!dynamic_cast<CRCore::crMatrixTransform *>(&node))
	{
		m_sucess = false;
	}
}
void crOptimizer::crFreezeTransformsVisitor::apply(CRCore::crMatrixTransform& node)
{
	if(crDisplaySettings::instance()->getRunMode() == 0 || isNodeEmpty(node))
	{
		CRCore::ref_ptr<RefMatrix> matrix = new RefMatrix;
		node.computeLocalToWorldMatrix(*matrix,this);

		if( node.getReferenceFrame() == crTransform::RELATIVE_TO_PARENTS )
		{
			if(!m_matrixList.empty())
				matrix->preMult(*(m_matrixList.back().get()));
		}

		pushMatrix(matrix.get());

#if _DEBUG
		try
		{
#endif
			traverse(node);
#if _DEBUG
		}
		catch (...)
		{
			CRCore::notify(CRCore::ALWAYS)<<"crFreezeTransformsVisitor error "<<std::endl;
		}
#endif

		popMatrix();

		if(m_sucess)
		{
			m_freezeSet.insert(&node);
			//ref_ptr<crGroup> group = &node;
			//ref_ptr<crGroup> parent;
			//parent = group->getParent(0);
			//parent->removeChild(group.get());

			//int num = group->getNumChildren();
			//for(int i = 0; i<num; i++)
			//	parent->addChild(group->getChild(i));
			//group->removeChild(0,num);
		}
	}
	else
	{
		m_sucess = false;
	}
}

void crOptimizer::crFreezeTransformsVisitor::freeze()
{
	ref_ptr<crMatrixTransform> matrixNode;
	ref_ptr<crGroup> parent;
	int num;
	for( FreezeSet::iterator itr = m_freezeSet.begin();
		 itr != m_freezeSet.end();
		 ++itr )
	{
		matrixNode = *itr;
		parent = matrixNode->getParent(0);
		parent->removeChild(matrixNode.get());
		num = matrixNode->getNumChildren();
		for(int i = 0; i<num; i++)
			parent->addChild(matrixNode->getChild(i));
		matrixNode->removeChild(0,num);
	}
}

void crOptimizer::crFreezeTransformsVisitor::apply(CRCore::crObject& node)
{
	if( m_matrixList.empty() ) return;
	crMatrix &mat = *(m_matrixList.back().get());
	//Vec3Array *vertexArray = NULL;
	//Vec3Array *normalArray = NULL;
	CRUtil::crTransformAttributeFunctor tf(mat);
	for(CRCore::crObject::DrawableList::iterator itr = node.getDrawableList().begin();
		itr != node.getDrawableList().end();
		++itr)
	{
		(*itr)->accept(tf);
		//if(dynamic_cast<CRCore::crGeometry *>(itr->get()))
		//{
		//	crGeometry *geometry = dynamic_cast<CRCore::crGeometry *>(itr->get());
		//	vertexArray = (Vec3Array*)(geometry->getVertexArray());
		//	normalArray = (Vec3Array*)(geometry->getNormalArray());
		//	for( Vec3Array::iterator vitr = vertexArray->begin();
		//		vitr != vertexArray->end();
		//		++vitr )
		//	{
		//		*vitr = *vitr * mat;
		//	}
		//	if(normalArray)
		//	{
		//		crMatrix rotmat = mat;
		//		rotmat.setTrans(0,0,0);
		//		for( Vec3Array::iterator nitr = normalArray->begin();
		//			nitr != normalArray->end();
		//			++nitr )
		//		{
		//			*nitr = (*nitr * rotmat).normalize();
		//		}
		//	}
		//}
		//else if(dynamic_cast<CRCore::crShapeDrawable *>(itr->get()))
		//{

		//}
		(*itr)->dirtyBound();
	}
	m_sucess = true;
}

void crOptimizer::crFreezeTransformsVisitor::apply(CRCore::crBillboard& node)
{
	m_sucess = false;
}