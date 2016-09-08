;
// Modify Registry to store a flt::AttrData object instead of a CRCore::StateSet
// Julian Ortiz, June 18th 2003.

#include <CRCore/crNode.h>
#include <CRCore/crGroup.h>
#include <CRCore/crNotify.h>

#include <algorithm>
#include <set>

#include <Plugin flt\Record.h>
#include <Plugin flt\Input.h>
#include <Plugin flt\FltFile.h>
#include <Plugin flt\AttrData.h>
#include <Plugin flt\Registry.h>

using namespace flt;

Registry* Registry::instance()
{
    static Registry s_nodeFactory;
    return &s_nodeFactory;
}


void Registry::addPrototype(Record* rec)
{
    if (rec==0L) return;

    //CRCore::notify(CRCore::INFO) << "flt::Registry::addPrototype("<< rec->className()<<")\n";
    int op = rec->classOpcode();
    _recordProtoMap[op] = rec;
}


Record* Registry::getPrototype(const int opcode)
{
    RecordProtoMap::iterator itr = _recordProtoMap.find(opcode);
    if (itr != _recordProtoMap.end()) {
        return (*itr).second.get();
    }
    //CRCore::notify( CRCore::INFO )
    //<< "flt::Registry::getPrototype: Unkown opcode: " << opcode << "\n";

    return NULL;

}


///////////////////////////////////////////////////////////////////


void Registry::addTexture(const std::string& name, AttrData* texture)
{
    if (texture == NULL) return;
    _textureMap[name] = texture;
}


AttrData* Registry::getTexture(const std::string name)
{
    TextureMap::iterator itr = _textureMap.find(name);
    if (itr != _textureMap.end())
        return (*itr).second.get();

    return NULL;
}


void Registry::addFltFile(const std::string& name, FltFile* file)
{
    if (file == NULL) return;
    _fltFileMap[name] = file;
}


FltFile* Registry::getFltFile(const std::string& name)
{
    FltFileMap::iterator itr = _fltFileMap.find(name);
    if (itr != _fltFileMap.end())
        return (*itr).second.get();

    return NULL;
}

void Registry::clearObjectCache()
{
    _textureMap.clear();
    _fltFileMap.clear();
    
    _recordForFutureDeleteList.clear();
}
