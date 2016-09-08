/* Creature3D - Online Game Engine, Copyright (C) 2005 吴财华(26756325@qq.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
*/

#include <Plugin cre/crException.h>
#include <Plugin cre/crMatterObject.h>
#include <Plugin cre/crGroup.h>
#include <Plugin cre/crMatrixTransform.h>
#include <CRCore/crEventCallbackManager.h>
#include <CRCore/crShapeDrawable.h>
using namespace cre;

void crMatterObject::write(crDataOutputStream* out){
    out->writeInt(CREMATTEROBJECT);

    CRCore::crMatrixTransform*  tx = dynamic_cast<CRCore::crMatrixTransform*>(this);
	tx->setMatrix(CRCore::crMatrix::identity());
    if(tx){
        ((cre::crMatrixTransform*)(tx))->write(out);
    }
    else
        throw crException("crMatterObject::write(): Could not cast this CRPhysics::crMatterObject to an CRCore::crMatrixTransform.");

	out->writeFloat(getMass());
	out->writeFloat(m_density);
	out->writeVec3(getBarycenter());
	out->writeMatrixf(getInertiaMass());

	out->writeVec3(getInitPosition());
	
	out->writeInt(m_eventMap.size());
	for( EventMap::iterator itr = m_eventMap.begin();
		 itr != m_eventMap.end();
		 ++itr )
	{
		out->writeInt64(itr->first);
		out->writeString(itr->second->_name());
	}
	//out->writeBool(m_collideCallback.valid());
	//if(m_collideCallback.valid())
	//{
	//	out->writeCollideCallback(m_collideCallback.get());
	//}
}

void crMatterObject::read(crDataInputStream* in){
    int id = in->peekInt();
    if(id == CREMATTEROBJECT){
        id = in->readInt();
        CRCore::crMatrixTransform*  tx = dynamic_cast<CRCore::crMatrixTransform*>(this);
        if(tx){
            ((cre::crMatrixTransform*)(tx))->read(in);
        }
        else
            throw crException("crMatterObject::read(): Could not cast this CRPhysics::crMatterObject to an CRCore::crGroup.");

		tx->setMatrix(CRCore::crMatrix::identity());

		setMass(in->readFloat());
		m_density = in->readFloat();
		setBarycenter(in->readVec3());
		setInertiaMass(in->readMatrixf());

        setInitPosition(in->readVec3());
        
		int size = in->readInt();
		CRCore::ref_ptr<CRCore::crEventCallback> event;
		_crInt64 kbmsg;
		std::string eventStr;
		for(int i = 0; i<size; ++i)
		{
			kbmsg = in->readInt64();
			eventStr = in->readString();
			event = CRCore::crEventCallbackManager::getInstance()->getEventCallback(eventStr);
			if(!event) 
			{
				CRCore::notify(CRCore::NOTICE)<<"crMatterObject::read(): 未找到EventFunction: "<<eventStr<<std::endl;
				continue;
			}
            insertEvent(kbmsg,event.get());
		}
		//if(in->readBool())
		//{
		//	m_collideCallback = in->readCollideCallback();
		//}
    }
    else{
        throw crException("crMatterObject::read(): Expected crMatterObject identification");
    }
}

void crBoxMatterObject::write(crDataOutputStream* out){
    out->writeInt(CREBOXMATTEROBJECT);

    CRPhysics::crMatterObject*  tx = dynamic_cast<CRPhysics::crMatterObject*>(this);
    if(tx){
        ((cre::crMatterObject*)(tx))->write(out);
    }
    else
        throw crException("crBoxMatterObject::write(): Could not cast this CRPhysics::crBoxMatterObject to an CRPhysics::crMatterObject.");
}

void crBoxMatterObject::read(crDataInputStream* in){
    int id = in->peekInt();
    if(id == CREBOXMATTEROBJECT)
	{
        id = in->readInt();
        CRPhysics::crMatterObject*  tx = dynamic_cast<CRPhysics::crMatterObject*>(this);
        if(tx){
            ((cre::crMatterObject*)(tx))->read(in);
        }
        else
            throw crException("crBoxMatterObject::read(): Could not cast this CRPhysics::crBoxMatterObject to an CRPhysics::crMatterObject.");
    }
    else{
        throw crException("crBoxMatterObject::read(): Expected crBoxMatterObject identification");
    }
}

void crSphereMatterObject::write(crDataOutputStream* out){
    out->writeInt(CRESPHEREMATTEROBJECT);

    CRPhysics::crMatterObject*  tx = dynamic_cast<CRPhysics::crMatterObject*>(this);
    if(tx){
        ((cre::crMatterObject*)(tx))->write(out);
    }
    else
        throw crException("crBoxMatterObject::write(): Could not cast this CRPhysics::crBoxMatterObject to an CRPhysics::crMatterObject.");
}

void crSphereMatterObject::read(crDataInputStream* in){
    int id = in->peekInt();
    if(id == CRESPHEREMATTEROBJECT)
	{
        id = in->readInt();
        CRPhysics::crMatterObject*  tx = dynamic_cast<CRPhysics::crMatterObject*>(this);
        if(tx){
            ((cre::crMatterObject*)(tx))->read(in);
        }
        else
            throw crException("crSphereMatterObject::read(): Could not cast this CRPhysics::crSphereMatterObject to an CRPhysics::crMatterObject.");

    }
    else{
        throw crException("crSphereMatterObject::read(): Expected crSphereMatterObject identification");
    }
}

void crCCylinderMatterObject::write(crDataOutputStream* out){
	out->writeInt(CRECCYLINDERMATTEROBJECT);

	CRPhysics::crMatterObject*  tx = dynamic_cast<CRPhysics::crMatterObject*>(this);
	if(tx){
		((cre::crMatterObject*)(tx))->write(out);
	}
	else
		throw crException("crCCylinderMatterObject::write(): Could not cast this CRPhysics::crCCylinderMatterObject to an CRPhysics::crMatterObject.");
}

void crCCylinderMatterObject::read(crDataInputStream* in){
	int id = in->peekInt();
	if(id == CRECCYLINDERMATTEROBJECT)
	{
		id = in->readInt();
		CRPhysics::crMatterObject*  tx = dynamic_cast<CRPhysics::crMatterObject*>(this);
		if(tx){
			((cre::crMatterObject*)(tx))->read(in);
		}
		else
			throw crException("crCCylinderMatterObject::read(): Could not cast this CRPhysics::crCCylinderMatterObject to an CRPhysics::crMatterObject.");

	}
	else{
		throw crException("crCCylinderMatterObject::read(): Expected crCCylinderMatterObject identification");
	}
}

void crMeshMatterObject::write(crDataOutputStream* out){
    out->writeInt(CREMESHMATTEROBJECT);

    CRPhysics::crMatterObject*  tx = dynamic_cast<CRPhysics::crMatterObject*>(this);
    if(tx){
        ((cre::crMatterObject*)(tx))->write(out);
    }
    else
        throw crException("crBoxMatterObject::write(): Could not cast this CRPhysics::crBoxMatterObject to an CRPhysics::crMatterObject.");
}

void crMeshMatterObject::read(crDataInputStream* in){
    int id = in->peekInt();
    if(id == CREMESHMATTEROBJECT)
	{
        id = in->readInt();
        CRPhysics::crMatterObject*  tx = dynamic_cast<CRPhysics::crMatterObject*>(this);
        if(tx){
            ((cre::crMatterObject*)(tx))->read(in);
        }
        else
            throw crException("crBoxMatterObject::read(): Could not cast this CRPhysics::crBoxMatterObject to an CRPhysics::crMatterObject.");

    }
    else{
        throw crException("crBoxMatterObject::read(): Expected crBoxMatterObject identification");
    }
}

//void crViewMatterObject::write(crDataOutputStream* out){
//	out->writeInt(CREVIEWMATTEROBJECT);
//
//	CRPhysics::crMatterObject*  tx = dynamic_cast<CRPhysics::crMatterObject*>(this);
//	if(tx){
//		((cre::crMatterObject*)(tx))->write(out);
//	}
//	else
//		throw crException("crBoxMatterObject::write(): Could not cast this CRPhysics::crBoxMatterObject to an CRPhysics::crMatterObject.");
//
//	out->writeFloat(getMass());
//	out->writeFloat(m_density);
//	out->writeVec3(getBarycenter());
//	out->writeMatrixf(getInertiaMass());
//
//	out->writeVec3(getInitPosition());
//
//	out->writeBool(m_groundDetect);
//	out->writeUInt(m_currentBodyPhysicsType);
/////////////////////
//	out->writeVec2(m_move_strafeSpeed);
//	out->writeVec3(m_lastPos);
//	out->writeBool(m_groundDetect);
//	out->writeFloat(m_time_in_air);
//	out->writeDouble(m_previousFrameTime);
//	out->writeDouble(m_frameInterval);
//	out->writeFloat(m_physicsSwapInterval);
//    out->writeUInt(m_currentBodyPhysicsType);
//
//	int size = m_weaponMatterObjects.size();
//    out->writeInt(size);
//	for(int i = 0; i<size; ++i)
//	{
//		m_weaponMatterObjects[i]->write(out);
//	}
//
//	////weapon
//	WeaponMatterObjectMap m_weaponMatterObjects;
//	CRCore::ref_ptr<crWeaponMatterObject> m_currentWeaponMatterObject;
//
//	/////关于body的信息
//	int m_hp;//生命值
//	int m_defend;//防御力
//	int m_team;//0表示中立，>0表示队友，<0表示敌人 
//	int m_killedCount;
//	int m_deathCount;
//	///////////
//	////radar
//	CRCore::ref_ptr<crRadarMatterObject> m_radarMatterObject;
//
//	double      m_viewPitch;
//	float       m_viewOffset;
//	float       m_viewHeight;
//	//CRCore::crMatrix m_viewOffsetMatrix;
//
//	bool        m_cameraBind;
//	bool        m_deadFlag;
//	bool        m_requireRenascence;
//	bool        m_hideBody;
//
//}
//
//void crViewMatterObject::read(crDataInputStream* in){
//	int id = in->peekInt();
//	if(id == CREVIEWMATTEROBJECT)
//	{
//		id = in->readInt();
//		CRPhysics::crMatterObject*  tx = dynamic_cast<CRPhysics::crMatterObject*>(this);
//		if(tx){
//			((cre::crMatterObject*)(tx))->read(in);
//		}
//		else
//			throw crException("crBoxMatterObject::read(): Could not cast this CRPhysics::crBoxMatterObject to an CRPhysics::crMatterObject.");
//
//	}
//	else{
//		throw crException("crBoxMatterObject::read(): Expected crBoxMatterObject identification");
//	}
//}
//
//void crCarMatterObject::write(crDataOutputStream* out){
//	out->writeInt(CRECARMATTEROBJECT);
//
//	CRPhysics::crViewMatterObject*  tx = dynamic_cast<CRPhysics::crViewMatterObject*>(this);
//	if(tx){
//		((cre::crViewMatterObject*)(tx))->write(out);
//	}
//	else
//		throw crException("crBoxMatterObject::write(): Could not cast this CRPhysics::crBoxMatterObject to an CRPhysics::crMatterObject.");
//}
//
//void crCarMatterObject::read(crDataInputStream* in){
//	int id = in->peekInt();
//	if(id == CRECARMATTEROBJECT)
//	{
//		id = in->readInt();
//		CRPhysics::crViewMatterObject*  tx = dynamic_cast<CRPhysics::crViewMatterObject*>(this);
//		if(tx){
//			((cre::crViewMatterObject*)(tx))->read(in);
//		}
//		else
//			throw crException("crBoxMatterObject::read(): Could not cast this CRPhysics::crBoxMatterObject to an CRPhysics::crMatterObject.");
//
//	}
//	else{
//		throw crException("crBoxMatterObject::read(): Expected crBoxMatterObject identification");
//	}
//}

//void crRayMatterObject::write(crDataOutputStream* out){
//    out->writeInt(CRERAYMATTEROBJECT);
//
//    CRPhysics::crMatterObject*  tx = dynamic_cast<CRPhysics::crMatterObject*>(this);
//    if(tx){
//        ((cre::crMatterObject*)(tx))->write(out);
//    }
//    else
//        throw crException("crRayMatterObject::write(): Could not cast this CRPhysics::crRayMatterObject to an CRPhysics::crMatterObject.");
//
//	out->writeFloat(m_length);
//	out->writeVec3(m_offset);
//
//	out->writeBool(m_hostMatterObject!=0);
//	if(m_hostMatterObject)
//	{
//        out->writeNode(m_hostMatterObject);
//	}
//
//}
//
//void crRayMatterObject::read(crDataInputStream* in){
//    int id = in->peekInt();
//    if(id == CRERAYMATTEROBJECT)
//	{
//        id = in->readInt();
//        CRPhysics::crMatterObject*  tx = dynamic_cast<CRPhysics::crMatterObject*>(this);
//        if(tx){
//            ((cre::crMatterObject*)(tx))->read(in);
//        }
//        else
//            throw crException("crRayMatterObject::read(): Could not cast this CRPhysics::crRayMatterObject to an CRPhysics::crMatterObject.");
//
//	    m_length = in->readFloat();
//		m_offset = in->readVec3();
//
//		if(in->readBool())
//		{
//            m_hostMatterObject = dynamic_cast<CRPhysics::crMatterObject*>(in->readNode());
//		}
//    }
//    else{
//        throw crException("crRayMatterObject::read(): Expected crRayMatterObject identification");
//    }
//}
