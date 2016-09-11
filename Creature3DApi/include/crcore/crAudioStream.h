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

#ifndef CRCORE_AUDIOSTREAM
#define CRCORE_AUDIOSTREAM 1

#include <CRCore/crImage.h>
#include <stdlib.h>

namespace CRCore {

/** Pure virtual crAudioSink bass class that is used to connect the audio system with AudioStreams. */
class CR_EXPORT crAudioSink : public CRCore::crBase
{
public:

    crAudioSink();

    virtual const char * libraryName() const { return "CRCore"; }
    virtual const char * className() const { return "crAudioSink"; }

    virtual void play() = 0;
    virtual void pause()  = 0;
    virtual void stop()  = 0;

    virtual bool playing() const = 0;

    virtual double getDelay() const { return m_delay; }
    virtual void setDelay(const double delay) { m_delay = delay; }

    virtual void setVolume(float) {}
    virtual float getVolume() const { return 0.0f; }

private:

    virtual crAudioSink * cloneType() const { return 0; }
    virtual crAudioSink * clone(const CRCore::crCopyOp &) const { return 0; }

    double  m_delay;
};

/** Pure virtual crAudioStream base class. Subclasses provide mechanism for reading/generating audio data*/
class CR_EXPORT crAudioStream : public CRCore::crBase
{
    public:
        crAudioStream();

        /** Copy constructor using CopyOp to manage deep vs shallow copy. */
        crAudioStream(const crAudioStream& audio,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY);

        virtual bool isSameKindAs(const crBase* obj) const { return dynamic_cast<const crAudioStream*>(obj)!=0; }
        virtual const char* libraryName() const { return "CRCore"; }
        virtual const char* className() const { return "crAudioStream"; }

        virtual void setAudioSink(CRCore::crAudioSink* audio_sink) = 0;
        
        virtual void consumeAudioBuffer(void * const buffer, const size_t size) = 0;
        
        virtual int audioFrequency() const = 0;
        virtual int audioNbChannels() const = 0;

        enum SampleFormat
        {
            SAMPLE_FORMAT_U8,
            SAMPLE_FORMAT_S16,
            SAMPLE_FORMAT_S24,
            SAMPLE_FORMAT_S32,
            SAMPLE_FORMAT_F32
        };

        virtual SampleFormat audioSampleFormat() const = 0;
};

} // namespace

#endif
