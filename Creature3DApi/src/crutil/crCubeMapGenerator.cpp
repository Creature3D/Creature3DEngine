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
#include <CRUtil/crCubeMapGenerator.h>
#include <stdlib.h>

#include <CRCore/crMatrix.h>

using namespace CRUtil;

crCubeMapGenerator::crCubeMapGenerator(int texture_size):    
    CRCore::Referenced(),
    m_texture_size(texture_size)
{
    for (int i=0; i<6; ++i)
    {                
        CRCore::ref_ptr<CRCore::crImage> image = new CRCore::crImage;
        unsigned char* data = new unsigned char [texture_size*texture_size*4];
        image->setImage(texture_size, texture_size, 1, 4, GL_RGBA, GL_UNSIGNED_BYTE, data, CRCore::crImage::USE_NEW_DELETE);
        m_images.push_back(image);
    }
}

crCubeMapGenerator::crCubeMapGenerator(const crCubeMapGenerator &copy, const CRCore::crCopyOp &copyop):    
    CRCore::Referenced(copy),
    m_texture_size(copy.m_texture_size)
{
    Image_list::const_iterator i;
    for (i=copy.m_images.begin(); i!=copy.m_images.end(); ++i)
    {
        m_images.push_back(static_cast<CRCore::crImage *>(copyop(i->get())));
    }
}

void crCubeMapGenerator::generateMap(bool use_osg_system)
{
    CRCore::crMatrix M;
    
    if (use_osg_system) 
	{
        M = CRCore::crMatrix::rotate(CRCore::PI_2, CRCore::crVector3(1, 0, 0));
    } 
	else 
	{
        M = CRCore::crMatrix::identity();
    }

    const float dst = 2.0f/(m_texture_size-1);
    
    float t = -1;
    for (int i=0; i<m_texture_size; ++i) 
	{
        float s = -1;
        for (int j=0; j<m_texture_size; ++j) 
		{
            set_pixel(0, j, i, compute_color(CRCore::crVector3(1, -t, -s)  * M));
            set_pixel(1, j, i, compute_color(CRCore::crVector3(-1, -t, s)  * M));
            set_pixel(2, j, i, compute_color(CRCore::crVector3(s, 1, t)    * M));
            set_pixel(3, j, i, compute_color(CRCore::crVector3(s, -1, -t)  * M));
            set_pixel(4, j, i, compute_color(CRCore::crVector3(s, -t, 1)   * M));
            set_pixel(5, j, i, compute_color(CRCore::crVector3(-s, -t, -1) * M));
            s += dst;
        }
        t += dst;
    }
}
