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
#ifndef CRCORE_CRDOFTRANSFORM
#define CRCORE_CRDOFTRANSFORM 1

//base class:
#include <CRCore/crTransform.h>

#include <CRCore/crExport.h>

namespace CRCore {

/** crDOFTransform - encapsulates Multigen DOF behavior*/
class CR_EXPORT crDOFTransform : public CRCore::crTransform
{
    public:
		enum DOFMask
		{
			XTran  = 0x0001,
			YTran  = 0x0002,
			ZTran  = 0x0004,
			XRot   = 0x0008,
			YRot   = 0x0010,
			ZRot   = 0x0020,
			XScale = 0x0040,
			YScale = 0x0080,
			ZScale = 0x0100,
			ALL = XTran|YTran|ZTran|XRot|YRot|ZRot|XScale|YScale|ZScale
		};
        /** constructor*/
        crDOFTransform();

        /**copy constructor*/
        crDOFTransform(const crDOFTransform& dof, const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);

        META_Node(CRCore, crDOFTransform);

        virtual void traverse(CRCore::crNodeVisitor& nv);


        void setMinHPR(const CRCore::crVector3& hpr) { m_minHPR = hpr;}
        const CRCore::crVector3& getMinHPR() const { return m_minHPR;}

        void setMaxHPR(const CRCore::crVector3& hpr) {m_maxHPR = hpr;}
        const CRCore::crVector3& getMaxHPR() const { return m_maxHPR;}

        void setIncrementHPR(const CRCore::crVector3& hpr) {m_incrementHPR = hpr;}
        const CRCore::crVector3& getIncrementHPR() const { return m_incrementHPR;}

        void setCurrentHPR(const CRCore::crVector3& hpr) {m_currentHPR = hpr;}
        const CRCore::crVector3& getCurrentHPR() const {return m_currentHPR;}

        void updateCurrentHPR(const CRCore::crVector3& hpr);


        void setMinTranslate(const CRCore::crVector3& translate) {m_minTranslate = translate;}
        const CRCore::crVector3& getMinTranslate() const { return m_minTranslate;}

        void setMaxTranslate(const CRCore::crVector3& translate) {m_maxTranslate = translate;}
        const CRCore::crVector3& getMaxTranslate() const { return m_maxTranslate;}

        void setIncrementTranslate(const CRCore::crVector3& translate) { m_incrementTranslate = translate;}
        const CRCore::crVector3& getIncrementTranslate() const { return m_incrementTranslate;}

        void setCurrentTranslate(const CRCore::crVector3& translate){ m_currentTranslate = translate;}
        inline const CRCore::crVector3& getCurrentTranslate() const { return m_currentTranslate;}

        void updateCurrentTranslate(const CRCore::crVector3& translate);


        void setMinScale(const CRCore::crVector3& scale) { m_minScale = scale;}
        const CRCore::crVector3& getMinScale() const { return m_minScale;}

        void setMaxScale(const CRCore::crVector3& scale) { m_maxScale = scale;}
        const CRCore::crVector3& getMaxScale() const { return m_maxScale;}

        void setIncrementScale(const CRCore::crVector3& scale) { m_incrementScale = scale;}
        const CRCore::crVector3& getIncrementScale() const { return m_incrementScale;}

        void setCurrentScale(const CRCore::crVector3& scale) { m_currentScale = scale;}
        inline const CRCore::crVector3& getCurrentScale() const { return m_currentScale;}

        void updateCurrentScale(const CRCore::crVector3& scale);


        void setPutMatrix(const CRCore::crMatrix& put) { m_Put = put;}
        inline const CRCore::crMatrix& getPutMatrix() const {return m_Put;}

        void setInversePutMatrix(const CRCore::crMatrix& inversePut) { m_inversePut = inversePut;}
        inline const CRCore::crMatrix& getInversePutMatrix() const {return m_inversePut;}

        void setLimitationFlags(unsigned short flags) { m_limitationFlags = flags;}
        inline unsigned short getLimitationFlags() const {return m_limitationFlags;}

        enum MultOrder
        {
            PRH,
            PHR,
            HPR,
            HRP,
            RPH,
            RHP
        };

        void setHPRMultOrder(const MultOrder order) {m_multOrder = order;}
        inline const MultOrder getHPRMultOrder() const { return m_multOrder;}

        inline void setAnimationOn(bool do_animate) {m_animationOn = do_animate;}
        inline bool getAnimationOn() const {return m_animationOn;}

		//void setEndStop(bool endStop);
		//bool getEndStop() const;
		void setEndStopHPR(const crVector3b& endStop);
		const CRCore::crVector3b& getEndStopHPR() const;
		void setEndStopTrans(const crVector3b& endStop);
		const CRCore::crVector3b& getEndStopTrans() const;
		void setEndStopScale(const crVector3b& endStop);
		const CRCore::crVector3b& getEndStopScale() const;

		inline void setCurrentTime(double t){m_currentTime = t;}
		double getCurrentTime() const;

		inline void reset() { setCurrentTime(0); }

		void setResetTime(double resetTime);
		double getResetTime() const;

		void setLifeTime(double lifeTime);
		double getLifeTime() const;

		void setStartTime(double startTime);
		double getStartTime() const;

		void setEndless(bool endless);
		bool getEndless() const;

		//void animate(CRCore::crNodeVisitor& nv);
        void animate(float deltaTime);

        virtual bool computeLocalToWorldMatrix(CRCore::crMatrix& matrix,CRCore::crNodeVisitor* nv) const;

        virtual bool computeWorldToLocalMatrix(CRCore::crMatrix& matrix,CRCore::crNodeVisitor* nv) const;

		void setSurgeFlags(unsigned short flags);
        inline unsigned short getSurgeFlags() const {return m_surgeFlags;}

		virtual void addParam(int i, const std::string &str);

		void setOrigin(const CRCore::crVector3& origin) { m_O = origin;}
		const CRCore::crVector3& getOrigin() const { return m_O;}
		void setXAxis(const CRCore::crVector3& x) { m_xAxis = x;}
		const CRCore::crVector3& getXAxis() const { return m_xAxis;}
		void setXYPlane(const CRCore::crVector3& xy) { m_xyPlane = xy;}
		const CRCore::crVector3& getXYPlane() const { return m_xyPlane;}
		void computePutMatrix();
    protected:

        virtual ~crDOFTransform() {}

        int       m_previousTraversalNumber;
        //double    m_previousTime;

        CRCore::crVector3 m_minHPR;
        CRCore::crVector3 m_maxHPR;
        CRCore::crVector3 m_currentHPR;
        CRCore::crVector3 m_incrementHPR;
		crVector3b m_endStopHPR;

        CRCore::crVector3 m_minTranslate;
        CRCore::crVector3 m_maxTranslate;
        CRCore::crVector3 m_currentTranslate;
        CRCore::crVector3 m_incrementTranslate;
		crVector3b m_endStopTrans;

        CRCore::crVector3 m_minScale;
        CRCore::crVector3 m_maxScale;
        CRCore::crVector3 m_currentScale;
        CRCore::crVector3 m_incrementScale;
		crVector3b m_endStopScale;

        CRCore::crMatrix m_Put;
        CRCore::crMatrix m_inversePut;

        unsigned short m_limitationFlags;
        /* bits from left to right
        0 = x translation limited (2^31)
        1 = y translation limited (2^30)
        2 = z translation limited (2^29)
        3 = pitch limited (2^28)
        4 = roll limited (2^27)
        5 = yaw limited (2^26)
        6 = x scale limited (2^25)
        7 = y scale limited (2^24)
        8 = z scale limited (2^23)

        else reserved
        */

        bool m_animationOn;
        /** flags indicating whether value is incerasing or decreasing in animation
        bits form right to left, 1 means increasing while 0 is decreasing
        0 = x translation
        1 = y translation
        2 = z translation
        3 = pitch
        4 = roll
        5 = yaw
        6 = x scale
        7 = y scale
        8 = z scale
        */
        unsigned short m_increasingFlags;

        MultOrder m_multOrder;

		unsigned short m_surgeFlags;
		/** flags indicating whether value is incerasing or decreasing in animation
		bits form right to left, 1 means surge while 0 is nosurge
		0 = x translation surge
		1 = y translation surge
		2 = z translation surge
		3 = pitch surge
		4 = roll surge
		5 = yaw surge
		6 = x scale surge
		7 = y scale surge
		8 = z scale surge
		*/
		//bool m_endStop;
		//crVector3b m_endStop;//0:HPR, 1:Translate, 2:Scale

		bool m_endless;
		double m_lifeTime;
		double m_startTime;
		double m_currentTime;
		double m_resetTime;

		crVector3 m_O,m_xAxis,m_xyPlane;
		float m_dt;
    };

}

#endif
