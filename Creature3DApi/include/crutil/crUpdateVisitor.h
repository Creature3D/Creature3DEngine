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
#ifndef CRUTIL_CRUPDATEVISITOR_H
#define CRUTIL_CRUPDATEVISITOR_H 1

#include <CRCore/crNodeVisitor.h>
#include <CRCore/crNode.h>
#include <CRCore/crObject.h>
#include <CRCore/crBillboard.h>
#include <CRCore/crLod.h>
#include <CRCore/crSwitch.h>
#include <CRCore/crLightSource.h>
#include <CRCore/crTransform.h>
#include <CRCore/crProjection.h>
//#include <CRCore/crImpostor.h>
#include <CRCore/crOccluderNode.h>
#include <CRCore/crEventGroup.h>
#include <CRCore/crMatrix.h>
#include <CRUtil/crExport.h>
#include <CRCore/crStreamBuf.h>
namespace CRUtil {

/**
 * Basic crUpdateVisitor implementation for animating a scene.
 * This visitor traverses the scene graph, call each nodes appCallback if
 * it exists. 
 */
class CRUTIL_EXPORT crUpdateVisitor : public CRCore::crNodeVisitor
{
    public:

        crUpdateVisitor();
        virtual ~crUpdateVisitor();

        virtual void reset();

        virtual void apply(CRCore::crNode& node)         
		{/* if(node.getVisiable()) */handle_callbacks_and_traverse(node); }
        
        virtual void apply(CRCore::crObject& node)        { /*if(node.getVisiable())*/ handle_geode_callbacks(node); }
        virtual void apply(CRCore::crBillboard& node)    { /*if(node.getVisiable())*/ handle_geode_callbacks(node); }
        //
		virtual void apply(CRCore::crLightSource& node)  { /*if(node.getVisiable())*/ handle_callbacks_and_traverse(node); }
        
        virtual void apply(CRCore::crGroup& node)        { /*if(node.getVisiable())*/ handle_callbacks_and_traverse(node); }
        virtual void apply(CRCore::crTransform& node)    { /*if(node.getVisiable())*/ handle_callbacks_and_traverse(node); }
        virtual void apply(CRCore::crProjection& node)   { /*if(node.getVisiable())*/ handle_callbacks_and_traverse(node); }
        virtual void apply(CRCore::crSwitch& node)       { /*if(node.getVisiable())*/ handle_callbacks_and_traverse(node); }
        virtual void apply(CRCore::crLod& node)          { /*if(node.getVisiable())*/ handle_callbacks_and_traverse(node); }
        //virtual void apply(CRCore::crImpostor& node)     { if(node.getVisiable()) handle_callbacks_and_traverse(node); }
        virtual void apply(CRCore::crOccluderNode& node) { handle_callbacks_and_traverse(node); }

		void setEyePoint(CRCore::crVector3 &eye) { m_eyePoint = eye; }
		virtual CRCore::crVector3 getEyePoint() const{ return m_eyePoint; }

		inline void setViewMatrix(CRCore::crMatrixd* matrix) { m_viewMatrix = matrix; }
        inline void setProjectionMatrix(CRCore::crMatrixd* matrix) { m_projectionMatrix = matrix; }
		inline CRCore::crMatrixd* getViewMatrix() { return m_viewMatrix; }
		inline CRCore::crMatrixd* getProjectionMatrix() { return m_projectionMatrix; }

		//virtual float getDistanceToEyePoint(const CRCore::crVector3& pos, bool) const { return (pos-getEyeLocal()).length(); }
		//virtual float getDistanceFromEyePoint(const CRCore::crVector3& pos, bool) const { return (pos-getEyeLocal()).length(); }
    protected:

//         /** prevent unwanted copy construction.*/
//         crUpdateVisitor(const crUpdateVisitor&):CRCore::crNodeVisitor() {}

        /** prevent unwanted copy operator.*/
        crUpdateVisitor& operator = (const crUpdateVisitor&) { return *this; }
        
		inline void handle_callbacks(CRCore::crStateSet* stateset)
		{
			if (stateset && stateset->requiresUpdateTraversal())
			{
				stateset->runUpdateCallbacks(this);
			}
		}

        inline void handle_callbacks_and_traverse(CRCore::crNode& node)
        {
#ifdef _DEBUG
			try
			{
#endif
			handle_callbacks(node.getStateSet());

			node.doEvent(WCH_UPDATEVISITOR,MAKEINT64(this,NULL));
			CRCore::crNodeCallback* callback = node.getUpdateCallback();
			if (callback) (*callback)(&node,this);
			else if (node.getNumChildrenRequiringUpdateTraversal()>0) traverse(node);
#ifdef _DEBUG
			}
			catch (...)
			{
 				//CRCore::notify(CRCore::ALWAYS)<<"crUpdateVisitor::handle_callbacks_and_traverse error "<<node.getName()<<std::endl;
				char gbuf[256];
				sprintf(gbuf,"crUpdateVisitor::handle_callbacks_and_traverse error %s\n\0",node.getName().c_str());
				gDebugInfo->debugInfo(CRCore::ALWAYS,gbuf);
			}
#endif
        }

        inline void handle_geode_callbacks(CRCore::crObject& node)
        {
			handle_callbacks(node.getStateSet());

			node.doEvent(WCH_UPDATEVISITOR,MAKEINT64(this,NULL));
			CRCore::crNodeCallback* callback = node.getUpdateCallback();
			if (callback) (*callback)(&node,this);

			for(unsigned int i=0;i<node.getNumDrawables();++i)
			{
				CRCore::crDrawable::UpdateCallback* callback = node.getDrawable(i)->getUpdateCallback();
				if (callback) callback->update(this,node.getDrawable(i));

				handle_callbacks(node.getDrawable(i)->getStateSet());
			}
		}

		CRCore::crVector3 m_eyePoint;
		CRCore::crMatrixd* m_viewMatrix;
        CRCore::crMatrixd* m_projectionMatrix;
		
};

//class CRUTIL_EXPORT crAIUpdateVisitor : public CRCore::crNodeVisitor
//{
//public:
//
//	crAIUpdateVisitor();
//	virtual ~crAIUpdateVisitor();
//
//	virtual void apply(CRCore::crEventGroup& node);
//protected:
//
//	crAIUpdateVisitor& operator = (const crAIUpdateVisitor&) { return *this; }
//
//};

}

#endif

