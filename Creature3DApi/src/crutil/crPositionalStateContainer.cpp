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
#include <CRUtil/crPositionalStateContainer.h>

using namespace CRCore;
using namespace CRUtil;

// register a crPositionalStateContainer prototype with the RenderBin prototype list.
//RegisterRenderBinProxy<crPositionalStateContainer> s_registerPositionalStateContainerProxy;

crPositionalStateContainer::crPositionalStateContainer()
{
}

crPositionalStateContainer::~crPositionalStateContainer()
{
}

void crPositionalStateContainer::reset()
{
    m_attrList.clear();
    m_texAttrListMap.clear();
}

void crPositionalStateContainer::draw(CRCore::crState& state,crRenderLeaf*& previous, const CRCore::crMatrix* postMultMatrix)
{

    if (previous)
    {
        crRenderGraph::moveToRootRenderGraph(state,previous->m_parent);
        state.apply();
        previous = NULL;
    }

    // apply the light list.
	//std::vector<crStateAttribute::VRMode> modes;
    for(AttrMatrixList::iterator litr=m_attrList.begin();
        litr!=m_attrList.end();
        ++litr)
    {
        if (postMultMatrix)
        {
            if ((*litr).second.valid())
                state.applyModelViewMatrix(new CRCore::RefMatrix( (*((*litr).second)) * (*postMultMatrix)));
            else
                state.applyModelViewMatrix(new CRCore::RefMatrix( *postMultMatrix));
        }
        
        else
        {
            state.applyModelViewMatrix((*litr).second.get());
        }

        // apply the light source.
  //      modes.clear();
		//litr->first->getAssociatedModes(modes);
		//for(std::vector<crStateAttribute::VRMode>::iterator itr=modes.begin();
		//	itr!=modes.end();
		//	++itr)
		//{
		//	state.applyMode(*itr,true);
		//}
        litr->first->apply(state);

        // tell state about.
        state.haveAppliedAttribute(litr->first.get());
        
        // set this state as a global default
        state.setGlobalDefaultAttribute(litr->first.get());
    }

    for(TexUnitAttrMatrixListMap::iterator titr=m_texAttrListMap.begin();
        titr!=m_texAttrListMap.end();
        ++titr)
    {
        state.setActiveTextureUnit(titr->first);
        
        AttrMatrixList attrList = titr->second;
        
        for(AttrMatrixList::iterator litr=attrList.begin();
            litr!=attrList.end();
            ++litr)
        {
            if (postMultMatrix)
            {
                if ((*litr).second.valid())
                    state.applyModelViewMatrix(new CRCore::RefMatrix( (*((*litr).second)) * (*postMultMatrix)));
                else
                    state.applyModelViewMatrix(new CRCore::RefMatrix( *postMultMatrix));
            }
            else
            {
                state.applyModelViewMatrix((*litr).second.get());
            }

            // apply the light source.
			//modes.clear();
			//litr->first->getAssociatedModes(modes);
			//for(std::vector<crStateAttribute::VRMode>::iterator itr=modes.begin();
			//	itr!=modes.end();
			//	++itr)
			//{
			//	state.applyMode(*itr,true);
			//}
            litr->first->apply(state);

            // tell state about.
            state.haveAppliedTextureAttribute(titr->first, litr->first.get());

            // set this state as a global default
            state.setGlobalDefaultTextureAttribute(titr->first, litr->first.get());
        }

    }
}
