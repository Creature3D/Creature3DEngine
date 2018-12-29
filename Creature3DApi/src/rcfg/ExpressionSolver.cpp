// $Id: ExpressionSolver.cpp,v 1.1 2005/02/09 17:26:57 deepone Exp $

#ifdef _WIN32
#pragma warning(disable:4786)
#endif


#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <string.h>
#include <sstream>
#include <time.h>
#include "rcfg/ExpressionSolver.h"
#include "rcfg/utils.h"

#ifndef M_PI
#define M_PI    3.14159265358979323846
#endif
#ifndef M_E
#define M_E     2.71828182845904523536
#endif

// Space/tab sign?
#define iswhite(c)  (c == ' ' || c == '\t')
// Numeric (includes .)
#define isnumer(c)  ((c >= '0' && c <= '9') || c == '.')

// Alphanumeric includes _
#define isalphanumeric(c)  ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||(c >= '0' && c <= '9') \
                    || c == '_' || c=='.')

// is operator/delimiter?
#define isdelim(c)  (c == '+' || c == '-' || c == '*' || c == '/' || c == '%' \
                    || c == '^' || c == '(' || c == ')' || c == ',' || c == '=')


/*************************************************************************
**                                                                       **
** PROTOTYPES FOR CUSTOM MATH FUNCTIONS                                  **
**                                                                       **
 *************************************************************************/

/*************************************************************************
**                                                                       **
** Some custom math functions...   Note that they must be prototyped     **
** above (if your compiler requires it)                                  **
**                                                                       **
** deg( x )             Converts x radians to degrees.                   **
** rad( x )             Converts x degrees to radians.                   **
**                                                                       **
 *************************************************************************/
#define DEG(X) (57.295779513082320876798154814105*(X))
#define RAD(X) (0.017453292519943295769236907684886*(X))

double deg( double x );
double rad( double x );

double
deg( double x )
{
   return DEG(x);
}

double
rad( double x )
{
   return RAD(x);
}



// ==============================================
// Constructor, builds up function table and predefined constant table
ExpressionSolver::ExpressionSolver() : 
   m_error_pos(0), m_expression(0), m_exp_start(0),  m_buffer(0),
   m_buffer_size(0), m_type(NOTYPE), m_enable_assignments(true)
{

  // Predefined constants
  m_constants.insert(VariableType("pi", M_PI));
  m_constants.insert(VariableType("e", M_E));
 
  // Function definitions
  m_functions.insert(FunctionType("sin", Function(1, &sin)));
  m_functions.insert(FunctionType("cos", Function (1, &cos)));
  m_functions.insert(FunctionType("tan", Function (1, &tan)));
  m_functions.insert(FunctionType("asin", Function (1, &asin)));
  m_functions.insert(FunctionType("acos", Function (1, &acos)));
  m_functions.insert(FunctionType("atan", Function (1, &atan)));
  m_functions.insert(FunctionType("sinh", Function (1, &sinh)));
  m_functions.insert(FunctionType("cosh", Function (1, &cosh)));
  m_functions.insert(FunctionType("tanh", Function (1, &tanh)));
  m_functions.insert(FunctionType("exp", Function (1, &exp)));
  m_functions.insert(FunctionType("log", Function (1, &log)));
  m_functions.insert(FunctionType("log10", Function (1, &log10)));
  m_functions.insert(FunctionType("sqrt", Function (1, &sqrt)));
  m_functions.insert(FunctionType("floor", Function (1, &floor)));
  m_functions.insert(FunctionType("ceil", Function (1, &ceil)));
  m_functions.insert(FunctionType("fabs", Function (1, &fabs)));
  m_functions.insert(FunctionType("hypot", Function (2, &hypot)));
  m_functions.insert(FunctionType("deg", Function (1, &deg)));
  m_functions.insert(FunctionType("rad", Function (1, &rad)));
  m_functions.insert(FunctionType("randInterval", Function (2, &RandInterval)));


  // Error messages
  m_err_messages.push_back("URK");
  m_err_messages.push_back("Syntax error");
  m_err_messages.push_back("Unbalanced parenthesis");
  m_err_messages.push_back("Division by zero");
  m_err_messages.push_back("Unknown variable");
  m_err_messages.push_back("Maximum variables exceeded");
  m_err_messages.push_back("Unrecognised function");
  m_err_messages.push_back("Wrong number of arguments to function");
  m_err_messages.push_back("Missing an argument");
  m_err_messages.push_back("Empty expression");
}

void ExpressionSolver::allocateMemory(std::size_t size)
{
  if (m_buffer_size >= size)
    return;
  else {
    delete [] m_buffer;
    m_buffer = new char[size];
    m_buffer_size = size;
  }
}

// ==============================================
// Destructor, deallocates any previously allocated memory
ExpressionSolver::~ExpressionSolver() 
{
  if (m_buffer)
    delete [] m_buffer;
}

// ==============================================
// Converts s to all lowercase
void strlwr( std::string& s )
{
  for(unsigned int i=0; i < s.length(); i++)
    s[i] = tolower(s[i]);
}

// ==============================================
// 
void ExpressionSolver::clearAllVars()
{
  m_variables.clear();   
}


// ==============================================
// Removes name from variable symboltable
void ExpressionSolver::clearVar( const std::string& name )
{
   m_variables.erase(name);   
}


/* =================================================================
**                                                                       
**                                                                       
** Looks up the specified variable (or constant) known as name and       
** returns its contents.                                                 
**                                                                       
** First the user-defined variables are searched, then the constants are 
** searched.                                                             
**                                                                       
** Throws an error if variable name is not found
**                                                                       
** ================================================================= */

ExpressionSolver::ExpressionType ExpressionSolver::getValue( const std::string& name )
{
  ExpressionType value; 
  if (!getVariableValue(name, value))
    if (!getConstantValue(name, value))
      throw E_UNKNOWN;
  
  return value;
}

bool ExpressionSolver::getVariableValue( const std::string& name, ExpressionType& value)
{
  /* Check the user-defined variables. */
  VariableMapIterator vmi = m_variables.find(name);
  if (vmi != m_variables.end()) {
    value = vmi->second;
    return true;
  }
  return false;
}

bool ExpressionSolver::getConstantValue( const std::string& name, ExpressionType& value)
{
  /* Check the programmer-defined constants. */
  VariableMapIterator vmi = m_constants.find(name);
  if (vmi != m_constants.end()) {
    value = vmi->second;
    return true;
  }
  return false;
}

  /*************************************************************************
**                                                                       **
** SetValue( char* name, ExpressionType* value )                                   **
**                                                                       **
** First, it erases any user-defined variable that is called NAME.  Then **
** it creates a new variable called NAME and gives it the value VALUE.   **
**                                                                       **
** Returns 1 if the value was added, or 0 if there was no more room.     **
**                                                                       **
 *************************************************************************/

void ExpressionSolver::setValue( const std::string& name, const ExpressionType& value )
{
   clearVar( name );
   m_variables.insert(VariableType(name, value));
}


/*************************************************************************
**                                                                       **
** Parse()   Internal use only                                           **
**                                                                       **
** This function is used to grab the next token from the expression that **
** is being evaluated.                                                   **
**                                                                       **
 *************************************************************************/

void
ExpressionSolver::parse()
{
   char* t;

   m_type = NOTYPE;
   t = m_token;
   while( iswhite( *m_expression ) )
      m_expression++;
   if( isdelim( *m_expression ) )
   {
      m_type = DELIMITER;
      *t++ = *m_expression++;
   }
   else if( isnumer( *m_expression ) )
   {
      m_type = NUMBER;
      while( isnumer( *m_expression ) )
         *t++ = *m_expression++;
   }
   else if( isalphanumeric( *m_expression ) )
   {
      m_type = VARIABLE;
      while( isalphanumeric( *m_expression ) )
        *t++ = *m_expression++;
      m_token[VARLEN] = 0;
   }
   else if( *m_expression )
   {
      *t++ = *m_expression++;
      *t = 0;
      throw( E_SYNTAX );
   }
   *t = 0;
   while( iswhite( *m_expression ) )
      m_expression++;
}


/*************************************************************************
**                                                                       **
** Level1( ExpressionType* r )   Internal use only                                 **
**                                                                       **
** This function handles any variable assignment operations.             **
** It returns a value of 1 if it is a top-level assignment operation,    **
** otherwise it returns 0                                                **
**                                                                       **
 *************************************************************************/


bool ExpressionSolver::level1( ExpressionType& r )
{
  char t[VARLEN + 1];

  if (m_enable_assignments) {
    if( m_type == VARIABLE )
      if( *m_expression == '=' ) {
        strcpy( t, m_token );
        parse();
        parse();
        if( !*m_token ) {
          clearVar( t );
          return true;
        }
        level2( r );
        setValue( t, r );
        return true;
      }
  }
  level2( r );
  return false;
}


/*************************************************************************
**                                                                       **
** Level2( ExpressionType* r )   Internal use only                                 **
**                                                                       **
** This function handles any addition and subtraction operations.        **
**                                                                       **
 *************************************************************************/

void 
ExpressionSolver::level2( ExpressionType& r )
{
  ExpressionType t = 0;
  char o;

  level3( r );
  while( (o = *m_token) == '+' || o == '-' ) {
    parse();
    level3( t );
    if( o == '+' )
       r = r + t;
    else if( o == '-' )
       r = r - t;
  }
}


/*************************************************************************
**                                                                       **
** Level3( ExpressionType* r )   Internal use only                                 **
**                                                                       **
** This function handles any multiplication, division, or modulo.        **
**                                                                       **
 *************************************************************************/

void
ExpressionSolver::level3( ExpressionType& r )
{
  ExpressionType t;
  char o;

  level4( r );
  while( (o = *m_token) == '*' || o == '/' || o == '%' ) {
    parse();
    level4( t );
    if( o == '*' )
       r = r * t;
    else if( o == '/' ){
      if( t == 0 )
        throw( E_DIVZERO );
      r = r / t;
    }
    else if( o == '%' ) {
      if( t == 0 )
        throw( E_DIVZERO );
      r = fmod( r, t );
    }
  }
}


/*************************************************************************
**                                                                       **
** Level4( ExpressionType* r )   Internal use only                                 **
**                                                                       **
** This function handles any "to the power of" operations.               **
**                                                                       **
 *************************************************************************/

void 
ExpressionSolver::level4( ExpressionType& r )
{
  ExpressionType t;

  level5( r );
  if( *m_token == '^' )  {
    parse();
    level5( t );
    r = pow( r, t );
  }
}


/*************************************************************************
**                                                                       **
** Level5( ExpressionType* r )   Internal use only                                 **
**                                                                       **
** This function handles any unary + or - signs.                         **
**                                                                       **
 *************************************************************************/

void ExpressionSolver::level5( ExpressionType& r )
{
  char o = 0;

  if( *m_token == '+' || *m_token == '-' ){
    o = *m_token;
    parse();
  }
  level6( r );
  if( o == '-' )
    r = -r;
}


/*************************************************************************
**                                                                       **
** Level6( ExpressionType* r )   Internal use only                                 **
**                                                                       **
** This function handles any literal numbers, variables, or functions.   **
**                                                                       **
 *************************************************************************/

void
ExpressionSolver::level6( ExpressionType& r )
{
  int  n;
  ExpressionType a[4];

  if( *m_token == '(' ) {
    parse();
    if( *m_token == ')' )
      throw( E_NOARG );

    level1( r );
    if( *m_token != ')' )
       throw( E_UNBALAN );
    parse();
  }
  else {
    if( m_type == NUMBER ) {
      r = (ExpressionType) atof( m_token );
      parse();
    }
    else if( m_type == VARIABLE ) {
      if( *m_expression == '(' ) {
        FunctionMapIterator fmi;
        fmi = m_functions.find(m_token);
        if (fmi != m_functions.end()) { 
          parse();
          n = 0;
          do {
            parse();
            if( *m_token == ')' || *m_token == ',' )
              throw( E_NOARG );
            a[n] = 0;
            level1( a[n] );
            n++;
          } while( n <= 4 && *m_token == ',' );
          parse();
          if( n != (fmi->second).args ){
            strcpy( m_token, fmi->first.c_str() );
            throw( E_NUMARGS );
          }
          switch(n) {
            case 1:
              r = ((FuncPtr1)(fmi->second).func)( a[0]);
              break;
            case 2:
              r = ((FuncPtr2)(fmi->second).func)( a[0], a[1]); 
              break;
            case 3:
              r = ((FuncPtr3)(fmi->second).func)( a[0], a[1], a[2] );
              break;
            case 4:
              r = ((FuncPtr4)(fmi->second).func)( a[0], a[1], a[2], a[3] );
              break;
          } // switch
          return;
        } // if found function
        else 
          // We did not found a matching function
          throw ( E_BADFUNC );
      } // current letter is a (
      else {
        r = getValue( m_token) ;
      }
      parse();
    } // m_type == VARIABLE
    else
      throw( E_SYNTAX );
  }
}


/*************************************************************************
**                                                                       **
** Evaluate( char* e, ExpressionType* result, int* a )                             **
**                                                                       **
** This function is called to evaluate the expression E and return the   **
** answer in RESULT.  If the expression was a top-level assignment, a    **
** value of 1 will be returned in A, otherwise it will contain 0.        **
**                                                                       **
** Returns E_OK if the expression is valid, or an error code.            **
**                                                                       **
 *************************************************************************/

int ExpressionSolver::evaluate( const std::string& e, ExpressionType& result, bool *a )
{
  m_exp_start=0;
  try {

    bool assignment;
    std::string exp = e;
    //strlwr( exp );

    allocateMemory(e.length()+1);

    m_expression = m_buffer;
    strcpy(m_expression, exp.c_str());
    m_expression[exp.length()] = '\0';
    
    m_exp_start = m_expression;
    parse();
    if( ! *m_token )
      throw( E_EMPTY );
    assignment = level1( result );
    if (a)
      *a = assignment;
  }
  catch(ReturnCode err) {
    std::ostringstream str;
    m_error_pos = (unsigned int)(m_expression-m_exp_start-1);
    str << "ERROR: " << m_err_messages[err] << " - " << m_token << "\n";
    str << m_exp_start << "\n";
    str.width(m_error_pos+1);
    str << "^" ;
    m_last_error = str.str();
    return err;
  }
  return( E_OK );
}


/*************************************************************************
**                                                                       **
** What follows is a main() routine that evaluates the command line      **
** arguments one at a time, and displays the results of each expression. **
** Without arguments, it becomes an interactive calculator.              **
**                                                                       **
 *************************************************************************/

#if 0

main( int argc, char* argv[] )
{
ExpressionSolver::ExpressionType  result;
   int   i;
   int   ec;
   bool   a;
   char  line[1024];
   ExpressionSolver exp;
   exp.enableAssignments(false);

   /* If we have command line arguments then we evaluate them and exit. */
   if( argc > 1 )
   {
      /* Concatenate all arguments into one string. */
      strcpy( line, argv[1] );
      for( i = 2; i < argc; i++ )
         strcat( line, argv[i] );

      /* Call the evaluator. */
      if( (ec = exp.evaluate( line, result, &a )) == ExpressionSolver::E_OK )
      {
         /* If we didn't assign a variable, then print the result. */
         if( ! a )
            printf( "%g\n", result );
      }
      else if( ec != ExpressionSolver::E_EMPTY )
      {
         /* Display error info.  In this example, an E_EMPTY error is ignored. */
        std::cerr << exp.getLastError() << std::endl; 
        /*printf( "ERROR: %s - %s", exp.getLastError(), exp.getErrToken() );
         printf( "\n%s", exp.getErrStr() );
         printf( "\n%*s^\n", exp.getErrPos(), "" );*/
      }
      return;
   }

   /* There were no command line arguments, so lets go interactive. */
   printf( "\nEE - Equation ExpressionSolver" );
   printf( "\nEnter EXIT to quit.\n" );
   printf( "\nEE> " );

   for( gets( line ); !feof( stdin ); gets( line ) )
   {
      strlwr( line );

      /* Did the user ask to exit? */
      if( ! strcmp( line, "exit" ) )
         return;

      /* Did the user ask to see the variables in memory? */
      else if( ! strcmp( line, "vars" ) )
      {
/*         for( i = 0; i < MAXVARS; i++ )
            if( *Vars[i].name )
               printf( "%s = %g\n", Vars[i].name, Vars[i].value );*/
      }

      /* Did the user ask to see the constants in memory? */
     /* else if( ! strcmp( line, "cons" ) )
      {
         for( i = 0; *Consts[i].name; i++ )
            printf( "%s = %g\n", Consts[i].name, Consts[i].value );
      }*/

      /* Did the user ask to clear all variables? */
      else if( ! strcmp( line, "clr" ) )
         exp.clearAllVars();

      /* If none of the above, then we attempt to evaluate the user's input. */
      else
      {
         /* Call the evaluator. */
      if( (ec = exp.evaluate( line, result, &a )) == ExpressionSolver::E_OK )
         {
            /* Only display the result if it was not an assignment. */
            if( ! a )
               printf( "%g\n", result );
         }
      else if( ec != ExpressionSolver::E_EMPTY )
         {
            /* Display error information.  E_EMPTY is ignored. */
            std::cerr << exp.getLastError() << std::endl; 
            /*printf( "ERROR: %s - %s", exp.getLastError(), exp.getErrToken() );
            printf( "\n%s", exp.getErrStr() );
            printf( "\n%*s^\n", exp.getErrPos(), "" );*/
         }
      }
      printf( "EE> " );
   }
}

#endif

/*------------------------------------------

* $Source: /cvsroot/replicantbody/replicantbody/src/rcfg/ExpressionSolver.cpp,v $
* $Revision: 1.1 $ 
* $Date: 2005/02/09 17:26:57 $
* $Author: deepone $ 
* $Locker:  $
* Description: Implementation of an mathematical expression evaluator.
  Anders Backman, 020705, VRlab, Ume?University

* $Log: ExpressionSolver.cpp,v $
* Revision 1.1  2005/02/09 17:26:57  deepone
* vrutils and cfgscript put into new namespaces
*
* Revision 1.1.1.1  2005/02/06 06:52:59  deepone
* initial sourceforge checkin
*
* Revision 1.12  2003/12/16 11:34:45  andersb
* *** empty log message ***
*
* Revision 1.11  2003/12/05 10:43:37  deepone
* *** empty log message ***
*
* Revision 1.10  2003/03/04 11:19:53  Anders Backman
* Fixed bug that required data beginning on the same line as the key,
* Added a search path feature. Default the environment variable CFG_FILE_PATH is enquired
* for a list of directories to search config files in.
*
* Revision 1.9  2003/03/03 12:16:51  Anders Backman
* removed more std::ends
*
* Revision 1.8  2003/02/27 14:02:58  andersb
* Works under gcc3.2
*
* Revision 1.7  2002/12/17 16:55:11  Anders Backman
* no message
*
* Revision 1.6  2002/11/18 13:19:49  Anders Backman
* no message
*
* Revision 1.5  2002/10/22 12:30:57  Anders Backman
* Moved around include files.
*
* Revision 1.4  2002/08/06 14:24:26  andersb
* Ahh it works again, Daniels fault (remember alphanumeric?)
*
* Revision 1.3  2002/08/06 09:55:20  deepone
* configure stuff
*
* Revision 1.2  2002/07/09 21:28:57  andersb
* Fixed case bug in Expressions allowing only lowercase variables.
* Added expressions in arrays too.
* Added randInterval(min,max) function in Expressions allowing expressions like:
*
* Position [randInterval(-10,10) randInterval(-10,10) randInterval(0,3)]
*
* Revision 1.1  2002/07/04 23:08:28  andersb
* Added expressions.
* This helps alot writing scripts, where math expressions is available.
* Most standard functions: ceil, floor, sin, cos, etc is available.
* Also pi (3.1415...) and e ( 2.718 ) as constants.
* Scope is also implemented. (a variable name is backtraced upwards from the current
* scope.
*
--------------------------------------------*/
