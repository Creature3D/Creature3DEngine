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
//Modified by Îâ²Æ»ª
#ifndef CRUTIL_CUBEMAPGENERATOR_H
#define CRUTIL_CUBEMAPGENERATOR_H

#include <CRUtil/crExport.h>

#include <CRCore/crVector3.h>
#include <CRCore/crVector4.h>
#include <CRCore/crCopyOp.h>
#include <CRCore/Referenced.h>
#include <CRCore/crTextureCubeMap.h>
#include <CRCore/crImage.h>
#include <CRCore/crNotify.h>

#include <vector>

namespace CRUtil
{

    /** This is the base class for cube map generators. 
        It exposes the necessary interface to access the six generated images;
        descendants    should only override the compute_color() method.
    */
    class CRUTIL_EXPORT crCubeMapGenerator: public CRCore::Referenced 
	{
    public:
        explicit crCubeMapGenerator(int texture_size = 64);
        crCubeMapGenerator(const crCubeMapGenerator &copy, const CRCore::crCopyOp &copyop = CRCore::crCopyOp::SHALLOW_COPY);

        inline CRCore::crImage *getImage(CRCore::crTextureCubeMap::Face face);
        inline const CRCore::crImage *getImage(CRCore::crTextureCubeMap::Face face) const;

        /** generate the six cube images. 
            If use_osg_system is true, then the OSG's coordinate system is used instead 
            of the default OpenGL one.
        */
        void generateMap(bool use_osg_system = true);

    protected:
        virtual ~crCubeMapGenerator() {}
        crCubeMapGenerator &operator=(const crCubeMapGenerator &) { return *this; }

        inline void set_pixel(int index, int c, int r, const CRCore::crVector4 &color);
        inline static CRCore::crVector4 vector_to_color(const CRCore::crVector3 &vec);        

        /** override this method to define how colors are computed. 
            The parameter R is the reflection vector, pointing from the center of the cube.
            The return value should be the RGBA color associated with that reflection ray.
        */
        virtual CRCore::crVector4 compute_color(const CRCore::crVector3 &R) const = 0;

    private:
        int m_texture_size;

        typedef std::vector<CRCore::ref_ptr<CRCore::crImage> > Image_list;
        Image_list m_images;        
    };

    // INLINE METHODS

    inline CRCore::crImage *crCubeMapGenerator::getImage(CRCore::crTextureCubeMap::Face face)
    {
        switch (face) 
		{
            case CRCore::crTextureCubeMap::POSITIVE_X: return m_images[0].get();
            case CRCore::crTextureCubeMap::NEGATIVE_X: return m_images[1].get();
            case CRCore::crTextureCubeMap::POSITIVE_Y: return m_images[2].get();
            case CRCore::crTextureCubeMap::NEGATIVE_Y: return m_images[3].get();
            case CRCore::crTextureCubeMap::POSITIVE_Z: return m_images[4].get();
            case CRCore::crTextureCubeMap::NEGATIVE_Z: return m_images[5].get();            
            default: return 0;
        }
    }

    inline const CRCore::crImage *crCubeMapGenerator::getImage(CRCore::crTextureCubeMap::Face face) const
    {
        switch (face) 
		{
            case CRCore::crTextureCubeMap::POSITIVE_X: return m_images[0].get();
            case CRCore::crTextureCubeMap::NEGATIVE_X: return m_images[1].get();
            case CRCore::crTextureCubeMap::POSITIVE_Y: return m_images[2].get();
            case CRCore::crTextureCubeMap::NEGATIVE_Y: return m_images[3].get();
            case CRCore::crTextureCubeMap::POSITIVE_Z: return m_images[4].get();
            case CRCore::crTextureCubeMap::NEGATIVE_Z: return m_images[5].get();            
            default: return 0;
        }
    }

    inline void crCubeMapGenerator::set_pixel(int index, int c, int r, const CRCore::crVector4 &color)
    {
        CRCore::crImage *i = m_images[index].get();
        if (i) 
		{
            *(i->data(c, r)+0) = static_cast<unsigned char>(CRCore::clampBetween(color.x(),0.0f,1.0f) * 255);
            *(i->data(c, r)+1) = static_cast<unsigned char>(CRCore::clampBetween(color.y(),0.0f,1.0f) * 255);
            *(i->data(c, r)+2) = static_cast<unsigned char>(CRCore::clampBetween(color.z(),0.0f,1.0f) * 255);
            *(i->data(c, r)+3) = static_cast<unsigned char>(CRCore::clampBetween(color.w(),0.0f,1.0f) * 255);            
        } 
		else 
		{
            CRCore::notify(CRCore::WARN) << "Warning: crCubeMapGenerator::set_pixel(): invalid image index\n";
        }
    }

    inline CRCore::crVector4 crCubeMapGenerator::vector_to_color(const CRCore::crVector3 &vec)
    {
        return CRCore::crVector4(
            vec.x() / vec.length() / 2 + 0.5f,
            vec.y() / vec.length() / 2 + 0.5f,
            vec.z() / vec.length() / 2 + 0.5f,
            1);
    }

}

#endif
