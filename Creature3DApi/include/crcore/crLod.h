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
#ifndef CRCORE_CRLOD_H
#define CRCORE_CRLOD_H

#include <CRCore/crGroup.h>

namespace CRCore {

class CR_EXPORT crLod : public crGroup
{
    public :
    
        crLod();

        /** Copy constructor using crCopyOp to manage deep vs shallow copy.*/
        crLod(const crLod&,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY);

        META_Node(CRCore, crLod);

        virtual void traverse(crNodeVisitor& nv);
        
        virtual bool addChild(crNode *child);

        virtual bool addChild(crNode *child, float min, float max);

        //virtual bool removeChild(crNode *child);
        virtual bool removeChild(unsigned int pos,unsigned int numChildrenToRemove=1);

        typedef std::pair<float,float>  MinMaxPair;
        typedef std::vector<MinMaxPair> RangeList;
      
        /** Modes which control how the center of object should be determined when computed which child is active.*/
        enum CenterMode
        {
            USE_BOUNDING_SPHERE_CENTER,
            USER_DEFINED_CENTER
        };

        /** Set how the center of object should be determined when computed which child is active.*/
        void setCenterMode(CenterMode mode) { m_centerMode=mode; }

        /** Get how the center of object should be determined when computed which child is active.*/
        CenterMode getCenterMode() const { return m_centerMode; }

        /** Sets the object-space point which defines the center of the CRCore::crLod.  
            center is affected by any transforms in the hierarchy above the CRCore::crLod.*/
        inline void setCenter(const crVector3f& center) { m_centerMode=USER_DEFINED_CENTER; m_userDefinedCenter = center; }
        
        /** return the crLod center point. */
        inline const crVector3f& getCenter() const { if (m_centerMode==USER_DEFINED_CENTER) return m_userDefinedCenter; else return getBound().center(); }


        /** Set the object-space reference radius of the volume enclosed by the crLod. 
          * Used to detmine the bounding sphere of the crLod in the absense of any children.*/
        inline void setRadius(float radius) { m_radius = radius; }
        
        /** Get the object-space radius of the volume enclosed by the crLod.*/
        inline float getRadius() const { return m_radius; }



        /** Modes that control how the range values should be intepreted when computing which child is active.*/
        enum RangeMode
        {
            DISTANCE_FROM_EYE_POINT,
            PIXEL_SIZE_ON_SCREEN
        };
        
        /** Set how the range values should be intepreted when computing which child is active.*/
        void setRangeMode(RangeMode mode) { m_rangeMode = mode; }

        /** Get how the range values should be intepreted when computing which child is active.*/
        RangeMode getRangeMode() const { return m_rangeMode; }


        /** Sets the min and max visible ranges of range of specifiec child.
            Values are floating point distance specified in local objects coordinates.*/
        void setRange(unsigned int childNo, float min,float max);
        
        /** returns the min visible range for specified child.*/
        inline float getMinRange(unsigned int childNo) const { return m_rangeList[childNo].first; }

        /** returns the max visible range for specified child.*/
        inline float getMaxRange(unsigned int childNo) const { return m_rangeList[childNo].second; }
        
        /** returns the number of ranges currently set. 
          * An crLod which has been fully set up will have getNumChildren()==getNumRanges(). */
        inline unsigned int getNumRanges() const { return m_rangeList.size(); }

        /** set the list of MinMax ranges for each child.*/
        inline void setRangeList(const RangeList& rangeList) { m_rangeList=rangeList; }

        /** return the list of MinMax ranges for each child.*/
        inline const RangeList& getRangeList() const { return m_rangeList; }

        //virtual crBoundingSphere computeBound() const;

		virtual void dirtyBound();
    protected :
        virtual ~crLod();

		virtual bool computeBound() const;

        virtual void childRemoved(unsigned int pos, unsigned int numChildrenToRemove);
        virtual void childInserted(unsigned int pos);

        virtual void rangeRemoved(unsigned int /*pos*/, unsigned int /*numChildrenToRemove*/) {}
        virtual void rangeInserted(unsigned int /*pos*/) {}
		
		typedef std::vector<int> ActiveChildVec;
		void swapActiveChildVec();

        CenterMode                      m_centerMode;
        crVector3f                      m_userDefinedCenter;
        float                           m_radius;

        RangeMode                       m_rangeMode;
        RangeList                       m_rangeList;

		crCriticalMutex					m_activeChildVecMutex;
        ActiveChildVec                  *m_thisActiveChildVec;
		ActiveChildVec                  *m_futureActiveChildVec;
		int                             m_frameNumber;
};

}

#endif
