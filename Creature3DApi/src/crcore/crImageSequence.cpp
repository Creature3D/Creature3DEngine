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

#include <CRCore/thread/crScopedLock.h>
#include <CRCore/crImageSequence.h>
#include <CRCore/crNotify.h>
#include <CRCore/crCameraNode.h>
#include <CRCore/crNodeVisitor.h>
#include <CRCore/crTexture2D.h>

#include <math.h>

using namespace CRCore;

crImageSequence::crImageSequence()
{
    m_referenceTime = DBL_MAX;
    m_timeMultiplier = 1.0;

    m_mode = PRE_LOAD_ALL_IMAGES;
    m_length = 1.0;
    m_timePerImage = 1.0;
    
    m_seekTime = 0.0;
    m_seekTimeSet = false;
    
    m_previousAppliedImageIndex = -1;
}

crImageSequence::crImageSequence(const crImageSequence& is,const crCopyOp& copyop):
    CRCore::crImageStream(is,copyop),
    m_referenceTime(is.m_referenceTime),
    m_timeMultiplier(is.m_timeMultiplier),
    m_mode(is.m_mode),
    m_length(is.m_length),
    m_timePerImage(is.m_timePerImage)
{
    m_seekTime = is.m_seekTime;
    m_seekTimeSet = is.m_seekTimeSet;

    m_previousAppliedImageIndex = -1;
}

int crImageSequence::compare(const crImage& rhs) const
{
    return crImageStream::compare(rhs);
}

void crImageSequence::seek(double time)
{
    m_seekTime = time;
    m_seekTimeSet = true;
}

void crImageSequence::play()
{
    m_status=PLAYING;
}

void crImageSequence::pause()
{
    m_status=PAUSED;
}

void crImageSequence::rewind()
{
    seek(0.0f);
}

void crImageSequence::setMode(Mode mode)
{
    m_mode = mode;
}

void crImageSequence::setLength(double length)
{
    if (length<=0.0)
    {
        notify(NOTICE)<<"crImageSequence::setLength("<<length<<") invalid length value, must be greater than 0."<<std::endl;
        return;
    }
    
    m_length = length;
    computeTimePerImage();
}

void crImageSequence::computeTimePerImage()
{
    if (!m_fileNames.empty()) m_timePerImage = m_length / double(m_fileNames.size());
    else if (!m_images.empty()) m_timePerImage = m_length / double(m_images.size());
    else m_timePerImage = m_length;
}

void crImageSequence::setImageFile(unsigned int pos, const std::string& fileName)
{
    CRCore::ScopedLock<crMutex> lock(m_mutex);

    if (pos>=m_fileNames.size()) m_fileNames.resize(pos);
    m_fileNames[pos] = fileName;
}

std::string crImageSequence::getImageFile(unsigned int pos) const
{
    CRCore::ScopedLock<crMutex> lock(m_mutex);
    return pos<m_fileNames.size() ? m_fileNames[pos] : std::string();
}
        
void crImageSequence::addImageFile(const std::string& fileName)
{
    CRCore::ScopedLock<crMutex> lock(m_mutex);
    m_fileNames.push_back(fileName);
    computeTimePerImage();
}

void crImageSequence::setImage(unsigned int pos, CRCore::crImage* image)
{
    CRCore::ScopedLock<crMutex> lock(m_mutex);

    //notify(INFO)<<"crImageSequence::setImage("<<pos<<","<<image->getFileName()<<")"<<std::endl;

    if (pos>=m_images.size()) m_images.resize(pos+1);
    
    m_images[pos] = image;

    // prune from file requested list.
    FilesRequested::iterator itr = m_filesRequested.find(image->getFileName());
    if (itr!=m_filesRequested.end()) m_filesRequested.erase(itr);
}

crImage* crImageSequence::getImage(unsigned int pos)
{
    CRCore::ScopedLock<crMutex> lock(m_mutex);
    return pos<m_images.size() ? m_images[pos].get() : 0;
}

const crImage* crImageSequence::getImage(unsigned int pos) const
{
    CRCore::ScopedLock<crMutex> lock(m_mutex);
    return pos<m_images.size() ? m_images[pos].get() : 0;
}

void crImageSequence::addImage(CRCore::crImage* image)
{
    if (image==0) return;

    CRCore::ScopedLock<crMutex> lock(m_mutex);

    // notify(NOTICE)<<"merging image in order expected : "<<image->getFileName()<<std::endl;
    m_images.push_back(image);

    computeTimePerImage();
    
    if (data()==0)
    {
        setImageToChild(m_images.front().get());
    }
}

void crImageSequence::setImageToChild(const CRCore::crImage* image)
{
    // notify(NOTICE)<<"setImageToChild("<<image<<")"<<std::endl;

    if (image==0) return;

    // check to see if data is changing, if not don't apply
    if (image->data() == data()) return;

    setImage(image->s(),image->t(),image->r(),
             image->getInternalTextureFormat(),
             image->getPixelFormat(),image->getDataType(),
             const_cast<unsigned char*>(image->data()),
             CRCore::crImage::NO_DELETE,
             image->getPacking());
}

void crImageSequence::applyLoopingMode()
{
}

int crImageSequence::imageIndex(double time)
{
    if (getLoopingMode()==LOOPING) 
    {
        double positionRatio = time/m_length;
        time = (positionRatio - floor(positionRatio))*m_length;
    }

    if (time<0.0) return 0;
    int index = int(time/m_timePerImage);
    if (index>=int(m_images.size())) return int(m_images.size())-1;
    return index;
}

void crImageSequence::update(CRCore::crNodeVisitor* nv)
{
    CRCore::ScopedLock<crMutex> lock(m_mutex);
    
    CRCore::crNodeVisitor::ImageRequestHandler* irh = nv->getImageRequestHandler();
    CRCore::crFrameStamp* fs = crFrameStamp::getInstance();

    // notify(NOTICE)<<"crImageSequence::update("<<fs<<", "<<irh<<")"<<std::endl;

    if (m_referenceTime == DBL_MAX)
    {
        m_referenceTime = fs->getReferenceTime();//getSimulationTime();
    }
    
    bool looping = getLoopingMode()==LOOPING;
    double time = (fs->getReferenceTime() - m_referenceTime)*m_timeMultiplier;
    
    if (m_seekTimeSet || m_status==PAUSED || m_status==INVALID)
    {
        time = m_seekTime;        
        m_referenceTime =  fs->getReferenceTime() - time/m_timeMultiplier;
    }
    else
    {
        if (looping)
        {
            while (time>m_length)
            {
                m_referenceTime += m_length/m_timeMultiplier;
                time -= m_length;
            }
        }
        else
        {
            if (time>m_length)
            {
                m_referenceTime = fs->getReferenceTime() - m_length/m_timeMultiplier;
                time = m_length;
            }
        }
    }
    
    m_seekTime = time;
    m_seekTimeSet = false;

    bool pruneOldImages = false;
    
    
    switch(m_mode)
    {
        case(PRE_LOAD_ALL_IMAGES):
        {
            if (m_fileNames.size()>m_images.size())
            {
                FileNames::iterator itr = m_fileNames.begin();
                for(unsigned int i=0;i<m_images.size();++i) ++itr;

                for(; itr!=m_fileNames.end(); ++itr)
                {
                    CRCore::crImage* image = irh->readImageFile(*itr);
                    m_images.push_back(image);
                }
            }
        
            irh = 0;
            break;
        }
        case(PAGE_AND_RETAIN_IMAGES):
        {
            break;
        }
        case(PAGE_AND_DISCARD_USED_IMAGES):
        {
            pruneOldImages = true;
            break;
        }
    }

    int index = int(time/m_timePerImage);
    // notify(NOTICE)<<"time= "<<time<<" m_timePerImage="<<m_timePerImage<<" index="<<index<<" m_length="<<m_length<<std::endl;

    if (index>=int(m_images.size())) index = int(m_images.size())-1;

    if (index>=0 && index<int(m_images.size()))
    {

        if (pruneOldImages)
        {
            while (index>=0 && !m_images[index].valid())
            {
                --index;
            }
        }
                
        if (index>=0)
        {
            // notify(NOTICE)<<"at time "<<time<<" setting child = "<<index<<std::endl;

            if (m_previousAppliedImageIndex!=index)
            {
                if (m_previousAppliedImageIndex >= 0 && 
                    m_previousAppliedImageIndex<int(m_images.size()) && 
                    pruneOldImages)
                {
                    m_images[m_previousAppliedImageIndex] = 0;
                }
            
                setImageToChild(m_images[index].get());
                
                m_previousAppliedImageIndex = index;
            }
        }
    }

    // notify(NOTICE)<<"time = "<<time<<std::endl;

    if (irh)
    {
        double preLoadTime = time + CRCore::minimum(irh->getPreLoadTime()*m_timeMultiplier, m_length);
        
        int startLoadIndex = int(time/m_timePerImage);
        if (startLoadIndex>=int(m_images.size())) startLoadIndex = int(m_images.size())-1;
        if (startLoadIndex<0) startLoadIndex = 0;

        int endLoadIndex = int(preLoadTime/m_timePerImage);
        if (endLoadIndex>=int(m_fileNames.size())) 
        {
            if (looping)
            {
                endLoadIndex -= int(m_fileNames.size());
            }
            else
            {
                endLoadIndex = int(m_fileNames.size())-1;
            }
        }
        if (endLoadIndex<0) endLoadIndex = 0;
        
        double requestTime = time;
        
        if (endLoadIndex<startLoadIndex)
        {
            for(int i=startLoadIndex; i<int(m_fileNames.size()); ++i)
            {
                if ((i>=int(m_images.size()) || !m_images[i]) && m_filesRequested.count(m_fileNames[i])==0)
                {
                    m_filesRequested.insert(m_fileNames[i]);
                    irh->requestImageFile(m_fileNames[i], this, i, requestTime, fs);
                }
                requestTime += m_timePerImage;
            }

            for(int i=0; i<=endLoadIndex; ++i)
            {
                if ((i>=int(m_images.size()) || !m_images[i]) && m_filesRequested.count(m_fileNames[i])==0)
                {
                    m_filesRequested.insert(m_fileNames[i]);
                    irh->requestImageFile(m_fileNames[i], this, i, requestTime, fs);
                }
                requestTime += m_timePerImage;
            }
        }
        else
        {
            for(int i=startLoadIndex; i<=endLoadIndex; ++i)
            {
                if ((i>=int(m_images.size()) || !m_images[i]) && m_filesRequested.count(m_fileNames[i])==0)
                {
                    m_filesRequested.insert(m_fileNames[i]);
                    irh->requestImageFile(m_fileNames[i], this, i, requestTime, fs);
                }
                requestTime += m_timePerImage;
            }
        }


    }

}
