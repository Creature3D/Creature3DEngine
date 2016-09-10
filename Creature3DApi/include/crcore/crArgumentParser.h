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
//Modified by Wucaihua

#ifndef CRCORE_CRARGUMENTPARSER_H
#define CRCORE_CRARGUMENTPARSER_H 1

#include <CRCore/crExport.h>
#include <CRCore/crVector2.h>
#include <CRCore/crVector3.h>
#include <CRCore/crVector4.h>
#include <CRCore/crNode.h>

#include <map>
#include <string>
#include <iostream>

namespace CRCore {

class CR_EXPORT crArgumentParser
{
    public:
		class CR_EXPORT Parameter
		{
		public:
			enum ParameterType
			{
				FLOAT_PARAMETER,
				DOUBLE_PARAMETER,
				INT_PARAMETER,
				UNSIGNED_INT_PARAMETER,
				STRING_PARAMETER
			};

			union ValueUnion
			{
				float*          m_float;
				double*         m_double;
				int*            m_int;
				unsigned int*   m_uint;
				std::string*    m_string;
			};

			Parameter(float& value) { m_type = FLOAT_PARAMETER; m_value.m_float = &value; }

			Parameter(double& value) { m_type = DOUBLE_PARAMETER; m_value.m_double = &value; }

			Parameter(int& value) { m_type = INT_PARAMETER; m_value.m_int = &value; }

			Parameter(unsigned int& value)  { m_type = UNSIGNED_INT_PARAMETER; m_value.m_uint = &value; }

			Parameter(std::string& value) { m_type = STRING_PARAMETER; m_value.m_string = &value; }

			Parameter(const Parameter& param) { m_type = param.m_type; m_value = param.m_value; }

			Parameter& operator = (const Parameter& param) { m_type = param.m_type; m_value = param.m_value; return *this; }

			bool valid(const char* str) const;
			bool assign(const char* str);

		protected:

			ParameterType   m_type;
			ValueUnion      m_value;
		};

		/** Return true if the specified string is an option in the form
		* -option or --option. */
		static bool isOption(const char* str);

		/** Return true if string is non-NULL and not an option in the form
		* -option or --option. */
		static bool isString(const char* str);

		/** Return true if specified parameter is a number. */
		static bool isNumber(const char* str);

public:

	crArgumentParser(int* argc,char **argv);

	/** Return the argument count. */
	int& argc() { return *m_argc; }

	/** Return the argument array. */
	char** argv() { return m_argv; }

	/** Return the char* argument at the specified position. */
	char* operator [] (int pos) { return m_argv[pos]; }

	/** Return the const char* argument at the specified position. */
	const char* operator [] (int pos) const { return m_argv[pos]; }

	/** Return the application name, as specified by argv[0]  */
	std::string getApplicationName() const;

	/** Return the position of an occurance of a string in the argument list.
	* Return -1 if no string is found. */
	int find(const std::string& str) const;

	/** Return true if the specified parameter is an option in the form of
	* -option or --option. */
	bool isOption(int pos) const;

	/** Return true if the specified parameter is a string not in
	* the form of an option. */
	bool isString(int pos) const;

	/** Return true if the specified parameter is a number. */
	bool isNumber(int pos) const;

	bool containsOptions() const;

	/** Remove one or more arguments from the argv argument list,
	* and decrement the argc respectively. */
	void remove(int pos,int num=1);

	/** Return true if the specified argument matches the given string. */
	bool match(int pos, const std::string& str) const;

	/** Search for an occurance of a string in the argument list. If found,
	* remove that occurance and return true. Otherwise, return false. */
	bool read(const std::string& str);
	bool read(const std::string& str, Parameter value1);
	bool read(const std::string& str, Parameter value1, Parameter value2);
	bool read(const std::string& str, Parameter value1, Parameter value2, Parameter value3);
	bool read(const std::string& str, Parameter value1, Parameter value2, Parameter value3, Parameter value4);
	bool read(const std::string& str, Parameter value1, Parameter value2, Parameter value3, Parameter value4, Parameter value5);
	bool read(const std::string& str, Parameter value1, Parameter value2, Parameter value3, Parameter value4, Parameter value5, Parameter value6);
	bool read(const std::string& str, Parameter value1, Parameter value2, Parameter value3, Parameter value4, Parameter value5, Parameter value6, Parameter value7);
	bool read(const std::string& str, Parameter value1, Parameter value2, Parameter value3, Parameter value4, Parameter value5, Parameter value6, Parameter value7, Parameter value8);


	/** If the argument value at the specified position matches the given string,
	* and subsequent parameters are also matched, then set the parameter values,
	* remove the arguments from the list, and return true. Otherwise, return false. */
	bool read(int pos, const std::string& str);
	bool read(int pos, const std::string& str, Parameter value1);
	bool read(int pos, const std::string& str, Parameter value1, Parameter value2);
	bool read(int pos, const std::string& str, Parameter value1, Parameter value2, Parameter value3);
	bool read(int pos, const std::string& str, Parameter value1, Parameter value2, Parameter value3, Parameter value4);
	bool read(int pos, const std::string& str, Parameter value1, Parameter value2, Parameter value3, Parameter value4, Parameter value5);
	bool read(int pos, const std::string& str, Parameter value1, Parameter value2, Parameter value3, Parameter value4, Parameter value5, Parameter value6);
	bool read(int pos, const std::string& str, Parameter value1, Parameter value2, Parameter value3, Parameter value4, Parameter value5, Parameter value6, Parameter value7);
	bool read(int pos, const std::string& str, Parameter value1, Parameter value2, Parameter value3, Parameter value4, Parameter value5, Parameter value6, Parameter value7, Parameter value8);


	enum ErrorSeverity
	{
		BENIGN = 0,
		CRITICAL = 1
	};

	typedef std::map<std::string,ErrorSeverity> ErrorMessageMap;

	/** Return the error flag, true if an error has occured when reading arguments. */
	bool errors(ErrorSeverity severity=BENIGN) const;

	/** Report an error message by adding to the ErrorMessageMap. */
	void reportError(const std::string& message,ErrorSeverity severity=CRITICAL);

	/** For each remaining option, report it as unrecognized. */
	void reportRemainingOptionsAsUnrecognized(ErrorSeverity severity=BENIGN);

	/** Return the error message, if any has occured. */
	ErrorMessageMap& getErrorMessageMap() { return m_errorMessageMap; }

	/** Return the error message, if any has occured. */
	const ErrorMessageMap& getErrorMessageMap() const { return m_errorMessageMap; }

	/** Write error messages to the given ostream, if at or above the given severiity. */
	void writeErrorMessages(std::ostream& output,ErrorSeverity sevrity=BENIGN);


	static char *trim( char *str );
	static char *trimQuot( char *str, char chflg );

	static bool readKeyValue(std::string str, const char *key, int &value );
	static bool readKeyValue(std::string str, const char *key, float &value );
	static bool readKeyValue(std::string str, const char *key, crVector2 &vec );
	static bool readKeyValue(std::string str, const char *key, crVector3 &vec );
	static bool readKeyValue(std::string str, const char *key, crVector4 &vec );
	static bool readKeyValue(std::string str, const char *key, crVector2i &vec );
	static bool readKeyValue(std::string str, const char *key, crVector3i &vec );
	static bool readKeyValue(std::string str, const char *key, crVector4i &vec );
	static bool readKeyValue(std::string str, const char *key, crVector2s &vec );
	static bool readKeyValue(std::string str, const char *key, crVector3s &vec );
	static bool readKeyValue(std::string str, const char *key, crVector4s &vec );
	static bool readKeyValue(std::string str, const char *key, std::string &value );
	static bool readKeyValue(const std::vector<std::string>& strList, const char *key, int &value );
	static bool readKeyValue(const std::vector<std::string>& strList, const char *key, float &value );
	static bool readKeyValue(const std::vector<std::string>& strList, const char *key, crVector2 &vec );
	static bool readKeyValue(const std::vector<std::string>& strList, const char *key, crVector3 &vec );
	static bool readKeyValue(const std::vector<std::string>& strList, const char *key, crVector4 &vec );
	static bool readKeyValue(const std::vector<std::string>& strList, const char *key, crVector2i &vec );
	static bool readKeyValue(const std::vector<std::string>& strList, const char *key, crVector3i &vec );
	static bool readKeyValue(const std::vector<std::string>& strList, const char *key, crVector4i &vec );
	static bool readKeyValue(const std::vector<std::string>& strList, const char *key, crVector2s &vec );
	static bool readKeyValue(const std::vector<std::string>& strList, const char *key, crVector3s &vec );
	static bool readKeyValue(const std::vector<std::string>& strList, const char *key, crVector4s &vec );
	static bool readKeyValue(const std::vector<std::string>& strList, const char *key, std::string &value );

	static void appItoaAppend( int inNum,std::string &numberString );
	static std::string appItoa( int inNum );
	static std::string appFtoa(float value);
	static std::string appVectoa(const crVector2 &value,char sign=',');
	static std::string appVectoa(const crVector3 &value,char sign=',');
	static std::string appVectoa(const crVector4 &value,char sign=',');
	static std::string appVectoa(const crVector2i &value,char sign=',');
	static std::string appVectoa(const crVector3i &value,char sign=',');
	static std::string appVectoa(const crVector4i &value,char sign=',');
	static std::string appVectoa(const crVector2s &value,char sign=',');
	static std::string appVectoa(const crVector3s &value,char sign=',');
	static std::string appVectoa(const crVector4s &value,char sign=',');
	static std::string appVectoa(const crVector2b &value,char sign=',');
	static std::string appVectoa(const crVector3b &value,char sign=',');
	static std::string appVectoa(const crVector4b &value,char sign=',');
	static std::string appVectoa(const std::vector<float> &value,char sign=',');
	static std::string appVectoa(const std::vector<int> &value,char sign=',');
	static std::string appVectoa(const std::vector<short> &value,char sign=',');

	static bool appAtoVec(std::string str, crVector2 &value,char sign=',');
	static bool appAtoVec(std::string str, crVector3 &value,char sign=',');
	static bool appAtoVec(std::string str, crVector4 &value,char sign=',');
	static bool appAtoVec(std::string str, crVector2i &value,char sign=',');
	static bool appAtoVec(std::string str, crVector3i &value,char sign=',');
	static bool appAtoVec(std::string str, crVector4i &value,char sign=',');
	static bool appTimetoVec(std::string str, crVector3i &value,char sign=':');
	static bool appAtoVec(std::string str, crVector2s &value,char sign=',');
	static bool appAtoVec(std::string str, crVector3s &value,char sign=',');
	static bool appAtoVec(std::string str, crVector4s &value,char sign=',');
	static bool appAtoVec(std::string str, crVector2b &value,char sign=',');
	static bool appAtoVec(std::string str, crVector3b &value,char sign=',');
	static bool appAtoVec(std::string str, crVector4b &value,char sign=',');
	static bool appAtoVec(std::string str, crVector4ub &value,char sign=',');
	static bool appAtoVec(std::string str, std::vector<float> &value,char sign=',');
	static bool appAtoVec(std::string str, std::vector<int> &value,char sign=',');
	static bool appAtoVec(std::string str, std::vector<short> &value,char sign=',');
	static bool appAtoVec(std::string str, std::vector<std::string> &value,char sign=',');

	static std::string appI64toa( _crInt64 inNum );
	static _crInt64 appAtoI64(const std::string& str);

	static void updateOrAddKeyValue(std::string& str, const std::string &key, int value);
	static void updateOrAddKeyValue(std::string& str, const std::string &key, float value);
	static void updateOrAddKeyValue(std::string& str, const std::string &key, const std::string &value);
	static void updateOrAddKeyValue(std::string& str, const std::string &key, const crVector2i &value);
	static void updateOrAddKeyValue(std::string& str, const std::string &key, const crVector3i &value);
	static void updateOrAddKeyValue(std::string& str, const std::string &key, const crVector4i &value);
	static void updateOrAddKeyValue(std::string& str, const std::string &key, const crVector2 &value);
	static void updateOrAddKeyValue(std::string& str, const std::string &key, const crVector3 &value);
	static void updateOrAddKeyValue(std::string& str, const std::string &key, const crVector4 &value);

	static std::string getFilePath(const std::string& filename);
	static std::string getSimpleFileName(const std::string& fileName);
	static std::string getFileExtension(const std::string& fileName);
	static std::string getFileNameEliminateExt(const std::string& fileName);
	static bool equalCaseInsensitive(const std::string& lhs,const char* rhs);
	
	static bool deleteKey(std::string& str, const std::string &key);
  protected:
        
    int*                m_argc;
    char**              m_argv;
    ErrorMessageMap     m_errorMessageMap;
        
};

}

#endif
