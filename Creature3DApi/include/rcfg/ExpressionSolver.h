// $Id: ExpressionSolver.h,v 1.1 2005/02/09 17:26:55 deepone Exp $

#ifndef __ExpressionSolver_h__
#define __ExpressionSolver_h__

#ifdef _WIN32
#pragma warning(disable:4786)
#include "rvrutils/os/memory_debug.h"

#endif

#include <map>
#include <string>
#include <vector>
#include "rcfg/export.h"



/// Solves mathematical expressions
class CONFIGSCRIPT_API  ExpressionSolver {

public:

  /// The resulting type of the expression.
  typedef double ExpressionType; /* Type of numbers to work with */

/// Codes returned from the evaluator 
  enum ReturnCode  {E_OK,      /// Successful evaluation 
                    E_SYNTAX,  /// Syntax error 
                    E_UNBALAN, /// Unbalanced parenthesis 
                    E_DIVZERO, /// Attempted division by zero 
                    E_UNKNOWN, /// Reference to unknown variable 
                    E_MAXVARS, ///Maximum variables exceeded 
                    E_BADFUNC, /// Unrecognised function 
                    E_NUMARGS, /// Wrong number of arguments to funtion 
                    E_NOARG,   /// Missing an argument to a funtion 
                    E_EMPTY    /// Empty expression 
    };

  /// Constructor
  ExpressionSolver();
  
  // Destructor
  virtual ~ExpressionSolver();

  /*!
    Evaluates the expression given in e. The result is returned in result.
    If the expression was an assignment the *a is set to true (unless the pointer is null)
    \return E_OK (0) if the calculation was successful, otherwise an error code.
  */
  int evaluate( const std::string& e, ExpressionType& result, bool *a=0 );

  /// Returns the error message if the evaluate method failed.
  std::string getLastError() { return m_last_error; }
    
  /*! Specifies if the class should allow assignments or not.
    if assignments is not allowed the setVariable will never be called.
  */
  void enableAssignments(bool flag) { m_enable_assignments = flag; }

  /// Removes all variables from symboltable
  void clearAllVars();

private:
   /// removes the variable named name from the symbol table
   void clearVar( const std::string& name );

   /*! Get the value of a named variable
   \param name - The variable to look up.
   \param value - The value will be put here
   \return true if variable is found otherwise false
   */
   virtual bool getVariableValue( const std::string& name, ExpressionType& value);

   /*! Get the value of a named constant
   \param name - The constant to look up.
   \param value - The value will be put here
   \return true if constant is found otherwise false
   */
   virtual bool getConstantValue( const std::string& name, ExpressionType& value);

   /// Get the value of a named variable/constant
   ExpressionType getValue( const std::string& name );

   /// Store the value in a variable named name
   void setValue( const std::string& name, const ExpressionType& value );
  
   /// Superlevel parser
   void parse();
   
   /// Parses 1st level assignments a=expr
   bool level1( ExpressionType& r );
   
   /// Handles addition and subtractions (a+b, a-b)
   void level2( ExpressionType& r );
   
   // Multiplication, division and modulo (a*b, a/b, a%b)
   void level3( ExpressionType& r );

   /// Handles power of (a^b)
   void level4( ExpressionType& r );
   
   /// Handles unary - and + signs: (-a, +a)
   void level5( ExpressionType& r );
   
   /// Handles literals, variables and function calls
   void level6( ExpressionType& r );

   void allocateMemory(std::size_t size);

#ifdef _WIN32
#define __CDECL         __cdecl
#else
#define __CDECL
#endif

  typedef ExpressionType (__CDECL *FuncPtr1)(ExpressionType);
  typedef ExpressionType (__CDECL *FuncPtr2)(ExpressionType, ExpressionType);
  typedef ExpressionType (__CDECL *FuncPtr3)(ExpressionType, ExpressionType, ExpressionType);
  typedef ExpressionType (__CDECL *FuncPtr4)(ExpressionType, ExpressionType, ExpressionType, ExpressionType);

  /// Helper class for storing pointers to predefined functions such as sin, cos, etc...
  class Function
  {
    public:
      Function(int a, FuncPtr1 fp) : args(a),func((void *)fp) {}
      Function(int a, FuncPtr2 fp) : args(a),func((void *)fp) {}
      Function(int a, FuncPtr3 fp) : args(a),func((void *)fp) {}
      Function(int a, FuncPtr4 fp) : args(a),func((void *)fp) {}
      int   args;                          /* Number of arguments to expect */
      void *func;                        /* Pointer to function */
  };

private:
  std::string m_last_error;
  unsigned int m_error_pos;

  typedef std::map<std::string, ExpressionType> VariableMap;
  typedef VariableMap::value_type VariableType;
  typedef VariableMap::iterator VariableMapIterator;
  VariableMap m_constants;

  VariableMap m_variables;

  typedef std::map<std::string, Function> FunctionMap;
  typedef FunctionMap::iterator FunctionMapIterator;
  typedef FunctionMap::value_type FunctionType;
  FunctionMap m_functions;

   char *m_expression;
   char *m_exp_start;
   char *m_buffer;
   unsigned int m_buffer_size;
  /// Ouch, only static size in this class, the maximum size if a variable name
#define VARLEN 40
   char m_token[VARLEN+1];

   /// Specifies the type of a character
   enum CharacterType { 
     NOTYPE, /// No type specified yet
     VARIABLE, /// A variable
     DELIMITER,  /// A operator/delimiter
     NUMBER /// A number
   };
   CharacterType m_type;

  std::vector<std::string> m_err_messages;

  bool m_enable_assignments;
};

#endif

/*------------------------------------------

* $Source: /cvsroot/replicantbody/replicantbody/include/rcfg/ExpressionSolver.h,v $
* $Revision: 1.1 $ 
* $Date: 2005/02/09 17:26:55 $
* $Author: deepone $ 
* $Locker:  $
* Description: Implementation of an mathematical expression evaluator.
  Anders Backman, 020705, VRlab, Ume?University

* $Log: ExpressionSolver.h,v $
* Revision 1.1  2005/02/09 17:26:55  deepone
* vrutils and cfgscript put into new namespaces
*
* Revision 1.1.1.1  2005/02/06 06:53:00  deepone
* initial sourceforge checkin
*
* Revision 1.7  2003/12/17 09:28:50  andersb
* Made configscript a dll library under windows
* Fixed memory leaks.
* Fixed bug in processing included files in wrong order.
*
* Revision 1.6  2003/12/16 11:34:44  andersb
* *** empty log message ***
*
* Revision 1.5  2003/12/05 10:43:37  deepone
* *** empty log message ***
*
* Revision 1.4  2003/02/27 13:27:53  andersb
* Moved over to sstream instead of strstream
*
* Revision 1.3  2002/12/20 14:19:31  axelsson
* Minor fix of compability problem with gcc 3
*
* Revision 1.2  2002/11/18 13:19:48  Anders Backman
* no message
*
* Revision 1.1  2002/10/22 12:30:55  Anders Backman
* Moved around include files.
*
* Revision 1.4  2002/08/06 14:24:01  andersb
* Ahh it works again, Daniels fault (remember alphanumeric?)
*
* Revision 1.3  2002/08/06 09:55:20  deepone
* configure stuff
*
* Revision 1.2  2002/07/09 21:28:46  andersb
* Fixed case bug in Expressions allowing only lowercase variables.
* Added expressions in arrays too.
* Added randInterval(min,max) function in Expressions allowing expressions like:
*
* Position [randInterval(-10,10) randInterval(-10,10) randInterval(0,3)]
*
* Revision 1.1  2002/07/04 23:08:47  andersb
* Added expressions.
* This helps alot writing scripts, where math expressions is available.
* Most standard functions: ceil, floor, sin, cos, etc is available.
* Also pi (3.1415...) and e ( 2.718 ) as constants.
* Scope is also implemented. (a variable name is backtraced upwards from the current
* scope.
*
--------------------------------------------*/
