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
#ifndef CRCORE_CRCULLINGSET_H
#define CRCORE_CRCULLINGSET_H 1

#include <CRCore/crExport.h>
#include <CRCore/crPolytope.h>
#include <CRCore/crShadowVolumeOccluder.h>
#include <CRCore/Referenced.h>
#include <CRCore/crNode.h>
#include <CRCore/crViewPort.h>
#include <CRCore/crMatrix.h>

namespace CRCore {
#define COMPILE_WITH_SHADOW_OCCLUSION_CULLING
class CR_EXPORT crCullingSet : public Referenced
{

    public:
    
		typedef std::pair< ref_ptr<crStateSet>, crPolytope > StateFrustumPair;
		typedef std::vector< StateFrustumPair > StateFrustumList;   
		typedef std::vector< crPolytope > FrustumList;

        crCullingSet();

        crCullingSet(const crCullingSet& cs):
            Referenced(),
            m_mask(cs.m_mask),
            m_frustum(cs.m_frustum),
			m_stateFrustumList(cs.m_stateFrustumList),
            m_occluderList(cs.m_occluderList),
            m_pixelSizeVector(cs.m_pixelSizeVector),
            m_smallFeatureCullingPixelSize(cs.m_smallFeatureCullingPixelSize)
        {
        }

        crCullingSet(const crCullingSet& cs,const crMatrix& matrix, const crVector4& pixelSizeVector):
            m_mask(cs.m_mask),
            m_frustum(cs.m_frustum),
			m_stateFrustumList(cs.m_stateFrustumList),
            m_occluderList(cs.m_occluderList),
            m_pixelSizeVector(pixelSizeVector),
            m_smallFeatureCullingPixelSize(cs.m_smallFeatureCullingPixelSize)
        {
            m_frustum.transformProvidingInverse(matrix);
           for(OccluderList::iterator itr=m_occluderList.begin();
                itr!=m_occluderList.end();
                ++itr)
            {
                itr->transformProvidingInverse(matrix);
            }
        }
        
        crCullingSet& operator = (const crCullingSet& cs)
        {
            if (this==&cs) return *this;
            
            m_mask = cs.m_mask;
            m_frustum = cs.m_frustum;
			m_stateFrustumList = cs.m_stateFrustumList;
            m_occluderList = cs.m_occluderList;
            m_pixelSizeVector = cs.m_pixelSizeVector;
            m_smallFeatureCullingPixelSize = cs.m_smallFeatureCullingPixelSize;
            
            return *this;
        }

		inline void set(const crCullingSet& cs)
		{
			m_mask = cs.m_mask;
			m_frustum = cs.m_frustum;
			m_stateFrustumList = cs.m_stateFrustumList;
			m_occluderList = cs.m_occluderList;
			m_pixelSizeVector = cs.m_pixelSizeVector;
			m_smallFeatureCullingPixelSize = cs.m_smallFeatureCullingPixelSize;
		}
		inline void set(const crCullingSet& cs,const crMatrix& matrix, const crVector4& pixelSizeVector)
		{
			m_mask = cs.m_mask;
			m_stateFrustumList = cs.m_stateFrustumList;
			m_occluderList = cs.m_occluderList;
			m_pixelSizeVector = pixelSizeVector;
			m_smallFeatureCullingPixelSize = cs.m_smallFeatureCullingPixelSize;

			//m_frustum = cs.m_frustum;
			//m_frustum.transformProvidingInverse(matrix);

			m_frustum.setAndTransformProvidingInverse(cs.m_frustum,matrix);

			for(StateFrustumList::iterator sitr=m_stateFrustumList.begin();
				sitr!=m_stateFrustumList.end();
				++sitr)
			{
				sitr->second.transformProvidingInverse(matrix);
			}

			for(OccluderList::iterator oitr=m_occluderList.begin();
				oitr!=m_occluderList.end();
				++oitr)
			{
				oitr->transformProvidingInverse(matrix);
			}

		}
       typedef std::vector<crShadowVolumeOccluder>   OccluderList;

        typedef unsigned int Mask;

        enum MaskValues
        {
            NO_CULLING                  = 0x0,
            VIEW_FRUSTUM_SIDES_CULLING  = 0x1,
            NEAR_PLANE_CULLING          = 0x2,
            FAR_PLANE_CULLING           = 0x4,
            VIEW_FRUSTUM_CULLING        = VIEW_FRUSTUM_SIDES_CULLING|
                                          NEAR_PLANE_CULLING|
                                          FAR_PLANE_CULLING,
            SMALL_FEATURE_CULLING       = 0x8,
            SHADOW_OCCLUSION_CULLING    = 0x10,
			CLUSTER_CULLING             = 0x20,
            EXTERN_FRUSTUM_CULLING      = 0x40,
			EXTERN_OCCLUSION_CULLING    = 0x80,
            DEFAULT_CULLING             = VIEW_FRUSTUM_SIDES_CULLING|
                                          SMALL_FEATURE_CULLING|
                                          SHADOW_OCCLUSION_CULLING|
										  CLUSTER_CULLING,
            ENABLE_ALL_CULLING          = VIEW_FRUSTUM_CULLING|
                                          SMALL_FEATURE_CULLING|
                                          SHADOW_OCCLUSION_CULLING|
										  CLUSTER_CULLING
        };
        
        inline void setCullingMask(Mask mask) { m_mask = mask; }
		inline Mask getCullingMask() const { return m_mask; }

        inline void setFrustum(crPolytope& cv) { m_frustum = cv; }

        inline crPolytope& getFrustum() { return m_frustum; }
        inline const crPolytope& getFrustum() const { return m_frustum; }    
        
		inline void addStateFrustum(crStateSet* stateset, crPolytope& polytope) { m_stateFrustumList.push_back(StateFrustumPair(stateset,polytope)); }
		inline void setStateFrustumList(StateFrustumList& sfl) { m_stateFrustumList = sfl; }
		inline StateFrustumList& getStateFrustumList() { return m_stateFrustumList; }

		inline void addExternFrustum(const crPolytope& polytope) { m_externFrustumList.push_back(polytope); }
		inline void setExternFrustumList(const FrustumList& efl) { m_externFrustumList = efl; }
		inline FrustumList& getExternFrustumList() { return m_externFrustumList; }
		inline void clearExternFrustum(){ m_externFrustumList.clear(); }

		inline void addExternOccluderFrustum(const crPolytope& polytope) { m_externOccluderFrustumList.push_back(polytope); }
		inline void setExternOccluderFrustumList(const FrustumList& efl) { m_externOccluderFrustumList = efl; }
		inline FrustumList& getExternOccluderFrustumList() { return m_externOccluderFrustumList; }
		inline void clearExternOccluderFrustum(){ m_externOccluderFrustumList.clear(); }

        inline void addOccluder(crShadowVolumeOccluder& cv) { m_occluderList.push_back(cv); }


        inline void setPixelSizeVector(const crVector4& v) { m_pixelSizeVector = v; }

        inline crVector4& getPixelSizeVector() { return m_pixelSizeVector; }
        inline const crVector4& getPixelSizeVector() const { return m_pixelSizeVector; }

        inline void setSmallFeatureCullingPixelSize(float value) { m_smallFeatureCullingPixelSize=value; }
        
        inline float& getSmallFeatureCullingPixelSize() { return m_smallFeatureCullingPixelSize; }

        inline float getSmallFeatureCullingPixelSize() const { return m_smallFeatureCullingPixelSize; }
    
    
        /** Compute the pixel of an object at position v, with specified radius.*/
        inline float pixelSize(const crVector3& v,float radius) const { return radius/(v*m_pixelSizeVector); }

        /** Compute the pixel of an bounding sphere.*/
        inline float pixelSize(const crBoundingSphere& bs) const { return bs.radius()/(bs.center()*m_pixelSizeVector); }

		/** Compute the pixel of an object at position v, with specified radius. fabs()ed to always be positive. */
		inline float clampedPixelSize(const crVector3& v,float radius) const { return fabs(pixelSize(v, radius)); }

		/** Compute the pixel of a bounding sphere. fabs()ed to always be positive. */
		inline float clampedPixelSize(const crBoundingSphere& bs) const { return fabs(pixelSize(bs)); }

        inline bool isCulled(const std::vector<crVector3>& vertices)
        {
            if (m_mask&VIEW_FRUSTUM_CULLING)
            {
                // is it outside the view frustum...
                if (!m_frustum.contains(vertices)) return true;
            }
            
			if (m_mask&EXTERN_FRUSTUM_CULLING)
			{
				for(FrustumList::iterator itr = m_externFrustumList.begin();
					itr != m_externFrustumList.end();
					++itr)
				{
					if (!itr->contains(vertices)) return true;
				}
			}

			if (m_mask&EXTERN_OCCLUSION_CULLING)
			{
				for(FrustumList::iterator itr = m_externOccluderFrustumList.begin();
					itr != m_externOccluderFrustumList.end();
					++itr)
				{
					if (itr->containsAllOf(vertices)) return true;
				}
			}

            if (m_mask&SMALL_FEATURE_CULLING)
            {
            }

            if (m_mask&SHADOW_OCCLUSION_CULLING)
            {
                // is it in one of the shadow occluder volumes.
                if (!m_occluderList.empty())
                {
                    for(OccluderList::iterator itr=m_occluderList.begin();
                        itr!=m_occluderList.end();
                        ++itr)
                    {
                        if (itr->contains(vertices)) return true;
                    }
                }
            }
                        
            return false;
        }
        
        inline bool isCulled(const crBoundingBox& bb)
        {
            if (m_mask&VIEW_FRUSTUM_CULLING)
            {
                // is it outside the view frustum...
                if (!m_frustum.contains(bb)) return true;
            }

			if (m_mask&EXTERN_FRUSTUM_CULLING)
			{
				for(FrustumList::iterator itr = m_externFrustumList.begin();
					itr != m_externFrustumList.end();
					++itr)
				{
					if (!itr->contains(bb)) return true;
				}
			}

			if (m_mask&EXTERN_OCCLUSION_CULLING)
			{
				for(FrustumList::iterator itr = m_externOccluderFrustumList.begin();
					itr != m_externOccluderFrustumList.end();
					++itr)
				{
					if (itr->containsAllOf(bb)) return true;
				}
			}

			if (m_mask&SMALL_FEATURE_CULLING)
            {
            }

            if (m_mask&SHADOW_OCCLUSION_CULLING)
            {
                // is it in one of the shadow occluder volumes.
                if (!m_occluderList.empty())
                {
                    for(OccluderList::iterator itr=m_occluderList.begin();
                        itr!=m_occluderList.end();
                        ++itr)
                    {
                        if (itr->contains(bb)) return true;
                    }
                }
            }
                        
            return false;
        }
        
		inline bool isCulled(const crBoundingSphere& bs)
		{
			if (m_mask&VIEW_FRUSTUM_CULLING)
			{
				// is it outside the view frustum...
				if (!m_frustum.contains(bs)) return true;
			}

			if (m_mask&EXTERN_FRUSTUM_CULLING)
			{
				for(FrustumList::iterator itr = m_externFrustumList.begin();
					itr != m_externFrustumList.end();
					++itr)
				{
					if (!itr->contains(bs)) return true;
				}
			}

			if (m_mask&EXTERN_OCCLUSION_CULLING)
			{
				for(FrustumList::iterator itr = m_externOccluderFrustumList.begin();
					itr != m_externOccluderFrustumList.end();
					++itr)
				{
					if (itr->containsAllOf(bs)) return true;
				}
			}

			if (m_mask&SMALL_FEATURE_CULLING)
			{
				if (((bs.center()*m_pixelSizeVector)*m_smallFeatureCullingPixelSize)>bs.radius()) return true;
			}
#ifdef COMPILE_WITH_SHADOW_OCCLUSION_CULLING
			if (m_mask&SHADOW_OCCLUSION_CULLING)
			{
				// is it in one of the shadow occluder volumes.
				if (!m_occluderList.empty())
				{
					for(OccluderList::iterator itr=m_occluderList.begin();
						itr!=m_occluderList.end();
						++itr)
					{
						if (itr->contains(bs)) return true;
					}
				}
			}
#endif            
			return false;
		}

		inline void pushCurrentMask()
		{
			m_frustum.pushCurrentMask();

			for(FrustumList::iterator itr = m_externFrustumList.begin();
				itr != m_externFrustumList.end();
				++itr)
			{
				itr->pushCurrentMask();
			}

			for(FrustumList::iterator itr = m_externOccluderFrustumList.begin();
				itr != m_externOccluderFrustumList.end();
				++itr)
			{
				itr->pushCurrentMask();
			}

			if (!m_stateFrustumList.empty())
			{
				for(StateFrustumList::iterator itr=m_stateFrustumList.begin();
					itr!=m_stateFrustumList.end();
					++itr)
				{
					itr->second.pushCurrentMask();
				}
			}


#ifdef COMPILE_WITH_SHADOW_OCCLUSION_CULLING
			if (!m_occluderList.empty())
			{
				for(OccluderList::iterator itr=m_occluderList.begin();
					itr!=m_occluderList.end();
					++itr)
				{
					itr->pushCurrentMask();
				}
			}
#endif
		}

		inline void popCurrentMask()
		{
			m_frustum.popCurrentMask();

			for(FrustumList::iterator itr = m_externFrustumList.begin();
				itr != m_externFrustumList.end();
				++itr)
			{
				itr->popCurrentMask();
			}

			for(FrustumList::iterator itr = m_externOccluderFrustumList.begin();
				itr != m_externOccluderFrustumList.end();
				++itr)
			{
				itr->popCurrentMask();
			}

			if (!m_stateFrustumList.empty())
			{
				for(StateFrustumList::iterator itr=m_stateFrustumList.begin();
					itr!=m_stateFrustumList.end();
					++itr)
				{
					itr->second.popCurrentMask();
				}
			}

#ifdef COMPILE_WITH_SHADOW_OCCLUSION_CULLING
			if (!m_occluderList.empty())
			{
				for(OccluderList::iterator itr=m_occluderList.begin();
					itr!=m_occluderList.end();
					++itr)
				{
					itr->popCurrentMask();
				}
			}
#endif
		}
        
        void disableAndPushOccludersCurrentMask(NodePath& nodePath);

        void popOccludersCurrentMask(NodePath& nodePath);
        
		static crVector4 computePixelSizeVector(const crViewPort& W, const crMatrix& P, const crMatrix& M);

        virtual ~crCullingSet();

    protected:
    

        Mask            m_mask;
        crPolytope        m_frustum;
		StateFrustumList  m_stateFrustumList;
		FrustumList m_externFrustumList;
		FrustumList m_externOccluderFrustumList;
        OccluderList    m_occluderList;        
        crVector4            m_pixelSizeVector;
        float           m_smallFeatureCullingPixelSize;

};

}

#endif 
