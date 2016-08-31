/* Creature3D - Online Game Engine, Copyright (C) 2005 吴财华(26756325@qq.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
*/
#ifndef CRUTIL_CRRENDERINITVISITOR_H
#define CRUTIL_CRRENDERINITVISITOR_H 1

#include <CRUtil/crExport.h>
#include <CRCore/crNodeVisitor.h>
#include <CRCore/crObject.h>
#include <CRCore/crBillboard.h>
//#include <CRCore/crDepthSortedObject.h>
#include <CRCore/crState.h>
#include <CRCore/crLightSource.h>
#include <CRCore/crEventGroup.h>
#include <CRCore/crEventMsg.h>
#include <CRCore/crCameraNode.h>
#include <CRCore/crClipNode.h>
#include <CRParticle/crParticleSystem.h>

//#include <CRUtil/crTangentSpaceGenerator.h>
//#include <CRUtil/crTexCoordGenerator.h>
//
namespace CRUtil {

class CRUTIL_EXPORT crRenderInitVisitor : public CRCore::crNodeVisitor
{
public:

    /** Operation modes of the.*/
    enum ModeValues
    {
		NONE                                = 0x0,
        SWITCH_ON_DISPLAY_LISTS             = 0x1,
        SWITCH_OFF_DISPLAY_LISTS            = 0x2,
        COMPILE_DISPLAY_LISTS               = 0x4,
        COMPILE_STATE_ATTRIBUTES            = 0x8,
        RELEASE_DISPLAY_LISTS               = 0x10,
        RELEASE_STATE_ATTRIBUTES            = 0x20,
        SWITCH_ON_VERTEX_BUFFER_OBJECTS     = 0x40,
        SWITCH_OFF_VERTEX_BUFFER_OBJECTS    = 0x80,
		SWITCH_ON_INDEX_BUFFER_OBJECTS      = 0x100,
		SWITCH_OFF_INDEX_BUFFER_OBJECTS    = 0x200,
		CHECK_BLACK_LISTED_MODES            = 0x400,
		DEFAULT = COMPILE_DISPLAY_LISTS |
			        COMPILE_STATE_ATTRIBUTES |
			        CHECK_BLACK_LISTED_MODES
    };
    
    typedef unsigned int Mode;

    /** Construct a DisplayListVisior to traverse all child,
        * with set specified display list mode.  Default mode is to
        * gset->setUseDisplayList(true).
        */
	crRenderInitVisitor(Mode mode=COMPILE_DISPLAY_LISTS|COMPILE_STATE_ATTRIBUTES|CHECK_BLACK_LISTED_MODES);
    
    /** Set the operational mode of how the visitor should set up CRCore::Drawable's.*/
    void setMode(Mode mode) { m_mode = mode; }

    /** Get the operational mode.*/
    Mode getMode() const { return m_mode; }
    

    /** Set the crState to use during traversal. */
    void setState(CRCore::crState* state)
    {
        m_state = state;
    }
    
    CRCore::crState* getState()
    {
        return m_state.get();
    }
    
    virtual void apply(CRCore::crDB& node);
	virtual void apply(CRCore::crNode& node);
    virtual void apply(CRCore::crLightSource& node);
	virtual void apply(CRCore::crCameraNode& node);
	//virtual void apply(CRCore::crClipNode& node);

    virtual void apply(CRCore::crObject& node);
	virtual void apply(CRCore::crBillboard& node);
	virtual void apply(CRCore::crCanvasNode& node);
	virtual void apply(CRCore::crWidgetNode& node);
	//virtual void apply(CRCore::crDepthSortedObject& node);

    void apply(CRCore::crDrawable& drawable,bool needClusterCulling = true);
    //void apply(CRCore::crStateSet& stateset);

	void apply(CRParticle::crParticleSystem& ps,bool needClusterCulling = false);
	void initExtern();//做一些扩展的初始化
protected:

    Mode m_mode;
    CRCore::ref_ptr<CRCore::crState> m_state;
};

}

#endif
