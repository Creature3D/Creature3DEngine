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

using namespace CRIOManager;
using namespace std;

crField::crField()
{
    _init();
}


crField::crField(const crField& ic)
{
    _copy(ic);
}


crField::~crField()
{
    _free();
}


crField& crField::operator = (const crField& ic)
{
    if (this==&ic) return *this;
    _free();
    _copy(ic);
    return *this;
}


void crField::_free()
{
    // free all data
    if (m_fieldCache) delete [] m_fieldCache;

    _init();

}


void crField::_init()
{

    m_fieldCacheCapacity = 256;
    m_fieldCacheSize = 0;
    m_fieldCache = NULL;

    m_fieldType = UNINITIALISED;

    m_withinQuotes = false;

    m_noNestedBrackets = 0;

}


void crField::_copy(const crField& ic)
{

    // copy string cache.
    if (ic.m_fieldCache)
    {
        m_fieldCacheCapacity = ic.m_fieldCacheCapacity;
        m_fieldCacheSize = ic.m_fieldCacheSize;
        m_fieldCache = new char [m_fieldCacheCapacity];
        strncpy(m_fieldCache,ic.m_fieldCache,m_fieldCacheCapacity);
    }
    else
    {
        m_fieldCacheCapacity = 0;
        m_fieldCacheSize = 0;
        m_fieldCache = NULL;
    }

    m_fieldType = ic.m_fieldType;

    m_withinQuotes = ic.m_withinQuotes;

    m_noNestedBrackets = ic.m_noNestedBrackets;
}


void crField::setWithinQuotes(bool withinQuotes)
{
    m_withinQuotes=withinQuotes;
    m_fieldType = UNINITIALISED;
}


bool crField::getWithinQuotes()
{
    return m_withinQuotes;
}


void crField::setNoNestedBrackets(int no)
{
    m_noNestedBrackets=no;
}


int crField::getNoNestedBrackets()
{
    return m_noNestedBrackets;
}


const char* crField::getStr() const
{
    if (m_fieldCacheSize!=0) return m_fieldCache;
    else return NULL;
}


char* crField::takeStr()
{
    char* field = m_fieldCache;

    m_fieldCache = NULL;
    m_fieldCacheSize = 0;

    m_fieldType = UNINITIALISED;
    m_withinQuotes = false;

    return field;
}


void crField::reset()
{
    m_fieldCacheSize = 0;
    if (m_fieldCache)
    {
        m_fieldCache[m_fieldCacheSize] = 0;
    }

    m_withinQuotes = false;
    m_noNestedBrackets = 0;
}


void crField::addChar(char c)
{
    if (m_fieldCache==NULL)
    {
        if (m_fieldCacheCapacity<MIN_CACHE_SIZE) m_fieldCacheCapacity=MIN_CACHE_SIZE;
        m_fieldCache = new char[m_fieldCacheCapacity];
        memset(m_fieldCache,0,m_fieldCacheCapacity);
        m_fieldCacheSize = 0;
    }
    else if (m_fieldCacheSize>=m_fieldCacheCapacity-1)
    {
        if (m_fieldCacheCapacity<MIN_CACHE_SIZE) m_fieldCacheCapacity=MIN_CACHE_SIZE;
        while (m_fieldCacheSize>=m_fieldCacheCapacity-1) m_fieldCacheCapacity *= 2;
        char* tmp_str = m_fieldCache;
        m_fieldCache = new char[m_fieldCacheCapacity];
        memset(m_fieldCache,0,m_fieldCacheCapacity);
        strncpy(m_fieldCache,tmp_str,m_fieldCacheSize);
        delete [] tmp_str;

    }
    m_fieldCache[m_fieldCacheSize++] = c;
    m_fieldCache[m_fieldCacheSize] = 0;
    m_fieldType = UNINITIALISED;
}


crField::FieldType crField::getFieldType() const
{
    if (m_fieldType==UNINITIALISED && m_fieldCache)
    {
        m_fieldType = calculateFieldType(m_fieldCache,m_withinQuotes);
    }
    return m_fieldType;
}


bool crField::isValid() const
{
    if (m_fieldCacheSize>0  && !m_withinQuotes) return true;
    else return false;
}


bool crField::isOpenBracket() const
{
    if (m_fieldCacheSize==1) return m_fieldCache[0]=='{';
    else return false;
}


bool crField::isCloseBracket() const
{
    if (m_fieldCacheSize==1) return m_fieldCache[0]=='}';
    else return false;
}


bool crField::isWord() const
{
    getFieldType();
    return (m_fieldType==WORD);
}


bool crField::matchWord(const char* str) const
{
    getFieldType();
    return m_fieldType==WORD && strcmp(m_fieldCache,str)==0;
}


bool crField::matchWord(const char* str,int noCharacters) const
{
    getFieldType();
    return m_fieldType==WORD && strncmp(m_fieldCache,str,noCharacters)==0;
}


bool crField::isString() const
{
    return getNoCharacters()!=0;
}


bool crField::matchString(const char* str) const
{
    return strcmp(m_fieldCache,str)==0;
}


bool crField::matchString(const char* str,int noCharacters) const
{
    return strncmp(m_fieldCache,str,noCharacters)==0;
}


bool crField::isQuotedString() const
{
    return m_withinQuotes;
}


bool crField::isInt() const
{
    getFieldType();
    return m_fieldType==INTEGER;
}


bool crField::matchInt(int i) const
{
    getFieldType();
    if (m_fieldType==INTEGER)
    {
        return strtol(m_fieldCache,NULL,0)==i;
    }
    else
    {
        return false;
    }
}


bool crField::getInt(int& i) const
{
    getFieldType();
    if (m_fieldType==INTEGER)
    {
        i = strtol(m_fieldCache,NULL,0);
        return true;
    }
    else
    {
        return false;
    }
}

bool crField::isUInt() const
{
    getFieldType();
    return m_fieldType==INTEGER;
}


bool crField::matchUInt(unsigned int i) const
{
    getFieldType();
    if (m_fieldType==INTEGER)
    {
        return (unsigned int) strtoul(m_fieldCache,NULL,0)==i;
    }
    else
    {
        return false;
    }
}


bool crField::getUInt(unsigned int& i) const
{
    getFieldType();
    if (m_fieldType==INTEGER)
    {
        i = strtoul(m_fieldCache,NULL,0);
        return true;
    }
    else
    {
        return false;
    }
}

bool crField::isFloat() const
{
    getFieldType();
    return m_fieldType==REAL || m_fieldType==INTEGER;
}


bool crField::matchFloat(float f) const
{
    getFieldType();
    if (m_fieldType==REAL || m_fieldType==INTEGER)
    {
        return (float)atof(m_fieldCache)==f;
    }
    else
    {
        return false;
    }
}


bool crField::getFloat(float& f) const
{
    getFieldType();
    if (m_fieldType==REAL || m_fieldType==INTEGER)
    {
        f = (float)atof(m_fieldCache);
        return true;
    }
    else
    {
        return false;
    }
}

bool crField::getFloat(double& f) const
{
    getFieldType();
    if (m_fieldType==REAL || m_fieldType==INTEGER)
    {
        f = atof(m_fieldCache);
        return true;
    }
    else
    {
        return false;
    }
}


crField::FieldType crField::calculateFieldType(const char* str,bool withinQuotes)
{
    if (str==NULL) return BLANK;
    if (*str==0) return BLANK;

    if (withinQuotes) return STRING;

    bool hadPlusMinus = false;
    bool hadDecimalPlace = false;
    bool hadExponent = false;
    bool couldBeInt = true;
    bool couldBeFloat = true;
    int noZeroToNine = 0;

    const char* ptr = str;
    
    // check if could be a hex number.
    if (strncmp(ptr,"0x",2)==0)
    {
        // skip over leading 0x, and then go through rest of string
        // checking to make sure all values are 0...9 or a..f.
        ptr+=2;
        while (
               *ptr!=0 &&
               ((*ptr>='0' && *ptr<='9') ||
                (*ptr>='a' && *ptr<='f') ||  
                (*ptr>='A' && *ptr<='F'))
              )
        {
            ++ptr;
        }
        
        // got to end of string without failure, therefore must be a hex integer.
        if (*ptr==0) return INTEGER;
    }
    
    ptr = str;
    // check if a float or an int.
    while (*ptr!=0 && couldBeFloat)
    {
        if (*ptr=='+' || *ptr=='-')
        {
            if (hadPlusMinus)
            {
                couldBeInt = false;
                couldBeFloat = false;
            } else hadPlusMinus = true;
        }
        else if (*ptr>='0' && *ptr<='9')
        {
            noZeroToNine++;
        }
        else if (*ptr=='.')
        {
            if (hadDecimalPlace)
            {
                couldBeInt = false;
                couldBeFloat = false;
            }
            else
            {
                hadDecimalPlace = true;
                couldBeInt = false;
            }
        }
        else if (*ptr=='e' || *ptr=='E')
        {
            if (hadExponent || noZeroToNine==0)
            {
                couldBeInt = false;
                couldBeFloat = false;
            }
            else
            {
                hadExponent = true;
                couldBeInt = false;
                hadDecimalPlace = false;
                hadPlusMinus = false;
                noZeroToNine=0;
            }
        }
        else
        {
            couldBeInt = false;
            couldBeFloat = false;
        }
        ++ptr;
    }

    if (couldBeInt && noZeroToNine>0) return INTEGER;
    if (couldBeFloat && noZeroToNine>0) return REAL;
    if (str[0]=='{') return OPEN_BRACKET;
    if (str[0]=='}') return CLOSE_BRACKET;
    return WORD;
}
