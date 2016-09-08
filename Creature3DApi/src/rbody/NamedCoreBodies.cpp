#include "rbody/NamedCoreBodies.h"
#include <CRCore/crNotify.h>
#include <CRCore/crBrain.h>
#include <CRIOManager/crCookFile.h>
using namespace CRCore;
using namespace rbody;


/*----------------------------------------------------------------------------
  Constructor
  ----------------------------------------------------------------------------*/
ref_ptr<NamedCoreBodies> NamedCoreBodies::m_instance = NULL;

NamedCoreBodies* NamedCoreBodies::getInstance()
{
	if (m_instance==NULL) 
	{
		m_instance = new NamedCoreBodies;
		//crBrain::getInstance()->pushInstance(m_instance.get());
	}
	return m_instance.get();
}
void NamedCoreBodies::clear()
{
	m_instance = NULL;
}

NamedCoreBodies::NamedCoreBodies()
{
    // Set default header to be "# ReplicantBody v0.1"
    m_config_file_header = CreScriptParserHeader;

    // m_path_container = new rvrutils::FilePathContainer("REPLICANTBODY_FILE_PATH");

    m_path_container = new rvrutils::FilePathContainer();

    //if (!m_path_container->setEnvironmentVariable(REPLICANTBODY_ENVIRONMENT_VARIABLE)) {
        //CRCore::notify(CRCore::WARN) << "Warning! Environmentvariable " << REPLICANTBODY_ENVIRONMENT_VARIABLE << " not set." << std::endl;;
    //}
    // Add empty path to make it possible to use absolute and relative paths
    m_path_container->getFilePathList().push_back("");
}


/*----------------------------------------------------------------------------
  Destructor
  ----------------------------------------------------------------------------*/
NamedCoreBodies::~NamedCoreBodies()
{

    CoreBody *tmp_core_body;

    std::map<std::string, CoreBody *>::iterator core_bodies_iter;

    // Free memory for all core bodies in map
    for(core_bodies_iter = m_core_body_map.begin(); core_bodies_iter != m_core_body_map.end(); core_bodies_iter++){

        tmp_core_body = core_bodies_iter->second;

        delete tmp_core_body;

    }

    // Erase all elements in map
    m_core_body_map.clear();

    // Erase all elements in path list
    m_config_file_path_list.clear();

	delete m_path_container;
}

/*----------------------------------------------------------------------------
  Set header of config files
  ----------------------------------------------------------------------------*/
void NamedCoreBodies::setConfigFileHeader(const std::string& header)
{

    m_config_file_header = header;

}


/*----------------------------------------------------------------------------
  Remove instance of core body -> decrease number of instances using given core body
  ----------------------------------------------------------------------------*/
void NamedCoreBodies::removeCoreBodyInstance(const std::string& core_body_name)
{
    std::map<std::string, int>::iterator no_core_inst_iter;

    // Try to find core body name in number of instances map
    no_core_inst_iter = m_core_body_instances_map.find(core_body_name);

    // If core body isn't found, do nothing, return
    if (no_core_inst_iter == m_core_body_instances_map.end())
        return;

    // Decrease number of instances
    no_core_inst_iter->second--;

    // If number of instances is equal to zero, remove core body
    if (no_core_inst_iter->second < 1)
        removeCoreBody(core_body_name);
	//CRCore::notify(CRCore::FATAL)<<" NamedCoreBodies::removeCoreBodyInstance = "<<no_core_inst_iter->second<<std::endl;
}

/*----------------------------------------------------------------------------
  Increase number of instances using given core body
  ----------------------------------------------------------------------------*/
void NamedCoreBodies::incNrOfCoreBodyInstances(const std::string& core_body_name)
{
    std::map<std::string, int>::iterator no_core_inst_iter;

    // Try to find core body name in number of instances map
    no_core_inst_iter = m_core_body_instances_map.find(core_body_name);

    // If core body isn't found, add core body and set number of instacnes to 1
    if (no_core_inst_iter == m_core_body_instances_map.end())
        m_core_body_instances_map.insert(std::make_pair(core_body_name, 1));
    else
        // Increase number of instances of this core body if it already exists
        no_core_inst_iter->second++;

}

/*----------------------------------------------------------------------------
  Return reference to core body if the given core body exists in list
  ----------------------------------------------------------------------------*/
CoreBody *NamedCoreBodies::getCoreBodyFromMap(const std::string& core_body_name)
{

    if (m_core_body_map.empty())
        return NULL;

    std::map<std::string, CoreBody *>::iterator core_bodies_iter;

    // Try to find core body name
    core_bodies_iter = m_core_body_map.find(core_body_name);

    // If core body doesn't exist, return NULL
    if (core_bodies_iter == m_core_body_map.end())
        return NULL;

    // Return pointer to core body
    return core_bodies_iter->second;

}


/*----------------------------------------------------------------------------
  Load shared attributes for all animation types
  ----------------------------------------------------------------------------*/
void NamedCoreBodies::loadAttributesCommonToAllAnimations(AnimationBase *anim)
{

    std::string str;
    float f_value;
    int i_value;

    // Get and set name
    if (!m_cfg_script.Get("name", str))
        throw (config_error("NamedCoreBodies::loadAttributesCommonToAllAnimations(): " + m_cfg_script.GetLastError()));
    anim->setName(str);

    // Get and set act name
    if (!m_cfg_script.Get("act_name", str))
        throw (config_error("NamedCoreBodies::loadAttributesCommonToAllAnimations(): " + m_cfg_script.GetLastError()));
    anim->setActName(str);

    // Get and set act weight. If not given, set it to 1.0
    if (!m_cfg_script.Get("act_weight", f_value)) 
        anim->setWeight(1.0f);
    else
        anim->setWeight(f_value);

    // Get and set is_default. If not given, set to default
    if (!m_cfg_script.Get("is_default", i_value)) 
        anim->setIsDefault(true);
    else 
        anim->setIsDefault(i_value != 0);

}


/*----------------------------------------------------------------------------
  Load core animations
  ----------------------------------------------------------------------------*/
void NamedCoreBodies::loadAnimations(CoreBody *new_core_body)
{

    Animation *anim;
    std::string str;
    float f_value;
    int i_value;
    int i = 1;

    // Load all core animations

    while (m_cfg_script.Push("Animation",i)) {

        // Allocate memory for new animation;
        anim = new Animation();

        // Load common attributes
        loadAttributesCommonToAllAnimations(anim);

        // Get and set filename
        if (!m_cfg_script.Get("filename", str))
		{
			char gbuf[256];
			sprintf(gbuf,"NamedCoreBodies::loadAnimations(): %s\n\0",m_cfg_script.GetLastError().c_str());
			gDebugInfo->debugInfo(CRCore::WARN,gbuf);
			delete anim;
			continue;
           // throw (config_error("NamedCoreBodies::loadAnimations(): " + m_cfg_script.GetLastError()));
		}
        anim->setFilename(str);

        // Get and set speed. If not given, set it to 0.0
        if (!m_cfg_script.Get("speed", f_value)) 
            anim->setSpeed(1.0f);
        else
            anim->setSpeed(f_value);

        // Get and set is_looped
        if (!m_cfg_script.Get("is_looped", i_value))
		{
			char gbuf[256];
			sprintf(gbuf,"NamedCoreBodies::loadAnimations(): %s\n\0",m_cfg_script.GetLastError().c_str());
			gDebugInfo->debugInfo(CRCore::WARN,gbuf);
			delete anim;
			continue;
            //throw (config_error("NamedCoreBodies::loadAnimations(): " + m_cfg_script.GetLastError()));
		}
        anim->setIsLooped(i_value);

        // Add animation to core body
        new_core_body->addAnimation(anim);

        // Update animation nr
        i++;

        // Pop animation struct
        if (!m_cfg_script.Pop())
		{
			char gbuf[256];
			sprintf(gbuf,"NamedCoreBodies::loadAnimations(): %s\n\0",m_cfg_script.GetLastError().c_str());
			gDebugInfo->debugInfo(CRCore::WARN,gbuf);
			break;
            //throw (config_error("NamedCoreBodies::loadAnimations(): " + m_cfg_script.GetLastError())); 
		}
        // Print success message
		//CRCore::notify(CRCore::INFO) << "Core animation \"" << anim->getName() << "\" with act name \"" << anim->getActName() << "\" loaded." << std::endl;

    }

    // Print warning if no animations were loaded
    //if (i == 1)
    //    CRCore::notify(CRCore::INFO) << "NamedCoreBodies::loadAnimations(): Warning: No animations loaded from config file" << std::endl;

}


/*----------------------------------------------------------------------------
  Load speed animations
  ----------------------------------------------------------------------------*/
void NamedCoreBodies::loadSpeedAnimations(CoreBody *new_core_body)
{
    AnimationSpeed *anim;
    std::string str;
    int i = 1;

    while (m_cfg_script.Push("AnimationSpeed",i)) {

        // Allocate memory for new animation;
        anim = new AnimationSpeed();

        // Load common attributes
        loadAttributesCommonToAllAnimations(anim);

        // Read the two animations that the degree animation consist of

        int k = 1;
        while (m_cfg_script.Push("Animation", k) && (k < 3)) {

            // Get name and add animation name to speed animation
            if (!m_cfg_script.Get("name", str))
                throw (config_error("NamedCoreBodies::loadSpeedAnimations(): " + m_cfg_script.GetLastError()));
            anim->addAnimation(str);

            // Update animation nr
            k++;

            // Pop animation struct
            if (!m_cfg_script.Pop())
                throw (config_error("NamedCoreBodies::loadSpeedAnimations(): " + m_cfg_script.GetLastError()));
        }

        // Add animation to core body
        new_core_body->addAnimation(anim);

        // Update animation nr
        i++;

        // Pop animation struct
        if (!m_cfg_script.Pop())
            throw (config_error("NamedCoreBodies::loadSpeedAnimations(): " + m_cfg_script.GetLastError()));

        // Print success message
        //CRCore::notify(CRCore::INFO) << "Speed animation \"" << anim->getName() << "\" with act name \"" << anim->getActName() << "\" loaded." << std::endl;

    }

    // Print warning if no speed animations loaded
    //if (i == 1)
    //    CRCore::notify(CRCore::INFO) << "No speed animations loaded." << std::endl;

}


/*----------------------------------------------------------------------------
  Load degree animations
  ----------------------------------------------------------------------------*/
void NamedCoreBodies::loadDegreeAnimations(CoreBody *new_core_body)
{

    AnimationDegree *anim;
    std::string str;
    float f_value;
    int i = 1;

    while (m_cfg_script.Push("AnimationDegree", i)) {

        // Allocate memory for new animation;
        anim = new AnimationDegree();

        // Load common attributes
        loadAttributesCommonToAllAnimations(anim);

        // Read the two animations that the degree animation consist of

        int k = 1;

        while (m_cfg_script.Push("Animation", k) && (k < 3)) {

            // Get name
            if (!m_cfg_script.Get("name", str))
                throw (config_error("NamedCoreBodies::loadDegreeAnimations(): " + m_cfg_script.GetLastError()));

            // Get degree representation
            if (!m_cfg_script.Get("degree", f_value))
                throw (config_error("NamedCoreBodies::loadDegreeAnimations(): " + m_cfg_script.GetLastError()));

            // Add animation with name and degree
            anim->addAnimation(str,f_value);

            // Update animation struct nr
            k++;

            // Pop animation struct
            if (!m_cfg_script.Pop())
                throw (config_error("NamedCoreBodies::loadDegreeAnimations(): " + m_cfg_script.GetLastError()));
        }

        // Add animation to core body
        new_core_body->addAnimation(anim);

        // Update animation struct nr
        i++;

        // Pop animation struct
        if (!m_cfg_script.Pop())
            throw (config_error("NamedCoreBodies::loadDegreeAnimations(): " + m_cfg_script.GetLastError()));

        // Print success message
        //CRCore::notify(CRCore::INFO) << "Degree animation \"" << anim->getName() << "\" with act name \"" << anim->getActName() << "\" loaded." << std::endl;

    }

    // Print warning if no degree animations loaded
    //if (i == 1)
     //   CRCore::notify(CRCore::INFO) << "No degree animations loaded." << std::endl;

}


/*----------------------------------------------------------------------------
  Load blend animations
  ----------------------------------------------------------------------------*/
void NamedCoreBodies::loadBlendAnimations(CoreBody *new_core_body)
{

    AnimationBlend *anim;
    std::string str;
    float f_value;
    int i = 1;

    while (m_cfg_script.Push("AnimationBlend", i)) {

        // Allocate memory for new animation;
        anim = new AnimationBlend();

        // Load common attributes
        loadAttributesCommonToAllAnimations(anim);

        // Read the animations that the blend animation consist of

        int k = 1;

        while (m_cfg_script.Push("Animation", k)) {

            // Get name
            if (!m_cfg_script.Get("name", str))
			{
				char gbuf[256];
				sprintf(gbuf,"NamedCoreBodies::loadBlendAnimations(): %s\n\0",m_cfg_script.GetLastError().c_str());
				gDebugInfo->debugInfo(CRCore::WARN,gbuf);
				continue;
                //throw (config_error("NamedCoreBodies::loadBlendAnimations(): " + m_cfg_script.GetLastError()));
			}

            // Get weight
            if (!m_cfg_script.Get("weight", f_value))
			{
				char gbuf[256];
				sprintf(gbuf,"NamedCoreBodies::loadBlendAnimations(): %s\n\0",m_cfg_script.GetLastError().c_str());
				gDebugInfo->debugInfo(CRCore::WARN,gbuf);
				continue;
                //throw (config_error("NamedCoreBodies::loadBlendAnimations(): " + m_cfg_script.GetLastError()));
			}

            // Add animation with name and weight
            anim->addAnimation(str,f_value);

            // Update animation struct nr
            k++;

            // Pop animation struct
            if (!m_cfg_script.Pop())
			{
				char gbuf[256];
				sprintf(gbuf,"NamedCoreBodies::loadBlendAnimations(): %s\n\0",m_cfg_script.GetLastError().c_str());
				gDebugInfo->debugInfo(CRCore::WARN,gbuf);
				break;
                //throw (config_error("NamedCoreBodies::loadBlendAnimations(): " + m_cfg_script.GetLastError()));
			}
        }

        // Add animation to core body
        new_core_body->addAnimation(anim);

        // Update animation struct name
        i++;

        // Pop animation struct
        if (!m_cfg_script.Pop())
		{
			char gbuf[256];
			sprintf(gbuf,"NamedCoreBodies::loadBlendAnimations(): %s\n\0",m_cfg_script.GetLastError().c_str());
			gDebugInfo->debugInfo(CRCore::WARN,gbuf);
			break;
            //throw (config_error("NamedCoreBodies::loadBlendAnimations(): " + m_cfg_script.GetLastError()));
		}
        // Print success message
        //CRCore::notify(CRCore::INFO) << "Blend animation \"" << anim->getName() << "\" with act name \"" << anim->getActName() << "\" loaded." << std::endl;

    }

    // Print warning if no blend animations loaded
   // if (i == 1)
     //   CRCore::notify(CRCore::INFO) << "No blend animations loaded." << std::endl;


}


/*----------------------------------------------------------------------------
  Load script animations
  ----------------------------------------------------------------------------*/
void NamedCoreBodies::loadScriptAnimations(CoreBody *new_core_body)
{

    AnimationScript *anim;
    std::string str;
    float f_value;
    int i = 1;

    while (m_cfg_script.Push("AnimationScript", i)) {

        // Allocate memory for new animation
        anim = new AnimationScript();

        // Load common attributes
        loadAttributesCommonToAllAnimations(anim);

        // Read the animations that the script animation consist of

        int k = 1;

        while (m_cfg_script.Push("Animation", k)) {

            // Get name
            if (!m_cfg_script.Get("name", str))
                throw (config_error("NamedCoreBodies::loadBlendAnimations(): " + m_cfg_script.GetLastError()));

            // Get start time - set to 0 if not given
            if (!m_cfg_script.Get("degree", f_value))
                f_value = 0.0f;

            // Add animation with name and degree
            anim->addAnimation(str,f_value);

            // Update animation struct nr
            k++;

            // Pop animation struct
            if (!m_cfg_script.Pop())
                throw (config_error("NamedCoreBodies::loadScriptAnimations(): " + m_cfg_script.GetLastError()));
        }

        // Add animation to core body
        new_core_body->addAnimation(anim);

        // Update animation struct nr
        i++;

        // Pop animation struct
        if (!m_cfg_script.Pop())
            throw (config_error("NamedCoreBodies::loadScriptAnimations(): " + m_cfg_script.GetLastError()));

        // Print success message
        //CRCore::notify(CRCore::INFO) << "Script animation \"" << anim->getName() << "\" with act name \"" << anim->getActName() << "\" loaded." << std::endl;

    }

    // Print warning if no script animations loaded
    //if (i == 1)
    //    CRCore::notify(CRCore::INFO) << "No script animations loaded." << std::endl;


}


/*----------------------------------------------------------------------------
  Load meshes
  ----------------------------------------------------------------------------*/
void NamedCoreBodies::loadMeshes(CoreBody *new_core_body,const std::string& fileName)
{

    Mesh *mesh;
    std::string str;
    int i_value;
    int i = 1;

    while (m_cfg_script.Push("Mesh", i)) {

        // Allocate memory for new mesh
        mesh = new Mesh();

        // Get and set name
        if (!m_cfg_script.Get("name", str))
		{
			char gbuf[256];
			sprintf(gbuf,"NamedCoreBodies::loadMeshes(): %s\n\0",m_cfg_script.GetLastError().c_str());
			gDebugInfo->debugInfo(CRCore::WARN,gbuf);
			delete mesh;
			continue;
            //throw (config_error("NamedCoreBodies::loadMeshes(): " + m_cfg_script.GetLastError()));
		}
        mesh->setName(str);

        // Get and set filename
        if (!m_cfg_script.Get("filename", str))
		{
			char gbuf[256];
			sprintf(gbuf,"NamedCoreBodies::loadMeshes(): %s\n\0",m_cfg_script.GetLastError().c_str());
			gDebugInfo->debugInfo(CRCore::WARN,gbuf);
			delete mesh;
			continue;
            //throw (config_error("NamedCoreBodies::loadMeshes(): " + m_cfg_script.GetLastError()));
		}
        mesh->setFilename(str);

		mesh->setMaterialFileName(fileName);

		if (m_cfg_script.Get("partid", i_value))
			mesh->setPartID(i_value);

        // Get and set is_default. If not given, set to default
        if (!m_cfg_script.Get("is_default", i_value)) 
            mesh->setIsDefault(true);
        else 
            mesh->setIsDefault(i_value != 0);

        // Add mesh to core body
        new_core_body->addMesh(mesh);

        // Update animation struct nr
        i++;

        // Pop animation struct
        if (!m_cfg_script.Pop())
		{
			char gbuf[256];
			sprintf(gbuf,"NamedCoreBodies::loadMeshes(): %s\n\0",m_cfg_script.GetLastError().c_str());
			gDebugInfo->debugInfo(CRCore::WARN,gbuf);
			break;
            //throw (config_error("NamedCoreBodies::loadMeshes(): " + m_cfg_script.GetLastError()));
		}

        // Print success message
        //CRCore::notify(CRCore::INFO) << "Mesh \"" << mesh->getName() << "\" loaded." << std::endl;

    }

    // Print warning if no meshes were loaded
    //if (i == 1)
    //    CRCore::notify(CRCore::INFO) << "NamedCoreBodies::loadMeshes(): Warning: No meshes loaded from config file" << std::endl;

}


/*----------------------------------------------------------------------------
  Load materials
  ----------------------------------------------------------------------------*/
void NamedCoreBodies::loadMaterials(CoreBody *new_core_body)
{

    Cal3dBase material;
    std::string str;
    int i = 1;

    while (m_cfg_script.Push("Material", i)) {

        // Get and set filename
        if (!m_cfg_script.Get("filename", str))
            throw (config_error("NamedCoreBodies::loadMaterials(): " + m_cfg_script.GetLastError()));

        material.setFilename(str);

        // Add material to core body
        new_core_body->addMaterial(material);

        // Update animation struct nr
        i++;

        // Pop animation struct
        if (!m_cfg_script.Pop())
            throw (config_error("NamedCoreBodies::loadMaterials(): " + m_cfg_script.GetLastError()));

        // Print success message
       // CRCore::notify(CRCore::INFO) << "Material \"" << material.getFilename() << "\" loaded." << std::endl;

    }

    // Print warning if no materials were loaded
    //if (i == 1)
    //    CRCore::notify(CRCore::INFO) << "NamedCoreBodies::loadMaterials(): Warning: No materials loaded from config file" << std::endl;

}


/*----------------------------------------------------------------------------
  Load material sets
  ----------------------------------------------------------------------------*/
void NamedCoreBodies::loadMaterialSets(CoreBody *new_core_body)
{
    MaterialSet *mat_set;
    std::string str;
    int i_value;
    int i = 1;

    while (m_cfg_script.Push("MaterialSet", i)) {

        // Allocate memory for new material set
        mat_set = new MaterialSet();

        // Get and set name
        if (!m_cfg_script.Get("name", str))
            throw (config_error("NamedCoreBodies::loadMaterialSets(): " + m_cfg_script.GetLastError()));
        mat_set->setName(str);

        // Get and set is_default. If not given, set to default
        if (!m_cfg_script.Get("is_default", i_value)) 
            mat_set->setIsDefault(true);
        else 
            mat_set->setIsDefault(i_value != 0);

        // Read the animations that the script animation consist of

        int k = 1;

        while (m_cfg_script.Push("Connection", k)) {

            int i_value1, i_value2;

            // Get name
            if (!m_cfg_script.Get("thread_id", i_value1))
                throw (config_error("NamedCoreBodies::loadMaterialSets(): " + m_cfg_script.GetLastError()));

            if (!m_cfg_script.Get("material_id", i_value2))
                throw (config_error("NamedCoreBodies::loadMaterialSets(): " + m_cfg_script.GetLastError()));

            mat_set->addConnection(i_value1, i_value2);


            // Update animation struct nr
            k++;

            // Pop animation struct
            if (!m_cfg_script.Pop())
                throw (config_error("NamedCoreBodies::loadMaterialSets(): " + m_cfg_script.GetLastError()));
        }

        // Add material set to core body - 
        new_core_body->addMaterialSet(mat_set);

        // Update animation struct nr
        i++;

        // Pop animation struct
        if (!m_cfg_script.Pop())
            throw (config_error("NamedCoreBodies::loadMaterialSets(): " + m_cfg_script.GetLastError()));

        // Print success message
       // CRCore::notify(CRCore::INFO) << "Material set \"" << mat_set->getName() << "\" loaded." << std::endl;

    }

    // Print warning if no material sets were loaded
    //if (i == 1)
     //   CRCore::notify(CRCore::INFO) << "NamedCoreBodies::loadMaterialSets(): Warning: No material sets loaded from config file" << std::endl;

}


/*----------------------------------------------------------------------------
  Load conflicting actions
  ----------------------------------------------------------------------------*/
void NamedCoreBodies::loadActionConflicts(CoreBody *new_core_body)
{

    ActionConflicts *confl_act;
    std::string str1, str2;
    int i = 1;

    while (m_cfg_script.Push("ActionConflicts", i)) {

        // Allocate memory for new material set
        confl_act = new ActionConflicts();

        // Get name of action 1
        if (!m_cfg_script.Get("action1", str1))
            throw (config_error("NamedCoreBodies::loadActionConflicts(): " + m_cfg_script.GetLastError()));

        // Get name of action 2
        if (!m_cfg_script.Get("action2", str2))
            throw (config_error("NamedCoreBodies::loadActionConflicts(): " + m_cfg_script.GetLastError()));

        new_core_body->getActionConflicts()->addConflictingActions(str1,str2);

        // Update animation struct nr
        i++;

        // Pop animation struct
        if (!m_cfg_script.Pop())
            throw (config_error("NamedCoreBodies::loadActionConflicts(): " + m_cfg_script.GetLastError()));

        // Print success message
        CRCore::notify(CRCore::WARN) << "Actions \"" << str1 << "\" and \"" << str2 << "\" marked as conflicting." << std::endl;

    }

    // Print warning if no conflicting actions added
    //if (i == 1)
    //    CRCore::notify(CRCore::INFO) << "No conflicting actions loaded." << std::endl;

}


/*----------------------------------------------------------------------------
  Load in-between activities
  ----------------------------------------------------------------------------*/
void NamedCoreBodies::loadInBetweenActivities(CoreBody *new_core_body)
{

    InBetweenActivity *inb_act;
    std::string str;
    int i = 1;

    while (m_cfg_script.Push("InBetweenActivity", i)) {

        // Allocate memory for new material set
        inb_act = new InBetweenActivity();

        // Get name of start activity
        if (!m_cfg_script.Get("start_activity", str))
            throw (config_error("NamedCoreBodies::loadInBetweenActivities(): " + m_cfg_script.GetLastError()));

        inb_act->setStartActivity(str);

        // Get name of end activity
        if (!m_cfg_script.Get("end_activity", str))
            throw (config_error("NamedCoreBodies::loadInBetweenActivities(): " + m_cfg_script.GetLastError()));

        inb_act->setEndActivity(str);

        // Get name of in-between activity
        if (!m_cfg_script.Get("in_between_activity", str))
            throw (config_error("NamedCoreBodies::loadInBetweenActivities(): " + m_cfg_script.GetLastError()));

        inb_act->setInBetweenActivity(str);

        // Add material set to core body
        new_core_body->addInBetweenActivity(inb_act);

        // Update animation struct nr
        i++;

        // Pop animation struct
        if (!m_cfg_script.Pop())
            throw (config_error("NamedCoreBodies::loadInBetweenActivities(): " + m_cfg_script.GetLastError()));

        // Print success message
        //CRCore::notify(CRCore::INFO) << "Transition activity \"" << inb_act->getInBetweenActivity() << "\" between \"" << inb_act->getStartActivity() << "\" and \"" << inb_act->getEndActivity() << "\" loaded." << std::endl;

    }

    // Print warning if no in-between activities were added
    //if (i == 1)
      //  CRCore::notify(CRCore::INFO) << "No in-between actvities loaded." << std::endl;

}


/*----------------------------------------------------------------------------
  Load given named core body
  ----------------------------------------------------------------------------*/
void NamedCoreBodies::loadCoreBody(const std::string& core_body_name,const std::string& fileName)
{
	//CRCore::notify(CRCore::ALWAYS)<<"loadCoreBody "<<core_body_name.c_str()<<std::endl;
	// Insert 1 into number of references map
	incNrOfCoreBodyInstances(core_body_name);
    // Return if core body already exists
    if (getCoreBodyFromMap(core_body_name) != NULL)
        return;

    // Initialize the config package, open config file, check if header is OK and parse it.
    // Stores all data locally inside the Config package for later retreival using m_cfg_script.Get() functions

    // Set filename
	CRIOManager::crLoadCookFile scopedLoad(core_body_name);
	std::string bodyfile = scopedLoad.getFileName();
    std::string filename = m_path_container->findFile(bodyfile);

    if (filename.empty())
	{
		char gbuf[256];
		sprintf(gbuf,"NamedCoreBodies::loadCoreBody error filename is empty %s\n\0",core_body_name.c_str());
		gDebugInfo->debugInfo(CRCore::WARN,gbuf);
		return;
	}

    // Print what core body we're trying to load
    //CRCore::notify(CRCore::ALWAYS) << "---- REPLICANT CORE BODY \"" << m_path_container->extractFilename(core_body_name) << "\" ----" << std::endl;
    //CRCore::notify(CRCore::ALWAYS) << "Path to configuration file: " << m_path_container->extractDirectory(filename) << std::endl;

    // Make sure header is correct
    if (!m_cfg_script.Open(filename, m_config_file_header))
	{
		char gbuf[256];
		sprintf(gbuf,"NamedCoreBodies::loadCoreBody error filename %s error:%s\n\0",core_body_name.c_str(),m_cfg_script.GetLastError().c_str());
		gDebugInfo->debugInfo(CRCore::WARN,gbuf);
		return;
	}

    // Allocate memory for new core body instance
    CoreBody *new_core_body = new CoreBody();

    std::string str;

    // Open avatar struct and make reday for reading
    // TODO: Avatar shouldn't be hard coded here...
    // if (!m_cfg_script.Push("Avatar"))
    //	throw (config_error("NamedCoreBodies::loadCoreBody(): " + m_cfg_script.GetLastError()));

    // Set name to name of config file
    new_core_body->setName(core_body_name);

    //	str = m_cfg_script.GetLastError();

    // Get and set path
    if (!m_cfg_script.Get("path", str))
	{
		char gbuf[256];
		sprintf(gbuf,"NamedCoreBodies::loadCoreBody error filename %s error:%s\n\0",core_body_name.c_str(),m_cfg_script.GetLastError().c_str());
		gDebugInfo->debugInfo(CRCore::WARN,gbuf);
		return;
	}

    new_core_body->setPath(str);

    // Get and create skeleton
    if (!m_cfg_script.Get("skeleton", str))
	{
		char gbuf[256];
		sprintf(gbuf,"NamedCoreBodies::loadCoreBody error filename %s error:%s\n\0",core_body_name.c_str(),m_cfg_script.GetLastError().c_str());
		gDebugInfo->debugInfo(CRCore::WARN,gbuf);
		return;
	}

    // Construct path to cal3d data files (skeleton, meshes, materials, animations)

    /*  std::list<std::string> test = m_path_container->getFilePathList();
        std::list<std::string>::iterator it;

        for (it = test.begin(); it != test.end(); it++)
        CRCore::notify(CRCore::INFO) << (*it) << std::endl; */

    // Is the path relative in config file?
    std::string cal3d_data_path = m_path_container->findFile(m_path_container->extractDirectory(filename) + new_core_body->getPath() + str);

    // If not relative, try to find absolute
    if (cal3d_data_path.empty())
        cal3d_data_path = m_path_container->findFile(new_core_body->getPath() + str);

    // If neither relative or absolute -
    // maybe the path from the config file is not needed
    if (cal3d_data_path.empty())
        cal3d_data_path = m_path_container->findFile(m_path_container->extractDirectory(filename) + str);

    cal3d_data_path = m_path_container->extractDirectory(cal3d_data_path);

    // If filename to skeleton file has a path, remove this path from path to cal3d data files
    if (!cal3d_data_path.empty() && !m_path_container->extractDirectory(str).empty())
        cal3d_data_path.erase(cal3d_data_path.end()-m_path_container->extractDirectory(str).size(),cal3d_data_path.end());

    new_core_body->setPath(cal3d_data_path);

    //CRCore::notify(CRCore::INFO) << "Path to cal3d data files: " << cal3d_data_path << std::endl << "Loading data..." << std::endl;

    // Get and set scale if found
    float scale;
    if (m_cfg_script.Get("scale", scale))
        new_core_body->setScale(scale);

	float angle;
	if (m_cfg_script.Get("rot_angle", angle))
		new_core_body->setRotAngle(angle);

	std::vector<float> v_i;
    if (m_cfg_script.Get("trans_offset", v_i))
        new_core_body->setTransOffset(crVector3(v_i[0],v_i[1],v_i[2]));

    //CRCore::notify(CRCore::INFO) << "Scale set to " << new_core_body->getScale() << "." << std::endl;

    new_core_body->createSkeleton(str);

    // Load core animations
    loadAnimations(new_core_body);

    // Load speed animations
    loadSpeedAnimations(new_core_body);

    // Load degree animations
    loadDegreeAnimations(new_core_body);

    // Load blend animations
    loadBlendAnimations(new_core_body);

    // Load script animations
    loadScriptAnimations(new_core_body);

    // Load meshes
    loadMeshes(new_core_body,fileName);

    // Load materials
    //loadMaterials(new_core_body);

    // Load textures bind to materials into opengl (skip, now using CRCore to do this)
    // new_core_body->loadAllMaterialTextures();

    // Load material sets
    //loadMaterialSets(new_core_body);

    // Load conflicting actions
    loadActionConflicts(new_core_body);

    // Load in-between activities
    loadInBetweenActivities(new_core_body);

    // Get and set default activity
    if (m_cfg_script.Get("default_act", str)) {
        new_core_body->setDefaultActivity(str);
        //CRCore::notify(CRCore::INFO) << "Default activity set to \"" << str << "\"." << std::endl;
    }
	//else 
 //       CRCore::notify(CRCore::INFO) << "No default activity set." << std::endl;

    // Get and set contact bones
    int nr = 1;
    if (m_cfg_script.Push("ContactBones")) 
	{
        while(m_cfg_script.Get("bone_name", str, nr++)) 
		{
            if (!new_core_body->addContactBone(str))
			{
				char gbuf[256];
				sprintf(gbuf,"Warning: Unable to load contact bone %s doesn't exist in skeleton.\n\0",str.c_str());
				gDebugInfo->debugInfo(CRCore::WARN,gbuf);
               // CRCore::notify(CRCore::WARN) << "Warning: Unable to load contact bone \"" << str << "\" - doesn't exist in skeleton." << std::endl;
			}
        }
        m_cfg_script.Pop();
    }

    //if (nr == 1)
    //    CRCore::notify(CRCore::INFO) << "No contact bones added." << std::endl;

    // Get/set name of head bone
    std::string head_name = m_cfg_script.Return("DirectionBones.head_bone", "No head bone name given");
    new_core_body->setHeadBoneName(head_name);

	std::string weapon_name = m_cfg_script.Return("WeaponBones.weapon_bone", "No weapon bone name given");
	new_core_body->setWeaponBoneName(weapon_name);

    // Close and free memory
    m_cfg_script.Close();

    // Construct prototype for each action/activity - used by AI to know
    // which actions that exist and their properties
    new_core_body->buildActionPrototypes();

    // Insert core body into map
    m_core_body_map[core_body_name] = new_core_body;

    //// Insert 1 into number of references map
    //incNrOfCoreBodyInstances(core_body_name);

}


/*----------------------------------------------------------------------------
  Get reference to given named core body
  Return NULL if core body doesn't exist and cannot be loaded
  ----------------------------------------------------------------------------*/
CoreBody *NamedCoreBodies::getCoreBody(const std::string& core_body_name)
{

    CoreBody *requested_core_body;

    requested_core_body = getCoreBodyFromMap(core_body_name);

	//CRCore::notify(CRCore::ALWAYS)<<"getCoreBody "<<core_body_name.c_str()<<" requested_core_body = "<<requested_core_body<<std::endl;

    // Try to load core body if it isn't already loaded
    //if (requested_core_body == NULL) {

    //    // Load core body
    //    loadCoreBody(core_body_name);

    //    // If we get here the core body has successfully been loaded
    //    requested_core_body = getCoreBodyFromMap(core_body_name);

    //}

    // Return pointer to core body
    return requested_core_body;

}


/*----------------------------------------------------------------------------
  Create core body with skeleton and given name and return reference to it
  Return NULL if a core body with given name already exists
  ----------------------------------------------------------------------------*/
CoreBody *NamedCoreBodies::createCoreBody(const std::string& core_body_name, const std::string& path, const std::string& skeleton_filename)
{

    // Return NULL if core body already exists
    if (getCoreBodyFromMap(core_body_name) != NULL)
        return NULL;

    CoreBody *new_core_body = new CoreBody();

    // Set name of core body
    new_core_body->setName(core_body_name);

    // Set path
    new_core_body->setPath(path);

    // Create skeleton
    new_core_body->createSkeleton(skeleton_filename);

    m_core_body_map[core_body_name] = new_core_body;

    // Insert 1 into numer of references map
    incNrOfCoreBodyInstances(core_body_name);

    return new_core_body;

}


/*----------------------------------------------------------------------------
  Remove given core body
  ----------------------------------------------------------------------------*/
void NamedCoreBodies::removeCoreBody(const std::string& core_body_name)
{

    CoreBody *requested_core_body;

    requested_core_body = getCoreBodyFromMap(core_body_name);

	// Take the short way out if core body doesn't exist
	if (requested_core_body == NULL)
		return;

	// Delete memory core body is taking up and erase it from map
	delete requested_core_body;

  // Remove core body from maps
	m_core_body_map.erase(core_body_name);
  m_core_body_instances_map.erase(core_body_name);

}


/*----------------------------------------------------------------------------
 Get number of core bodies
 ----------------------------------------------------------------------------*/
int NamedCoreBodies::getNrOfCoreBodies()
{

	return m_core_body_map.size();

}


/*----------------------------------------------------------------------------
 Get reference to core body at given position
 Throws exception out of range if nr isn't in range 0 <= nr < size
 ----------------------------------------------------------------------------*/
CoreBody *NamedCoreBodies::getCoreBodyAt(int nr)
{
	// This isn't a good way to do it, but it works temporarly

	std::map<std::string, CoreBody *>::iterator core_bodies_iter;
	int counter = 0;

	// Iterate over core bodies until right index is reached
	for(core_bodies_iter = m_core_body_map.begin(); core_bodies_iter != m_core_body_map.end(); core_bodies_iter++){

		if (counter++ == nr)
			return core_bodies_iter->second;
	}

	// If we get here the index isn't valid - throw exception
	throw std::out_of_range("NamedCoreBodies::getCoreBodyAt()");

}

/*------------------------------------------

* $Source: /cvsroot/replicantbody/replicantbody/src/rbody/NamedCoreBodies.cpp,v $
* $Revision: 1.2 $ 
* $Date: 2005/02/09 17:26:56 $
* $Author: deepone $ 
* $Locker:  $

* Description: 

 Copyright (C) Peter Sunna, VRlab, Ume?University 2001

 Created: 2001-12-12

* $Log: NamedCoreBodies.cpp,v $
* Revision 1.2  2005/02/09 17:26:56  deepone
* vrutils and cfgscript put into new namespaces
*
* Revision 1.1.1.1  2005/02/06 06:52:59  deepone
* initial sourceforge checkin
*
* Revision 1.11  2003/12/19 16:40:09  deepone
* *** empty log message ***
*
* Revision 1.10  2003/10/31 13:21:49  andersb
* Added ReplicantBodyMgr class to handle instances of CreBody
* Added .char files for specifying individual characters (CreBody)
* Added OSG plugin for reading char files
*
* Revision 1.9  2003/09/03 07:19:54  sunna
* *** empty log message ***
*
* Revision 1.8  2003/02/28 12:01:36  sunna
* computeBound is now called explicity (temp solution)
*
* Revision 1.7  2002/11/22 14:44:21  sunna
* no message
*
* Revision 1.6  2002/11/13 15:35:06  sunna
* no message
*
* Revision 1.5  2002/11/05 10:22:46  sunna
* no message
*
* Revision 1.4  2002/11/01 14:59:22  sunna
* no message
*
* Revision 1.3  2002/11/01 09:26:29  sunna
* no message
*
* Revision 1.2  2002/06/27 14:30:17  sunna
* Using new file path container class
*
* Revision 1.1  2002/06/20 13:23:11  sunna
* no message
*
* Revision 1.16  2002/05/27 09:14:31  sunna
* Keeps track of number of instances using each core body - making it possible to free memory of not used core bodies
*
* Revision 1.15  2002/05/14 15:03:35  sunna
* no message
*
* Revision 1.14  2002/05/08 15:49:55  sunna
* Scaling of models - read scaling factor from config file
*
* Revision 1.13  2002/04/12 13:36:28  sunna
* Implementation of "contact bones", not yet tested
*
* Revision 1.12  2002/04/10 12:17:22  deepone
* added fallback to check for cal3d data files in config path if not found
* in path given in config file.
* Fixed some case-dependancy errors.
*
* Revision 1.11  2002/04/10 06:55:06  sunna
* no message
*
* Revision 1.10  2002/04/04 15:41:55  sunna
* no message
*
* Revision 1.9  2002/04/04 14:30:08  sunna
* Load default activity from config file
*
* Revision 1.8  2002/03/27 16:58:46  sunna
* Possible to load confliting actions and in-between activities from config file
* In-between activities implemented but not tested
*
* Revision 1.7  2002/03/27 10:21:32  sunna
* InBetweenActivity is now handling transition activities
*
* Revision 1.6  2002/03/26 17:36:33  sunna
* Environment variable REPLICANTBODY_FILE_PATH is searched for file paths => new methods for working with files
* Core bodies are now named after the name of their config file
*
* Revision 1.5  2002/03/21 09:37:28  sunna
* Small changes in method headers
*
* Revision 1.4  2002/03/21 09:25:23  sunna
* Added get and set config file header
* Default header is now "# ReplicantBody v0.1"
* Removed avatar struct in config file and therefore changed the parsing
*
* Revision 1.3  2002/03/20 15:09:17  sunna
* Complemented error messages to include missing files etc (in a perfect world cal3d would handle this)
*
* Revision 1.2  2002/03/15 08:55:09  andersb
* no message
*
* Revision 1.1.1.1  2002/03/14 20:48:40  andersb
* First import as ReplicantBody
*
* Revision 1.3  2002/03/13 12:47:31  andersb
* Now using the ObjectOriented version of ConfigScript.
*
* Revision 1.2  2002/03/04 13:42:42  andersb
* no message
*
* Revision 1.1.1.1  2002/01/04 08:22:41  andersb
* Initial version
*
----------------------------------------------------------------------------*/


