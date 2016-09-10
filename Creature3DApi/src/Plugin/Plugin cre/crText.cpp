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
#include <Plugin cre/crText.h>
#include <Plugin cre/crDrawable.h>
#include <Plugin cre/crBase.h>

#include <CRIOManager/crFileUtils.h>
#include <CRIOManager/crFileNameUtils.h>
#include <CRCore/crNotify.h>

using namespace cre;

void crText::write(crDataOutputStream* out){
    // Write crText's identification.
    out->writeInt(CRETEXT);
    // If the CRCore class is inherited by any other class we should also write this to file.
    CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
    if(obj){
        ((cre::crDrawable*)(obj))->write(out);
    }
    else
        throw crException("crText::write(): Could not cast this CRText::crText to an CRCore::crDrawable.");
    // Write crText's properties.
    if( getFont() )
    {
        std::string fname = getFont()->getFileName();

        if(!fname.empty())
        {
            if(out->getUseOriginalExternalReferences())
            {
                out->writeString(fname); //Saving file name with local directory
            }
            else
            {
                out->writeString(CRIOManager::getSimpleFileName(fname)); //Saving original file name
            }
        }
        else
            out->writeString(""); //Blank string
    }
    else
        out->writeString(""); //Blank string

    out->writeUInt(getFontWidth());
    out->writeUInt(getFontHeight());
    out->writeFloat(getCharacterHeight());
    out->writeFloat(getCharacterAspectRatio());
    out->writeUInt(getCharacterSizeMode());
    out->writeFloat(getMaximumWidth());
    out->writeFloat(getMaximumHeight());
    out->writeUInt(getAlignment());

    out->writeQuat(getRotation()); //FIXME: controllare che ci sia

    out->writeBool(getAutoRotateToScreen());
    out->writeUInt(getLayout());
    out->writeVec3(getPosition());
    out->writeVec4(getColor());
    out->writeUInt(getDrawMode());

    // text :: Modified from osgPlugins::CRCore
	const CRText::crString& textstring = getText();
    bool isACString = true;
    CRText::crString::const_iterator itr;
    for(itr=textstring.begin();
        itr!=textstring.end() && isACString;
        ++itr)
    {
        if (*itr==0 || *itr>256) isACString=false;
    }

    if (isACString)
    {
        std::string str;

        for(itr=textstring.begin();
            itr!=textstring.end();
            ++itr)
        {
            str += (char)(*itr);
        }

        //std::copy(textstring.begin(),textstring.end(),std::back_inserter(str));
        
        out->writeBool(true);
        out->writeString(str);
    }
    else
    {
        // do it the hardway...output each character as an int
        CRCore::ref_ptr<CRCore::UByteArray> strarr = new CRCore::UByteArray(textstring.size());
        
        for(itr=textstring.begin();
            itr!=textstring.end();
            ++itr)
        {
            strarr->push_back((char)(*itr));
        }

        out->writeBool(false);
        out->writeUByteArray(strarr.get());
    }
}

void crText::read(crDataInputStream* in){
    // Peek on crText's identification.
    int id = in->peekInt();
    if(id == CRETEXT){
        // Read crText's identification.
        id = in->readInt();
        // If the CRCore class is inherited by any other class we should also read this from file.
        CRCore::crBase*  obj = dynamic_cast<CRCore::crBase*>(this);
        if(obj){
            ((cre::crDrawable*)(obj))->read(in);
        }
        else
            throw crException("crText::read(): Could not cast this CRText::crText to an CRCore::crDrawable.");
        // Read crText's properties

    unsigned int width, height;
    float c_height, aspectRatio;

    setFont(in->readString());

    width = in->readUInt();
    height = in->readUInt();

    setFontResolution(width,height);

    c_height = in->readFloat();
    aspectRatio = in->readFloat();

    setCharacterSize(height,aspectRatio);

    setCharacterSizeMode((CRText::crText::CharacterSizeMode) in->readUInt());

    setMaximumWidth(in->readFloat());
    setMaximumHeight(in->readFloat());

    setAlignment((CRText::crText::AlignmentType) in->readUInt());

    //Nothing to do...
    //setAxisAlignment((CRText::crText::AxisAlignment) in->readUint());

    setRotation(in->readQuat());
    setAutoRotateToScreen(in->readBool());
    setLayout((CRText::crText::Layout) in->readUInt());

    setPosition(in->readVec3());
    setColor(in->readVec4());
    setDrawMode(in->readUInt());

    if(in->readBool())
        setText(in->readString());
    else
    {
        std::string textstr;

        CRCore::ref_ptr<CRCore::UByteArray> arr = in->readUByteArray();

        for(unsigned int i = 0; i < arr->getNumElements(); i++)
        {
            textstr += (char) arr->at(i);
        }

        setText(textstr);
    }

    }
    else{
        throw crException("crShadeModel::read(): Expected crShadeModel identification.");
    }
}
