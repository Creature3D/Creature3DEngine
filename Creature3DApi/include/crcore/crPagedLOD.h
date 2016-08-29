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
#ifndef CRCORE_CRPAGEDLOD
#define CRCORE_CRPAGEDLOD 1

#include <CRCore/crLod.h>

namespace CRCore {

/** crPagedLOD.
*/
class CR_EXPORT crPagedLOD : public crLod
{
    public :
    
        crPagedLOD();

        /** Copy constructor using crCopyOp to manage deep vs shallow copy.*/
        crPagedLOD(const crPagedLOD&,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY);

        META_Node(CRCore, crPagedLOD);
        
        

        virtual void traverse(crNodeVisitor& nv);
        
        virtual bool addChild(crNode *child);

        virtual bool addChild(crNode *child, float min, float max);

        virtual bool addChild(crNode *child, float min, float max,const std::string& filename, float priorityOffset=0.0f, float priorityScale=1.0f);

        //virtual bool removeChild(crNode *child);
        virtual bool removeChild(unsigned int pos,unsigned int numChildrenToRemove);

        /** Set the database path to prepend to children's filenames.*/
        void setDatabasePath(const std::string& path);

        /** Get the database path used to prepend to children's filenames.*/
        inline const std::string& getDatabasePath() const { return m_databasePath; }

        
        struct CR_EXPORT PerRangeData
        {
            PerRangeData();
            PerRangeData(const PerRangeData& prd);
            PerRangeData& operator = (const PerRangeData& prd);

            std::string m_filename;
            float       m_priorityOffset;
            float       m_priorityScale;
            double      m_timeStamp;
			int         m_frameNumber;
        };

        typedef std::vector<PerRangeData> PerRangeDataList;
        
        void setFileName(unsigned int childNo, const std::string& filename) { expandPerRangeDataTo(childNo); m_perRangeDataList[childNo].m_filename=filename; }
        const std::string& getFileName(unsigned int childNo) const { return m_perRangeDataList[childNo].m_filename; }
        unsigned int getNumFileNames() const { return m_perRangeDataList.size(); }


        void setPriorityOffset(unsigned int childNo, float priorityOffset) { expandPerRangeDataTo(childNo); m_perRangeDataList[childNo].m_priorityOffset=priorityOffset; }
        float getPriorityOffset(unsigned int childNo) const { return m_perRangeDataList[childNo].m_priorityOffset; }
        unsigned int getNumPriorityOffsets() const { return m_perRangeDataList.size(); }

        void setPriorityScale(unsigned int childNo, float priorityScale) { expandPerRangeDataTo(childNo); m_perRangeDataList[childNo].m_priorityScale=priorityScale; }
        float getPriorityScale(unsigned int childNo) const { return m_perRangeDataList[childNo].m_priorityScale; }
        unsigned int getNumPriorityScales() const { return m_perRangeDataList.size(); }


        void setTimeStamp(unsigned int childNo, double timeStamp) { expandPerRangeDataTo(childNo); m_perRangeDataList[childNo].m_timeStamp=timeStamp; }
        double getTimeStamp(unsigned int childNo) const { return m_perRangeDataList[childNo].m_timeStamp; }
        unsigned int getNumTimeStamps() const { return m_perRangeDataList.size(); }


        /** Set the frame number of the last time that this PageLOD node was traversed.
          * Note, this frame number is automatically set by the traverse() method for all traversals (update, cull etc.). 
          */
        inline void setFrameNumberOfLastTraversal(int frameNumber) { m_frameNumberOfLastTraversal=frameNumber; }

        /** Get the frame number of the last time that this PageLOD node was traversed.*/
        inline int getFrameNumberOfLastTraversal() const { return m_frameNumberOfLastTraversal; }


        /** Set the number of children that the crPagedLOD must keep around, even if they are older than their expiry time.*/
        inline void setNumChildrenThatCannotBeExpired(unsigned int num) { m_numChildrenThatCannotBeExpired = num; }

        /** Get the number of children that the crPagedLOD must keep around, even if they are older than their expiry time.*/
        unsigned int getNumChildrenThatCannotBeExpired() const { return m_numChildrenThatCannotBeExpired; }

        /** Remove the children from the crPagedLOD which haven't been visited since specified expiry time.
          * The removed children are added to the removeChildren list passed into the method,
          * this allows the children to be deleted later at the caller's discretion.
          * Return true if children are removed, false otherwise. */
        virtual bool removeExpiredChildren(double expiryTime,NodeArray& removedChildren,std::map< std::string, CRCore::ref_ptr<CRCore::crNode> > &loadedMap);
        
    protected :
    
        virtual ~crPagedLOD();

        virtual void childRemoved(unsigned int pos, unsigned int numChildrenToRemove);
        virtual void childInserted(unsigned int pos);

        virtual void rangeRemoved(unsigned int pos, unsigned int numChildrenToRemove);
        virtual void rangeInserted(unsigned int pos);
        
        void expandPerRangeDataTo(unsigned int pos);

        std::string         m_databasePath;

        int                 m_frameNumberOfLastTraversal;
        unsigned int        m_numChildrenThatCannotBeExpired;
        PerRangeDataList    m_perRangeDataList;
};

}

#endif
