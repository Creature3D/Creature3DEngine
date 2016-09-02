// -*-c++-*-

/*
 * FBX writer for Open Scene Graph
 *
 * Copyright (C) 2009
 *
 * Writing support added 2009 by Thibault Caporal and Sukender (Benoit Neil - http://sukender.free.fr)
 *
 * The Open Scene Graph (OSG) is a cross platform C++/OpenGL library for
 * real-time rendering of large 3D photo-realistic models.
 * The OSG homepage is http://www.openscenegraph.org/
 */

#ifndef _FBX_WRITER_NODE_VISITOR_HEADER__
#define _FBX_WRITER_NODE_VISITOR_HEADER__

#include <map>
#include <set>
#include <stack>
#include <CRCore/crGeometry.h>
#include <CRCore/crMaterial.h>
#include <CRCore/crNodeVisitor.h>
#include <CRCore/crPrimitive.h>
#include <CRIOManager/crFileNameUtils.h>
#include <CRIOManager/crReaderWriter.h>
//#include <CRIOManager/ExternalFileWriter>

#if defined(_MSC_VER)
#pragma warning( disable : 4505 )
#pragma warning( default : 4996 )
#endif
#include <fbxsdk.h>

struct Triangle
{
    unsigned int t1;
    unsigned int t2;
    unsigned int t3;
    unsigned int normalIndex1;        ///< Normal index for all bindings except BIND_PER_VERTEX and BIND_OFF.
    unsigned int normalIndex2;
    unsigned int normalIndex3;
    int material;
};

struct VertexIndex
{
    VertexIndex(unsigned int vertexIndex, unsigned int drawableIndex, unsigned int normalIndex)
        : vertexIndex(vertexIndex), drawableIndex(drawableIndex), normalIndex(normalIndex)
    {}
    VertexIndex(const VertexIndex & v) : vertexIndex(v.vertexIndex), drawableIndex(v.drawableIndex), normalIndex(v.normalIndex) {}

    unsigned int vertexIndex;        ///< Index of the vertice position in the vec3 array
    unsigned int drawableIndex;
    unsigned int normalIndex;        ///< Normal index for all bindings except BIND_PER_VERTEX and BIND_OFF.

    bool operator<(const VertexIndex & v) const {
        if (drawableIndex!=v.drawableIndex) return drawableIndex<v.drawableIndex;
        return vertexIndex<v.vertexIndex;
    }
};

typedef std::vector<std::pair<Triangle, int> > ListTriangle; //the int is the drawable of the triangle
typedef std::map<VertexIndex, unsigned int> MapIndices;        ///< Map OSG indices to FBX mesh indices

namespace pluginfbx
{

///\author Capo (Thibault Caporal), Sukender (Benoit Neil)
class WriterNodeVisitor: public CRCore::crNodeVisitor
{
    public:
        WriterNodeVisitor(FbxScene* pScene,
                          FbxManager* pSdkManager,
                          const std::string& fileName,
                          const CRIOManager::crReaderWriter::Options* options,
                          const std::string& srcDirectory) :
            CRCore::crNodeVisitor(CRCore::crNodeVisitor::TRAVERSE_ALL_CHILDREN),
            _pSdkManager(pSdkManager),
            _succeedLastApply(true),
            _pScene(pScene),
            _curFbxNode(pScene->GetRootNode()),
            _currentStateSet(new CRCore::crStateSet()),
            _lastMaterialIndex(0),
            _lastMeshIndex(0),
            _options(options),
            _externalWriter(srcDirectory, CRIOManager::getFilePath(fileName), true, 0)
        {}

        ///Tell us if last crNode succeed traversing.
        bool succeedLastApply() const { return _succeedLastApply; }

        ///Set the flag _succeedLastApply to false.
        void failedApply() { _succeedLastApply = false; }

        virtual void apply(CRCore::crObject& node);
        virtual void apply(CRCore::crGroup& node);
        virtual void apply(CRCore::crMatrixTransform& node);

        void traverse (CRCore::crNode& node)
        {
            pushStateSet(node.getStateSet());
            CRCore::crNodeVisitor::traverse(node);
            popStateSet(node.getStateSet());
        }

        void pushStateSet(const CRCore::crStateSet* ss)
        {
            if (ss)
            {
                // Save our current stateset
                _stateSetStack.push(_currentStateSet.get());

                // merge with node stateset
                _currentStateSet = static_cast<CRCore::crStateSet*>(
                    _currentStateSet->clone(CRCore::crCopyOp::SHALLOW_COPY));
                _currentStateSet->merge(*ss);
            }
        }


        void popStateSet(const CRCore::crStateSet* ss)
        {
            if (ss)
            {
                // restore the previous stateset
                _currentStateSet = _stateSetStack.top();
                _stateSetStack.pop();
            }
        }

        /// Copy the texture file in current path.
        void copyTexture();
        typedef std::map<const CRCore::crImage*, std::string> ImageSet;
        typedef std::set<std::string> ImageFilenameSet;        // Sub-optimal because strings are doubled (in ImageSet). Moreover, an unordered_set (= hashset) would be more efficient (Waiting for unordered_set to be included in C++ standard ;) ).

        ///\todo Add support for 2nd texture, opacity_map, bump_map, specular_map, shininess_map, self_illum_map, reflection_map.
        class crMaterial
        {
        public:
            ///Create a KfbxMaterial and KfbxTexture from CRCore::crTexture and CRCore::crMaterial.
            crMaterial(WriterNodeVisitor&   writerNodeVisitor,
                     CRIOManager::ExternalFileWriter & externalWriter,
                     const CRCore::crStateSet* stateset,
                     const CRCore::crMaterial* mat,
                     const CRCore::crTexture*  tex,
                     FbxManager*      pSdkManager,
                     const CRIOManager::crReaderWriter::Options * options,
                     int                  index = -1);

            FbxFileTexture* getFbxTexture() const
            {
                return _fbxTexture;
            }

            FbxSurfaceMaterial* getFbxMaterial() const
            {
                return _fbxMaterial;
            }

            const CRCore::crImage* getOsgImage() const
            {
                return _osgImage;
            }

            const int getIndex() const
            {
                return _index;
            }

            void setIndex(int index)
            {
                _index = index;
            }

        private:
            FbxSurfacePhong*  _fbxMaterial;
            FbxFileTexture*   _fbxTexture;
            int                _index;///< Index in the Map
            const CRCore::crImage*  _osgImage;
        };

    protected:
        /// Compares StateSets.
        ///\todo It may be useful to compare stack of pointers (see pushStateset()) in order to keep the same number of FBX materials when doing reading and then writing without further processing.
        struct CompareStateSet
        {
            bool operator () (const CRCore::ref_ptr<const CRCore::crStateSet>& ss1, const CRCore::ref_ptr<const CRCore::crStateSet>& ss2) const
            {
				if (ss1==ss2) return 0;
				return ss1->compare(*ss2,true)<0;
            }
        };

    private:
        /**
        *  Fill the faces field of the mesh and call buildMesh().
        *  \param geo is the geode which contains the vertices and faces.
        *  \param listTriangles contain all the mesh's faces.
        *  \param texcoords tell us if we have to handle texture coordinates.
        */
        void buildFaces(const CRCore::crObject&   geo,
                        ListTriangle&       listTriangles,
                        bool                texcoords);

        /// Set the layer for texture and crMaterial in layer 0.
        void setLayerTextureAndMaterial(FbxMesh* mesh);

        /// Set Vertices, normals, and UVs
        void setControlPointAndNormalsAndUV(const CRCore::crObject& geo,
                                            MapIndices&       index_vert,
                                            bool              texcoords,
                                            FbxMesh*         fbxMesh);

        /**
        *  Create the list of faces from the geode.
        *  \param geo is the geode to study.
        *  \param listTriangles is the list to fill.
        *  \param texcoords tell us if we have to treat texture coord.
        *  \param drawable_n tell us which drawable we are building.
        */
        void createListTriangle(const CRCore::crGeometry* geo,
                                ListTriangle&        listTriangles,
                                bool&                texcoords,
                                unsigned int&        drawable_n);

        ///Store the material of the stateset in the MaterialMap.
        int processStateSet(const CRCore::crStateSet* stateset);

        typedef std::stack<CRCore::ref_ptr<CRCore::crStateSet> > StateSetStack;
        typedef std::map<CRCore::ref_ptr<const CRCore::crStateSet>, crMaterial, CompareStateSet> MaterialMap;

        ///We need this for every new crNode we create.
        FbxManager* _pSdkManager;

        ///Tell us if the last apply succeed, useful to stop going through the graph.
        bool _succeedLastApply;

        ///The current directory.
        std::string _directory;

        ///The Scene to save.
        FbxScene* _pScene;

        ///The current Fbx crNode.
        FbxNode* _curFbxNode;

        ///The Stack of different stateSet.
        StateSetStack _stateSetStack;

        ///The current stateSet.
        CRCore::ref_ptr<CRCore::crStateSet> _currentStateSet;

        ///We store the fbx Materials and Textures in this map.
        MaterialMap                         _materialMap;
        unsigned int                        _lastMaterialIndex;
        unsigned int                        _lastMeshIndex;
        const CRIOManager::crReaderWriter::Options* _options;
        CRIOManager::ExternalFileWriter           _externalWriter;
};

// end namespace pluginfbx
}

#endif // _FBX_WRITER_NODE_VISITOR_HEADER__
