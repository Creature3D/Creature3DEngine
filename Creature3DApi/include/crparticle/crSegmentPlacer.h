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
#ifndef CRPARTICLE_SEGMENT_PLACER
#define CRPARTICLE_SEGMENT_PLACER 1

#include <CRParticle/crPlacer.h>
#include <CRParticle/crParticle.h>

#include <CRCore/crCopyOp.h>
#include <CRCore/crBase.h>
#include <CRCore/crVector3.h>

namespace CRParticle {

    /**    A segment-shaped particle placer.
        To use this placer you have to define a segment, by setting its two vertices (<B>A</B> and <B>B</B>);
        when an emitter requests a <CODE>crSegmentPlacer</CODE> to place a particle, the position is chosen randomly
        within that segment.
    */
    class crSegmentPlacer: public crPlacer {
    public:
        inline crSegmentPlacer();
        inline crSegmentPlacer(const crSegmentPlacer& copy, const CRCore::crCopyOp& copyop = CRCore::crCopyOp::SHALLOW_COPY);

        META_Base(CRParticle, crSegmentPlacer);

        /// get vertex <B>A</B>.
        inline const CRCore::crVector3& getVertexA() const;
        
        /// Set vertex <B>A</B> of the segment as a vector.
        inline void setVertexA(const CRCore::crVector3& v);
        
        /// Set vertex <B>A</B> of the segment as three floats.
        inline void setVertexA(float x, float y, float z);
        
        /// get vertex <B>B</B>.
        inline const CRCore::crVector3& getVertexB() const;
        
        /// Set vertex <B>B</B> of the segment as a vector.
        inline void setVertexB(const CRCore::crVector3& v);
        
        /// Set vertex <B>B</B> of the segment as three floats.
        inline void setVertexB(float x, float y, float z);
        
        /// Set both vertices.
        inline void setSegment(const CRCore::crVector3& A, const CRCore::crVector3& B);
        
        /// Place a particle. This method is called by <CODE>crModularEmitter</CODE>, do not call it manually.
        inline void place(crParticle* P) const;

        /// return the control position
        inline CRCore::crVector3 getControlPosition() const;

		inline virtual void addPlacerParam(int i, const std::string &str)
		{
			switch(i) 
			{
			case 0:
				CRCore::crArgumentParser::appAtoVec(str,m_vertexA);
				break;
			case 1:
				CRCore::crArgumentParser::appAtoVec(str,m_vertexB);
				break;
			}
		}
		/// return the length of the segment
		inline float volume() const;
    protected:
        virtual ~crSegmentPlacer() {}
        crSegmentPlacer& operator=(const crSegmentPlacer&) { return *this; }        

    private:
        CRCore::crVector3 m_vertexA;
        CRCore::crVector3 m_vertexB;
    };

    // INLINE FUNCTIONS
    
    inline crSegmentPlacer::crSegmentPlacer()
    : crPlacer(), m_vertexA(-1, 0, 0), m_vertexB(1, 0, 0)
    {
    }
    
    inline crSegmentPlacer::crSegmentPlacer(const crSegmentPlacer& copy, const CRCore::crCopyOp& copyop)
    : crPlacer(copy, copyop), m_vertexA(copy.m_vertexA), m_vertexB(copy.m_vertexB)
    {
    }

    inline const CRCore::crVector3& crSegmentPlacer::getVertexA() const
    {
        return m_vertexA;
    }

    inline const CRCore::crVector3& crSegmentPlacer::getVertexB() const
    {
        return m_vertexB;
    }

    inline void crSegmentPlacer::setSegment(const CRCore::crVector3& A, const CRCore::crVector3& B)
    {
        m_vertexA = A;
        m_vertexB = B;
    }

    inline void crSegmentPlacer::place(crParticle* P) const
    {
		P->setPosition(CRCore::rangev3(m_vertexA, m_vertexB).get_random());
    }
    
    inline void crSegmentPlacer::setVertexA(const CRCore::crVector3& v)
    {
        m_vertexA = v;
    }
    
    inline void crSegmentPlacer::setVertexA(float x, float y, float z)
    {
        m_vertexA.set(x, y, z);
    }
    
    inline void crSegmentPlacer::setVertexB(const CRCore::crVector3& v)
    {
        m_vertexB = v;
    }

    inline void crSegmentPlacer::setVertexB(float x, float y, float z)
    {
        m_vertexB.set(x, y, z);
    }    
    
    inline CRCore::crVector3 crSegmentPlacer::getControlPosition() const
    {
        return (m_vertexA+m_vertexB)*0.5f;
    }
	inline float crSegmentPlacer::volume() const
	{
		return (m_vertexB - m_vertexA).length();
	}
}

#endif
