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
#ifndef CRCORE_CRNODE_H
#define CRCORE_CRNODE_H

#include <CRCore/crBase.h>
#include <CRCore/crStateSet.h>
#include <CRCore/crBoundingSphere.h>
#include <CRCore/crBoundingBox.h>
#include <CRCore/crNodeCallback.h>
#include <CRCore/crArray.h>
#include <CRCore/crCollideCallback.h>
#include <CRCore/crEventCallbackManager.h>
#include <CRCore/crDisplaySettings.h>
//#include <CRCore/crEventMsg.h>
//#include <CRCore/crEventGroup.h>
#include <CRCore/thread/crMutex.h>

#include <string>
#include <vector>
#include <map>
//NodeMask
#define AutoTransMask 0x1
#define CulledMask 0x2

namespace CRCore {

    class crNodeVisitor;
	class crGroup;
	class crTransform;

	/** A vector of crNodes pointers which is used to describe the path from a root node to a descendant.*/
	typedef std::vector<crNode*> NodePath;
	/** A vector of NodePath, typically used to describe all the paths from a node to the potential root nodes it has.*/
	typedef std::vector< NodePath > NodePathList;

#define META_Node(library,name) \
	virtual CRCore::crBase* cloneType() const { return new name (); } \
	virtual CRCore::crBase* clone(const CRCore::crCopyOp& copyop) const { return new name (*this,copyop); } \
	virtual bool isSameKindAs(const CRCore::crBase* obj) const { return dynamic_cast<const name *>(obj)!=NULL; } \
	virtual const char* className() const { return #name; } \
	virtual const char* libraryName() const { return #library; } \
	virtual void accept(CRCore::crNodeVisitor& nv) { if (nv.validNodeMask(*this)) { nv.pushOntoNodePath(this); nv.apply(*this); nv.popFromNodePath(); } } \


	/** Base class for all internal nodes in the scene graph.
	Provides interface for most common node operations (Composite Pattern).
	*/
	class CR_EXPORT crNode : public crBase
	{
	public:
		enum NodeType
		{
			NT_NODE,
			NT_OBJECT,
			NT_GROUP,
			NT_SECTOR,
			NT_DB
		};
		NodeType m_NodeType;

		/** Construct a node.
		Initialize the parent list to empty, node name to "" and 
		bounding sphere dirty flag to true.*/
		crNode();

		/** Copy constructor using crCopyOp to manage deep vs shallow copy.*/
		crNode(const crNode&,const crCopyOp& copyop = crCopyOp::SHALLOW_COPY);

		/** clone the an object of the same type as the node.*/
		virtual crBase* cloneType() const { return new crNode(); }

		/** return a clone of a node, with crBase* return type.*/
		virtual crBase* clone(const crCopyOp& copyop) const { return new crNode(*this,copyop); }

		/** return true if this and obj are of the same kind of object.*/
		virtual bool isSameKindAs(const crBase* base) const { return dynamic_cast<const crNode*>(base)!=NULL; }

		/** return the name of the node's library.*/
		virtual const char* libraryName() const { return "CRCore"; }

		/** return the name of the node's class type.*/
		virtual const char* className() const { return "crNode"; }

		/** convert 'this' into a crGroup pointer if Node is a crGroup, otherwise return 0.
		* Equivalent to dynamic_cast<crGroup*>(this).*/
		virtual crGroup* asGroup() { return 0; }
		/** convert 'const this' into a const crGroup pointer if crNode is a crGroup, otherwise return 0.
		* Equivalent to dynamic_cast<const Group*>(this).*/
		virtual const crGroup* asGroup() const { return 0; }

		/** convert 'this' into a Transform pointer if crNode is a crTransform, otherwise return 0.
		* Equivalent to dynamic_cast<Transform*>(this).*/
		virtual crTransform* asTransform() { return 0; }
		/** convert 'const this' into a const crTransform pointer if crNode is a crTransform, otherwise return 0.
		* Equivalent to dynamic_cast<const crTransform*>(this).*/
		virtual const crTransform* asTransform() const { return 0; }

		/** Visitor Pattern : calls the apply method of a crNodeVisitor with this node's type.*/
		virtual void accept(crNodeVisitor& nv);
		/** Traverse upwards : calls parents' accept method with crNodeVisitor.*/
		virtual void ascend(crNodeVisitor& nv);
		/** Traverse downwards : calls children's accept method with crNodeVisitor.*/
		virtual void traverse(crNodeVisitor& nv) {}

		/** A vector of CRCore::crGroup pointers which is used to store the parent(s) of node.*/
		typedef std::vector<crGroup*> ParentArray;

		/** Get the parent list of node. */
		inline const ParentArray& getParents() const { return m_parentArray; }

		/** Get the a copy of parent list of node. A copy is returned to 
		* prevent modification of the parent list.*/
		inline ParentArray& getParents() { return m_parentArray; }

		inline crGroup* getParent(unsigned int i)  { return i<m_parentArray.size()?m_parentArray[i]:NULL; }
		/**
		* Get a single const parent of node.
		* @param i index of the parent to get.
		* @return the parent i.
		*/
		inline const crGroup* getParent(unsigned int i) const  { return i<m_parentArray.size()?m_parentArray[i]:NULL; }

		/**
		* Get the number of parents of node.
		* @return the number of parents of this node.
		*/
		inline unsigned int getNumParents() const { return m_parentArray.size(); }

		/** Get the list of node paths parent paths.
		* The optional Node* haltTraversalAtNode allows the user to prevent traversal beyond a specifed node. */
		NodePathList getParentalNodePaths(CRCore::crNode* haltTraversalAtNode=0) const;

		/** Set update node callback, called during update traversal. */
		inline void setUpdateCallback(crNodeCallback* nc){ addUpdateCallback(nc); }
		void addUpdateCallback(crNodeCallback* nc);
		void removeUpdateCallback(const std::string &name);
		crNodeCallback* getUpdateCallback(const std::string &name);

		/** Get update node callback, called during update traversal. */
		inline crNodeCallback* getUpdateCallback() { return m_refUpdateCallback.get(); }

		/** Get const update node callback, called during update traversal. */
		inline const crNodeCallback* getUpdateCallback() const { return m_refUpdateCallback.get(); }

		/** Get the number of Children of this node which require App traversal,
		* since they have an AppCallback attached to them or their children.*/
		inline unsigned int getNumChildrenRequiringUpdateTraversal() const { return m_nNumChildrenRequiringUpdateTraversal; }
		//inline unsigned int getNumChildrenRequiringUpdateTraversal() { return m_nNumChildrenRequiringUpdateTraversal; }

		///** Set update node callback, called during update traversal. */
		//void setUpdateUniformCallback(crNodeCallback* nc);

		///** Get update node callback, called during update traversal. */
		//inline crNodeCallback* getUpdateUniformCallback() { return m_refUpdateUniformCallback.get(); }

		///** Get const update node callback, called during update traversal. */
		//inline const crNodeCallback* getUpdateUniformCallback() const { return m_refUpdateUniformCallback.get(); }

		///** Get the number of Children of this node which require App traversal,
		//* since they have an AppCallback attached to them or their children.*/
		//inline unsigned int getNumChildrenRequiringUpdateUniformTraversal() const { return m_nNumChildrenRequiringUpdateUniformTraversal; }

		//virtual void updateUniform(){}

		///** Set update node callback, called during update traversal. */
		//void setEventCallback(crNodeCallback* nc);

		///** Get update node callback, called during update traversal. */
		//inline crNodeCallback* getEventCallback() { return m_eventCallback.get(); }

		///** Get const update node callback, called during update traversal. */
		//inline const crNodeCallback* getEventCallback() const { return m_eventCallback.get(); }

		///** Get the number of Children of this node which require Event traversal,
		//* since they have an Event Callback attached to them or their children.*/
		//inline unsigned int getNumChildrenRequiringEventTraversal() const { return m_numChildrenRequiringEventTraversal; }

		/** Set cull node callback, called during cull traversal. */
		void addCullCallback(crNodeCallback* nc);
		inline void setCullCallback(crNodeCallback* nc) { addCullCallback(nc); }
		void removeCullCallback(const std::string &name);
		crNodeCallback* getCullCallback(const std::string &name);

		/** Get cull node callback, called during cull traversal. */
		inline crNodeCallback* getCullCallback() { return m_refCullCallback.get(); }

		/** Get const cull node callback, called during cull traversal. */
		inline const crNodeCallback* getCullCallback() const { return m_refCullCallback.get(); }

		/** Set the view frustum/small feature culling of this node to be active or inactive.
		* The default value to true for m_bCullingActive. Used a guide
		* to the cull traversal.*/
		void setCullingActive(bool active);

		/** Get the view frustum/small feature m_bCullingActive flag for this node. Used a guide
		* to the cull traversal.*/
		inline bool getCullingActive() const { return m_bCullingActive; }

		/** Get the number of Children of this node which have culling disabled.*/
		inline unsigned int getNumChildrenWithCullingDisabled() const { return m_nNumChildrenWithCullingDisabled; }

		/** Return true if this node can be culled by view frustum, occlusion or small feature culling during the cull traversal.
		* note, return true only if no children have culling disabled, and the local _cullingActive flag is true.*/
		inline bool isCullingActive() const { return m_nNumChildrenWithCullingDisabled==0 && m_bCullingActive && getBoundBox().valid(); }

		/** Get the number of Children of this node which are or have OccluderNode's.*/
		inline unsigned int getNumChildrenWithOccluderNodes() const { return m_nNumChildrenWithOccluderNodes; }

		inline unsigned int getNumChildrenWithEventGroups() const { return m_nNumChildrenWithEventGroups; }

		//inline unsigned int getNumChildrenRequiringCalcShadow() const { return m_nNumChildrenRequiringCalcShadow; }

		/** return true if this node is an OccluderNode or the subgraph below this node are OccluderNodes.*/
		bool containsOccluderNodes() const;
		bool containsEventGroups() const;

		typedef unsigned int NodeMask;
		/** Set the node mask. Note, node mask is will be replaced by TraversalMask.*/
		inline void setNodeMask(NodeMask nm) { m_nodeMask = nm; }
		/** Get the node Mask. Note, node mask is will be replaced by TraversalMask.*/
		inline NodeMask getNodeMask() const { return m_nodeMask; }

		/** A vector of std::string's which are used to describe the object.*/
		typedef std::vector<std::string> DescriptionArray;

		/** Get the description list of the const node.*/        
		inline const DescriptionArray& getDescriptions() const { return m_descriptionArray; }
		/** Get the description list of the const node.*/        
		inline DescriptionArray& getDescriptions() { return m_descriptionArray; }
		/** Get a single const description of the const node.*/
		inline const std::string& getDescription(unsigned int i) const { return m_descriptionArray[i]; }
		/** Get a single description of the node.*/
		inline std::string& getDescription(unsigned int i) { return m_descriptionArray[i]; }
		/** Get the number of descriptions of the node.*/
		inline unsigned int getNumDescriptions() const { return m_descriptionArray.size(); }
		/** Add a description string to the node.*/
		void addDescription(const std::string& desc) { m_descriptionArray.push_back(desc); }
		std::string getDescriptionsInString()const;
		void setDescriptionsInString( const std::string &command );
		/** set the node's crStateSet.*/
		inline virtual void setStateSet(CRCore::crStateSet* dstate);

		/** return the node's crStateSet, if one does not already exist create it
		* set the node and return the newly created crStateSet. This ensures
		* that a valid crStateSet is always returned and can be used directly.*/
		CRCore::crStateSet* getOrCreateStateSet();

		/** return the node's crStateSet. returns NULL if a stateset is not attached.*/
		inline CRCore::crStateSet* getStateSet() { return m_stateset.get(); }

		/** return the node's const crStateSet. returns NULL if a stateset is not attached.*/
		inline const CRCore::crStateSet* getStateSet() const { return m_stateset.get(); }

		/** get the bounding sphere of node.
		Using lazy evaluation computes the bounding sphere if it is 'dirty'.*/
		inline const crBoundingSphere& getBound() const
		{
			if(!m_boundSetted && !m_bBoundSphere_computed) computeBound();
			return m_boundSphere;
		}

		inline crBoundingSphere& getBound()
		{
			if(!m_boundSetted && !m_bBoundSphere_computed) computeBound();
			return m_boundSphere;
		}

		inline const crBoundingBox& getBoundBox() const
		{
			if(!m_boundSetted && !m_bBoundSphere_computed)
				computeBound();
			return m_bbox;
		}

		//inline crBoundingBox& getBoundBox()
		//{
		//	if(!m_boundSetted  && !m_bBoundSphere_computed)
		//		computeBound();
		//	return m_bbox;
		//}

		/** Mark this node's bounding sphere dirty.
		Forcing it to be computed on the next call to getBound().*/
		virtual void dirtyBound();
        
		//inline void setCulled( unsigned char culled ){ m_culled = culled; }
		//inline unsigned char getCulled() { return m_culled; }
		//inline unsigned char getCulled() const { return m_culled; }

		inline void lock(){ m_mutex.lock(); }
		inline void unlock(){ m_mutex.unlock(); }
		inline crMutex &getNodeMutex() { return m_mutex; }

		virtual void releaseObjects(crState* = 0);
		virtual void releaseOde();

		void setPhysicsMeshVertexArray(CRCore::Vec3Array *array){ m_physicsMeshVtxArray = array; }
		void setPhysicsMeshIndexArray(CRCore::UIntArray *array){ m_physicsMeshIndexArray = array; }

		//inline void showBoundingBox( bool bShow ){ m_isShowBoundingBox = bShow; }
		//inline bool isShowBoundingBox()const{ return m_isShowBoundingBox; }

		inline virtual void setEnableIntersect( bool enableIntersect){ m_enableIntersect = enableIntersect; }
		inline virtual bool getEnableIntersect()const{ return crDisplaySettings::instance()->getRunMode() == 0 ? m_editorIntersectable&&!m_editorHidden : m_enableIntersect; }
		
		inline virtual void setAvoidIntersect( bool avoidIntersect){ m_avoidIntersect = avoidIntersect; }
		inline virtual bool getAvoidIntersect()const{ return crDisplaySettings::instance()->getRunMode() == 0 ? m_avoidIntersect&&!m_editorHidden : m_avoidIntersect; }

		inline void setCalcShadow( bool calcShadow ){ m_isCalcShadow = calcShadow; }
		inline bool isCalcShadow()const{ return m_isCalcShadow; }
        
		inline virtual void setVisiable( bool visiable ){ m_visiable = visiable; }
		inline bool getVisiable()const{ return crDisplaySettings::instance()->getRunMode() == 0 ? !m_editorHidden : m_visiable && m_renderable; }
		inline bool getVisiable2()const{ return crDisplaySettings::instance()->getRunMode() == 0 ? !m_editorHidden : m_visiable; }

		inline void setRenderable( bool renderable ){ m_renderable = renderable; }
		inline bool getRenderable()const{ return crDisplaySettings::instance()->getRunMode() == 0 ? true : m_renderable; }
		inline bool getRenderable2()const{ return m_renderable; }

		inline void setPhysicsInited(bool inited) { m_physicsInited = inited; }
		inline bool getPhysicsInited()const { return m_physicsInited; }
		inline void setMapCollide(bool enable) { m_mapCollide = enable; }
		inline bool isMapCollide()const { return m_mapCollide; }
		inline void setGIMapRenderable(bool enable) { m_giMapRenderable = enable; }
		inline bool isGIMapRenderable()const { return m_giMapRenderable; }

		void setEditorHidden(bool hide) { m_editorHidden = hide; }
		void setEditorIntersectable(bool able) { m_editorIntersectable = able; }

		enum CollideMode
		{
			CollideNone,
			PrecisionCollide,
			CollideWithBoundBox,
			CollideWithBoundSphere,
			AvoidCollide
		};

		void setCollideMode( CollideMode cmode ){ m_collideMode = cmode; }
		CollideMode getCollideMode()const{ return m_collideMode; }

		void setPhysicsObjID( void* id) { m_physicsObjID = id; }
		void* getPhysicsObjID()const { return m_physicsObjID; }
		void setHeightfieldDataID(void *id);

		//void physicsCulled(bool culled);

		inline void setCollideCallback(crCollideCallback* nc){ addCollideCallback(nc); }
		void addCollideCallback(crCollideCallback* nc);
		void removeCollideCallback(const std::string &name);
		inline crCollideCallback* getCollideCallback() { return m_collideCallback.get(); }
		crCollideCallback* getCollideCallback(const std::string &name);

		void setBoundBox(const crVector3& center,const crVector3& length);
		inline bool getBoundSetted() const { return m_boundSetted; }

		inline virtual void swapBuffers(int frameNumber)
		{//剪裁线程与绘制线程的同步
			 if(m_swapFrameNumber != frameNumber)
			 {
				 if(m_stateset.valid()) m_stateset->swapBuffers(frameNumber);
				 if(m_refUpdateCallback.valid()) m_refUpdateCallback->swapBuffers(frameNumber);
				 if(m_refCullCallback.valid()) m_refCullCallback->swapBuffers(frameNumber);
				 if(m_dataClass.valid() && dynamic_cast<crNodeData *>(m_dataClass.get()))
				 {
					 (dynamic_cast<crNodeData *>(m_dataClass.get()))->swapBuffers(frameNumber);
				 }
				 m_swapFrameNumber = frameNumber;
			 }
		}

		class PhysicsObjectCallback : public CRCore::crEventCallback
		{
		public:
			PhysicsObjectCallback(){}
			virtual crEventCallback* cloneType() const = 0;
			virtual crEventCallback* clone() const = 0;
			virtual bool isSameKindAs(const crEventCallback* obj) const { return true; }
			virtual const char* libraryName() const = 0;
			virtual const char* className() const = 0;
			virtual const char* _name() const = 0;

			PhysicsObjectCallback(const PhysicsObjectCallback& callback):CRCore::crEventCallback(callback){}

			virtual void operator()(crNode &node, __int64 param = 0){};
		protected:
			virtual ~PhysicsObjectCallback(){}
		};
		typedef std::multimap< __int64, CRCore::ref_ptr<CRCore::crEventCallback> > EventMap;
		void insertEvent(__int64 kbmsg, CRCore::crEventCallback *callback);
		EventMap &getEventMap();
		inline bool hasMsgEvent(__int64 kbmsg) const { return m_eventMap.empty()?false:m_eventMap.find(kbmsg) != m_eventMap.end(); }
		crEventCallback* getEventCallback(__int64 kbmsg, const std::string& name);

		virtual void doEvent(__int64 kbmsg, __int64 param = 0);
		inline bool hasEvent() const { return !m_eventMap.empty()||m_dataClass.valid(); }

		virtual void setDataClass( CRCore::crData *data );
		void setRenderInited(bool bln);
		bool getRenderInited();
		void setNumChildrenRequiringUpdateTraversal(unsigned int num);
		virtual void preRemoveNode(){};
	protected:

		/** Node destructor. Note, is protected so that Nodes cannot
		be deleted other than by being dereferenced and the reference
		count being zero (see CRCore::Referenced), preventing the deletion
		of nodes which are still in use. This also means that
		Node's cannot be created on stack i.e Node node will not compile,
		forcing all nodes to be created on the heap i.e Node* node
		= new Node().*/
		virtual ~crNode();


		/** Compute the bounding sphere around Node's geometry or children.
		This method is automatically called by getBound() when the bounding
		sphere has been marked dirty via dirtyBound().*/
		virtual bool computeBound() const;

		mutable crBoundingSphere    m_boundSphere;
		mutable crBoundingBox       m_bbox;
        
		mutable bool m_bBoundSphere_computed;
		bool    m_boundSetted;

		void addParent(CRCore::crGroup* node);
		void removeParent(CRCore::crGroup* node);

		ParentArray m_parentArray;
		friend class CRCore::crGroup;
		friend class CRCore::crDrawable;
		friend class CRCore::crStateSet;

		ref_ptr<crNodeCallback> m_refUpdateCallback;
		unsigned int m_nNumChildrenRequiringUpdateTraversal;

		//ref_ptr<crNodeCallback> m_refUpdateUniformCallback;
		//unsigned int m_nNumChildrenRequiringUpdateUniformTraversal;
		//void setNumChildrenRequiringUpdateUniformTraversal(unsigned int num);

		//ref_ptr<crNodeCallback> m_eventCallback;
		//unsigned int m_numChildrenRequiringEventTraversal;
		//void setNumChildrenRequiringEventTraversal(unsigned int num);

		ref_ptr<crNodeCallback> m_refCullCallback;
		ref_ptr<crCollideCallback>  m_collideCallback;

		bool m_bCullingActive;
		unsigned int m_nNumChildrenWithCullingDisabled;        
		void setNumChildrenWithCullingDisabled(unsigned int num);

		unsigned int m_nNumChildrenWithOccluderNodes;        
		void setNumChildrenWithOccluderNodes(unsigned int num);

		unsigned int m_nNumChildrenWithEventGroups;
		void setNumChildrenWithEventGroups(unsigned int num);

		//unsigned int m_nNumChildrenRequiringCalcShadow;
		//void setNumChildrenRequiringCalcShadow(unsigned int num);

		NodeMask m_nodeMask;

		DescriptionArray m_descriptionArray;

		ref_ptr<crStateSet> m_stateset;

		//unsigned char m_culled;//－1：初始值，0：没有被剪裁  1：被剪裁
		crMutex                  m_mutex;
        
        
		//bool m_isShowBoundingBox;
        
		//for physics mesh collide
		CRCore::ref_ptr<CRCore::Vec3Array> m_physicsMeshVtxArray;
		CRCore::ref_ptr<CRCore::UIntArray> m_physicsMeshIndexArray;
        
		bool m_visiable;//false 不绘制，不可拾取
		bool m_enableIntersect;//false 不启用物理碰撞
        bool m_isCalcShadow;//false 不计算其造成的实时阴影
		bool m_renderable;//false表示该节点不能被渲染所以不需要进行crRenderInitVisitor
        bool m_editorHidden;//编辑器隐藏
		bool m_editorIntersectable;//编辑器拾取标志
		bool m_avoidIntersect;//true 不启用拾取
		bool m_giMapRenderable;//默认true，false表示giMapRender不渲染该对象

		CollideMode    m_collideMode;
		mutable void*          m_physicsObjID;
		mutable void*          m_heightData;
		mutable bool m_physicsInited;
		bool m_mapCollide;

		EventMap m_eventMap;
		bool m_renderInited;
	};
}

#endif