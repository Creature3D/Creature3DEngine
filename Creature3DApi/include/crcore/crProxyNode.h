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
#ifndef CORE_CRPROXYNODE
#define CORE_CRPROXYNODE 1

#include <CRCore/crGroup.h>

namespace CRCore {

/** crProxyNode.
*/
class CR_EXPORT crProxyNode : public crGroup
{
    public :
    
        crProxyNode();

        /** Copy constructor using crCopyOp to manage deep vs shallow copy.*/
        crProxyNode(const crProxyNode&,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY);

        META_Node(CRCore, crProxyNode);
        
        virtual void traverse(crNodeVisitor& nv);
       
        virtual bool addChild(crNode *child);
        virtual bool addChild(crNode *child, const std::string& filename);
        virtual bool removeChild(crNode *child);

        /** Set the database path to prepend to children's filenames.*/
        void setDatabasePath(const std::string& path);
        /** Get the database path used to prepend to children's filenames.*/
        inline const std::string& getDatabasePath() const { return m_databasePath; }

        typedef std::vector<std::string> FileNameList;
        
        void setFileName(unsigned int childNo, const std::string& filename) { expandFileNameListTo(childNo); m_filenameList[childNo]=filename; }
        const std::string& getFileName(unsigned int childNo) const { return m_filenameList[childNo]; }
        unsigned int getNumFileNames() const { return m_filenameList.size(); }

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

        /** Sets the object-space point which defines the center of the CRCore::crProxyNode.  
            center is affected by any transforms in the hierarchy above the CRCore::crProxyNode.*/
        inline void setCenter(const crVector3f& center) { m_centerMode=USER_DEFINED_CENTER; m_userDefinedCenter = center; }
        
        /** return the crProxyNode center point. */
        inline const crVector3f& getCenter() const { if (m_centerMode==USER_DEFINED_CENTER) return m_userDefinedCenter; else return getBound().center(); }


        /** Set the object-space reference radius of the volume enclosed by the crProxyNode. 
          * Used to detmine the bounding sphere of the crProxyNode in the absense of any children.*/
        inline void setRadius(float radius) { m_radius = radius; }
        
        /** Get the object-space radius of the volume enclosed by the crProxyNode.*/
        inline float getRadius() const { return m_radius; }

        //virtual crBoundingSphere computeBound() const;

    protected :
    
        virtual ~crProxyNode() {}
		virtual bool computeBound() const;

        void expandFileNameListTo(unsigned int pos);

        FileNameList        m_filenameList;
        std::string         m_databasePath;
        
        CenterMode          m_centerMode;
        crVector3f                m_userDefinedCenter;
        float               m_radius;
        
};

class CR_EXPORT crExternNode : public crGroup
{
public:
	crExternNode(){}

	crExternNode(const crExternNode&node,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY):
	crGroup(node,copyop),
		m_filename(node.m_filename){}

	META_Node(CRCore, crExternNode)

	inline void setfilename(const std::string &fileName){ m_filename = fileName; }
	inline const std::string &getfilename()const{ return m_filename; }

protected:
	~crExternNode(){}
	std::string m_filename;
};

}

#endif
