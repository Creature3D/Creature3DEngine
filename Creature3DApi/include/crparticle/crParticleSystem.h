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
#ifndef CRPARTICLE_PARTICLESYSTEM
#define CRPARTICLE_PARTICLESYSTEM 1

#include <CRParticle/crExport.h>
#include <CRParticle/crParticle.h>

#include <vector>
#include <stack>
#include <algorithm>
#include <string>

#include <CRCore/crBase.h>
#include <CRCore/crDrawable.h>
#include <CRCore/crCopyOp.h>
#include <CRCore/crState.h>
#include <CRCore/crVector3.h>
#include <CRCore/crBoundingBox.h>
#include <CRCore/thread/crCriticalMutex.h>
#include <CRCore/thread/crScopedLock.h>
namespace CRParticle
{

    /** The heart of this class library; its purpose is to hold a set of particles and manage particle creation, update, rendering and destruction.
      * You can add this drawable to any crObject as you usually do with other
      * crDrawable classes. Each instance of crParticleSystem is a separate set of
      * particles; it provides the interface for creating particles and iterating
      * through them (see the crEmitter and crProgram classes).
    */
    class CRPARTICLE_EXPORT crParticleSystem: public CRCore::crDrawable {
    public:
    
        enum Alignment {
            BILLBOARD,
            FIXED
        };

        crParticleSystem();
        crParticleSystem(const crParticleSystem& copy, const CRCore::crCopyOp& copyop = CRCore::crCopyOp::SHALLOW_COPY);

        META_Base(CRParticle, crParticleSystem);
        
        /// Get the alignment type of particles.
        inline Alignment getParticleAlignment() const;
        
        /// Set the alignment type of particles.
        inline void setParticleAlignment(Alignment a);
        
        /// Get the X-axis alignment vector.
        inline const CRCore::crVector3& getAlignVectorX() const;
        
        /// Set the X-axis alignment vector.
        inline void setAlignVectorX(const CRCore::crVector3& v);

        /// Get the Y-axis alignment vector.
        inline const CRCore::crVector3& getAlignVectorY() const;
        
        /// Set the Y-axis alignment vector.
        inline void setAlignVectorY(const CRCore::crVector3& v);
        
        /// Set the alignment vectors.
        inline void setAlignVectors(const CRCore::crVector3& X, const CRCore::crVector3& Y);

        /// Get the default bounding box
        inline const CRCore::crBoundingBox& getDefaultBoundingBox() const;        
        
        /**    Set the default bounding box.
            The default bounding box is used when a real bounding box cannot be computed, for example
            because no particles has been updated yet.
        */
        inline void setDefaultBoundingBox(const CRCore::crBoundingBox& bbox);

        /// Get the double pass rendering flag.
        inline bool getDoublePassRendering() const;
        
        /** Set the double pass rendering flag.
            Double pass rendering avoids overdraw problems between particle systems
            and other opaque objects. If you can render all the particle systems after
            the opaque objects, then double pass is not necessary and can be turned off (best choice).
            If you set the default attributes with setDefaultAttributes, then the particle
            system will fall into a transparent bin.
        */
        inline void setDoublePassRendering(bool v);
        
        /// Return true if the particle system is frozen.
        inline bool isFrozen() const;
        
        /**    Set or reset the <I>frozen</I> state.
            When the particle system is frozen, emitters and programs won't do anything on it.
        */
        inline void setFrozen(bool v);

        /// Get the number of allocated particles (alive + dead).
        inline int numParticles() const;
        
        /// Get the number of dead particles.
        inline int numDeadParticles() const;

        /// Get whether all particles are dead        
        inline bool areAllParticlesDead() const { return numDeadParticles()==numParticles(); }
        
        /// Get a pointer to the i-th particle.
        inline crParticle* getParticle(int i);
        
        /// Get a const pointer to the i-th particle.
        inline const crParticle* getParticle(int i) const;
        
		inline unsigned int getParticleIndex( const crParticle* p ) const;

        /// Create a new particle from the specified template (or the default one if <CODE>ptemplate</CODE> is null).
        inline virtual crParticle* createParticle(const crParticle* ptemplate);
        
        /// Destroy the i-th particle.
        inline virtual void destroyParticle(int i);
        
        /// Reuse the i-th particle.
        inline virtual void reuseParticle(int i) { m_deadparts.push(m_particles[i].get()); }

        /// Get the last frame number.
        inline int getLastFrameNumber() const;

        /// Get a reference to the default particle template.
        inline crParticle& getDefaultParticleTemplate();
        
        /// Get a const reference to the default particle template.
        inline const crParticle& getDefaultParticleTemplate() const;

        /// Set the default particle template (particle is copied).
        inline void setDefaultParticleTemplate(const crParticle& p);
        
        /// Get whether the particle system can freeze when culled
        inline bool getFreezeOnCull() const;
        
        /// Set whether the particle system can freeze when culled (default is true)
        inline void setFreezeOnCull(bool v);
        
        /** A useful method to set the most common <CODE>crStateAttribute</CODE>'s in one call.
            If <CODE>texturefile</CODE> is empty, then texturing is turned off.
        */
        void setDefaultAttributes(const std::string& texturefile = "", bool emissive_particles = true, bool lighting = false, int texture_unit = 0);
        
        /// (<B>EXPERIMENTAL</B>) Get the level of detail.
        inline int getLevelOfDetail() const;
        
        /** (<B>EXPERIMENTAL</B>) Set the level of detail. The total number of particles is divided by the detail value to
            get the actual number of particles to be drawn. This value must be greater than zero.
        */
        inline void setLevelOfDetail(int v);

        /// Update the particles. Don't call this directly, use a <CODE>crParticleSystemUpdater</CODE> instead.
        virtual void update(double dt);

        virtual void drawImplementation(CRCore::crState& state) const;

		inline virtual void swapBuffers(int frameNumber)
		{
			if(m_swapFrameNumber != frameNumber)
			{
				if (!isFrozen() &&!getFreezeOnCull())
				{
					m_particles_buf.resize(0);
					//m_particles_buf.clear();
					m_particlesMutex.acquire();
					for( Particle_vector::iterator itr = m_particles.begin();
						itr != m_particles.end();
						++itr )
					{
						if((*itr)->isAlive())
						{
							(*itr)->swapBuffers();
							m_particles_buf.push_back(*itr);
						}
					}
					m_particlesMutex.release();
					if(m_alignment == FIXED)
					{
						m_align_X_axis_buf = m_align_X_axis;
						m_align_Y_axis_buf = m_align_Y_axis;
					}
					m_detail_buf = m_detail;

					crDrawable::swapBuffers(frameNumber);
				}
				CRCore::crMatrix matrix = m_parentMatrix;
				m_parentMatrix = m_parentMatrix_buf;
				m_parentMatrix_buf = matrix;
				m_swapFrameNumber = frameNumber;
			}
		}

		void setParticleModelObject(CRCore::crObject *obj);
		CRCore::crObject *getParticleModelObject();

		void setTraverseString(const std::string &ts);
		inline const std::string& getTraverseString() const { return m_traverseString; }

		//void setDistortParam(const CRCore::crVector2& distortParam);
		//CRCore::crVector2& getDistortParam() { return m_distortParam; }
		void setLodValue(float lodvalue);
		float getLodValue();
		inline void updateParentMatrix(const CRCore::crMatrix& matrix){ m_parentMatrix = matrix; }
		bool getNeedUpdateTraverse();
		virtual void setStateSet(CRCore::crStateSet *state);
    protected:

        virtual ~crParticleSystem();

		virtual bool computeBound() const;

        crParticleSystem& operator=(const crParticleSystem&) { return *this; }

        inline void update_bounds(const CRCore::crVector3& p, const CRCore::crVector3& s);
        void single_pass_render(CRCore::crState& state, const CRCore::crMatrix& modelview) const;
		void particlemodel_render(CRCore::crState&  state) const;

		typedef std::vector< CRCore::ref_ptr<crParticle> > Particle_vector;
        typedef std::stack<crParticle*> Death_stack;

        Particle_vector m_particles;
		CRCore::crCriticalMutex m_particlesMutex;
        Death_stack m_deadparts;

		Particle_vector m_particles_buf;//////
        
        CRCore::crBoundingBox m_def_bbox;
        
        Alignment m_alignment;
        CRCore::crVector3 m_align_X_axis;
        CRCore::crVector3 m_align_Y_axis;

		CRCore::crVector3 m_align_X_axis_buf;///////
		CRCore::crVector3 m_align_Y_axis_buf;///////

        bool m_doublepass;
        bool m_frozen;

        CRCore::crVector3 m_bmin;
        CRCore::crVector3 m_bmax;

        bool m_reset_bounds_flag;
        bool m_bounds_computed;

        crParticle m_def_ptemp;
        mutable int m_last_frame;
        bool m_freeze_on_cull;
        
        int m_detail;
		int m_detail_buf;//////
        mutable int m_draw_count;

		std::string m_traverseString;
		CRCore::ref_ptr<CRCore::crObject> m_particleModelObject;
		//CRCore::crVector2 m_distortParam;
		float m_lodvalue;
		CRCore::crMatrix m_parentMatrix;
		CRCore::crMatrix m_parentMatrix_buf;
    };
    
    // INLINE FUNCTIONS
    
    inline crParticleSystem::Alignment crParticleSystem::getParticleAlignment() const
    {
        return m_alignment;
    }
    
    inline void crParticleSystem::setParticleAlignment(Alignment a)
    {
        m_alignment = a;
    }
    
    inline const CRCore::crVector3& crParticleSystem::getAlignVectorX() const
    {
        return m_align_X_axis;
    }
    
    inline void crParticleSystem::setAlignVectorX(const CRCore::crVector3& v)
    {
        m_align_X_axis = v;
    }

    inline const CRCore::crVector3& crParticleSystem::getAlignVectorY() const
    {
        return m_align_Y_axis;
    }
    
    inline void crParticleSystem::setAlignVectorY(const CRCore::crVector3& v)
    {
        m_align_Y_axis = v;
    }
    
    inline void crParticleSystem::setAlignVectors(const CRCore::crVector3& X, const CRCore::crVector3& Y)
    {
        m_align_X_axis = X;
        m_align_Y_axis = Y;
    }

    inline bool crParticleSystem::isFrozen() const
    {
        return m_frozen;
    }
    
    inline void crParticleSystem::setFrozen(bool v)
    {
        m_frozen = v;
    }
    
    inline const CRCore::crBoundingBox& crParticleSystem::getDefaultBoundingBox() const
    {
        return m_def_bbox;
    }
    
    inline void crParticleSystem::setDefaultBoundingBox(const CRCore::crBoundingBox& bbox)
    {
        m_def_bbox = bbox;
    }

    inline bool crParticleSystem::getDoublePassRendering() const
    {
        return m_doublepass;
    }

    inline void crParticleSystem::setDoublePassRendering(bool v)
    {
        m_doublepass = v;
    }

    inline int crParticleSystem::numParticles() const
    {
        return static_cast<int>(m_particles.size());
    }

    inline int crParticleSystem::numDeadParticles() const
    {
        return static_cast<int>(m_deadparts.size());
    }

    inline crParticle* crParticleSystem::getParticle(int i)
    {
        return m_particles[i].get();
    }

    inline const crParticle* crParticleSystem::getParticle(int i) const
    {
        return m_particles[i].get();
    }

    inline void crParticleSystem::destroyParticle(int i)
    {
        m_particles[i]->kill();
    }
    
    inline int crParticleSystem::getLastFrameNumber() const
    {
        return m_last_frame;
    }

    inline void crParticleSystem::update_bounds(const CRCore::crVector3& p, const CRCore::crVector3& s)
    {
        if (m_reset_bounds_flag) {
            m_reset_bounds_flag = false;
            m_bmin = p - s;//CRCore::crVector3(r,r,r);
            m_bmax = p + s;//CRCore::crVector3(r,r,r);
        } else {
            if (p.x() - s.x() < m_bmin.x()) m_bmin.x() = p.x() - s.x();
            if (p.y() - s.y() < m_bmin.y()) m_bmin.y() = p.y() - s.y();
            if (p.z() - s.z() < m_bmin.z()) m_bmin.z() = p.z() - s.z();
            if (p.x() + s.x() > m_bmax.x()) m_bmax.x() = p.x() + s.x();
            if (p.y() + s.y() > m_bmax.y()) m_bmax.y() = p.y() + s.y();
            if (p.z() + s.z() > m_bmax.z()) m_bmax.z() = p.z() + s.z();
            
            if (!m_bounds_computed) 
                m_bounds_computed = true;
        }
    }

    inline crParticle& crParticleSystem::getDefaultParticleTemplate()
    {
        return m_def_ptemp;
    }

    inline const crParticle& crParticleSystem::getDefaultParticleTemplate() const
    {
        return m_def_ptemp;
    }

    inline void crParticleSystem::setDefaultParticleTemplate(const crParticle& p)
    {
        m_def_ptemp = p;
    }
    
    inline bool crParticleSystem::getFreezeOnCull() const
    {
        return m_freeze_on_cull;
    }
    
    inline void crParticleSystem::setFreezeOnCull(bool v)
    {
        m_freeze_on_cull = v;
    }
    
    inline int crParticleSystem::getLevelOfDetail() const
    {
        return m_detail;
    }
    
    inline void crParticleSystem::setLevelOfDetail(int v)
    {
        if (v < 1) v = 1;
        m_detail = v;
    }
    
	inline unsigned int crParticleSystem::getParticleIndex( const crParticle* p ) const
	{
		for( unsigned int particleNum=0; particleNum<m_particles.size(); ++particleNum )
		{
			if( m_particles[particleNum] == p ) return particleNum;
		}
		return m_particles.size(); // node not found.
	}

	// I'm not sure this function should be inlined...     
    inline crParticle* crParticleSystem::createParticle(const crParticle* ptemplate)
    {
		CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_particlesMutex);
        // is there any dead particle?
        if (!m_deadparts.empty()) 
		{
            // retrieve a pointer to the last dead particle
            crParticle* P = m_deadparts.top();

            // create a new (alive) particle in the same place
            *P = crParticle(ptemplate? *ptemplate: m_def_ptemp);

            // remove the pointer from the death stack
            m_deadparts.pop();
            return P;
        } 
		else 
		{
            // add a new particle to the vector
            //m_particles.push_back(crParticle(ptemplate? *ptemplate: m_def_ptemp));
			crParticle *P = new crParticle;
			*P = crParticle(ptemplate? *ptemplate: m_def_ptemp);
			m_particles.push_back(P);
            return m_particles.back().get();
        }
    }


}

#endif
