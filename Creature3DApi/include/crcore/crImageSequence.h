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

#ifndef CRCORE_CRIMAGESEQUENCE_H
#define CRCORE_CRIMAGESEQUENCE_H 1

#include <CRCore/thread/crMutex.h>
#include <CRCore/crImageStream.h>

#include <list>
#include <set>

namespace CRCore {

/**
  * crImage Buffer class.
*/
class CR_EXPORT crImageSequence : public crImageStream
{
    public:
        crImageSequence();

        /** Copy constructor using CopyOp to manage deep vs shallow copy. */
        crImageSequence(const crImageSequence& crImageSequence, const crCopyOp& copyop=crCopyOp::SHALLOW_COPY);

        virtual crBase* cloneType() const { return new crImageSequence(); }
        virtual crBase* clone(const crCopyOp& copyop) const { return new crImageSequence(*this,copyop); }
        virtual bool isSameKindAs(const crBase* obj) const { return dynamic_cast<const crImageSequence*>(obj)!=0; }
        virtual const char* libraryName() const { return "CRCore"; }
        virtual const char* className() const { return "crImageSequence"; }

        /** Return -1 if *this < *rhs, 0 if *this==*rhs, 1 if *this>*rhs. */
        virtual int compare(const crImage& rhs) const;

        virtual void setReferenceTime(double t) { m_referenceTime = t; }
        virtual double getReferenceTime() const { return m_referenceTime; }
                
        virtual void setTimeMultiplier(double tm) { m_timeMultiplier = tm; }
        virtual double getTimeMultiplier() const { return m_timeMultiplier; }

        typedef std::vector< CRCore::ref_ptr<CRCore::crImage> >   Images;
        typedef std::vector< std::string >                FileNames;
        
        virtual void seek(double time);

        virtual void play();

        virtual void pause();

        virtual void rewind();
        
        enum Mode
        {
            PRE_LOAD_ALL_IMAGES,
            PAGE_AND_RETAIN_IMAGES,
            PAGE_AND_DISCARD_USED_IMAGES
        };
        
        void setMode(Mode mode);
        Mode getMode() const { return m_mode; }

        void setLength(double length);
        virtual double getLength() const { return m_length; }
        

        void addImageFile(const std::string& fileName);
        
        void setImageFile(unsigned int pos, const std::string& fileName);
        std::string getImageFile(unsigned int pos) const;
        
        unsigned int getNumImageFiles() const { return m_fileNames.size(); }

        FileNames& getFileNames() { return m_fileNames; }
        const FileNames& getFileNames() const { return m_fileNames; }

        void addImage(CRCore::crImage* image);
        
        void setImage(int s,int t,int r,
                      GLint internalTextureformat,
                      GLenum pixelFormat,GLenum type,
                      unsigned char* data,
                      AllocationMode mode,
                      int packing=1) { crImage::setImage(s,t,r,internalTextureformat, pixelFormat, type, data, mode, packing); }

        void setImage(unsigned int pos, CRCore::crImage* image);
        crImage* getImage(unsigned int pos);
        const crImage* getImage(unsigned int pos) const;
        
        unsigned int getNumImages() const { return m_images.size(); }

        Images& getImages() { return m_images; }
        const Images& getImages() const { return m_images; }

        /** crImageSequence requires a call to update(NodeVisitor*) during the update traversal so return true.*/
        virtual bool requiresUpdateCall() const { return true; }

        /** update method for CRCore::crImage subclasses that update themselves during the update traversal.*/
        virtual void update(crNodeVisitor* nv);

    protected:

        virtual ~crImageSequence() {}
        
        virtual void applyLoopingMode();

        void setImageToChild(const CRCore::crImage* image);
        
        void computeTimePerImage();

        int imageIndex(double time);


        double                          m_referenceTime;
        double                          m_timeMultiplier;

        Mode                            m_mode;
        double                          m_length;

        double                          m_timePerImage;

        mutable crMutex      m_mutex;
        FileNames                       m_fileNames;

        Images                          m_images;
        
        typedef std::set< std::string > FilesRequested;
        FilesRequested                  m_filesRequested;
        
        int                             m_previousAppliedImageIndex;
                
        
        bool                            m_seekTimeSet;
        double                          m_seekTime;
        
        

};

} // namespace

#endif
