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

#ifndef DXF_FILE
#define DXF_FILE 1

#include <CRCore/crGroup.h>

#include <iostream>
#include <string>

#include "dxfSectionBase.h"
#include "dxfReader.h"
#include "dxfSection.h"
#include "scene.h"
#include "codeValue.h"

class dxfFile {
public:
    dxfFile(std::string fileName) : 
                _fileName(fileName),
                _isNewSection(false)
    {}
    bool            parseFile();
    CRCore::crGroup*        dxf2CRCore();
    dxfBlock*        findBlock(std::string name);
    VariableList    getVariable(std::string var);
    
protected:

    short assign(codeValue& cv);
    std::string                 _fileName;
    bool                        _isNewSection;
    CRCore::ref_ptr<dxfReader>     _reader;
    CRCore::ref_ptr<dxfSection>    _current;
    CRCore::ref_ptr<dxfHeader>     _header;
    CRCore::ref_ptr<dxfTables>     _tables;
    CRCore::ref_ptr<dxfBlocks>     _blocks;
    CRCore::ref_ptr<dxfEntities>   _entities;
    CRCore::ref_ptr<dxfSection>    _unknown;
    CRCore::ref_ptr<scene>         _scene;
};


#endif
