/* dxfReader for OpenSceneGraph  Copyright (C) 2005 by GraphArchitecture ( grapharchitecture.com )
 * Programmed by Paul de Repentigny <pdr@grapharchitecture.com>
 * 
 * OpenSceneGraph is (C) 2004 Robert Osfield
 * 
 * This library is provided as-is, without support of any kind.
 *
 * Read DXF docs or OSG docs for any related questions.
 * 
 * You may contact the author if you have suggestions/corrections/enhancements.
 */

#ifndef DXF_BLOCK
#define DXF_BLOCK 1

#include <map>
#include <vector>
#include <string>

#include <CRCore/Referenced.h>
#include <CRCore/crVector3.h>
#include <CRCore/ref_ptr.h>

class dxfFile;
class codeValue;
class dxfEntity;

typedef std::vector<CRCore::ref_ptr<dxfEntity> > EntityList;

class dxfBlock : public CRCore::Referenced
{
public:
    dxfBlock() : _currentEntity(NULL) {}
    virtual ~dxfBlock() {}
    inline const std::string& getName() const { return _name; }
    virtual void assign(dxfFile* dxf, codeValue& cv);
    EntityList& getEntityList() { return _entityList; }
    const CRCore::crVector3d& getPosition() const;
    
protected:
    EntityList _entityList;
    dxfEntity* _currentEntity;
    std::string _name;
    CRCore::crVector3d _position;
};

#endif
