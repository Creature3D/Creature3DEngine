//Creature3D
//modify by Îâ²Æ»ª
#include "rbody/CoreBody.h"
#include <rvrutils/CodeLineException.h>
#include <rvrutils/Notify.h>
#include <CRCore/crNotify.h>
#include <CRCore/crStreamBuf.h>
using namespace rbody;


/*----------------------------------------------------------------------------
  Constructor
  ----------------------------------------------------------------------------*/
CoreBody::CoreBody()
{

    m_cal3d_core_model = new CalCoreModel("dummy");
    // Set default scale factor to 1 (no scaling)
    m_scale = 1.0f;
    m_action_conflicts = new ActionConflicts();
    m_default_activity = std::string("");
    m_head_bone = std::string("");
    //m_foot_offset = 0.0f;

}


/*----------------------------------------------------------------------------
  Destructor
  ----------------------------------------------------------------------------*/
CoreBody::~CoreBody()
{
    std::list<AnimationBase *>::iterator anim_list_iter;

    // Loop through all animations and free memory
    for (anim_list_iter = m_animation_list.begin(); anim_list_iter != m_animation_list.end(); anim_list_iter++) {

        delete (*anim_list_iter);
    }

    m_animation_list.clear();

    std::list<Mesh *>::iterator mesh_list_iter;

    // Loop through all meshes and free memory
    for (mesh_list_iter = m_mesh_list.begin(); mesh_list_iter != m_mesh_list.end(); mesh_list_iter++) {

        delete (*mesh_list_iter);
    }

    m_mesh_list.clear();

    std::list<MaterialSet *>::iterator m_set_iter;

    // Loop through all material sets and free memory
    for (m_set_iter = m_material_set_list.begin(); m_set_iter != m_material_set_list.end(); m_set_iter++) {

        delete (*m_set_iter);
    }

    m_material_set_list.clear();

    std::list<InBetweenActivity *>::iterator m_act_iter;

    // Loop through all in-between activities and free memory
    for (m_act_iter = m_in_between_activity_list.begin(); m_act_iter != m_in_between_activity_list.end(); m_act_iter++) {

        delete (*m_act_iter);
    }

    m_in_between_activity_list.clear();

    // Free memory core cal3d model is using
    delete m_cal3d_core_model;

    // Free memory that confliting actions have been using
    if (m_action_conflicts != NULL)
        delete m_action_conflicts;

    //  m_action_prototypes

}


/*----------------------------------------------------------------------------
  Set name
  ----------------------------------------------------------------------------*/
void CoreBody::setName(const std::string &name)
{

    m_name = name;

}


/*----------------------------------------------------------------------------
  Get name of core body
  ----------------------------------------------------------------------------*/
std::string CoreBody::getName()
{

    return m_name;

}


/*----------------------------------------------------------------------------
  Create skeleton from given cal3d skeleton file
  ----------------------------------------------------------------------------*/
void CoreBody::createSkeleton(const std::string &filename)
{
   // std::string path_filename = m_path + filename;
	m_skeletonFileName = m_path + filename;
    // Load skeleton
    bool success = m_cal3d_core_model->loadCoreSkeleton(m_skeletonFileName);
////#ifdef _DEBUG
//	if(success)
//	{
//		std::string boneNames;
//		for( int i = 0; i<m_cal3d_core_model->getCoreSkeleton()->getVectorCoreBone().size(); i++ )
//		{
//			CalCoreBone *bone = m_cal3d_core_model->getCoreSkeleton()->getVectorCoreBone()[i];
//			boneNames += bone->getName()+'\n';
//		}
//		std::string file = m_path + filename + ".txt";
//		FILE *fp=fopen(file.c_str(),"w");
//		fprintf(fp,"%s",boneNames.c_str());
//		fclose(fp);
//
//		
//	}
////#endif 
	// Throw exception if skeleton couldn't be loaded
    if (!success)
	{
		//CRCore::notify(CRCore::ALWAYS)<<"CoreBody::createSkeleton(): "<< CalError::getLastErrorDescription()<<" ("<<m_skeletonFileName<<")"<<std::endl;
		char gbuf[256];
		sprintf(gbuf,"CoreBody::createSkeleton():%s (%s)\n\0",CalError::getLastErrorDescription().c_str(),m_skeletonFileName.c_str());
		gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
        //throw config_error("CoreBody::createSkeleton(): " + CalError::getLastErrorDescription() + " (" + path_filename + ")");
	}

}


/*----------------------------------------------------------------------------
  Add animation of any type
  ----------------------------------------------------------------------------*/
void CoreBody::addAnimation(AnimationBase *animation)
{
    if (getAnimation(animation->getName()) != NULL)
	{
        //CRCore::notify(CRCore::ALWAYS)<<"CoreBody::addAnimation(): Animation name "<< animation->getName()<<" already exists"<<std::endl;
		char gbuf[256];
		sprintf(gbuf,"CoreBody::addAnimation(): Animation name %s already exists\n\0",animation->getName().c_str());
		gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
		return;
	}

    if (animation->getName().empty())
	{
		//CRCore::notify(CRCore::ALWAYS)<<"CoreBody::addAnimation(): Animation name must be set ("<< animation->getName()<<")"<<std::endl;
		char gbuf[256];
		sprintf(gbuf,"CoreBody::addAnimation(): Animation name must be set (%s)\n\0",animation->getName().c_str());
		gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
		return;
	}

    if (animation->getActName().empty())
	{
		//CRCore::notify(CRCore::ALWAYS)<<"CoreBody::addAnimation(): Activity/action name must be set"<<std::endl;
		char gbuf[256];
		sprintf(gbuf,"CoreBody::addAnimation(): Activity/action name must be set (%s)\n\0",animation->getName().c_str());
		gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
		return;
	}

    if (animation->getType() == AnimationBase::REAL){

        Animation *animation_real;

        // Cast base animation to animation real
        animation_real = (Animation *) animation;

        // Load animation into cal3d and get id

        std::string filename = m_path + animation_real->getFilename();

        int cal3d_id = m_cal3d_core_model->loadCoreAnimation(filename);

        // Check if animation loaded successfully
        if (cal3d_id == -1)
		{
         //   throw config_error("CoreBody::addAnimation(): " + CalError::getLastErrorDescription() + " (" + filename + ")");
			//CRCore::notify(CRCore::ALWAYS)<<"CoreBody::addAnimation error = "<<filename<<std::endl;
			char gbuf[256];
			sprintf(gbuf,"CoreBody::addAnimation error = %s\n\0",filename.c_str());
			gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
		}
		else
		{
            // Set cal3d id
            animation_real->setCal3dId(cal3d_id);
            // Insert animation real into animation list
            m_animation_list.push_back(animation_real);
		}
        return;	
    }

    if (animation->getType() == AnimationBase::DEGREE){

        AnimationDegree *animation_degree;

        // Cast base animation to animation degree
        animation_degree = (AnimationDegree *) animation;

        if (animation_degree->getNrOfAnimations() != 2)
		{
			CRCore::notify(CRCore::ALWAYS)<<"CoreBody::addAnimation(): Degree animation must consist of two animations ("<<animation->getName()<<")"<<std::endl;
			return;
		}

        // Insert animation degree into animation list
        m_animation_list.push_back(animation_degree);

        return;

    }

    if (animation->getType() == AnimationBase::SPEED){

        AnimationSpeed *animation_speed;
        AnimationBase *anim_base;
        Animation *anim;

        // Cast base animation to animation degree
        animation_speed = (AnimationSpeed *) animation;

        if (animation_speed->getNrOfAnimations() != 2)
		{
			CRCore::notify(CRCore::ALWAYS)<<"CoreBody::addAnimation(): Speed animation must consist of two animations"<<std::endl;
			return;
		}
        // Get speed of the two animations and set it
        for (int i = 0; i < 2; i++) {

            anim_base = getAnimation(animation_speed->getAnimationNameAt(i));

            if (anim_base == NULL)
			{
				CRCore::notify(CRCore::ALWAYS)<<"CoreBody::addAnimation(): Animation "<<animation_speed->getAnimationNameAt(i)<<" in speed animation doesn't exist ("<<animation->getName()<<")"<<std::endl;
				return;
			}
            if (anim_base->getType() != AnimationBase::REAL)
			{
				CRCore::notify(CRCore::ALWAYS)<<"CoreBody::addAnimation(): Animations in speed animation must be of type real ("<<animation->getName()<<")"<<std::endl;
				return;
			}

            // Convert to real animation
            anim = (Animation *) anim_base;

            animation_speed->setAnimationSpeedAt(i,anim->getSpeed());
        }

        // Insert animation speed into animation list
        m_animation_list.push_back(animation_speed);

        return;

    }

    if (animation->getType() == AnimationBase::BLEND){

        AnimationBlend *animation_blend;

        // Cast base animation to animation blend
        animation_blend = (AnimationBlend *) animation;

        if (animation_blend->getNrOfAnimations() < 2) 
		{
			CRCore::notify(CRCore::ALWAYS)<<"CoreBody::addAnimation(): Blend animation must consist of two or more animations ("<<animation->getName()<<")"<<std::endl;
			return;
		}

        // Insert animation blend into animation list
        m_animation_list.push_back(animation_blend);

        return;

    }

    if (animation->getType() == AnimationBase::SCRIPT){

        AnimationScript *animation_script;

        // Cast base animation to animation script
        animation_script = (AnimationScript *) animation;

        if (animation_script->getNrOfAnimations() < 2)
		{
			CRCore::notify(CRCore::ALWAYS)<<"CoreBody::addAnimation(): Script animation must consist of two or more animations ("<<animation->getName()<<")"<<std::endl;
			return;
		}

        // Insert animation script into animation list
        m_animation_list.push_back(animation_script);

        return;
    }

    // Throw exception here - not an animation type
	CRCore::notify(CRCore::ALWAYS)<<"CoreBody::addAnimation(): Not an animation type ("<<animation->getName()<<")"<<std::endl;
}

void CoreBody::deleteAnimation(AnimationBase *animation)
{
	if (animation->getName().empty())
	{
		CRCore::notify(CRCore::ALWAYS)<<"CoreBody::addAnimation(): Animation name must be set ("<<animation->getName()<<")"<<std::endl;
		return;
	}

	if (animation->getActName().empty())
	{
		CRCore::notify(CRCore::ALWAYS)<<"CoreBody::addAnimation(): Activity/action name must be set("<<animation->getName()<<")"<<std::endl;
		return;
	}

	if (animation->getType() == AnimationBase::REAL)
	{
		Animation *animation_real;
		animation_real = (Animation *) animation;
		//m_cal3d_core_model->unloadCoreAnimation(animation_real->getCal3dId());
	}

	for( std::list<AnimationBase *>::iterator itr = m_animation_list.begin();
		itr != m_animation_list.end();
		++itr )
	{
		if((*itr) == animation)
		{
			delete (*itr);
			m_animation_list.erase(itr);
			return;
		}
	}
}

/*----------------------------------------------------------------------------
  Add mesh
  ----------------------------------------------------------------------------*/
void CoreBody::addMesh(Mesh *mesh)
{
    if (mesh->getName().empty())
	{
		//CRCore::notify(CRCore::ALWAYS)<<"NamedCoreBodies::addMesh(): No mesh name set"<<std::endl;
		char gbuf[256];
		sprintf(gbuf,"NamedCoreBodies::addMesh(): No mesh name set\n\0");
		gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
		return;
	}

    std::string filename = m_path + mesh->getFilename();

    int cal3d_id = m_cal3d_core_model->loadCoreMesh(filename);

    if (cal3d_id == -1)
	{
		//CRCore::notify(CRCore::ALWAYS)<<"CoreBody::addMesh(): "<<CalError::getLastErrorDescription()<<" ("<<filename<<")"<<std::endl;
		char gbuf[256];
		sprintf(gbuf,"CoreBody::addMesh():%s (%s)\n\0",CalError::getLastErrorDescription().c_str(),filename.c_str());
		gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
        return;
	}

    // Set cal3d id
    mesh->setCal3dId(cal3d_id);

    // Add mesh to list
    m_mesh_list.push_back(mesh);

}

void CoreBody::deleteMesh(const std::string &meshName)
{
	std::list<Mesh *>::iterator mesh_list_iter;

	for (mesh_list_iter = m_mesh_list.begin(); mesh_list_iter != m_mesh_list.end(); mesh_list_iter++) 
	{
		if ((*mesh_list_iter)->getName() == meshName)
		{
			m_cal3d_core_model->unloadCoreMesh((*mesh_list_iter)->getCal3dId());
            delete (*mesh_list_iter);
            m_mesh_list.erase(mesh_list_iter);
			break;
		}
	}
}

void CoreBody::deleteMesh(int mesh_id)
{
	std::list<Mesh *>::iterator mesh_list_iter;

	for (mesh_list_iter = m_mesh_list.begin(); mesh_list_iter != m_mesh_list.end(); mesh_list_iter++) 
	{
		if ((*mesh_list_iter)->getCal3dId() == mesh_id)
		{
			m_cal3d_core_model->unloadCoreMesh(mesh_id);
			delete (*mesh_list_iter);
			m_mesh_list.erase(mesh_list_iter);
			break;
		}
	}
}

std::string CoreBody::getCurrentPartMeshName(int partid)
{
	std::list<Mesh *>::iterator mesh_list_iter;

	for (mesh_list_iter = m_mesh_list.begin(); mesh_list_iter != m_mesh_list.end(); mesh_list_iter++) 
	{
		if ((*mesh_list_iter)->getPartID() == partid)
		{
			return (*mesh_list_iter)->getName();
		}
	}
	return "NULL";
}

Mesh * CoreBody::findMesh(const std::string &meshName)
{
	std::list<Mesh *>::iterator mesh_list_iter;

	for (mesh_list_iter = m_mesh_list.begin(); mesh_list_iter != m_mesh_list.end(); mesh_list_iter++) 
	{
		if ((*mesh_list_iter)->getName() == meshName)
		{
			return (*mesh_list_iter);
		}
	}
	return 0;
}

/*----------------------------------------------------------------------------
  Add material
  ----------------------------------------------------------------------------*/
void CoreBody::addMaterial(Cal3dBase material)
{


    std::string filename = m_path + material.getFilename();

    int cal3d_id = m_cal3d_core_model->loadCoreMaterial(filename);

    if (cal3d_id == -1)
        throw (config_error("CoreBody::addMaterial(): " + CalError::getLastErrorDescription() + " (" + filename + ")"));
}


/*----------------------------------------------------------------------------
  Add material set
  ----------------------------------------------------------------------------*/
int CoreBody::addMaterialSet(MaterialSet *material_set)
{
    if (material_set->getName().empty())
        throw (config_error("CoreBody::addMaterialSet(): No material set name set"));

    if (material_set->getNrOfConnections() == 0)
        throw (config_error("CoreBody::addMaterialSet(): Material set contains no connections (" + material_set->getName() + ")"));

    material_set->setCal3dId(m_material_set_list.size());

    int thread_id, material_id;
    int m_set_id = material_set->getCal3dId();

    // Add material - material thread connections to cal3d

    for (int i=0; i<material_set->getNrOfConnections(); i++) {

        // Get connection
        material_set->getConnectionAt(i, thread_id, material_id);

        // Create a material thread
        m_cal3d_core_model->createCoreMaterialThread(thread_id);

        // Initialize the material thread
        m_cal3d_core_model->setCoreMaterialId(thread_id, m_set_id, material_id);
    }

    m_material_set_list.push_back(material_set);
    return m_set_id;
}


/*----------------------------------------------------------------------------
  Add in-between activity
  ----------------------------------------------------------------------------*/
void CoreBody::addInBetweenActivity(InBetweenActivity *in_between_activity)
{

    if (in_between_activity->getStartActivity().empty())
	{
		CRCore::notify(CRCore::ALWAYS)<<"CoreBody::addInBetweenActivity(): Start activity name not set"<<std::endl;
        return;
	}

    if (in_between_activity->getInBetweenActivity().empty())
	{
		CRCore::notify(CRCore::ALWAYS)<<"CoreBody::addInBetweenActivity(): In-between activity name not set"<<std::endl;
		return;
	}

    if (in_between_activity->getEndActivity().empty())
	{
		CRCore::notify(CRCore::ALWAYS)<<"CoreBody::addInBetweenActivity(): End activity name not set"<<std::endl;
		return;
	}
    m_in_between_activity_list.push_back(in_between_activity);
}


/*----------------------------------------------------------------------------
  Get named animation of any type
  ----------------------------------------------------------------------------*/
AnimationBase *CoreBody::getAnimation(const std::string &animation_name)
{

    std::list<AnimationBase *>::iterator anim_list_iter;

    // Loop through all animations and search for given animation name
    for (anim_list_iter = m_animation_list.begin(); anim_list_iter != m_animation_list.end(); anim_list_iter++) {

        // Return animation if name is correct
        if ((*anim_list_iter)->getName() == animation_name)
            return (*anim_list_iter);
    }

    return NULL;

}


/*----------------------------------------------------------------------------
  Return true if given activity/action exists
  ----------------------------------------------------------------------------*/
bool CoreBody::actNameExists(const std::string &act_name)
{

    std::list<AnimationBase *>::iterator anim_list_iter;

    // Loop through all animations and search for given activity/action name
    for (anim_list_iter = m_animation_list.begin(); anim_list_iter != m_animation_list.end(); anim_list_iter++) {

        // Return true if activity/action name is correct
        if ((*anim_list_iter)->getActName() == act_name)
            return true;
    }

    // Return false if activity/action isn't found
    return false;

}



/*----------------------------------------------------------------------------
  Get animations connected to given activity/action
  ----------------------------------------------------------------------------*/
void CoreBody::getAnimationsWithActName(const std::string &act_name, std::list<AnimationBase *> &act_anim_list)
{

    std::list<AnimationBase *>::iterator anim_list_iter;

    // Loop through all animations and search for given activity/action name
    for (anim_list_iter = m_animation_list.begin(); anim_list_iter != m_animation_list.end(); anim_list_iter++) {

        // Add animation to list if activity/action name is correct
        if ((*anim_list_iter)->getActName() == act_name)
            act_anim_list.push_back(*anim_list_iter);
    }

}

void CoreBody::getCoreActNameSet(std::set<std::string> &actNameSet)
{
	actNameSet.clear();
	std::list<AnimationBase *>::iterator anim_list_iter;
	for (anim_list_iter = m_animation_list.begin(); anim_list_iter != m_animation_list.end(); anim_list_iter++) 
	{
		actNameSet.insert((*anim_list_iter)->getActName());
	}
}
void CoreBody::getDefaultActNameSet(std::set<std::string> &actNameSet)
{
	actNameSet.clear();
	std::list<AnimationBase *>::iterator anim_list_iter;
	for (anim_list_iter = m_animation_list.begin(); anim_list_iter != m_animation_list.end(); anim_list_iter++) 
	{
		if((*anim_list_iter)->isDefault())
		    actNameSet.insert((*anim_list_iter)->getActName());
	}
}
const std::string &CoreBody::getSkeletonFileName()
{
	return m_skeletonFileName;
}
void CoreBody::getMeshFileNames(std::set<std::string> &fileNameSet)
{
	std::string filename;
	std::list<Mesh *>::iterator mesh_list_iter;
	for (mesh_list_iter = m_mesh_list.begin(); mesh_list_iter != m_mesh_list.end(); mesh_list_iter++) 
	{
		filename = m_path + (*mesh_list_iter)->getFilename();
		fileNameSet.insert(filename);
	}
}
void CoreBody::getAnimationFileNames(std::set<std::string> &fileNameSet)
{
	std::string filename;
	std::list<AnimationBase *>::iterator anim_list_iter;
	Animation *animation_real;
	for (anim_list_iter = m_animation_list.begin(); anim_list_iter != m_animation_list.end(); anim_list_iter++) 
	{
		if ((*anim_list_iter)->getType() == AnimationBase::REAL)
		{
			// Cast base animation to animation real
			animation_real = (Animation *) (*anim_list_iter);

			filename = m_path + animation_real->getFilename();
			fileNameSet.insert(filename);
		}
	}
}
/*----------------------------------------------------------------------------
  Get number of available material sets
  ----------------------------------------------------------------------------*/
//int CoreBody::getNrOfMaterialSets()
//{
//
//    return m_material_set_list.size();
//
//}


/*----------------------------------------------------------------------------
  Get cal3d id of given mesh name
  Return -1 if no mesh with given name found
  ----------------------------------------------------------------------------*/
int CoreBody::getCal3dMeshId(const std::string &mesh_name)
{

    std::list<Mesh *>::iterator mesh_list_iter;

    // Loop through all meshes
    for (mesh_list_iter = m_mesh_list.begin(); mesh_list_iter != m_mesh_list.end(); mesh_list_iter++) {

        // Return cal3d id if name is correct
        if ((*mesh_list_iter)->getName() == mesh_name)
            return (*mesh_list_iter)->getCal3dId();
    }

    // Return -1 if no mesh with that name found
    return -1;

}

std::string CoreBody::getMeshName(int mesh_id)
{
	std::list<Mesh *>::iterator mesh_list_iter;

	// Loop through all meshes
	for (mesh_list_iter = m_mesh_list.begin(); mesh_list_iter != m_mesh_list.end(); mesh_list_iter++) {

		// Return cal3d id if name is correct
		if ((*mesh_list_iter)->getCal3dId() == mesh_id)
			return (*mesh_list_iter)->getName();
	}

	// Return -1 if no mesh with that name found
	return "NULL";
}
std::string CoreBody::getMeshMaterialFileName(int mesh_id)
{
	std::list<Mesh *>::iterator mesh_list_iter;

	// Loop through all meshes
	for (mesh_list_iter = m_mesh_list.begin(); mesh_list_iter != m_mesh_list.end(); mesh_list_iter++) {

		// Return cal3d id if name is correct
		if ((*mesh_list_iter)->getCal3dId() == mesh_id)
			return (*mesh_list_iter)->getMaterialFileName();
	}

	// Return -1 if no mesh with that name found
	return "NULL";
}

/*----------------------------------------------------------------------------
  Get cal3d id of all meshes that should be attached as default
  ----------------------------------------------------------------------------*/
void CoreBody::getDefaultMeshIds(std::list<int> &mesh_id_list)
{

    std::list<Mesh *>::iterator mesh_list_iter;

    // Loop through all meshes
    for (mesh_list_iter = m_mesh_list.begin(); mesh_list_iter != m_mesh_list.end(); mesh_list_iter++) {

        // If mesh is default, add its cal3d id to mesh id list
        if ((*mesh_list_iter)->isDefault()) {
            mesh_id_list.push_back((*mesh_list_iter)->getCal3dId());
        }
    }

}


/*----------------------------------------------------------------------------
  Get cal3d if of default material set
  ----------------------------------------------------------------------------*/
int CoreBody::getDefaultMaterialSet()
{

    std::list<MaterialSet *>::iterator m_set_iter;

    // Loop through all material sets
    for (m_set_iter = m_material_set_list.begin(); m_set_iter != m_material_set_list.end(); m_set_iter++) {

        if ((*m_set_iter)->isDefault())
            return (*m_set_iter)->getCal3dId();
    }

	return 0;
    // Handle the case when there is no default material set - throw exception
    //throw (config_error("CoreBody::getDefaultMaterialSet(): There is no default material set"));

}


/*----------------------------------------------------------------------------
  Get in-between (transition) activity 
  ----------------------------------------------------------------------------*/
std::string CoreBody::getInBetweenActivity(const std::string &start_activity, const std::string &end_activity)
{

    std::list<InBetweenActivity *>::iterator m_act_iter;

    // Loop through all in-between activities and search for start and end activities
    for (m_act_iter = m_in_between_activity_list.begin(); m_act_iter != m_in_between_activity_list.end(); m_act_iter++) {

        // Return in-between activity is start and end match
        if (((*m_act_iter)->getStartActivity() == start_activity) && ((*m_act_iter)->getEndActivity() == end_activity))
            return std::string((*m_act_iter)->getInBetweenActivity());
    }

    // Return empty string if not found
    return std::string("");

}


/*----------------------------------------------------------------------------
  Get action conflicts object
  ----------------------------------------------------------------------------*/
ActionConflicts* CoreBody::getActionConflicts()
{

    return m_action_conflicts;

}

/*----------------------------------------------------------------------------
  Set path (where to find animations, meshes, materials)
  ----------------------------------------------------------------------------*/
void CoreBody::setPath(const std::string& path)
{

    m_path = path;

    // Make sure path ends with '/'
    if (!m_path.empty())
        if (m_path[path.size()-1] != '/')
            m_path += "/";

}


/*----------------------------------------------------------------------------
  Get path
  ----------------------------------------------------------------------------*/
std::string CoreBody::getPath()
{

    return m_path;

}


/*----------------------------------------------------------------------------
  Set scale of core body (default is 1)
  ----------------------------------------------------------------------------*/
void CoreBody::setScale(float scale)
{

    //m_cal3d_core_model->setScale(scale);
    m_scale = scale;

}


/*----------------------------------------------------------------------------
  Get scale of core body
  ----------------------------------------------------------------------------*/
float CoreBody::getScale()
{

    return m_scale;

}
void CoreBody::setRotAngle(float angle)
{
	m_rot_angle = angle;
}
float CoreBody::getRotAngle()
{
	return m_rot_angle;
}
/*----------------------------------------------------------------------------
  Set offset between foot ground level and foot bone in skeleton
  ----------------------------------------------------------------------------*/
void CoreBody::setTransOffset(const CRCore::crVector3& offset)
{
    m_trans_offset = offset;
}


/*----------------------------------------------------------------------------
  Get foot offset of core body
  ----------------------------------------------------------------------------*/
const CRCore::crVector3& CoreBody::getTransOffset()
{
    return m_trans_offset;
}


/*----------------------------------------------------------------------------
  Get reference to cal3d core model
  ----------------------------------------------------------------------------*/
CalCoreModel *CoreBody::getCal3dCoreModel()
{

    return m_cal3d_core_model;

}

/*----------------------------------------------------------------------------
  Add bone that, when touching the ground, affects speed of character - 
  returns true if bone name exist
  ----------------------------------------------------------------------------*/
bool CoreBody::addContactBone(const std::string &bone_name)
{

    // Get id
    int id = m_cal3d_core_model->getCoreSkeleton()->getCoreBoneId(bone_name);

    // Make sure bone exists
    if (id == -1)
        return false;

    // Add bone name to contact bone list
    m_contact_bone_list.push_back(id);

    return true;

}


/*----------------------------------------------------------------------------
  Get list with names of contact bones
  ----------------------------------------------------------------------------*/
std::list<int> CoreBody::getContactBoneList()
{

    return m_contact_bone_list;

}

void CoreBody::addActionPrototype(ActionRequest *action)
{

    EXCEPTION_IF_FALSE(action);
    if ( m_action_prototypes.find( action->getName() ) != m_action_prototypes.end() ) {
        NOTIFY_WARNING <<  "Adding (and overwriting) existing action: " << action->getName() << std::endl;
    }
    m_action_prototypes[action->getName()] = action;

}


void CoreBody::buildActionPrototypes()
{

    //m_action_prototypes.clear();

    std::list<AnimationBase *>::iterator anim_it;

    for ( anim_it = m_animation_list.begin(); anim_it != m_animation_list.end(); anim_it++ ) {

        // All animations connected to same activity/action has the same attributes (at least in a perfect world)
        // If this activity/action already is processed, just continue to next post
        if ( (m_action_prototypes.find( (*anim_it)->getActName() )) != m_action_prototypes.end() )
            continue;

		CRCore::ref_ptr<ActionRequest> action = new ActionRequest( (*anim_it)->getActName() );

        if ( (*anim_it)->getType() == AnimationBase::REAL ) {

            Animation *anim = dynamic_cast<Animation *> (*anim_it);
            EXCEPTION_IF_FALSE(anim);

			float duration = m_cal3d_core_model->getCoreAnimation(anim->getCal3dId())->getDuration();
			action->addPropertyFloat( "duration", duration );
			action->addPropertyInt( "loop", anim->isLooped() );
            // Get speed of animation (ie translation of root bone speed)
            float speed = anim->getSpeed();
            //action->addPropertyFloat( "speed_min", speed );
            //action->addPropertyFloat( "speed_max", speed );
            //action->addPropertyFloat( "speed_default", speed );
			action->addPropertyFloat( "speed", speed );

            addActionPrototype( action.get() );

            continue;
        }

        if ( (*anim_it)->getType() == AnimationBase::SPEED ) {

            AnimationSpeed *anim = dynamic_cast<AnimationSpeed *> (*anim_it);
            EXCEPTION_IF_FALSE(anim);

            // Get min and max speed and make sure they're in right order
            double speed_min = anim->getAnimationSpeedAt(0);
            double speed_max = anim->getAnimationSpeedAt(1);

            // Shift order if speed_min > speed_max
            if (speed_min > speed_max) {
                double tmp = speed_max;
                speed_max = speed_min;
                speed_min = tmp;
            }

            // Get default speed
            double speed_default = anim->getDefaultSpeed();

            // Get speed of animation (ie translation of root bone speed)
            action->addPropertyFloat( "speed_min", speed_min );
            action->addPropertyFloat( "speed_max", speed_max );
            action->addPropertyFloat( "speed", speed_default );

            addActionPrototype( action.get() );

            continue;
        }

        if ( (*anim_it)->getType() == AnimationBase::DEGREE ) {

            AnimationDegree *anim = dynamic_cast<AnimationDegree *> (*anim_it);
            EXCEPTION_IF_FALSE(anim);

            // Get min and max speed and make sure they're in right order
            double angle_min = anim->getAnimationDegreeAt(0);
            double angle_max = anim->getAnimationDegreeAt(1);

            // Shift order if angle_min > angle_max
            if (angle_min > angle_max) {
                double tmp = angle_max;
                angle_max = angle_min;
                angle_min = tmp;
            }

            // Get default angle (hey! there isn't a default angle for this type of animation :)
            double angle_default = angle_min;
            action->addPropertyFloat( "angle_min", angle_min );
            action->addPropertyFloat( "angle_max", angle_max );
            action->addPropertyFloat( "angle", angle_default );

            addActionPrototype( action.get() );

            continue;
        }

		if ( (*anim_it)->getType() == AnimationBase::BLEND ) {

			AnimationBlend *anim = dynamic_cast<AnimationBlend *> (*anim_it);
			EXCEPTION_IF_FALSE(anim);

			addActionPrototype( action.get() );

			continue;
		}


        // Currently no animation prototypes for blend and script animation...
        // How come? The AI isn't using them at the moment

    } // for anim_it

}

ActionRequest* CoreBody::getActionPrototype(const std::string& name)
{

    ActionPrototypeMap::iterator i = m_action_prototypes.find( name );

    if (i == m_action_prototypes.end() )
        return NULL;

	return i->second.get();//->clone();
}

/*------------------------------------------

 * $Source: /cvsroot/replicantbody/replicantbody/src/rbody/CoreBody.cpp,v $
 * $Revision: 1.2 $ 
 * $Date: 2005/02/09 17:26:56 $
 * $Author: deepone $ 
 * $Locker:  $

 * Description: 

 Copyright (C) Peter Sunna, VRlab, Ume?University 2001

Created: 2001-12-12

 * $Log: CoreBody.cpp,v $
 * Revision 1.2  2005/02/09 17:26:56  deepone
 * vrutils and cfgscript put into new namespaces
 *
 * Revision 1.1.1.1  2005/02/06 06:52:59  deepone
 * initial sourceforge checkin
 *
 * Revision 1.13  2004/02/03 12:58:05  deepone
 * new Property.h, Body.h and Notify.h in rvrutils
 *
 * Revision 1.12  2003/10/31 13:21:49  andersb
 * Added ReplicantBodyMgr class to handle instances of CreBody
 * Added .char files for specifying individual characters (CreBody)
 * Added OSG plugin for reading char files
 *
 * Revision 1.11  2003/09/03 07:19:54  sunna
 * *** empty log message ***
 *
 * Revision 1.10  2002/11/22 14:44:21  sunna
 * no message
 *
 * Revision 1.9  2002/11/05 16:15:32  sunna
 * no message
 *
 * Revision 1.8  2002/11/05 15:37:12  sunna
 * no message
 *
 * Revision 1.7  2002/11/05 12:26:03  sunna
 * no message
 *
 * Revision 1.6  2002/11/01 14:59:22  sunna
 * no message
 *
 * Revision 1.5  2002/11/01 13:42:05  sunna
 * no message
 *
 * Revision 1.4  2002/11/01 13:29:29  sunna
 * no message
 *
 * Revision 1.3  2002/11/01 09:26:29  sunna
 * no message
 *
 * Revision 1.2  2002/10/22 16:29:02  sunna
 * no message
 *
 * Revision 1.1  2002/06/20 13:23:11  sunna
 * no message
 *
* Revision 1.13  2002/05/27 09:13:29  sunna
* Fixed bug in destructor
*
* Revision 1.12  2002/05/15 14:40:08  sunna
* Checks texture file format
*
* Revision 1.11  2002/05/14 08:01:32  sunna
* no message
*
* Revision 1.10  2002/05/08 15:49:55  sunna
* Scaling of models - read scaling factor from config file
*
* Revision 1.9  2002/04/18 10:07:02  sunna
* Added actNameExists
*
* Revision 1.8  2002/04/12 13:36:28  sunna
* Implementation of "contact bones", not yet tested
*
* Revision 1.7  2002/04/04 15:41:55  sunna
* no message
*
* Revision 1.6  2002/04/04 14:33:25  sunna
* Set default activity in constructor
* Material sets methods added
*
* Revision 1.5  2002/03/27 10:21:32  sunna
* InBetweenActivity is now handling transition activities
*
* Revision 1.4  2002/03/26 17:36:33  sunna
* Environment variable REPLICANTBODYFILEPATH is searched for file paths => new methods for working with files
* Core bodies are now named after the name of their config file
*
* Revision 1.3  2002/03/20 15:09:17  sunna
* Complemented error messages to include missing files etc (in a perfect world cal3d would handle this)
*
* Revision 1.2  2002/03/15 08:55:08  andersb
* no message
*
* Revision 1.1.1.1  2002/03/14 20:48:40  andersb
* First import as ReplicantBody
*
* Revision 1.2  2002/02/19 15:46:17  andersb
* *** empty log message ***
*
* Revision 1.1.1.1  2002/01/04 08:22:40  andersb
* Initial version
*
----------------------------------------------------------------------------*/

