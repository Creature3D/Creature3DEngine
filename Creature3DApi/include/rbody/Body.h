//  $Id: Body.h,v 1.2 2005/02/09 17:26:54 deepone Exp $
//Creature3D
//Modified by Îâ²Æ»ª
#ifndef _RBODY_BODY_H
#define _RBODY_BODY_H
#include <cal3d/cal3d.h>
#include <set>
#include <list>
//#include "rbody/global.h" // If global isn't included before gl.h errors will appear under windows
#include <string>
//#include <vector>
//#include <fstream>

//#include <GL/gl.h>
//#include <GL/glu.h>
////#include <GL/glut.h>
//

#include "rbody/CoreBody.h"
#include "rbody/ActAttributes.h"
#include "rbody/AnimationActivityHandler.h"
#include "rbody/AnimationActionHandler.h"
#include "CRCore/crVector3.h"
//
#include "rbody/config_error.h"

#include <CRCore/ref_ptr.h>

#include "rbody/export.h"

#include <rbody/ActionRequest.h>
#include "rbody/NamedCoreBodies.h"
//
////#include "EulerAngles.h"
////#include "QuatTypes.h"
//
//// Print debug
////#define DEBUG_CERR 1

namespace rbody {


/// Body class
class REPLICANTBODY_API Body :  public rvrutils::PropertyContainer {
    public:

        /// Specifies the mode of rendering of the body
        enum RenderingMode {
            RENDER_NONE=0,        /// No rendering at all
            RENDER_BOUNDINGBOX=1, /// Render the boundingbox
            RENDER_SKELETON=2,    /// Render the skeleton
            RENDER_MESH=4,         /// Render the mesh
            RENDER_TEXTURES=8,     //  Render the textures
            RENDER_DEFAULT = RENDER_MESH|RENDER_TEXTURES
        };

    public:
		typedef std::set<int> MeshIDSet;//meshid
        const MeshIDSet& getBodyMeshSet() const;
        /// Constructor
        Body();

        /// Constructor - create body with given core name which can be found in static core body container
        /**
         * Throws exception if core body cannot be found
         * \param core_body_name is name of core body which can be found in named_core_bodies
         **/
        Body(const std::string& core_body_name,const std::string& fileName,const Body::MeshIDSet& meshIDSet);

        /// Destructor
        virtual ~Body();

        /// Create body with given core name which can be found in static core body container
        /**
         * Throws exception if core body cannot be found
         * \param core_body_name is name of core body which can be found in named_core_bodies
         **/
        void create(const std::string& core_body_name,const std::string& fileName,const Body::MeshIDSet& meshIDSet);

        /// Stop action
        /**
         * \param action is action to stop
         **/
        virtual RequestResult stopAction(const std::string& action);

        /// Get conflicting actions
        /**
         * \param action is action to get conflicting actions for
         * \param action_list is referens list where conflicting actions are placed
         **/
        void getConflictingActions(const std::string& action, std::list<std::string> &action_list);

        /// Get state of action - returns UNKNOWN if not known
        /**
         * \param action is action to get state for
         **/
        RequestResult getActionState(const std::string& action);

        /// Attach mesh to body
        /**
         * \param mesh_name is name of mesh
         **/
		bool attachMesh(const std::string& mesh_name);
        void attachMesh(int mesh_id);

        /// Detach mesh from body
        /**
         * \param mesh_name is name of mesh
         **/
        void detachMesh(const std::string& mesh_name);
		void detachMesh(int mesh_id);

		void addMesh(Mesh *mesh/*,int materialsetid*/);
        void deleteMesh(const std::string &meshName);
		void deleteMesh(int mesh_id);
        /// Get axis aligned bounding box
        /**
         * \param min is vector with smallest x,y and z values for body
         * \param max is vector with largest x,y and z values for body
         **/
        void getBoundingBox(CRCore::crVector3 &min, CRCore::crVector3 &max) const;

        /// Set level of detail on polygons
        /**
         * \param lod_poly is float between 0 and 1
         **/
        void setLodPoly(float lod_poly);

        /// Get level of detail on polygons
        float getLodPoly() { return m_lod_poly; }

        /// Update the body's animations according to delta time
        /**
         * \param elapsed_seconds is time passed in seconds since last update
         **/
        virtual bool update(float elapsed_seconds);


        /*!
         * Specifies the renderingmode, i.a. how the body will look.
         * \param rendering_mode - compound of RenderingModes that specifies the rendering mode.
         */
        void setRenderingMode(unsigned int rendering_mode) { m_rendering_mode = (RenderingMode)rendering_mode; }

        /*!
         * Specifies the renderingmode, i.a. how the body will look.
         * \return Return the current renderingmode
         */
        RenderingMode getRenderingMode() { return m_rendering_mode; };

        /// Get current material set
        //int getCurrentMaterialSet() { return m_material_set_id; };

        /// Set material set to be used, return true on success
        //bool setMaterialSet(int material_set_id);

        /// Get number of material sets
        //int getNrOfMaterialSets() { return m_core_body->getNrOfMaterialSets(); };

        /// Get id of bones that touch the ground in pose mode, ie, bones with lowest y-values
        std::list<int> getContactBoneIds();

        /// Get translation of bone with given id
        CRCore::crVector3 getBoneTranslation(int bone_id);

        /// Get delta translation (since last call of method) of bone with given id
        float getDeltaContactBoneTranslation(float elapsed);

        /// Enable contact bone with given id for translation calculation
        void enableContactBone(int bone_id);

        /// Enable all contact bones for speed calculation
        void enableAllContactBones();

        /// Disable all contact bones for speed calculation
        void disableAllContactBones();

        /// Get current speed
        float getCurrentSpeed() { return m_activity_handler.getCurrentSpeed(); };

        /*!
         * Specifies the factor to be used when scaling the model (geometries, animations)
         * \param factor is the scale factor
         */
        void setScale(float factor);

        /*!
         * Get scale factor
         */
        float getScale();

        /// Get container of file paths (exists in core body container which is static)
        static rvrutils::FilePathContainer *getFilePathContainer();

        CoreBody* getCoreBody() { return m_core_body; }

        CalModel* getCalModel() { return m_cal3d_model; }

        CalCoreModel *getCalCoreModel() { return m_cal3d_core_model; }

        /// Get map with available action prototypes
        //void getActionPrototypeNames( std::list<std::string>& names_list )
        //{ names_list.push_back( "FIXME - NOT_IMPLEMENTED" ); };

		typedef std::set<std::string> ActNameSet;
		const ActNameSet &getActNameSet() const;
		bool isBodyAct(const std::string &actName);
		void attachAct(const std::string &actName);
		void detachAct(const std::string &actName);

        ActionRequest* getActionPrototype(const std::string& name);

        virtual RequestResult executeAction( ActionRequest *action, bool force = false );

        /// Add action prototype
        //virtual void addActionPrototype( ActionRequest* ) {};

		void deleteAnimation(AnimationBase *animation);
		const std::string &getCoreBodyName();
	protected :
        /// Core body name
        std::string m_core_body_name;

        /// Cal3d core model
        CalCoreModel *m_cal3d_core_model;

        /// Cal3d model
        CalModel *m_cal3d_model;

        /// Core body
        CoreBody *m_core_body;

        /// Activity animations handler
        AnimationActivityHandler m_activity_handler;

        /// Action animations handler
        AnimationActionHandler m_action_handler;
		//std::string m_lastActivityName;//add by wch

        /// Level of detail for polygons
        float m_lod_poly;

        /// Current material set that is being used
        //int m_material_set_id;

        /// Keeps track of delta factor (translation/elapsed time) of active contact bone last frame
        float m_last_delta_factor;

        /// List of enabled contact bones
        std::list<int> m_enabled_contact_bones;

        // Contact bone used when calculating translation
        int m_active_contact_bone;

        /// List with last translation of each contact bone
        std::list<float> m_contact_bone_trans;

        /// Map with contact bones ids and their y-translation 
        std::map<int, float> m_contact_bone_map;

        /// Static core body container - shared between all body instances
        //static CNamedCoreBodies m_static_core_body_container;

        static NamedCoreBodies* getCoreBodyContainer();

        RenderingMode m_rendering_mode;

        /// Scale factor
        float m_scale_factor;
		MeshIDSet m_bodyMeshSet;
		ActNameSet m_bodyActNameSet;
};


} // namespace end

#endif // _RBODY_BODY_H

/*------------------------------------------

 * $Source: /cvsroot/replicantbody/replicantbody/include/rbody/Body.h,v $
 * $Revision: 1.2 $ 
 * $Date: 2005/02/09 17:26:54 $
 * $Author: deepone $ 
 * $Locker:  $

 * Description: 

 Copyright (C) Peter Sunna, VRlab, Ume?University 2001

Created: 2001-12-12


 * $Log: Body.h,v $
 * Revision 1.2  2005/02/09 17:26:54  deepone
 * vrutils and cfgscript put into new namespaces
 *
 * Revision 1.1.1.1  2005/02/06 06:52:59  deepone
 * initial sourceforge checkin
 *
 * Revision 1.19  2004/03/03 16:46:27  deepone
 * *** empty log message ***
 *
 * Revision 1.18  2004/02/03 12:58:02  deepone
 * new Property.h, Body.h and Notify.h in rvrutils
 *
 * Revision 1.17  2003/11/13 10:12:23  andersb
 * *** empty log message ***
 *
 * Revision 1.16  2003/11/12 10:19:25  andersb
 * Removed forward declaration of CreBody
 *
 * Revision 1.15  2003/11/12 10:12:10  andersb
 * Body inherits from PropertyContainer again.
 * Moved Direction property out of Body.
 *

 * Revision 1.14  2003/11/11 10:59:19  sunna
 * *** empty log message ***
 *
 * Revision 1.13  2003/11/11 10:52:06  sunna
 * *** empty log message ***
 *
 * Revision 1.12  2003/10/31 13:21:44  andersb
 * Added ReplicantBodyMgr class to handle instances of CreBody
 * Added .char files for specifying individual characters (CreBody)
 * Added OSG plugin for reading char files
 *
 * Revision 1.11  2003/09/09 07:55:19  sunna
 * Scaling node now working properly.\n Possible to set individual scaling for each body
 *
 * Revision 1.10  2003/09/03 07:19:54  sunna
 * *** empty log message ***
 *
 * Revision 1.9  2003/06/09 14:55:55  sunna
 * *** empty log message ***
 *
 * Revision 1.8  2003/06/04 12:41:45  sunna
 * *** empty log message ***
 *
 * Revision 1.7  2003/05/20 17:12:40  deepone
 * mupp
 *
 * Revision 1.6  2002/11/22 14:44:20  sunna
 * no message
 *
 * Revision 1.5  2002/11/13 15:35:06  sunna
 * no message
 *
 * Revision 1.4  2002/11/05 15:18:03  sunna
 * no message
 *
 * Revision 1.3  2002/11/04 10:32:35  deepone
 * fix
 *
 * Revision 1.2  2002/11/01 13:29:28  sunna
* no message
*
* Revision 1.1  2002/11/01 09:26:28  sunna
* no message
*
* Revision 1.5  2002/10/22 16:29:02  sunna
* no message
*
* Revision 1.4  2002/08/15 13:19:59  sunna
* no message
*
* Revision 1.3  2002/07/05 13:39:35  sunna
* Added writeOsgVortexFile() to write vortex skeleton information to file
*
* Revision 1.2  2002/06/27 14:30:48  sunna
* Using new file path container class
*
* Revision 1.1  2002/06/20 12:51:50  sunna
* Restructured directories
*
    * Revision 1.14  2002/05/28 12:59:34  sunna
* Added enableAllContactBones()
    *
    * Revision 1.13  2002/05/27 09:15:28  sunna
    * Added name of core body
    *
    * Revision 1.12  2002/05/24 15:06:39  sunna
    * Made core body container static, that is, shared between all instances of bodies
    *
    * Revision 1.11  2002/05/23 15:28:26  sunna
    * Added constructors
    *
    * Revision 1.10  2002/05/23 08:16:54  sunna
    * getCurrentSpeed() added
    *
    * Revision 1.9  2002/05/16 15:00:17  sunna
* Added getCurrentSpeed, changed name of getContactBoneTranslation() to getDeltaContactBoneTranslation()
    *
    * Revision 1.8  2002/04/17 12:06:31  sunna
    * Improved contact bone handling
    *
    * Revision 1.7  2002/04/15 16:02:27  sunna
    * Contact bones' handling doesn't work properly, restrictions on change of
    * contact bone must be set, ie, make sure correct contact bone is controlling
    * the speed.
    *
    * Revision 1.6  2002/04/12 13:36:38  sunna
    * Implementation of "contact bones", not yet tested
    *
    * Revision 1.5  2002/04/05 17:48:40  deepone
    * include cal3d files as
    * #include <cal3d/cal3d.h>
    * instead of
    * #include "rbody/cal3d.h"
    *
    * Revision 1.4  2002/04/04 14:34:19  sunna
    * Set default activity in constructor
    * Material sets methods added
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
    * Revision 1.10  2002/03/08 14:07:57  andersb
    * no message
    *
    * Revision 1.9  2002/03/06 15:23:16  sunna
    * Implemented quaternion to rotation matrix conversion
    *
    * Revision 1.8  2002/03/06 10:28:46  sunna
    * Added define DEBUG_CERR to switch on/off debug print
    *
    * Revision 1.7  2002/03/04 13:42:32  andersb
    * no message
    *
    * Revision 1.6  2002/03/01 12:24:54  andersb
    * Removed a };
*
* Revision 1.5  2002/03/01 10:36:35  andersb
* Changed som get* methods to inline style.
*
* Revision 1.4  2002/02/28 10:19:15  andersb
* no message
*
* Revision 1.3  2002/01/25 12:34:34  andersb
* A working Avatar example with OSG.
*
* Revision 1.2  2002/01/08 09:02:33  sunna
* no message
*
* Revision 1.1.1.1  2002/01/04 08:22:35  andersb
* Initial version
*
----------------------------------------------------------------------------*/
