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
//Modified by Wucaihua
#ifndef CRTERRAIN_CRGEOMETRYTECHNIQUE
#define CRTERRAIN_CRGEOMETRYTECHNIQUE 1

#include <CRCore/crMatrixTransform.h>
#include <CRCore/crObject.h>
#include <CRCore/crGeometry.h>

#include <CRTerrain/crTerrainTechnique.h>
#include <CRTerrain/crLocator.h>

namespace CRTerrain {

class CRTERRAIN_EXPORT crGeometryTechnique : public crTerrainTechnique
{
    public:

        crGeometryTechnique();

        /** Copy constructor using crCopyOp to manage deep vs shallow copy.*/
        crGeometryTechnique(const crGeometryTechnique&,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);
        
        META_Base(CRTerrain, crGeometryTechnique);
        
        virtual void init();
        
        virtual crLocator* computeMasterLocator();
        
        virtual CRCore::crVector3d computeCenterModel(crLocator* masterLocator);
        
        virtual void generateGeometry(crLocator* masterLocator, const CRCore::crVector3d& centerModel);
		virtual void generateGridGeometry();

        virtual void applyColorLayers();
        
        virtual void applyTransparency();
        
        virtual void smoothGeometry();

        //virtual void update(CRUtil::crUpdateVisitor* nv);

        //virtual void cull(CRUtil::crCullVisitor* nv);

        ///** Traverse the terain subgraph.*/
        //virtual void traverse(CRCore::crNodeVisitor& nv);

        //virtual void cleanSceneGraph();

        //void setFilterBias(float filterBias);
        //float getFilterBias() const { return m_filterBias; }
        //
        //void setFilterWidth(float filterWidth);
        //float getFilterWidth() const { return m_filterWidth; }

        //void setFilterMatrix(const CRCore::crMatrix3& matrix);
        //CRCore::crMatrix3& getFilterMatrix() { return m_filterMatrix; }
        //const CRCore::crMatrix3& getFilterMatrix() const { return m_filterMatrix; }
        //
        //enum FilterType
        //{
        //    GAUSSIAN,
        //    SMOOTH,
        //    SHARPEN
        //};
        //
        //void setFilterMatrixAs(FilterType filterType);

		virtual void releaseObjects(CRCore::crState* state);

		void setShowGrid(bool showGrid);
		bool getShowGrid();

		void setEditMode(bool editMode);
    private:

        virtual ~crGeometryTechnique();

        struct BufferData
        {
            CRCore::ref_ptr<CRCore::crMatrixTransform>  m_transform;
            CRCore::ref_ptr<CRCore::crObject>            m_geode;
            CRCore::ref_ptr<CRCore::crGeometry>         m_geometry;
			CRCore::ref_ptr<CRCore::crDrawable>         m_gridShape;
        };
        
        unsigned int                        m_currentReadOnlyBuffer;
        unsigned int                        m_currentWriteBuffer;
        
        mutable BufferData                          m_bufferData[2];
        
        void swapBuffers();

        inline BufferData& getReadOnlyBuffer() { return m_bufferData[m_currentReadOnlyBuffer]; }
        inline BufferData& getWriteBuffer() { return m_bufferData[m_currentWriteBuffer]; }
                
        //float                               m_filterBias;
        //CRCore::ref_ptr<CRCore::crUniform>          m_filterBiasUniform;
        //float                               m_filterWidth;
        //CRCore::ref_ptr<CRCore::crUniform>          m_filterWidthUniform;
        //CRCore::crMatrix3                        m_filterMatrix;
        //CRCore::ref_ptr<CRCore::crUniform>          m_filterMatrixUniform;
		bool m_editMode;
};

}

#endif
