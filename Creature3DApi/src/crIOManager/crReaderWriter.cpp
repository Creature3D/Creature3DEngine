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
#include <CRIOManager/crReaderWriter.h>
#include <CRIOManager/crArchive.h>

using namespace CRIOManager;

CRCore::crBase* crReaderWriter::ReadResult::getObject() { return m_object.get(); }
CRCore::crImage* crReaderWriter::ReadResult::getImage() { return dynamic_cast<CRCore::crImage*>(m_object.get()); }
CRCore::crHeightField* crReaderWriter::ReadResult::getHeightField() { return dynamic_cast<CRCore::crHeightField*>(m_object.get()); }
CRCore::crNode* crReaderWriter::ReadResult::getNode() { return dynamic_cast<CRCore::crNode*>(m_object.get()); }
CRIOManager::crArchive* crReaderWriter::ReadResult::getArchive() { return dynamic_cast<CRIOManager::crArchive*>(m_object.get()); }

CRCore::crBase* crReaderWriter::ReadResult::takeObject() { CRCore::crBase* obj = m_object.get(); if (obj) { obj->ref(); m_object=NULL; obj->unref_nodelete(); } return obj; }
CRCore::crImage* crReaderWriter::ReadResult::takeImage() { CRCore::crImage* image=dynamic_cast<CRCore::crImage*>(m_object.get()); if (image) { image->ref(); m_object=NULL; image->unref_nodelete(); } return image; }
CRCore::crHeightField* crReaderWriter::ReadResult::takeHeightField() { CRCore::crHeightField* hf=dynamic_cast<CRCore::crHeightField*>(m_object.get()); if (hf) { hf->ref(); m_object=NULL; hf->unref_nodelete(); } return hf; }
CRCore::crNode* crReaderWriter::ReadResult::takeNode() { CRCore::crNode* node=dynamic_cast<CRCore::crNode*>(m_object.get()); if (node) { node->ref(); m_object=NULL; node->unref_nodelete(); } return node; }
CRIOManager::crArchive* crReaderWriter::ReadResult::takeArchive() { CRIOManager::crArchive* archive=dynamic_cast<CRIOManager::crArchive*>(m_object.get()); if (archive) { archive->ref(); m_object=NULL; archive->unref_nodelete(); } return archive; }

crReaderWriter::~crReaderWriter()
{
}
