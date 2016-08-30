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

#ifndef CRTEXT_STRING
#define CRTEXT_STRING 1

#include <vector>
#include <set>
#include <string>

#include <CRText/crExport.h>

namespace CRText {

// ********************************  HACK  **********************************
// Following class is needed to work around a DLL export problem. See file
// include/CRCore/crPrimitive for details.

class VectorUInt: public std::vector<unsigned int>
{
    typedef std::vector<value_type> vector_type;
public:
    VectorUInt(): vector_type() {}
    VectorUInt(const VectorUInt &copy): vector_type(copy) {}
    VectorUInt(unsigned int* beg, unsigned int* end): vector_type(beg, end) {}
    explicit VectorUInt(unsigned int n): vector_type(n) {}
};

// **************************************************************************

class crText;

class CRTEXT_EXPORT crString : public VectorUInt
{
    public:

        typedef VectorUInt vector_type;

        /**
         * Types of string encodings supported
         */
        enum Encoding
        {
            ENCODING_UNDEFINED,                 /// not using Unicode
            ENCODING_ASCII = ENCODING_UNDEFINED,/// unsigned char ASCII
            ENCODING_UTF8,                      /// 8-bit unicode transformation format
            ENCODING_UTF16,                     /// 16-bit signature
            ENCODING_UTF16_BE,                  /// 16-bit big-endian
            ENCODING_UTF16_LE,                  /// 16-bit little-endian
            ENCODING_UTF32,                     /// 32-bit signature
            ENCODING_UTF32_BE,                  /// 32-bit big-endian
            ENCODING_UTF32_LE,                  /// 32-bit little-endian
            ENCODING_SIGNATURE                  /// detect encoding from signature
        };


        crString() {}
        crString(const crString& str);
        crString(const std::string& str) { set(str); }
        crString(const wchar_t* text) { set(text); }
        crString(const std::string& text,Encoding encoding) { set(text,encoding); }

        crString& operator = (const crString& str);

        void set(const std::string& str);

        /** Set the text using a wchar_t string, 
          * which is converted to an internal TextString.*/
        void set(const wchar_t* text);

        /** Set the text using a Unicode encoded std::string, which is converted to an internal TextString.
          * The encoding parameter specifies which Unicode encoding is used in the std::string. */
        void set(const std::string& text,Encoding encoding);

        /** returns a UTF8 encoded version of this CRText::crString.*/
        std::string createUTF8EncodedString() const;

    protected:

};

}


#endif
