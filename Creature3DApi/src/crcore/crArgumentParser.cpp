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
#include <CRCore/crArgumentParser.h>
#include <CRCore/crNotify.h>

#include <set>
#include <iostream>
#include <stdio.h> 
#include <stdarg.h>
using namespace CRCore;

bool crArgumentParser::isOption(const char* str)
{
    return str && str[0]=='-';
}

bool crArgumentParser::isString(const char* str)
{
    if (!str) return false;
    return true;
    //return !isOption(str);
}

bool crArgumentParser::isNumber(const char* str)
{
    if (!str) return false;

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
        if (*ptr==0) return true;
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

    if (couldBeInt && noZeroToNine>0) return true;
    if (couldBeFloat && noZeroToNine>0) return true;

    return false;
    
}

bool crArgumentParser::Parameter::valid(const char* str) const
{
    switch(m_type)
    {
    case Parameter::FLOAT_PARAMETER:        return isNumber(str); break;
    case Parameter::DOUBLE_PARAMETER:       return isNumber(str); break;
    case Parameter::INT_PARAMETER:          return isNumber(str); break;
    case Parameter::UNSIGNED_INT_PARAMETER: return isNumber(str); break;
    case Parameter::STRING_PARAMETER:       return isString(str); break;
    }
    return false;
}

bool crArgumentParser::Parameter::assign(const char* str)
{
    if (valid(str))
    {
        switch(m_type)
        {
        case Parameter::FLOAT_PARAMETER:        *m_value.m_float = atof(str); break;
        case Parameter::DOUBLE_PARAMETER:       *m_value.m_double = atof(str); break;
        case Parameter::INT_PARAMETER:          *m_value.m_int = atoi(str); break;
        case Parameter::UNSIGNED_INT_PARAMETER: *m_value.m_uint = atoi(str); break;
        case Parameter::STRING_PARAMETER:       *m_value.m_string = str; break;
        }
        return true;
    }
    else
    {
        return false;
    }
}



crArgumentParser::crArgumentParser(int* argc,char **argv):
    m_argc(argc),
    m_argv(argv)
{
}

std::string crArgumentParser::getApplicationName() const
{
    if (m_argc && *m_argc>0 ) return std::string(m_argv[0]);
    return "";
}

 
bool crArgumentParser::isOption(int pos) const
{
    return pos<*m_argc && isOption(m_argv[pos]);
}

bool crArgumentParser::isString(int pos) const
{
    return pos < *m_argc && isString(m_argv[pos]);
}

bool crArgumentParser::isNumber(int pos) const
{
    return pos < *m_argc && isNumber(m_argv[pos]);
}


int crArgumentParser::find(const std::string& str) const
{
    for(int pos=1;pos<*m_argc;++pos)
    {
        if (str==m_argv[pos])
        {
            return pos;
        }
    }
    return 0;
}

bool crArgumentParser::match(int pos, const std::string& str) const
{
    return pos<*m_argc && str==m_argv[pos];
}


bool crArgumentParser::containsOptions() const
{
    for(int pos=1;pos<*m_argc;++pos)
    {
        if (isOption(pos)) return true;
    }
    return false;
}


void crArgumentParser::remove(int pos,int num)
{
    if (num==0) return;
    
    for(;pos+num<*m_argc;++pos)
    {
        m_argv[pos]=m_argv[pos+num];
    }
    for(;pos<*m_argc;++pos)
    {
        m_argv[pos]=0;
    }
    *m_argc-=num;
}

bool crArgumentParser::read(const std::string& str)
{
    int pos=find(str);
    if (pos<=0) return false;
    remove(pos);
    return true;
}

bool crArgumentParser::read(const std::string& str, Parameter value1)
{
    int pos=find(str);
    if (pos<=0) return false;
    return read(pos,str,value1);
}

bool crArgumentParser::read(const std::string& str, Parameter value1, Parameter value2)
{
    int pos=find(str);
    if (pos<=0) return false;
    return read(pos,str,value1, value2);
}

bool crArgumentParser::read(const std::string& str, Parameter value1, Parameter value2, Parameter value3)
{
    int pos=find(str);
    if (pos<=0) return false;
    return read(pos,str,value1, value2, value3);
}

bool crArgumentParser::read(const std::string& str, Parameter value1, Parameter value2, Parameter value3, Parameter value4)
{
    int pos=find(str);
    if (pos<=0) return false;
    return read(pos,str,value1, value2, value3, value4);
}

bool crArgumentParser::read(const std::string& str, Parameter value1, Parameter value2, Parameter value3, Parameter value4, Parameter value5)
{
    int pos=find(str);
    if (pos<=0) return false;
    return read(pos,str,value1, value2, value3, value4, value5);
}

bool crArgumentParser::read(const std::string& str, Parameter value1, Parameter value2, Parameter value3, Parameter value4, Parameter value5, Parameter value6)
{
    int pos=find(str);
    if (pos<=0) return false;
    return read(pos,str,value1, value2, value3, value4, value5, value6);
}

bool crArgumentParser::read(const std::string& str, Parameter value1, Parameter value2, Parameter value3, Parameter value4, Parameter value5, Parameter value6, Parameter value7)
{
    int pos=find(str);
    if (pos<=0) return false;
    return read(pos,str,value1, value2, value3, value4, value5, value6, value7);
}

bool crArgumentParser::read(const std::string& str, Parameter value1, Parameter value2, Parameter value3, Parameter value4, Parameter value5, Parameter value6, Parameter value7, Parameter value8)
{
    int pos=find(str);
    if (pos<=0) return false;
    return read(pos,str,value1, value2, value3, value4, value5, value6, value7, value8);
}

bool crArgumentParser::read(int pos, const std::string& str)
{
    if (match(pos,str))
    {
        remove(pos,1);
        return true;
    }
    return false;
}

bool crArgumentParser::read(int pos, const std::string& str, Parameter value1)
{
    if (match(pos,str))
    {
        if ((pos+1)<*m_argc)
        {
            if (value1.valid(m_argv[pos+1]))
            {
                value1.assign(m_argv[pos+1]);
                remove(pos,2);
                return true;
            }
            reportError("argument to `"+str+"` is not valid");
            return false;
        }
        reportError("argument to `"+str+"` is missing");
        return false;
    }
    return false;
}

bool crArgumentParser::read(int pos, const std::string& str, Parameter value1, Parameter value2)
{
    if (match(pos,str))
    {
        if ((pos+2)<*m_argc)
        {
            if (value1.valid(m_argv[pos+1]) &&
                value2.valid(m_argv[pos+2]))
            {
                value1.assign(m_argv[pos+1]);
                value2.assign(m_argv[pos+2]);
                remove(pos,3);
                return true;
            }
            reportError("argument to `"+str+"` is not valid");
            return false;
        }
        reportError("argument to `"+str+"` is missing");
        return false;
    }
    return false;
}

bool crArgumentParser::read(int pos, const std::string& str, Parameter value1, Parameter value2, Parameter value3)
{
    if (match(pos,str))
    {
        if ((pos+3)<*m_argc)
        {
			if (value1.valid(m_argv[pos+1]) &&
				value2.valid(m_argv[pos+2]) &&
				value3.valid(m_argv[pos+3]))
			{
				value1.assign(m_argv[pos+1]);
				value2.assign(m_argv[pos+2]);
				value3.assign(m_argv[pos+3]);
				remove(pos,4);
				return true;
			}
			reportError("argument to `"+str+"` is not valid");
			return false;
		}
		reportError("argument to `"+str+"` is missing");
		return false;
	}
	return false;
}

bool crArgumentParser::read(int pos, const std::string& str, Parameter value1, Parameter value2, Parameter value3, Parameter value4)
{
	if (match(pos,str))
	{
		if ((pos+4)<*m_argc)
		{
			if (value1.valid(m_argv[pos+1]) &&
				value2.valid(m_argv[pos+2]) &&
				value3.valid(m_argv[pos+3]) &&
				value4.valid(m_argv[pos+4]))
			{
				value1.assign(m_argv[pos+1]);
				value2.assign(m_argv[pos+2]);
				value3.assign(m_argv[pos+3]);
				value4.assign(m_argv[pos+4]);
				remove(pos,5);
				return true;
			}
			reportError("argument to `"+str+"` is not valid");
			return false;
		}
		reportError("argument to `"+str+"` is missing");
		return false;
	}
	return false;
}

bool crArgumentParser::read(int pos, const std::string& str, Parameter value1, Parameter value2, Parameter value3, Parameter value4, Parameter value5)
{
	if (match(pos,str))
	{
		if ((pos+5)<*m_argc)
		{
			if (value1.valid(m_argv[pos+1]) &&
				value2.valid(m_argv[pos+2]) &&
				value3.valid(m_argv[pos+3]) &&
				value4.valid(m_argv[pos+4]) &&
				value5.valid(m_argv[pos+5]))
			{
				value1.assign(m_argv[pos+1]);
				value2.assign(m_argv[pos+2]);
				value3.assign(m_argv[pos+3]);
				value4.assign(m_argv[pos+4]);
				value5.assign(m_argv[pos+5]);
				remove(pos,6);
				return true;
			}
			reportError("argument to `"+str+"` is not valid");
			return false;
		}
		reportError("argument to `"+str+"` is missing");
		return false;
	}
	return false;
}

bool crArgumentParser::read(int pos, const std::string& str, Parameter value1, Parameter value2, Parameter value3, Parameter value4, Parameter value5, Parameter value6)
{
	if (match(pos,str))
	{
		if ((pos+6)<*m_argc)
		{
			if (value1.valid(m_argv[pos+1]) &&
				value2.valid(m_argv[pos+2]) &&
				value3.valid(m_argv[pos+3]) &&
				value4.valid(m_argv[pos+4]) &&
				value5.valid(m_argv[pos+5]) &&
				value6.valid(m_argv[pos+6]))
			{
				value1.assign(m_argv[pos+1]);
				value2.assign(m_argv[pos+2]);
				value3.assign(m_argv[pos+3]);
				value4.assign(m_argv[pos+4]);
				value5.assign(m_argv[pos+5]);
				value6.assign(m_argv[pos+6]);
				remove(pos,7);
				return true;
			}
			reportError("argument to `"+str+"` is not valid");
			return false;
		}
		reportError("argument to `"+str+"` is missing");
		return false;
	}
	return false;
}

bool crArgumentParser::read(int pos, const std::string& str, Parameter value1, Parameter value2, Parameter value3, Parameter value4, Parameter value5,  Parameter value6,  Parameter value7)
{
	if (match(pos,str))
	{
		if ((pos+7)<*m_argc)
		{
			if (value1.valid(m_argv[pos+1]) &&
				value2.valid(m_argv[pos+2]) &&
				value3.valid(m_argv[pos+3]) &&
				value4.valid(m_argv[pos+4]) &&
				value5.valid(m_argv[pos+5]) &&
				value6.valid(m_argv[pos+6]) &&
				value7.valid(m_argv[pos+7]))
			{
				value1.assign(m_argv[pos+1]);
				value2.assign(m_argv[pos+2]);
				value3.assign(m_argv[pos+3]);
				value4.assign(m_argv[pos+4]);
				value5.assign(m_argv[pos+5]);
				value6.assign(m_argv[pos+6]);
				value7.assign(m_argv[pos+7]);
				remove(pos,8);
				return true;
			}
			reportError("argument to `"+str+"` is not valid");
			return false;
		}
		reportError("argument to `"+str+"` is missing");
		return false;
	}
	return false;
}

bool crArgumentParser::read(int pos, const std::string& str, Parameter value1, Parameter value2, Parameter value3, Parameter value4, Parameter value5,  Parameter value6,  Parameter value7,  Parameter value8)
{
	if (match(pos,str))
	{
		if ((pos+8)<*m_argc)
		{
			if (value1.valid(m_argv[pos+1]) &&
				value2.valid(m_argv[pos+2]) &&
				value3.valid(m_argv[pos+3]) &&
				value4.valid(m_argv[pos+4]) &&
				value5.valid(m_argv[pos+5]) &&
				value6.valid(m_argv[pos+6]) &&
				value7.valid(m_argv[pos+7]) &&
				value8.valid(m_argv[pos+8]))
			{
				value1.assign(m_argv[pos+1]);
				value2.assign(m_argv[pos+2]);
				value3.assign(m_argv[pos+3]);
				value4.assign(m_argv[pos+4]);
				value5.assign(m_argv[pos+5]);
				value6.assign(m_argv[pos+6]);
				value7.assign(m_argv[pos+7]);
				value8.assign(m_argv[pos+8]);
				remove(pos,9);
				return true;
			}
			reportError("argument to `"+str+"` is not valid");
			return false;
		}
		reportError("argument to `"+str+"` is missing");
		return false;
	}
	return false;
}

bool crArgumentParser::errors(ErrorSeverity severity) const
{
	for(ErrorMessageMap::const_iterator itr=m_errorMessageMap.begin();
		itr!=m_errorMessageMap.end();
		++itr)
	{
		if (itr->second>=severity) return true;
	}
	return false;
}

void crArgumentParser::reportError(const std::string& message,ErrorSeverity severity)
{
	m_errorMessageMap[message]=severity;
}

void crArgumentParser::reportRemainingOptionsAsUnrecognized(ErrorSeverity severity)
{
	std::set<std::string> options;
	for(int pos=1;pos<argc();++pos)
	{
		if (isOption(pos) && options.find(m_argv[pos])==options.end()) 
		{
			reportError(getApplicationName() +": unrecognized option "+m_argv[pos],severity);
		}
	}
}
void crArgumentParser::writeErrorMessages(std::ostream& output,ErrorSeverity severity)
{
	for(ErrorMessageMap::iterator itr=m_errorMessageMap.begin();
		itr!=m_errorMessageMap.end();
		++itr)
	{
		if (itr->second>=severity)
		{
			output<< getApplicationName() << ": " << itr->first << std::endl;
		}
	}
}

// È¥³ý×Ö·û´®Ê×Î²¿Õ¸ñºÍTab·û£¬ÒÔ¼°´®Ä©µÄ»Ø³µ·û
char *crArgumentParser::trim( char *str )
{
	char *p1, *p2;
	p1 = p2 = str;
	while ( ( (*p1 == ' ') || (*p1 == '\t') ) && *p1++ );
	while ( *p2++ = *p1++ );
	if ( *str == '\0' )
		return( str );

	p2 -= 2;
	while ( *p2 == ' ' || *p2 == '\t' || *p2 == '\n'|| *p2 == '\r' )
		*p2-- = '\0';

	return( str );
}
std::string crArgumentParser::trim(const std::string& str)
{
	if (!str.size()) return str;
	std::string::size_type first = str.find_first_not_of(" \t");
	std::string::size_type last = str.find_last_not_of("  \t\r\n");
	return str.substr(first, last - first + 1);
}

//È¡³öÁ½chflg×Ö·ûÀïµÄ×Ö·û´Ü
char *crArgumentParser::trimQuot( char *str, char chflg )
{
	char *p1, *p2;
	p1 = p2 = str;
	while ( (*p1 != chflg) && *p1++ );
	while ( *p2++ = *++p1 );
	if ( *str == '\0' )
		return( str );

	p2 -= 2;
	while ( (*p2 != chflg) || p2==str+1 )
		*p2-- = '\0';
	*p2 = '\0';

	return( str );
}

bool crArgumentParser::readKeyValue(std::string str, const char *key, int &value )
{
	int i = str.find(key);
	if(i<0) return false;
	int keylen = strlen(key);
	if(str[i+keylen]==' '||str[i+keylen]==':')	
	{	
		str.erase(0,i+keylen+1);
		i = str.find('\n');
		if(i>-1)
			str.erase(i,str.size());
		value = atoi(str.c_str());

		return true;
	}
	return false;
}

bool crArgumentParser::readKeyValue(std::string str, const char *key, float &value )
{
	int i = str.find(key);
	if(i<0) return false;
	int keylen = strlen(key);
	if(str[i+keylen]==' '||str[i+keylen]==':')	
	{
		str.erase(0,i+keylen+1);
		i = str.find('\n');
		if(i>-1)
			str.erase(i,str.size());	
		value = atof(str.c_str());

		return true;
	}
    return false;
}

bool crArgumentParser::readKeyValue(std::string str, const char *key, crVector2 &vec )
{
	int i = str.find(key);
	if(i<0) return false;
	int keylen = strlen(key);
	if(str[i+keylen]==' '||str[i+keylen]==':')	
	{
		str.erase(0,i+keylen+1);
		i = str.find('\n');
		if(i>-1)
			str.erase(i,str.size());
		vec.x() = atof(str.c_str());

		i = str.find(',');
		if(i==std::string::npos)
			i = str.find('|');
		str.erase(0,i+1);
		vec.y() = atof(str.c_str());

		return true;
	}
	return false;
}

bool crArgumentParser::readKeyValue(std::string str, const char *key, crVector3 &vec )
{
	int i = str.find(key);
	if(i<0) return false;
	int keylen = strlen(key);
	if(str[i+keylen]==' '||str[i+keylen]==':')	
	{
		str.erase(0,i+keylen+1);
		i = str.find('\n');
		if(i>-1)
			str.erase(i,str.size());
		vec.x() = atof(str.c_str());

		i = str.find(',');
		if(i==std::string::npos)
			i = str.find('|');
		str.erase(0,i+1);
		vec.y() = atof(str.c_str());

		i = str.find(',');
		if(i==std::string::npos)
			i = str.find('|');
		str.erase(0,i+1);
		vec.z() = atof(str.c_str());

		return true;
	}
	return false;
}

bool crArgumentParser::readKeyValue(std::string str, const char *key, crVector4 &vec )
{
	int i = str.find(key);
	if(i<0) return false;
	int keylen = strlen(key);
	if(str[i+keylen]==' '||str[i+keylen]==':')	
	{
		str.erase(0,i+keylen+1);
		i = str.find('\n');
		if(i>-1)
			str.erase(i,str.size());
		vec.x() = atof(str.c_str());

		i = str.find(',');
		if(i==std::string::npos)
			i = str.find('|');
		str.erase(0,i+1);
		vec.y() = atof(str.c_str());

		i = str.find(',');
		if(i==std::string::npos)
			i = str.find('|');
		str.erase(0,i+1);
		vec.z() = atof(str.c_str());

		i = str.find(',');
		if(i==std::string::npos)
			i = str.find('|');
		str.erase(0,i+1);
		vec.w() = atof(str.c_str());

		return true;
	}
	return false;
}

bool crArgumentParser::readKeyValue(std::string str, const char *key, crVector2i &vec )
{
	int i = str.find(key);
	if(i<0) return false;
	int keylen = strlen(key);
	if(str[i+keylen]==' '||str[i+keylen]==':')	
	{
		str.erase(0,i+keylen+1);
		i = str.find('\n');
		if(i>-1)
			str.erase(i,str.size());
		vec.x() = atoi(str.c_str());

		i = str.find(',');
		if(i==std::string::npos)
			i = str.find('|');
		str.erase(0,i+1);
		vec.y() = atoi(str.c_str());

		return true;
	}
	return false;
}

bool crArgumentParser::readKeyValue(std::string str, const char *key, crVector3i &vec )
{
	int i = str.find(key);
	if(i<0) return false;
	int keylen = strlen(key);
	if(str[i+keylen]==' '||str[i+keylen]==':')	
	{
		str.erase(0,i+keylen+1);
		i = str.find('\n');
		if(i>-1)
			str.erase(i,str.size());
		vec.x() = atoi(str.c_str());

		i = str.find(',');
		if(i==std::string::npos)
			i = str.find('|');
		str.erase(0,i+1);
		vec.y() = atoi(str.c_str());

		i = str.find(',');
		if(i==std::string::npos)
			i = str.find('|');
		str.erase(0,i+1);
		vec.z() = atoi(str.c_str());

		return true;
	}
	return false;
}

bool crArgumentParser::readKeyValue(std::string str, const char *key, crVector4i &vec )
{
	int i = str.find(key);
	if(i<0) return false;
	int keylen = strlen(key);
	if(str[i+keylen]==' '||str[i+keylen]==':')	
	{
		str.erase(0,i+keylen+1);
		i = str.find('\n');
		if(i>-1)
			str.erase(i,str.size());
		vec.x() = atoi(str.c_str());

		i = str.find(',');
		if(i==std::string::npos)
			i = str.find('|');
		str.erase(0,i+1);
		vec.y() = atoi(str.c_str());

		i = str.find(',');
		if(i==std::string::npos)
			i = str.find('|');
		str.erase(0,i+1);
		vec.z() = atoi(str.c_str());

		i = str.find(',');
		if(i==std::string::npos)
			i = str.find('|');
		str.erase(0,i+1);
		vec.w() = atoi(str.c_str());

		return true;
	}
	return false;
}

bool crArgumentParser::readKeyValue(std::string str, const char *key, crVector2s &vec )
{
	int i = str.find(key);
	if(i<0) return false;
	int keylen = strlen(key);
	if(str[i+keylen]==' '||str[i+keylen]==':')	
	{
		str.erase(0,i+keylen+1);
		i = str.find('\n');
		if(i>-1)
			str.erase(i,str.size());
		vec.x() = (short)atoi(str.c_str());

		i = str.find(',');
		if(i==std::string::npos)
			i = str.find('|');
		str.erase(0,i+1);
		vec.y() = (short)atoi(str.c_str());

		return true;
	}
	return false;
}

bool crArgumentParser::readKeyValue(std::string str, const char *key, crVector3s &vec )
{
	int i = str.find(key);
	if(i<0) return false;
	int keylen = strlen(key);
	if(str[i+keylen]==' '||str[i+keylen]==':')	
	{
		str.erase(0,i+keylen+1);
		i = str.find('\n');
		if(i>-1)
			str.erase(i,str.size());
		vec.x() = (short)atoi(str.c_str());

		i = str.find(',');
		if(i==std::string::npos)
			i = str.find('|');
		str.erase(0,i+1);
		vec.y() = (short)atoi(str.c_str());

		i = str.find(',');
		if(i==std::string::npos)
			i = str.find('|');
		str.erase(0,i+1);
		vec.z() = (short)atoi(str.c_str());

		return true;
	}
	return false;
}

bool crArgumentParser::readKeyValue(std::string str, const char *key, crVector4s &vec )
{
	int i = str.find(key);
	if(i<0) return false;
	int keylen = strlen(key);
	if(str[i+keylen]==' '||str[i+keylen]==':')	
	{
		str.erase(0,i+keylen+1);
		i = str.find('\n');
		if(i>-1)
			str.erase(i,str.size());
		vec.x() = (short)atoi(str.c_str());

		i = str.find(',');
		if(i==std::string::npos)
			i = str.find('|');
		str.erase(0,i+1);
		vec.y() = (short)atoi(str.c_str());

		i = str.find(',');
		if(i==std::string::npos)
			i = str.find('|');
		str.erase(0,i+1);
		vec.z() = (short)atoi(str.c_str());

		i = str.find(',');
		if(i==std::string::npos)
			i = str.find('|');
		str.erase(0,i+1);
		vec.w() = (short)atoi(str.c_str());

		return true;
	}
	return false;
}

bool crArgumentParser::readKeyValue(std::string str, const char *key, std::string &value )
{
	int i = str.find(key);
	if(i<0) return false;
	int keylen = strlen(key);
	if(str[i+keylen]==' '||str[i+keylen]==':')	
	{
		str.erase(0,i+keylen+1);
		i = str.find('\n');
		if(i>-1)
			str.erase(i,str.size());

		value = trim(const_cast<char *>(str.c_str()));

		return true;
	}
	return false;
}

bool crArgumentParser::readKeyValue(const std::vector<std::string>& strList, const char *key, int &value )
{
	for( std::vector<std::string>::const_iterator itr = strList.begin();
		itr != strList.end();
		++itr )
	{
		if(readKeyValue(*itr,key,value))
			return true;
	}

	return false;
}

bool crArgumentParser::readKeyValue(const std::vector<std::string>& strList, const char *key, float &value )
{
	for( std::vector<std::string>::const_iterator itr = strList.begin();
		itr != strList.end();
		++itr )
	{
		if(readKeyValue(*itr,key,value))
			return true;
	}

	return false;
}

bool crArgumentParser::readKeyValue(const std::vector<std::string>& strList, const char *key, crVector2 &value )
{
	for( std::vector<std::string>::const_iterator itr = strList.begin();
		itr != strList.end();
		++itr )
	{
		if(readKeyValue(*itr,key,value))
			return true;
	}

	return false;
}

bool crArgumentParser::readKeyValue(const std::vector<std::string>& strList, const char *key, crVector3 &value )
{
	for( std::vector<std::string>::const_iterator itr = strList.begin();
		itr != strList.end();
		++itr )
	{
		if(readKeyValue(*itr,key,value))
			return true;
	}

	return false;
}

bool crArgumentParser::readKeyValue(const std::vector<std::string>& strList, const char *key, crVector4 &value )
{
	for( std::vector<std::string>::const_iterator itr = strList.begin();
		itr != strList.end();
		++itr )
	{
		if(readKeyValue(*itr,key,value))
			return true;
	}

	return false;
}

bool crArgumentParser::readKeyValue(const std::vector<std::string>& strList, const char *key, crVector2i &value )
{
	for( std::vector<std::string>::const_iterator itr = strList.begin();
		itr != strList.end();
		++itr )
	{
		if(readKeyValue(*itr,key,value))
			return true;
	}

	return false;
}

bool crArgumentParser::readKeyValue(const std::vector<std::string>& strList, const char *key, crVector3i &value )
{
	for( std::vector<std::string>::const_iterator itr = strList.begin();
		itr != strList.end();
		++itr )
	{
		if(readKeyValue(*itr,key,value))
			return true;
	}

	return false;
}

bool crArgumentParser::readKeyValue(const std::vector<std::string>& strList, const char *key, crVector4i &value )
{
	for( std::vector<std::string>::const_iterator itr = strList.begin();
		itr != strList.end();
		++itr )
	{
		if(readKeyValue(*itr,key,value))
			return true;
	}

	return false;
}

bool crArgumentParser::readKeyValue(const std::vector<std::string>& strList, const char *key, crVector2s &value )
{
	for( std::vector<std::string>::const_iterator itr = strList.begin();
		itr != strList.end();
		++itr )
	{
		if(readKeyValue(*itr,key,value))
			return true;
	}

	return false;
}

bool crArgumentParser::readKeyValue(const std::vector<std::string>& strList, const char *key, crVector3s &value )
{
	for( std::vector<std::string>::const_iterator itr = strList.begin();
		itr != strList.end();
		++itr )
	{
		if(readKeyValue(*itr,key,value))
			return true;
	}

	return false;
}

bool crArgumentParser::readKeyValue(const std::vector<std::string>& strList, const char *key, crVector4s &value )
{
	for( std::vector<std::string>::const_iterator itr = strList.begin();
		itr != strList.end();
		++itr )
	{
		if(readKeyValue(*itr,key,value))
			return true;
	}

	return false;
}

bool crArgumentParser::readKeyValue(const std::vector<std::string>& strList, const char *key, std::string &value )
{
	for( std::vector<std::string>::const_iterator itr = strList.begin();
		 itr != strList.end();
		 ++itr )
	{
        if(readKeyValue(*itr,key,value))
			return true;
	}

	return false;
}

//
// Convert an integer to a string.
//
// Faster Itoa that also appends to a string
void crArgumentParser::appItoaAppend( int inNum,std::string &numberString )
{
	if(inNum<-INT_MAX)
		return;
	int	num					= inNum; // This avoids having to deal with negating -MAXINT-1
	char*	numberChar[11]		= { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "-" };
	bool	bIsNumberNegative	= false;
	char	tempNum[16];		// 16 is big enough
	int		tempAt				= 16; // fill the temp string from the top down.

	// Correctly handle negative numbers and convert to positive integer.
	if( num < 0 )
	{
		bIsNumberNegative = true;
		num = -num;
	}

	tempNum[--tempAt] = 0; // NULL terminator

	// Convert to string assuming base ten and a positive integer.
	do 
	{
		tempNum[--tempAt] = *numberChar[num % 10];
		num /= 10;
	} while( num );

	// Append sign as we're going to reverse string afterwards.
	if( bIsNumberNegative )
	{
		tempNum[--tempAt] = *numberChar[10];
	}

	numberString += tempNum + tempAt;
}

std::string crArgumentParser::appItoa( int inNum )
{
	std::string numberString;
	appItoaAppend(inNum,numberString );
	return numberString;
}

std::string crArgumentParser::appFtoa(float value)
{
    char buf[16];
	memset(buf,0,16);
	sprintf(buf,"%f\0",value);
	for(int i = 15; i>0; i--)
	{
		if(buf[i] == '\0') continue;
		if(buf[i] == '.')
		{
			buf[i] = '\0';
			break;
		}
		if(buf[i] > '0' && buf[i] <= '9') break;
		buf[i] = '\0';
	}
	return trim(buf);
	//if(value<0) return '-'+appFtoa(-value);

	//int A = (int)value;
	//float B = value - A;
	//int i = 0;
	//while (B!=floorf(B) && i++<precision)
	//{
	//	B*=10.0f;
	//}

	//return appItoa(A) + '.' + appItoa((int)B);
}

std::string crArgumentParser::appVectoa(const crVector2 &value,char sign)
{
    return appFtoa(value[0]) + sign + appFtoa(value[1]);
}

std::string crArgumentParser::appVectoa(const crVector3 &value,char sign)
{
    return appFtoa(value[0]) + sign + appFtoa(value[1]) + sign + appFtoa(value[2]);
}

std::string crArgumentParser::appVectoa(const crVector4 &value,char sign)
{
    return appFtoa(value[0]) + sign + appFtoa(value[1]) + sign + appFtoa(value[2]) + sign + appFtoa(value[3]);
}

std::string crArgumentParser::appVectoa(const crVector2i &value,char sign)
{
    return appItoa(value[0]) + sign + appItoa(value[1]);
}

std::string crArgumentParser::appVectoa(const crVector3i &value,char sign)
{
    return appItoa(value[0]) + sign + appItoa(value[1]) + sign + appItoa(value[2]);
}

std::string crArgumentParser::appVectoa(const crVector4i &value,char sign)
{
    return appItoa(value[0]) + sign + appItoa(value[1]) + sign + appItoa(value[2]) + sign + appItoa(value[3]);
}

std::string crArgumentParser::appVectoa(const crVector2s &value,char sign)
{
	return appItoa(value[0]) + sign + appItoa(value[1]);
}

std::string crArgumentParser::appVectoa(const crVector3s &value,char sign)
{
	return appItoa(value[0]) + sign + appItoa(value[1]) + sign + appItoa(value[2]);
}

std::string crArgumentParser::appVectoa(const crVector4s &value,char sign)
{
	return appItoa(value[0]) + sign + appItoa(value[1]) + sign + appItoa(value[2]) + sign + appItoa(value[3]);
}

std::string crArgumentParser::appVectoa(const crVector2b &value,char sign)
{
	return appItoa(value[0]) + sign + appItoa(value[1]);
}

std::string crArgumentParser::appVectoa(const crVector3b &value,char sign)
{
	return appItoa(value[0]) + sign + appItoa(value[1]) + sign + appItoa(value[2]);
}

std::string crArgumentParser::appVectoa(const crVector4b &value,char sign)
{
	return appItoa(value[0]) + sign + appItoa(value[1]) + sign + appItoa(value[2]) + sign + appItoa(value[3]);
}
std::string crArgumentParser::appVectoa(const std::vector<float> &value,char sign)
{
	std::string str;
	int count = value.size();
	if(count == 0)
		return str;
	int i = 0;
	for( ; i<count-1; i++ )
	{
		str += appFtoa(value[i]) + sign;
	}
	str += appFtoa(value[i]);
	return str;
}
std::string crArgumentParser::appVectoa(const std::vector<int> &value,char sign)
{
	std::string str;
	int count = value.size();
	if(count == 0)
		return str;
	int i = 0;
	for( ; i<count-1; i++ )
	{
		str += appItoa(value[i]) + sign;
	}
	str += appItoa(value[i]);
	return str;
}
std::string crArgumentParser::appVectoa(const std::vector<short> &value,char sign)
{
	std::string str;
	int count = value.size();
	if(count == 0)
		return str;
	int i = 0;
	for( ; i<count-1; i++ )
	{
		str += appItoa(value[i]) + sign;
	}
	str += appItoa(value[i]);
	return str;
}
bool crArgumentParser::appAtoVec(std::string str, crVector2 &value,char sign)
{
	if(str.find("\"")==0)
	{
		str.erase(0,1);
	}
	if(str.find("VEC2") == 0)
	{//¼æÈÝreadKeyValue(str,"VEC2",value);
		str.erase(0,5);
	}
	int i;
	value.x() = atof(str.c_str());

	i = str.find(sign);//','
	if(i==std::string::npos)
		i = str.find('|');
	if(i==std::string::npos) return false;
	str.erase(0,i+1);
	value.y() = atof(str.c_str());
	return true;
}

bool crArgumentParser::appAtoVec(std::string str, crVector3 &value,char sign)
{
	if(str.find("\"")==0)
	{
		str.erase(0,1);
	}
	if(str.find("VEC3") == 0)
	{//¼æÈÝreadKeyValue(str,"VEC3",value);
		str.erase(0,5);
	}
	int i;
	value.x() = atof(str.c_str());

	i = str.find(sign);//','
	if(i==std::string::npos)
		i = str.find('|');
	if(i==std::string::npos)
		i = str.find(':');
	if(i==std::string::npos) return false;
	str.erase(0,i+1);
	value.y() = atof(str.c_str());
	
	i = str.find(sign);
	if(i==std::string::npos)
		i = str.find('|');
	if(i==std::string::npos)
		i = str.find(':');
	if(i==std::string::npos) return false;
	str.erase(0,i+1);
	value.z() = atof(str.c_str());
	return true;
}

bool crArgumentParser::appAtoVec(std::string str, crVector4 &value,char sign)
{
	if(str.find("\"")==0)
	{
		str.erase(0,1);
	}
	if(str.find("VEC4") == 0)
	{//¼æÈÝreadKeyValue(str,"VEC4",value);
		str.erase(0,5);
	}
	int i;
	value.x() = atof(str.c_str());

	i = str.find(sign);
	if(i==std::string::npos)
		i = str.find('|');
	if(i==std::string::npos) return false;
	str.erase(0,i+1);
	value.y() = atof(str.c_str());

	i = str.find(sign);
	if(i==std::string::npos)
		i = str.find('|');
	if(i==std::string::npos) return false;
	str.erase(0,i+1);
	value.z() = atof(str.c_str());

	i = str.find(sign);
	if(i==std::string::npos)
		i = str.find('|');
	if(i==std::string::npos) return false;
	str.erase(0,i+1);
	value.w() = atof(str.c_str());

	return true;
}

bool crArgumentParser::appAtoVec(std::string str, crVector2i &value,char sign)
{
	if(str.find("\"")==0)
	{
		str.erase(0,1);
	}
	if(str.find("VEC2") == 0)
	{//¼æÈÝreadKeyValue(str,"VEC2",value);
		str.erase(0,5);
	}
	int i;
	value.x() = atoi(str.c_str());

	i = str.find(sign);
	if(i==std::string::npos)
		i = str.find('|');
	if(i==std::string::npos) return false;
	str.erase(0,i+1);
	value.y() = atoi(str.c_str());
	return true;
}

bool crArgumentParser::appAtoVec(std::string str, crVector3i &value,char sign)
{
	if(str.find("\"")==0)
	{
		str.erase(0,1);
	}
	if(str.find("VEC3") == 0)
	{//¼æÈÝreadKeyValue(str,"VEC3",value);
		str.erase(0,5);
	}
	int i;
	value.x() = atoi(str.c_str());

	i = str.find(sign);
	if(i==std::string::npos)
		i = str.find('|');
	if(i==std::string::npos)
		i = str.find(':');
	if(i==std::string::npos) return false;
	str.erase(0,i+1);
	value.y() = atoi(str.c_str());

	i = str.find(sign);
	if(i==std::string::npos)
		i = str.find('|');
	if(i==std::string::npos)
		i = str.find(':');
	if(i==std::string::npos) return false;
	str.erase(0,i+1);
	value.z() = atoi(str.c_str());
	return true;
}

bool crArgumentParser::appAtoVec(std::string str, crVector4i &value,char sign)
{
	if(str.find("\"")==0)
	{
		str.erase(0,1);
	}
	if(str.find("VEC4") == 0)
	{//¼æÈÝreadKeyValue(str,"VEC4",value);
		str.erase(0,5);
	}
	int i;
	value.x() = atoi(str.c_str());

	i = str.find(sign);
	if(i==std::string::npos)
		i = str.find('|');
	if(i==std::string::npos) return false;
	str.erase(0,i+1);
	value.y() = atoi(str.c_str());

	i = str.find(sign);
	if(i==std::string::npos)
		i = str.find('|');
	if(i==std::string::npos) return false;
	str.erase(0,i+1);
	value.z() = atoi(str.c_str());

	i = str.find(sign);
	if(i==std::string::npos)
		i = str.find('|');
	if(i==std::string::npos) return false;
	str.erase(0,i+1);
	value.w() = atoi(str.c_str());
	return true;
}
bool crArgumentParser::appTimetoVec(std::string str, crVector3i &value,char sign)
{
	if(str.find("\"")==0)
	{
		str.erase(0,1);
	}
	if(str.find("VEC3") == 0)
	{//¼æÈÝreadKeyValue(str,"VEC4",value);
		str.erase(0,5);
	}
	int i;
	value.x() = atoi(str.c_str());

	i = str.find(sign);
	if(i==std::string::npos) return false;
	str.erase(0,i+1);
	value.y() = atoi(str.c_str());

	i = str.find(sign);
	if(i==std::string::npos) return false;
	str.erase(0,i+1);
	value.z() = atoi(str.c_str());

	return true;
}
bool crArgumentParser::appAtoVec(std::string str, crVector2s &value,char sign)
{
	if(str.find("\"")==0)
	{
		str.erase(0,1);
	}
	if(str.find("VEC2") == 0)
	{//¼æÈÝreadKeyValue(str,"VEC2",value);
		str.erase(0,5);
	}
	int i;
	value.x() = (short)(atoi(str.c_str()));

	i = str.find(sign);
	if(i==std::string::npos)
		i = str.find('|');
	if(i==std::string::npos) return false;
	str.erase(0,i+1);
	value.y() = (short)(atoi(str.c_str()));
	return true;
}

bool crArgumentParser::appAtoVec(std::string str, crVector3s &value,char sign)
{
	if(str.find("\"")==0)
	{
		str.erase(0,1);
	}
	if(str.find("VEC3") == 0)
	{//¼æÈÝreadKeyValue(str,"VEC3",value);
		str.erase(0,5);
	}
	int i;
	value.x() = (short)(atoi(str.c_str()));

	i = str.find(sign);
	if(i==std::string::npos)
		i = str.find('|');
	if(i==std::string::npos) return false;
	str.erase(0,i+1);
	value.y() = (short)(atoi(str.c_str()));

	i = str.find(sign);
	if(i==std::string::npos)
		i = str.find('|');
	if(i==std::string::npos) return false;
	str.erase(0,i+1);
	value.z() = (short)(atoi(str.c_str()));
	return true;
}

bool crArgumentParser::appAtoVec(std::string str, crVector4s &value,char sign)
{
	if(str.find("\"")==0)
	{
		str.erase(0,1);
	}
	if(str.find("VEC4") == 0)
	{//¼æÈÝreadKeyValue(str,"VEC4",value);
		str.erase(0,5);
	}
	int i;
	value.x() = (short)(atoi(str.c_str()));

	i = str.find(sign);
	if(i==std::string::npos)
		i = str.find('|');
	if(i==std::string::npos) return false;
	str.erase(0,i+1);
	value.y() = (short)(atoi(str.c_str()));

	i = str.find(sign);
	if(i==std::string::npos)
		i = str.find('|');
	if(i==std::string::npos) return false;
	str.erase(0,i+1);
	value.z() = (short)(atoi(str.c_str()));

	i = str.find(sign);
	if(i==std::string::npos)
		i = str.find('|');
	if(i==std::string::npos) return false;
	str.erase(0,i+1);
	value.w() = (short)(atoi(str.c_str()));
	return true;
}
bool crArgumentParser::appAtoVec(std::string str, crVector2b &value,char sign)
{
	if(str.find("\"")==0)
	{
		str.erase(0,1);
	}
	if(str.find("VEC2") == 0)
	{//¼æÈÝreadKeyValue(str,"VEC2",value);
		str.erase(0,5);
	}
	int i;
	value.x() = (char)(atoi(str.c_str()));

	i = str.find(sign);
	if(i==std::string::npos)
		i = str.find('|');
	if(i==std::string::npos) return false;
	str.erase(0,i+1);
	value.y() = (char)(atoi(str.c_str()));
	return true;
}

bool crArgumentParser::appAtoVec(std::string str, crVector3b &value,char sign)
{
	if(str.find("\"")==0)
	{
		str.erase(0,1);
	}
	if(str.find("VEC3") == 0)
	{//¼æÈÝreadKeyValue(str,"VEC3",value);
		str.erase(0,5);
	}
	int i;
	value.x() = (char)(atoi(str.c_str()));

	i = str.find(sign);
	if(i==std::string::npos)
		i = str.find('|');
	if(i==std::string::npos) return false;
	str.erase(0,i+1);
	value.y() = (char)(atoi(str.c_str()));

	i = str.find(sign);
	if(i==std::string::npos)
		i = str.find('|');
	if(i==std::string::npos) return false;
	str.erase(0,i+1);
	value.z() = (char)(atoi(str.c_str()));
	return true;
}

bool crArgumentParser::appAtoVec(std::string str, crVector4b &value,char sign)
{
	if(str.find("\"")==0)
	{
		str.erase(0,1);
	}
	if(str.find("VEC4") == 0)
	{//¼æÈÝreadKeyValue(str,"VEC4",value);
		str.erase(0,5);
	}
	int i;
	value.x() = (char)(atoi(str.c_str()));

	i = str.find(sign);
	if(i==std::string::npos)
		i = str.find('|');
	if(i==std::string::npos) return false;
	str.erase(0,i+1);
	value.y() = (char)(atoi(str.c_str()));

	i = str.find(sign);
	if(i==std::string::npos)
		i = str.find('|');
	if(i==std::string::npos) return false;
	str.erase(0,i+1);
	value.z() = (char)(atoi(str.c_str()));

	i = str.find(sign);
	if(i==std::string::npos)
		i = str.find('|');
	if(i==std::string::npos) return false;
	str.erase(0,i+1);
	value.w() = (char)(atoi(str.c_str()));
	return true;
}
bool crArgumentParser::appAtoVec(std::string str, crVector4ub &value,char sign)
{
	if(str.find("\"")==0)
	{
		str.erase(0,1);
	}
	if(str.find("VEC4") == 0)
	{//¼æÈÝreadKeyValue(str,"VEC4",value);
		str.erase(0,5);
	}
	int i;
	value.x() = (unsigned char)(atoi(str.c_str()));

	i = str.find(sign);
	if(i==std::string::npos)
		i = str.find('|');
	if(i==std::string::npos) return false;
	str.erase(0,i+1);
	value.y() = (unsigned char)(atoi(str.c_str()));

	i = str.find(sign);
	if(i==std::string::npos)
		i = str.find('|');
	if(i==std::string::npos) return false;
	str.erase(0,i+1);
	value.z() = (unsigned char)(atoi(str.c_str()));

	i = str.find(sign);
	if(i==std::string::npos)
		i = str.find('|');
	if(i==std::string::npos) return false;
	str.erase(0,i+1);
	value.w() = (unsigned char)(atoi(str.c_str()));
	return true;
}
bool crArgumentParser::appAtoVec(std::string str, std::vector<float> &value,char sign)
{
	value.clear();
	if(str.find("\"")==0)
	{
		str.erase(0,1);
	}
	int i;
	while(1)
	{
		value.push_back(atof(str.c_str()));
		i = str.find(sign);
		if(i==std::string::npos)
			i = str.find('|');
		if(i==std::string::npos) break;
		str.erase(0,i+1);
	}
	return true;
}
bool crArgumentParser::appAtoVec(std::string str, std::vector<int> &value,char sign)
{
	value.clear();
	if(str.find("\"")==0)
	{
		str.erase(0,1);
	}
	int i;
	while(1)
	{
		value.push_back(atoi(str.c_str()));
		i = str.find(sign);
		if(i==std::string::npos)
			i = str.find('|');
		if(i==std::string::npos) break;
		str.erase(0,i+1);
	}
	return true;
}
bool crArgumentParser::appAtoVec(std::string str, std::vector<short> &value,char sign)
{
	value.clear();
	if(str.find("\"")==0)
	{
		str.erase(0,1);
	}
	int i;
	while(1)
	{
		value.push_back(atoi(str.c_str()));
		i = str.find(sign);
		if(i==std::string::npos)
			i = str.find('|');
		if(i==std::string::npos) break;
		str.erase(0,i+1);
	}
	return true;
}
bool crArgumentParser::appAtoVec(std::string str, std::vector<std::string> &value,char sign)
{
	value.clear();
	if(str.find("\"")==0)
	{
		str.erase(0,1);
	}
	int i;
	while(1)
	{
		i = str.find(sign);
		if(i==std::string::npos)
			i = str.find('|');
		if(i==std::string::npos) break;
		value.push_back(str.substr(0,i));
		str.erase(0,i+1);
	}
	if(!str.empty())
		value.push_back(str);
	return true;
}
std::string crArgumentParser::appI64toa( _crInt64 inNum )
{
	int lo = LOINT64(inNum);
	int hi = HIINT64(inNum);
	return appVectoa(crVector2i(lo,hi));
}
_crInt64 crArgumentParser::appAtoI64(const std::string& str)
{
	crVector2i vec;
	appAtoVec(str,vec);
	return MAKEINT64(vec[0],vec[1]);
}

#define Semicolon '\n'

void crArgumentParser::updateOrAddKeyValue(std::string& str, const std::string &key, int value)
{
	std::string strValue = appItoa(value);

	int i = str.find(key);
	if(i<0)
	{
		if(str.empty())
			str = key + ':' + strValue;
		else
			str += Semicolon + key + ':' + strValue;
	}
	else
	{
		int keylen = key.length();
		if(str[i+keylen]==' '||str[i+keylen]==':')	
		{
			std::string endstr = str;
			endstr.erase(0,i+keylen+1);
			int j = endstr.find_first_of(Semicolon);
			if(j>0)
			{
				endstr.erase(0,j);
				str = str.substr(0,i+keylen+1) + strValue + endstr;
			}
			else
			{
				str = str.substr(0,i+keylen+1) + strValue;
			}
		}
	}
}

void crArgumentParser::updateOrAddKeyValue(std::string& str, const std::string &key, float value)
{
	std::string strValue = appFtoa(value);

	int i = str.find(key);
	if(i<0)
	{
		if(str.empty())
			str = key + ':' + strValue;
		else
			str += Semicolon + key + ':' + strValue;
	}
	else
	{
		int keylen = key.length();
		if(str[i+keylen]==' '||str[i+keylen]==':')	
		{
			std::string endstr = str;
			endstr.erase(0,i+keylen+1);
			int j = endstr.find_first_of(Semicolon);
			if(j>0)
			{
				endstr.erase(0,j);
				str = str.substr(0,i+keylen+1) + strValue + endstr;
			}
			else
			{
				str = str.substr(0,i+keylen+1) + strValue;
			}
		}
	}
}

void crArgumentParser::updateOrAddKeyValue(std::string& str, const std::string &key, const std::string &value)
{
	std::string strValue = value;
	int i = str.find(key);
	if(i<0)
	{
		if(str.empty())
			str = key + ':' + strValue;
		else
			str += Semicolon + key + ':' + strValue;
	}
	else
	{
		int keylen = key.length();
		if(str[i+keylen]==' '||str[i+keylen]==':')	
		{
			std::string endstr = str;
			endstr.erase(0,i+keylen+1);
			int j = endstr.find_first_of(Semicolon);
			if(j>0)
			{
				endstr.erase(0,j);
				str = str.substr(0,i+keylen+1) + strValue + endstr;
			}
			else
			{
				str = str.substr(0,i+keylen+1) + strValue;
			}
		}
	}
}
void crArgumentParser::updateOrAddKeyValue(std::string& str, const std::string &key, const crVector2i &value)
{
	std::string strValue = appItoa(value[0]) + ',' + appItoa(value[1]);

	int i = str.find(key);
	if(i<0)
	{
		if(str.empty())
			str = key + ':' + strValue;
		else
			str += Semicolon + key + ':' + strValue;
	}
	else
	{
		int keylen = key.length();
		if(str[i+keylen]==' '||str[i+keylen]==':')	
		{
			std::string endstr = str;
			endstr.erase(0,i+keylen+1);
			int j = endstr.find_first_of(Semicolon);
			if(j>0)
			{
				endstr.erase(0,j);
				str = str.substr(0,i+keylen+1) + strValue + endstr;
			}
			else
			{
				str = str.substr(0,i+keylen+1) + strValue;
			}
		}
	}
}
void crArgumentParser::updateOrAddKeyValue(std::string& str, const std::string &key, const crVector3i &value)
{
	std::string strValue = appItoa(value[0]) + ',' + appItoa(value[1]) + ',' + appItoa(value[2]);

	int i = str.find(key);
	if(i<0)
	{
		if(str.empty())
			str = key + ':' + strValue;
		else
			str += Semicolon + key + ':' + strValue;
	}
	else
	{
		int keylen = key.length();
		if(str[i+keylen]==' '||str[i+keylen]==':')	
		{
			std::string endstr = str;
			endstr.erase(0,i+keylen+1);
			int j = endstr.find_first_of(Semicolon);
			if(j>0)
			{
				endstr.erase(0,j);
				str = str.substr(0,i+keylen+1) + strValue + endstr;
			}
			else
			{
				str = str.substr(0,i+keylen+1) + strValue;
			}
		}
	}
}
void crArgumentParser::updateOrAddKeyValue(std::string& str, const std::string &key, const crVector4i &value)
{
	std::string strValue = appItoa(value[0]) + ',' + appItoa(value[1]) + ',' + appItoa(value[2]) + ',' + appItoa(value[3]);

	int i = str.find(key);
	if(i<0)
	{
		if(str.empty())
			str = key + ':' + strValue;
		else
			str += Semicolon + key + ':' + strValue;
	}
	else
	{
		int keylen = key.length();
		if(str[i+keylen]==' '||str[i+keylen]==':')	
		{
			std::string endstr = str;
			endstr.erase(0,i+keylen+1);
			int j = endstr.find_first_of(Semicolon);
			if(j>0)
			{
				endstr.erase(0,j);
				str = str.substr(0,i+keylen+1) + strValue + endstr;
			}
			else
			{
				str = str.substr(0,i+keylen+1) + strValue;
			}
		}
	}
}
void crArgumentParser::updateOrAddKeyValue(std::string& str, const std::string &key, const crVector2 &value)
{
	std::string strValue = appFtoa(value[0]) + ',' + appFtoa(value[1]);

	int i = str.find(key);
	if(i<0)
	{
		if(str.empty())
			str = key + ':' + strValue;
		else
			str += Semicolon + key + ':' + strValue;
	}
	else
	{
		int keylen = key.length();
		if(str[i+keylen]==' '||str[i+keylen]==':')	
		{
			std::string endstr = str;
			endstr.erase(0,i+keylen+1);
			int j = endstr.find_first_of(Semicolon);
			if(j>0)
			{
				endstr.erase(0,j);
				str = str.substr(0,i+keylen+1) + strValue + endstr;
			}
			else
			{
				str = str.substr(0,i+keylen+1) + strValue;
			}
		}
	}
}
void crArgumentParser::updateOrAddKeyValue(std::string& str, const std::string &key, const crVector3 &value)
{
	std::string strValue = appFtoa(value[0]) + ',' + appFtoa(value[1]) + ',' + appFtoa(value[2]);

	int i = str.find(key);
	if(i<0)
	{
		if(str.empty())
			str = key + ':' + strValue;
		else
		    str += Semicolon + key + ':' + strValue;
	}
	else
	{
		int keylen = key.length();
		if(str[i+keylen]==' '||str[i+keylen]==':')	
		{
			std::string endstr = str;
			endstr.erase(0,i+keylen+1);
			int j = endstr.find_first_of(Semicolon);
			if(j>0)
			{
				endstr.erase(0,j);
				str = str.substr(0,i+keylen+1) + strValue + endstr;
			}
			else
			{
				str = str.substr(0,i+keylen+1) + strValue;
			}
		}
	}
}

void crArgumentParser::updateOrAddKeyValue(std::string& str, const std::string &key, const crVector4 &value)
{
	std::string strValue = appFtoa(value[0]) + ',' + appFtoa(value[1]) + ',' + appFtoa(value[2]) + ',' + appFtoa(value[3]);

	int i = str.find(key);
	if(i<0)
	{
		if(str.empty())
			str = key + ':' + strValue;
		else
			str += Semicolon + key + ':' + strValue;
	}
	else
	{
		int keylen = key.length();
		if(str[i+keylen]==' '||str[i+keylen]==':')	
		{
			std::string endstr = str;
			endstr.erase(0,i+keylen+1);
			int j = endstr.find_first_of(Semicolon);
			if(j>0)
			{
				endstr.erase(0,j);
				str = str.substr(0,i+keylen+1) + strValue + endstr;
			}
			else
			{
				str = str.substr(0,i+keylen+1) + strValue;
			}
		}
	}
}

bool crArgumentParser::deleteKey(std::string& str, const std::string &key)
{
	std::string value;
	if(readKeyValue(str,key.c_str(),value))
	{
		int i = str.find(key);
		int j = str.find(value) + value.length();
		int strLength = str.length();
		if(j+1<strLength && str[j+1] == '\n')
		{
			j++;
		}
	    str.erase(i,j);
		return true;
	}
	return false;
}

std::string crArgumentParser::getFilePath(const std::string& fileName)
{
	std::string::size_type slash1 = fileName.find_last_of('/');
	std::string::size_type slash2 = fileName.find_last_of('\\');
	if (slash1==std::string::npos) 
	{
		if (slash2==std::string::npos) return std::string();
		return std::string(fileName,0,slash2);
	}
	if (slash2==std::string::npos) return std::string(fileName,0,slash1);
	return std::string(fileName, 0, slash1>slash2 ?  slash1 : slash2);
}


std::string crArgumentParser::getSimpleFileName(const std::string& fileName)
{
	std::string::size_type slash1 = fileName.find_last_of('/');
	std::string::size_type slash2 = fileName.find_last_of('\\');
	if (slash1==std::string::npos) 
	{
		if (slash2==std::string::npos) return fileName;
		return std::string(fileName.begin()+slash2+1,fileName.end());
	}
	if (slash2==std::string::npos) return std::string(fileName.begin()+slash1+1,fileName.end());
	return std::string(fileName.begin()+(slash1>slash2?slash1:slash2)+1,fileName.end());
}

std::string crArgumentParser::getFileExtension(const std::string& fileName)
{
	std::string::size_type dot = fileName.find_last_of('.');
	if (dot==std::string::npos) return std::string("");
	return std::string(fileName.begin()+dot+1,fileName.end());
}

std::string crArgumentParser::getFileNameEliminateExt(const std::string& fileName)
{
	std::string::size_type dot = fileName.find_last_of('.');
	if (dot==std::string::npos) return fileName;
	return std::string(fileName.begin(),fileName.begin()+dot);
}

bool crArgumentParser::equalCaseInsensitive(const std::string& lhs,const char* rhs)
{
	if (rhs==NULL || lhs.size()!=strlen(rhs)) return false;
	std::string::const_iterator litr = lhs.begin();
	const char* cptr = rhs;
	while (litr!=lhs.end())
	{
		if (tolower(*litr)!=tolower(*cptr)) return false;
		++litr;
		++cptr;
	}
	return true;
}