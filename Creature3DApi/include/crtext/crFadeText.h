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

#ifndef CRTEXT_CRFADETEXT
#define CRTEXT_CRFADETEXT 1

#include <CRText/crText.h>

namespace CRText {


class CRTEXT_EXPORT crFadeText : public CRText::crText
{
public:

    crFadeText();
    crFadeText(const crText& text,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);

    META_Node(CRText,crFadeText)


    /** Set the speed that the alpha value changes as the text is occluded or becomes visible.*/
    void setFadeSpeed(float fadeSpeed) { m_fadeSpeed = fadeSpeed; }

    /** Get the speed that the alpha value changes.*/
    float getFadeSpeed() const { return m_fadeSpeed; }

    /** Draw the text.*/
	virtual void drawImplementation(CRCore::crState& state) const;

protected:

    virtual ~crFadeText() {}

    void init();
  
    struct FadeTextUpdateCallback;
    friend struct FadeTextUpdateCallback;

    typedef std::map<CRCore::View*, CRCore::crVector4 > ViewBlendColourMap;
    
    ViewBlendColourMap& getViewBlendColourMap() { return m_viewBlendColourMap; }
    const ViewBlendColourMap& getViewBlendColourMap() const { return m_viewBlendColourMap; }

    float m_fadeSpeed;
    
    mutable ViewBlendColourMap m_viewBlendColourMap;
};

}


#endif
