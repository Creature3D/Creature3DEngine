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
#ifndef CRPARTICLE_PARTICLEEFFECT
#define CRPARTICLE_PARTICLEEFFECT

#include <CRParticle/crEmitter.h>
#include <CRParticle/crProgram.h>
#include <CRParticle/crParticleSystem.h>
#include <CRCore/crNodeVisitor.h>
namespace CRParticle
{
class CRPARTICLE_EXPORT LoadNodeCfgHandler : public CRCore::Referenced
{
public:            
	virtual void loadCfgFile(const std::string &cfgFile, CRCore::crNode *node){}
protected:
	virtual ~LoadNodeCfgHandler() {}
};

class CRPARTICLE_EXPORT crParticleEffect : public CRCore::crGroup
{
public:

    crParticleEffect();
    
    crParticleEffect(const crParticleEffect& copy, const CRCore::crCopyOp& copyop = CRCore::crCopyOp::SHALLOW_COPY);

	META_Node(CRParticle,crParticleEffect);
    /// Get whether all particles are dead 
    bool areAllParticlesDead() const { return m_particleSystem.valid()?m_particleSystem->areAllParticlesDead():true; }

	inline virtual bool isEffectIdle() const { return m_emitter.valid()?!(m_emitter->isEnabled()&&m_emitter->isAlive()):true;/*m_particleSystem.valid()?m_particleSystem->areAllParticlesDead():true;*/ }
	virtual void setEffectPosition(const CRCore::crVector3& position);
	virtual void setEffectDirection(const CRCore::crVector3& direction);
	virtual void setEffectDuration(float t);
	virtual void startEffect();
	virtual void stopEffect();
	
	void setEmitter(crEmitter* emitter);
	inline crEmitter* getEmitter(){ return m_emitter.get(); }
	inline const crEmitter* getEmitter() const{ return m_emitter.get(); }

	void setProgram(crProgram* program);
	inline crProgram* getProgram(){ return m_program.get(); }
    const crProgram* getProgram() const{ return m_program.get(); };
    
    void setParticleSystem(crParticleSystem* ps);
    inline crParticleSystem* getParticleSystem() { return m_particleSystem.get(); }
    inline const crParticleSystem* getParticleSystem() const { return m_particleSystem.get(); }

    //virtual void setDefaults();

	virtual void buildEffect();
	virtual void preRemoveNode();
	virtual void traverse(CRCore::crNodeVisitor& nv);

	static void setLoadNodeCfgHandler(LoadNodeCfgHandler* handler) { s_loadNodeCfgHandler = handler; }

	void setDirty(bool dirty);
	bool getDirty() const;
	virtual void releaseObjects(CRCore::crState* /* = 0 */);
protected:
    virtual ~crParticleEffect();
    
    CRCore::ref_ptr<crParticleSystem>    m_particleSystem;
	CRCore::ref_ptr<crEmitter>           m_emitter;
	CRCore::ref_ptr<crProgram>           m_program;

	static CRCore::ref_ptr<LoadNodeCfgHandler> s_loadNodeCfgHandler;
	bool m_dirty;
	bool m_removed;
	//bool m_needUpdate;
};

}

#endif
