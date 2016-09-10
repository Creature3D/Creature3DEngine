//Creature3D
//Modified by Wucaihua
#ifndef _CCOREBODY_H
#define _CCOREBODY_H

#include "rbody/global.h" // If global isn't included before gl.h errors will appear under windows
#include <string>
#include <map>
#include <vector>
#include <list>
#include <exception>
#include <set>

//#include <GL/gl.h>
//#include <GL/glu.h>
//#include <GL/glu.h>
//#include <GL/glut.h>

#include <cal3d/cal3d.h>

#include "rbody/AnimationBase.h"
#include "rbody/MaterialSet.h"
#include "rbody/Mesh.h"
#include "rbody/Cal3dBase.h"
#include "rbody/InBetweenActivity.h"
#include "rbody/Animation.h"
#include "rbody/AnimationDegree.h"
#include "rbody/AnimationSpeed.h"
#include "rbody/AnimationScript.h"
#include "rbody/AnimationBlend.h"
#include "rbody/ActionConflicts.h"
#include "rbody/config_error.h"
//#include "rbody/TgaReader.h"

#include <rvrutils/Property.h>

#include <rbody/ActionRequest.h>

#include <CRCore/crTexture2D.h>
#include <CRCore/crVector3.h>

namespace rbody {

/// Contains all named core bodies
class REPLICANTBODY_API CoreBody {

    public:

        /// Constructor
        CoreBody();

        /// Destructor
        virtual ~CoreBody();

        /// Set name of core body
        /**
         * \param name is name of core body 
         **/
        void setName(const std::string &name);

        /// Get name of core body
        std::string getName();

        /// Create skeleton
        /**
         * \param filename is name of file with cal3d skeleton data 
         **/
        void createSkeleton(const std::string &filename);

        void addActionPrototype(ActionRequest *action);

        /// Add animation of any type
        /**
         * \param animation is instance of type Animation, AnimationBlend, 
         * AnimationScript or AnimationDegree
         **/
        void addAnimation(AnimationBase *animation);
		void deleteAnimation(AnimationBase *animation);

        /// Add mesh
        /**
         * \param mesh is the mesh to add
         **/
        void addMesh(Mesh *mesh);
		void deleteMesh(const std::string &meshName);
		void deleteMesh(int mesh_id);

		Mesh * findMesh(const std::string &meshName);
		std::string getCurrentPartMeshName(int partid);
        /// Add material
        /**
         * \param material it the material to add
         **/
        void addMaterial(Cal3dBase material);

        /// Add material set
        /**
         * \param material_set is material set to add
         **/
        int addMaterialSet(MaterialSet *material_set);

        /// Add in-between activity (used to transition from one activity to another)
        /**
         * \param in_between_activity is class with start, end and in-between activity names
         **/
        void addInBetweenActivity(InBetweenActivity *in_between_activity);

        /// Get action conflicts object
        ActionConflicts* getActionConflicts(); 

        /// Get named animation of any type
        /**
         * \param animation_name is name of animation
         **/
        AnimationBase *getAnimation(const std::string &animation_name);

        /// Get animations connected to given activity/action
        /**
         * \param act_name is name of activity or action
         **/
        void getAnimationsWithActName(const std::string &act_name, std::list<AnimationBase *> &act_anim_list);

        /// Return true if there exists animations connected to given activity/action
        /**
         * \param act_name is name of activity or action
         **/
        bool actNameExists(const std::string &act_name);

        /// Get number of available material sets
        //int getNrOfMaterialSets();

        /// Get cal3d id of given mesh name
        /**
         * Return -1 if no mesh with given name is found
         * \param mesh_name is name of mesh
         **/
        int getCal3dMeshId(const std::string &mesh_name);
		std::string getMeshName(int mesh_id);
		std::string getMeshMaterialFileName(int mesh_id);

        /// Get cal3d id of all meshes that should be attached as default
        void getDefaultMeshIds(std::list<int> &mesh_id_list);

        /// Get cal3d if of default material set
        int getDefaultMaterialSet();

        /// Get activity that should be used as a transition activity between two activities
        /**
         * Returns empty string if no in-between activity is defined
         * \param start_activity is name of first activity
         * \param end activity is name of second activity
         **/
        std::string getInBetweenActivity(const std::string &start_activity, const std::string &end_activity);

        /// Set path (where to find animations, meshes, materials)
        /**
         * \param path is directory path
         **/
        void setPath(const std::string& path);

        /// Get path
        std::string getPath();

        /// Set scale of body (default is 1)
        /**
         * \param scale is new scale to set
         **/
        void setScale(float scale);

        /// Get scale of body
        float getScale();

		void setRotAngle(float angle);
		float getRotAngle();

        /// Set foot offset, offset between foot ground level and foot bone in skeleton
        /**
         * \param offset is foot offset
         **/
        void setTransOffset(const CRCore::crVector3& offset);

        /// Get scale of body
		const CRCore::crVector3& getTransOffset();

        /// Get reference to cal3d core model
        CalCoreModel *getCal3dCoreModel();

        /// Get default activity
        std::string getDefaultActivity() { return m_default_activity; };

        /// Set default activity
        void setDefaultActivity(const std::string &activity_name) { m_default_activity = activity_name; };

        /// Add bone that, when touching the ground, affects speed of character - returns true if bone name exist
        bool addContactBone(const std::string &bone_name);

        /// Get list with names of contact bones
        std::list<int> getContactBoneList();

        /**
         * Get proptotype (with properties) for given action/activity
         * Since this is something used by the AI, which doesn't know about the difference 
         * between action and activities, the name action is used
         **/ 
        ActionRequest* getActionPrototype(const std::string& name);

        /// Build map of action/activity prototypes
        void buildActionPrototypes();

        /// Get name of head bone
        std::string getHeadBoneName() { return m_head_bone; }

        /// Set name of head bone
        void setHeadBoneName(const std::string& name) { m_head_bone = name; }

		/// Get name of Weapon bone
		const std::string &getWeaponBoneName() { return m_weapon_bone; }
		/// Set name of Weapon bone
		void setWeaponBoneName(const std::string& name) { m_weapon_bone = name; }

	    void getCoreActNameSet(std::set<std::string> &actNameSet);
		typedef std::map<std::string, CRCore::ref_ptr<ActionRequest>, std::less<std::string> >	ActionPrototypeMap;
		void getDefaultActNameSet(std::set<std::string> &actNameSet);
		const std::string &getSkeletonFileName();
		void getMeshFileNames(std::set<std::string> &fileNameSet);
		void getAnimationFileNames(std::set<std::string> &fileNameSet);
    private:

        /// Name of core body
        std::string m_name;

        /// List with all animations
        std::list<AnimationBase *> m_animation_list;

        /// Cal3d core model reference
        CalCoreModel *m_cal3d_core_model;	

        /// List with material sets
        std::list<MaterialSet *> m_material_set_list;

        /// List with meshes
        std::list<Mesh *> m_mesh_list;

        /// List with transition (in-between) activities
        std::list<InBetweenActivity *> m_in_between_activity_list;

        /// Conflicting actions
        ActionConflicts *m_action_conflicts;

        /// Path (where to find animations, meshes, materials)
        std::string m_path;

        /// Activity to play at startup
        std::string m_default_activity;

        /// List with names of contact bones, that is, bones affecting speed
        std::list<int> m_contact_bone_list;

        /// Scale of core body
        float m_scale;
        float m_rot_angle;
        /// Offset between foot ground level and foot bone in skeleton
		CRCore::crVector3f m_trans_offset;

        /// Name of head bone (making it possible to get look direction from that bone)
        std::string m_head_bone;

		//Name of weapon bone;
		std::string m_weapon_bone;
        /**
         * Map with action/activity prototypes 
         * Since this is something used by the AI, which doesn't know about the difference 
         * between action and activities, the name action is used
         **/

        ActionPrototypeMap m_action_prototypes;
		std::string m_skeletonFileName;
};

} // namespace end

#endif // _CCOREBODY_H
/*------------------------------------------

 * $Source: /cvsroot/replicantbody/replicantbody/include/rbody/CoreBody.h,v $
 * $Revision: 1.2 $ 
 * $Date: 2005/02/09 17:26:54 $
 * $Author: deepone $ 
 * $Locker:  $

 * Description: 

 Copyright (C) Peter Sunna, VRlab, Ume?University 2001

Created: 2001-12-12

 * $Log: CoreBody.h,v $
 * Revision 1.2  2005/02/09 17:26:54  deepone
 * vrutils and cfgscript put into new namespaces
 *
 * Revision 1.1.1.1  2005/02/06 06:53:00  deepone
 * initial sourceforge checkin
 *
 * Revision 1.11  2004/02/03 12:58:02  deepone
 * new Property.h, Body.h and Notify.h in rvrutils
 *
 * Revision 1.10  2003/10/31 13:21:46  andersb
 * Added ReplicantBodyMgr class to handle instances of CreBody
 * Added .char files for specifying individual characters (CreBody)
 * Added OSG plugin for reading char files
 *
 * Revision 1.9  2003/09/03 07:19:54  sunna
 * *** empty log message ***
 *
 * Revision 1.8  2003/06/05 12:48:46  andersb
 * *** empty log message ***
 *
 * Revision 1.7  2003/06/04 12:41:45  sunna
 * *** empty log message ***
 *
 * Revision 1.6  2003/05/20 12:20:13  deepone
 * public addActionPrototype to be able to add more actions later
 *
 * Revision 1.5  2002/11/22 14:44:20  sunna
 * no message
 *
 * Revision 1.4  2002/11/01 14:59:22  sunna
 * no message
 *
 * Revision 1.3  2002/11/01 13:29:29  sunna
 * no message
 *
 * Revision 1.2  2002/11/01 09:26:28  sunna
 * no message
 *
 * Revision 1.1  2002/06/20 12:51:50  sunna
 * Restructured directories
 *
 * Revision 1.9  2002/05/08 15:49:07  sunna
 * Scaling of models - read scaling factor from config file
 *
 * Revision 1.8  2002/04/18 10:07:26  sunna
 * Added actNameExists
 *
 * Revision 1.7  2002/04/12 13:36:38  sunna
 * Implementation of "contact bones", not yet tested
 *
 * Revision 1.6  2002/04/05 17:48:41  deepone
 * include cal3d files as
 * #include <cal3d/cal3d.h>
 * instead of
 * #include "cal3d.h"
 *
 * Revision 1.5  2002/04/04 14:34:19  sunna
 * Set default activity in constructor
 * Material sets methods added
 *
 * Revision 1.4  2002/03/27 10:21:25  sunna
 * InBetweenActivity is now handling transition activities
 *
* Revision 1.3  2002/03/26 17:37:27  sunna
* Environment variable REPLICANTBODYFILEPATH is searched for file paths => new methods for working with files
* Core bodies are now named after the name of their config file
*
* Revision 1.2  2002/03/15 08:55:07  andersb
* no message
*
* Revision 1.1.1.1  2002/03/14 20:48:40  andersb
* First import as ReplicantBody
*
* Revision 1.1.1.1  2002/01/04 08:22:35  andersb
* Initial version
*
----------------------------------------------------------------------------*/


