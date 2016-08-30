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
#ifndef CRIOMANAGER_FIELDREADER
#define CRIOMANAGER_FIELDREADER 1

#include <CRIOManager/crExport.h>

#include <string.h>
#include <stdlib.h>

#include <istream>

namespace CRIOManager {

class crField;

class CRIOMANAGER_EXPORT crFieldReader
{
    public:

        crFieldReader();
        crFieldReader(const crFieldReader& ic);
        virtual ~crFieldReader();

        virtual crFieldReader& operator = (const crFieldReader& ic);

        void attach(std::istream* input);
        void detach();

        virtual bool eof() const;

        bool readField(crField& fieldPtr);
        void ignoreField();
        
        /** no of unmatched `{' encountered so far in file*/
        int getNoNestedBrackets() const;

    private:

        bool _readField(crField* fieldPtr);

        void _init();
        void _free();
        void _copy(const crFieldReader& ic);

        std::istream* m_fin;
        bool m_eof;

        bool findStartOfNextField();

        int m_noNestedBrackets;

        bool m_delimiterEatLookUp[256];
        bool m_delimiterKeepLookUp[256];

};

}

#endif                                            // __SG_FIELD_READER_H
