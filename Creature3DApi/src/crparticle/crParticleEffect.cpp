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
#include <CRParticle/crParticleEffect.h>
#include <CRParticle/crParticleSystemUpdater.h>
#include <CRParticle/crModularEmitter.h>
#include <CRParticle/crCenteredPlacer.h>
#include <CRParticle/crFluidProgram.h>
#include <CRCore/crObject.h>
#include <CRCore/crTraverseStringNode.h>
#include <CRCore/crUniform.h>
#include <CRCore/crVector2.h>
#include <CRCore/crLod.h>
#include <CRCore/crCalcNodeParentsMatrixVisitor.h>
#include <CRIOManager/crLoadManager.h>
using namespace CRParticle;
CRCore::ref_ptr<LoadNodeCfgHandler> crParticleEffect::s_loadNodeCfgHandler = NULL;

crParticleEffect::crParticleEffect():
m_dirty(true),
m_removed(false)
{
	setCalcShadow(false);
	setEnableIntersect(false);
	//if(CRCore::crDisplaySettings::instance()->getRunMode() == 0)
	setNumChildrenRequiringUpdateTraversal(getNumChildrenRequiringUpdateTraversal()+1);
}

crParticleEffect::crParticleEffect(const crParticleEffect& copy, const CRCore::crCopyOp& copyop):
	m_dirty(true),
	m_removed(false)
{
	m_particleSystem = dynamic_cast<crParticleSystem *>(copy.m_particleSystem->clone(copyop));
	m_emitter = dynamic_cast<crEmitter *>(copy.m_emitter->clone(copyop));
	m_program = dynamic_cast<crProgram *>(copy.m_program->clone(copyop));

	setName(copy.getName());
	setCalcShadow(copy.isCalcShadow());
	setEnableIntersect(copy.getEnableIntersect());
	setDescriptionsInString(copy.getDescriptionsInString());
	//if(CRCore::crDisplaySettings::instance()->getRunMode() == 0)
	setNumChildrenRequiringUpdateTraversal(getNumChildrenRequiringUpdateTraversal()+1);
}

crParticleEffect::~crParticleEffect()
{
}

void crParticleEffect::setEffectPosition(const CRCore::crVector3& position)
{
	crModularEmitter *emitter = dynamic_cast<crModularEmitter *>(m_emitter.get());
	if(emitter)
	{
        crCenteredPlacer *placer = dynamic_cast<crCenteredPlacer *>(emitter->getPlacer());
        if(placer) placer->setCenter(position);
	}
}

void crParticleEffect::setEffectDirection(const CRCore::crVector3& direction)
{
    if(dynamic_cast<crFluidProgram *>(m_program.get()))
	{
        dynamic_cast<crFluidProgram *>(m_program.get())->setWind(direction);
	}
}

void crParticleEffect::setEffectDuration(float t)
{
    m_emitter->setLifeTime(t);
}

void crParticleEffect::startEffect()
{
    m_emitter->setEnabled(true);
	m_program->setEnabled(true);
	setVisiable(true);
}

void crParticleEffect::stopEffect()
{
	m_emitter->setEnabled(false);
	m_program->setEnabled(false);
	setVisiable(false);
}

void crParticleEffect::setEmitter(crEmitter* emitter)
{
	m_emitter = emitter;
}


void crParticleEffect::setProgram(crProgram* program)
{
	m_program = program;
}


void crParticleEffect::setParticleSystem(crParticleSystem* ps)
{
	m_particleSystem = ps;
}

void crParticleEffect::buildEffect()
{
    if (!m_emitter.valid() || !m_program.valid() || !m_particleSystem.valid()) return; 

    // clear the children.
    //removeChild(0,getNumChildren());
	m_emitter->setParticleSystem(m_particleSystem.get());	
	m_program->setParticleSystem(m_particleSystem.get());

 //   // add the emitter
	//crParticleSystemUpdater::getInstance()->addParticleProcessor(m_emitter.get());
 //   //addChild(m_emitter.get());
 //   
 //   // add the program to update the particles
	//crParticleSystemUpdater::getInstance()->addParticleProcessor(m_program.get());
 //   //addChild(m_program.get());

 //   // add the particle system updater.
 //   //CRCore::ref_ptr<CRParticle::crParticleSystemUpdater> psu = new CRParticle::crParticleSystemUpdater;
 //   //psu->addParticleSystem(m_particleSystem.get());
 //   //addChild(psu.get());
	//crParticleSystemUpdater::getInstance()->addParticleSystem(m_particleSystem.get());

	CRCore::ref_ptr<CRCore::crObject> geode;
    geode = new CRCore::crObject;
	geode->setName("PSObject");
	geode->addDrawable(m_particleSystem.get());
	geode->setDrawOutLine(-1);
	//load object stateset
	char buf[16];
	std::string cfgFile;
	for(int i = 0; i<10; i++)
	{
		sprintf(buf,"cfgFile%d\0",i);
		if(CRCore::crArgumentParser::readKeyValue(getDescriptions(),buf,cfgFile))
		{
			s_loadNodeCfgHandler->loadCfgFile(cfgFile,geode.get());
		}
	}

	float lodvalue = m_particleSystem->getLodValue();
	if(lodvalue>0)
	{
		CRCore::ref_ptr<CRCore::crLod>lodNode = new CRCore::crLod;
		lodNode->setName("PSLod");
		//addChild(lodNode);
		const std::string ts = m_particleSystem->getTraverseString();
		if(!ts.empty())
		{
			CRCore::ref_ptr<CRCore::crTraverseStringNode>tsNode = new CRCore::crTraverseStringNode;
			tsNode->setName(ts);
			tsNode->addChild(geode.get());
			lodNode->addChild(tsNode.get(),0,lodvalue);
		}
		else
		{
			lodNode->addChild(geode.get(),0,lodvalue);
		}
		CRIOManager::crLoadManager::getInstance()->requestAddNode(this,lodNode.get(),false);
	}
	else
	{
		const std::string ts = m_particleSystem->getTraverseString();
		if(!ts.empty())
		{
			CRCore::ref_ptr<CRCore::crTraverseStringNode> tsNode = new CRCore::crTraverseStringNode;
			tsNode->setName(ts);
			tsNode->addChild(geode.get());
			CRIOManager::crLoadManager::getInstance()->requestAddNode(this,tsNode.get(),false);
			//addChild(tsNode);
		}
		else
		{
			//addChild(geode);
			CRIOManager::crLoadManager::getInstance()->requestAddNode(this,geode.get(),false);
		}
	}
	//startEffect();
	crParticleSystemUpdater::getInstance()->insertParticleEffect(this);
	m_dirty = false;
	m_removed = false;
	//if(m_particleSystem->getNeedUpdateTraverse())
	//{
	//	setNumChildrenRequiringUpdateTraversal(getNumChildrenRequiringUpdateTraversal()+1);
	//}
	//CRCore::crVector2 distortParam = m_particleSystem->getDistortParam();
	//if(distortParam[1] > 0.0f)
	//{
	//	CRCore::crUniform *uniform = m_particleSystem->getOrCreateStateSet()->getOrCreateUniform("DistortParam",CRCore::crUniform::FLOAT_VEC2);
	//	uniform->setDataVariance(crBase::STATIC);
	//	uniform->set(distortParam);
	//}
	//CRCore::notify(CRCore::FATAL)<<"crParticleEffect::buildEffect(): "<<std::endl;
}

void crParticleEffect::preRemoveNode()
{
	crParticleSystemUpdater::getInstance()->removeParticleEffect(this);
	m_removed = true;
}
void crParticleEffect::setDirty(bool dirty)
{
	m_dirty = dirty;
}
bool crParticleEffect::getDirty() const 
{
	return m_dirty;
}
void crParticleEffect::traverse(CRCore::crNodeVisitor& nv)
{
	switch (nv.getVisitorType())
	{
	//case CRCore::crNodeVisitor::INIT_VISITOR:
	//	if(m_dirty)
	//		buildEffect();
	//	//m_needUpdate = true;
	//	break;
	case CRCore::crNodeVisitor::UPDATE_VISITOR:
		if(getVisiable())
		{
			if(m_dirty)
				buildEffect();
			if(m_removed)
			{
				crParticleSystemUpdater::getInstance()->insertParticleEffect(this);
				m_removed = false;
			}
			if (m_particleSystem.valid() && m_particleSystem->getNeedUpdateTraverse())
			{
				CRCore::crCalcNodeParentsMatrixVisitor parentMatrix;
				accept(parentMatrix);
				m_particleSystem->updateParentMatrix(parentMatrix.getResult());
			}
			//if(m_particleSystem.valid())
			//{
			//	float length = (nv.getEyePoint() - getBound().center()).length();
			//	float lodLevel = (length - 30.0f)/5.0f;
			//	////// clamp the value to [1, n]
			//	if(lodLevel < 1.0f) lodLevel = 1.0f;
			//	m_particleSystem->setLevelOfDetail(lodLevel);
			//}
			//m_needUpdate = true;
		}
		break;
	//case CRCore::crNodeVisitor::SWAPBUFFER_VISITOR:
	//	if(m_dirty)
	//		buildEffect();
	//	if(m_removed)
	//	{
	//		crParticleSystemUpdater::getInstance()->insertParticleEffect(this);
	//		m_removed = false;
	//	}
	//	break;
	}
	crGroup::traverse(nv);
}
void crParticleEffect::releaseObjects(CRCore::crState* state)
{
	m_particleSystem->releaseObjects(state);
	m_emitter->releaseObjects(state);
	m_program->releaseObjects(state);
}