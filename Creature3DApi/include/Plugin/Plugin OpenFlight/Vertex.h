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

#ifndef FLT_VERTEX_H
#define FLT_VERTEX_H 1

#include <vector>
#include <CRCore/crVector2.h>
#include <CRCore/crVector3.h>
#include <CRCore/crVector4.h>
#include <CRCore/Referenced.h>

namespace flt {

class Vertex
{
public:

    Vertex();
    Vertex(const Vertex& vertex);

    void setCoord(const CRCore::crVector3& coord);
    void setColor(const CRCore::crVector4& color);
    void setNormal(const CRCore::crVector3& normal);
    void setUV(int layer, const CRCore::crVector2& uv);

    bool validColor() const { return _validColor; }
    bool validNormal() const { return _validNormal; }
    bool validUV(int layer) const { return layer>=0 && layer<MAX_LAYERS && _validUV[layer]; }

    static const int MAX_LAYERS = 8;

    CRCore::crVector3 _coord;
    CRCore::crVector4 _color;
    CRCore::crVector3 _normal;
    CRCore::crVector2 _uv[MAX_LAYERS];

    bool _validColor;
    bool _validNormal;
    bool _validUV[MAX_LAYERS];
};


class VertexList : public CRCore::Referenced , public std::vector<Vertex>
{
public:

    VertexList() {}

    explicit VertexList(int size) :
        std::vector<Vertex>(size) {}

protected:

    virtual ~VertexList() {}
};

} // end namespace

#endif
