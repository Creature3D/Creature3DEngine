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
#include <CRIOManager/crOutput.h>
#include <CRIOManager/crRegistry.h>
#include <CRIOManager/crFileNameUtils.h>

#include <CRCore/crNotify.h>

#include <sstream>
#include <stdio.h>

using namespace std;
using namespace CRIOManager;

crOutput::crOutput()
{
    init();
}

crOutput::crOutput(const char* name) : ofstream(name)
{
    init();
    m_filename = name;
}

crOutput::~crOutput()
{
}


void crOutput::init()
{
    m_indent = 0;
    m_indentStep = 2;
    m_numIndicesPerLine = 10;
    m_pathNameHint = AS_IS;
	m_outputTextureFiles = false;
	m_textureFileNameNumber = 0;
}

void crOutput::open(const char *name)
{
    init();
    ofstream::open(name);
    m_filename = name;
}

// Comment out to avoid compile errors under new compilers, the int mode
// is now a replaced by a class to wrap the mode.  
// This method is not used right now to hopefully nobody will miss it... 
// Jan 2002.
// void crOutput::open(const char *name,int mode)
// {
//     init();
//     ofstream::open(name,mode);
//     m_filename = name;
// }

crOutput& crOutput::indent()
{
    for(int i=0;i<m_indent;++i) *this<<' ';
    return *this;
}



void crOutput::moveIn()
{
    m_indent += m_indentStep;
}


void crOutput::moveOut()
{
    m_indent -= m_indentStep;
    if (m_indent<0) m_indent=0;
}

std::string crOutput::wrapString(const std::string& str)
{
    std::string newstring;
    newstring += '"';
    for(unsigned int i=0;i<str.size();++i)
    {
    	if (str[i]=='"')
	{
	    newstring += '\\';
	    newstring += '"';
	}
	else newstring += (str[i]);
    }
    newstring += '"';
    return newstring;
}

bool crOutput::writeObject(const CRCore::crBase& obj)
{
    return crRegistry::instance()->writeObject(obj,*this);
}

bool crOutput::getUniqueIDForObject(const CRCore::crBase* obj,std::string& uniqueID)
{
    UniqueIDToLabelMapping::iterator fitr = m_objectToUniqueIDMap.find(obj);
    if (fitr != m_objectToUniqueIDMap.end())
    {
        uniqueID = (*fitr).second;
        return true;
    }
    else return false;
}


bool crOutput::createUniqueIDForObject(const CRCore::crBase* obj,std::string& uniqueID)
{
    char str[256];
    sprintf(str,"%s_%i\0",obj->className(),(unsigned int)m_objectToUniqueIDMap.size());
    uniqueID = str;
    return true;
}


bool crOutput::registerUniqueIDForObject(const CRCore::crBase* obj,std::string& uniqueID)
{
    m_objectToUniqueIDMap[obj] = uniqueID;
    return true;
}

std::string crOutput::getFileNameForOutput(const std::string& filename) const
{
    switch(m_pathNameHint)
    {
    case(FULL_PATH):
        {
            // need to think about how best to implement this first...
            CRCore::notify(CRCore::WARN)<<"Warning: crOutput::getFileNameForOutput() does not support FULL_PATH yet."<< std::endl;        
            return filename;
        }
    case(RELATIVE_PATH):
        {
            // need to think about how best to implement this as well...
            CRCore::notify(CRCore::WARN)<<"Warning: crOutput::getFileNameForOutput() does not support RELATIVE_PATH yet."<< std::endl;        
            return filename;
        }
    case(FILENAME_ONLY):
        // this one is straight forward.
        return getSimpleFileName(filename);
    case(AS_IS):
    default:
        // and this one is even more trivial.
        return filename;
    }
}

std::string crOutput::getTextureFileNameForOutput()
{
	std::string fileName = CRIOManager::getNameLessExtension(m_filename);
	if (m_textureFileNameNumber>0)
	{
		std::ostringstream o;
		o << '_' << m_textureFileNameNumber;
		fileName += o.str();
	}

	fileName += ".dds";
	++m_textureFileNameNumber;

	return fileName;
}
