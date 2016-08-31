// $Id: Preprocessor.h,v 1.1 2005/02/09 17:26:55 deepone Exp $

#ifndef __Preprocessor_h__
#define __Preprocessor_h__

#ifdef _WIN32
#pragma warning(disable:4786)
#include "rvrutils/os/memory_debug.h"


#endif

#include <string>
#include <map>
#include <vector>
#include <stack>

#include "rcfg/export.h"


/// Class for handling preprocessor directives in a config script
/*!
*/
class CONFIGSCRIPT_API  Preprocessor
{
public:
  /// Constructor
  Preprocessor();
  
  // Destructor
  ~Preprocessor(){};
  
  /// Processes a line
  void processLine(std::string& line);

  /// returns true if there is currently a balance between #ifdef and #endif
  bool checkMatch() const ;

  /*! Adds the macro with its associated value to the existing map of macros
      Replaces any existing occurances of macro
  */
  void addMacro(const std::string& macro, const std::string& value);

  /*! \return the found include path during the last call to processLine.
      returns "" if none were found the last time processLine were called.
 */
  std::string getIncludePath() const  { return _include_path; }

private:
  
  /// Replaces any matching macros with its associated values
  void replacePhase(std::string& line);
  
  /*! Extract any macro definitions from line and put them into map of macros.
      If two macros with same name is added, it is always the last that is stored.
  */
  void extractMacroPhase(std::string& line);

  void undefMacroPhase(std::string& line);
  /*!
    Checks for existance of INCLUDE. If found it extracts the path and stores it in _include_path
    This can later be checked with getIncludePath. _include_path is "":ed during each call to processLine
  */
  void includePhase(std::string& line);

  /// Performs a logical ifdef else endif check to remove lines that are in a "false" area
  void logicIfPhase(std::string& line);
  
  /// Specifies the allowed preprocessor keys
  enum PreprocessorDirectives { UNDEF, DEFINE, INCLUDE, PRAGMA, IFDEF, ELSE, ENDIF, ELSEIF, ALWAYS_LAST };

  /// returns true if Preprocessor directive dir exists in line
  bool preprocessorDirExist(PreprocessorDirectives dir, std::string& line) const ;

  std::vector<std::string> _preprocessor_directives;
  const unsigned int MAX_LOOPS;

  typedef std::map<std::string, std::string> MacroMap;
  typedef MacroMap::const_iterator MacroMapIterator;
  typedef MacroMap::value_type MacroMapType;

  MacroMap _macro_map;

  std::string _include_path;

  typedef std::stack< std::pair< bool, unsigned short > > IfStateStack_t;
  typedef IfStateStack_t::value_type IfState;

  
  /// Stores the nested if:s according to its nested level and its state (true or not)
  class IfStateStack 
  {
  public: 
    /// Pushes a state to the stack
    void push(IfState s) { m_stack.push(s); }
    /// Pops a state from the stack
    void pop( void ) { m_stack.pop(); }
    
    /// Returns the last state
    IfState top( void ) const { if (size()) return m_stack.top(); else return IfState(true, 0); }

    /// Inverts the state of the last state
    void invert() { if (size()) {IfState s= m_stack.top(); pop(); push(IfState(!s.first, s.second)); } }
    
    /// Returns the size of the stack
    size_t size() const { return m_stack.size(); }
  private:
    IfStateStack_t m_stack;
  };

  IfStateStack _if_stack;

  int _line_no;
  int _if_start;

};
#endif

/*------------------------------------------

* $Source: /cvsroot/replicantbody/replicantbody/include/rcfg/Preprocessor.h,v $
* $Revision: 1.1 $ 
* $Date: 2005/02/09 17:26:55 $
* $Author: deepone $ 
* $Locker:  $
  Description: Header file for a C stylish preprocessor

* $Log: Preprocessor.h,v $
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
* Revision 1.5  2003/11/14 09:22:46  andersb
* *** empty log message ***
*
* Revision 1.4  2003/02/27 13:27:53  andersb
* Moved over to sstream instead of strstream
*
* Revision 1.3  2003/02/27 13:08:56  andersb
* *** empty log message ***
*
* Revision 1.2  2002/11/18 13:19:48  Anders Backman
* no message
*
* Revision 1.1  2002/10/22 12:30:56  Anders Backman
* Moved around include files.
*
* Revision 1.3  2002/09/27 08:01:06  andersb
* Added #undef
* Still bug under linux regarding #include non existing files
*
* Revision 1.2  2002/08/03 09:49:12  andersb
* Now also #include works but not finished and polished.
*
* Revision 1.1  2002/07/21 21:55:15  andersb
* Added preprocessor directives:
* #define <macro> <value>
* #ifdef <macroname>
* #else
* #endif
* Also removed MSv6.0 project files.
* Preprocessor.h and Preprocessor.cpp is added.
*

------------------------------------------*/
