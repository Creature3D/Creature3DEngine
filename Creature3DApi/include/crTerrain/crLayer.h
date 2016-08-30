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
#ifndef CRTERRAIN_CRLAYER_H
#define CRTERRAIN_CRLAYER_H 1

#include <CRCore/crImage.h>
#include <CRCore/crShape.h>
#include <CRCore/crArray.h>
#include <CRCore/crTransferFunction.h>

#include <CRTerrain/crLocator.h>
#include <CRTerrain/crValidDataOperator.h>

namespace CRTerrain {

#define MAXIMUM_NUMBER_OF_LEVELS 30

/** Extact the setname and filename from a compound string in the from set:setname:filename".
  * Returns a setname of "" when non set:setname: entry is present.*/
extern CRTERRAIN_EXPORT void extractSetNameAndFileName(const std::string& compoundstring, std::string& setname, std::string& filename);

/** Create a compound string in the form set:setname:filename, or just filename if setname is "".*/
extern CRTERRAIN_EXPORT std::string createCompondSetNameAndFileName(const std::string& setname, const std::string& filename);

class CRTERRAIN_EXPORT crLayer : public CRCore::crBase
{
    public:

        crLayer();

        /** Copy constructor using crCopyOp to manage deep vs shallow copy.*/
        crLayer(const crLayer&,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);
        
        META_Base(CRTerrain, crLayer);
        
        void setSetName(const std::string& setname) { setName(setname); }
        const std::string& getSetName() const { return getName(); }

        /** Set the file name of the data associated with this layer. */
        virtual void setFileName(const std::string& filename) { m_filename = filename; }

        /** Get the file name of the layer. */
        virtual const std::string& getFileName() const { return m_filename; }

        /** Return the compound name of the layer in the form set::name::filename string.*/
        std::string getCompoundName() const { return createCompondSetNameAndFileName(getName(), getFileName()); }

        void setLocator(crLocator* locator) { m_locator = locator; }
        crLocator* getLocator() { return m_locator.get(); }
        const crLocator* getLocator() const { return m_locator.get(); }
        
        void setMinLevel(unsigned int minLevel) { m_minLevel = minLevel; }
        unsigned int getMinLevel() const { return m_minLevel; }

        void setMaxLevel(unsigned int maxLevel) { m_maxLevel = maxLevel; }
        unsigned int getMaxLevel() const { return m_maxLevel; }

        void setValidDataOperator(crValidDataOperator* validDataOp) { m_validDataOperator = validDataOp; }
        crValidDataOperator* getValidDataOperator() { return m_validDataOperator.get(); }
        const crValidDataOperator* getValidDataOperator() const { return m_validDataOperator.get(); }


        virtual unsigned int getNumColumns() const { return 0; }
        virtual unsigned int getNumRows() const { return 0; }

        void setDefaultValue(const CRCore::crVector4f& value) { m_defaultValue = value; }
        const CRCore::crVector4f& getDefaultValue() const { return m_defaultValue; }
        

        /** Set the minification texture filter to use when do texture associated with this layer.*/
        void setMinFilter(CRCore::crTexture::FilterMode filter) { m_minFilter = filter; }

        /** Get the minification texture filter to use when do texture associated with this layer.*/
        CRCore::crTexture::FilterMode getMinFilter() const { return m_minFilter; }


        /** Set the magniification texture filter to use when do texture associated with this layer.*/
        void setMagFilter(CRCore::crTexture::FilterMode filter) { m_magFilter = filter; }

        /** Get the magnification texture filter to use when do texture associated with this layer.*/
        CRCore::crTexture::FilterMode getMagFilter() const { return m_magFilter; }



        /** Return image associated with layer if supported. */        
        virtual CRCore::crImage* getImage() { return 0; }

        /** Return const image associated with layer if supported. */        
        virtual const CRCore::crImage* getImage() const { return 0; }


        virtual bool transform(float /*offset*/, float /*scale*/) { return false; }


        virtual bool getValue(unsigned int /*i*/, unsigned int /*j*/, float& /*value*/) const { return false; }
        virtual bool getValue(unsigned int /*i*/, unsigned int /*j*/, CRCore::crVector2f& /*value*/) const { return false; }
        virtual bool getValue(unsigned int /*i*/, unsigned int /*j*/, CRCore::crVector3f& /*value*/) const { return false; }
        virtual bool getValue(unsigned int /*i*/, unsigned int /*j*/, CRCore::crVector4f& /*value*/) const { return false; }

        inline bool getValidValue(unsigned int i, unsigned int j, float& value) const
        {
            if (getValue(i,j,value)) return m_validDataOperator.valid() ? (*m_validDataOperator)(value) : true;
            return false;
        }

        inline bool getValidValue(unsigned int i, unsigned int j, CRCore::crVector2f& value) const
        {
            if (getValue(i,j,value)) return m_validDataOperator.valid() ? (*m_validDataOperator)(value) : true;
            return false;
        }

        inline bool getValidValue(unsigned int i, unsigned int j, CRCore::crVector3f& value) const
        {
            if (getValue(i,j,value)) return m_validDataOperator.valid() ? (*m_validDataOperator)(value) : true;
            return false;
        }

        inline bool getValidValue(unsigned int i, unsigned int j, CRCore::crVector4f& value) const
        {
            if (getValue(i,j,value)) return m_validDataOperator.valid() ? (*m_validDataOperator)(value) : true;
            return false;
        }


        inline void computeIndices(double ndc_x, double ndc_y, unsigned int& i, unsigned int& j, double& ir, double& jr)
        {
            ndc_x *= double(getNumColumns()-1);
            ndc_y *= double(getNumRows()-1);
            i = (unsigned int)(ndc_x);
            j = (unsigned int)(ndc_y);
            ir = ndc_x - double(i);
            jr = ndc_y - double(j);
        } 


        inline bool getInterpolatedValue(double ndc_x, double ndc_y, float& value)
        {
            unsigned int i,j;
            double ir, jr;
            computeIndices(ndc_x, ndc_y, i, j, ir, jr);
            value = 0.0f;
            double div = 0.0f;
            float v,r;
            
            r = (1.0f-ir)*(1.0f-jr);
            if (r>0.0 && getValue(i,j,v)) 
            {
                value += v*r;
                div += r;
            }
            
            r = (ir)*(1.0f-jr);
            if (r>0.0 && getValue(i+1,j,v)) 
            {
                value += v*r;
                div += r;
            }
            
            r = (ir)*(jr);
            if (r>0.0 && getValue(i+1,j+1,v)) 
            {
                value += v*r;
                div += r;
            }
            
            r = (1.0f-ir)*(jr);
            if (r>0.0 && getValue(i,j+1,v)) 
            {
                value += v*r;
                div += r;
            }
            
            if (div != 0.0)
            {
                value /= div;
                return true;
            }
            
            value = 0.0;
            return false;
        }

        /** increment the modified count."*/
        virtual void dirty() {};

        /** Set the modified count value.  */
        virtual void setModifiedCount(unsigned int /*value*/) {};

        /** Get modified count value. */
        virtual unsigned int getModifiedCount() const { return 0; }

        virtual CRCore::crBoundingSphere getBound(bool treatAsElevationLayer) const;

    protected:

        virtual ~crLayer();

        std::string                     m_filename;
        CRCore::ref_ptr<crLocator>           m_locator;
        unsigned int                    m_minLevel;
        unsigned int                    m_maxLevel;
        CRCore::ref_ptr<crValidDataOperator> m_validDataOperator;
        CRCore::crVector4f                       m_defaultValue;
        CRCore::crTexture::FilterMode        m_minFilter;
        CRCore::crTexture::FilterMode        m_magFilter;

};

class CRTERRAIN_EXPORT crImageLayer : public crLayer
{
    public:

        crImageLayer(CRCore::crImage* image=0);

        /** Copy constructor using crCopyOp to manage deep vs shallow copy.*/
        crImageLayer(const crImageLayer& imageLayer,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);
        
        META_Base(CRTerrain, crImageLayer);

        void setFileName(const std::string& filename) { m_filename = filename; if (m_image.valid()) m_image->setFileName(filename); }
        virtual const std::string& getFileName() const { return m_image.get() ? m_image->getFileName() : m_filename; }

        virtual bool transform(float offset, float scale);

        void setImage(CRCore::crImage* image);

        /** Return image associated with layer. */  
        virtual CRCore::crImage* getImage() { return m_image.get(); }

        /** Return const image associated with layer. */
        virtual const CRCore::crImage* getImage() const { return m_image.get(); }

        virtual unsigned int getNumColumns() const { return m_image.valid() ? m_image->s() : 0; }
        virtual unsigned int getNumRows() const { return m_image.valid() ? m_image->t() : 0;  }

        virtual bool getValue(unsigned int i, unsigned int j, float& value) const;
        virtual bool getValue(unsigned int i, unsigned int j, CRCore::crVector2f& value) const;
        virtual bool getValue(unsigned int i, unsigned int j, CRCore::crVector3f& value) const;
        virtual bool getValue(unsigned int i, unsigned int j, CRCore::crVector4f& value) const;

        virtual void dirty();
        virtual void setModifiedCount(unsigned int value);
        virtual unsigned int getModifiedCount() const;

    protected:

        virtual ~crImageLayer() {}

        CRCore::ref_ptr<CRCore::crImage>    m_image;

};

class CRTERRAIN_EXPORT crContourLayer : public crLayer
{
    public:

        crContourLayer(CRCore::crTransferFunction1D* tf=0);

        /** Copy constructor using crCopyOp to manage deep vs shallow copy.*/
        crContourLayer(const crContourLayer& tfLayer,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);
        
        META_Base(CRTerrain, crContourLayer);

        virtual bool transform(float offset, float scale);

        void setTransferFunction(CRCore::crTransferFunction1D* tf);
        CRCore::crTransferFunction1D* getTransferFunction() { return m_tf.get(); }
        const CRCore::crTransferFunction1D* getTransferFunction() const { return m_tf.get(); }

        /** Return image associated with layer. */  
        virtual CRCore::crImage* getImage() { return m_tf.valid() ? m_tf->getImage() : 0; }

        /** Return const image associated with layer. */
        virtual const CRCore::crImage* getImage() const { return m_tf.valid() ? m_tf->getImage() : 0; }


        virtual unsigned int getNumColumns() const { return m_tf.valid() ? m_tf->getNumberImageCells() : 0; }
        virtual unsigned int getNumRows() const { return m_tf.valid() ? 1 : 0;  }

        virtual bool getValue(unsigned int i, unsigned int j, float& value) const;
        virtual bool getValue(unsigned int i, unsigned int j, CRCore::crVector2f& value) const;
        virtual bool getValue(unsigned int i, unsigned int j, CRCore::crVector3f& value) const;
        virtual bool getValue(unsigned int i, unsigned int j, CRCore::crVector4f& value) const;

        virtual void dirty();
        virtual void setModifiedCount(unsigned int value);
        virtual unsigned int getModifiedCount() const;

    protected:

        virtual ~crContourLayer() {}

        CRCore::ref_ptr<CRCore::crTransferFunction1D>    m_tf;

};

class CRTERRAIN_EXPORT crHeightFieldLayer : public crLayer
{
    public:

        crHeightFieldLayer(CRCore::crHeightField* hf=0);

        /** Copy constructor using crCopyOp to manage deep vs shallow copy.*/
        crHeightFieldLayer(const crHeightFieldLayer& hfLayer,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);
        
        META_Base(CRTerrain, crHeightFieldLayer);

        void setFileName(const std::string& filename) { m_filename = filename; }
        virtual const std::string& getFileName() const { return m_filename; }

        virtual bool transform(float offset, float scale);

        void setHeightField(CRCore::crHeightField* hf);
        CRCore::crHeightField* getHeightField() { return m_heightField.get(); }
        const CRCore::crHeightField* getHeightField() const { return m_heightField.get(); }

        virtual unsigned int getNumColumns() const { return m_heightField.valid() ? m_heightField->getNumColumns() : 0; }
        virtual unsigned int getNumRows() const { return m_heightField.valid() ? m_heightField->getNumRows() : 0;  }

        virtual bool getValue(unsigned int i, unsigned int j, float& value) const;
        virtual bool getValue(unsigned int i, unsigned int j, CRCore::crVector2f& value) const;
        virtual bool getValue(unsigned int i, unsigned int j, CRCore::crVector3f& value) const;
        virtual bool getValue(unsigned int i, unsigned int j, CRCore::crVector4f& value) const;

        virtual void dirty();
        virtual void setModifiedCount(unsigned int value);
        virtual unsigned int getModifiedCount() const;

    protected:

        virtual ~crHeightFieldLayer() {}

        unsigned int                    m_modifiedCount;
        CRCore::ref_ptr<CRCore::crHeightField>  m_heightField;

};


class CRTERRAIN_EXPORT crProxyLayer : public crLayer
{
    public:

        crProxyLayer();

        /** Copy constructor using crCopyOp to manage deep vs shallow copy.*/
        crProxyLayer(const crProxyLayer& proxyLayer,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);
        
        META_Base(CRTerrain, crProxyLayer);
        
        /** Return image associated with layer if supported. */        
        virtual CRCore::crImage* getImage()
        {
            return m_implementation.valid() ? m_implementation->getImage() : 0;
        }

        /** Return const image associated with layer if supported. */
        virtual const CRCore::crImage* getImage() const
        {
            return m_implementation.valid() ? m_implementation->getImage() : 0;
        }

        /** Set the implementation layer that does the actual work.*/
        void setImplementation(crLayer* layer) { m_implementation = layer; }
        
        /** Get the implementation layer that does the actual work.*/
        crLayer* getImplementation() { return m_implementation.get(); }

        /** Get the const implementation layer that does the actual work.*/
        const crLayer* getImplementation() const { return m_implementation.get(); }

        virtual void setFileName(const std::string& filename);
        virtual const std::string& getFileName() const { return m_filename; }

        virtual unsigned int getNumColumns() const;
        virtual unsigned int getNumRows() const;
        
        virtual bool transform(float offset, float scale);

        virtual bool getValue(unsigned int i, unsigned int j, float& value) const;
        virtual bool getValue(unsigned int i, unsigned int j, CRCore::crVector2f& value) const;
        virtual bool getValue(unsigned int i, unsigned int j, CRCore::crVector3f& value) const;
        virtual bool getValue(unsigned int i, unsigned int j, CRCore::crVector4f& value) const;

        virtual void dirty();
        virtual void setModifiedCount(unsigned int value);
        virtual unsigned int getModifiedCount() const;

        virtual CRCore::crBoundingSphere getBound(bool treatAsElevationLayer) const;

    protected:

        virtual ~crProxyLayer();
        
        CRCore::ref_ptr<crLayer> m_implementation;


};

class CRTERRAIN_EXPORT crCompositeLayer : public crLayer
{
    public:

        crCompositeLayer();

        /** Copy constructor using crCopyOp to manage deep vs shallow copy.*/
        crCompositeLayer(const crCompositeLayer& compositeLayer,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);
        
        META_Base(CRTerrain, crCompositeLayer);

        void clear();

        void setSetName(const std::string& setname) { setName(setname); }
        const std::string& getSetName() const { return getName(); }

        void setSetName(unsigned int i, const std::string& setname) { m_layers[i].setname = setname; if (m_layers[i].layer.valid()) m_layers[i].layer->setName(setname); }
        const std::string& getSetName(unsigned int i) const { return m_layers[i].layer.valid() ? m_layers[i].layer->getName() : m_layers[i].setname; }

        void setFileName(unsigned int i, const std::string& filename) { m_layers[i].filename = filename; if (m_layers[i].layer.valid()) m_layers[i].layer->setFileName(filename); }
        const std::string& getFileName(unsigned int i) const { return m_layers[i].layer.valid() ? m_layers[i].layer->getFileName() : m_layers[i].filename; }

        void setCompoundName(unsigned int i, const std::string& compoundname);
        std::string getCompoundName(unsigned int i) const;


        void setLayer(unsigned int i, crLayer* layer) { if (i>=m_layers.size()) m_layers.resize(i+1); m_layers[i].layer = layer; }
        crLayer* getLayer(unsigned int i) { return i<m_layers.size() ? m_layers[i].layer.get() : 0; }
        const crLayer* getLayer(unsigned int i) const { return i<m_layers.size() ? m_layers[i].layer.get() : 0; }

        void addLayer(const std::string& compoundname);
        void addLayer(const std::string& setname, const std::string& filename);
        
        void addLayer(crLayer* layer) { m_layers.push_back(CompoundNameLayer(layer->getName(),layer->getFileName(),layer)); }

        void removeLayer(unsigned int i) { m_layers.erase(m_layers.begin()+i); }
        
        unsigned int getNumLayers() const { return m_layers.size(); }

    protected:

        virtual ~crCompositeLayer() {}

        struct CompoundNameLayer
        {
            CompoundNameLayer() {}
        
            CompoundNameLayer(const CompoundNameLayer& cnl):
                setname(cnl.setname),
                filename(cnl.filename),
                layer(cnl.layer) {}

            CompoundNameLayer(const std::string& sn, const std::string& fn, crLayer* l):
                setname(sn),
                filename(fn),
                layer(l) {}

            CompoundNameLayer& operator = (const CompoundNameLayer& cnl)
            {
                if (&cnl==this) return *this;
                
                setname = cnl.setname;
                filename = cnl.filename;
                layer = cnl.layer;
                return *this;
            }

            std::string         setname;
            std::string         filename;
            CRCore::ref_ptr<crLayer> layer;
        };
        
        typedef std::vector< CompoundNameLayer > Layers;
        
        Layers m_layers;
};


class CRTERRAIN_EXPORT crSwitchLayer : public crCompositeLayer
{
    public:

        crSwitchLayer();

        /** Copy constructor using crCopyOp to manage deep vs shallow copy.*/
        crSwitchLayer(const crSwitchLayer& switchLayer,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);
        
        META_Base(CRTerrain, crSwitchLayer);
        
        void setActiveLayer(int i) { m_activeLayer = i; }
        int getActiveLayer() const { return m_activeLayer; }

        /** Return image associated with layer if supported. */        
        virtual CRCore::crImage* getImage()
        {
            if (m_activeLayer < 0) return 0;            
            if (m_activeLayer >= static_cast<int>(getNumLayers())) return 0;
            return m_layers[m_activeLayer].layer->getImage();            
        }

        /** Return const image associated with layer if supported. */
        virtual const CRCore::crImage* getImage() const
        {
            if (m_activeLayer < 0) return 0;            
            if (m_activeLayer >= static_cast<int>(getNumLayers())) return 0;
            return m_layers[m_activeLayer].layer->getImage();            
        }

    protected:

        virtual ~crSwitchLayer() {}
        
        int m_activeLayer;
};



}

#endif
