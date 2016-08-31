// $Id: Item.h,v 1.2 2005/02/09 21:17:47 deepone Exp $

#ifndef __ITEM_H__
#define __ITEM_H__

#ifdef _WIN32
#pragma warning(disable:4786) // Disable warnings about long names

#include "rvrutils/os/memory_debug.h"

#endif



#include <string>
#include <iostream>
#include <vector>
#include <map>
#include "rcfg/ConfigValue.h"
#include "rcfg/utils.h"
#include <sstream>


#include "rcfg/ExpressionSolver.h"
#include "rcfg/export.h"

typedef std::vector<float> FloatVector;

class StringItem;
typedef std::vector<StringItem> StringVector;

class StructMap;
class ConfigValue;

class  CONFIGSCRIPT_API Array /*: public std::vector<ConfigValue *> */{
public:
typedef std::vector<ConfigValue *> ConfigVector;
 typedef ConfigVector::const_iterator const_iterator;
 typedef ConfigVector::iterator iterator;
 typedef ConfigVector::value_type value_type;
public:
  Array() {}
  Array(const FloatVector& vec);
  Array(const std::vector<std::string>& vec);

  Array(const Array& copy) {
    if (&copy == this)
      return;

   *this = copy;
  }

  Array& operator=(const Array& copy);
  Array& operator=(const FloatVector& copy);
  Array& operator=(const std::vector<std::string>& copy);

  iterator begin() { return m_vec.begin(); }
  iterator end() { return m_vec.end(); }

  const_iterator begin() const { return m_vec.begin(); }
  const_iterator end() const { return m_vec.end(); }

  void clear( bool delete_flag=false );

  size_t size() { return m_vec.size(); }
  iterator erase(iterator it) { return m_vec.erase(it); }
  
  void push_back(value_type v) { m_vec.push_back(v); }

  void update(){};

  ~Array();
  float operator[](unsigned int i);

  void ith(unsigned int i, std::string& s_val);
//  void push_back(ConfigValue *v) { m_vec.push_back(v); }
//  unsigned int size() {return m_vec.size(); }
  void GetFloatVector(std::vector<float>& vec);
  void GetStringVector(std::vector<std::string>& vec);
private:
  ConfigVector m_vec;
 
};
///


  /*! This is a major hack. We should not know about std::string item here. But as I have to use
    the old iostream.h, iostream.h does not know about std::string so we have to make it know!
    The alternative would have been to force all users to use a derived std::string that has a method Put.
  */
//std::ostream& operator <<(std::ostream& os, const std::string s);


class Expression;

/// Class for storing items, derived from ConfigValue
template<class T> class Item : public ConfigValue {
public:
    
  /// Constructor, sets the value and the type of the Item
  Item( T val, ValueType type ) : ConfigValue(type) { _val = val; };

  ///
  virtual ~Item() {};


  /// Writes an item to ostream
  virtual std::ostream& Put (std::ostream& os) const {/* os.flags(std::ios_base::showpoint); */os << _val; return os; };

  virtual ConfigValue *Copy( void ) {std::cerr << "Never get here" << std::endl; return 0;}; //std::cerr << _val << std::endl; Item<T> *ptr = new Item<T>(_val, Type()); return ptr; };
  

  /// Returns a reference to the stored value
  T GetVal( void )  { return _val; };
  T& GetRef( void ) { return _val; }
  bool GetVal( int& i ) 
  { 
    if (Type() == ConfigValue::VALUE_INT)  {
      i = (int)_val; 
      return true;
    }
    else
      return false;
  };

  bool GetVal( float& i ) 
  { 
    if (Type() == ConfigValue::VALUE_FLOAT)  {
      i = (float)_val; 
      return true;
    }
    else
      return false;
  };

  bool GetVal( std::string& i ) 
  { 
    std::ostringstream str;
    if (Type() == ConfigValue::VALUE_STRING)  {
      str << _val;
      i = str.str();
      return true;
    }
    else
      return false;
  };


  bool GetVal( Array &i ) 
  { 
    if (Type() == ConfigValue::VALUE_FLOAT_ARRAY ||Type() == ConfigValue::VALUE_STRING_ARRAY )  {
      i = (Array )_val; 
      return true;
    }
    else
      return false;
  };
  
  bool GetVal( std::vector<float>& i ) 
  { 
    if (Type() == ConfigValue::VALUE_FLOAT_ARRAY)  {
      ((Array *)(&_val))->GetFloatVector(i); 
      return true;
    }
    else
      return false;
  };

  bool GetVal( std::vector<std::string>& i ) 
  { 
    if (Type() == ConfigValue::VALUE_STRING_ARRAY)  {
      ((Array *)(&_val))->GetStringVector(i); 
      return true;
    }
    else
      return false;
  };

  bool GetVal( StructMap * &i ) 
  { 
    if (Type() == ConfigValue::VALUE_STRUCT)  {
      i = (StructMap *)_val; 
      return true;
    }
    else
      return false;
  };

  bool GetVal( Expression &i ) 
  { 
    if (Type() == ConfigValue::VALUE_EXPRESSION)  {
      i = (Expression )_val; 
      return true;
    }
    else
      return false;
  };

protected:
  ///
  T _val;
};


///
class  IntItem : public Item<int> {
public:
  ///
  IntItem(int i ) : Item<int>(i, ConfigValue::VALUE_INT) {};
  ~IntItem(){};
  ConfigValue *Copy( void ) { /*std::cerr << _val << std::endl; */IntItem *ptr = new IntItem(_val); return ptr; };
  std::string Description() const { return "integer"; };


};

///
class   FloatItem : public Item<float> {
public:
  ///
  FloatItem(float i ) : Item<float>(i, ConfigValue::VALUE_FLOAT) {};
  ~FloatItem() {};

  ConfigValue *Copy( void ) { /*std::cerr << _val << std::endl; */FloatItem *ptr = new FloatItem(_val); return ptr; };
  
  std::ostream& Put (std::ostream& os) const {  os << _val; return os; };
  std::string Description() const { return "float"; };
};

///
class   StringItem : public Item<std::string> {
public:
  ///
  StringItem(std::string i ) : Item<std::string>(i, ConfigValue::VALUE_STRING) {};
  ~StringItem() {};


  ConfigValue *Copy( void ) { /*std::cerr << _val.c_str() << std::endl; */StringItem *ptr = new StringItem(_val); return ptr; };
  std::ostream& Put (std::ostream& os) const { os << "\"" << _val.c_str() << "\""; return os; };
  std::string Description() const { return "string"; };

};



///
class   FloatVectorItem : public Item<Array> {
public:
  FloatVectorItem(const Array& i, ConfigValue::ValueType type=ConfigValue::VALUE_FLOAT_ARRAY) : Item<Array>(i, type) {};
  FloatVectorItem(const FloatVector& vec) : Item<Array>(vec, ConfigValue::VALUE_FLOAT_ARRAY) {};
  FloatVectorItem(const std::vector<std::string>& vec) : Item<Array>(vec, ConfigValue::VALUE_STRING_ARRAY) {};
  ConfigValue *Copy( void ) {  FloatVectorItem *ptr = new FloatVectorItem(_val); return ptr; };
  ~FloatVectorItem() {  };
  std::string Description() const { return "array"; };

  virtual void Update(){};


};

/*class FloatVectorItem : public Item<FloatVector *> {
public:
  ///
  FloatVectorItem(const FloatVector *i) : Item<FloatVector *>(new FloatVector(*i), ConfigValue::VALUE_ARRAY) {};
};
*/


//#define __STRUCTMAP_STRING_CONFIG std::map<std::string, ConfigValue *>
///
//typedef __STRUCTMAP_STRING_CONFIG::iterator StructMapIterator;
typedef std::multimap<std::string, ConfigValue *>::iterator StructMapIterator;
///
class   StructMap : public std::multimap<std::string, ConfigValue *> {
public:
  
  StructMap() : m_parent(0L) {}
  ///
  virtual ~StructMap();

  ConfigValue *findConfigValue(const std::string& key)
  {
    StructMapIterator smi;
    smi = this->find(key);
    if (smi != this->end())
      return (smi->second);
    
    return 0L;
  }

  ///
  /*StructMapIterator find(const std::string& s) {

    for(StructMapIterator ii=begin(); ii != end(); ii++)
      if (!cmp_nocase(s, ii->first))
        return ii;
    return end();
  };*/

//  std::pair<StructMapIterator,bool> replace(const std::string& s, ConfigValue *value) 
  StructMapIterator replace(const std::string& s, ConfigValue *value);


  bool GetVal(const std::string& key, FloatVector& result);
  bool GetVal(const std::string& key, std::vector<std::string>& result);

  bool GetVal(const std::string& key, Array& result);
  
  bool GetVal(const std::string& key, std::string& result);
 
  bool GetVal(const std::string& key, int& result);

  bool GetVal(const std::string& key, float& result);

  bool GetVal(const std::string& key, StructMap* &result);
  bool GetVal(const std::string& key, ConfigValue* &result);

  // recursivly recalculates all subelements that are expressions
  void Update(){};

  StructMap *GetParent() const { return m_parent; }
  void SetParent( StructMap *sm) { m_parent = sm; }

  /*!
  Tries to find the item named name. name must be a float, integer or an expression item
  First the current scope is searched (within this StructItem), if it is not found, it will be searched for
  in the parents scope, etc, until the top scope is reached.
  \return true if variable name is found, otherwise false
  */
  bool getScopeVariable(const std::string& name, float& value);


private:
  StructMap *m_parent;
   
};



typedef Item<StructMap *> StructItem;
///
class   StructMapItem : public Item<StructMap *> {
public:
  ///
  StructMapItem(StructMap *i) : Item<StructMap *>(i, ConfigValue::VALUE_STRUCT) {};
  ~StructMapItem () {
    delete _val;
  }
  
  // recursivly recalculates all subelements that are expressions
  virtual void Update(){};

  std::string Description() const { return "struct"; };

};

inline bool StructMap::GetVal(const std::string& key, StructMap* &result)
 {

  bool flag=false;
  StructMapIterator smi;
  smi = find(key);
  if (smi != end()) {
    flag = ((StructItem *)smi->second)->GetVal(result);
  }

  return flag;
}

inline bool StructMap::GetVal(const std::string& key, ConfigValue* &result)
 {

  bool flag=false;
  StructMapIterator smi;
  smi = find(key);
  if (smi != end()) {
    result = smi->second;
    return true;
  }

  return flag;
}

class CONFIGSCRIPT_API Expression {
public:
  Expression(const std::string& expression, const ExpressionSolver::ExpressionType result) : 
      m_expression(expression), m_result(result) {}

    Expression() : m_expression(""), m_result(0) {}
    std::string GetExpression() { return m_expression; }
    ExpressionSolver::ExpressionType GetVal() { return m_result; }

    // recursivly recalculates all subelements that are expressions
    virtual void Update() {};
    virtual ~Expression() {}

private:
  std::string m_expression;
  ExpressionSolver::ExpressionType m_result;
};

class   ExpressionItem : public Item<Expression> {
public:
  ExpressionItem(const Expression& e) : Item<Expression>(e, ConfigValue::VALUE_EXPRESSION) {};

  // recursivly recalculates all subelements that are expressions
  virtual void Update() {};

  ConfigValue *Copy( void ) {  ExpressionItem *ptr = new ExpressionItem (_val); return ptr; };
  virtual ~ExpressionItem() {};
  std::string Description() const { return "Expression"; };
};


/// Prints a Array
CONFIGSCRIPT_API std::ostream&   operator <<(std::ostream& os, const Array& v); 

/// Prints a FloatVector
CONFIGSCRIPT_API  std::ostream&   operator <<(std::ostream& os, const FloatVector& v); 

/// Prints a StringVector
CONFIGSCRIPT_API std::ostream&   operator <<(std::ostream& os, const StringVector& v); 

/// Prints a Array
CONFIGSCRIPT_API std::ostream&   operator <<(std::ostream& os, Array *);

/// Prints a StructMap
CONFIGSCRIPT_API std::ostream&   operator <<(std::ostream& os, StructMap *);

/// Prints an Expression
CONFIGSCRIPT_API std::ostream&   operator <<(std::ostream& os, Expression *);

/// Prints an Expression
CONFIGSCRIPT_API std::ostream&   operator <<(std::ostream& os, Expression e);

#endif

/*------------------------------------------

* $Source: /cvsroot/replicantbody/replicantbody/include/rcfg/Item.h,v $
* $Revision: 1.2 $ 
* $Date: 2005/02/09 21:17:47 $
* $Author: deepone $ 
* $Locker:  $

* $Log: Item.h,v $
* Revision 1.2  2005/02/09 21:17:47  deepone
* *** empty log message ***
*
* Revision 1.1  2005/02/09 17:26:55  deepone
* vrutils and cfgscript put into new namespaces
*
* Revision 1.1.1.1  2005/02/06 06:53:00  deepone
* initial sourceforge checkin
*
* Revision 1.8  2003/12/17 09:28:50  andersb
* Made configscript a dll library under windows
* Fixed memory leaks.
* Fixed bug in processing included files in wrong order.
*
* Revision 1.7  2003/12/16 11:34:44  andersb
* *** empty log message ***
*
* Revision 1.6  2003/12/05 10:43:37  deepone
* *** empty log message ***
*
* Revision 1.5  2003/12/02 15:40:13  andersb
* Added scoped struct access within the scripts.
*
* Revision 1.4  2003/11/19 19:14:31  andersb
* Added GetType method for ConfigScript. Returns the type of a named item
*
* Revision 1.3  2003/04/24 06:20:58  andersb
* Added support for Array of Strings
*
* Revision 1.2  2003/02/27 13:27:53  andersb
* Moved over to sstream instead of strstream
*
* Revision 1.1  2002/10/22 12:30:55  Anders Backman
* Moved around include files.
*
* Revision 1.18  2002/08/12 19:11:01  andersb
* Added some missing methods for converting between float vectors and Array.
*
* Revision 1.17  2002/08/06 14:24:01  andersb
* Ahh it works again, Daniels fault (remember alphanumeric?)
*
* Revision 1.16  2002/08/06 11:34:50  andersb
* Rewrote Array class so it hides the std::vector. This solved problem
* when using method at(i) with overloaded operator[] under unix.
*
* Revision 1.15  2002/08/01 14:36:17  andersb
* Fixed so that requesting a int casts an expression and a float to an int.
*
* Revision 1.14  2002/07/09 21:28:46  andersb
* Fixed case bug in Expressions allowing only lowercase variables.
* Added expressions in arrays too.
* Added randInterval(min,max) function in Expressions allowing expressions like:
*
* Position [randInterval(-10,10) randInterval(-10,10) randInterval(0,3)]
*
* Revision 1.13  2002/07/04 23:08:47  andersb
* Added expressions.
* This helps alot writing scripts, where math expressions is available.
* Most standard functions: ceil, floor, sin, cos, etc is available.
* Also pi (3.1415...) and e ( 2.718 ) as constants.
* Scope is also implemented. (a variable name is backtraced upwards from the current
* scope.
*
* Revision 1.12  2002/06/10 10:36:24  andersb
* StructMap::GetVal(float) can now also return a integer value.
*
* Revision 1.11  2002/04/09 09:04:48  andersb
* Major rewrite of ConfigIO.cxx.
* Now supports multiline strings and arrays.
*
* Revision 1.10  2002/03/13 09:03:54  andersb
* Compile with -D_THREAD_SAFE if thread safety is required. This also requires
* CommonC++ version 1.9.3 or higher.
*
* Also now Case sensitive for all items.
*
* Revision 1.9  2002/02/01 13:35:21  andersb
* Now fully updated to support recursive structs.
* Still problems with case sensitivity.
*
* Revision 1.2  2002/01/08 12:24:13  andersb
* *** empty log message ***
*
* Revision 1.1.1.1  2002/01/02 13:45:30  andersb
* A new version of Configscript using namespace rcfg.
* Also supporting recursive structures.
*
* Revision 1.8  2001/11/14 13:12:47  andersb
* New features added:
* Hiearchial structures supported.
* Possible to use a mapID when opening to
* support multiple open configscripts.
*
* Revision 1.7  2001/09/19 13:15:44  andersb
* Minur bug in output detected and fixed.
*
* Revision 1.6  2001/09/05 09:03:19  andersb
* Working under SGI.Working under SGI.
*
* Revision 1.5  2001/08/29 09:23:35  andersb
* Now using the old iostream.h library to get it to work together with
* CommonC++.
*
* Revision 1.4  2001/08/10 14:13:55  andersb
* no message
*
* Revision 1.3  2001/08/01 07:11:20  andersb
* Major update
*
* Revision 1.2  2001/04/25 12:45:01  andersb
* Added makefiles for the IRIX platform.
*
* Revision 1.1.1.1  2001/04/25 10:56:27  andersb
* no message
*
* Revision 1.13  2000/11/24 14:09:31  andersb
* no message
*
* Revision 1.12  2000/11/14 16:04:26  andersb
* Getting there...
*
* Revision 1.11  2000/11/09 15:21:26  andersb
* no message
*
* Revision 1.10  2000/11/03 14:48:03  andersb
* no message
*
* Revision 1.9  2000/10/16 08:43:40  andersb
* no message
*
* Revision 1.8  2000/10/10 15:04:27  andersb
* no message
*
* Revision 1.7  2000/10/09 13:02:46  andersb
* no message
*
* Revision 1.6  2000/10/05 10:27:08  andersb
* no message
*
* Revision 1.5  2000/10/03 09:37:24  andersb
* *** empty log message ***
*
* Revision 1.4  2000/10/03 09:21:25  andersb
* no message
*
* Revision 1.3  2000/10/03 07:44:38  andersb
* Using DOC++
*
* Revision 1.2  2000/10/03 07:20:32  andersb
* no message
*

--------------------------------------------*/
