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
#ifndef CRIOMANAGER_FIELDREADERITERATOR
#define CRIOMANAGER_FIELDREADERITERATOR 1

#include <CRCore/crVector2.h>
#include <CRCore/crVector3.h>
#include <CRCore/crVector4.h>

#include <CRIOManager/crField.h>
#include <CRIOManager/crFieldReader.h>

namespace CRIOManager {


class CRIOMANAGER_EXPORT crFieldReaderIterator
{
    public:

        enum {
            MINIMUM_FIELD_READER_QUEUE_SIZE = 10
        };

        crFieldReaderIterator();
        crFieldReaderIterator(const crFieldReaderIterator& ic);
        virtual ~crFieldReaderIterator();

        crFieldReaderIterator& operator = (const crFieldReaderIterator& ic);

        void attach(std::istream* input);
        void detach();

        virtual bool eof() const;

        crFieldReader& getFieldReader() { return m_reader; }

        void insert(int pos,crField* field);
        void insert(int pos,const char* str);

        crField& operator [] (int pos);
        crField& field (int pos);

        crFieldReaderIterator& operator ++ ();
        crFieldReaderIterator& operator += (int no);

        /** increments the iterator of the next simple field or
          * whole block if the current field[0] is an open bracket */
        void advanceOverCurrentFieldOrBlock();
        void advanceToEndOfCurrentBlock();
        void advanceToEndOfBlock(int noNestBrackets);

        bool matchSequence(const char* str);
        
        bool readSequence(const char* keyword,std::string& value);
        bool readSequence(const char* keyword,unsigned int& value);
        bool readSequence(const char* keyword,int& value);
        bool readSequence(const char* keyword,float& value);
        bool readSequence(const char* keyword,CRCore::crVector2f& value);
        bool readSequence(const char* keyword,CRCore::crVector3f& value);
        bool readSequence(const char* keyword,CRCore::crVector4f& value);
        bool readSequence(const char* keyword,CRCore::crVector2d& value);
        bool readSequence(const char* keyword,CRCore::crVector3d& value);
        bool readSequence(const char* keyword,CRCore::crVector4d& value);

        bool readSequence(std::string& value);
        bool readSequence(unsigned int& value);
        bool readSequence(int& value);
        bool readSequence(float& value);
        bool readSequence(CRCore::crVector2f& value);
        bool readSequence(CRCore::crVector3f& value);
        bool readSequence(CRCore::crVector4f& value);
        bool readSequence(CRCore::crVector2d& value);
        bool readSequence(CRCore::crVector3d& value);
        bool readSequence(CRCore::crVector4d& value);

    private:

        void _init();
        void _free();
        void _copy(const crFieldReaderIterator& ic);

        crFieldReader m_reader;

        crField m_blank;

        crField* m_previousField;

        crField** m_fieldQueue;
        int m_fieldQueueSize;
        int m_fieldQueueCapacity;

};

}

#endif                                            // __CRIOMANAGER_FIELD_READER_QUEUE_H
