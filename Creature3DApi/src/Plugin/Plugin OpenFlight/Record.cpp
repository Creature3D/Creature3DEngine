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

#include <stdexcept>
#include <CRCore/crTexture2D.h>
#include <CRCore/crMatrixTransform.h>
#include <Plugin OpenFlight/Record.h>
#include <Plugin OpenFlight/Document.h>

using namespace flt;


Record::Record()
{
}

Record::~Record()
{
}

void Record::read(RecordInputStream& in, Document& document)
{
    _parent = document.getCurrentPrimaryRecord();

    // Read record body.
    readRecord(in,document);
}

void Record::readRecord(RecordInputStream& /*in*/, Document& /*document*/)
{
}

PrimaryRecord::PrimaryRecord() :
    _numberOfReplications(0)
{
}

void PrimaryRecord::read(RecordInputStream& in, Document& document)
{
    PrimaryRecord* parentPrimary = document.getTopOfLevelStack();
    PrimaryRecord* currentPrimary = document.getCurrentPrimaryRecord();

    // Finally call dispose() for primary without push, pop level pair.
    if (currentPrimary && currentPrimary!=parentPrimary)
    {
        currentPrimary->dispose(document);
    }

   // Update current primary record.
    document.setCurrentPrimaryRecord(this);

     _parent = parentPrimary;

    // Read record body.
    readRecord(in,document);
}

///////////////////////////////////////////////////////////////////////////////////
// Helper methods

// Insert matrix-tranform(s)
//
// node: node to apply transform
// matrix: transformation matrix
// numberOfReplications: zero for regular transform, number of copies if replication is used.
void flt::insertMatrixTransform(CRCore::crNode& node, const CRCore::crMatrix& matrix, int numberOfReplications)
{
    CRCore::ref_ptr<CRCore::crNode> ref = &node;
    CRCore::crNode::ParentArray parents = node.getParents();

    // Disconnect node from parents.
    for (CRCore::crNode::ParentArray::iterator itr=parents.begin();
        itr!=parents.end();
        ++itr)
    {
        (*itr)->removeChild(&node);
    }

    // Start without transformation if replication.
    CRCore::crMatrix accumulatedMatrix = (numberOfReplications > 0)? CRCore::crMatrix::identity() : matrix;

    for (int n=0; n<=numberOfReplications; n++)
    {
        // Accumulate transformation for each replication.
        CRCore::ref_ptr<CRCore::crMatrixTransform> transform = new CRCore::crMatrixTransform(accumulatedMatrix);
        transform->setDataVariance(CRCore::crBase::STATIC);

        // Add transform to parents
        for (CRCore::crNode::ParentArray::iterator itr=parents.begin();
            itr!=parents.end();
            ++itr)
        {
            (*itr)->addChild(transform.get());
        }

        // Make primary a child of matrix transform.
        transform->addChild(&node);

        // Accumulate transform if multiple replications.
        accumulatedMatrix *= matrix;
    }
}


///////////////////////////////////////////////////////////////////////////////////


CRCore::Vec3Array* flt::getOrCreateVertexArray(CRCore::crGeometry& geometry)
{
    CRCore::Vec3Array* vertices = dynamic_cast<CRCore::Vec3Array*>(geometry.getVertexArray());
    if (!vertices)
    {
        vertices = new CRCore::Vec3Array;
        geometry.setVertexArray(vertices);
    }
    return vertices;
}

CRCore::Vec3Array* flt::getOrCreateNormalArray(CRCore::crGeometry& geometry)
{
    CRCore::Vec3Array* normals = dynamic_cast<CRCore::Vec3Array*>(geometry.getNormalArray());
    if (!normals)
    {
        normals = new CRCore::Vec3Array;
        geometry.setNormalArray(normals);
    }
    return normals;
}

CRCore::Vec4Array* flt::getOrCreateColorArray(CRCore::crGeometry& geometry)
{
    CRCore::Vec4Array* colors = dynamic_cast<CRCore::Vec4Array*>(geometry.getColorArray());
    if (!colors)
    {
        colors = new CRCore::Vec4Array;
        geometry.setColorArray(colors);
    }
    return colors;
}


CRCore::Vec2Array* flt::getOrCreateTextureArray(CRCore::crGeometry& geometry, int unit)
{
    CRCore::Vec2Array* UVs = dynamic_cast<CRCore::Vec2Array*>(geometry.getTexCoordArray(unit));
    if (!UVs)
    {
        UVs = new CRCore::Vec2Array;
        geometry.setTexCoordArray(unit,UVs);
    }
    return UVs;
}
