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
#include <CRTerrain/crGeometryTechnique.h>
#include <CRTerrain/crTerrainTile.h>
#include <CRTerrain/crTerrain.h>

#include <CRUtil/crSmoothingVisitor.h>
#include <CRUtil/crPrepareVisitor.h>

#include <CRIOManager/crFileUtils.h>

#include <CRCore/crTexture2D.h>
#include <CRCore/crTexture1D.h>
#include <CRCore/crTexEnvCombine.h>
#include <CRCore/crProgram.h>
#include <CRCore/crMath.h>
#include <CRCore/crTimer.h>
#include <CRCore/crDisplaySettings.h>
#include <CRCore/crShapeDrawable.h>
#include <CRCore/crLineWidth.h>
#include <CRCore/crPolygonMode.h>
#include <CRIOManager/crLoadManager.h>
using namespace CRTerrain;
using namespace CRCore;
#define NEW_COORD_CODE

crGeometryTechnique::crGeometryTechnique():
    m_currentReadOnlyBuffer(1),
    m_currentWriteBuffer(0),
	m_editMode(false)
{
    //setFilterBias(0);
    //setFilterWidth(0.1);
    //setFilterMatrixAs(GAUSSIAN);
}

crGeometryTechnique::crGeometryTechnique(const crGeometryTechnique& gt,const CRCore::crCopyOp& copyop):
    crTerrainTechnique(gt,copyop),
    m_currentReadOnlyBuffer(1),
    m_currentWriteBuffer(0),
	m_editMode(false)
{
    //setFilterBias(gt.m_filterBias);
    //setFilterWidth(gt.m_filterWidth);
    //setFilterMatrix(gt.m_filterMatrix);
}

crGeometryTechnique::~crGeometryTechnique()
{
}

void crGeometryTechnique::swapBuffers()
{
	if (!m_terrainTile) return;
	//BufferData& rbuffer = getReadOnlyBuffer();
	//CRCore::crMatrixTransform *transform = dynamic_cast<CRCore::crMatrixTransform *>(m_terrainTile->getChild(0));
	//crStateSet *obj_ss = NULL;
	//crStateSet *geo_ss = NULL;
	//if(transform)
	//{
	//	rbuffer.m_transform = transform;
	//	rbuffer.m_geode = dynamic_cast<CRCore::crObject *>(transform->getChild(0));
	//	if(rbuffer.m_geode.valid())
	//	{
	//		rbuffer.m_geometry = dynamic_cast<CRCore::crGeometry *>(rbuffer.m_geode->getDrawable(0));
	//		obj_ss = rbuffer.m_geode->getStateSet();
	//		geo_ss = rbuffer.m_geometry->getStateSet();
	//	}
	//}
	std::swap(m_currentReadOnlyBuffer,m_currentWriteBuffer);
	BufferData& wbuffer = getReadOnlyBuffer();

	if (wbuffer.m_transform.valid())
	{
		CRIOManager::crLoadManager::getInstance()->requestAddNode(m_terrainTile,wbuffer.m_transform.get(),false);
		crNode *child = m_terrainTile->getChild(0);
		if(child)
		    CRIOManager::crLoadManager::getInstance()->requestRemoveNode(m_terrainTile,child);
		//m_terrainTile->removeChild(0,m_terrainTile->getNumChildren());
	    //m_terrainTile->addChild(wbuffer.m_transform.get());
	}

}
void crGeometryTechnique::setEditMode(bool editMode)
{
	m_editMode = editMode;
}
//void crGeometryTechnique::setFilterBias(float filterBias)
//{
//    m_filterBias = filterBias;
//    if (!m_filterBiasUniform) m_filterBiasUniform = new CRCore::crUniform("filterBias",m_filterBias);
//    else m_filterBiasUniform->set(filterBias);
//}
//
//void crGeometryTechnique::setFilterWidth(float filterWidth)
//{
//    m_filterWidth = filterWidth;
//    if (!m_filterWidthUniform) m_filterWidthUniform = new CRCore::crUniform("filterWidth",m_filterWidth);
//    else m_filterWidthUniform->set(filterWidth);
//}
//
//void crGeometryTechnique::setFilterMatrix(const CRCore::crMatrix3& matrix)
//{
//    m_filterMatrix = matrix; 
//    if (!m_filterMatrixUniform) m_filterMatrixUniform = new CRCore::crUniform("filterMatrix",m_filterMatrix);
//    else m_filterMatrixUniform->set(m_filterMatrix);
//}
//
//void crGeometryTechnique::setFilterMatrixAs(FilterType filterType)
//{
//    switch(filterType)
//    {
//        case(SMOOTH):
//            setFilterMatrix(CRCore::crMatrix3(0.0, 0.5/2.5, 0.0,
//                                         0.5/2.5, 0.5/2.5, 0.5/2.5,
//                                         0.0, 0.5/2.5, 0.0));
//            break;
//        case(GAUSSIAN):
//            setFilterMatrix(CRCore::crMatrix3(0.0, 1.0/8.0, 0.0,
//                                         1.0/8.0, 4.0/8.0, 1.0/8.0,
//                                         0.0, 1.0/8.0, 0.0));
//            break;
//        case(SHARPEN):
//            setFilterMatrix(CRCore::crMatrix3(0.0, -1.0, 0.0,
//                                         -1.0, 5.0, -1.0,
//                                         0.0, -1.0, 0.0));
//            break;
//
//    };
//}

void crGeometryTechnique::init()
{
    //CRCore::notify(CRCore::INFO)<<"Doing crGeometryTechnique::init()"<<std::endl;
    
    if (!m_terrainTile) return;

    BufferData& buffer = getWriteBuffer();
	CRCore::crMatrixTransform *transform = dynamic_cast<CRCore::crMatrixTransform *>(m_terrainTile->getChild(0));
	if(transform)
	{
		buffer.m_transform = dynamic_cast<crMatrixTransform *>(transform->clone(CRCore::crCopyOp::DEEP_COPY_NODES));
		buffer.m_geode = dynamic_cast<CRCore::crObject *>(buffer.m_transform->getChild(0));
		if(buffer.m_geode.valid())
		{
			buffer.m_geometry = dynamic_cast<CRCore::crGeometry *>(buffer.m_geode->getDrawable(0));
		}
	}
    
    crLocator* masterLocator = computeMasterLocator();
    
    CRCore::crVector3d centerModel = computeCenterModel(masterLocator);
    
	try
	{
	    generateGeometry(masterLocator, centerModel);
	}
	catch (...)
	{
		CRCore::notify(CRCore::FATAL)<<"CRTerrain::generateGeometry error ÔÚµØÍ¼±ßÔµ±à¼­"<<std::endl;
		return;
	}

	if(!m_editMode)
	{
		generateGridGeometry();

		applyColorLayers();
		applyTransparency();
	}

	if(buffer.m_transform.valid()) buffer.m_transform->setName("TerrainMatrix");
	if(buffer.m_geode.valid()) buffer.m_geode->setName("TerrainObject");
	if(buffer.m_geometry.valid()) buffer.m_geometry->setName("TerrainMesh");
	if(buffer.m_gridShape.valid()) buffer.m_gridShape->setName("TerrainGrid");
    // smoothGeometry();

    //if (buffer.m_transform.valid()) buffer.m_transform->setThreadSafeRefUnref(true);

    swapBuffers();
}

crLocator* crGeometryTechnique::computeMasterLocator()
{
    CRTerrain::crLayer* elevationLayer = m_terrainTile->getElevationLayer();
    CRTerrain::crLayer* colorLayer = m_terrainTile->getColorLayer(0);

    crLocator* elevationLocator = elevationLayer ? elevationLayer->getLocator() : 0;
    crLocator* colorLocator = colorLayer ? colorLayer->getLocator() : 0;
    
    crLocator* masterLocator = elevationLocator ? elevationLocator : colorLocator;
    if (!masterLocator)
    {
        CRCore::notify(CRCore::NOTICE)<<"Problem, no locator found in any of the terrain layers"<<std::endl;
        return 0;
    }
    
    return masterLocator;
}

CRCore::crVector3d crGeometryTechnique::computeCenterModel(crLocator* masterLocator)
{
    if (!masterLocator) return CRCore::crVector3d(0.0,0.0,0.0);

    BufferData& buffer = getWriteBuffer();
    
    CRTerrain::crLayer* elevationLayer = m_terrainTile->getElevationLayer();
    CRTerrain::crLayer* colorLayer = m_terrainTile->getColorLayer(0);

    crLocator* elevationLocator = elevationLayer ? elevationLayer->getLocator() : 0;
    crLocator* colorLocator = colorLayer ? colorLayer->getLocator() : 0;
    
    if (!elevationLocator) elevationLocator = masterLocator;
    if (!colorLocator) colorLocator = masterLocator;

    CRCore::crVector3d bottomLeftNDC(DBL_MAX, DBL_MAX, 0.0);
    CRCore::crVector3d topRightNDC(-DBL_MAX, -DBL_MAX, 0.0);
    
    if (elevationLayer)
    {
        if (elevationLocator!= masterLocator)
        {
            masterLocator->computeLocalBounds(*elevationLocator, bottomLeftNDC, topRightNDC);
        }
        else
        {
            bottomLeftNDC.x() = CRCore::minimum(bottomLeftNDC.x(), 0.0);
            bottomLeftNDC.y() = CRCore::minimum(bottomLeftNDC.y(), 0.0);
            topRightNDC.x() = CRCore::maximum(topRightNDC.x(), 1.0);
            topRightNDC.y() = CRCore::maximum(topRightNDC.y(), 1.0);
        }
    }

    if (colorLayer)
    {
        if (colorLocator!= masterLocator)
        {
            masterLocator->computeLocalBounds(*colorLocator, bottomLeftNDC, topRightNDC);
        }
        else
        {
            bottomLeftNDC.x() = CRCore::minimum(bottomLeftNDC.x(), 0.0);
            bottomLeftNDC.y() = CRCore::minimum(bottomLeftNDC.y(), 0.0);
            topRightNDC.x() = CRCore::maximum(topRightNDC.x(), 1.0);
            topRightNDC.y() = CRCore::maximum(topRightNDC.y(), 1.0);
        }
    }

    //CRCore::notify(CRCore::INFO)<<"bottomLeftNDC = "<<bottomLeftNDC<<std::endl;
   // CRCore::notify(CRCore::INFO)<<"topRightNDC = "<<topRightNDC<<std::endl;

	if(!buffer.m_transform.valid())
        buffer.m_transform = new CRCore::crMatrixTransform;
	else
		buffer.m_transform->removeChild(0,buffer.m_transform->getNumChildren());

    CRCore::crVector3d centerNDC = (bottomLeftNDC + topRightNDC)*0.5;
    CRCore::crVector3d centerModel = (bottomLeftNDC + topRightNDC)*0.5;
    masterLocator->convertLocalToModel(centerNDC, centerModel);
    
    //buffer.m_transform->setMatrix(CRCore::crMatrix::translate(centerModel));
    
    return centerModel;
}

void crGeometryTechnique::generateGeometry(crLocator* masterLocator, const CRCore::crVector3d& centerModel)
{
    BufferData& buffer = getWriteBuffer();
    
    CRTerrain::crLayer* elevationLayer = m_terrainTile->getElevationLayer();

	if(!buffer.m_geode.valid())
        buffer.m_geode = new CRCore::crObject;
	else
		buffer.m_geode->removeDrawable(0,buffer.m_geode->getNumDrawables());

	if(buffer.m_transform.valid())
		buffer.m_transform->addChild(buffer.m_geode.get());
	
	crStateSet *geom_ss = NULL;;
	if(buffer.m_geometry.valid())
		geom_ss = buffer.m_geometry->getStateSet();
	//if(geom_ss)
	//{
	//	crUniform *uniform = geom_ss->getOrCreateUniform("uvw",CRCore::crUniform::BOOL);
	//	uniform->setDataVariance(crBase::STATIC);
	//	uniform->set(false);
	//}
	buffer.m_geometry = new CRCore::crGeometry;
	buffer.m_geometry->setStateSet(geom_ss);

	buffer.m_geode->addDrawable(buffer.m_geometry.get());
    
    CRCore::crGeometry* geometry = buffer.m_geometry.get();

    unsigned int numRows = 20;
    unsigned int numColumns = 20;
    
    if (elevationLayer)
    {
        numColumns = elevationLayer->getNumColumns();
        numRows = elevationLayer->getNumRows();
    }
    
    float sampleRatio = m_terrainTile->getTerrain() ? m_terrainTile->getTerrain()->getSampleRatio() : 1.0f;
    
    double i_sampleFactor = 1.0;
    double j_sampleFactor = 1.0;

    // CRCore::notify(CRCore::NOTICE)<<"Sample ratio="<<sampleRatio<<std::endl;

    if (sampleRatio!=1.0f)
    {
    
        unsigned int originalNumColumns = numColumns;
        unsigned int originalNumRows = numRows;
    
        numColumns = std::max((unsigned int) (float(originalNumColumns)*sqrtf(sampleRatio)), 4u);
        numRows = std::max((unsigned int) (float(originalNumRows)*sqrtf(sampleRatio)),4u);

        i_sampleFactor = double(originalNumColumns-1)/double(numColumns-1);
        j_sampleFactor = double(originalNumRows-1)/double(numRows-1);
    }
    
    

    bool treatBoundariesToValidDataAsDefaultValue = m_terrainTile->getTreatBoundariesToValidDataAsDefaultValue();
    //CRCore::notify(CRCore::INFO)<<"TreatBoundariesToValidDataAsDefaultValue="<<treatBoundariesToValidDataAsDefaultValue<<std::endl;
    
    float skirtHeight = 0.0f;
    crHeightFieldLayer* hfl = dynamic_cast<crHeightFieldLayer*>(elevationLayer);
    if (hfl && hfl->getHeightField()) 
    {
        skirtHeight = hfl->getHeightField()->getSkirtHeight();
    }
    
    bool createSkirt = skirtHeight != 0.0f;
  
    unsigned int numVerticesInBody = numColumns*numRows;
    unsigned int numVerticesInSkirt = createSkirt ? numColumns*2 + numRows*2 - 4 : 0;
    unsigned int numVertices = numVerticesInBody+numVerticesInSkirt;

    // allocate and assign vertices
    CRCore::ref_ptr<CRCore::Vec3Array> vertices = new CRCore::Vec3Array;
    vertices->reserve(numVertices);
    geometry->setVertexArray(vertices.get());

    // allocate and assign normals
    CRCore::ref_ptr<CRCore::Vec3Array> normals = new CRCore::Vec3Array;
    if (normals.valid()) normals->reserve(numVertices);
    geometry->setNormalArray(normals.get());
    geometry->setNormalBinding(CRCore::crGeometry::BIND_PER_VERTEX);
    

    //float minHeight = 0.0;
    float scaleHeight = m_terrainTile->getTerrain() ? m_terrainTile->getTerrain()->getVerticalScale() : 1.0f;

    // allocate and assign tex coords
    typedef std::pair< CRCore::ref_ptr<CRCore::Vec2Array>, crLocator* > TexCoordLocatorPair;
    typedef std::map< crLayer*, TexCoordLocatorPair > LayerToTexCoordMap;

    LayerToTexCoordMap layerToTexCoordMap;
    for(unsigned int layerNum=0; layerNum<m_terrainTile->getNumColorLayers(); ++layerNum)
    {
        CRTerrain::crLayer* colorLayer = m_terrainTile->getColorLayer(layerNum);
        if (colorLayer)
        {
            LayerToTexCoordMap::iterator itr = layerToTexCoordMap.find(colorLayer);
            if (itr!=layerToTexCoordMap.end())
            {
                geometry->setTexCoordArray(layerNum, itr->second.first.get());
            }
            else
            {

                crLocator* locator = colorLayer->getLocator();
                if (!locator)
                {            
                    CRTerrain::crSwitchLayer* switchLayer = dynamic_cast<CRTerrain::crSwitchLayer*>(colorLayer);
                    if (switchLayer)
                    {
                        if (switchLayer->getActiveLayer()>=0 &&
                            static_cast<unsigned int>(switchLayer->getActiveLayer())<switchLayer->getNumLayers() &&
                            switchLayer->getLayer(switchLayer->getActiveLayer()))
                        {
                            locator = switchLayer->getLayer(switchLayer->getActiveLayer())->getLocator();
                        }
                    }
                }            
            
                TexCoordLocatorPair& tclp = layerToTexCoordMap[colorLayer];
                tclp.first = new CRCore::Vec2Array;
                tclp.first->reserve(numVertices);
                tclp.second = locator ? locator : masterLocator;
                geometry->setTexCoordArray(layerNum, tclp.first.get());
            }
        }
    }

    CRCore::ref_ptr<CRCore::FloatArray> elevations = new CRCore::FloatArray;
    if (elevations.valid()) elevations->reserve(numVertices);
        

    // allocate and assign color
    //CRCore::ref_ptr<CRCore::Vec4Array> colors = new CRCore::Vec4Array(1);
    //(*colors)[0].set(1.0f,1.0f,1.0f,1.0f);
    //
    //geometry->setColorArray(colors.get());
    //geometry->setColorBinding(CRCore::crGeometry::BIND_OVERALL);


    typedef std::vector<int> Indices;
    Indices indices(numVertices, -1);
    
    // populate vertex and tex coord arrays
    unsigned int i, j;
    for(j=0; j<numRows; ++j)
    {
        for(i=0; i<numColumns; ++i)
        {
            unsigned int iv = j*numColumns + i;
            CRCore::crVector3d ndc( ((double)i)/(double)(numColumns-1), ((double)j)/(double)(numRows-1), 0.0);
     
            bool validValue = true;
     
            
            unsigned int i_equiv = i_sampleFactor==1.0 ? i : (unsigned int) (double(i)*i_sampleFactor);
            unsigned int j_equiv = i_sampleFactor==1.0 ? j : (unsigned int) (double(j)*j_sampleFactor);
            
            if (elevationLayer)
            {
                float value = 0.0f;
                validValue = elevationLayer->getValidValue(i_equiv,j_equiv, value);
                // CRCore::notify(CRCore::INFO)<<"i="<<i<<" j="<<j<<" z="<<value<<std::endl;
                ndc.z() = value*scaleHeight;
            }
            
            if (validValue)
            {
                indices[iv] = vertices->size();
            
                CRCore::crVector3d model;
                masterLocator->convertLocalToModel(ndc, model);

                (*vertices).push_back(model - centerModel);

                for(LayerToTexCoordMap::iterator itr = layerToTexCoordMap.begin();
                    itr != layerToTexCoordMap.end();
                    ++itr)
                {
                    CRCore::Vec2Array* texcoords = itr->second.first.get();
                    crLocator* colorLocator = itr->second.second;
                    if (colorLocator != masterLocator)
                    {
                        CRCore::crVector3d color_ndc;
                        crLocator::convertLocalCoordBetween(*masterLocator, ndc, *colorLocator, color_ndc);
                        (*texcoords).push_back(CRCore::crVector2f(color_ndc.x(), color_ndc.y()));
                    }
                    else
                    {
                        (*texcoords).push_back(CRCore::crVector2f(ndc.x(), ndc.y()));
                    }
                }

                if (elevations.valid())
                {
                    (*elevations).push_back(ndc.z());
                }

                // compute the local normal
                CRCore::crVector3d ndc_one = ndc; ndc_one.z() += 1.0;
                CRCore::crVector3d model_one;
                masterLocator->convertLocalToModel(ndc_one, model_one);
                model_one = model_one - model;
                model_one.normalize();            
                (*normals).push_back(model_one);
            }
            else
            {
                indices[iv] = -1;
            }
        }
    }
    
    // populate primitive sets
//    bool optimizeOrientations = elevations!=0;
    bool swapOrientation = !(masterLocator->orientationOpenGL());
    
    CRCore::ref_ptr<CRCore::DrawElementsUShort> elements = new CRCore::DrawElementsUShort(GL_TRIANGLES);//DrawElementsUInt
    elements->reserve((numRows-1) * (numColumns-1) * 6);

    geometry->addPrimitive(elements.get());

    for(j=0; j<numRows-1; ++j)
    {
        for(i=0; i<numColumns-1; ++i)
        {
            int i00;
            int i01;
            if (swapOrientation)
            {
                i01 = j*numColumns + i;
                i00 = i01+numColumns;
            }
            else
            {
                i00 = j*numColumns + i;
                i01 = i00+numColumns;
            }

            int i10 = i00+1;
            int i11 = i01+1;

            // remap indices to final vertex positions
            i00 = indices[i00];
            i01 = indices[i01];
            i10 = indices[i10];
            i11 = indices[i11];
            
            unsigned int numValid = 0;
            if (i00>=0) ++numValid;
            if (i01>=0) ++numValid;
            if (i10>=0) ++numValid;
            if (i11>=0) ++numValid;
            
            if (numValid==4)
            {
                float e00 = (*elevations)[i00];
                float e10 = (*elevations)[i10];
                float e01 = (*elevations)[i01];
                float e11 = (*elevations)[i11];

                if (fabsf(e00-e11)<fabsf(e01-e10))
                {
                    elements->push_back(i01);
                    elements->push_back(i00);
                    elements->push_back(i11);

                    elements->push_back(i00);
                    elements->push_back(i10);
                    elements->push_back(i11);
                }
                else
                {
                    elements->push_back(i01);
                    elements->push_back(i00);
                    elements->push_back(i10);

                    elements->push_back(i01);
                    elements->push_back(i10);
                    elements->push_back(i11);
                }
            }
            else if (numValid==3)
            {
                if (i00>=0) elements->push_back(i00);
                if (i01>=0) elements->push_back(i01);
                if (i11>=0) elements->push_back(i11);
                if (i10>=0) elements->push_back(i10);
            }
            
        }
    }
    
    if (elevationLayer)
    {
        smoothGeometry();
        
        normals = dynamic_cast<CRCore::Vec3Array*>(geometry->getNormalArray());
        
        if (!normals) createSkirt = false;
    }

    if (createSkirt)
    {
		CRCore::ref_ptr<CRCore::Vec3Array> skirtVectors = new CRCore::Vec3Array((*normals));

        CRCore::ref_ptr<CRCore::DrawElementsUShort> skirtDrawElements = new CRCore::DrawElementsUShort(GL_QUAD_STRIP);
        // create bottom skirt vertices
        int r,c;
        r=0;
        for(c=0;c<static_cast<int>(numColumns);++c)
        {
            int orig_i = indices[(r)*numColumns+c]; // index of original vertex of grid
            if (orig_i>=0)
            {
                unsigned int new_i = vertices->size(); // index of new index of added skirt point
                CRCore::crVector3f new_v = (*vertices)[orig_i] - ((*skirtVectors)[orig_i])*skirtHeight;
                (*vertices).push_back(new_v);
                if (normals.valid()) (*normals).push_back((*normals)[orig_i]);

                for(LayerToTexCoordMap::iterator itr = layerToTexCoordMap.begin();
                    itr != layerToTexCoordMap.end();
                    ++itr)
                {
                    itr->second.first->push_back((*itr->second.first)[orig_i]);
                }
                
                skirtDrawElements->push_back(orig_i);
                skirtDrawElements->push_back(new_i);
            }
            else
            {
                if (!skirtDrawElements->empty())
                {
                    geometry->addPrimitive(skirtDrawElements.get());
                    skirtDrawElements = new CRCore::DrawElementsUShort(GL_QUAD_STRIP);
                }
                
            }
        }

        if (!skirtDrawElements->empty())
        {
            geometry->addPrimitive(skirtDrawElements.get());
            skirtDrawElements = new CRCore::DrawElementsUShort(GL_QUAD_STRIP);
        }

        // create right skirt vertices
        c=numColumns-1;
        for(r=0;r<static_cast<int>(numRows);++r)
        {
            int orig_i = indices[(r)*numColumns+c]; // index of original vertex of grid
            if (orig_i>=0)
            {
                unsigned int new_i = vertices->size(); // index of new index of added skirt point
                CRCore::crVector3f new_v = (*vertices)[orig_i] - ((*skirtVectors)[orig_i])*skirtHeight;
                (*vertices).push_back(new_v);
                if (normals.valid()) (*normals).push_back((*normals)[orig_i]);
                for(LayerToTexCoordMap::iterator itr = layerToTexCoordMap.begin();
                    itr != layerToTexCoordMap.end();
                    ++itr)
                {
                    itr->second.first->push_back((*itr->second.first)[orig_i]);
                }
                
                skirtDrawElements->push_back(orig_i);
                skirtDrawElements->push_back(new_i);
            }
            else
            {
                if (!skirtDrawElements->empty())
                {
                    geometry->addPrimitive(skirtDrawElements.get());
                    skirtDrawElements = new CRCore::DrawElementsUShort(GL_QUAD_STRIP);
                }
                
            }
        }

        if (!skirtDrawElements->empty())
        {
            geometry->addPrimitive(skirtDrawElements.get());
            skirtDrawElements = new CRCore::DrawElementsUShort(GL_QUAD_STRIP);
        }

        // create top skirt vertices
        r=numRows-1;
        for(c=numColumns-1;c>=0;--c)
        {
            int orig_i = indices[(r)*numColumns+c]; // index of original vertex of grid
            if (orig_i>=0)
            {
                unsigned int new_i = vertices->size(); // index of new index of added skirt point
                CRCore::crVector3f new_v = (*vertices)[orig_i] - ((*skirtVectors)[orig_i])*skirtHeight;
                (*vertices).push_back(new_v);
                if (normals.valid()) (*normals).push_back((*normals)[orig_i]);
                for(LayerToTexCoordMap::iterator itr = layerToTexCoordMap.begin();
                    itr != layerToTexCoordMap.end();
                    ++itr)
                {
                    itr->second.first->push_back((*itr->second.first)[orig_i]);
                }
                
                skirtDrawElements->push_back(orig_i);
                skirtDrawElements->push_back(new_i);
            }
            else
            {
                if (!skirtDrawElements->empty())
                {
                    geometry->addPrimitive(skirtDrawElements.get());
                    skirtDrawElements = new CRCore::DrawElementsUShort(GL_QUAD_STRIP);
                }
                
            }
        }

        if (!skirtDrawElements->empty())
        {
            geometry->addPrimitive(skirtDrawElements.get());
            skirtDrawElements = new CRCore::DrawElementsUShort(GL_QUAD_STRIP);
        }

        // create left skirt vertices
        c=0;
        for(r=numRows-1;r>=0;--r)
        {
            int orig_i = indices[(r)*numColumns+c]; // index of original vertex of grid
            if (orig_i>=0)
            {
                unsigned int new_i = vertices->size(); // index of new index of added skirt point
                CRCore::crVector3f new_v = (*vertices)[orig_i] - ((*skirtVectors)[orig_i])*skirtHeight;
                (*vertices).push_back(new_v);
                if (normals.valid()) (*normals).push_back((*normals)[orig_i]);
                for(LayerToTexCoordMap::iterator itr = layerToTexCoordMap.begin();
                    itr != layerToTexCoordMap.end();
                    ++itr)
                {
                    itr->second.first->push_back((*itr->second.first)[orig_i]);
                }
                
                skirtDrawElements->push_back(orig_i);
                skirtDrawElements->push_back(new_i);
            }
            else
            {
                if (!skirtDrawElements->empty())
                {
                    geometry->addPrimitive(skirtDrawElements.get());
                    skirtDrawElements = new CRCore::DrawElementsUShort(GL_QUAD_STRIP);
                }
                
            }
        }

        if (!skirtDrawElements->empty())
        {
            geometry->addPrimitive(skirtDrawElements.get());
            skirtDrawElements = new CRCore::DrawElementsUShort(GL_QUAD_STRIP);
        }
    }


    geometry->setUseDisplayList(false);
    geometry->setUseVertexBufferObjects(true);
	geometry->setUseIndexBufferObjects(true);
	
	if (hfl) geometry->setShape(hfl->getHeightField());
    
	CRUtil::crPrepareVisitor prepareVisitor;
	prepareVisitor.prepareTBN(geometry);

#if 0
    if (CRIOManager::crRegistry::instance()->getBuildKdTreesHint()==CRIOManager::crReaderWriter::Options::BUILD_KDTREES &&
        CRIOManager::crRegistry::instance()->getKdTreeBuilder())
    {
    
        
        //CRCore::Timer_t before = CRCore::Timer::instance()->tick();
        //CRCore::notify(CRCore::NOTICE)<<"CRTerrain::crGeometryTechnique::build kd tree"<<std::endl;
        CRCore::ref_ptr<CRCore::crKdTreeBuilder> builder = CRIOManager::Registry::instance()->getKdTreeBuilder()->clone();
        buffer.m_geode->accept(*builder);
        //CRCore::Timer_t after = CRCore::Timer::instance()->tick();
        //CRCore::notify(CRCore::NOTICE)<<"KdTree build time "<<CRCore::Timer::instance()->delta_m(before, after)<<std::endl;
    }
#endif 
}

void crGeometryTechnique::generateGridGeometry()
{
	if(crDisplaySettings::instance()->getRunMode() == 0)
	{//Editor
		CRTerrain::crLayer* elevationLayer = m_terrainTile->getElevationLayer();
		crHeightFieldLayer* hfl = dynamic_cast<crHeightFieldLayer*>(elevationLayer);
		if (hfl && hfl->getHeightField()) 
		{
			BufferData& buffer = getWriteBuffer();
			crShapeDrawable *gridShape = new crShapeDrawable(hfl->getHeightField());
			buffer.m_gridShape = gridShape;
			//buffer.m_geode->addDrawable(buffer.m_gridShape.get());

			//crTessellationHints *hint = new crTessellationHints;
			//hint->setCreateBackFace(true);
			//hint->setCreateFrontFace(false);
			//gridshape->setTessellationHints(hint);
			gridShape->setColor(crVector4(0.7f,0.7f,0.7f,1.0f));

			crStateSet *stateset = buffer.m_gridShape->getOrCreateStateSet(); 
			stateset->setMode(GL_LIGHTING,crStateAttribute::OFF);
			crDepth *depth = new crDepth(crDepth::ALWAYS,0.0,1.0,true);
			stateset->setAttributeAndModes(depth,crStateAttribute::ON);
			crLineWidth *lineWidth = new crLineWidth(1.0f);
			stateset->setAttributeAndModes(lineWidth,crStateAttribute::ON);
			crPolygonMode *polygonMode = new crPolygonMode;
			polygonMode->setMode(crPolygonMode::FRONT_AND_BACK,crPolygonMode::LINE);
			stateset->setAttributeAndModes(polygonMode,crStateAttribute::ON);
			stateset->setMode(GL_CULL_FACE,crStateAttribute::OFF);
			stateset->setRenderingHint(crStateSet::TRANSPARENT_BIN);
		}
	}
}

void crGeometryTechnique::applyColorLayers()
{
    BufferData& buffer = getWriteBuffer();

    typedef std::map<CRTerrain::crLayer*, CRCore::crTexture*> LayerToTextureMap;
    LayerToTextureMap layerToTextureMap;
    
    for(unsigned int layerNum=0; layerNum<m_terrainTile->getNumColorLayers(); ++layerNum)
    {
        CRTerrain::crLayer* colorLayer = m_terrainTile->getColorLayer(layerNum);
        if (!colorLayer) continue;

        CRTerrain::crSwitchLayer* switchLayer = dynamic_cast<CRTerrain::crSwitchLayer*>(colorLayer);
        if (switchLayer)
        {
            if (switchLayer->getActiveLayer()<0 || 
                static_cast<unsigned int>(switchLayer->getActiveLayer())>=switchLayer->getNumLayers())
            {
                continue;
            }

            colorLayer = switchLayer->getLayer(switchLayer->getActiveLayer());
            if (!colorLayer) continue;
        }

        CRCore::crImage* image = colorLayer->getImage();
        if (!image) continue;

        CRTerrain::crImageLayer* imageLayer = dynamic_cast<CRTerrain::crImageLayer*>(colorLayer);
        CRTerrain::crContourLayer* contourLayer = dynamic_cast<CRTerrain::crContourLayer*>(colorLayer);
        if (imageLayer)
        {
            CRCore::crStateSet* stateset = buffer.m_geometry->getOrCreateStateSet();

            CRCore::crTexture2D* texture2D = dynamic_cast<CRCore::crTexture2D*>(layerToTextureMap[colorLayer]);
            if (!texture2D)
            {
                texture2D = new CRCore::crTexture2D;
                texture2D->setImage(image);
				texture2D->setMaxAnisotropy(crDisplaySettings::instance()->getMaxAnisotropy());//16.0f
                texture2D->setResizeNonPowerOfTwoHint(false);

                texture2D->setFilter(CRCore::crTexture::MIN_FILTER, colorLayer->getMinFilter());
                texture2D->setFilter(CRCore::crTexture::MAG_FILTER, colorLayer->getMagFilter());

                texture2D->setWrap(CRCore::crTexture::WRAP_S,CRCore::crTexture::REPEAT);
                texture2D->setWrap(CRCore::crTexture::WRAP_T,CRCore::crTexture::REPEAT);

                bool mipMapping = !(texture2D->getFilter(CRCore::crTexture::MIN_FILTER)==CRCore::crTexture::LINEAR || texture2D->getFilter(CRCore::crTexture::MIN_FILTER)==CRCore::crTexture::NEAREST);
                bool s_NotPowerOfTwo = image->s()==0 || (image->s() & (image->s() - 1));
                bool t_NotPowerOfTwo = image->t()==0 || (image->t() & (image->t() - 1));

                if (mipMapping && (s_NotPowerOfTwo || t_NotPowerOfTwo))
                {
                    //CRCore::notify(CRCore::INFO)<<"Disabling mipmapping for non power of two tile size("<<image->s()<<", "<<image->t()<<")"<<std::endl;
                    texture2D->setFilter(CRCore::crTexture::MIN_FILTER, CRCore::crTexture::LINEAR);
                }


                layerToTextureMap[colorLayer] = texture2D;

                // CRCore::notify(CRCore::NOTICE)<<"Creating new crImageLayer texture "<<layerNum<<" image->s()="<<image->s()<<"  image->t()="<<image->t()<<std::endl;

            }
            else
            {
                // CRCore::notify(CRCore::NOTICE)<<"Reusing crImageLayer texture "<<layerNum<<std::endl;
            }

            stateset->setTextureAttributeAndModes(layerNum, texture2D, CRCore::crStateAttribute::ON);
            
        }
        else if (contourLayer)
        {
            CRCore::crStateSet* stateset = buffer.m_geometry->getOrCreateStateSet();

            CRCore::crTexture1D* texture1D = dynamic_cast<CRCore::crTexture1D*>(layerToTextureMap[colorLayer]);
            if (!texture1D)
            {
                texture1D = new CRCore::crTexture1D;
                texture1D->setImage(image);
                texture1D->setResizeNonPowerOfTwoHint(false);
                texture1D->setFilter(CRCore::crTexture::MIN_FILTER, CRCore::crTexture::NEAREST);
                texture1D->setFilter(CRCore::crTexture::MAG_FILTER, colorLayer->getMagFilter());

                layerToTextureMap[colorLayer] = texture1D;
            }
            
            stateset->setTextureAttributeAndModes(layerNum, texture1D, CRCore::crStateAttribute::ON);

        }
    }
}

void crGeometryTechnique::applyTransparency()
{
    BufferData& buffer = getWriteBuffer();
    
    bool containsTransparency = false;
    for(unsigned int i=0; i<m_terrainTile->getNumColorLayers(); ++i)
    {
        CRCore::crImage* image = (m_terrainTile->getColorLayer(i)!=0) ? m_terrainTile->getColorLayer(i)->getImage() : 0;
        if (image)
        {
            containsTransparency = image->isImageTranslucent();
            break;
        }        
    }
    
    if (containsTransparency)
    {
        CRCore::crStateSet* stateset = buffer.m_geometry->getOrCreateStateSet();
        stateset->setMode(GL_BLEND, CRCore::crStateAttribute::ON);
        stateset->setRenderingHint(CRCore::crStateSet::TRANSPARENT_BIN);
    }

}

void crGeometryTechnique::smoothGeometry()
{
    BufferData& buffer = getWriteBuffer();
    
    if (buffer.m_geometry.valid())
    {
        CRUtil::crSmoothingVisitor smoother;
        smoother.smooth(*buffer.m_geometry);
    }
}
void crGeometryTechnique::releaseObjects(crState* state)
{
	BufferData& buffer = m_bufferData[m_currentReadOnlyBuffer];
	buffer.m_transform = NULL;
	buffer.m_geode = NULL;
	buffer.m_geometry = NULL;
	buffer.m_gridShape = NULL;
}

void crGeometryTechnique::setShowGrid(bool showGrid)
{
	BufferData& buffer = getReadOnlyBuffer();
    if(buffer.m_gridShape.valid())
	{
		if(!showGrid)
		    buffer.m_geode->removeDrawable(buffer.m_gridShape.get());
		else
			buffer.m_geode->addDrawable(buffer.m_gridShape.get());
	}
}

bool crGeometryTechnique::getShowGrid()
{
	BufferData& buffer = getReadOnlyBuffer();
	if(buffer.m_gridShape.valid())
	{
		return buffer.m_geode->containsDrawable(buffer.m_gridShape.get());
	}
	return false;
}

//void crGeometryTechnique::update(CRUtil::crUpdateVisitor* uv)
//{
//    if (m_terrainTile) m_terrainTile->CRCore::crGroup::traverse(*uv);
//}
//
//
//void crGeometryTechnique::cull(CRUtil::crCullVisitor* cv)
//{
//    BufferData& buffer = getReadOnlyBuffer();
//
//#if 0
//    if (buffer.m_terrainTile) buffer.m_terrainTile->CRCore::crGroup::traverse(*cv);
//#else
//    if (buffer.m_transform.valid())
//    {
//        buffer.m_transform->accept(*cv);
//    }
//#endif    
//}


//void crGeometryTechnique::traverse(CRCore::crNodeVisitor& nv)
//{
//    if (!m_terrainTile) return;
//
//    // if app traversal update the frame count.
//    if (nv.getVisitorType()==CRCore::crNodeVisitor::UPDATE_VISITOR)
//    {
//        if (m_terrainTile->getDirty()) m_terrainTile->init();
//
//        CRUtil::crUpdateVisitor* uv = dynamic_cast<CRUtil::crUpdateVisitor*>(&nv);
//        if (uv)
//        {
//            update(uv);
//            return;
//        }        
//        
//    }
//    else if (nv.getVisitorType()==CRCore::crNodeVisitor::CULL_VISITOR)
//    {
//        CRUtil::crCullVisitor* cv = dynamic_cast<CRUtil::crCullVisitor*>(&nv);
//        if (cv)
//        {
//            cull(cv);
//            return;
//        }
//    }
//
//
//    //if (m_terrainTile->getDirty()) 
//    //{
//    //    CRCore::notify(CRCore::INFO)<<"******* Doing init ***********"<<std::endl;
//    //    m_terrainTile->init();
//    //}
//
//    BufferData& buffer = getReadOnlyBuffer();
//    if (buffer.m_transform.valid()) buffer.m_transform->accept(nv);
//}
//
//
//void crGeometryTechnique::cleanSceneGraph()
//{
//}

