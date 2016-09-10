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
#include <CRParticle/crParticleSystem.h>

#include <vector>

#include <CRCore/crDrawable.h>
#include <CRCore/crCopyOp.h>
#include <CRCore/crState.h>
#include <CRCore/crMatrix.h>
#include <crgl/gl.h>
#include <CRCore/crStateSet.h>
#include <CRCore/crTexture2D.h>
#include <CRCore/crBlendFunc.h>
#include <CRCore/crTexEnv.h>
#include <CRCore/crMaterial.h>
#include <CRCore/crNotify.h>
#include <CRCore/crBrain.h>
#include <CRCore/crDisplaySettings.h>
#include <CRCore/crShaderManager.h>
#include <CRCore/crStatistics.h>
#include <CRParticle/crParticleSystemUpdater.h>
using namespace CRCore;

CRParticle::crParticleSystem::crParticleSystem()
:    CRCore::crDrawable(), 
    m_def_bbox(CRCore::crVector3(-10, -10, -10), CRCore::crVector3(10, 10, 10)),
    m_alignment(BILLBOARD),
    m_align_X_axis(1, 0, 0),
    m_align_Y_axis(0, 1, 0),
    m_doublepass(false),
    m_frozen(false),
    m_bmin(0, 0, 0), 
    m_bmax(0, 0, 0), 
    m_reset_bounds_flag(false),
    m_bounds_computed(false),
    m_def_ptemp(crParticle()),
    m_last_frame(0),
    m_freeze_on_cull(false),
    m_detail(1),
    m_draw_count(0),
	m_lodvalue(0),
	m_traverseString("NoLight")
{
    // we don't support display lists because particle systems
    // are dynamic, and they always changes between frames
    setSupportsDisplayList(false);
}

CRParticle::crParticleSystem::crParticleSystem(const crParticleSystem& copy, const CRCore::crCopyOp& copyop)
:    CRCore::crDrawable(copy, copyop), 
    m_def_bbox(copy.m_def_bbox),
    m_alignment(copy.m_alignment),
    m_align_X_axis(copy.m_align_X_axis),
    m_align_Y_axis(copy.m_align_Y_axis),
    m_doublepass(copy.m_doublepass),
    m_frozen(copy.m_frozen),
    m_bmin(copy.m_bmin), 
    m_bmax(copy.m_bmax), 
    m_reset_bounds_flag(copy.m_reset_bounds_flag),
    m_bounds_computed(copy.m_bounds_computed),
    m_def_ptemp(copy.m_def_ptemp),
    m_last_frame(copy.m_last_frame),
    m_freeze_on_cull(copy.m_freeze_on_cull),
    m_detail(copy.m_detail),
    m_draw_count(0),
	m_traverseString(copy.m_traverseString),
	m_particleModelObject(copy.m_particleModelObject),
	m_lodvalue(copy.m_lodvalue)
{
}

CRParticle::crParticleSystem::~crParticleSystem()
{
}

void CRParticle::crParticleSystem::update(double dt)
{
    // reset bounds
    m_reset_bounds_flag = true;

	m_particlesMutex.acquire();
    for(unsigned int i=0; i<m_particles.size(); ++i)
    {
        crParticle* particle = m_particles[i].get();
        if (particle->isAlive())
        {
            if (particle->update(dt))
            {
                update_bounds(particle->getPosition(), particle->getCurrentSize());
            }
            else
            {
                reuseParticle(i);
            }
        }
    }
	m_particlesMutex.release();
    // force recomputing of bounding box on next frame
    dirtyBound();

}

void CRParticle::crParticleSystem::drawImplementation(CRCore::crState& state) const
{
    // update the frame count, so other objects can detect when
    // this particle system is culled
    m_last_frame = crFrameStamp::getInstance()->getFrameNumber();
	if(m_particleModelObject.valid())
	{
		particlemodel_render(state);
	}
	else
	{
		// get the current modelview matrix
		CRCore::crMatrix modelview = state.getModelViewMatrix();

		if (m_alignment == BILLBOARD)
		{
			//state.applyModelViewMatrix(0);
			glMatrixMode( GL_MODELVIEW );
			glPushMatrix();
			glLoadIdentity();
		}

		// set up depth mask for first rendering pass
		glPushAttrib(GL_DEPTH_BUFFER_BIT); 
		glDepthMask(GL_FALSE);

		//if(!m_stateset.valid())
		//    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

		// render, first pass
		single_pass_render(state, modelview);

		// restore depth mask settings
		glPopAttrib();

		// render, second pass
		if (m_doublepass) {    
			// set up color mask for second rendering pass
			glPushAttrib(GL_COLOR_BUFFER_BIT);
			glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
			//state.applyColorMask(crVector4ub(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE));

			// render the particles onto the depth buffer
			single_pass_render(state, modelview);

			// restore color mask settings
			glPopAttrib();
		}

		if (m_alignment == BILLBOARD)
		{
			glPopMatrix();
		}
	}
	crStatistics::getInstance()->addParticleCount(m_draw_count);
	state.colorDirty();
}

void CRParticle::crParticleSystem::setParticleModelObject(CRCore::crObject *obj)
{
	if(obj->getNumDrawables()>0)
	{
		m_particleModelObject = obj;
		m_stateset = m_particleModelObject->getDrawable(0)->getStateSet();
		m_def_ptemp.setDrawableIdRange(CRCore::rangei(0,m_particleModelObject->getNumDrawables()));
		m_def_ptemp.setShape(crParticle::Object);
		//setParticleAlignment(crParticleSystem::FIXED);
	}
}

CRCore::crObject *CRParticle::crParticleSystem::getParticleModelObject()
{
    return m_particleModelObject.get();
}

void CRParticle::crParticleSystem::setTraverseString(const std::string &ts)
{
	m_traverseString = ts;
}
void CRParticle::crParticleSystem::setLodValue(float lodvalue)
{
	m_lodvalue = lodvalue;
}
float CRParticle::crParticleSystem::getLodValue()
{
	return m_lodvalue;
}
//void CRParticle::crParticleSystem::setDistortParam(const CRCore::crVector2& distortParam)
//{
//	m_distortParam = distortParam;
//}

void CRParticle::crParticleSystem::setDefaultAttributes(const std::string& texturefile, bool emissive_particles, bool lighting, int texture_unit)
{
	//if(m_particleModelObject.valid())
	//{
	//	m_stateset = m_particleModelObject->getStateSet();
	//	return;
	//}
	if(texturefile.empty()/* || m_stateset.valid()*/)
	{
		//CRCore::notify(CRCore::WARN)<<"crParticleSystem::setDefaultAttributes m_stateset.valid()"<< std::endl;
        return;
	}
	//CRCore::notify(CRCore::WARN)<<"crParticleSystem::setDefaultAttributes m_stateset = "<<m_stateset.get()<<" texturefile = "<<texturefile<< std::endl;

	CRCore::ref_ptr<CRCore::crStateSet> stateset = new CRCore::crStateSet;
	//stateset->setName(texturefile);
	stateset->setMode(GL_LIGHTING, lighting? CRCore::crStateAttribute::ON: CRCore::crStateAttribute::OFF);
	stateset->setRenderingHint(CRCore::crStateSet::TRANSPARENT_BIN);

	if (!texturefile.empty()) 
	{
		CRCore::crTexture2D * texture = CRCore::crBrain::getInstance()->getTexture2D(texturefile);
		if(!texture)
		{
			texture = new CRCore::crTexture2D;
			texture->setImage(texture->getImageDataRequestHandler()->requestImageFile(texturefile,texture));
			texture->setFilter(CRCore::crTexture2D::MIN_FILTER, CRCore::crTexture2D::LINEAR);
			texture->setFilter(CRCore::crTexture2D::MAG_FILTER, CRCore::crTexture2D::LINEAR);
			texture->setWrap(CRCore::crTexture2D::WRAP_S, CRCore::crTexture2D::MIRROR);
			texture->setWrap(CRCore::crTexture2D::WRAP_T, CRCore::crTexture2D::MIRROR);
			//texture->setInternalFormatMode(CRCore::crTexture2D::USE_S3TC_DXT5_COMPRESSION);
			CRCore::crBrain::getInstance()->insertTexture2D(texture->getImageNameID(),texture);
			//CRCore::notify(CRCore::WARN)<<"crParticleSystem::setDefaultAttributes not find texture = "<<texture<<" name = "<<texture->getImageNameID()<< std::endl;
		}
		stateset->setTextureAttributeAndModes(texture_unit, texture, CRCore::crStateAttribute::ON);
		//CRCore::crTexEnv *texenv = new CRCore::crTexEnv;
		//texenv->setMode(CRCore::crTexEnv::MODULATE);
		//stateset->setTextureAttribute(texture_unit, texenv);
	}

	CRCore::crBlendFunc *blend = new CRCore::crBlendFunc;
	if (emissive_particles) 
	{    
		blend->setFunction(CRCore::crBlendFunc::SRC_ALPHA, CRCore::crBlendFunc::ONE);
	} 
	else 
	{
		blend->setFunction(CRCore::crBlendFunc::SRC_ALPHA, CRCore::crBlendFunc::ONE_MINUS_SRC_ALPHA);
	}
	stateset->setMode(GL_BLEND, CRCore::crStateAttribute::ON);
	stateset->setAttribute(blend, CRCore::crStateAttribute::ON);

	CRCore::crMaterial *material = new CRCore::crMaterial;
	material->setSpecular(CRCore::crMaterial::FRONT, CRCore::crVector4(0, 0, 0, 1));
	material->setEmission(CRCore::crMaterial::FRONT, CRCore::crVector4(0, 0, 0, 1));
	material->setColorMode(lighting? CRCore::crMaterial::AMBIENT_AND_DIFFUSE : CRCore::crMaterial::OFF);
	stateset->setAttribute(material, CRCore::crStateAttribute::ON);
	
	//if(crDisplaySettings::instance()->getHDR())
	//{
		//CRCore::crStateSet *shader = crShaderManager::getInstance()->getShaderStateSet("particle");
  //      stateset->merge(*shader);
	//}

	//CRCore::notify(CRCore::WARN)<<"crParticleSystem::setDefaultAttributes checkStateSetExistAndGet "<< std::endl;
	CRCore::crStateSet *chk_ss = CRCore::crBrain::getInstance()->checkStateSetExistAndGet(stateset.get());
	if(!chk_ss) 
	{
		//CRCore::notify(CRCore::WARN)<<"crParticleSystem::setDefaultAttributes not find stateset = "<<chk_ss<< std::endl;
		CRCore::crBrain::getInstance()->insertStateSet(stateset.get());
		//CRCore::notify(CRCore::WARN)<<"crParticleSystem::setDefaultAttributes insertStateSet "<< std::endl;
		chk_ss = stateset.get();
	}
    setStateSet(chk_ss);
}


void CRParticle::crParticleSystem::single_pass_render(CRCore::crState&  state, const CRCore::crMatrix& modelview) const
{
    m_draw_count = 0;
    if (m_particles_buf.size() <= 0) return;

    Particle_vector::const_iterator i;
    Particle_vector::const_iterator i0 = m_particles_buf.begin();
    Particle_vector::const_iterator end = m_particles_buf.end();
    
    (*i0)->beginRender();

    float scale = 1.0f;//sqrtf(static_cast<float>(m_detail_buf));
    for (i=i0; i<end; i+=m_detail_buf) 
	{
        //if ((*i)->isBufAlive()) 
		//{
            if ((*i)->getShape() != (*i0)->getShape())
			{
                (*i0)->endRender();
                (*i)->beginRender();
                i0 = i;
            }
            ++m_draw_count;

            switch (m_alignment) 
			{
                case BILLBOARD:
                    (*i)->render(state,modelview.preMult((*i)->getBufPosition()), CRCore::crVector3(1, 0, 0), CRCore::crVector3(0, 1, 0), scale);
                    break;
                case FIXED:
                    (*i)->render(state,m_parentMatrix_buf,(*i)->getBufPosition(), m_align_X_axis_buf, m_align_Y_axis_buf, scale);
                    break;
                default: ;
            }
        //}    
    }

    (*i0)->endRender();
    
}

void CRParticle::crParticleSystem::particlemodel_render(CRCore::crState&  state) const
{
	m_draw_count = 0;
	if (m_particles_buf.size() <= 0) return;

	Particle_vector::const_iterator i;
	Particle_vector::const_iterator i0 = m_particles_buf.begin();
	Particle_vector::const_iterator end = m_particles_buf.end();

	float scale = 1.0f;//sqrtf(static_cast<float>(m_detail_buf));
	for (i=i0; i<end; i+=m_detail_buf) 
	{
		//if ((*i)->isBufAlive()) 
		//{
			++m_draw_count;

			switch (m_alignment) 
			{
			case BILLBOARD:
				(*i)->render(state,m_particleModelObject.get(),scale);
				break;
			case FIXED:
				//(*i)->render((*i)->getBufPosition(), m_align_X_axis_buf, m_align_Y_axis_buf, scale);
				(*i)->render(state,m_particleModelObject.get(),scale);
				break;
			default: ;
			}
		//}    
	}
}


bool CRParticle::crParticleSystem::computeBound() const
{
	m_bbox.set(m_bmin,m_bmax);
	m_bbox_computed = true;
	return true;
}

bool CRParticle::crParticleSystem::getNeedUpdateTraverse()
{
	return m_alignment == FIXED && m_def_ptemp.getAlignedMode() != crParticle::AXIAL_None;
}
void CRParticle::crParticleSystem::setStateSet(CRCore::crStateSet *state)
{
	crDrawable::setStateSet(state);
	if(m_particleModelObject.valid() && m_particleModelObject->getNumDrawables()>0)
	{
		m_particleModelObject->getDrawable(0)->setStateSet(m_stateset.get());
	}
}