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
#ifndef CRCORE_POINT
#define CRCORE_POINT 1

#include <CRCore/crVector3.h>
#include <CRCore/crStateAttribute.h>

namespace CRCore {

/** crPoint - encapsulates the OpenGL point smoothing and size state.*/
class CR_EXPORT crPoint : public crStateAttribute
{
    public :

        crPoint();

        /** Copy constructor using crCopyOp to manage deep vs shallow copy.*/
        crPoint(const crPoint& point,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY):
            crStateAttribute(point,copyop),
            m_size(point.m_size),
            m_fadeThresholdSize(point.m_fadeThresholdSize),
            m_distanceAttenuation(point.m_distanceAttenuation),
            m_minSize(point.m_minSize),
            m_maxSize(point.m_maxSize) {}

        META_StateAttribute(CRCore, crPoint, POINT);
        
        /** return -1 if *this < *rhs, 0 if *this==*rhs, 1 if *this>*rhs.*/
        virtual int compare(const crStateAttribute& sa) const
        {
            // check the types are equal and then create the rhs variable
            // used by the COMPARE_StateAttribute_Paramter macro's below.
            COMPARE_StateAttribute_Types(crPoint,sa)

            // compare each paramter in turn against the rhs.
            COMPARE_StateAttribute_Parameter(m_size)
            COMPARE_StateAttribute_Parameter(m_fadeThresholdSize)
            COMPARE_StateAttribute_Parameter(m_distanceAttenuation)
            COMPARE_StateAttribute_Parameter(m_minSize)
            COMPARE_StateAttribute_Parameter(m_maxSize)

            return 0; // passed all the above comparison macro's, must be equal.
        }

		virtual int compare(const crStateAttribute& sa)
		{
			// check the types are equal and then create the rhs variable
			// used by the COMPARE_StateAttribute_Paramter macro's below.
			COMPARE_StateAttribute_Types(crPoint,sa)

			// compare each paramter in turn against the rhs.
			COMPARE_StateAttribute_Parameter(m_size)
			COMPARE_StateAttribute_Parameter(m_fadeThresholdSize)
			COMPARE_StateAttribute_Parameter(m_distanceAttenuation)
			COMPARE_StateAttribute_Parameter(m_minSize)
			COMPARE_StateAttribute_Parameter(m_maxSize)

			return 0; // passed all the above comparison macro's, must be equal.
		}
 /*       virtual bool getModeUsage(ModeUsage& usage) const
        {
            usage.usesMode(GL_POINT_SMOOTH);
            return true;
        }*/

		virtual void getAssociatedModes(std::vector<VRMode>& modes) const 
		{
			// Have to think about the role of m_colorMode
			// in setting the colormaterial... also need to take the
			// color material enable/disable out of the the apply()...
			modes.push_back(GL_POINT_SMOOTH);
		}

        void setSize(float size);
        inline float getSize() const { return m_size; }

        void setFadeThresholdSize(float fadeThresholdSize);
        inline float getFadeThresholdSize() const { return m_fadeThresholdSize; }

        void setDistanceAttenuation(const crVector3& distanceAttenuation);
        inline const crVector3& getDistanceAttenuation() const { return m_distanceAttenuation; }

        void setMinSize(float minSize);
        inline float getMinSize() const {return m_minSize;}

        void setMaxSize(float maxSize);
        inline float getMaxSize() const {return m_maxSize;}

        virtual void apply(crState& state) const;

    protected :

        virtual ~crPoint();

        float       m_size;
        float       m_fadeThresholdSize;
        crVector3   m_distanceAttenuation;
        float       m_minSize;
        float       m_maxSize;

};

}

#endif
