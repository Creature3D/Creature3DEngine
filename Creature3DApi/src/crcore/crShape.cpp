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
//Modified by 吴财华
#include <CRCore/crShape.h>
#include <algorithm>

using namespace CRCore;

crShape::~crShape()
{
}

crHeightField::~crHeightField()
{
}


void crHeightField::allocate(unsigned int numColumns,unsigned int numRows)
{
    if (m_columns!=numColumns || m_rows!=numRows)
    {
    	m_heights->resize(numColumns*numRows);
		m_heightsForOde->resize(numColumns*numRows);
    }
    m_columns=numColumns;
    m_rows=numRows;
}

crVector3 crHeightField::getNormal(unsigned int c,unsigned int r) const 
{
    // four point normal generation.
    float dz_dx;
    if (c==0)
    {
    	dz_dx = (getHeight(c+1,r)-getHeight(c,r))/getXInterval();
    }
    else if (c==getNumColumns()-1)
    {
    	dz_dx = (getHeight(c,r)-getHeight(c-1,r))/getXInterval();
    }
    else // assume 0<c<_numColumns-1
    {
    	dz_dx = 0.5f*(getHeight(c+1,r)-getHeight(c-1,r))/getXInterval();
    }
    
    float dz_dy;
    if (r==0)
    {
    	dz_dy = (getHeight(c,r+1)-getHeight(c,r))/getYInterval();
    }
    else if (r==getNumRows()-1)
    {
    	dz_dy = (getHeight(c,r)-getHeight(c,r-1))/getYInterval();
    }
    else // assume 0<r<_numRows-1
    {
    	dz_dy = 0.5f*(getHeight(c,r+1)-getHeight(c,r-1))/getYInterval();
    }
    
    crVector3 normal(-dz_dx,-dz_dy,1.0f);
    normal.normalize();
    
    return normal;
}

crVector2 crHeightField::getHeightDelta(unsigned int c,unsigned int r) const
{
	// four point normal generation.
	crVector2 heightDelta;
	if (c==0)
	{
		heightDelta.x() = (getHeight(c+1,r)-getHeight(c,r));
	}
	else if (c==getNumColumns()-1)
	{
		heightDelta.x() = (getHeight(c,r)-getHeight(c-1,r));
	}
	else // assume 0<c<_numColumns-1
	{
		heightDelta.x() = 0.5f*(getHeight(c+1,r)-getHeight(c-1,r));
	}

	if (r==0)
	{
		heightDelta.y() = (getHeight(c,r+1)-getHeight(c,r));
	}
	else if (r==getNumRows()-1)
	{
		heightDelta.y() = (getHeight(c,r)-getHeight(c,r-1));
	}
	else // assume 0<r<_numRows-1
	{
		heightDelta.y() = 0.5f*(getHeight(c,r+1)-getHeight(c,r-1));
	}

	return heightDelta;
}

void crHeightField::buildFromHeightMap(crImage *heightimage,float maxHeight)
{
	m_image = heightimage;
	m_heightMap = heightimage->getFileName();;
	m_maxHeight = maxHeight;

	int c,r;
	float height;
	float cs = float(m_columns-1);
	float rs = float(m_rows-1);
	for(r=0;r<m_rows;++r)
	{
		for(c=0;c<m_columns;++c)
		{
			height = heightimage->getColor(crVector3(float(c)/cs,float(r)/rs,0.0f))[0];
			height = height * 2.0f - 1.0f;
			setHeight(c,r,height * maxHeight);
		}
	}
}

const std::string &crHeightField::getHeightMap() const
{
	return m_heightMap;
}
float crHeightField::getMaxHeight() const
{
	return m_maxHeight;
}
crImage *crHeightField::getHeightImage()
{
	return m_image.get();
}
void crHeightField::setHeightImage(crImage *image)
{
	m_image = image;
}
void crHeightField::setHeightMapName(const std::string &name)
{
	m_heightMap = name;
	if(m_image.valid())
		m_image->setFileName(name);
}
void crHeightField::allocateColor()
{
	m_colors = new Vec4Array(m_columns * m_rows);
}