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
#ifndef CRUTIL_POSTIONALSTATECONTIANER
#define CRUTIL_POSTIONALSTATECONTIANER 1

#include <CRCore/crBase.h>
#include <CRCore/crLight.h>
#include <CRCore/crState.h>

#include <CRUtil/crRenderLeaf.h>
#include <CRUtil/crRenderGraph.h>

namespace CRUtil {

/**
 * crPositionalStateContainer base class. Used in RenderStage class.
 */
class CRUTIL_EXPORT crPositionalStateContainer : public CRCore::crBase
{
    public:
    

        crPositionalStateContainer();
        virtual CRCore::crBase* cloneType() const { return new crPositionalStateContainer(); }
        virtual CRCore::crBase* clone(const CRCore::crCopyOp&) const { return new crPositionalStateContainer(); } // note only implements a clone of type.
        virtual bool isSameKindAs(const CRCore::crBase* obj) const { return dynamic_cast<const crPositionalStateContainer*>(obj)!=0L; }
        virtual const char* libraryName() const { return "CRUtil"; }
        virtual const char* className() const { return "crPositionalStateContainer"; }

        virtual void reset();
        
        typedef std::pair< CRCore::ref_ptr<const CRCore::crStateAttribute> , CRCore::ref_ptr<CRCore::RefMatrix> >    AttrMatrixPair;
        typedef std::vector< AttrMatrixPair >                                            AttrMatrixList;
        typedef std::map< unsigned int, AttrMatrixList >                                 TexUnitAttrMatrixListMap;

        virtual void addPositionedAttribute(CRCore::RefMatrix* matrix,const CRCore::crStateAttribute* attr)
        {
            m_attrList.push_back(AttrMatrixPair(attr,matrix));
        }

        virtual void addPositionedTextureAttribute(unsigned int textureUnit, CRCore::RefMatrix* matrix,const CRCore::crStateAttribute* attr)
        {
            m_texAttrListMap[textureUnit].push_back(AttrMatrixPair(attr,matrix));
        }

        virtual void draw(CRCore::crState& state,crRenderLeaf*& previous, const CRCore::crMatrix* postMultMatrix = 0);
        
    public:
        
        AttrMatrixList              m_attrList;
        TexUnitAttrMatrixListMap    m_texAttrListMap;
        
    protected:
    
        virtual ~crPositionalStateContainer();

};

}

#endif

