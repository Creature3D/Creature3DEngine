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
#include <CRTerrain/crLayer.h>
#include <CRCore/crNotify.h>

using namespace CRTerrain;

void CRTerrain::extractSetNameAndFileName(const std::string& compoundstring, std::string& setname, std::string& filename)
{
    std::string::size_type setcolonpos = compoundstring.find("set:");
    if (setcolonpos==std::string::npos)
    {
        setname = "";
        filename = compoundstring;
        return;
    }
    
    if (compoundstring.size()==4)
    {
        setname = "";
        filename = "";
        return;
    }
    
    std::string::size_type secondcolonpos = compoundstring.find_first_of(':', setcolonpos+4);
    if (secondcolonpos==std::string::npos)
    {
        setname = compoundstring.substr(setcolonpos+4,std::string::npos);
        filename = "";
        return;
    }

    setname = compoundstring.substr(setcolonpos+4,secondcolonpos-setcolonpos-4);
    filename = compoundstring.substr(secondcolonpos+1, std::string::npos);
}

std::string CRTerrain::createCompondSetNameAndFileName(const std::string& setname, const std::string& filename)
{
    if (setname.empty()) return filename;
    return std::string("set:")+setname+std::string(":")+filename;
}


crLayer::crLayer():
    m_minLevel(0),
    m_maxLevel(MAXIMUM_NUMBER_OF_LEVELS),
    m_minFilter(CRCore::crTexture::LINEAR_MIPMAP_LINEAR),
    m_magFilter(CRCore::crTexture::LINEAR)
{
}

crLayer::crLayer(const crLayer& layer,const CRCore::crCopyOp& copyop):
    CRCore::crBase(layer,copyop),
    m_filename(layer.m_filename),
    m_minLevel(layer.m_minLevel),
    m_maxLevel(layer.m_maxLevel),
    m_minFilter(layer.m_minFilter),
    m_magFilter(layer.m_magFilter)
{
}

crLayer::~crLayer()
{
}

CRCore::crBoundingSphere crLayer::getBound(bool treatAsElevationLayer) const
{
    CRCore::crBoundingSphere bs;
    if (!getLocator()) return bs;
    
    if (treatAsElevationLayer)
    {
        CRCore::crBoundingBox bb;
        unsigned int numColumns = getNumColumns();
        unsigned int numRows = getNumRows();
        for(unsigned int r=0;r<numRows;++r)
        {
            for(unsigned int c=0;c<numColumns;++c)
            {
                float value = 0.0f;
                bool validValue = getValidValue(c,r, value);
                if (validValue) 
                {
                    CRCore::crVector3d ndc, v;
                    ndc.x() = ((double)c)/(double)(numColumns-1), 
                    ndc.y() = ((double)r)/(double)(numRows-1);
                    ndc.z() = value;

                    if (getLocator()->convertLocalToModel(ndc, v))
                    {
                        bb.expandBy(v);
                    }
                }
            }
        }
        bs.expandBy(bb);
    }
    else
    {
    
        CRCore::crVector3d v;
        if (getLocator()->convertLocalToModel(CRCore::crVector3d(0.5,0.5,0.0), v))
        {
            bs.center() = v;
        }

        if (getLocator()->convertLocalToModel(CRCore::crVector3d(0.0,0.0,0.0), v))
        {
            bs.radius() = (bs.center() - v).length();
        }

    }
        
    return bs;
}


/////////////////////////////////////////////////////////////////////////////
//
// crImageLayer
//
crImageLayer::crImageLayer(CRCore::crImage* image):
    m_image(image)
{
}

crImageLayer::crImageLayer(const crImageLayer& imageLayer,const CRCore::crCopyOp& copyop):
    crLayer(imageLayer, copyop),
    m_image(imageLayer.m_image)
{
}

void crImageLayer::setImage(CRCore::crImage* image)
{
    m_image = image;
}

template <typename T, class O>    
void _processRow(unsigned int num, GLenum pixelFormat, T* data,const O& operation)
{
    switch(pixelFormat)
    {
        case(GL_LUMINANCE):         { for(unsigned int i=0;i<num;++i) { operation(*data++); } }  break;
        case(GL_ALPHA):             { for(unsigned int i=0;i<num;++i) { operation(*data++); } }  break;
        case(GL_LUMINANCE_ALPHA):   { for(unsigned int i=0;i<num;++i) { operation(*data++); operation(*data++); } }  break;
        case(GL_RGB):               { for(unsigned int i=0;i<num;++i) { operation(*data++); operation(*data++); operation(*data++); } }  break;
        case(GL_RGBA):              { for(unsigned int i=0;i<num;++i) { operation(*data++); operation(*data++); operation(*data++); operation(*data++);  } }  break;
        case(GL_BGR):               { for(unsigned int i=0;i<num;++i) { operation(*data++); operation(*data++); operation(*data++); } }  break;
        case(GL_BGRA):              { for(unsigned int i=0;i<num;++i) { operation(*data++); operation(*data++); operation(*data++); operation(*data++);  } }  break;
    }
}

template <class O>    
void processRow(unsigned int num, GLenum pixelFormat, GLenum dataType, unsigned char* data, const O& operation)
{
    switch(dataType)
    {
        case(GL_BYTE):              _processRow(num,pixelFormat, (char*)data,            operation); break;
        case(GL_UNSIGNED_BYTE):     _processRow(num,pixelFormat, (unsigned char*)data,   operation); break;
        case(GL_SHORT):             _processRow(num,pixelFormat, (short*) data,          operation); break;
        case(GL_UNSIGNED_SHORT):    _processRow(num,pixelFormat, (unsigned short*)data,  operation); break;
        case(GL_INT):               _processRow(num,pixelFormat, (int*) data,            operation); break;
        case(GL_UNSIGNED_INT):      _processRow(num,pixelFormat, (unsigned int*) data,   operation); break;
        case(GL_FLOAT):             _processRow(num,pixelFormat, (float*) data,          operation); break;
    }
}

template <class O>    
void processImage(CRCore::crImage* image, const O& operation)
{
    if (!image) return;
    
    for(int r=0;r<image->r();++r)
    {
        for(int t=0;t<image->t();++t)
        {
            processRow(image->s(), image->getPixelFormat(), image->getDataType(), image->data(0,t,r), operation);
        }
    }
}

struct TransformOperator
{
    TransformOperator(float offset, float scale):
        _offset(offset),
        _scale(scale) {}

    inline void operator() (unsigned char& v) const { v = (unsigned char)(_offset + (float)v * _scale); }
    inline void operator() (unsigned short& v) const { v = (unsigned short)(_offset + (float)v * _scale); }
    inline void operator() (unsigned int& v) const { v = (unsigned int)(_offset + (float)v * _scale); }
    inline void operator() (char& v) const { v = (char)(_offset + (float)v * _scale); }
    inline void operator() (short& v) const { v = (short)(_offset + (float)v * _scale); }
    inline void operator() (int& v) const { v = (int)(_offset + (float)v * _scale); }
    inline void operator() (float& v) const { v = _offset + v * _scale; }
    
    float _offset, _scale;
};


bool crImageLayer::transform(float offset, float scale)
{
    if (!m_image.valid()) return false;

    CRCore::notify(CRCore::NOTICE)<<"crImageLayer::transform("<<offset<<","<<scale<<")"<<std::endl;;

    processImage(m_image.get(), TransformOperator(offset,scale));
    
    dirty();

    return true;
}

bool crImageLayer::getValue(unsigned int i, unsigned int j, float& value) const
{
    const unsigned char* data = m_image->data(i,j);
    switch(m_image->getDataType())
    {
        case(GL_BYTE): 
            value = *((const char*)data); 
            // CRCore::notify(CRCore::NOTICE)<<"byte "<<value<<std::endl;
            break;
        case(GL_UNSIGNED_BYTE): 
            value = *data;
            // CRCore::notify(CRCore::NOTICE)<<"Unsigned byte "<<value<<std::endl;
            break;
        case(GL_SHORT):
            value = *((const short*)data);
            // CRCore::notify(CRCore::NOTICE)<<"Short "<<value<<std::endl;
            break;
        case(GL_UNSIGNED_SHORT):
            value = *((const unsigned short*)data);
            // CRCore::notify(CRCore::NOTICE)<<"Unsigned Short "<<value<<std::endl;
            break;
        case(GL_INT):
            value = *((const int*)data);
            // CRCore::notify(CRCore::NOTICE)<<"Int "<<value<<std::endl;
            break;
        case(GL_UNSIGNED_INT):
            // CRCore::notify(CRCore::NOTICE)<<"Unsigned Int "<<value<<std::endl;
            value = *((const unsigned int*)data);
            break;
        case(GL_FLOAT):
            // CRCore::notify(CRCore::NOTICE)<<"Float "<<value<<std::endl;
            value = *((const float*)data);
            break;
        default: 
            value = m_defaultValue.x(); 
            return false;
    }

    return true;
}

bool crImageLayer::getValue(unsigned int i, unsigned int j, CRCore::crVector2f& value) const
{
    CRCore::notify(CRCore::NOTICE)<<"Not implemented yet"<<std::endl;
    return false;
}

bool crImageLayer::getValue(unsigned int i, unsigned int j, CRCore::crVector3f& value) const
{
    CRCore::notify(CRCore::NOTICE)<<"Not implemented yet"<<std::endl;
    return false;
}

bool crImageLayer::getValue(unsigned int i, unsigned int j, CRCore::crVector4f& value) const
{
    CRCore::notify(CRCore::NOTICE)<<"Not implemented yet"<<std::endl;
    return false;
}

void crImageLayer::dirty()
{
    if (m_image.valid()) m_image->dirty();
}

void crImageLayer::setModifiedCount(unsigned int value)
{
    if (!m_image) return;
    else m_image->setModifiedCount(value);
}

unsigned int crImageLayer::getModifiedCount() const
{
    if (!m_image) return 0;
    else return m_image->getModifiedCount();
}


/////////////////////////////////////////////////////////////////////////////
//
// crContourLayer
//
crContourLayer::crContourLayer(CRCore::crTransferFunction1D* tf):
    m_tf(tf)
{
    m_minFilter = CRCore::crTexture::NEAREST;
    m_magFilter = CRCore::crTexture::NEAREST;
}

crContourLayer::crContourLayer(const crContourLayer& contourLayer,const CRCore::crCopyOp& copyop):
    crLayer(contourLayer, copyop),
    m_tf(contourLayer.m_tf)
{
}

void crContourLayer::setTransferFunction(CRCore::crTransferFunction1D* tf)
{
    m_tf = tf;
}

bool crContourLayer::transform(float offset, float scale)
{
    if (!m_tf) return false;

    CRCore::notify(CRCore::NOTICE)<<"crContourLayer::transform("<<offset<<","<<scale<<")"<<std::endl;;

    CRCore::crTransferFunction1D::ColorMap newColorMap = m_tf->getColorMap();
    for(CRCore::crTransferFunction1D::ColorMap::iterator itr = newColorMap.begin();
        itr != newColorMap.end();
        ++itr)
    {
        CRCore::crVector4f& value = itr->second;
        value.r() = offset + value.r()* scale;
        value.g() = offset + value.g()* scale;
        value.b() = offset + value.b()* scale;
        value.a() = offset + value.a()* scale;
    }

    m_tf->assign(newColorMap);

    dirty();

    return true;
}

bool crContourLayer::getValue(unsigned int i, unsigned int j, float& value) const
{
    if (!m_tf) return false;

    const CRCore::crVector4f& v = m_tf->getPixelValue(i);
    value = v[0];

    return true;
}

bool crContourLayer::getValue(unsigned int i, unsigned int j, CRCore::crVector2f& value) const
{
    if (!m_tf) return false;

    const CRCore::crVector4f& v = m_tf->getPixelValue(i);
    value.x() = v.x();
    value.y() = v.y();

    return true;
}

bool crContourLayer::getValue(unsigned int i, unsigned int j, CRCore::crVector3f& value) const
{
    if (!m_tf) return false;

    const CRCore::crVector4f& v = m_tf->getPixelValue(i);
    value.x() = v.x();
    value.y() = v.y();
    value.z() = v.z();

    return true;
}

bool crContourLayer::getValue(unsigned int i, unsigned int j, CRCore::crVector4f& value) const
{
    if (!m_tf) return false;

    value = m_tf->getPixelValue(i);

    return true;
}

void crContourLayer::dirty()
{
    if (getImage()) getImage()->dirty();
}

void crContourLayer::setModifiedCount(unsigned int value)
{
    if (getImage()) getImage()->setModifiedCount(value);
}

unsigned int crContourLayer::getModifiedCount() const
{
    if (!getImage()) return 0;
    else return getImage()->getModifiedCount();
}


/////////////////////////////////////////////////////////////////////////////
//
// crHeightFieldLayer
//
crHeightFieldLayer::crHeightFieldLayer(CRCore::crHeightField* hf):
    m_modifiedCount(0),
    m_heightField(hf)
{
}

crHeightFieldLayer::crHeightFieldLayer(const crHeightFieldLayer& hfLayer,const CRCore::crCopyOp& copyop):
    crLayer(hfLayer,copyop),
    m_modifiedCount(0),
    m_heightField(hfLayer.m_heightField)
{
    if (m_heightField.valid()) ++m_modifiedCount;
}


void crHeightFieldLayer::setHeightField(CRCore::crHeightField* hf)
{
    m_heightField = hf;
    dirty();
}



bool crHeightFieldLayer::transform(float offset, float scale)
{
    if (!m_heightField) return false;

    CRCore::FloatArray* heights = m_heightField->getFloatArray();
    if (!heights) return false;
    
    CRCore::notify(CRCore::NOTICE)<<"crHeightFieldLayer::transform("<<offset<<","<<scale<<")"<<std::endl;;

    for(CRCore::FloatArray::iterator itr = heights->begin();
        itr != heights->end();
        ++itr)
    {
        *itr = offset + (*itr) * scale;
    }
    
    dirty();

    return true;
}

bool crHeightFieldLayer::getValue(unsigned int i, unsigned int j, float& value) const
{
    value = m_heightField->getHeight(i,j);
    return true;
}

bool crHeightFieldLayer::getValue(unsigned int i, unsigned int j, CRCore::crVector2f& value) const
{
    value.x() = m_heightField->getHeight(i,j);
    value.y() = m_defaultValue.y();
    return true;
}

bool crHeightFieldLayer::getValue(unsigned int i, unsigned int j, CRCore::crVector3f& value) const
{
    value.x() = m_heightField->getHeight(i,j);
    value.y() = m_defaultValue.y();
    value.z() = m_defaultValue.z();
    return true;
}

bool crHeightFieldLayer::getValue(unsigned int i, unsigned int j, CRCore::crVector4f& value) const
{
    value.x() = m_heightField->getHeight(i,j);
    value.y() = m_defaultValue.y();
    value.z() = m_defaultValue.z();
    value.w() = m_defaultValue.w();
    return true;
}

void crHeightFieldLayer::dirty()
{
    ++m_modifiedCount;
}

void crHeightFieldLayer::setModifiedCount(unsigned int value)
{
    m_modifiedCount = value;
}

unsigned int crHeightFieldLayer::getModifiedCount() const
{
    return m_modifiedCount;
}

/////////////////////////////////////////////////////////////////////////////
//
// crProxyLayer
//
crProxyLayer::crProxyLayer()
{
}

crProxyLayer::crProxyLayer(const crProxyLayer& proxyLayer,const CRCore::crCopyOp& copyop):
    crLayer(proxyLayer,copyop)
{
}

crProxyLayer::~crProxyLayer()
{
}

void crProxyLayer::setFileName(const std::string& filename)
{
    m_filename = filename;
    if (m_implementation.valid())
    {
        m_implementation->setFileName(m_filename);
    }
}

unsigned int crProxyLayer::getNumColumns() const
{
    if (m_implementation.valid()) return m_implementation->getNumColumns();
    else return 0;
}

unsigned int crProxyLayer::getNumRows() const
{
    if (m_implementation.valid()) return m_implementation->getNumRows();
    else return 0;
}

bool crProxyLayer::transform(float offset, float scale)
{
    if (m_implementation.valid()) return m_implementation->transform(offset,scale);
    else return false;
}

bool crProxyLayer::getValue(unsigned int i, unsigned int j, float& value) const
{
    if (m_implementation.valid()) return m_implementation->getValue(i,j,value);
    else return false;
}

bool crProxyLayer::getValue(unsigned int i, unsigned int j, CRCore::crVector2f& value) const
{
    if (m_implementation.valid()) return m_implementation->getValue(i,j,value);
    else return false;
}

bool crProxyLayer::getValue(unsigned int i, unsigned int j, CRCore::crVector3f& value) const
{
    if (m_implementation.valid()) return m_implementation->getValue(i,j,value);
    else return false;
}

bool crProxyLayer::getValue(unsigned int i, unsigned int j, CRCore::crVector4f& value) const
{
    if (m_implementation.valid()) return m_implementation->getValue(i,j,value);
    else return false;
}

void crProxyLayer::dirty()
{
    if (m_implementation.valid()) m_implementation->dirty();
}

void crProxyLayer::setModifiedCount(unsigned int value)
{
    if (m_implementation.valid()) m_implementation->setModifiedCount(value);
}

unsigned int crProxyLayer::getModifiedCount() const
{
    return m_implementation.valid() ? m_implementation->getModifiedCount() : 0;
}


CRCore::crBoundingSphere crProxyLayer::getBound(bool treatAsElevationLayer) const
{
    if (m_implementation.valid()) return m_implementation->getBound(treatAsElevationLayer);
    else return CRCore::crBoundingSphere();
}



/////////////////////////////////////////////////////////////////////////////
//
// crCompositeLayer
//
crCompositeLayer::crCompositeLayer()
{
}

crCompositeLayer::crCompositeLayer(const crCompositeLayer& compositeLayer,const CRCore::crCopyOp& copyop):
    crLayer(compositeLayer,copyop)
{
}


void crCompositeLayer::clear()
{
    m_layers.clear();
}

void crCompositeLayer::setCompoundName(unsigned int i, const std::string& compoundname)
{
    std::string setname;
    std::string filename;
    extractSetNameAndFileName(compoundname, setname, filename);
    
    m_layers[i].setname = setname;
    m_layers[i].filename = filename;
}

std::string crCompositeLayer::getCompoundName(unsigned int i) const
{
    return createCompondSetNameAndFileName(m_layers[i].setname, m_layers[i].filename);
}

void crCompositeLayer::addLayer(const std::string& compoundname)
{
    std::string setname;
    std::string filename;
    extractSetNameAndFileName(compoundname, setname, filename);

    m_layers.push_back(CompoundNameLayer(setname,filename,0));
}

void crCompositeLayer::addLayer(const std::string& setname, const std::string& filename)
{
    m_layers.push_back(CompoundNameLayer(setname,filename,0));
}

/////////////////////////////////////////////////////////////////////////////
//
// crSwitchLayer
//
crSwitchLayer::crSwitchLayer():
    m_activeLayer(-1)
{
}

crSwitchLayer::crSwitchLayer(const crSwitchLayer& switchLayer,const CRCore::crCopyOp& copyop):
    crCompositeLayer(switchLayer,copyop),
    m_activeLayer(switchLayer.m_activeLayer)
{
}
