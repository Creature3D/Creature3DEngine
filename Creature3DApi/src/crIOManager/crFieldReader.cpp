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
#include <CRIOManager/crField.h>
#include <CRIOManager/crFieldReader.h>

using namespace CRIOManager;

crFieldReader::crFieldReader()
{
    _init();
}


crFieldReader::crFieldReader(const crFieldReader& ic)
{
    _copy(ic);
}


crFieldReader::~crFieldReader()
{
    _free();
}


crFieldReader& crFieldReader::operator = (const crFieldReader& ic)
{
    if (this==&ic) return *this;
    _free();
    _copy(ic);
    return *this;
}


void crFieldReader::_free()
{
    // free all data

    _init();

}


void crFieldReader::_init()
{
    m_fin = NULL;
    m_eof = true;

    m_noNestedBrackets = 0;

    int i;
    for(i=0;i<256;++i) m_delimiterEatLookUp[i]=false;
    m_delimiterEatLookUp[int(' ')] = true;
    m_delimiterEatLookUp[int('\t')] = true;
    m_delimiterEatLookUp[int('\n')] = true;
    m_delimiterEatLookUp[int('\r')] = true;

    for(i=0;i<256;++i) m_delimiterKeepLookUp[i]=false;
    m_delimiterKeepLookUp[int('{')] = true;
    m_delimiterKeepLookUp[int('}')] = true;
    m_delimiterKeepLookUp[int('"')] = true;
    m_delimiterKeepLookUp[int('\'')] = true;

}


void crFieldReader::_copy(const crFieldReader& ic)
{

    m_fin = ic.m_fin;
    m_eof = ic.m_eof;

    m_noNestedBrackets = ic.m_noNestedBrackets;

    int i;
    for(i=0;i<256;++i) m_delimiterEatLookUp[i]=ic.m_delimiterEatLookUp[i];
    for(i=0;i<256;++i) m_delimiterKeepLookUp[i]=ic.m_delimiterKeepLookUp[i];
}


void crFieldReader::attach(std::istream* input)
{
    m_fin = input;

    if (m_fin)
    {
        m_eof = m_fin->eof()!=0;
    }
    else
    {
        m_eof = true;
    }
}


void crFieldReader::detach()
{
    m_fin = NULL;
    m_eof = true;
}


bool crFieldReader::eof() const
{
    return m_eof;
}


bool crFieldReader::findStartOfNextField()
{
    int ch = 0;
    while (true)
    {
        ch = m_fin->peek();
        if (ch==EOF)
        {
            m_eof = true;
            return false;
        }
        else if (m_delimiterEatLookUp[ch])
        {
            m_fin->ignore(1);
        }
        else
        {
            return true;
        }
    }
}


bool crFieldReader::readField(crField& fieldPtr)
{
    return _readField(&fieldPtr);
}


void crFieldReader::ignoreField()
{
    _readField(NULL);
}


bool crFieldReader::_readField(crField* fieldPtr)
{
    if (fieldPtr) fieldPtr->reset();

    if (!eof() && findStartOfNextField())
    {

        int ch = m_fin->peek();
        if (ch==EOF)
        {
            m_eof = true;
            if (fieldPtr) fieldPtr->setNoNestedBrackets(getNoNestedBrackets());
            return fieldPtr && fieldPtr->getNoCharacters()!=0;
        }
        else if (ch=='"')
        {
            if (fieldPtr)
            {
                fieldPtr->setWithinQuotes(true);
                fieldPtr->setNoNestedBrackets(getNoNestedBrackets());
            }
            m_fin->ignore(1);
            char c;
	    bool escape = false; // use the escape character sequence \" to allow " to included in strings.
            while (true)
            {
                ch = m_fin->peek();
                if (ch==EOF)
                {
                    m_eof = true;
                    return fieldPtr && fieldPtr->getNoCharacters()!=0;
                }
                c = ch;	
		if (ch=='\\' && !escape)
		{
    	    	    escape = true;
                    m_fin->ignore(1);
		}
                else if (ch=='"')
                {
		    if (escape)
		    {
		    	escape = false;
                	m_fin->get(c);
                	if (fieldPtr) fieldPtr->addChar(c);
		    }
		    else
		    {
                	m_fin->ignore(1);
                	//return fieldPtr && fieldPtr->getNoCharacters()!=0;
                	return fieldPtr!=NULL;
		    }
                }
                else
                {
		    if (escape)
		    {
		    	escape = false;
                        if (fieldPtr) fieldPtr->addChar('\\');
		    }
                    m_fin->get(c);
                    if (fieldPtr) fieldPtr->addChar(c);
                }
            }
        }
        else if (ch=='\'')
        {
            if (fieldPtr)
            {
                fieldPtr->setWithinQuotes(true);
                fieldPtr->setNoNestedBrackets(getNoNestedBrackets());
            }
            m_fin->ignore(1);
            char c;
	    bool escape = false; // use the escape character sequence \' to allow ' to included in strings.
            while (true)
            {
                ch = m_fin->peek();
                if (ch==EOF)
                {
                    m_eof = true;
                    return fieldPtr && fieldPtr->getNoCharacters()!=0;
                }
                c = ch;
		if (ch=='\\' && !escape)
		{
    	    	    escape = true;
                    m_fin->ignore(1);
		}
                else if (ch=='\'')
                {
		    if (escape)
		    {
		    	escape = false;
                	m_fin->get(c);
                	if (fieldPtr) fieldPtr->addChar(c);
		    }
		    else
		    {
                	m_fin->ignore(1);
                	//return fieldPtr && fieldPtr->getNoCharacters()!=0;
                	return fieldPtr!=NULL;
		    }
                }
                else
                {
		    if (escape)
		    {
		    	escape = false;
                        if (fieldPtr) fieldPtr->addChar('\\');
		    }
                    m_fin->get(c);
                    if (fieldPtr) fieldPtr->addChar(c);
                }
            }
        }
        else if (m_delimiterKeepLookUp[ch])
        {
            char c;
            m_fin->get(c);
            if (fieldPtr) fieldPtr->addChar(c);
            if (c=='{') ++m_noNestedBrackets;
            else if (c=='}') --m_noNestedBrackets;
            if (fieldPtr) fieldPtr->setNoNestedBrackets(getNoNestedBrackets());
            return fieldPtr && fieldPtr->getNoCharacters()!=0;
        }
        else
        {
            if (fieldPtr) fieldPtr->setNoNestedBrackets(getNoNestedBrackets());
            char c;
            while (true)
            {
                ch = m_fin->peek();
                if (ch==EOF)
                {
                    m_eof = true;
                    return fieldPtr && fieldPtr->getNoCharacters()!=0;
                }
                c = ch;
                if (m_delimiterEatLookUp[int(c)])
                {
                    m_fin->ignore(1);
                    return fieldPtr && fieldPtr->getNoCharacters()!=0;
                }
                if (m_delimiterKeepLookUp[int(c)])
                {
                    return fieldPtr && fieldPtr->getNoCharacters()!=0;
                }
                else
                {
                    m_fin->get(c);
                    if (fieldPtr) fieldPtr->addChar(c);
                }
            }
        }

    }
    else
    {
        return false;
    }
}


int crFieldReader::getNoNestedBrackets() const
{
    return m_noNestedBrackets;
}
