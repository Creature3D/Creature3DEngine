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

//
// OpenFlight?loader for OpenSceneGraph
//
//  Copyright (C) 2005-2007  Brede Johansen
//

#ifndef FLT_REGISTRY_H
#define FLT_REGISTRY_H 1

#include <queue>
#include <map>
#include <CRCore/ref_ptr.h>
#include <Plugin OpenFlight/Opcodes.h>
#include <Plugin OpenFlight/Record.h>

namespace flt {

class crRegistry : public CRCore::Referenced
{
    public:

        ~crRegistry();
        static crRegistry* instance();

        // Record prototypes
        void addPrototype(int opcode, Record* prototype);
        Record* getPrototype(int opcode);

        // External read queue
        typedef std::pair<std::string, CRCore::crGroup*> FilenameParentPair; // ExtNameNodePair;
        typedef std::queue<FilenameParentPair> ExternalQueue;

        inline ExternalQueue& getExternalReadQueue() { return _externalReadQueue; }
        void addToExternalReadQueue(const std::string& filename, CRCore::crGroup* parent);

        // Local cache
        void addExternalToLocalCache(const std::string& filename, CRCore::crNode* node);
        CRCore::crNode* getExternalFromLocalCache(const std::string& filename);
        void addTextureToLocalCache(const std::string& filename, CRCore::crStateSet* stateset);
        CRCore::crStateSet* getTextureFromLocalCache(const std::string& filename);
        void clearLocalCache();

    protected:

        crRegistry();

        typedef std::map<int, CRCore::ref_ptr<Record> > RecordProtoMap;
        RecordProtoMap     _recordProtoMap;

        ExternalQueue      _externalReadQueue;

        // External cache
        typedef std::map<std::string, CRCore::ref_ptr<CRCore::crNode> > ExternalCacheMap;
        ExternalCacheMap _externalCacheMap;

        // crTexture cache
        typedef std::map<std::string, CRCore::ref_ptr<CRCore::crStateSet> > TextureCacheMap;
        TextureCacheMap    _textureCacheMap;
};

inline void crRegistry::addToExternalReadQueue(const std::string& filename, CRCore::crGroup* parent)
{
    _externalReadQueue.push( FilenameParentPair(filename,parent) );
}

inline void crRegistry::addExternalToLocalCache(const std::string& filename, CRCore::crNode* node)
{
    _externalCacheMap[filename] = node;
}

inline CRCore::crNode* crRegistry::getExternalFromLocalCache(const std::string& filename)
{
    ExternalCacheMap::iterator itr = _externalCacheMap.find(filename);
    if (itr != _externalCacheMap.end())
        return (*itr).second.get();
    return NULL;
}

inline void crRegistry::addTextureToLocalCache(const std::string& filename, CRCore::crStateSet* stateset)
{
    _textureCacheMap[filename] = stateset;
}

inline CRCore::crStateSet* crRegistry::getTextureFromLocalCache(const std::string& filename)
{
    TextureCacheMap::iterator itr = _textureCacheMap.find(filename);
    if (itr != _textureCacheMap.end())
        return (*itr).second.get();
    return NULL;
}

inline void crRegistry::clearLocalCache()
{
    _externalCacheMap.clear();
    _textureCacheMap.clear();
}

/** Proxy class for automatic registration of reader/writers with the crRegistry.*/
template<class T>
class RegisterRecordProxy
{
    public:

        explicit RegisterRecordProxy(int opcode)
        {
            crRegistry::instance()->addPrototype(opcode,new T);
        }

        ~RegisterRecordProxy() {}
};

//////////////////////////////////////////////////////////////////////////

extern "C"
{
    typedef void (* CRecordFunction) (void);
}

struct RecordFunctionProxy
{
    RecordFunctionProxy(CRecordFunction function) { (function)(); }
};

#define USE_FLTRECORD(recname, opcode) \
    extern "C" void osgfltrec_##recname_##opcode(void); \
    static flt::RecordFunctionProxy proxy_fltrecord_##recname_##opcode(osgfltrec_##recname_##opcode);

#define REGISTER_FLTRECORD(recname, opcode) \
    extern "C" void osgfltrec_##recname_##opcode(void) {} \
    static flt::RegisterRecordProxy<recname> g_proxy_fltrecord_##recname_##opcode(opcode);


} // end namespace

#endif
