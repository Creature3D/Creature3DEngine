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
#ifndef CRCORE_NODEVISITOR_H
#define CRCORE_NODEVISITOR_H 1

#include <CRCore/crNode.h>
#include <CRCore/crMatrix.h>
#include <CRCore/crFrameStamp.h>
#include <CRCore/crPolytope.h>

namespace CRTerrain
{
	class crTerrainTile;
	class crTerrain;
}

namespace CRPhysics
{
	class crMatterGroup;
	class crMatterObject;
	class crBoxMatterObject;
	class crSphereMatterObject;
	class crCCylinderMatterObject;
	class crMeshMatterObject;
	class crRayMatterObject;
	class crRadarMatterObject;

	class crWeaponMatterObject;
	class crBulletMatterObject;
    class crCaissonMatterObject;
    class crHpMatterObject;
    class crArmorMatterObject;

	class crViewMatterObject;
	class crAircraftMatterObject;
	class crCarMatterObject;
	class crCharacterMatterObject;
	class crMissileSetOrFlackMatterObject;
	class crBodyCaseMatterObject;

	class crVolumeNode;
	class crPolytopeVolumeNode;
	class crMatrixVolumeNode;
	class crVolumeNodeManager;
	class crCameraMatterObject;
	class crCameraMatterObjectManager;
}

//namespace CRAI
//{
//	class crKeyboardMouseEventGroup;
//	class crNerveCellGroup;
//	class crAIGroup;
//}

namespace rbody
{
	class CreBodyNode;
}

namespace CRParticle
{
	class crParticleEffect;
}

namespace CRUI
{
	class crButtonWidgetNode;
	class crScrollBarWidgetNode;
	class crTextAttrWidgetNode;
	class crStaticTextWidgetNode;
	class crHypertextWidgetNode;
	class crEditWidgetNode;
	class crListBoxWidgetNode;
	class crImageBoxWidgetNode;
	class crCheckBoxWidgetNode;
	class crRadioWidgetNode;
	class crRadioGroupWidgetNode;
	class crProgressWidgetNode;
	class crSliderWidgetNode;
	class crComboBoxWidgetNode;
	class crChooseColorWidgetNode;
	class crTableWidgetNode;
	class crListControlWidgetNode;
}

namespace CRCore {
class crDB;
class crObject;
class crGroup;
class crBillboard;
//class crDepthSortedObject;
class crLightSource;
class crClipNode;
class crTransform;
class crMatrixTransform;
class crPositionAttitudeTransform;
class crProjection;
class crLod;
class crPagedLOD;
class crSwitch;
class crMultiSwitch;
//class crImpostor;
class crClearNode;
class crOccluderNode;
class crSequence;
class crMatrixSequence;
class crEventGroup;
class crCoordinateSystemNode;
class crProxyNode;
class crTexGenNode;
class crExternNode;
class crCameraNode;
class crTraverseStringNode;
//class crWaterSimulationNode;
//class crWaterReflectNode;
class crRenderToTextureManager;
class crFilterRenderManager;
class crEffectSequenceManager;
class crDOFTransform;
class crWidgetNode;
class crCanvasNode;
/** Visitor for type safe operations on CRCore::crNode's.
    Based on GOF's Visitor pattern. The crNodeVisitor 
    is useful for developing type safe operations to nodes
    in the scene graph (as per Visitor pattern), and adds to this
    support for optional scene graph traversal to allow
    operations to be applied to whole scenes at once. The Visitor
    pattern uses a technique of double dispatch as a mechanism to
    called the appropriate apply(..) method of the crNodeVisitor.  To
    use this feature one must use the crNode::accept(crNodeVisitor) which
    is extended in each crNode subclass, rather than the crNodeVisitor
    apply directly.  So use root->accept(myVisitor); instead of
    myVisitor.apply(*root).  The later method will bypass the double
    dispatch and the appropriate crNodeVisitor::apply(..) method will
    not be called. */ 
class CR_EXPORT crNodeVisitor : public virtual Referenced
{
    public:

        enum TraversalMode
        {
            TRAVERSE_NONE,
            TRAVERSE_PARENTS,
            TRAVERSE_ALL_CHILDREN,
            TRAVERSE_ACTIVE_CHILDREN
        };
        
        enum VisitorType
        {
            NODE_VISITOR = 0,
			INIT_VISITOR,
			OPT_PHYSICSCHECK_VISITOR,
            UPDATE_VISITOR,
			UPDATE_UNIFORM_VISITOR,
			EVENT_VISITOR,
            COLLECT_OCCLUDER_VISITOR,
            CULL_VISITOR,
			BROADCASTEVENTMSGVISITOR,
			UNITENODESVISITOR,
			PHYSICS_UPDATE_VISITOR,
			AI_UPDATE_VISITOR,
			SWAPBUFFER_VISITOR,
			NET_UPDATE_VISITOR
        };

        crNodeVisitor(TraversalMode tm=TRAVERSE_NONE);
        
        crNodeVisitor(VisitorType type,TraversalMode tm=TRAVERSE_NONE);
        
        virtual ~crNodeVisitor();

        /** Method to call to reset visitor. Useful for your visitor accumulates
            state during a traversal, and you plan to reuse the visitor. 
            To flush that state for the next traversal than call reset() prior
            to each traversal.*/
        virtual void reset() {}


        /** Set the VisitorType, used to distingush different visitors during
          * traversal of the scene, typically used in the crNode::traverse() method
          * to select which behaviour to use for different types of traversal/visitors.*/
        inline void setVisitorType(VisitorType type) { m_visitorType = type; }
        
        /** Get the VisitorType.*/
        inline VisitorType getVisitorType() const { return m_visitorType; }

        /** Set the traversal number. Typically used to denote the frame count.*/
        inline void setTraversalNumber(int fn) { m_traversalNumber = fn; }
        
        /** Get the traversal number. Typically used to denote the frame count.*/
        inline int getTraversalNumber() const { return m_traversalNumber; }

        ///** Set the crFrameStamp that this traversal is assoicated with.*/
        //inline void setFrameStamp(crFrameStamp* fs) { m_frameStamp = fs; }
        //
        ///** Get the crFrameStamp that this traversal is assoicated with.*/
        //inline const crFrameStamp* getFrameStamp() const { return m_frameStamp.get(); }


        /** Set the TraversalMask of this crNodeVisitor.
          * The TraversalMask is used by the crNodeVisitor::validNodeMask() method
          * to determine whether to operate on a node and its subgraph.
          * validNodeMask() is called automaticaly in the crNode::accept() method before
          * any call to crNodeVisitor::apply(), apply() is only ever called if validNodeMask
          * returns true. Note, if crNodeVisitor::m_traversalMask is 0 then all operations
          * will be swithced off for all nodes.  Whereas setting both m_traversalMask and
          * m_nodeMaskOverride to 0xffffffff will allow a visitor to work on all nodes
          * regardless of their own crNode::_nodeMask state.*/
		inline void setTraversalMask(crNode::NodeMask mask) { m_traversalMask = mask; }

        /** Get the TraversalMask.*/
		inline crNode::NodeMask getTraversalMask() const { return m_traversalMask; }

        /** Set the NodeMaskOverride mask.
          * Used in validNodeMask() to determine whether to operate on a node or its 
          * subgraph, by OR'ing crNodeVisitor::m_nodeMaskOverride with the crNode's own crNode::_nodeMask.
          * Typically used to force on nodes which may have
          * been switched off by their own crNode::_nodeMask.*/
        inline void setNodeMaskOverride(crNode::NodeMask mask) { m_nodeMaskOverride = mask; }

        /** Get the NodeMaskOverride mask.*/
        inline crNode::NodeMask getNodeMaskOverride() const { return m_nodeMaskOverride; }
        
        /** Method to called by crNode and its subclass' crNode::accept() method, if the result is true
          * to be used to cull operations of nodes and their subgraphs.
          * Return true if the result of a bit wise and of the crNodeVisitor::m_traversalMask
          * with the bit or between NodeVistor::m_nodeMaskOverride and the crNode::_nodeMask.
          * default values for m_traversalMask is 0xffffffff, m_nodeMaskOverride is 0x0,
          * and CRCore::crNode::_nodeMask is 0xffffffff. */
        inline bool validNodeMask(const CRCore::crNode& node) const
        {
            return (getTraversalMask() & (getNodeMaskOverride() | node.getNodeMask()))!=0;
        }
        /** Set the traversal mode for crNode::traverse() to use when 
            deciding which children of a node to traverse. If a
            crNodeVisitor has been attached via setTraverseVisitor()
            and the new mode is not TRAVERSE_VISITOR then the attached
            visitor is detached. Default mode is TRAVERSE_NONE.*/
        inline void setTraversalMode(TraversalMode mode) { m_traversalMode = mode; }
        
        /** Get the traversal mode.*/
        inline TraversalMode getTraversalMode() const { return m_traversalMode; }

        /**
         * Set user data, data must be subclased from Referenced to allow
         * automatic memory handling.  If you own data isn't directly 
         * subclassed from Referenced then create and adapter object
         * which points to your own objects and handles the memory addressing.
         */
        inline void setUserData(Referenced* obj) { m_userData = obj; }
        
        /** Get user data.*/
        inline Referenced* getUserData() { return m_userData.get(); }
        
        /** Get const user data.*/
        inline const Referenced* getUserData() const { return m_userData.get(); }


        /** Method for handling traversal of a nodes.
            If you intend to use the visitor for actively traversing 
            the scene graph then make sure the accept() methods call
            this method unless they handle traversal directly.*/
        inline void traverse(crNode& node)
        {
            if (m_traversalMode==TRAVERSE_PARENTS) node.ascend(*this);
            else if (m_traversalMode!=TRAVERSE_NONE) node.traverse(*this);
        }
        
        /** Method called by CRCore::crNode::accept() method before
          * a call the crNodeVisitor::apply(..).  The back of the list will,
          * therefore, be the current node being visited inside the apply(..),
          * and the rest of the list will be the parental sequence of nodes 
          * from the top most node applied down the graph to the current node.
          * Note, the user does not typically call pushNodeOnPath() as it
          * will be called automatically by the crNode::accept() method.*/
        inline void pushOntoNodePath(crNode* node) { m_nodePath.push_back(node); }
        
        /** Method callby CRCore::crNode::accept() method after
          * a call the crNodeVisitor::apply(..).
          * Note, the user does not typically call pushNodeOnPath() as it
          * will be called automatically by the crNode::accept() method.*/
        inline void popFromNodePath()            { m_nodePath.pop_back(); }
        
        /** Get the non const NodePath from the top most node applied down
          * to the current crNode being visited.*/
        NodePath& getNodePath() { return m_nodePath; }

        /** Get the const NodePath from the top most node applied down
          * to the current crNode being visited.*/
        const NodePath& getNodePath() const { return m_nodePath; }
        
        /** Get the eye point in local coordinates.
          * Note, not all crNodeVisitor implement this method, it is mainly cull visitors which will implement.*/
        virtual CRCore::crVector3 getEyePoint() const { return crVector3(0.0f,0.0f,0.0f); }

        /** Get the distance from a point to the eye point, distance value in local coordinate system.
          * Note, not all crNodeVisitor implement this method, it is mainly cull visitors which will implement.
          * If the getDistanceToEyePoint(pos) is not implmented than a default value of 0.0 is returned.*/
        virtual float getDistanceToEyePoint(const crVector3& /*pos*/, bool /*useLODScale*/) const { return -1.0f; }

        /** Get the distance of a point from the eye point, distance value in the eye coordinate system.
          * Note, not all crNodeVisitor implement this method, it is mainly cull visitors which will implement.
          * If the getDistianceFromEyePoint(pos) is not implmented than a default value of 0.0 is returned.*/
        virtual float getDistanceFromEyePoint(const crVector3& /*pos*/, bool /*useLODScale*/) const { return -1.0f; }

        virtual void apply(crNode& node)                      { traverse(node); }
      
        virtual void apply(crObject& node)                    { apply((crNode&)node); }
        
        virtual void apply(crGroup& node)                     { apply((crNode&)node); }

		virtual void apply(crDB& node)                        { traverse((crNode&)node); }

		virtual void apply(crRenderToTextureManager& node)    { apply((crNode&)node); }
		virtual void apply(crFilterRenderManager& node) { apply((crNode&)node); }
		virtual void apply(crEffectSequenceManager& node) { apply((crNode&)node); }
		virtual void apply(CRParticle::crParticleEffect& node) { apply((crGroup&)node); }

        virtual void apply(crProjection& node)                { apply((crGroup&)node); }
        virtual void apply(crBillboard& node)                 { apply((crObject&)node); }
		//virtual void apply(crDepthSortedObject& node)         { apply((crObject&)node); }
		virtual void apply(crProxyNode& node)                 { apply((crGroup&)node); }
		virtual void apply(crExternNode& node)                { apply((crGroup&)node); }

        virtual void apply(crClipNode& node)                  { apply((crGroup&)node); }
		virtual void apply(crTexGenNode& node)                { apply((crGroup&)node); }
        virtual void apply(crLightSource& node)               { apply((crGroup&)node); }

        virtual void apply(crTransform& node)                 { apply((crGroup&)node); }
        virtual void apply(crMatrixTransform& node)           { apply((crTransform&)node); }
        virtual void apply(crPositionAttitudeTransform& node) { apply((crTransform&)node); }
		virtual void apply(crDOFTransform& node)              { apply((crTransform&)node); }
		virtual void apply(crCameraNode& node)                { apply((crTransform&)node); }
		virtual void apply(crTraverseStringNode& node)         { apply((crGroup&)node); }
		//virtual void apply(crWaterSimulationNode& node)       { apply((crGroup&)node); }
		//virtual void apply(crWaterReflectNode& node)          { apply((crMatrixTransform&)node); }

        virtual void apply(crSwitch& node)                    { apply((crGroup&)node); }
		virtual void apply(crMultiSwitch& node)                    { apply((crGroup&)node); }
        virtual void apply(crSequence& node)                  { apply((crGroup&)node); }
		virtual void apply(crMatrixSequence& node)            { apply((crSequence&)node); }
        virtual void apply(crLod& node)                       { apply((crGroup&)node); }
        virtual void apply(crPagedLOD& node)                  { apply((crLod&)node); }
     //   virtual void apply(crImpostor& node)                  { apply((crLod&)node); }
        virtual void apply(crClearNode& node)                 { apply((crGroup&)node); }
        virtual void apply(crOccluderNode& node)              { apply((crGroup&)node); }
        
		virtual void apply(crEventGroup& node)                { apply((crGroup&)node); }
		virtual void apply(CRPhysics::crMatterGroup& node)    { apply((crEventGroup&)node); }
		virtual void apply(CRPhysics::crMatterObject& node)   { apply((crMatrixTransform&)node); }
		virtual void apply(CRPhysics::crBoxMatterObject& node){ apply((CRPhysics::crMatterObject&)node); }
		virtual void apply(CRPhysics::crSphereMatterObject& node){ apply((CRPhysics::crMatterObject&)node); }
		virtual void apply(CRPhysics::crCCylinderMatterObject& node){ apply((CRPhysics::crMatterObject&)node); }
		virtual void apply(CRPhysics::crMeshMatterObject& node){ apply((CRPhysics::crMatterObject&)node); }
		virtual void apply(CRPhysics::crRayMatterObject& node){ apply((CRPhysics::crMatterObject&)node); }
		virtual void apply(CRPhysics::crRadarMatterObject& node){ apply((CRPhysics::crMatterObject&)node); }

		virtual void apply(CRPhysics::crWeaponMatterObject& node){ apply((CRPhysics::crMatterObject&)node); }
		virtual void apply(CRPhysics::crBulletMatterObject& node){ apply((CRPhysics::crMatterObject&)node); }
		virtual void apply(CRPhysics::crCaissonMatterObject& node){ apply((CRPhysics::crMatterObject&)node); }
		virtual void apply(CRPhysics::crHpMatterObject& node){ apply((CRPhysics::crMatterObject&)node); }
		virtual void apply(CRPhysics::crArmorMatterObject& node){ apply((CRPhysics::crMatterObject&)node); }

		virtual void apply(CRPhysics::crViewMatterObject& node){ apply((CRPhysics::crMatterObject&)node); }
		virtual void apply(CRPhysics::crAircraftMatterObject& node){ apply((CRPhysics::crViewMatterObject&)node); }
		virtual void apply(CRPhysics::crCarMatterObject& node){ apply((CRPhysics::crViewMatterObject&)node); }
		virtual void apply(CRPhysics::crCharacterMatterObject& node){ apply((CRPhysics::crViewMatterObject&)node); }
		virtual void apply(CRPhysics::crMissileSetOrFlackMatterObject& node){ apply((CRPhysics::crViewMatterObject&)node); }
		virtual void apply(CRPhysics::crBodyCaseMatterObject& node){ apply((CRPhysics::crViewMatterObject&)node); }

		virtual void apply(CRPhysics::crVolumeNode& node){ apply((crGroup&)node); }
		virtual void apply(CRPhysics::crPolytopeVolumeNode& node){ apply((CRPhysics::crVolumeNode&)node); }
		virtual void apply(CRPhysics::crMatrixVolumeNode& node){ apply((CRPhysics::crVolumeNode&)node); }
		virtual void apply(CRPhysics::crVolumeNodeManager& node){ apply((crNode&)node); }
		virtual void apply(CRPhysics::crCameraMatterObject& node){ apply((CRPhysics::crSphereMatterObject&)node); }
		virtual void apply(CRPhysics::crCameraMatterObjectManager& node){ apply((crNode&)node); }
		
		virtual void apply(rbody::CreBodyNode& node){ apply((crGroup&)node); }

		//virtual void apply(CRAI::crKeyboardMouseEventGroup& node) { apply((crEventGroup&)node); }
		//virtual void apply(CRAI::crNerveCellGroup& node)      { apply((crEventGroup&)node); }
		//virtual void apply(CRAI::crAIGroup& node)             { apply((CRAI::crNerveCellGroup&)node); }

		virtual void apply(crCoordinateSystemNode& node)      { apply((crGroup&)node); }

		virtual void apply(CRTerrain::crTerrainTile& node){ apply((crGroup&)node); }
		virtual void apply(CRTerrain::crTerrain& node){ apply((crGroup&)node); }

		//3DUI Node
		virtual void apply(crWidgetNode& node)                { apply((crMatrixTransform&)node); }
		virtual void apply(crCanvasNode& node)                { apply((crWidgetNode&)node); }
		virtual void apply(CRUI::crButtonWidgetNode& node){ apply((crWidgetNode&)node); }
		virtual void apply(CRUI::crScrollBarWidgetNode& node) { apply((crWidgetNode&)node); }
		virtual void apply(CRUI::crTextAttrWidgetNode& node){ apply((crWidgetNode&)node); }
		virtual void apply(CRUI::crStaticTextWidgetNode& node){ apply((CRUI::crTextAttrWidgetNode&)node); }
		virtual void apply(CRUI::crHypertextWidgetNode& node){ apply((CRUI::crTextAttrWidgetNode&)node); }
		virtual void apply(CRUI::crEditWidgetNode& node){ apply((CRUI::crStaticTextWidgetNode&)node); }
		virtual void apply(CRUI::crListBoxWidgetNode& node){ apply((CRUI::crStaticTextWidgetNode&)node); }
		virtual void apply(CRUI::crImageBoxWidgetNode& node){ apply((crWidgetNode&)node); }
		virtual void apply(CRUI::crCheckBoxWidgetNode& node){ apply((crWidgetNode&)node); }
		virtual void apply(CRUI::crRadioWidgetNode& node){ apply((crWidgetNode&)node); }
		virtual void apply(CRUI::crRadioGroupWidgetNode& node){ apply((crWidgetNode&)node); }
		virtual void apply(CRUI::crProgressWidgetNode& node){ apply((crWidgetNode&)node); }
		virtual void apply(CRUI::crSliderWidgetNode& node){ apply((CRUI::crProgressWidgetNode&)node); }
		virtual void apply(CRUI::crComboBoxWidgetNode& node){ apply((crWidgetNode&)node); }
		virtual void apply(CRUI::crChooseColorWidgetNode& node){ apply((crWidgetNode&)node); }
		virtual void apply(CRUI::crTableWidgetNode& node){ apply((crWidgetNode&)node); }
		virtual void apply(CRUI::crListControlWidgetNode& node){ apply((crWidgetNode&)node); }

        /** callback for managing database paging, such as generated by crPagedLOD nodes.*/
        class DatabaseRequestHandler : public CRCore::Referenced
        {
        public:            
            virtual void requestNodeFile(const std::string& fileName,CRCore::crGroup* group, float priority/*, const crFrameStamp* framestamp*/) = 0;
            
        protected:
            virtual ~DatabaseRequestHandler() {}
        };
        
        /** Set the handler for database requests.*/
        void setDatabaseRequestHandler(DatabaseRequestHandler* handler) { m_databaseRequestHandler = handler; }
        
        /** Get the handler for database requests.*/
        DatabaseRequestHandler* getDatabaseRequestHandler() { return m_databaseRequestHandler.get(); }

        /** Get the const handler for database requests.*/
        const DatabaseRequestHandler* getDatabaseRequestHandler() const { return m_databaseRequestHandler.get(); }

        inline virtual void clearExternFrustum(){}
		inline virtual void addExternFrustum(const CRCore::crPolytope& frustum){}
		inline virtual void clearExternOccluderFrustum(){}
        inline virtual void addExternOccluderFrustum(const CRCore::crPolytope& frustum){}
		inline void setTraverseString(const std::string &traversalStr){ m_traversalString = traversalStr; }
		inline const std::string &getTraverseString(){ return m_traversalString; }

		/** Callback for managing database paging, such as generated by PagedLOD nodes.*/
		class ImageRequestHandler : public CRCore::Referenced
		{
		public:

			ImageRequestHandler(){}

			  virtual double getPreLoadTime() const = 0;

			  virtual CRCore::crImage* readImageFile(const std::string& fileName) = 0;

			  virtual void requestImageFile(const std::string& fileName,crBase* attachmentPoint, int attachmentIndex, double timeToMergeBy, const crFrameStamp* framestamp) = 0;

		protected:
			virtual ~ImageRequestHandler() {}
		};

		/** Set the handler for image requests.*/
		void setImageRequestHandler(ImageRequestHandler* handler) { m_imageRequestHandler = handler; }

		/** Get the handler for image requests.*/
		ImageRequestHandler* getImageRequestHandler() { return m_imageRequestHandler.get(); }

		/** Get the const handler for image requests.*/
		const ImageRequestHandler* getImageRequestHandler() const { return m_imageRequestHandler.get(); }
    protected:

        VisitorType                     m_visitorType;
        int                             m_traversalNumber;
        
        //ref_ptr<crFrameStamp>           m_frameStamp;
        
        TraversalMode                   m_traversalMode;
        crNode::NodeMask                m_traversalMask;
        crNode::NodeMask                m_nodeMaskOverride;
        
        NodePath                        m_nodePath;

        ref_ptr<Referenced>             m_userData;

        ref_ptr<DatabaseRequestHandler> m_databaseRequestHandler;
		ref_ptr<ImageRequestHandler>    m_imageRequestHandler;

		std::string              m_traversalString;

};


/** Convenience functor for assisting visiting of arrays of CRCore::crNode's.*/ 
struct crNodeAcceptOp
{
    crNodeVisitor& m_nv;
    crNodeAcceptOp(crNodeVisitor& nv):m_nv(nv) {}
	void operator () (crNode* node) { node?node->accept(m_nv):NULL; }
	void operator () (ref_ptr<crNode> node) { node.valid()?node->accept(m_nv):NULL; }
};

}

#endif
