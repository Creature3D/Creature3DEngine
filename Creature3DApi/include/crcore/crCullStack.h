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
#ifndef CRCORE_CULLSTACK_H
#define CRCORE_CULLSTACK_H 1

#include <CRCore/crExport.h>
#include <CRCore/crCullingSet.h>
#include <CRCore/crCullSettings.h>
#include <CRCore/crViewPort.h>
#include <CRCore/fast_back_stack.h>

namespace CRCore {

/** A crCullStack class which accumulates the current project, modelview matrices
and the crCullingSet. */
class CR_EXPORT crCullStack : public crCullSettings
{

    public:
        crCullStack();
        
        ~crCullStack();
    
        typedef std::vector<crShadowVolumeOccluder>   OccluderList;

        void reset();
        
        void setOccluderList(const ShadowVolumeOccluderList& svol) { m_occluderList = svol; }
        ShadowVolumeOccluderList& getOccluderList() { return m_occluderList; }
        const ShadowVolumeOccluderList& getOccluderList() const { return m_occluderList; }

        void pushViewport(CRCore::crViewPort* viewport);
        void popViewport();

        void pushProjectionMatrix(CRCore::RefMatrix* matrix);
        void popProjectionMatrix();

        void pushModelViewMatrix(CRCore::RefMatrix* matrix);
        void popModelViewMatrix();

		void pushCameraViewInverseMatrix(CRCore::RefMatrix* matrix);			
		void popCameraViewInverseMatrix();

        inline float getFrustumVolume() { if (m_frustumVolume<0.0f) computeFrustumVolume(); return m_frustumVolume; }


        /** Compute the pixel of an object at position v, with specified radius.*/
        float pixelSize(const CRCore::crVector3& v,float radius) const
        {
            return getCurrentCullingSet().pixelSize(v,radius);
        }
        
        /** Compute the pixel of an bounding sphere.*/
        float pixelSize(const CRCore::crBoundingSphere& bs) const
        {
            return pixelSize(bs.center(),bs.radius());
        }

		/** Compute the pixel size of an object at position v, with specified radius. fabs()ed to always be positive. */
		float clampedPixelSize(const crVector3& v,float radius) const
		{
			return getCurrentCullingSet().clampedPixelSize(v,radius);
		}

		/** Compute the pixel size of the bounding sphere. fabs()ed to always be positive. */
		float clampedPixelSize(const crBoundingSphere& bs) const
		{
			return clampedPixelSize(bs.center(),bs.radius());
		}

        inline void disableAndPushOccludersCurrentMask(NodePath& nodePath)
        {
            getCurrentCullingSet().disableAndPushOccludersCurrentMask(nodePath);
        }

        inline void popOccludersCurrentMask(NodePath& nodePath)
        {
            getCurrentCullingSet().popOccludersCurrentMask(nodePath);
        }

        inline bool isCulled(const std::vector<CRCore::crVector3>& vertices)
        {
            return getCurrentCullingSet().isCulled(vertices);
        }

        inline bool isCulled(const CRCore::crBoundingBox& bb)
        {
            return bb.valid() && getCurrentCullingSet().isCulled(bb);
        }
        
        inline bool isCulled(const CRCore::crBoundingSphere& bs)
        {
            return getCurrentCullingSet().isCulled(bs);
        }
        
        inline bool isCulled(const CRCore::crNode& node)
        {
            return node.isCullingActive() && getCurrentCullingSet().isCulled(node.getBound());
        }

        inline void pushCurrentMask()
        {
            getCurrentCullingSet().pushCurrentMask();
        }
        
        inline void popCurrentMask()
        {
            getCurrentCullingSet().popCurrentMask();
        }
        
        
        typedef std::vector< crCullingSet > CullingStack;

        inline CullingStack& getClipSpaceCullingStack() { return m_clipspaceCullingStack; }
        
        inline CullingStack& getProjectionCullingStack() { return m_projectionCullingStack; }
        
        inline CullingStack& getModelViewCullingStack() { return m_modelviewCullingStack; }
        
//        inline crCullingSet& getCurrentCullingSet() { return m_modelviewCullingStack.back(); }
//        inline const crCullingSet& getCurrentCullingSet() const { return m_modelviewCullingStack.back(); }

        inline crCullingSet& getCurrentCullingSet() { return *m_back_modelviewCullingStack; }
        inline const crCullingSet& getCurrentCullingSet() const { return *m_back_modelviewCullingStack; }
        
        inline CRCore::crViewPort* getViewport();
        inline CRCore::RefMatrix& getModelViewMatrix();
        inline CRCore::RefMatrix& getProjectionMatrix();
		inline CRCore::RefMatrix& getCameraViewInverseMatrix();
        inline CRCore::crMatrix getWindowMatrix();
        inline const CRCore::RefMatrix& getMVPW();

        inline const CRCore::crVector3& getEyeLocal() const { return m_eyePointStack.back(); }

        inline const CRCore::crVector3 getUpLocal() const
        {
            const CRCore::crMatrix& matrix = *m_modelviewStack.back();
            return CRCore::crVector3(matrix(0,1),matrix(1,1),matrix(2,1));
        }

        inline const CRCore::crVector3 getLookVectorLocal() const
        {
            const CRCore::crMatrix& matrix = *m_modelviewStack.back();
            return CRCore::crVector3(-matrix(0,2),-matrix(1,2),-matrix(2,2));
        }
		//enum OCCLUDERFACE
		//{
		//	ALL,
		//	LEFT,
		//	RIGHT,
		//	FRONT,
		//	FRONTLEFT,
		//	FRONTRIGHT,
		//	BACK,
		//	BACKLEFT,
		//	BACKRIGHT
		//};
		//OCCLUDERFACE getOccluderFace(const crBoundingBox &bb) const;

	    inline CRCore::RefMatrix* createOrReuseMatrix(const CRCore::crMatrix& value);
    protected:
    
        void pushCullingSet();
        void popCullingSet();

        // base set of shadow volume occluder to use in culling.
        ShadowVolumeOccluderList                                    m_occluderList;

        typedef fast_back_stack< ref_ptr<RefMatrix> >               MatrixStack;

        MatrixStack                                                 m_projectionStack;

        MatrixStack                                                 m_modelviewStack;
        MatrixStack                                                 m_MVPW_Stack;
		MatrixStack                                                 m_cameraViewInverseStack;

        typedef fast_back_stack<ref_ptr<crViewPort> >               ViewportStack;
        ViewportStack                                               m_viewportStack;
        
        typedef fast_back_stack<CRCore::crVector3>                  EyePointStack;
        EyePointStack                                               m_eyePointStack;

        CullingStack                                                m_clipspaceCullingStack;
        CullingStack                                                m_projectionCullingStack;

        CullingStack                                                m_modelviewCullingStack;
        unsigned int                                                m_index_modelviewCullingStack;
        crCullingSet*                                               m_back_modelviewCullingStack;

        void computeFrustumVolume();
        float                                                       m_frustumVolume;

        unsigned int                                                m_bbCornerNear;
        unsigned int                                                m_bbCornerFar;

        ref_ptr<CRCore::RefMatrix>                                  m_identity;
        
		typedef std::vector< CRCore::ref_ptr<CRCore::RefMatrix> > MatrixList;
		MatrixList m_reuseMatrixList;
		unsigned int m_currentReuseMatrixIndex;
};

inline CRCore::crViewPort* crCullStack::getViewport()
{
    if (!m_viewportStack.empty())
    {
        return m_viewportStack.back().get();
    }
    else
    {
        return 0L;
    }
}

inline CRCore::RefMatrix& crCullStack::getModelViewMatrix()
{
    if (!m_modelviewStack.empty())
    {
        return *m_modelviewStack.back();
    }
    else
    {
        return *m_identity;
    }
}

inline CRCore::RefMatrix& crCullStack::getCameraViewInverseMatrix()
{
	if (!m_cameraViewInverseStack.empty())
	{
		return *m_cameraViewInverseStack.back();
	}
	else
	{
		return *m_identity;
	}
}

inline CRCore::RefMatrix& crCullStack::getProjectionMatrix()
{
    if (!m_projectionStack.empty())
    {
        return *m_projectionStack.back();
    }
    else
    {
        return *m_identity;
    }
}

inline CRCore::crMatrix crCullStack::getWindowMatrix()
{
    if (!m_viewportStack.empty())
    {
        CRCore::crViewPort* viewport = m_viewportStack.back().get();
        return viewport->computeWindowMatrix();
    }
    else
    {
        return *m_identity;
    }
}

inline const CRCore::RefMatrix& crCullStack::getMVPW()
{
    if (!m_MVPW_Stack.empty())
    {
        if (!m_MVPW_Stack.back())
        {
            m_MVPW_Stack.back() = createOrReuseMatrix(getModelViewMatrix());
            (*m_MVPW_Stack.back()) *= getProjectionMatrix();
            (*m_MVPW_Stack.back()) *= getWindowMatrix();
        }
        return *m_MVPW_Stack.back();
    }
    else
    {
        return *m_identity;
    }
}

inline RefMatrix* crCullStack::createOrReuseMatrix(const CRCore::crMatrix& value)
{
    // skip of any already reused matrix.
    while (m_currentReuseMatrixIndex<m_reuseMatrixList.size() && 
           m_reuseMatrixList[m_currentReuseMatrixIndex]->referenceCount()>1)
    {
        ++m_currentReuseMatrixIndex;
    }

    // if still within list, element must be singularly referenced
    // there return it to be reused.
    if (m_currentReuseMatrixIndex<m_reuseMatrixList.size())
    {
        RefMatrix* matrix = m_reuseMatrixList[m_currentReuseMatrixIndex++].get();
        matrix->set(value);
        return matrix;
    }

     //otherwise need to create new matrix.
    CRCore::RefMatrix* matrix = new RefMatrix(value);
    m_reuseMatrixList.push_back(matrix);
    ++m_currentReuseMatrixIndex;
    return matrix;
}

}	// end of namespace

#endif 
