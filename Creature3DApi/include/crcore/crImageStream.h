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
#ifndef CRCORE_CRIMAGESTREAM_H
#define CRCORE_CRIMAGESTREAM_H 1

#include <CRCore/crImage.h>
#include <CRCore/crAudioStream.h>

namespace CRCore {

/**
  * crImage Stream class.
*/
class CR_EXPORT crImageStream : public crImage
{
    public:
        crImageStream();

        /** Copy constructor using CopyOp to manage deep vs shallow copy. */
        crImageStream(const crImageStream& image,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY);

        virtual crBase* cloneType() const { return new crImageStream(); }
        virtual crBase* clone(const crCopyOp& copyop) const { return new crImageStream(*this,copyop); }
        virtual bool isSameKindAs(const crBase* obj) const { return dynamic_cast<const crImageStream*>(obj)!=0; }
        virtual const char* libraryName() const { return "CRCore"; }
        virtual const char* className() const { return "crImageStream"; }

        /** Return -1 if *this < *rhs, 0 if *this==*rhs, 1 if *this>*rhs. */
        virtual int compare(const crImage& rhs) const;

        enum StreamStatus
        {
            INVALID,
            PLAYING,
            PAUSED,
            REWINDING
        };
        
        virtual void seek(double /*time*/) {}

        virtual void play() { m_status=PLAYING; }

        virtual void pause() { m_status=PAUSED; }

        virtual void rewind() { m_status=REWINDING; }
        
        virtual void quit(bool /*waitForThreadToExit*/ = true) {}

        StreamStatus getStatus() { return m_status; }
        

        enum LoopingMode
        {
            NO_LOOPING,
            LOOPING
        };
        
        void setLoopingMode(LoopingMode mode)
        {
            if (m_loopingMode == mode) return;
            
            m_loopingMode = mode;
            applyLoopingMode();
        }
        
        LoopingMode getLoopingMode() const { return m_loopingMode; }

        virtual double getCreationTime() const { return HUGE_VAL; }
        virtual double getLength() const { return 0.0; }
        virtual double getFrameRate() const { return 0.0; } 

        virtual void setReferenceTime(double) {}
        virtual double getReferenceTime() const { return 0.0; }
                
        virtual void setTimeMultiplier(double) {}
        virtual double getTimeMultiplier() const { return 0.0; }
        
        virtual void setVolume(float) {}
        virtual float getVolume() const { return 0.0f; }

        typedef std::vector< CRCore::ref_ptr<CRCore::crAudioStream> > AudioStreams;
        void setAudioStreams(const AudioStreams& asl) { m_audioStreams = asl; }
        AudioStreams& getAudioStreams() { return m_audioStreams; }
        const AudioStreams& getAudioStreams() const { return m_audioStreams; }
        

    protected:
        virtual void applyLoopingMode() {}

        virtual ~crImageStream() {}

        StreamStatus    m_status;
        LoopingMode     m_loopingMode;
        
        AudioStreams    m_audioStreams;
};

} // namespace

#endif
