// $Id: Item.cpp,v 1.1 2005/02/09 17:26:57 deepone Exp $

#include "rcfg/Item.h"
#include <cassert>
#include <sstream>
#include <stdexcept>



std::ostream& operator <<(std::ostream& os, const std::string s) 
{ 
  os << "\"" << s.c_str() << "\""; 
  return os; 
}

///
std::ostream& operator <<(std::ostream& os, const Array& v)
{
  os << "[ ";
  float val;
  for(Array::const_iterator vi=v.begin(); vi != v.end(); vi++) {
    bool b;
    // Is it a string?
    if ((*vi)->Type() == ConfigValue::VALUE_STRING) {
      std::string s_val;
      b = (*vi)->GetStringValue(s_val);
      os << s_val << " ";
    }
    // No, its a string of other things...
    else {
      b = (*vi)->GetLiteralValue(val);
      if (!b) {
        std::string s_val;
        b = (*vi)->GetStringValue(s_val);
        std::ostringstream str;
        str << __FILE__ << ": " << __LINE__ << ": " << "Invalid value in array, unable to stream value";
        throw std::runtime_error( str.str() );
      }
      os << val << " ";
    }
  }

  os << "]";


  return os;
};

std::ostream& operator <<(std::ostream& os, const FloatVector& v)
{
  os << "[ ";
  for(FloatVector::const_iterator vi=v.begin(); vi != v.end(); vi++)
    os << *vi << " ";
  os << "]";

  return os;
};

std::ostream& operator <<(std::ostream& os, const StringVector& v)
{
  os << "[ ";
  for(StringVector::const_iterator vi=v.begin(); vi != v.end(); vi++)
    os << *vi << " ";
  os << "]";

  return os;
};

///
std::ostream& operator <<(std::ostream& os, Expression* e)
{
  assert(e && "Expression set to null");
  os << e->GetExpression();

  return os;
};

///
std::ostream& operator <<(std::ostream& os, Expression e)
{

  os << e.GetExpression();

  return os;
};

std::ostream& operator << (std::ostream& os, Array *vec)
{

  os << "[ ";
  for(Array::iterator vi=vec->begin(); vi < vec->end(); vi++)
    os << *vi << " ";

  os << "]";


  return os;
}

/// 
std::ostream& operator << (std::ostream& os, StructMap *struct_map)
{

  
  StructMap::const_iterator smi;

  os << "{\n";

  for(smi=struct_map->begin(); smi != struct_map->end(); smi++) {
    os << " \t " << smi->first.c_str() << " \t ";
    os << *(smi->second)  <<"\n";
  }
  
  os << "}\n";
  return os;
}

StructMap::~StructMap()
{
    StructMapIterator si = begin(); 
    for(;si != end(); si++) {
      delete si->second;
    }

    clear();
}

StructMapIterator StructMap::replace(const std::string& s, ConfigValue *value) 
  { 
    StructMapIterator si = find(s); 
    
    if (si != end()) {
      delete si->second;
      erase(si);
    }

    return insert(std::make_pair(s, value)); 
  }


  bool StructMap::GetVal(const std::string& key, Array& result) {

    bool flag=false;
    StructMapIterator smi;
    smi = find(key);
    if (smi != end()) {
      flag = ((FloatVectorItem *)smi->second)->GetVal(result);
    }

    return flag;
 }
  
  bool StructMap::GetVal(const std::string& key, std::vector<float>& result) {

    bool flag=false;
    StructMapIterator smi;
    smi = find(key);
    if (smi != end()) {
      flag = ((FloatVectorItem *)smi->second)->GetVal(result);
    }

    return flag;
 }

  bool StructMap::GetVal(const std::string& key, std::vector<std::string>& result) {

    bool flag=false;
    StructMapIterator smi;
    smi = find(key);
    if (smi != end()) {
      flag = ((FloatVectorItem *)smi->second)->GetVal(result);
    }

    return flag;
 }

 bool StructMap::GetVal(const std::string& key, std::string& result) {

    bool flag=false;
    StructMapIterator smi;
    smi = find(key);
    if (smi != end()) {
      flag = ((StringItem *)smi->second)->GetVal(result);
    }

    return flag;
  };
 
  bool StructMap::GetVal(const std::string& key, int& result) {

    bool flag=false;
    StructMapIterator smi;
    smi = find(key);
    if (smi != end()) {
      flag = ((IntItem *)smi->second)->GetVal(result);
    }

    return flag;
  };

  bool StructMap::GetVal(const std::string& key, float& result) {

    bool flag=false;
    ConfigValue *smptr=0;
    StructMapIterator smi;
    smi = find(key);
    if (smi != end()) 
      smptr = smi->second;
    else 
      return false;

    flag = smptr->GetLiteralValue(result);
    return flag;
  }

  bool StructMap::getScopeVariable(const std::string& name, float& value)
  {
    StructMap  *smptr = this;
    assert(smptr && "StructMapIterm::getScopeLiteral: smptr == null");


    bool found = false;
    do {
      found = smptr->GetVal(name, value);
      if (!found) {
        smptr = smptr->GetParent();
      }
    } while (!found && smptr);

    return found;
  }


  void Array::clear(bool delete_flag)
  {
    if (delete_flag) {
      iterator it;
      for(it=begin(); it != end(); it++) {
        delete *it;
      }
    }
    m_vec.clear();
    m_vec.resize(0);
  }

  Array& Array::operator=(const Array& copy)
  {
    if (this == &copy)
      return *this;

    const_iterator it;
    this->clear(true);

    for(it=copy.begin(); it != copy.end(); it++)
      push_back((*it)->Copy());

    return *this;
  }

  Array::~Array()
  {
    clear(true);
  }

  float Array::operator[](unsigned int i)
  {
    float value;
    bool b;
    assert(i < size() && "Invalid index into array");

    ConfigValue *v = m_vec[i];//at(i);
    
    b = v->GetLiteralValue(value);
    assert(b && "ConfigValue pointer not pointing at a value that can be converted to a float");
    return value;
    
  }

  void Array::GetFloatVector(std::vector<float>& vec)
  {
    const_iterator it;

    vec.clear();

    int i=0;
    for(it=begin(); it != end(); it++,i++)
      vec.push_back((*this)[i]);
  }

  void Array::ith(unsigned int i, std::string& s_val) 
  {
    std::string value;
    bool b;
    assert(i < size() && "Invalid index into array");

    ConfigValue *v = m_vec[i];//at(i);
    
    b = v->GetStringValue(s_val);
    assert(b && "ConfigValue pointer not pointing at a value that can be converted to a string");
  }
  
  void Array::GetStringVector(std::vector<std::string>& vec)
  {
    const_iterator it;

    vec.clear();

    int i=0;
    std::string val;
    for(it=begin(); it != end(); it++,i++) {
      this->ith(i, val);    
      vec.push_back(val);
    }
  }


  Array::Array(const FloatVector& vec) 
  { 
    *this = vec;
/*    FloatVector::const_iterator ci;
    for(ci=vec.begin(); ci != vec.end(); ci++) {
      m_vec.push_back( new FloatItem(*ci) );
    }*/
  }

  Array::Array(const std::vector<std::string>& vec) 
  { 
    *this = vec;
/*    FloatVector::const_iterator ci;
    for(ci=vec.begin(); ci != vec.end(); ci++) {
      m_vec.push_back( new FloatItem(*ci) );
    }*/
  }

  Array& Array::operator=(const std::vector<std::string>& copy)
  {
    this->clear(true);

    std::vector<std::string>::const_iterator ci;
    for(ci=copy.begin(); ci != copy.end(); ci++)
      push_back( new StringItem(*ci) );
    return *this;
  }

 

  Array& Array::operator=(const FloatVector& copy)
  {
    this->clear(true);

    FloatVector::const_iterator ci;
    for(ci=copy.begin(); ci != copy.end(); ci++)
      push_back( new FloatItem(*ci) );
    return *this;
  }

  /*------------------------------------------

* $Source: /cvsroot/replicantbody/replicantbody/src/rcfg/Item.cpp,v $
* $Revision: 1.1 $ 
* $Date: 2005/02/09 17:26:57 $
* $Author: deepone $ 
* $Locker:  $

* $Log: Item.cpp,v $
* Revision 1.1  2005/02/09 17:26:57  deepone
* vrutils and cfgscript put into new namespaces
*
* Revision 1.1.1.1  2005/02/06 06:52:59  deepone
* initial sourceforge checkin
*
* Revision 1.5  2003/12/17 09:28:51  andersb
* Made configscript a dll library under windows
* Fixed memory leaks.
* Fixed bug in processing included files in wrong order.
*
* Revision 1.4  2003/12/16 11:34:45  andersb
* *** empty log message ***
*
* Revision 1.3  2003/12/05 10:43:37  deepone
* *** empty log message ***
*
* Revision 1.2  2003/12/02 15:40:14  andersb
* Added scoped struct access within the scripts.
*
* Revision 1.1  2003/10/27 13:45:48  andersb
* Moved .cxx to .cpp
*
* Revision 1.18  2003/05/05 14:11:18  andersb
* *** empty log message ***
*
* Revision 1.17  2003/04/24 06:23:05  andersb
* Added support for Array of Strings
*
* Revision 1.16  2003/03/04 11:19:53  Anders Backman
* Fixed bug that required data beginning on the same line as the key,
* Added a search path feature. Default the environment variable CFG_FILE_PATH is enquired
* for a list of directories to search config files in.
*
* Revision 1.15  2003/02/27 13:27:54  andersb
* Moved over to sstream instead of strstream
*
* Revision 1.14  2002/10/22 12:30:57  Anders Backman
* Moved around include files.
*
* Revision 1.13  2002/09/24 07:12:45  andersb
* *** empty log message ***
*
* Revision 1.12  2002/09/23 06:33:35  andersb
* no message
*
* Revision 1.11  2002/08/12 19:11:28  andersb
* Added some missing methods for converting between float vectors and Array.
*
* Revision 1.10  2002/08/06 11:34:50  andersb
* Rewrote Array class so it hides the std::vector. This solved problem
* when using method at(i) with overloaded operator[] under unix.
*
* Revision 1.9  2002/07/09 21:28:57  andersb
* Fixed case bug in Expressions allowing only lowercase variables.
* Added expressions in arrays too.
* Added randInterval(min,max) function in Expressions allowing expressions like:
*
* Position [randInterval(-10,10) randInterval(-10,10) randInterval(0,3)]
*
* Revision 1.8  2002/07/04 23:08:08  andersb
* Added expressions.
* This helps alot writing scripts, where math expressions is available.
* Most standard functions: ceil, floor, sin, cos, etc is available.
* Also pi (3.1415...) and e ( 2.718 ) as constants.
* Scope is also implemented. (a variable name is backtraced upwards from the current
* scope.
*
* Revision 1.7  2002/06/10 10:36:34  andersb
* StructMap::GetVal(float) can now also return a integer value.
*
* Revision 1.6  2002/02/01 13:35:30  andersb
* Now fully updated to support recursive structs.
* Still problems with case sensitivity.
*
* Revision 1.1.1.1  2002/01/02 13:45:31  andersb
* A new version of Configscript using namespace rcfg.
* Also supporting recursive structures.
*
* Revision 1.5  2001/08/29 09:23:37  andersb
* Now using the old istd::ostream.h library to get it to work together with
* CommonC++.
*
* Revision 1.4  2001/08/10 14:13:57  andersb
* no message
*
* Revision 1.3  2001/08/01 07:11:23  andersb
* Major update
*
* Revision 1.2  2001/04/25 12:45:04  andersb
* Added makefiles for the IRIX platform.
*
* Revision 1.1.1.1  2001/04/25 10:56:28  andersb
* no message
*
* Revision 1.7  2000/11/03 14:48:03  andersb
* no message
*
* Revision 1.6  2000/10/10 15:04:26  andersb
* no message
*
* Revision 1.5  2000/10/05 10:27:07  andersb
* no message
*
* Revision 1.4  2000/10/03 09:21:24  andersb
* no message
*
* Revision 1.3  2000/10/03 07:44:37  andersb
* Using DOC++
*
* Revision 1.2  2000/10/03 07:20:31  andersb
* no message
*

--------------------------------------------*/
