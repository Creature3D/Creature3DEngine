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
#ifndef CRPARTICLE_PARTICLEPROCESSOR
#define CRPARTICLE_PARTICLEPROCESSOR 1

#include <CRParticle/crExport.h>
#include <CRParticle/crParticleSystem.h>

#include <CRCore/ref_ptr.h>
#include <CRCore/crBase.h>
#include <CRCore/crTransform.h>
#include <CRCore/crNodeVisitor.h>
#include <CRCore/crCopyOp.h>
#include <CRCore/crVector3.h>
#include <CRCore/crMatrix.h>

namespace CRParticle
{

    /** A common base interface for those classes which need to do something on particles. Such classes
      * are, for example, crEmitter (particle generation) and crProgram (particle animation).
      * This class holds some properties, like a <I>reference frame</I> and a reference to a crParticleSystem;
      * descendant classes should process the particles taking into account the reference frame, computing the right
      * transformations when needed.
    */
	class CRPARTICLE_EXPORT crParticleProcessor: public CRCore::crBase {
    public:
    
        enum ReferenceFrame {
            RELATIVE_RF,
            ABSOLUTE_RF
        };
        
        crParticleProcessor();
        crParticleProcessor(const crParticleProcessor& copy, const CRCore::crCopyOp& copyop = CRCore::crCopyOp::SHALLOW_COPY);

        virtual const char* libraryName() const { return "CRParticle"; }
        virtual const char* className() const { return "crParticleProcessor"; }
        virtual bool isSameKindAs(const CRCore::crBase* obj) const { return dynamic_cast<const crParticleProcessor*>(obj) != 0; }
        
        /// Get the reference frame.
        inline ReferenceFrame getReferenceFrame() const;
        
        /// Set the reference frame.
        inline void setReferenceFrame(ReferenceFrame rf);
        
        /// Get whether this processor is enabled or not.
        inline bool isEnabled() const;
        
        /// Set whether this processor is enabled or not.
        inline void setEnabled(bool v);
        
        /// Get a pointer to the destination particle system.
        inline crParticleSystem* getParticleSystem();
        
        /// Get a const pointer to the destination particle system.
        inline const crParticleSystem* getParticleSystem() const;
        
        /// Set the destination particle system.
        inline void setParticleSystem(crParticleSystem* ps);
        
        /// Set the endless flag of this processor.
        inline void setEndless(bool type);
        
        /// Check whether this processor is endless.
        inline bool isEndless() const;
        
        /// Set the lifetime of this processor.
        inline void setLifeTime(double t);
        
        /// Get the lifetime of this processor.
        inline double getLifeTime() const;
        
        /// Set the start time of this processor.
        inline void setStartTime(double t);
        
        /// Get the start time of this processor.
        inline double getStartTime() const;

        /// Set the current time of this processor.
        inline void setCurrentTime(double t);
        
        /// Get the current time of this processor.
        inline double getCurrentTime() const;
        
        /// Set the reset time of this processor. A value of 0 disables reset.
        inline void setResetTime(double t);
        
        /// Get the reset time of this processor.
        inline double getResetTime() const;
        virtual void reset(){}
        /**
          Check whether the processor is alive with respect to start time and
          life duration. Note that this method may return true even if the 
          processor has been disabled by calling setEnabled(false). To test
          whether the processor is actually processing particles or not, you
          should evaluate (isEnabled() && isAlive()).
         */
        inline bool isAlive() const;
        
        inline const CRCore::crMatrix& getMatrix();//ltw      
        inline const CRCore::crMatrix& getPreviousMatrix();
        inline CRCore::crVector3 transformLocalToWorld(const CRCore::crVector3& P); 
        inline CRCore::crVector3 rotateLocalToWorld(const CRCore::crVector3& P);

		inline void updateMatrix(const CRCore::crMatrix &mat);
		inline void update(double dt);
    protected:
        virtual ~crParticleProcessor() {}
        crParticleProcessor& operator=(const crParticleProcessor&) { return *this; }
        
        virtual void process(double dt) = 0;
        
    private:
        ReferenceFrame m_rf;
        bool m_enabled;
        CRCore::ref_ptr<crParticleSystem> m_ps;
        CRCore::crMatrix m_matrix;//ltw
        CRCore::crMatrix m_previous_matrix;
        
        bool m_endless;
        
        double m_lifeTime;
        double m_startTime;
        double m_currentTime;
        double m_resetTime;
    };
    
    // INLINE FUNCTIONS
    
    inline crParticleProcessor::ReferenceFrame crParticleProcessor::getReferenceFrame() const
    {
        return m_rf;
    }
    
    inline void crParticleProcessor::setReferenceFrame(ReferenceFrame rf)
    {
        m_rf = rf;
    }
    
    inline bool crParticleProcessor::isEnabled() const
    {
        return m_enabled;
    }
    
    inline void crParticleProcessor::setEnabled(bool v)
    {
        m_enabled = v;
        if (m_enabled)
        {
            m_currentTime = 0;
        }
    }
    
    inline crParticleSystem* crParticleProcessor::getParticleSystem()
    {
        return m_ps.get();
    }

    inline const crParticleSystem* crParticleProcessor::getParticleSystem() const
    {
        return m_ps.get();
    }
    
    inline void crParticleProcessor::setParticleSystem(crParticleSystem* ps)
    {
        m_ps = ps;
    }
   
    inline void crParticleProcessor::setEndless(bool type)
    {
        m_endless = type;
    }
        
    inline bool crParticleProcessor::isEndless() const
    {
        return m_endless;
    }

    inline void crParticleProcessor::setLifeTime(double t)
    {
        m_lifeTime = t;
    }
        
    inline double crParticleProcessor::getLifeTime() const
    {
        return m_lifeTime;
    }
       
    inline void crParticleProcessor::setStartTime(double t)
    {
        m_startTime = t;
    }
        
    inline double crParticleProcessor::getStartTime() const
    {
        return m_startTime;
    }
    inline void crParticleProcessor::setCurrentTime(double t)
    {
        m_currentTime = t;
    }
        
    inline double crParticleProcessor::getCurrentTime() const
    {
        return m_currentTime;
    }
    
    inline void crParticleProcessor::setResetTime(double t)
    {
        m_resetTime = t;
    }
    
    inline double crParticleProcessor::getResetTime() const
    {
        return m_resetTime;
    }

    inline const CRCore::crMatrix& crParticleProcessor::getMatrix()    
    {
        return m_matrix;
    }
    
    inline const CRCore::crMatrix& crParticleProcessor::getPreviousMatrix()
    {
        return m_previous_matrix;
    }

    inline CRCore::crVector3 crParticleProcessor::transformLocalToWorld(const CRCore::crVector3& P)
    {
        return getMatrix().preMult(P);
    }
    
    inline CRCore::crVector3 crParticleProcessor::rotateLocalToWorld(const CRCore::crVector3& P)
    {
        return getMatrix().preMult(P) - 
            getMatrix().preMult(CRCore::crVector3(0, 0, 0));
    }
    
    inline bool crParticleProcessor::isAlive() const
    {
        return m_currentTime < (m_lifeTime + m_startTime);
    }

	inline void crParticleProcessor::updateMatrix(const CRCore::crMatrix &mat)
	{
        m_previous_matrix = m_matrix;
		m_matrix = mat;
	}

	inline void crParticleProcessor::update(double dt)
	{
		if (m_ps.valid())
		{
			if ((m_currentTime >= m_resetTime) && (m_resetTime > 0))
			{
				m_currentTime = 0;
				reset();
			}
			bool alive = false;
			if (m_currentTime >= m_startTime)
			{
				if (m_endless || (m_currentTime < (m_startTime + m_lifeTime)))
					alive = true;
			}
			m_currentTime += dt;
			if (alive && 
				m_enabled && 
				!m_ps->isFrozen() && 
				!m_ps->getFreezeOnCull())
			{
				process(dt);
			} 
		}
	}
}


#endif
