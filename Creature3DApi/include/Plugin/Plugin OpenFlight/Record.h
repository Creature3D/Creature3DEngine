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

#ifndef FLT_RECORD_H
#define FLT_RECORD_H 1

#include <stdexcept>
#include <string>
#include <CRCore/crArray.h>
#include <CRCore/crGeometry.h>
#include <CRCore/crObject.h>
#include <CRCore/crGroup.h>
#include <CRCore/crMatrixTransform.h>
#include <Plugin OpenFlight/Vertex.h>

namespace flt
{

class RecordInputStream;
class Document;
class PrimaryRecord;
class crMatrix;

#define META_Record(name) \
        virtual flt::Record* cloneType() const { return new name (); } \
        virtual bool isSameKindAs(const flt::Record* rec) const { return dynamic_cast<const name *>(rec)!=NULL; }
#define META_setID(imp) virtual void setID(const std::string& id) { if (imp.valid()) imp->setName(id); }
#define META_setComment(imp) virtual void setComment(const std::string& id) { if (imp.valid()) imp->addDescription(id); }
#define META_setMultitexture(imp) virtual void setMultitexture(CRCore::crStateSet& multitexture) { if (imp.valid()) imp->getOrCreateStateSet()->merge(multitexture); }
#define META_addChild(imp) virtual void addChild(CRCore::crNode& child) { if (imp.valid()) imp->addChild(&child); }
#define META_dispose(imp) virtual void dispose(Document&) { if (imp.valid() && _matrix.valid()) insertMatrixTransform(*imp,*_matrix,_numberOfReplications); }

// pure virtual base class
class Record : public CRCore::Referenced
{
public:

    Record();

    virtual Record* cloneType() const = 0;
    virtual bool isSameKindAs(const Record* rec) const = 0;
    virtual void read(RecordInputStream& in, Document& document);

    void setParent(PrimaryRecord* parent);

protected:

    virtual ~Record();

    virtual void readRecord(RecordInputStream& in, Document& document);

    CRCore::ref_ptr<PrimaryRecord> _parent;
};


class PrimaryRecord : public Record
{
public:

    PrimaryRecord();

    virtual void read(RecordInputStream& in, Document& document);
    virtual void dispose(Document& /*document*/) {}

    // Ancillary operations
    virtual void setID(const std::string& /*id*/) {}
    virtual void setComment(const std::string& /*comment*/) {}
    virtual void setMultitexture(CRCore::crStateSet& /*multitexture*/) {}
    virtual void addChild(CRCore::crNode& /*child*/) {}
    virtual void addVertex(Vertex& /*vertex*/) {}
    virtual void addVertexUV(int /*layer*/,const CRCore::crVector2& /*uv*/) {}
    virtual void addMorphVertex(Vertex& /*vertex0*/, Vertex& /*vertex100*/) {}
    virtual void setMultiSwitchValueName(unsigned int /*switchSet*/, const std::string& /*name*/) {}

    void setNumberOfReplications(int num) { _numberOfReplications = num; }
    void setMatrix(const CRCore::crMatrix& matrix) { _matrix = new CRCore::RefMatrix(matrix); }

    void setLocalVertexPool(VertexList* pool) { _localVertexPool = pool; }
    VertexList* getLocalVertexPool() { return _localVertexPool.get(); }

protected:

    virtual ~PrimaryRecord() {}

    int _numberOfReplications;
    CRCore::ref_ptr<CRCore::RefMatrix> _matrix;
    CRCore::ref_ptr<VertexList> _localVertexPool;
};


/** DummyRecord -
  */
class DummyRecord : public Record
{
    public:

        DummyRecord() {}

        META_Record(DummyRecord)

    protected:

        virtual ~DummyRecord() {}
};


void insertMatrixTransform(CRCore::crNode& node, const CRCore::crMatrix& matrix, int numberOfReplications);

CRCore::Vec3Array* getOrCreateVertexArray(CRCore::crGeometry& geometry);
CRCore::Vec3Array* getOrCreateNormalArray(CRCore::crGeometry& geometry);
CRCore::Vec4Array* getOrCreateColorArray(CRCore::crGeometry& geometry);
CRCore::Vec2Array* getOrCreateTextureArray(CRCore::crGeometry& geometry, int unit);


} // end namespace

#endif
