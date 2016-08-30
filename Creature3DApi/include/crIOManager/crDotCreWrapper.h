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
#ifndef CRIOMANAGER_DOTCREWRAPPER
#define CRIOMANAGER_DOTCREWRAPPER 1

#include <CRCore/ref_ptr.h>
#include <CRCore/crBase.h>

#include <CRIOManager/crInput.h>
#include <CRIOManager/crOutput.h>

#include <string>
#include <vector>
//#include <stdlib.h>

namespace CRIOManager {


/** Wrapper class for specifying read and write functions for extending
  * the .CRCore file format. */ 
class CRIOMANAGER_EXPORT crDotCreWrapper : public CRCore::Referenced
{
    public:

        typedef std::vector<std::string> Associates;
        typedef bool (*ReadFunc)(CRCore::crBase&,CRIOManager::crInput&);
        typedef bool (*WriteFunc)(const CRCore::crBase&,CRIOManager::crOutput&);
        
        enum ReadWriteMode
        {
            READ_AND_WRITE,
            READ_ONLY
        };

        crDotCreWrapper(CRCore::crBase* proto,
                      const std::string& name,
                      const std::string& associates,
                      ReadFunc readFunc,
                      WriteFunc writeFunc,
                      ReadWriteMode readWriteMode=READ_AND_WRITE);
                      

        inline const CRCore::crBase* getPrototype() const { return m_prototype.get(); }
        inline const std::string& getName() const      { return m_name; }
        inline const Associates& getAssociates() const { return m_associates; }
        inline ReadFunc getReadFunc() const            { return m_readFunc; }
        inline WriteFunc getWriteFunc() const          { return m_writeFunc; }
        inline ReadWriteMode getReadWriteMode() const  { return m_readWriteMode; }

    protected:

        /// protected to prevent inappropriate creation of wrappers.
        crDotCreWrapper() {}

        /// protected to prevent inappropriate creation of wrappers.
        crDotCreWrapper(crDotCreWrapper&):CRCore::Referenced() {}

        /// protected to prevent wrapper being created on stack.
        virtual ~crDotCreWrapper() {}

        CRCore::ref_ptr<CRCore::crBase>   m_prototype;
        
        std::string                 m_name;
        Associates                  m_associates;
        
        ReadFunc                    m_readFunc;
        WriteFunc                   m_writeFunc;

        ReadWriteMode               m_readWriteMode;
};

}

#endif 
