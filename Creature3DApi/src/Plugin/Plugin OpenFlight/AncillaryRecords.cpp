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

#include <CRCore/crMatrixTransform.h>
#include <CRCore/crTexture2D.h>
#include <CRCore/crTexEnv.h>

#include <Plugin OpenFlight/Registry.h>
#include <Plugin OpenFlight/Document.h>
#include <Plugin OpenFlight/RecordInputStream.h>

namespace flt {

/** Comment -
  */
class Comment : public Record
{
    public:

        Comment() {}

        META_Record(Comment)

    protected:

        virtual ~Comment() {}

        virtual void readRecord(RecordInputStream& in, Document& /*document*/)
        {
            std::streamsize size = in.getRecordSize();
            std::string commentfield = in.readString(size-4);

            if (_parent.valid())
            {
#if 0
                _parent->setComment(commentfield);
#else
                unsigned int front_of_line = 0;
                unsigned int end_of_line = 0;
                while (end_of_line<commentfield.size())
                {
                    if (commentfield[end_of_line]=='\r')
                    {
                        _parent->setComment( std::string( commentfield, front_of_line, end_of_line-front_of_line) );

                        if (end_of_line+1<commentfield.size() &&
                            commentfield[end_of_line+1]=='\n') ++end_of_line;

                        ++end_of_line;
                        front_of_line = end_of_line;
                    }
                    else if (commentfield[end_of_line]=='\n')
                    {
                        _parent->setComment( std::string( commentfield, front_of_line, end_of_line-front_of_line) );
                        ++end_of_line;
                        front_of_line = end_of_line;
                    }
                    else ++end_of_line;
                }
                if (front_of_line<end_of_line)
                {
                    _parent->setComment( std::string( commentfield, front_of_line, end_of_line-front_of_line) );
                }

            }
#endif

        }
};

REGISTER_FLTRECORD(Comment, COMMENT_OP)



/** LongID -
  */
class LongID : public Record
{
    public:

        LongID() {}

        META_Record(LongID)

    protected:

        virtual ~LongID() {}

        virtual void readRecord(RecordInputStream& in, Document& /*document*/)
        {
            std::streamsize size = in.getRecordSize();
            std::string id = in.readString(size-4);

            if (_parent.valid())
                _parent->setID(id);
        }
};

REGISTER_FLTRECORD(LongID, LONG_ID_OP)



/** crMatrix -
  */
class crMatrix : public Record
{
    public:

        crMatrix() {}

        META_Record(crMatrix)

    protected:

        virtual ~crMatrix() {}

        virtual void readRecord(RecordInputStream& in, Document& document)
        {
            CRCore::crMatrix matrix;
            for (int i=0; i<4; ++i)
            {
                for (int j=0; j<4; ++j)
                {
                    matrix(i,j) = in.readFloat32();
                }
            }

            // scale position.
            CRCore::crVector3 pos = matrix.getTrans();
            matrix *= CRCore::crMatrix::translate(-pos);
            pos *= (float)document.unitScale();
            matrix *= CRCore::crMatrix::translate(pos);

            if (_parent.valid())
                _parent->setMatrix(matrix);
        }
};

REGISTER_FLTRECORD(crMatrix, MATRIX_OP)



/** Multitexture -
  */
class Multitexture : public Record
{
    public:

        Multitexture() {}

        META_Record(Multitexture)

        // Effect
        enum EffectMode
        {
            TEXTURE_ENVIRONMENT = 0,
            BUMP_MAP = 1
        };

    protected:

        virtual ~Multitexture() {}

        virtual void readRecord(RecordInputStream& in, Document& document)
        {
            CRCore::ref_ptr<CRCore::crStateSet> stateset = new CRCore::crStateSet;

            uint32 mask = in.readUInt32();
            for (int layer=1; layer<8; layer++)
            {
                uint32 layerBit = 0x80000000u >> (layer-1);
                if (mask & layerBit)
                {
                    int16 textureIndex = in.readInt16();
                    int16 effect = in.readInt16();
                    /*int16 mappingIndex =*/ in.readInt16();
                    /*uint16 data =*/ in.readUInt16();

                    CRCore::ref_ptr<CRCore::crStateSet> texturePoolStateset = document.getOrCreateTexturePool()->get(textureIndex);
                    if (stateset.valid() && texturePoolStateset.valid())
                    {
                        // Apply texture from texture pool.
                        CRCore::crTexture* texture = dynamic_cast<CRCore::crTexture*>(texturePoolStateset->getTextureAttribute(0,CRCore::crStateAttribute::TEXTURE));
                        if (texture)
                            stateset->setTextureAttributeAndModes(layer,texture,CRCore::crStateAttribute::ON);

                        // Apply texture environment
                        switch (effect)
                        {
                        case TEXTURE_ENVIRONMENT:
                            {
                                // Use texture environment setting from .attr file.
                                CRCore::crTexEnv* texenv = dynamic_cast<CRCore::crTexEnv*>(texturePoolStateset->getTextureAttribute(0,CRCore::crStateAttribute::TEXENV));
                                if (texenv)
                                    stateset->setTextureAttribute(layer,texenv);
                            }
                            break;
                        case BUMP_MAP:
                            {
                                // Dot3 bumpmap
                                //CRCore::TexEnvCombine* texEnvCombine = new CRCore::TexEnvCombine;
                                //texEnvCombine->setCombine_RGB(CRCore::TexEnvCombine::DOT3_RGB);
                                //texEnvCombine->setSource0_RGB(CRCore::TexEnvCombine::PRIMARY_COLOR);
                                //texEnvCombine->setSource1_RGB(CRCore::TexEnvCombine::TEXTURE);
                                //stateset->setTextureAttribute(layer,texEnvCombine);
                            }
                            break;
                        }
                    }
                }
            }

            if (_parent.valid())
                _parent->setMultitexture(*stateset);
        }
};

REGISTER_FLTRECORD(Multitexture, MULTITEXTURE_OP)



/** UVList - crTexture coordinates used with multitexture.
    UVList is an ancillary to VertexList.
  */
class UVList : public Record
{
    public:

        UVList() {}

        META_Record(UVList)

    protected:

        virtual ~UVList() {}

        // count number of 1's in mask.
        int bitCount(uint32 mask)
        {
            int count = 0;
            while (mask)
            {
                if (mask & 0x0001)
                    ++count;
                mask >>= 1;
            }
            return count;
        }

        virtual void readRecord(RecordInputStream& in, Document& /*document*/)
        {
            uint32 mask = in.readUInt32(0);

            int numLayers = bitCount(mask);
            int numVertices = (in.getRecordSize()-8) / (8 * numLayers);
            for (int n=0; n < numVertices; ++n)
            {
                for (unsigned int layer=1; layer<8; layer++)
                {
                    uint32 layerBit = 0x80000000u >> (layer-1);
                    if (mask & layerBit)
                    {
                        float32    u = in.readFloat32();
                        float32    v = in.readFloat32();

                        // Add texture coordinates to geometry.
                        if (_parent.valid())
                            _parent->addVertexUV(layer,CRCore::crVector2(u,v));
                    }
                }
            }
        }
};

REGISTER_FLTRECORD(UVList, UV_LIST_OP)


/** Replicate -
  */
class Replicate : public Record
{
    public:

        Replicate() {}

        META_Record(Replicate)

    protected:

        virtual ~Replicate() {}

        virtual void readRecord(RecordInputStream& in, Document& /*document*/)
        {
            int16 replicate = in.readInt16();

            if (_parent.valid())
                _parent->setNumberOfReplications((int)replicate);
        }
};

REGISTER_FLTRECORD(Replicate, REPLICATE_OP)


/** IndexedString -
  */
class IndexedString : public Record
{
    public:

        IndexedString() {}

        META_Record(IndexedString)

    protected:

        virtual ~IndexedString() {}

        virtual void readRecord(RecordInputStream& in, Document& /*document*/)
        {
            std::streamsize size = in.getRecordSize();
            uint32 index = in.readUInt32();
            std::string name = in.readString(size-8);

            if (_parent.valid())
                _parent->setMultiSwitchValueName(index, name);
        }
};

REGISTER_FLTRECORD(IndexedString, INDEXED_STRING_OP)


// Prevent "unknown record" message for the following ancillary records:
REGISTER_FLTRECORD(DummyRecord, OLD_TRANSLATE2_OP)
REGISTER_FLTRECORD(DummyRecord, OLD_ROTATE_ABOUT_POINT_OP)
REGISTER_FLTRECORD(DummyRecord, OLD_ROTATE_ABOUT_EDGE_OP)
REGISTER_FLTRECORD(DummyRecord, OLD_SCALE_OP)
REGISTER_FLTRECORD(DummyRecord, OLD_TRANSLATE_OP)
REGISTER_FLTRECORD(DummyRecord, OLD_NONUNIFORM_SCALE_OP)
REGISTER_FLTRECORD(DummyRecord, OLD_ROTATE_ABOUT_POINT2_OP)
REGISTER_FLTRECORD(DummyRecord, OLD_ROTATE_SCALE_TO_POINT_OP)
REGISTER_FLTRECORD(DummyRecord, OLD_PUT_TRANSFORM_OP)
REGISTER_FLTRECORD(DummyRecord, OLD_BOUNDING_BOX_OP)
REGISTER_FLTRECORD(DummyRecord, ROAD_ZONE_OP)
REGISTER_FLTRECORD(DummyRecord, ROTATE_ABOUT_EDGE_OP)
REGISTER_FLTRECORD(DummyRecord, TRANSLATE_OP)
REGISTER_FLTRECORD(DummyRecord, NONUNIFORM_SCALE_OP)
REGISTER_FLTRECORD(DummyRecord, ROTATE_ABOUT_POINT_OP)
REGISTER_FLTRECORD(DummyRecord, ROTATE_SCALE_TO_POINT_OP)
REGISTER_FLTRECORD(DummyRecord, PUT_TRANSFORM_OP)
REGISTER_FLTRECORD(DummyRecord, GENERAL_MATRIX_OP)
REGISTER_FLTRECORD(DummyRecord, VECTOR_OP)
REGISTER_FLTRECORD(DummyRecord, BOUNDING_BOX_OP)
REGISTER_FLTRECORD(DummyRecord, BOUNDING_SPHERE_OP)
REGISTER_FLTRECORD(DummyRecord, BOUNDING_CYLINDER_OP)
REGISTER_FLTRECORD(DummyRecord, BOUNDING_CONVEX_HULL_OP)
REGISTER_FLTRECORD(DummyRecord, BOUNDING_HISTOGRAM)
REGISTER_FLTRECORD(DummyRecord, BOUNDING_VOLUME_CENTER_OP)
REGISTER_FLTRECORD(DummyRecord, BOUNDING_VOLUME_ORIENTATION_OP)
REGISTER_FLTRECORD(DummyRecord, HISTOGRAM_BOUNDING_VOLUME_OP)


} // end namespace

