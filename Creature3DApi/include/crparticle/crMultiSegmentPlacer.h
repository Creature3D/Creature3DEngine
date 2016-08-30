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
#ifndef CRPARTICLE_MULTISEGMENT_PLACER
#define CRPARTICLE_MULTISEGMENT_PLACER 1

#include <CRParticle/crExport.h>
#include <CRParticle/crPlacer.h>
#include <CRParticle/crParticle.h>

#include <vector>
#include <utility>

#include <CRCore/crNotify.h>
#include <CRCore/crCopyOp.h>
#include <CRCore/crBase.h>
#include <CRCore/crVector3.h>
#include <CRCore/crArgumentParser.h>

namespace CRParticle {

    /** A polyline-shaped particle placer.
        This placer class sets the position of incoming particles by choosing a random point on the
        specified sequence of connected segments.
    */
    class CRPARTICLE_EXPORT crMultiSegmentPlacer: public crPlacer {
    public:
        crMultiSegmentPlacer();
        crMultiSegmentPlacer(const crMultiSegmentPlacer& copy, const CRCore::crCopyOp& copyop = CRCore::crCopyOp::SHALLOW_COPY);

        META_Base(CRParticle, crMultiSegmentPlacer);

        /// Get the number of vertices which define the segments.
        inline int numVertices() const;
        
        /// Get a vertex.
        inline const CRCore::crVector3& getVertex(int i) const;
        
        /// Set a vertex.
        inline void setVertex(int i, const CRCore::crVector3& v);
        
        /// Set a vertex.
        inline void setVertex(int i, float x, float y, float z);
        
        /// Add a vertex.
        inline void addVertex(const CRCore::crVector3& v);
        
        /// Add a vertex.
        inline void addVertex(float x, float y, float z);
        
        /// Remove a vertex.
        inline void removeVertex(int i);
        
        /// Place a partice. Called automatically by <CODE>crModularEmitter</CODE>, do not call this method manually.
        void place(crParticle* P) const;

        /// return the control position
        inline CRCore::crVector3 getControlPosition() const;

		inline virtual void addPlacerParam(int i, const std::string &str)
		{
			CRCore::crVector3 vertex;
			CRCore::crArgumentParser::appAtoVec(str,vertex);
            addVertex(vertex);
		}
		void reset();

		/// return the length of the multi-segment
		inline float volume() const;
    protected:
        virtual ~crMultiSegmentPlacer() {}
        crMultiSegmentPlacer& operator=(const crMultiSegmentPlacer&) { return *this; }        

    private:
        typedef std::pair<CRCore::crVector3, float> Vertex_data;
        typedef std::vector<Vertex_data> Vertex_vector;
        
        Vertex_vector m_vx;
        float m_total_length;
        
        void recompute_length();
    };

    // INLINE FUNCTIONS
    

    inline int crMultiSegmentPlacer::numVertices() const
    {
        return static_cast<int>(m_vx.size());
    }

    inline const CRCore::crVector3& crMultiSegmentPlacer::getVertex(int i) const
    {
        return m_vx[i].first;
    }

    inline void crMultiSegmentPlacer::setVertex(int i, const CRCore::crVector3& v)
    {
        m_vx[i].first = v;
        recompute_length();
    }

    inline void crMultiSegmentPlacer::setVertex(int i, float x, float y, float z)
    {
        m_vx[i].first.set(x, y, z);
        recompute_length();
    }

    inline void crMultiSegmentPlacer::addVertex(const CRCore::crVector3& v)
    {
        float l = 0;        
        if (m_vx.size() > 0) {
            l = (v - m_vx.back().first).length();
        }                
        m_total_length += l;
        m_vx.push_back(std::make_pair(v, m_total_length));        
    }

    inline void crMultiSegmentPlacer::addVertex(float x, float y, float z)
    {
        addVertex(CRCore::crVector3(x, y, z));
    }

    inline void crMultiSegmentPlacer::removeVertex(int i)
    {
        m_vx.erase(m_vx.begin()+i);
        recompute_length();
    }
    
    inline CRCore::crVector3 crMultiSegmentPlacer::getControlPosition() const
    {
        return m_vx.empty() ? CRCore::crVector3(0.0f,0.0f,0.0f) : m_vx[0].first;
    }
	inline float crMultiSegmentPlacer::volume() const
	{
		return m_total_length;
	}
}
#endif
