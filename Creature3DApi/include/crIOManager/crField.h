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
#ifndef CRIOMANAGER_FIELD
#define CRIOMANAGER_FIELD 1

#include <CRIOManager/crExport.h>

#include <string>
#include <stdlib.h>

namespace CRIOManager {


class CRIOMANAGER_EXPORT crField
{
    public:

        enum {
            MIN_CACHE_SIZE = 256
        };

        crField();
        crField(const crField& field);
        virtual ~crField();

        virtual crField& operator = (const crField& ic);

        void reset();
        void addChar(char c);
        int getNoCharacters() const { return m_fieldCacheSize; }

        void setWithinQuotes(bool withinQuotes=true);
        bool getWithinQuotes();

        void setNoNestedBrackets(int no);
        int getNoNestedBrackets();

        enum FieldType
        {
            OPEN_BRACKET,
            CLOSE_BRACKET,
            STRING,
            WORD,
            REAL,
            INTEGER,
            BLANK,
            UNINITIALISED
        };

        FieldType getFieldType() const;

        bool isValid() const;

        bool isOpenBracket() const;
        bool isCloseBracket() const;

        bool isWord() const;
        bool matchWord(const char* str) const;
        bool matchWord(const char* str,int noCharacters) const;

        bool isString() const;
        bool matchString(const char* str) const;
        bool matchString(const char* str,int noCharacters) const;
        bool isQuotedString() const;

        const char* getStr() const;
        char* takeStr();

        bool isInt() const;
        bool matchInt(int i) const;
        bool getInt(int& i) const;

        bool isUInt() const;
        bool matchUInt(unsigned int i) const;
        bool getUInt(unsigned int& i) const;

        bool isFloat() const;
        bool matchFloat(float f) const;
        bool getFloat(float& f) const;
        bool getFloat(double& f) const;

        static FieldType calculateFieldType(const char* str,bool withinQuotes=false);

    protected:

        void _init();
        void _free();
        void _copy(const crField& ic);

        int m_fieldCacheCapacity;
        int m_fieldCacheSize;
        char* m_fieldCache;

        mutable FieldType m_fieldType;

        bool m_withinQuotes;

        int m_noNestedBrackets;

};

}

#endif                                            // __SG_FIELD_H
