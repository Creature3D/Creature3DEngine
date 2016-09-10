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
#ifndef CRUTIL_CROPTIMIZER_H
#define CRUTIL_CROPTIMIZER_H

#include <CRCore/crNodeVisitor.h>
#include <CRCore/crMatrix.h>
#include <CRCore/crGeometry.h>
#include <CRCore/crTransform.h>
#include <CRCore/crBillboard.h>
//#include <CRCore/crDepthSortedObject.h>
#include <CRCore/crEventGroup.h>
#include <CRPhysics/crMatterObject.h>
#include <CRUtil/crExport.h>
#include <CRUtil/crTesselator.h>
//#include <CRUtil/crTriStripVisitor.h>

#include <set>
namespace rbody{
	class CreBodyNode;
};
namespace CRUtil {

/** Insert impostor nodes into scene graph.
  * For example of usage see src/Demos/osgimpostor.
  */

class crTriStripVisitor;

class CRUTIL_EXPORT crOptimizer
{
    public:
        crOptimizer() {}
        virtual ~crOptimizer() {}

        enum OptimizationOptions
        {
            FLATTEN_STATIC_TRANSFORMS = 0x00001,
            REMOVE_REDUNDANT_NODES    = 0x00002,
            COMBINE_ADJACENT_LODS     = 0x00004,
            SHARE_DUPLICATE_STATE     = 0x00008,
            MERGE_GEOMETRY            = 0x00010,
            CHECK_GEOMETRY            = 0x00020,
            SPATIALIZE_GROUPS         = 0x00040,
            COPY_SHARED_NODES         = 0x00080,
            TRISTRIP_GEOMETRY         = 0x00100,
            TESSELATE_GEOMETRY        = 0x00200,
			UNITE_NODES               = 0x00400, 
			TEXTURE_SETTINGS          = 0x00800,
			//
			CHECK_PHYSICS             = 0x01000,
			INITIAL_CHECK             = 0x02000,
			//BILLBOARD_OPT             = 0x2000,注：billboard最好放在场景的最后面，有利于提高速度
			PRERENDEROPT              = 0x04000,
			OBJECTCOMMANDBUILD        = 0x08000,
			NODECOMMANDPARSE          = 0x10000,
			MERGE_VERTEX              = 0x20000,
			//OPT_DRAWABLESTATESET      = 0x40000,
			FREEZE_TRANSFORMS         = 0x40000,
            DEFAULT_OPTIMIZATIONS = FLATTEN_STATIC_TRANSFORMS |//ConvertFlt_OPT
                                REMOVE_REDUNDANT_NODES |
                                //COMBINE_ADJACENT_LODS |
                                SHARE_DUPLICATE_STATE |
								//UNITE_NODES |
								TESSELATE_GEOMETRY |
                                MERGE_GEOMETRY |
                                CHECK_GEOMETRY |
								SPATIALIZE_GROUPS |
								TEXTURE_SETTINGS |
								CHECK_PHYSICS |
								INITIAL_CHECK |
								PRERENDEROPT |
								OBJECTCOMMANDBUILD |
								NODECOMMANDPARSE |
								MERGE_VERTEX/*|
								TRISTRIP_GEOMETRY*/,
			RUN_OPTIMIZATIONS = //FLATTEN_STATIC_TRANSFORMS | 20150512
								//REMOVE_REDUNDANT_NODES | 20150512
								//COMBINE_ADJACENT_LODS |
								//SHARE_DUPLICATE_STATE | 20150512
								//UNITE_NODES |
								//TESSELATE_GEOMETRY |
								//MERGE_GEOMETRY |
								CHECK_GEOMETRY |
								//SPATIALIZE_GROUPS | 20150512
								TEXTURE_SETTINGS |
								CHECK_PHYSICS |
								INITIAL_CHECK |
								PRERENDEROPT |
								//OBJECTCOMMANDBUILD |
								NODECOMMANDPARSE /*|
								MERGE_VERTEX|
								TRISTRIP_GEOMETRY*/,
			RUN_OPTIMIZATIONS_EDIT = FLATTEN_STATIC_TRANSFORMS | //20150512
								REMOVE_REDUNDANT_NODES | //20150512
								//COMBINE_ADJACENT_LODS |
								SHARE_DUPLICATE_STATE | //20150512
								//UNITE_NODES |
								//TESSELATE_GEOMETRY |
								//MERGE_GEOMETRY |
								CHECK_GEOMETRY |
								SPATIALIZE_GROUPS | //20150512
								TEXTURE_SETTINGS |
								CHECK_PHYSICS |
								INITIAL_CHECK |
								PRERENDEROPT |
								//OBJECTCOMMANDBUILD |
								NODECOMMANDPARSE /*|
								MERGE_VERTEX|
								TRISTRIP_GEOMETRY*/,
            ALL_OPTIMIZATIONS = FLATTEN_STATIC_TRANSFORMS |
                                REMOVE_REDUNDANT_NODES |
                                COMBINE_ADJACENT_LODS |
								CHECK_PHYSICS |
                                SHARE_DUPLICATE_STATE |
								//UNITE_NODES |
								TESSELATE_GEOMETRY |
                                MERGE_GEOMETRY |
                                CHECK_GEOMETRY |
                                SPATIALIZE_GROUPS |
                                COPY_SHARED_NODES |
                                TRISTRIP_GEOMETRY |
								TEXTURE_SETTINGS |
								INITIAL_CHECK    |
								OBJECTCOMMANDBUILD |
								NODECOMMANDPARSE |
								MERGE_VERTEX
        };

        /** reset internal data to initial state - the getPrimissableOptionsMap is cleared.*/
        void reset();
        
        /** traverse the node and its subgraph with a series of optimization
          * visitors, specificied by the OptizationOptions.*/
        void optimize(CRCore::crNode* node);

        /** traverse the node and its subgraph with a series of optimization
          * visitors, specificied by the OptizationOptions.*/
        virtual void optimize(CRCore::crNode* node, unsigned int options);


        inline void setPermissableOptimizationsForObject(const CRCore::crBase* base, unsigned int options)
        {
            m_permissableOptimizationsMap[base] = options;
        }
        
        inline unsigned int getPermissableOptimizationsForObject(const CRCore::crBase* base) const
        {
            PermissableOptimizationsMap::const_iterator itr = m_permissableOptimizationsMap.find(base);
            if (itr!=m_permissableOptimizationsMap.end()) return itr->second;
            else return 0xffffffff;
        }

		inline bool isOperationPermissableForObject(const CRCore::crBase* base,unsigned int option)
		{
			return (option & getPermissableOptimizationsForObject(base))!=0; 
		}

        typedef std::map<const CRCore::crBase*,unsigned int> PermissableOptimizationsMap;

        PermissableOptimizationsMap& getPrimissableOptionsMap() { return m_permissableOptimizationsMap; }
        const PermissableOptimizationsMap& getPrimissableOptionsMap() const { return m_permissableOptimizationsMap; }

		struct OptimizeFunctor : public virtual CRCore::Referenced
		{
		public:
			virtual void operator() (CRCore::crNode* node){}
		};

		typedef std::vector< CRCore::ref_ptr<OptimizeFunctor> > OptimizeFunctorArray;
		inline const OptimizeFunctorArray& getOptimizeFunctor()const{ return m_optimizeFunctorArray; }
		inline OptimizeFunctorArray& getOptimizeFunctor(){ return m_optimizeFunctorArray; }
		inline void addOptimizeFunctor( OptimizeFunctor *of ){ m_optimizeFunctorArray.push_back(of); }
		inline void clearOptimizeFunctor(){ m_optimizeFunctorArray.clear(); }

    protected:
		OptimizeFunctorArray                 m_optimizeFunctorArray;

        PermissableOptimizationsMap m_permissableOptimizationsMap;

    public:


        /** Flatten Static Trasform nodes by applying their transform to the
          * geometry on the leaves of the scene graph, then removing the 
          * now redundant transforms.*/        
        class CRUTIL_EXPORT crFlattenStaticTransformsVisitor : public CRCore::crNodeVisitor
        {
            public:



                crFlattenStaticTransformsVisitor(crOptimizer* optimizer=0):
                    CRCore::crNodeVisitor(CRCore::crNodeVisitor::TRAVERSE_ALL_CHILDREN),
                    m_optimizer(optimizer) {}

                virtual void apply(CRCore::crObject& obj);
                virtual void apply(CRCore::crBillboard& obj);
                virtual void apply(CRCore::crTransform& transform);

                bool removeTransforms(CRCore::crNode* nodeWeCannotRemove);

                inline bool isOperationPermissableForObject(const CRCore::crBase* base)
                {
                    return m_optimizer ? m_optimizer->isOperationPermissableForObject(base,FLATTEN_STATIC_TRANSFORMS) :  true; 
                }

            
            protected:

                typedef std::vector<CRCore::crTransform*>                TransformStack;
                typedef std::set<CRCore::crDrawable*>                    DrawableSet;
                typedef std::set<CRCore::crBillboard*>                   BillboardSet;
                typedef std::set<CRCore::crTransform*>                   TransformSet;
                
                crOptimizer*    m_optimizer;
                TransformStack  m_transformStack;
                DrawableSet     m_drawableSet;
                BillboardSet    m_billboardSet;
                TransformSet    m_transformSet;
        };


        /** Combine Static Trasform nodes that sit above on another.*/        
        class CRUTIL_EXPORT crCombineStaticTransformsVisitor : public CRCore::crNodeVisitor
        {
            public:

                crCombineStaticTransformsVisitor(crOptimizer* optimizer=0):
                    CRCore::crNodeVisitor(CRCore::crNodeVisitor::TRAVERSE_ALL_CHILDREN),
                    m_optimizer(optimizer) {}

                virtual void apply(CRCore::crMatrixTransform& transform);

                bool removeTransforms(CRCore::crNode* nodeWeCannotRemove);

                inline bool isOperationPermissableForObject(const CRCore::crBase* base)
                {
                    return m_optimizer ? m_optimizer->isOperationPermissableForObject(base,FLATTEN_STATIC_TRANSFORMS) :  true; 
                }

            protected:

                typedef std::set<CRCore::crMatrixTransform*> TransformSet;
                crOptimizer*      m_optimizer;
                TransformSet      m_transformSet;
        };

        /** Remove rendundant nodes, such as groups with one single child.*/
        class CRUTIL_EXPORT crRemoveEmptyNodesVisitor : public CRCore::crNodeVisitor
        {
        public:
            typedef std::set<CRCore::crNode*> NodeList;
            NodeList                          m_redundantNodeList;

            crRemoveEmptyNodesVisitor(crOptimizer* optimizer=0):
                CRCore::crNodeVisitor(CRCore::crNodeVisitor::TRAVERSE_ALL_CHILDREN),
				m_optimizer(optimizer),
				m_onlyCheckChild(false){}

            virtual void apply(CRCore::crObject& obj);
            virtual void apply(CRCore::crGroup& group);
            
            void removeEmptyNodes();

            inline bool isOperationPermissableForObject(const CRCore::crBase* base)
            {
                return m_optimizer ? m_optimizer->isOperationPermissableForObject(base,REMOVE_REDUNDANT_NODES) :  true; 
            }

            crOptimizer*      m_optimizer;
			void setOnlyCheckChild(bool bln) { m_onlyCheckChild = bln; }
		protected:
			bool m_onlyCheckChild;//如果没有子节点则认为是空节点
        };

        /** Remove rendundant nodes, such as groups with one single child.*/
        class CRUTIL_EXPORT crRemoveRedundantNodesVisitor : public CRCore::crNodeVisitor
        {
            public:

                typedef std::set<CRCore::crNode*> NodeList;
                NodeList                     m_redundantNodeList;

                crRemoveRedundantNodesVisitor(crOptimizer* optimizer=0):
                    CRCore::crNodeVisitor(CRCore::crNodeVisitor::TRAVERSE_ALL_CHILDREN),
                    m_optimizer(optimizer) {}
                
                virtual void apply(CRCore::crGroup& group);
                virtual void apply(CRCore::crTransform& transform);
                
                void removeRedundantNodes();

                inline bool isOperationPermissableForObject(const CRCore::crBase* base)
                {
                    return m_optimizer ? m_optimizer->isOperationPermissableForObject(base,REMOVE_REDUNDANT_NODES) :  true; 
                }

                crOptimizer*      m_optimizer;
        };

        /** Tesselate all geodes, to remove POLYGONS
          * complementary ranges.*/
        class CRUTIL_EXPORT crTesselateVisitor : public CRCore::crNodeVisitor
        {
            public:

                crTesselateVisitor():CRCore::crNodeVisitor(CRCore::crNodeVisitor::TRAVERSE_ALL_CHILDREN) {}
				virtual void apply(CRCore::crFilterRenderManager& filter);
                virtual void apply(CRCore::crObject& obj);
        };

        /** Optimize the crLod groups, by combining adjacent crLod's which have
          * complementary ranges.*/
        class CRUTIL_EXPORT crCombineLODsVisitor : public CRCore::crNodeVisitor
        {
            public:

                typedef std::set<CRCore::crGroup*>  GroupList;
                GroupList                      m_groupList;

                crCombineLODsVisitor(crOptimizer* optimizer=0):
                    CRCore::crNodeVisitor(CRCore::crNodeVisitor::TRAVERSE_ALL_CHILDREN),
                    m_optimizer(optimizer) {}

                virtual void apply(CRCore::crLod& lod);
				virtual void apply(CRCore::crPagedLOD& lod);

                void combineLODs();

                inline bool isOperationPermissableForObject(const CRCore::crBase* base)
                {
                    return m_optimizer ? m_optimizer->isOperationPermissableForObject(base,COMBINE_ADJACENT_LODS) :  true; 
                }

                crOptimizer*      m_optimizer;
        };
 
        /** Optimize crState in the scene graph by removing duplicate state,
          * replacing it with shared instances, both for StateAttributes,
          * and whole StateSets.*/
        class CRUTIL_EXPORT crStateVisitor : public CRCore::crNodeVisitor
        {
            public:

                /// default to traversing all children.
                crStateVisitor(crOptimizer* optimizer=0):
                    CRCore::crNodeVisitor(CRCore::crNodeVisitor::TRAVERSE_ALL_CHILDREN),
					m_optimizer(optimizer){};

                /** empty visitor, make it ready for next traversal.*/        
                virtual void reset();
				virtual void apply(CRCore::crFilterRenderManager& group){};
				virtual void apply(CRCore::crCanvasNode& group){};
				virtual void apply(CRCore::crWidgetNode& group){};
                virtual void apply(CRCore::crNode& node);

                virtual void apply(CRCore::crObject& obj);

                void optimize();

                inline bool isOperationPermissableForObject(const CRCore::crBase* base)
                {
                    return m_optimizer ? m_optimizer->isOperationPermissableForObject(base,SHARE_DUPLICATE_STATE) : true; 
                }

            protected:

                void addStateSet(CRCore::crStateSet* stateset,CRCore::crBase* obj);

				struct LessStateSetFunctor
				{
					bool operator () (const CRCore::crStateSet* lhs,const CRCore::crStateSet* rhs) const
					{
						if (lhs==rhs) return false;
						return lhs->compare(*rhs,true)<0;
					}
				};
				//template<typename T>
				//struct LessDerefFunctor 
				//{
				//	bool operator () (const T* lhs,const T* rhs) const
				//	{
				//		return (*lhs<*rhs); 
				//	}
				//};

                typedef std::set<CRCore::ref_ptr<CRCore::crBase> >                   BaseSet;
                typedef std::map<CRCore::crStateSet*,BaseSet,LessStateSetFunctor>    StateSetMap;

                crOptimizer*      m_optimizer;
                StateSetMap       m_statesets;

        };
        
        class CRUTIL_EXPORT crCheckGeometryVisitor : public CRCore::crNodeVisitor
        {
            public:

                /// default to traversing all children.
                crCheckGeometryVisitor(crOptimizer* optimizer=0):
                    CRCore::crNodeVisitor(CRCore::crNodeVisitor::TRAVERSE_ALL_CHILDREN),
                    m_optimizer(optimizer) {}

                virtual void apply(CRCore::crObject& obj) { checkObject(obj); }

                void checkObject(CRCore::crObject& obj);
                
                inline bool isOperationPermissableForObject(const CRCore::crBase* base)
                {
                    return m_optimizer ? m_optimizer->isOperationPermissableForObject(base,CHECK_GEOMETRY) :  true; 
                }

                crOptimizer*      m_optimizer;

        };
        
        class CRUTIL_EXPORT crMergeGeometryVisitor : public CRCore::crNodeVisitor
        {
            public:

                /// default to traversing all children.
                crMergeGeometryVisitor(crOptimizer* optimizer=0) :
                    CRCore::crNodeVisitor(CRCore::crNodeVisitor::TRAVERSE_ALL_CHILDREN),
                    m_optimizer(optimizer) {}

                virtual void apply(CRCore::crObject& obj) { mergeObject(obj); deuceObject(obj); }
                virtual void apply(CRCore::crBillboard&) { /* don't do anything*/ }

                bool mergeObject(CRCore::crObject& obj);
				void deuceObject(CRCore::crObject& obj);

                static bool geometryContainsSharedArrays(CRCore::crGeometry& geom);

                static bool mergeGeometry(CRCore::crGeometry& lhs,CRCore::crGeometry& rhs);

                static bool mergePrimitive(CRCore::DrawArrays& lhs,CRCore::DrawArrays& rhs);
                static bool mergePrimitive(CRCore::DrawArrayLengths& lhs,CRCore::DrawArrayLengths& rhs);
                static bool mergePrimitive(CRCore::DrawElementsUByte& lhs,CRCore::DrawElementsUByte& rhs);
                static bool mergePrimitive(CRCore::DrawElementsUShort& lhs,CRCore::DrawElementsUShort& rhs);
                static bool mergePrimitive(CRCore::DrawElementsUInt& lhs,CRCore::DrawElementsUInt& rhs);

                inline bool isOperationPermissableForObject(const CRCore::crBase* base)
                {
                    return m_optimizer ? m_optimizer->isOperationPermissableForObject(base,MERGE_GEOMETRY) :  true; 
                }

                crOptimizer*      m_optimizer;
        };

        /** Spatialize scene into a balanced quad/oct tree.*/
        class CRUTIL_EXPORT crSpatializeGroupsVisitor : public CRCore::crNodeVisitor
        {
            public:

                crSpatializeGroupsVisitor(crOptimizer* optimizer=0):
                    CRCore::crNodeVisitor(CRCore::crNodeVisitor::TRAVERSE_ALL_CHILDREN),
                    m_optimizer(optimizer) {}
                
                virtual void apply(CRCore::crGroup& group);
				virtual void apply(CRCore::crFilterRenderManager& group);
				virtual void apply(CRCore::crCanvasNode& group);
				virtual void apply(CRCore::crWidgetNode& group);

                bool divide(unsigned int maxNumTreesPerCell=8);
                
                bool divide(CRCore::crGroup* group, unsigned int maxNumTreesPerCell);
                
                typedef std::set<CRCore::crGroup*> GroupsToDivideList;
                GroupsToDivideList _groupsToDivideList;

                inline bool isOperationPermissableForObject(const CRCore::crBase* base)
                {
                    return m_optimizer ? m_optimizer->isOperationPermissableForObject(base,SPATIALIZE_GROUPS) :  true; 
                }

                crOptimizer*      m_optimizer;

        };

        /** Copy any shared subgraphs, enabling flattening of static transforms.*/
        class CRUTIL_EXPORT crCopySharedSubgraphsVisitor : public CRCore::crNodeVisitor
        {
            public:

                crCopySharedSubgraphsVisitor(crOptimizer* optimizer=0):
                    CRCore::crNodeVisitor(CRCore::crNodeVisitor::TRAVERSE_ALL_CHILDREN),
                    m_optimizer(optimizer) {}
                
                virtual void apply(CRCore::crNode& node);

                void copySharedNodes();
                
                typedef std::set<CRCore::crNode*> SharedNodeList;
                SharedNodeList m_sharedNodeList;
                
                inline bool isOperationPermissableForObject(const CRCore::crBase* base)
                {
                    return m_optimizer ? m_optimizer->isOperationPermissableForObject(base,COPY_SHARED_NODES) :  true; 
                }

                crOptimizer*      m_optimizer;

        };

		class CRUTIL_EXPORT crUniteNodesVisitor : public CRCore::crNodeVisitor
		{
		public:
			crUniteNodesVisitor(crOptimizer* optimizer=0):
			CRCore::crNodeVisitor(CRCore::crNodeVisitor::UNITENODESVISITOR,CRCore::crNodeVisitor::TRAVERSE_ALL_CHILDREN),
				m_optimizer(optimizer) {}

				virtual void apply(CRCore::crObject& obj)             {}
				virtual void apply(CRCore::crGroup& group);

				virtual void apply(CRCore::crBillboard& node)         {}

				//virtual void apply(CRCore::crLightSource& node) ;            

				//virtual void apply(CRCore::crTransform& transform);            
				//virtual void apply(CRCore::crMatrixTransform& node)           
				//virtual void apply(CRCore::crPositionAttitudeTransform& node) 

				virtual void apply(CRCore::crSwitch& node);                   
				virtual void apply(CRCore::crSequence& node);
				virtual void apply(CRCore::crLod& node);
				//virtual void apply(CRCore::crEventGroup& node);
				//virtual void apply(CRCore::crClearNode& node) ;                
				/** empty visitor, make it ready for next traversal.*/        
				virtual void reset();

				void optimize();

				inline bool isOperationPermissableForObject(const CRCore::crBase* base)
				{
					return m_optimizer ? m_optimizer->isOperationPermissableForObject(base,UNITE_NODES) : true; 
				}

		protected:
		/*	void addDuplicate(CRCore::crNode& node) {}
			void addDuplicate(CRCore::crGroup& node);
			void addDuplicate(CRCore::crObject& node);*/

			struct LessGroup
			{
				bool operator () (const CRCore::crGroup* lhs,const CRCore::crGroup* rhs) const
				{
					if(lhs == rhs) return false;
					return lhs->getDescriptionsInString().compare(rhs->getDescriptionsInString())<0;

					//if(lhs->getDescriptionsInString().compare(rhs->getDescriptionsInString())<0)
					//	return true;
					//if(rhs->getDescriptionsInString().compare(lhs->getDescriptionsInString())<0)
					//	return false;
					////const CRCore::crNode::ParentArray &lparents = lhs->getParents();
					////const CRCore::crNode::ParentArray &rparents = rhs->getParents();
					////CRCore::crNode::ParentArray::const_iterator liter;
					////CRCore::crNode::ParentArray::const_iterator riter;
     ////               if(lparents.size()!=rparents.size()) return true;
					////bool find = false;
					////for(liter=lparents.begin();liter!=lparents.end();liter++)
					////{
					////    find = false;
					////	for(riter=rparents.begin();riter!=rparents.end();riter++)
					////	{
					////		if(*liter==*riter) 
					////		{
					////			find = true;
					////			break;
					////		}
					////	}
					////	if(!find) return true;
					////}

					////if(lhs->getStateSet()!=rhs->getStateSet()) return true;
					////return false;
					//if (lhs->getStateSet()==rhs->getStateSet()) return false;
					//return lhs->getStateSet()->compare(*(rhs->getStateSet()),true) < 0;
                }
			};

			struct LessObject
			{
				bool operator () (const CRCore::crObject* lhs,const CRCore::crObject* rhs) const
				{
					if(lhs == rhs) return false;
					return lhs->getDescriptionsInString().compare(rhs->getDescriptionsInString())<0;
					
					//if(lhs->getDescriptionsInString().compare(rhs->getDescriptionsInString())<0)
					//	return true;
					//if(rhs->getDescriptionsInString().compare(lhs->getDescriptionsInString())<0)
					//	return false;
					//////const CRCore::crNode::ParentArray &lparents = lhs->getParents();
					//////const CRCore::crNode::ParentArray &rparents = rhs->getParents();
					//////CRCore::crNode::ParentArray::const_iterator liter;
					//////CRCore::crNode::ParentArray::const_iterator riter;
					//////if(lparents.size()!=rparents.size()) return true;
					//////bool find = false;
					//////for(liter=lparents.begin();liter!=lparents.end();liter++)
					//////{
					//////	find = false;
					//////	for(riter=rparents.begin();riter!=rparents.end();riter++)
					//////	{
					//////		if(*liter==*riter) 
					//////		{
					//////			find = true;
					//////			break;
					//////		}
					//////	}
					//////	if(!find) return true;
					//////}
					//////if(lhs->getStateSet()!=rhs->getStateSet()) return true;
					//////return false;
					//if (lhs->getStateSet()==rhs->getStateSet()) return false;
					//return lhs->getStateSet()->compare(*(rhs->getStateSet()),true) < 0;
				}
			};

            struct LessBillboard
			{
				bool operator () (const CRCore::crBillboard* lhs,const CRCore::crBillboard* rhs) const
				{
					if(lhs == rhs) return false;
					//if(lhs->getDescriptionsInString().compare(rhs->getDescriptionsInString())<0)
					//	return true;
                   
					if(lhs->getMode()<rhs->getMode()) return true;
					if(rhs->getMode()<lhs->getMode()) return false;
					return LessObject()(lhs,rhs);
				}
            };

			typedef std::vector<CRCore::crGroup*>                               GroupDuplicateList;
			typedef std::vector<CRCore::crObject*>                              ObjectDuplicateList;
			typedef std::vector<CRCore::crBillboard*>                           BillboardDuplicateList;
			typedef std::map<CRCore::crGroup*,GroupDuplicateList,LessGroup>     GroupDuplicateMap;
			typedef std::map<CRCore::crObject*,ObjectDuplicateList,LessObject>  ObjectDuplicateMap;
			typedef std::map<CRCore::crBillboard*,BillboardDuplicateList,LessBillboard> BillboardDuplicateMap;

			crOptimizer*           m_optimizer;
			GroupDuplicateMap      m_groups;
			ObjectDuplicateMap     m_objects;
			BillboardDuplicateMap  m_billboards;

		};

		class CRUTIL_EXPORT crTextureVisitor : public CRCore::crNodeVisitor
		{
		public:

			crTextureVisitor(bool changeAutoUnRef, bool valueAutoUnRef,
				bool changeClientImageStorage, bool valueClientImageStorage,
				bool changeAnisotropy, float valueAnisotropy,
				crOptimizer* optimizer=0):
			    CRCore::crNodeVisitor(CRCore::crNodeVisitor::TRAVERSE_ALL_CHILDREN),
				m_changeAutoUnRef(changeAutoUnRef), m_valueAutoUnRef(valueAutoUnRef),
				m_changeClientImageStorage(changeClientImageStorage), m_valueClientImageStorage(valueClientImageStorage),
				m_changeAnisotropy(changeAnisotropy), m_valueAnisotropy(valueAnisotropy),
				m_optimizer(optimizer),
				m_changeFilter(false),
				m_removeTexEnv(false),
				m_changeWrap(false){}

				virtual void apply(CRCore::crObject& node);
				virtual void apply(CRCore::crNode& node);
				virtual void apply(CRCore::crTraverseStringNode& node);
				
				void apply(CRCore::crStateSet& stateset);
				void apply(CRCore::crTexture& texture);

				void setChangeFilter(unsigned short minfilter,unsigned short magfilter);
				void setChangeWrap(CRCore::crTexture::WrapMode mode);
				bool            m_changeAutoUnRef, m_valueAutoUnRef;
				bool            m_changeClientImageStorage, m_valueClientImageStorage;
				bool            m_changeAnisotropy;
				float           m_valueAnisotropy;
				crOptimizer*    m_optimizer;
				bool            m_changeFilter;
				unsigned short  m_min_filter;
				unsigned short  m_mag_filter;
				bool            m_changeWrap;
				CRCore::crTexture::WrapMode m_wrapMode;

				bool m_removeTexEnv;
		};

		class CRUTIL_EXPORT crCheckPhysicsVisitor : public CRCore::crNodeVisitor
		{
		public:

			crCheckPhysicsVisitor():
			  CRCore::crNodeVisitor(CRCore::crNodeVisitor::OPT_PHYSICSCHECK_VISITOR,CRCore::crNodeVisitor::TRAVERSE_ALL_CHILDREN){}

		    virtual void apply(CRPhysics::crMatterObject& node);
			virtual void apply(CRCore::crGroup& node);
            virtual void apply(CRCore::crObject& node);

	        typedef std::vector<CRCore::ref_ptr<CRCore::RefMatrix> > MatrixList;
			inline void pushMatrix( CRCore::RefMatrix* matrix)
			{
				  m_matrixList.push_back(matrix);
			}
			inline void popMatrix()
			{
			    m_matrixList.pop_back();
			}
			inline CRCore::crMatrix topMatrix()
			{
				if(m_matrixList.empty())
					return CRCore::crMatrix::identity();
				else
					return *(m_matrixList.back().get());
			}
		protected:
		    MatrixList                              m_matrixList;

		};

		class CRUTIL_EXPORT crInitialCheckVisitor : public CRCore::crNodeVisitor
		{
		public:

			crInitialCheckVisitor():
				CRCore::crNodeVisitor(CRCore::crNodeVisitor::TRAVERSE_ALL_CHILDREN),
				m_enableIntersect(true){}

		    virtual void apply(CRCore::crObject& node);
			virtual void apply(CRCore::crGroup& node);

		protected:
			bool     m_enableIntersect;
		};

		class CRUTIL_EXPORT crBeforehandRenderVisitor : public CRCore::crNodeVisitor
		{
		public:

			crBeforehandRenderVisitor():
			  CRCore::crNodeVisitor(CRCore::crNodeVisitor::TRAVERSE_ALL_CHILDREN){}

		    virtual void apply(CRCore::crNode& node);
			virtual void apply(CRCore::crDB& node);

		protected:
		    void createFog(CRCore::crNode& node);
			void createCubeMap(CRCore::crNode& node);
            void createSpecularHighlights(CRCore::crNode& node);

			CRCore::ref_ptr<CRCore::crDB> m_currentDB;
		};
        ///flt 编辑器无法直接对Creature3D的Object级进行command编辑
		///该类是用于实现flt 编辑器对Creature3D的Object级进行command编辑
		class CRUTIL_EXPORT crObjectCommandBuildVisitor : public CRCore::crNodeVisitor
		{
		public:
			crObjectCommandBuildVisitor():
			  CRCore::crNodeVisitor(CRCore::crNodeVisitor::TRAVERSE_ALL_CHILDREN){}

			  virtual void apply(CRCore::crGroup& node);
			  virtual void apply(CRCore::crObject& node);

		protected:
			typedef std::list<std::string> StringList;
			StringList m_currentObjCommandList;
		};

		class CRUTIL_EXPORT crNodeCommandParseVisitor : public CRCore::crNodeVisitor
		{
		public:
			crNodeCommandParseVisitor():
			  CRCore::crNodeVisitor(CRCore::crNodeVisitor::TRAVERSE_ALL_CHILDREN){}

			  virtual void apply(CRPhysics::crMatterObject& node);
			  virtual void apply(CRCore::crSequence& node);
			  virtual void apply(CRCore::crGroup& node);
              virtual void apply(CRCore::crObject& node);

			 // void apply(CRCore::crDrawable& drawable);
			  //void apply(CRCore::crStateSet& stateset);

		//protected:
		//	std::stack<std::string> m_parentKeyWordsStack;
		};

		class CRUTIL_EXPORT crMergeVertexVisitor : public CRCore::crNodeVisitor
		{
		public:
			crMergeVertexVisitor():
			  CRCore::crNodeVisitor(CRCore::crNodeVisitor::TRAVERSE_ALL_CHILDREN){}

			virtual void apply(CRCore::crObject& node);

			void apply(CRCore::crGeometry& geometry);
			//void apply(CRCore::crStateSet& stateset);

		protected:
			typedef std::map< int, int, std::less<int> > VtxIndexMap;
			typedef std::set< int, std::less<int> > ToEraseList;
			void initVtxIndexMap(int size);
			void vtxiRemoved(int i);
			VtxIndexMap m_vtxIndexMap;
			ToEraseList m_toEraseList;
		};

		//class CRUTIL_EXPORT crSplitStateSetVisitor : public CRCore::crNodeVisitor
		//{//OPT_DRAWABLESTATESET
		//public:
		//	crSplitStateSetVisitor():
		//	  CRCore::crNodeVisitor(CRCore::crNodeVisitor::TRAVERSE_ALL_CHILDREN){}

		//	  virtual void apply(CRCore::crObject& node);

		//	  void apply(CRCore::crDrawable& drawable);
		//	  void apply(CRCore::crStateSet& stateset);

		//protected:
		//};

		class CRUTIL_EXPORT crFreezeTransformsVisitor : public CRCore::crNodeVisitor
		{
		public:
			crFreezeTransformsVisitor():
			  CRCore::crNodeVisitor(CRCore::crNodeVisitor::TRAVERSE_ALL_CHILDREN),
			  m_ignorePhysics(true),
			  m_sucess(false){}

			  virtual void apply(CRCore::crMatrixTransform& node);
			  virtual void apply(CRCore::crTransform& node);
			  virtual void apply(CRPhysics::crMatterObject& node);
			  virtual void apply(CRCore::crObject& node);
			  virtual void apply(CRCore::crBillboard& node);
			  virtual void apply(rbody::CreBodyNode& node);
			  typedef std::vector<CRCore::ref_ptr<CRCore::RefMatrix> > MatrixList;
			  inline void pushMatrix( CRCore::RefMatrix* matrix)
			  {
				  m_matrixList.push_back(matrix);
			  }
			  inline void popMatrix()
			  {
				  m_matrixList.pop_back();
			  }
			  inline CRCore::crMatrix topMatrix()
			  {
				  if(m_matrixList.empty())
					  return CRCore::crMatrix::identity();
				  else
					  return *(m_matrixList.back().get());
			  }
			  void setIgnorePhysics(bool bln){ m_ignorePhysics = bln; }
			  typedef std::set< CRCore::ref_ptr<CRCore::crMatrixTransform> > FreezeSet;
			  void freeze();
		protected:
			  MatrixList                              m_matrixList;
			  FreezeSet                               m_freezeSet;
              bool m_ignorePhysics;
			  bool m_sucess;
		};

};

class crAutoTextureMatrix: public CRCore::crStateAttribute 
{
public:
	crAutoTextureMatrix():    
	  CRCore::crStateAttribute(),
		  m_lightnum(0),
		  m_active(false)
	  {
	  }

	  crAutoTextureMatrix(const crAutoTextureMatrix &copy, const CRCore::crCopyOp &copyop):    
	  CRCore::crStateAttribute(copy, copyop),
		  m_lightnum(copy.m_lightnum),
		  m_active(copy.m_active)
	  {
	  }

	  crAutoTextureMatrix(int lightnum, bool active = true):    
	  CRCore::crStateAttribute(),
		  m_lightnum(lightnum),
		  m_active(active)
	  {
	  }

	  META_StateAttribute(CRUtil, crAutoTextureMatrix, CRCore::crStateAttribute::TEXMAT);

	  virtual bool isTextureAttribute() const { return true; }

	  int compare(const CRCore::crStateAttribute &sa) const
	  {
		  COMPARE_StateAttribute_Types(crAutoTextureMatrix, sa);
		  if (m_lightnum < rhs.m_lightnum) return -1;
		  if (m_lightnum > rhs.m_lightnum) return 1;
		  return 0;
	  }

	  int compare(const CRCore::crStateAttribute &sa)
	  {
		  COMPARE_StateAttribute_Types(crAutoTextureMatrix, sa);
		  if (m_lightnum < rhs.m_lightnum) return -1;
		  if (m_lightnum > rhs.m_lightnum) return 1;
		  return 0;
	  }

	  void apply(CRCore::crState &state) const;
private:
	int m_lightnum;
	bool m_active;
};

}

#endif
