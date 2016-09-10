/* Creature3D - Online Game Engine, Copyright (C) 2005 Wucaihua(26756325@qq.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
*/

#include <Plugin cre/crException.h>
#include <Plugin cre/crCameraNode.h>
#include <Plugin cre/crTransform.h>
#include <Plugin cre/crImage.h>

using namespace cre;

void crCameraNode::write(crDataOutputStream* out){
    // Write crCameraNode's identification.
    out->writeInt(CRECAMERANODE);

    // If the CRCore class is inherited by any other class we should also write this to file.
    CRCore::crTransform* transform = dynamic_cast<CRCore::crTransform*>(this);
    if(transform){
        ((cre::crTransform*)(transform))->write(out);
    }
    else
        throw crException("crCameraNode::write(): Could not cast this CRCore::crCameraNode to an CRCore::Group.");


    out->writeVec4(getClearColor());
    out->writeUInt(getClearMask());

    out->writeBool(getColorMask()!=0);
    if (getColorMask()!=0)
    {
        out->writeStateAttribute(getColorMask());
    }

    out->writeBool(getViewport()!=0);
    if (getViewport()!=0)
    {
        out->writeStateAttribute(getViewport());
    }
    
    out->writeInt(getTransformOrder());

    // Write crCameraNode's properties.
    out->writeMatrixd(*getProjectionMatrix());
    out->writeMatrixd(*getViewMatrix());
	out->writeMatrixd(*getBufProjectionMatrix());
	out->writeMatrixd(*getBufViewMatrix());

    out->writeInt(getRenderOrder());
    out->writeInt(getRenderTargetImplementation());
    out->writeInt(getRenderTargetFallback());
 
    out->writeUInt(getDrawBuffer());
    out->writeUInt(getReadBuffer());
     
    const BufferAttachmentMap& baf = getBufferAttachmentMap();
    out->writeUInt(baf.size());
    for(BufferAttachmentMap::const_iterator itr = baf.begin();
        itr != baf.end();
        ++itr)
    {
        BufferComponent buffer = itr->first;
        const Attachment& attachment = itr->second;

        out->writeInt( buffer );
        out->writeUInt( attachment.m_internalFormat);

        // this ain't right... what if we need to share images????
        out->writeBool(attachment.m_image.valid());
        if(attachment.m_image.valid())
            ((cre::crImage*)attachment.m_image.get())->write(out);

        out->writeBool(attachment.m_texture.valid());
        if(attachment.m_texture.valid())
            out->writeStateAttribute(attachment.m_texture.get());
            
        out->writeUInt(attachment.m_level);
        out->writeUInt(attachment.m_face);
        out->writeBool(attachment.m_mipMapGeneration);
    }
     
}

void crCameraNode::read(crDataInputStream* in)
{
    // Read crCameraNode's identification.
    int id = in->peekInt();
    if(id == CRECAMERANODE)
    {
        // Code to read crCameraNode's properties.
        id = in->readInt();
        // If the CRCore class is inherited by any other class we should also read this from file.
        CRCore::crTransform*  transform = dynamic_cast<CRCore::crTransform*>(this);
        if(transform)
        {
            ((cre::crTransform*)(transform))->read(in);
        }
        else
            throw crException("crCameraNode::read(): Could not cast this CRCore::crCameraNode to an CRCore::Group.");

        setClearColor(in->readVec4());
        setClearMask(in->readUInt());

        if (in->readBool())
        {
            CRCore::ref_ptr<CRCore::crStateAttribute> attribute = in->readStateAttribute();
            CRCore::crColorMask* cm = dynamic_cast<CRCore::crColorMask*>(attribute.get());
            if (cm) setColorMask(cm);
        }

        if (in->readBool())
        {
            CRCore::ref_ptr<CRCore::crStateAttribute> attribute = in->readStateAttribute();
            CRCore::crViewPort* vp = dynamic_cast<CRCore::crViewPort*>(attribute.get());
            if (vp) setViewport(vp);
        }
    
        setTransformOrder((TransformOrder)in->readInt());

        // Read matrix
        setProjectionMatrix(in->readMatrixd());
        setViewMatrix(in->readMatrixd());
		setBufProjectionMatrix(in->readMatrixd());
		setBufViewMatrix(in->readMatrixd());

    
        setRenderOrder((RenderOrder)in->readInt());
    
        RenderTargetImplementation impl = (RenderTargetImplementation)in->readInt();
        RenderTargetImplementation fallback = (RenderTargetImplementation)in->readInt();
        setRenderTargetImplementation(impl, fallback);
 
        setDrawBuffer((GLenum)in->readUInt());
        setReadBuffer((GLenum)in->readUInt());
     
        m_bufferAttachmentMap.clear();
        
        unsigned int numAttachments = in->readUInt();
        for(unsigned int i=0; i<numAttachments; ++i)
        {
            Attachment& attachment = m_bufferAttachmentMap[(BufferComponent)in->readInt()];
            attachment.m_internalFormat = (GLenum)in->readUInt();
            
            if (in->readBool())
            {
                // this ain't right... what if we need to share images????
                attachment.m_image = new CRCore::crImage;
               ((cre::crImage*)attachment.m_image.get())->read(in);
            }

            if (in->readBool())
            {
                CRCore::ref_ptr<CRCore::crStateAttribute> attribute = in->readStateAttribute();
                CRCore::crTexture* texture = dynamic_cast<CRCore::crTexture*>(attribute.get());
                if (texture) attachment.m_texture = texture;
            }

            attachment.m_level = in->readUInt();
            attachment.m_face = in->readUInt();
            attachment.m_mipMapGeneration = in->readBool();
        }
    }
    else{
        throw crException("crCameraNode::read(): Expected crCameraNode identification");
    }
}
