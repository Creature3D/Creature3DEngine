// $Id: ConfigValue.cpp,v 1.1 2005/02/09 17:26:57 deepone Exp $

#include "rcfg/ConfigValue.h"
#include "rcfg/Item.h"
#include <sstream>



bool ConfigValue::GetLiteralValue(float& value)
{
  bool b;
  
  switch(Type()) {
    case ConfigValue::VALUE_FLOAT:
    {
      b = ((FloatItem *)this)->GetVal(value);
      break;
    }

    case ConfigValue::VALUE_INT:
    {
      int i_val;
      b = ((IntItem *)this)->GetVal(i_val);
      value = (float)i_val;
      break;
    }

    case ConfigValue::VALUE_EXPRESSION:
    {
      Expression e;
      b = ((ExpressionItem *)this)->GetVal(e);
      value = (float)e.GetVal();
      break;
    }
    default:
      b = false;

  }
  return b;

}

bool ConfigValue::GetStringValue(std::string& value)
{
  bool b;
  std::ostringstream stream;

  switch(Type()) {
    case ConfigValue::VALUE_FLOAT:
    {
      float f;
      b = ((FloatItem *)this)->GetVal(f);
      stream << f;
      value = stream.str();
      break;
    }

    case ConfigValue::VALUE_INT:
    {
      int i_val;
      b = ((IntItem *)this)->GetVal(i_val);
      stream << i_val;
      value = stream.str();
      break;
    }
    case ConfigValue::VALUE_STRING:
    {
      b = ((StringItem *)this)->GetVal(value);
      break;
    }

    case ConfigValue::VALUE_EXPRESSION:
    {
      Expression e;
      b = ((ExpressionItem *)this)->GetVal(e);
      stream << e.GetVal();
      value = stream.str();
      break;
    }
    default:
      b = false;

  }
  return b;

}
/*------------------------------------------

* $Source: /cvsroot/replicantbody/replicantbody/src/rcfg/ConfigValue.cpp,v $
* $Revision: 1.1 $ 
* $Date: 2005/02/09 17:26:57 $
* $Author: deepone $ 
* $Locker:  $

* $Log: ConfigValue.cpp,v $
* Revision 1.1  2005/02/09 17:26:57  deepone
* vrutils and cfgscript put into new namespaces
*
* Revision 1.1.1.1  2005/02/06 06:52:59  deepone
* initial sourceforge checkin
*
* Revision 1.5  2003/12/16 11:34:45  andersb
* *** empty log message ***
*
* Revision 1.4  2003/12/05 10:43:37  deepone
* *** empty log message ***
*
* Revision 1.3  2003/04/24 06:23:05  andersb
* Added support for Array of Strings
*
* Revision 1.2  2002/10/22 12:30:57  Anders Backman
* Moved around include files.
*
* Revision 1.1  2002/07/09 21:28:57  andersb
* Fixed case bug in Expressions allowing only lowercase variables.
* Added expressions in arrays too.
* Added randInterval(min,max) function in Expressions allowing expressions like:
*
* Position [randInterval(-10,10) randInterval(-10,10) randInterval(0,3)]
*

--------------------------------------------*/
