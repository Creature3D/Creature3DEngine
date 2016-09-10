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
#include <CRCore/crDrawPixels.h>

using namespace CRCore;

crDrawPixels::crDrawPixels()
{
    // turn off display lists right now, just incase we want to modify the projection matrix along the way.
    setSupportsDisplayList(false);
    
    m_position.set(0.0f,0.0f,0.0f);
    
    m_useSubImage = false;
    m_offsetX = 0;
    m_offsetY = 0; 
    m_width = 0;
    m_height = 0;
	m_scaleWithScreen = Scale_None;
}

crDrawPixels::crDrawPixels(const crDrawPixels& drawimage,const crCopyOp& copyop):
    crDrawable(drawimage,copyop),
    m_position(drawimage.m_position),
    m_image(drawimage.m_image),
    m_useSubImage(drawimage.m_useSubImage),
    m_offsetX(drawimage.m_offsetX), 
    m_offsetY(drawimage.m_offsetY),
    m_width(drawimage.m_width),
    m_height(drawimage.m_height),
	m_scaleWithScreen(drawimage.m_scaleWithScreen)
{
}

crDrawPixels::~crDrawPixels()
{
    // image will delete itself thanks to ref_ptr :-)
}

void crDrawPixels::setPosition(const CRCore::crVector3& position)
{
    m_position = position;
    dirtyBound();
}

void crDrawPixels::setSubImageDimensions(unsigned int offsetX,unsigned int offsetY,unsigned int width,unsigned int height)
{
    m_useSubImage = true;
    m_offsetX = offsetX;
    m_offsetY = offsetY; 
    m_width = width;
    m_height = height;
}

void crDrawPixels::getSubImageDimensions(unsigned int& offsetX,unsigned int& offsetY,unsigned int& width,unsigned int& height) const
{
    offsetX = m_offsetX;
    offsetY = m_offsetY; 
    width = m_width;
    height = m_height;
}


bool crDrawPixels::computeBound() const
{
    // really needs to be dependant of view poistion and projection... will implement simple version right now.
    //crBoundingBox bbox;
    float diagonal = 0.0f;
    if (m_useSubImage)
    {
        diagonal = sqrtf(m_width*m_width+m_height*m_height);
    }
    else
    {
        diagonal = sqrtf(m_image->s()*m_image->s()+m_image->t()*m_image->t());
    }
    
    m_bbox.expandBy(m_position-CRCore::crVector3(diagonal,diagonal,diagonal));
    m_bbox.expandBy(m_position+CRCore::crVector3(diagonal,diagonal,diagonal));
	m_bbox_computed = true;
    return true;
}

void crDrawPixels::drawImplementation(crState&) const
{
    glRasterPos3f(m_position.x(),m_position.y(),m_position.z());

    if (m_useSubImage)
    {
        const GLvoid* pixels = m_image->data(m_offsetX,m_offsetY);
        glPixelStorei(GL_UNPACK_ROW_LENGTH,m_image->s());
        glDrawPixels(m_width,m_height,
                     (GLenum)m_image->getPixelFormat(),
                     (GLenum)m_image->getDataType(),
                     pixels);
        glPixelStorei(GL_UNPACK_ROW_LENGTH,0);
    }
    else
    {
		switch(m_scaleWithScreen)
		{
		case Scale_Both:
			{
				m_image->scaleImage(crDisplaySettings::instance()->getViewWidth(),crDisplaySettings::instance()->getViewHeight(),1.0);
				m_scaleWithScreen = Scale_None;
			}
			break;
		case Scale_Width:
			{
				m_image->scaleImage(crDisplaySettings::instance()->getViewWidth(),m_image->t(),1.0);
				m_scaleWithScreen = Scale_None;
			}
			break;
		case Scale_Height:
			{
				m_image->scaleImage(m_image->s(),crDisplaySettings::instance()->getViewHeight(),1.0);
				m_scaleWithScreen = Scale_None;
			}
			break;
		}
		
        glDrawPixels(m_image->s(), m_image->t(),
                     (GLenum)m_image->getPixelFormat(),
                     (GLenum)m_image->getDataType(),
                     m_image->data() );
    }
}

void crDrawPixels::setScaleWithScreen(crDrawPixels::ScaleWithScreen scaleWithScreen)
{
	m_scaleWithScreen = scaleWithScreen;
}

crDrawPixels::ScaleWithScreen crDrawPixels::getScaleWithScreen()
{
	return m_scaleWithScreen;
}


