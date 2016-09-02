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
//Modified by 吴财华

#ifndef CRCORE_CRANIMATIONPATH_H
#define CRCORE_CRANIMATIONPATH_H 1

#include <CRCore/crMatrixf.h>
#include <CRCore/crMatrixd.h>
#include <CRCore/crQuat.h>
#include <CRCore/crNodeCallback.h>
#include <CRCore/thread/crMutex.h>
#include <CRCore/thread/crScopedLock.h>
#include <map>
#include <float.h>

namespace CRCore {

class CR_EXPORT crAnimationPath : public virtual CRCore::crBase
{
    public:
    
        crAnimationPath():m_loopMode(LOOP) {}

        crAnimationPath(const crAnimationPath& ap, const crCopyOp& copyop=crCopyOp::SHALLOW_COPY):
            crBase(ap,copyop),
            m_timeControlPointMap(ap.m_timeControlPointMap),
            m_loopMode(ap.m_loopMode) {}

        META_Base(CRCore,crAnimationPath);

        struct CR_EXPORT ControlPoint
        {
            ControlPoint():
                m_scale(1.0f,1.0f,1.0f) {}

            ControlPoint(const CRCore::crVector3d& position):
                m_position(position),
                m_rotation(),
                m_scale(1.0f,1.0f,1.0f) {}

            ControlPoint(const CRCore::crVector3d& position, const CRCore::crQuat& rotation):
                m_position(position),
                m_rotation(rotation),
                m_scale(1.0f,1.0f,1.0f) {}

            ControlPoint(const CRCore::crVector3d& position, const CRCore::crQuat& rotation, const CRCore::crVector3d& scale):
                m_position(position),
                m_rotation(rotation),
                m_scale(scale) {}
        
            CRCore::crVector3d m_position;
            CRCore::crQuat     m_rotation;
            CRCore::crVector3d m_scale;

			void setPosition(const CRCore::crVector3d& position) { m_position = position; }
			const CRCore::crVector3d& getPosition() const { return m_position; }

			void setRotation(const CRCore::crQuat& rotation) { m_rotation = rotation; }
			const CRCore::crQuat& getRotation() const { return m_rotation; }

			void setScale(const CRCore::crVector3d& scale) { m_scale = scale; }
			const CRCore::crVector3d& getScale() const { return m_scale; }

            inline void interpolate(float ratio,const ControlPoint& first, const ControlPoint& second)
            {
                float one_minus_ratio = 1.0f-ratio;
                m_position = first.m_position*one_minus_ratio + second.m_position*ratio;
                m_rotation.slerp(ratio,first.m_rotation,second.m_rotation);
                m_scale = first.m_scale*one_minus_ratio + second.m_scale*ratio;
            }
            
            void getMatrix(crMatrixf& matrix) const;
            void getMatrix(crMatrixd& matrix) const;
            void getInverse(crMatrixf& matrix) const;
            void getInverse(crMatrixd& matrix) const;
        };
        

        /** get the transformation matrix for a point in time.*/        
        bool getMatrix(double time,crMatrixf& matrix) const
        {
            ControlPoint cp;
            if (!getInterpolatedControlPoint(time,cp)) return false;
            cp.getMatrix(matrix);
            return true;
        }

        /** get the transformation matrix for a point in time.*/        
        bool getMatrix(double time,crMatrixd& matrix) const
        {
            ControlPoint cp;
            if (!getInterpolatedControlPoint(time,cp)) return false;
            cp.getMatrix(matrix);
            return true;
        }

        /** get the inverse transformation matrix for a point in time.*/        
        bool getInverse(double time,crMatrixf& matrix) const
        {
            ControlPoint cp;
            if (!getInterpolatedControlPoint(time,cp)) return false;
            cp.getInverse(matrix);
            return true;
        }
        
        bool getInverse(double time,crMatrixd& matrix) const
        {
            ControlPoint cp;
            if (!getInterpolatedControlPoint(time,cp)) return false;
            cp.getInverse(matrix);
            return true;
        }

        /** get the local ControlPoint frame for a point in time.*/
        virtual bool getInterpolatedControlPoint(double time,ControlPoint& controlPoint) const;
        
        void insert(double time,const ControlPoint& controlPoint);
        
        double getFirstTime() const 
		{
			CRCore::ScopedLock<crMutex> lock(m_mutex);
			if (!m_timeControlPointMap.empty()) return m_timeControlPointMap.begin()->first; else return 0.0;
		}
        double getLastTime() const 
		{
			CRCore::ScopedLock<crMutex> lock(m_mutex);
			if (!m_timeControlPointMap.empty()) return m_timeControlPointMap.rbegin()->first; else return 0.0;
		}
        double getPeriod() const { return getLastTime()-getFirstTime();}
        
        enum LoopMode
        {
            SWING,
            LOOP,
            NO_LOOPING
        };
        
        void setLoopMode(LoopMode lm) { m_loopMode = lm; }
        
        LoopMode getLoopMode() const { return m_loopMode; }


        typedef std::map<double,ControlPoint> TimeControlPointMap;
        
        TimeControlPointMap& getTimeControlPointMap() { return m_timeControlPointMap; }
        
        const TimeControlPointMap& getTimeControlPointMap() const { return m_timeControlPointMap; }
        
		bool empty() const { return m_timeControlPointMap.empty(); }
        /** read the anumation path from a flat ascii file stream.*/
        void read(std::istream& in);

        /** write the anumation path to a flat ascii file stream.*/
        void write(std::ostream& out);

		void addEventParam(int i, const std::string &str);

		inline void lock(){ m_mutex.lock(); }
		inline void unlock(){ m_mutex.unlock(); }
		inline crMutex &getNodeMutex() { return m_mutex; }
    protected:
    
        virtual ~crAnimationPath() {}

        TimeControlPointMap m_timeControlPointMap;
        LoopMode            m_loopMode;
	private:
		double m_time_load;
		CRCore::crVector3 m_position_load;
		CRCore::crQuat m_rotation_load;
		CRCore::crVector3 m_scale_load;
		mutable crMutex                  m_mutex;
};


class CR_EXPORT crAnimationPathCallback : public crNodeCallback
{
    public:

        crAnimationPathCallback():
		    m_pivotPoint(0.0,0.0,0.0),
			m_useInverseMatrix(false),
            m_timeOffset(0.0),
            m_timeMultiplier(1.0),
            m_firstTime(DBL_MAX),
            m_latestTime(0.0),
            m_pause(false),
            m_pauseTime(0.0) 
			{
				m_animationPath = new crAnimationPath;
			}
            

        crAnimationPathCallback(const crAnimationPathCallback& apc,const crCopyOp& copyop):
            crNodeCallback(apc,copyop),
            //m_animationPath(apc.m_animationPath),
			m_pivotPoint(apc.m_pivotPoint),
            m_useInverseMatrix(apc.m_useInverseMatrix),
            m_timeOffset(apc.m_timeOffset),
            m_timeMultiplier(apc.m_timeMultiplier),
            m_firstTime(apc.m_firstTime),
            m_latestTime(apc.m_latestTime),
            m_pause(apc.m_pause),
            m_pauseTime(apc.m_pauseTime) 
			{
				m_animationPath = dynamic_cast<crAnimationPath *>(apc.m_animationPath->clone(crCopyOp::DEEP_COPY_ALL));
			}

		//crAnimationPathCallback(const crAnimationPathCallback& apc):
		//	crNodeCallback(apc),
		//	m_pivotPoint(apc.m_pivotPoint),
		//	m_useInverseMatrix(apc.m_useInverseMatrix),
		//	m_timeOffset(apc.m_timeOffset),
		//	m_timeMultiplier(apc.m_timeMultiplier),
		//	m_firstTime(apc.m_firstTime),
		//	m_latestTime(apc.m_latestTime),
		//	m_pause(apc.m_pause),
		//	m_pauseTime(apc.m_pauseTime)
		//	{
		//		m_animationPath = dynamic_cast<crAnimationPath *>(apc.m_animationPath->clone(crCopyOp::DEEP_COPY_ALL));
		//	}

		META_EventCallback(CRCore, AnimationPath)
		virtual void addEventParam(int i, const std::string &str);

        crAnimationPathCallback(crAnimationPath* ap,double timeOffset=0.0f,double timeMultiplier=1.0f):
            m_animationPath(ap),
		    m_pivotPoint(0.0,0.0,0.0),
            m_useInverseMatrix(false),
            m_timeOffset(timeOffset),
            m_timeMultiplier(timeMultiplier),
            m_firstTime(DBL_MAX),
            m_latestTime(0.0),
            m_pause(false),
            m_pauseTime(0.0) {}
            
        void setAnimationPath(crAnimationPath* path) { m_animationPath = path; }
        crAnimationPath* getAnimationPath() { return m_animationPath.get(); }
        const crAnimationPath* getAnimationPath() const { return m_animationPath.get(); }
        
		inline void setPivotPoint(const crVector3d& pivot) { m_pivotPoint = pivot; }
		inline const crVector3d& getPivotPoint() const { return m_pivotPoint; }

		void setUseInverseMatrix(bool useInverseMatrix) { m_useInverseMatrix = useInverseMatrix; }
		bool getUseInverseMatrix() const { return m_useInverseMatrix; }

		void setTimeOffset(double timeOffset) { m_timeOffset = timeOffset; }
		double getTimeOffset() const { return m_timeOffset; }

		void setTimeMultiplier(double timeMultiplier) { m_timeMultiplier = timeMultiplier; }
		double getTimeMultiplier() const { return m_timeMultiplier; }

        void reset();

        void setPause(bool pause);
		bool getPause() const { return m_pause; }

        /** get the animation time that is used to specify the position along the crAnimationPath.
          * Animation time is computed from the formula ((m_latestTime-m_firstTime)-m_timeOffset)*m_timeMultiplier.*/
        double getAnimationTime() const;

        /** implements the callback*/
        virtual void operator()(crNode* node, crNodeVisitor* nv);
        
        void update(CRCore::crNode& node);

    public:

        ref_ptr<crAnimationPath>  m_animationPath;
		CRCore::crVector3d        m_pivotPoint;
        bool                    m_useInverseMatrix;
        double                  m_timeOffset;
        double                  m_timeMultiplier;
        double                  m_firstTime;
        double                  m_latestTime;
        bool                    m_pause;
        double                  m_pauseTime;
};

}

#endif
