#ifndef _CNAMEDCOREBODIES_H
#define _CNAMEDCOREBODIES_H

//#include "rbody/global.h"
#include "rbody/export.h"
#include <string>
#include <map>
#include <vector>
#include <stdexcept>
#include <exception>

#include "rbody/CoreBody.h"
#include "rbody/config_error.h"
#include <rvrutils/os/FilePathContainer.h>
#include <CRCore/Referenced.h>
#include <rcfg/ConfigScriptOO.h>


//#define REPLICANTBODY_ENVIRONMENT_VARIABLE "REPLICANTBODY_FILE_PATH"

namespace rbody {

/// Contains all named core bodies
class REPLICANTBODY_API NamedCoreBodies : public CRCore::Referenced {

    public:

        /// Constructor
        NamedCoreBodies();

        /// Destructor
        ~NamedCoreBodies();

        /// Set header of config files (default is # ReplicantBody v0.1)
        /**
         * \param header is header of config files
         **/
        void setConfigFileHeader(const std::string& header);

        /// Return extension of config files
        std::string getConfigFileHeader();

        /// Load given named core body
        /**
         * \param core_body_name is name of core body to load
         **/
        void loadCoreBody(const std::string& core_body_name,const std::string& fileName);

        /// Get reference to given named core body
        /**
         * If core body isn't available it will try load from config file
         * \param core_body_name is name of core body to get
         **/
        CoreBody *getCoreBody(const std::string& core_body_name);

        /// Create an core body with skeleton and given name and return reference to it
        /**
         * \param core_body_name is name of core body to create
         * \param path is directory path to skeleton
         * \param skeleton_filename is name of file with cal3d skeleton data
         **/
        CoreBody *createCoreBody(const std::string& core_body_name, const std::string& path, const std::string& skeleton_filename);

        /// Get number of core bodies
        int getNrOfCoreBodies();

        /// Get reference to core body at given position
        /**
         * Throws eception out of range if nr isn't in range 0 <= nr < size
         * \param nr is postion of core body in list
         **/
        CoreBody *getCoreBodyAt(int nr);

        /// Get path container
        rvrutils::FilePathContainer* getPathContainer() { return m_path_container; };

        /// Remove instacne of core body -> decrease number of instances of given core body
        /**
         * \param core_body_name is name of core body
         **/
        void removeCoreBodyInstance(const std::string& core_body_name);

		static NamedCoreBodies *getInstance();
		virtual void clear();		
    protected :
	    static CRCore::ref_ptr<NamedCoreBodies> m_instance;

        /// Load shared attributes for all animation types
        /**
         * \param anim is animation to set attributes for
         **/
        void loadAttributesCommonToAllAnimations(AnimationBase *anim);

        /// Load core animations
        /**
         * \param new_core_body is core body to load animations to
         **/
        void loadAnimations(CoreBody *new_core_body);

        /// Load speed animations
        /**
         * \param new_core_body is core body to load animations to
         **/
        void loadSpeedAnimations(CoreBody *new_core_body);

        /// Load degree animations
        /**
         * \param new_core_body is core body to load animations to
         **/
        void loadDegreeAnimations(CoreBody *new_core_body);

        /// Load blend animations
        /**
         * \param new_core_body is core body to load animations to
         **/
        void loadBlendAnimations(CoreBody *new_core_body);

        /// Load script animations
        /**
         * \param new_core_body is core body to load animations to
         **/
        void loadScriptAnimations(CoreBody *new_core_body);

        /// Load meshes
        /**
         * \param new_core_body is core body to load animations to
         **/
        void loadMeshes(CoreBody *new_core_body,const std::string& fileName);

        /// Load materials
        /**
         * \param new_core_body is core body to load animations to
         **/
        void loadMaterials(rbody::CoreBody *new_core_body);

        /// Load material sets
        /**
         * \param new_core_body is core body to load animations to
         **/
        void loadMaterialSets(CoreBody *new_core_body);

        /// Load conflicting actions
        /**
         * \param new_core_body is core body to load conflicting actions to
         **/
        void loadActionConflicts(CoreBody *new_core_body);

        /// Load in-between activities
        /**
         * \param new_core_body is core body to load in-between activities to
         **/
        void loadInBetweenActivities(CoreBody *new_core_body);

        /// Return reference to the given core body name
        /**
         * \param core_body_name is name of core body to get
         **/
        CoreBody *getCoreBodyFromMap(const std::string& core_body_name);

        /// Increase number of instances of given core body
        /**
         * \param core_body_name is name of core body
         **/
        void incNrOfCoreBodyInstances(const std::string& core_body_name);

        /// Remove given core body
        /**
         * \param core_body_name is name of core body to delete
         **/
        void removeCoreBody(const std::string& core_body_name);

    private:

        /// List with paths to config files
        std::list<std::string> m_config_file_path_list;

        /// Config file header
        std::string m_config_file_header;

        /// Map which connects core body name with core body reference
        std::map<std::string, CoreBody *> m_core_body_map;

        /// Map which connects core body name with number of core body instances
        std::map<std::string, int> m_core_body_instances_map;

        /// Path delimitor in environment variable
        std::string m_path_delimitor;

        /// Config script parser
        rcfg::ConfigScript m_cfg_script;

        /// File path handler
        rvrutils::FilePathContainer *m_path_container;

};

} // namespace end

#endif // _CNAMEDCOREBODIES_H
/*------------------------------------------

 * $Source: /cvsroot/replicantbody/replicantbody/include/rbody/NamedCoreBodies.h,v $
 * $Revision: 1.2 $ 
 * $Date: 2005/02/09 17:26:54 $
 * $Author: deepone $ 
 * $Locker:  $

 * Description: 

 Copyright (C) Peter Sunna, VRlab, Ume?University 2001

Created: 2001-12-12

 * $Log: NamedCoreBodies.h,v $
 * Revision 1.2  2005/02/09 17:26:54  deepone
 * vrutils and cfgscript put into new namespaces
 *
 * Revision 1.1.1.1  2005/02/06 06:53:00  deepone
 * initial sourceforge checkin
 *
 * Revision 1.7  2004/02/03 12:58:02  deepone
 * new Property.h, Body.h and Notify.h in rvrutils
 *
 * Revision 1.6  2003/10/31 13:21:46  andersb
 * Added ReplicantBodyMgr class to handle instances of CreBody
 * Added .char files for specifying individual characters (CreBody)
 * Added OSG plugin for reading char files
 *
 * Revision 1.5  2002/11/13 15:35:06  sunna
 * no message
 *
 * Revision 1.4  2002/11/01 09:26:28  sunna
 * no message
 *
 * Revision 1.3  2002/10/23 07:52:51  sunna
 * no message
 *
 * Revision 1.2  2002/06/27 14:30:42  sunna
 * Using new file path container class
 *
 * Revision 1.1  2002/06/20 12:51:50  sunna
 * Restructured directories
 *
 * Revision 1.8  2002/05/27 09:14:39  sunna
 * Keeps track of number of instances using each core body - making it possible to free memory of not used core bodies
 *
 * Revision 1.7  2002/03/27 16:58:55  sunna
 * Possible to load confliting actions and in-between activities from config file
 * In-between activities implemented but not tested
 *
 * Revision 1.6  2002/03/27 10:21:25  sunna
 * InBetweenActivity is now handling transition activities
 *
 * Revision 1.5  2002/03/26 17:37:27  sunna
 * Environment variable REPLICANTBODYFILEPATH is searched for file paths => new methods for working with files
 * Core bodies are now named after the name of their config file
 *
 * Revision 1.4  2002/03/21 15:04:46  andersb
 * Modified some datafiles to reflect
 *
 * Revision 1.3  2002/03/21 09:25:41  sunna
 * Added get and set config file header
 *
 * Revision 1.2  2002/03/15 08:55:07  andersb
 * no message
 *
 * Revision 1.1.1.1  2002/03/14 20:48:40  andersb
 * First import as ReplicantBody
 *
 * Revision 1.3  2002/03/13 12:47:41  andersb
 * Now using the ObjectOriented version of ConfigScript.
 *
 * Revision 1.2  2002/03/01 10:37:27  andersb
 * no message
 *
 * Revision 1.1.1.1  2002/01/04 08:22:35  andersb
 * Initial version
*
----------------------------------------------------------------------------*/


