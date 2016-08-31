// $Id: ConfigValue.h,v 1.1 2005/02/09 17:26:55 deepone Exp $

#ifndef __CONFIGVALUE_H__
#define __CONFIGVALUE_H__

#ifdef _WIN32
#pragma warning(disable:4786) // Disable warnings about long names

#include "rvrutils/os/memory_debug.h"
#endif



#include <string>
#include <iostream>
#include <vector>
#include <map>
#include "rcfg/export.h"

///


/// Baseclass for Items stored in setupfile
class  CONFIGSCRIPT_API ConfigValue {
public:

  /// Defines the type of the item
  enum ValueType { VALUE_NONE, 
                   VALUE_INT, 
                   VALUE_FLOAT, 
                   VALUE_STRING, 
                   VALUE_FLOAT_ARRAY, 
                   VALUE_STRUCT, 
                   VALUE_EXPRESSION, 
                   VALUE_STRING_ARRAY,
                   VALUE_NUMBER = VALUE_INT | VALUE_FLOAT | VALUE_EXPRESSION
                  };

  
  ConfigValue( ValueType type ) : _type(type) {};

  virtual ConfigValue *Copy( void ) { throw "ConfigValue::Copy() Never get here"; };
    
  ///
  virtual ~ConfigValue() {};


  /// Recalculates value if it is an expression (or a struct containing an expression
  virtual void Update() {}
  ///
  virtual std::ostream& Put( std::ostream& os )const { os << "Never get here" << std::endl; return os; };

  ///
  friend std::ostream& operator <<(std::ostream& os, const ConfigValue &vo ) { vo.Put(os); return os; }

  bool GetLiteralValue( float& value );
  bool GetStringValue(std::string& value);

  /// Returns the type of the stored item
  ValueType Type( void ) const { return _type; };

  virtual std::string Description() const =0;

protected:
  ///
  ValueType _type;

};





#endif
/*------------------------------------------

* $Source: /cvsroot/replicantbody/replicantbody/include/rcfg/ConfigValue.h,v $
* $Revision: 1.1 $ 
* $Date: 2005/02/09 17:26:55 $
* $Author: deepone $ 
* $Locker:  $

* $Log: ConfigValue.h,v $
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
* Revision 1.3  2002/07/09 21:28:46  andersb
* Fixed case bug in Expressions allowing only lowercase variables.
* Added expressions in arrays too.
* Added randInterval(min,max) function in Expressions allowing expressions like:
*
* Position [randInterval(-10,10) randInterval(-10,10) randInterval(0,3)]
*
* Revision 1.2  2002/07/04 23:08:47  andersb
* Added expressions.
* This helps alot writing scripts, where math expressions is available.
* Most standard functions: ceil, floor, sin, cos, etc is available.
* Also pi (3.1415...) and e ( 2.718 ) as constants.
* Scope is also implemented. (a variable name is backtraced upwards from the current
* scope.
*
* Revision 1.1  2002/04/09 09:04:48  andersb
* Major rewrite of ConfigIO.cxx.
* Now supports multiline strings and arrays.
*
* Revision 1.4  2002/02/01 13:35:20  andersb
* Now fully updated to support recursive structs.
* Still problems with case sensitivity.
*
* Revision 1.1.1.1  2002/01/02 13:45:30  andersb
* A new version of Configscript using namespace rcfg.
* Also supporting recursive structures.
*
* Revision 1.3  2001/08/29 09:23:35  andersb
* Now using the old iostream.h library to get it to work together with
* CommonC++.
*
* Revision 1.2  2001/08/01 07:11:20  andersb
* Major update
*
* Revision 1.1.1.1  2001/04/25 10:56:27  andersb
* no message
*
* Revision 1.1.1.1  2001/04/23 09:07:25  andersb
* no message
*
* Revision 1.5  2000/11/03 14:48:01  andersb
* no message
*
* Revision 1.4  2000/10/09 13:02:45  andersb
* no message
*
* Revision 1.3  2000/10/03 09:21:24  andersb
* no message
*
* Revision 1.2  2000/10/03 07:44:37  andersb
* Using DOC++
*
* Revision 1.1  2000/10/03 07:02:41  andersb
* no message
*
* Revision 1.6  2000/10/02 14:04:15  andersb
* no message
*
* Revision 1.5  2000/10/02 10:20:20  andersb
* no message
*
* Revision 1.4  2000/10/02 09:31:13  andersb
* A Working SGI version of the ConfigIO files.
*
* Revision 1.3  2000/10/02 09:11:48  andersb
* no message  
*

--------------------------------------------*/
