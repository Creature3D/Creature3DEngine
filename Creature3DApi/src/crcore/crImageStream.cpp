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
#include <CRCore/crImageStream.h>

using namespace CRCore;

crImageStream::crImageStream():
    m_status(INVALID),
    m_loopingMode(LOOPING)
{
    setDataVariance(DYNAMIC); 
    
#ifndef __APPLE__
    // disabled under OSX for time being while we resolve why PBO
    // doesn't function properly under OSX.
    setPixelBufferObject(new crPixelBufferObject(this));
#endif

}

crImageStream::crImageStream(const crImageStream& image,const crCopyOp& copyop):
    crImage(image,copyop),
    m_status(image.m_status),
    m_loopingMode(image.m_loopingMode),
    m_audioStreams(image.m_audioStreams)
{
}

int crImageStream::compare(const crImage& rhs) const
{
    return crImage::compare(rhs);
}

