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
#ifndef CRIOMANAGER_INPUT
#define CRIOMANAGER_INPUT 1

#include <CRCore/crImage.h>
#include <CRCore/crNode.h>
#include <CRCore/crDrawable.h>
#include <CRCore/crStateAttribute.h>

#include <CRIOManager/crFieldReaderIterator.h>
#include <CRIOManager/crReaderWriter.h>

#include <map>
#include <string>

namespace CRIOManager {

struct basic_type_wrapper;

/** Class for managing the reading of ASCII .CRCore files.*/
class CRIOMANAGER_EXPORT crInput : public crFieldReaderIterator
{
    public:

        crInput();
        virtual ~crInput();
        
        void setOptions(const crReaderWriter::Options* options) { m_options = options; }
        const crReaderWriter::Options* getOptions() const { return m_options.get(); }

        virtual CRCore::crBase*         readObjectOfType(const CRCore::crBase& compObj);
        virtual CRCore::crBase*         readObjectOfType(const basic_type_wrapper &btw);

        virtual CRCore::crBase*         readObject();
        virtual CRCore::crImage*          readImage();
        virtual CRCore::crDrawable*       readDrawable();
        virtual CRCore::crStateAttribute* readStateAttribute();
        virtual CRCore::crUniform*        readUniform();
        virtual CRCore::crNode*           readNode();

        virtual CRCore::crBase*         readObject(const std::string& fileName);
        virtual CRCore::crImage*          readImage(const std::string& fileName);
        virtual CRCore::crNode*           readNode(const std::string& fileName);
        
        virtual CRCore::crBase*         getObjectForUniqueID(const std::string& uniqueID);
        virtual void                 regisiterUniqueIDForObject(const std::string& uniqueID,CRCore::crBase* obj);

    private:
    
        typedef std::map< std::string, CRCore::ref_ptr<CRCore::crBase> > UniqueIDToObjectMapping;
        UniqueIDToObjectMapping m_uniqueIDToObjectMap;
        
        CRCore::ref_ptr<const crReaderWriter::Options> m_options;

};

}

#endif                                            // __SG_INPUT_H
