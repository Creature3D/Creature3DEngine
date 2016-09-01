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
#include <CRIOManager/crFieldReaderIterator.h>

using namespace CRIOManager;

crFieldReaderIterator::crFieldReaderIterator()
{
    _init();
}


crFieldReaderIterator::crFieldReaderIterator(const crFieldReaderIterator& ic)
{
    _copy(ic);
}


crFieldReaderIterator::~crFieldReaderIterator()
{
    _free();
}


crFieldReaderIterator& crFieldReaderIterator::operator = (const crFieldReaderIterator& ic)
{
    if (this==&ic) return *this;
    _free();
    _copy(ic);
    return *this;
}


void crFieldReaderIterator::_free()
{
    // free all data
    if (m_previousField)
    {
        delete m_previousField;
    }
    if (m_fieldQueue)
    {
        for(int i=0;i<m_fieldQueueCapacity;++i)
        {
            if (m_fieldQueue[i]) delete m_fieldQueue[i];
            m_fieldQueue[i] = NULL;
        }
        delete [] m_fieldQueue;
    }
    _init();

}


void crFieldReaderIterator::_init()
{
    m_previousField = NULL;
    m_fieldQueue = NULL;
    m_fieldQueueSize = 0;
    m_fieldQueueCapacity = 0;

}


void crFieldReaderIterator::_copy(const crFieldReaderIterator& ic)
{
    m_reader = ic.m_reader;

    if (ic.m_previousField)
    {
        m_previousField = new crField(*ic.m_previousField);
    }

    if (ic.m_fieldQueue && ic.m_fieldQueueCapacity>0)
    {
        m_fieldQueue = new crField* [ic.m_fieldQueueCapacity];
        for(int i=0;i<ic.m_fieldQueueCapacity;++i)
        {
            if (ic.m_fieldQueue[i])
            {
                m_fieldQueue[i] = new crField(*ic.m_fieldQueue[i]);
            }
            else
            {
                m_fieldQueue[i] = NULL;
            }
        }
        m_fieldQueueSize = ic.m_fieldQueueSize;
        m_fieldQueueCapacity = ic.m_fieldQueueCapacity;
    }
    else
    {
        m_fieldQueue = NULL;
        m_fieldQueueSize = 0;
        m_fieldQueueCapacity = 0;
    }

}


void crFieldReaderIterator::attach(std::istream* input)
{
    m_reader.attach(input);
}


void crFieldReaderIterator::detach()
{
    m_reader.detach();
}


bool crFieldReaderIterator::eof() const
{
    return m_fieldQueueSize==0 && m_reader.eof();
}


void crFieldReaderIterator::insert(int pos,crField* field)
{
    if (field==NULL) return;

    if (pos<0) pos=0;
    if (pos>m_fieldQueueSize) pos=m_fieldQueueSize;

    int i;
                                 // need to reallocate the stack
    if (m_fieldQueueSize>=m_fieldQueueCapacity)
    {
        int newCapacity = m_fieldQueueCapacity*2;
        if (newCapacity<MINIMUM_FIELD_READER_QUEUE_SIZE) newCapacity = MINIMUM_FIELD_READER_QUEUE_SIZE;
        while(m_fieldQueueSize>=newCapacity) newCapacity*=2;
        crField** newFieldStack = new crField* [newCapacity];
        for(i=0;i<m_fieldQueueCapacity;++i)
        {
            newFieldStack[i] = m_fieldQueue[i];
        }
        for(;i<newCapacity;++i)
        {
            newFieldStack[i] = NULL;
        }

        // free the old memory.
        delete [] m_fieldQueue;

        m_fieldQueue = newFieldStack;
        m_fieldQueueCapacity = newCapacity;
    }

    for(i=m_fieldQueueSize-1;i>=pos;++i)
    {
        m_fieldQueue[i+1]=m_fieldQueue[i];
    }
    m_fieldQueue[pos] = field;
    ++m_fieldQueueSize;
}


void crFieldReaderIterator::insert(int pos,const char* str)
{
    if (str)
    {
        crField* field = new crField;
        while(*str!=0)
        {
            field->addChar(*str);
            ++str;
        }
        insert(pos,field);
    }
}


crField& crFieldReaderIterator::operator [] (int pos)
{
    return field(pos);
}


crField& crFieldReaderIterator::field (int pos)
{
    if (pos<0)
    {
        m_blank.setNoNestedBrackets(m_reader.getNoNestedBrackets());
        return m_blank;
    }                            // can directly access field
    else if (pos<m_fieldQueueSize)
    {
        return *m_fieldQueue[pos];
    }                            // need to read the new fields.
    else
    {
                                 // need to reallocate the stack
        if (pos>=m_fieldQueueCapacity)
        {
            int newCapacity = m_fieldQueueCapacity*2;
            if (newCapacity<MINIMUM_FIELD_READER_QUEUE_SIZE) newCapacity = MINIMUM_FIELD_READER_QUEUE_SIZE;
            while(m_fieldQueueSize>=newCapacity) newCapacity*=2;
            crField** newFieldStack = new crField* [newCapacity];
            int i;
            for(i=0;i<m_fieldQueueCapacity;++i)
            {
                newFieldStack[i] = m_fieldQueue[i];
            }
            for(;i<newCapacity;++i)
            {
                newFieldStack[i] = NULL;
            }
            // free the old memory.
            delete [] m_fieldQueue;

            m_fieldQueue = newFieldStack;
            m_fieldQueueCapacity = newCapacity;
        }
        while(!m_reader.eof() && pos>=m_fieldQueueSize)
        {
            if (m_fieldQueue[m_fieldQueueSize]==NULL) m_fieldQueue[m_fieldQueueSize] = new crField;
            if (m_reader.readField(*m_fieldQueue[m_fieldQueueSize]))
            {
                ++m_fieldQueueSize;
            }
        }
        if (pos<m_fieldQueueSize)
        {
            return *m_fieldQueue[pos];
        }
        else
        {
            m_blank.setNoNestedBrackets(m_reader.getNoNestedBrackets());
            return m_blank;
        }
    }
}


crFieldReaderIterator& crFieldReaderIterator::operator ++ ()
{
    return (*this)+=1;
}


crFieldReaderIterator& crFieldReaderIterator::operator += (int no)
{
    if (no>m_fieldQueueSize)
    {
        while (!m_reader.eof() && no>m_fieldQueueSize)
        {
            m_reader.ignoreField();
            --no;
        }
        m_fieldQueueSize=0;
    }
    else if (no>0)
    {
        crField** tmpFields = new crField* [no];
        int i;
        for(i=0;i<no;++i)
        {
            tmpFields[i] = m_fieldQueue[i];
        }
        for(i=no;i<m_fieldQueueSize;++i)
        {
            m_fieldQueue[i-no] = m_fieldQueue[i];
        }
        m_fieldQueueSize -= no;
        for(i=0;i<no;++i)
        {
            m_fieldQueue[m_fieldQueueSize+i] = tmpFields[i];
        }
        delete [] tmpFields;
    }
    return *this;
}


// increments the itetor of the next simple field or
// whole block if the current field[0] is an open bracket
void crFieldReaderIterator::advanceOverCurrentFieldOrBlock()
{
    if (field(0).isOpenBracket())
    {
        advanceToEndOfCurrentBlock();
        ++(*this); // skip the trailing '}'
    }
    else ++(*this);
}


void crFieldReaderIterator::advanceToEndOfCurrentBlock()
{
    int entry = field(0).getNoNestedBrackets();
    while(!eof() && field(0).getNoNestedBrackets()>=entry)
    {
        ++(*this);
    }
}


void crFieldReaderIterator::advanceToEndOfBlock(int noNestedBrackets)
{
    while(!eof() && field(0).getNoNestedBrackets()>=noNestedBrackets)
    {
        ++(*this);
    }
}


bool crFieldReaderIterator::matchSequence(const char* str)
{
    if (str==NULL) return false;
    if (*str==0) return false;
    
    int fieldCount = 0;
    const char* end = str;
    while((*end)!=0 && (*end)==' ') ++end;
    const char* start = end;
    while((*start)!=0)
    {
        if (*end!=' ' && *end!=0)
        {
            ++end;
        }
        else
        {
            if (start!=end)
            {
                if (end-start>1 && *start=='%')
                {
                    const char type = *(start+1);
                    switch(type)
                    {
                                 // expecting an integer
                        case('i') :
                        {
                            if (!field(fieldCount).isInt()) return false;
                            break;
                        }
                                 // expecting an floating point number
                        case('f') :
                        {
                            if (!field(fieldCount).isFloat()) return false;
                            break;
                        }
                                 // expecting an quoted string
                        case('s') :
                        {
                            if (!field(fieldCount).isQuotedString()) return false;
                            break;
                        }
                        case('w') :
                        default :// expecting an word
                        {
                            if (!field(fieldCount).isWord()) return false;
                            break;
                        }
                    }

                }
                else
                {
                    if (*start=='{')
                    {
                        if (!field(fieldCount).isOpenBracket()) return false;
                    }
                    else if (*start=='}')
                    {
                        if (!field(fieldCount).isCloseBracket()) return false;
                    }
                    else
                    {
                        if (!field(fieldCount).matchWord(start,end-start)) return false;
                    }
                }
                fieldCount++;
            }
            while((*end)==' ') ++end;
            start = end;
        }
    }
    return true;
}


bool crFieldReaderIterator::readSequence(const char* keyword,std::string& value)
{
    if ((*this)[0].matchWord(keyword) && (*this)[1].isString())
    {
        value = (*this)[1].getStr();
        (*this)+=2;
        return true;
    }
    return false;
}

bool crFieldReaderIterator::readSequence(const char* keyword,unsigned int& value)
{
    if ((*this)[0].matchWord(keyword) && (*this)[1].getUInt(value))
    {
        (*this)+=2;
        return true;
    }
    return false;
}

bool crFieldReaderIterator::readSequence(const char* keyword,int& value)
{
    if ((*this)[0].matchWord(keyword) && (*this)[1].getInt(value))
    {
        (*this)+=2;
        return true;
    }
    return false;
}

bool crFieldReaderIterator::readSequence(const char* keyword,float& value)
{
    if ((*this)[0].matchWord(keyword) &&
        (*this)[1].getFloat(value))
    {
        (*this)+=2;
        return true;
    }
    return false;
}

bool crFieldReaderIterator::readSequence(const char* keyword,CRCore::crVector2f& value)
{
    if ((*this)[0].matchWord(keyword) &&
        (*this)[1].getFloat(value[0]) &&
        (*this)[2].getFloat(value[1]))
    {
        (*this)+=3;
        return true;
    }
    return false;
}

bool crFieldReaderIterator::readSequence(const char* keyword,CRCore::crVector3f& value)
{
    if ((*this)[0].matchWord(keyword) &&
        (*this)[1].getFloat(value[0]) &&
        (*this)[2].getFloat(value[1]) &&
        (*this)[3].getFloat(value[2]))
    {
        (*this)+=4;
        return true;
    }
    return false;
}

bool crFieldReaderIterator::readSequence(const char* keyword,CRCore::crVector4f& value)
{
    if ((*this)[0].matchWord(keyword) &&
        (*this)[1].getFloat(value[0]) &&
        (*this)[2].getFloat(value[1]) &&
        (*this)[3].getFloat(value[2]) &&
        (*this)[4].getFloat(value[3]))
    {
        (*this)+=5;
        return true;
    }
    return false;
}

bool crFieldReaderIterator::readSequence(const char* keyword,CRCore::crVector2d& value)
{
    if ((*this)[0].matchWord(keyword) &&
        (*this)[1].getFloat(value[0]) &&
        (*this)[2].getFloat(value[1]))
    {
        (*this)+=3;
        return true;
    }
    return false;
}

bool crFieldReaderIterator::readSequence(const char* keyword,CRCore::crVector3d& value)
{
    if ((*this)[0].matchWord(keyword) &&
        (*this)[1].getFloat(value[0]) &&
        (*this)[2].getFloat(value[1]) &&
        (*this)[3].getFloat(value[2]))
    {
        (*this)+=4;
        return true;
    }
    return false;
}

bool crFieldReaderIterator::readSequence(const char* keyword,CRCore::crVector4d& value)
{
    if ((*this)[0].matchWord(keyword) &&
        (*this)[1].getFloat(value[0]) &&
        (*this)[2].getFloat(value[1]) &&
        (*this)[3].getFloat(value[2]) &&
        (*this)[4].getFloat(value[3]))
    {
        (*this)+=5;
        return true;
    }
    return false;
}

bool crFieldReaderIterator::readSequence(std::string& value)
{
    if ((*this)[0].isString())
    {
        value = (*this)[0].getStr();
        (*this)+=1;
        return true;
    }
    return false;
}

bool crFieldReaderIterator::readSequence(unsigned int& value)
{
    if ((*this)[0].getUInt(value))
    {
        (*this)+=1;
        return true;
    }
    return false;
}

bool crFieldReaderIterator::readSequence(int& value)
{
    if ((*this)[0].getInt(value))
    {
        (*this)+=1;
        return true;
    }
    return false;
}

bool crFieldReaderIterator::readSequence(float& value)
{
    if ((*this)[0].getFloat(value))
    {
        (*this)+=1;
        return true;
    }
    return false;
}

bool crFieldReaderIterator::readSequence(CRCore::crVector2f& value)
{
    if ((*this)[0].getFloat(value[0]) &&
        (*this)[1].getFloat(value[1]))
    {
        (*this)+=2;
        return true;
    }
    return false;
}

bool crFieldReaderIterator::readSequence(CRCore::crVector3f& value)
{
    if ((*this)[0].getFloat(value[0]) &&
        (*this)[1].getFloat(value[1]) &&
        (*this)[2].getFloat(value[2]))
    {
        (*this)+=3;
        return true;
    }
    return false;
}

bool crFieldReaderIterator::readSequence(CRCore::crVector4f& value)
{
    if ((*this)[0].getFloat(value[0]) &&
        (*this)[1].getFloat(value[1]) &&
        (*this)[2].getFloat(value[2]) &&
        (*this)[3].getFloat(value[3]))
    {
        (*this)+=4;
        return true;
    }
    return false;
}


bool crFieldReaderIterator::readSequence(CRCore::crVector2d& value)
{
    if ((*this)[0].getFloat(value[0]) &&
        (*this)[1].getFloat(value[1]))
    {
        (*this)+=2;
        return true;
    }
    return false;
}

bool crFieldReaderIterator::readSequence(CRCore::crVector3d& value)
{
    if ((*this)[0].getFloat(value[0]) &&
        (*this)[1].getFloat(value[1]) &&
        (*this)[2].getFloat(value[2]))
    {
        (*this)+=3;
        return true;
    }
    return false;
}

bool crFieldReaderIterator::readSequence(CRCore::crVector4d& value)
{
    if ((*this)[0].getFloat(value[0]) &&
        (*this)[1].getFloat(value[1]) &&
        (*this)[2].getFloat(value[2]) &&
        (*this)[3].getFloat(value[3]))
    {
        (*this)+=4;
        return true;
    }
    return false;
}
