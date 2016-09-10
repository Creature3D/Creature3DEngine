// $Id: Body.cpp,v 1.3 2005/02/09 17:26:56 deepone Exp $
//Creature3D
//modify by Wucaihua
#define DO_TIMING 0

#include <rbody/Body.h>
#include <CRCore/crStreamBuf.h>
using namespace rbody;


// Create and return static core body container - shared between all instances of class
//NamedCoreBodies Body::m_static_core_body_container;

NamedCoreBodies* Body::getCoreBodyContainer()
{
    /*static CRCore::ref_ptr<NamedCoreBodies> s_core_body = new NamedCoreBodies();
    return s_core_body.get();*/
	return NamedCoreBodies::getInstance();
}

const Body::MeshIDSet& Body::getBodyMeshSet() const
{
	return m_bodyMeshSet;
}
/*----------------------------------------------------------------------------
  Constructor
  ----------------------------------------------------------------------------*/
Body::Body(): m_rendering_mode(RENDER_DEFAULT), m_active_contact_bone(-1), m_last_delta_factor(0.0f), m_scale_factor(1.0f)
{
}

/*----------------------------------------------------------------------------
  Constructor
  ----------------------------------------------------------------------------*/
Body::Body(const std::string& core_body_name,const std::string& fileName,const Body::MeshIDSet& meshIDSet): m_rendering_mode(RENDER_DEFAULT), m_active_contact_bone(-1), m_last_delta_factor(0.0f)
{


    // Create and setup body
    create(core_body_name,fileName,meshIDSet);

}


/*----------------------------------------------------------------------------
  Destructor
  ----------------------------------------------------------------------------*/
Body::~Body()
{
    // Deregister this instance of core body in core body container
    if (getCoreBodyContainer())
        getCoreBodyContainer()->removeCoreBodyInstance(m_core_body_name);
	// Destroy the cal3d model instance
	delete m_cal3d_model;
	m_cal3d_model = NULL;
}

/*----------------------------------------------------------------------------
  Create body with given core name which can be found in given core bodies 
  instance
  ----------------------------------------------------------------------------*/
void Body::create(const std::string& core_body_name,const std::string& fileName,const Body::MeshIDSet& meshIDSet)
{
    //CRIOManager::crLoadCookFile scopedLoad(core_body_name);
    // Set name
    m_core_body_name = core_body_name;
	//std::string bodyfile = scopedLoad.getFileName();

    // Get pointer to core body from named core body container
    if (!getCoreBodyContainer())
        return;

	//CRCore::notify(CRCore::ALWAYS)<<"Body::create "<<core_body_name.c_str()<<std::endl;

	getCoreBodyContainer()->loadCoreBody(m_core_body_name,fileName);
    m_core_body = getCoreBodyContainer()->getCoreBody(m_core_body_name);

    if (m_core_body == NULL)
	{
		char gbuf[256];
		sprintf(gbuf,"Body::create(): Unable to load core body %s\n\0",core_body_name.c_str());
		gDebugInfo->debugInfo(CRCore::WARN,gbuf);
        //throw (config_error("Body::create(): Unable to load core body " + core_body_name));	
		return;
	}

    // Get pointer to cal3d core model
    m_cal3d_core_model = m_core_body->getCal3dCoreModel();

	if(m_cal3d_core_model->getCoreSkeleton())
	{
		// Initiate cal3d model from cal3d core model
		m_cal3d_model = new CalModel( m_cal3d_core_model );

		if(meshIDSet.empty())
		{// Get cal3d id of all meshes that should be attached as default
			std::list<int> mesh_id_list;
			std::list<int>::iterator mesh_id_iter;

			m_core_body->getDefaultMeshIds(mesh_id_list);

			// Attach all default meshes
			for (mesh_id_iter = mesh_id_list.begin(); mesh_id_iter != mesh_id_list.end(); mesh_id_iter++) {

				int id = *mesh_id_iter;
				attachMesh(id);
				//m_cal3d_model->attachMesh(id);
			}
		}
		else
		{
			for( MeshIDSet::const_iterator itr = meshIDSet.begin();
				 itr != meshIDSet.end();
				 ++itr )
			{
				attachMesh(*itr);
			}
		}
		// Get default material set id
		//m_material_set_id = m_core_body->getDefaultMaterialSet();

		// Set default material set
		//m_cal3d_model->setMaterialSet(m_material_set_id);

		// Initiate activity animation handler (TODO: 0.50f is a hard coded transition time...)
		m_activity_handler.onInit(m_core_body, m_cal3d_model, /*0.0f*/0.1f);

		// Initiate action animation handler (TODO: 0.50f is a hard coded transition time...)
		m_action_handler.onInit(m_core_body, m_cal3d_model, /*0.0f*/0.1f);

		// Highest details on polygons
		m_lod_poly = 1.0;

		// Get scale factor from core model
		m_scale_factor = m_core_body->getScale();

		//Get Default Anim from corebody
		//m_bodyActionPrototypeMap
		m_core_body->getDefaultActNameSet(m_bodyActNameSet);
	}
	//else
	//{
	//	MessageBox(::GetActiveWindow(),"文件错误!","Creature3D",MB_OK);
	//	_asm   int   3   //只是为了让程序崩溃
	//}
}


/*----------------------------------------------------------------------------
  Stop action
  ----------------------------------------------------------------------------*/
RequestResult Body::stopAction(const std::string& action)
{

    return m_action_handler.stopAction(action);
}


/*----------------------------------------------------------------------------
  Get conflicting actions
  ----------------------------------------------------------------------------*/
void Body::getConflictingActions(const std::string& action, std::list<std::string> &action_list)
{
    m_action_handler.getConflictingActions(action, action_list);
}

/*----------------------------------------------------------------------------
  Get state of action - returns UNKNOWN if not known
  ----------------------------------------------------------------------------*/
RequestResult Body::getActionState(const std::string& action)
{
    return m_action_handler.getActionState(action);
}


/*----------------------------------------------------------------------------
  Attach named mesh to body
  ----------------------------------------------------------------------------*/
bool Body::attachMesh(const std::string& mesh_name)
{
	int mesh_id = m_core_body->getCal3dMeshId(mesh_name);
	if(mesh_id != -1)
	{
		if(m_bodyMeshSet.find(mesh_id)==m_bodyMeshSet.end())
		{
			attachMesh(mesh_id);
			return true;
		}
	}
	return false;
}
void Body::attachMesh(int mesh_id)
{
    //// Get cal3d id of given mesh name
    //int mesh_id = m_core_body->getCal3dMeshId(mesh_name);
    // Attach mesh to cal3d model renderer
    if(m_cal3d_model->attachMesh(mesh_id))
	    m_bodyMeshSet.insert(mesh_id);
}


/*----------------------------------------------------------------------------
  Detach named mesh from body
  ----------------------------------------------------------------------------*/
void Body::detachMesh(int mesh_id)
{
    // Detach mesh from cal3d model renderer
    m_cal3d_model->detachMesh(mesh_id);
	m_bodyMeshSet.erase(mesh_id);
}

void Body::detachMesh(const std::string& mesh_name)
{
	detachMesh(m_core_body->getCal3dMeshId(mesh_name));
}

void Body::deleteMesh(int mesh_id)
{
	CalMesh *cal3d_mesh = m_cal3d_model->getMesh(mesh_id);
	if(cal3d_mesh)
	{
		CalSubmesh *submesh;
		for(int i = 0; i<cal3d_mesh->getSubmeshCount(); ++i)
		{
			submesh = cal3d_mesh->getSubmesh(i);
			if(submesh)
			{
				m_cal3d_core_model->unloadCoreMaterial(submesh->getCoreMaterialId());
			}
		}
		detachMesh(mesh_id);
		m_core_body->deleteMesh(mesh_id);
	}
}

void Body::deleteMesh(const std::string &meshName)
{
	deleteMesh(m_core_body->getCal3dMeshId(meshName));
}

void Body::addMesh(Mesh *mesh/*,int materialsetid*/)
{
    m_core_body->addMesh(mesh);
	attachMesh(mesh->getCal3dId());
 //   CalMesh *cal3d_mesh = m_cal3d_model->getMesh(mesh->getCal3dId());
	//cal3d_mesh->setMaterialSet(materialsetid);
}

/*----------------------------------------------------------------------------
  Set material set to be used, return true on success
  ----------------------------------------------------------------------------*/
//bool Body::setMaterialSet(int material_set_id)
//{
//
//    if (material_set_id < 0 || material_set_id >= m_core_body->getNrOfMaterialSets())
//        return false;
//
//    if (m_material_set_id != material_set_id) {
//        m_material_set_id = material_set_id;
//        m_cal3d_model->setMaterialSet(m_material_set_id);
//    }
//
//    return true;
//
//}

/*----------------------------------------------------------------------------
  Get id of bones that touch the ground in pose mode, ie, bones with lowest y-values
  ----------------------------------------------------------------------------*/
std::list<int> Body::getContactBoneIds()
{

    return m_core_body->getContactBoneList();

}



/*----------------------------------------------------------------------------
  Get translation of bone with given id
  ----------------------------------------------------------------------------*/
CRCore::crVector3 Body::getBoneTranslation(int bone_id)
{

    CRCore::crVector3 transl;

    // Get translation of bone
    CalVector v = m_cal3d_model->getSkeleton()->getBone(bone_id)->getTranslationAbsolute();

    // Copy values
    transl.set(v.x, v.y, v.z);

    return transl;

}


/*----------------------------------------------------------------------------
  Get delta translation (since last call of method) of contact bone
  ----------------------------------------------------------------------------*/
float Body::getDeltaContactBoneTranslation(float elapsed)
{

	if ( elapsed < 0.000000001f ) {
		return 0.0;
	}

	std::list<int>::iterator iter_bone_id;

	// If it is the first time this method is called, fill contact bone map with data
	if (m_contact_bone_map.empty()) {

		std::list<int> contact_ids = m_core_body->getContactBoneList();

		for (iter_bone_id = contact_ids.begin(); iter_bone_id != contact_ids.end(); iter_bone_id++) {

			// Insert bone id and y translation of bone into map
			m_contact_bone_map.insert(std::make_pair(*iter_bone_id, m_cal3d_model->getSkeleton()->getBone(*iter_bone_id)->getTranslationAbsolute().y));
		}

		// Return no translation
		return 0.0f;
	}

	// float active_delta = 0.0f;
	float delta_translation = 0.0f;

	int new_active_bone = -1;

	// Loop through all bones in map
	std::map<int, float>::iterator iter_bone_map;
	for (iter_bone_map = m_contact_bone_map.begin(); iter_bone_map != m_contact_bone_map.end(); iter_bone_map++) {

		int bone_id = (*iter_bone_map).first;

		bool enabled = false;

		// Is this bone enabled?
		for (iter_bone_id = m_enabled_contact_bones.begin(); iter_bone_id != m_enabled_contact_bones.end(); iter_bone_id++ ) {
			if (*iter_bone_id == bone_id) {
				enabled = true;
				break;
			}
		}

		// Get new translation of current bone
		float new_pos = m_cal3d_model->getSkeleton()->getBone(bone_id)->getTranslationAbsolute().y;


		// Only take enabled bones into account
		if (enabled) {

			// Calculate delta translation
			float delta_pos = new_pos - (*iter_bone_map).second;

			// Only take positive delta translation into account - temporary solution
			if (delta_pos > 0) {

				// If this is the active contact bone (contact bone used last step) - use this delta translation
				//if (m_active_contact_bone == bone_id)
				//active_delta = delta_pos;
				if (delta_pos > delta_translation) {
					delta_translation = delta_pos;
					new_active_bone = bone_id;
				}
			}
		}

		// Update translation of bone
		(*iter_bone_map).second = m_cal3d_model->getSkeleton()->getBone(bone_id)->getTranslationAbsolute().y;

	}

	// Make sure there are enabled contact bones
	if (m_enabled_contact_bones.empty()) {
		// std::cerr << "Warning: Body::getContactBoneTranslation(): No contact bones enabled , use enableContactBone()." << std::endl;
		return 0.0f;
	}

	// If active delta is set (delta translation of active contact bone), return this delta
	/*if (active_delta)
	return active_delta; */

	// Else, return delta translation of the enabled contact bone with largest delta translation
	// and set new active bone to be this bone

	//std::cerr << "Change of contact bone: " << new_active_bone << std::endl;
	m_active_contact_bone = new_active_bone;

	float new_delta_factor = delta_translation / elapsed;

	// Make sure movement is in range
	if (new_delta_factor < (getCurrentSpeed() * 0.5 / getScale())) {

		// This prohibits sliding steps when changing between animations
		if (m_last_delta_factor > new_delta_factor) {
			delta_translation = m_last_delta_factor * elapsed;
		}
	} else if (new_delta_factor > (getCurrentSpeed() * 2.0 / getScale())) {

		//if (m_last_delta_factor < new_delta_factor) {
		delta_translation = m_last_delta_factor * elapsed;
		//}
	}

	// Update delta factor
	m_last_delta_factor = delta_translation / elapsed;

	return delta_translation * getScale();

}


/*----------------------------------------------------------------------------
  Enable all contact bones for speed calculation
  ----------------------------------------------------------------------------*/
void Body::enableAllContactBones() 
{

    // Get all contact bones, this really isn't a good way of doing it since the copy operator is used
    // Referenced should be used (but then getContactBoneList must be redefined)
    std::list<int> contact_bones = m_core_body->getContactBoneList();
    std::list<int>::iterator cont_iter;

    // Make sure enabled contact bones list is empty
    m_enabled_contact_bones.clear();

    // Enable all contact bones
    for (cont_iter = contact_bones.begin(); cont_iter != contact_bones.end(); cont_iter++)
        m_enabled_contact_bones.push_back(*cont_iter);

};


/*----------------------------------------------------------------------------
  Enable contact bone with given id for translation calculation
  ----------------------------------------------------------------------------*/
void Body::enableContactBone(int bone_id) 
{

    m_enabled_contact_bones.push_back(bone_id);

};

/*----------------------------------------------------------------------------
  Disable all contact bones for speed calculation
  ----------------------------------------------------------------------------*/
void Body::disableAllContactBones() 
{

    m_enabled_contact_bones.clear();

};


/*----------------------------------------------------------------------------
  Set level of detail on polygons
  ----------------------------------------------------------------------------*/
void Body::setLodPoly(float lod_poly)
{

    // If the value of lod hasn't changed, then don't recalculate the level of detail.
    if (fabs(lod_poly - m_lod_poly) > 0.05) {
        // Svae new level of detail level for polygons
        m_lod_poly = lod_poly;
        // Set the new lod level in the cal3d model renderer
        if(m_cal3d_model) m_cal3d_model->setLodLevel(m_lod_poly);
    }

}


/*----------------------------------------------------------------------------
  Set scale factor applied on geometries and animations
  ----------------------------------------------------------------------------*/
void Body::setScale(float factor)
{
    m_scale_factor = factor;
}


/*----------------------------------------------------------------------------
  Get scale factor
  ----------------------------------------------------------------------------*/
float Body::getScale()
{
    return m_scale_factor;
}


/*----------------------------------------------------------------------------
  Update the body's animations according to elapsed seconds
  ----------------------------------------------------------------------------*/
bool Body::update(float elapsed_seconds)
{
	// Update the activity animations
	m_activity_handler.update(elapsed_seconds);
    // Update the action animations
	//if(elapsed_seconds>2.0f)
	//{
	//	int i = 0;
	//}
    bool stopact = m_action_handler.update(elapsed_seconds);
	//if(stopact)
	//{
	//	CRCore::notify(CRCore::NOTICE)<<"Body::update stopact "<<elapsed_seconds<<std::endl;
	//}
    // Update the cal3d model
    //if(!stopact)// elapsed_seconds = 0.0f;
	m_cal3d_model->update(elapsed_seconds);//deleteAnimation时会导致此处出现内存错误

    // Update direction property around z-axis (if angle velocity is set)
    //if (hasPropertyVec3("direction")) {
    //    updatePropertyVec3("direction");
    //}
	return stopact;
}
const Body::ActNameSet &Body::getActNameSet() const
{
	return m_bodyActNameSet;
}
bool Body::isBodyAct(const std::string &actName)
{
	return m_bodyActNameSet.find(actName) != m_bodyActNameSet.end();
}
void Body::attachAct(const std::string &actName)
{
	m_bodyActNameSet.insert(actName);
}
void Body::detachAct(const std::string &actName)
{
	m_bodyActNameSet.erase(actName);
}
ActionRequest* Body::getActionPrototype(const std::string& name) 
{
	if(isBodyAct(name) && m_core_body)
	    return m_core_body->getActionPrototype( name ); 
	return NULL;
}

void Body::deleteAnimation(AnimationBase *animation)
{
	if (animation->getType() == AnimationBase::REAL)
	{
		Animation *animation_real;
		animation_real = (Animation *) animation;
		m_activity_handler.deleteAnimation(animation_real);
		m_action_handler.deleteAnimation(animation_real);
	}
	m_core_body->deleteAnimation(animation);
}
const std::string &Body::getCoreBodyName()
{
	return m_core_body_name;
}
RequestResult Body::executeAction( ActionRequest *action, bool force )
{
    ActRequestArg req_arg;

    if (action->hasPropertyFloat("speed")) {
        req_arg.setSpeed(action->getPropertyFloat("speed"));
    }

    if (action->hasPropertyFloat("angle")) {
        req_arg.setDegree(action->getPropertyFloat("angle"));
    }

	//if(!m_lastActivityName.empty())
	//	stopAction(m_lastActivityName);
	//m_lastActivityName = action->getName();
	//if(action->getName() == "ACT_Siwang")
	//	CRCore::notify(CRCore::ALWAYS)<<"executeAction "<<action->getName()<<std::endl;
	if (action->getPrioritized())
        return m_action_handler.addAction(action->getName(), &req_arg, force);
    else
        return m_activity_handler.addActivity(action->getName(), &req_arg);
}

rvrutils::FilePathContainer *Body::getFilePathContainer()
{ 
    if (!getCoreBodyContainer()) return 0L; 

    return getCoreBodyContainer()->getPathContainer(); 
}

/*------------------------------------------

 * $Source: /cvsroot/replicantbody/replicantbody/src/rbody/Body.cpp,v $
 * $Revision: 1.3 $ 
 * $Date: 2005/02/09 17:26:56 $
 * $Author: deepone $ 
 * $Locker:  $

 * Description: 

 Copyright (C) Peter Sunna, VRlab, Ume?University 2001

Created: 2001-12-12

$Log: Body.cpp,v $
Revision 1.3  2005/02/09 17:26:56  deepone
vrutils and cfgscript put into new namespaces

Revision 1.2  2005/02/07 13:35:57  deepone
char plugin

Revision 1.1.1.1  2005/02/06 06:52:59  deepone
initial sourceforge checkin

Revision 1.15  2004/02/03 12:58:04  deepone
new Property.h, Body.h and Notify.h in rvrutils

Revision 1.14  2003/11/13 10:12:27  andersb
 *** empty log message ***

 Revision 1.13  2003/11/12 10:13:35  andersb
 CreBody HasA Body instead of IsA Body. Added getBody() method.
 Moved Direction property into CreBody from Body.
 Added methods StopAction and Execute action.

 Revision 1.12  2003/11/06 08:24:37  andersb
 Removed the use of rbody::crVector3

 ----------------------------------------------------------------------------*/
