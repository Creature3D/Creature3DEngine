// $Id: Preprocessor.cpp,v 1.1 2005/02/09 17:26:57 deepone Exp $

#include "rcfg/Preprocessor.h"

#include "rcfg/utils.h"
#include <sstream>
#include <stdexcept>




Preprocessor::Preprocessor() : MAX_LOOPS(1000), _line_no(0)
{
  _preprocessor_directives.resize(ALWAYS_LAST);

  _preprocessor_directives[DEFINE]= "#define ";
  _preprocessor_directives[UNDEF]= "#undef ";
  _preprocessor_directives[INCLUDE]= "#include ";
  _preprocessor_directives[PRAGMA]= "#pragma ";
  _preprocessor_directives[IFDEF]= "#ifdef ";
  _preprocessor_directives[ELSE]= "#else";
  _preprocessor_directives[ELSEIF]= "#elseif";
  _preprocessor_directives[ENDIF]= "#endif";

}

void Preprocessor::processLine(std::string& line)
{

  _include_path = ""; // No include path found so far
  _line_no++;

  if (!line.length())
    return;

  // Check for #ifdef #else #endif
  logicIfPhase(line);
  if (!line.length())
    return;

  // If the stack of if sets is not empty, 
  IfState curr_state = _if_stack.top();
  if (!curr_state.first) { // check if we should use this line or not
    line = ""; // no this line is between a false statement and should be discarded
  }


  // Are there any undef macros?
  undefMacroPhase(line);
  if (!line.length())
    return;

  // Check if any macros should be used for replacement
  replacePhase(line);
  if (!line.length())
    return;

  line = Trim(line);

  // Are there any #define here?
  extractMacroPhase(line);
  if (!line.length())
    return;

  includePhase(line);
  if (!line.length())
    return;
}

void Preprocessor::includePhase(std::string& line)
{
  // Does INCLUDE exist?
  bool exist = preprocessorDirExist(INCLUDE, line);
  if (exist) {
    std::string filepath = GetWord(line);

    // There shouldnt be anything after a #include <path>
    if (line.length()) {
      std::ostringstream msg;
      msg << "Extra item following " << _preprocessor_directives[INCLUDE] << ": " << line;
      throw std::logic_error(msg.str());
    }

    if (!filepath.length()) {
      std::ostringstream msg;
      msg << "Zero length path for " << _preprocessor_directives[INCLUDE] << " is not allowed." ;
      throw std::logic_error(msg.str());
    }
    _include_path = filepath;
    line  = "";
  }
}

// check for existance of a given preprocessordirective
bool Preprocessor::preprocessorDirExist(PreprocessorDirectives dir, std::string& line) const
{
  bool exist=false;

  std::string key = _preprocessor_directives[dir];
  // Test for existance of a macro?
  std::string tmp = line.substr(0, key.length());
  if (line.length() >= key.length() &&  tmp == key) {
    line = line.substr(key.length(), line.length());
    line = Trim(line);
    exist = true;
  }

  return exist;
}



void Preprocessor::logicIfPhase(std::string& line)
{
  // Does IFDEF exist?
  bool exist = preprocessorDirExist(IFDEF, line);
  if (exist) {
    std::string macro = GetWord(line);

    // There shouldnt be anything after a #IFDEF <macroname>
    if (line.length()) {
      std::ostringstream msg;
      msg << "Extra item following " << _preprocessor_directives[IFDEF] << ": " << line;
      throw std::logic_error(msg.str());
    }

    IfState curr_state = _if_stack.top();
    unsigned short level = curr_state.second;

    if (curr_state.first)
      level++;

    // Now check if this macro is defined
    if (_macro_map.find(macro) != _macro_map.end()) {
      // yes it was found


      _if_stack.push(IfState(curr_state.first, level)); // Ok current state of if is dont skip lines, push that to the stack

      _if_start = _line_no; // found one if on line _line_no
    }
    else {
     
      _if_stack.push(IfState(false, level)); // Ok current state of if is dont skip lines, push that to the stack
    }
    line = ""; // Remove this line, we have taken the macro definition, we are satisfied with that
    return;
  }

  exist = preprocessorDirExist(ELSE, line);
  if (exist) {
    // There shouldnt be anything after a #else
    if (line.length()) {
      std::ostringstream msg;
      msg << "Extra item following " << _preprocessor_directives[ELSE] << ": " << line;
      throw std::logic_error(msg.str());
    }

    if (!_if_stack.size()) { // No previous Ifs?
      // else without if
      throw std::logic_error("#else without matching #if or #ifdef"); 
    }

    IfState curr_state = _if_stack.top();
    
    if (curr_state.second==_if_stack.size())
      _if_stack.invert();

    
    line = ""; // Remove this line, we have taken the macro definition, we are satisfied with that
    return;
  }

  exist = preprocessorDirExist(ENDIF, line);
  if (exist) {
    // There shouldnt be anything after a #endif
    if (line.length()) {
      std::ostringstream msg;
      msg << "Extra item following " << _preprocessor_directives[ENDIF] << ": " << line;
      throw std::logic_error(msg.str());
    }

    if (!_if_stack.size()) { // ouch found an endif, and no if has been found previously
      // else without if
      throw std::logic_error("#endif without matching #if"); 
    }
    _if_stack.pop(); // decrease the depth of if:s

    line = ""; // Remove this line, we have taken the macro definition, we are satisfied with that
    return;
  }
}



void Preprocessor::undefMacroPhase(std::string& line)
{

  if (preprocessorDirExist(UNDEF, line)) { 
    std::string macro = GetWord(line);

    std::string value;
    if (line.length()) {
      value = line;
    }
    line = ""; // Remove this line, we have taken the macro definition, we are satisfied with that

    if (_macro_map.find(macro) != _macro_map.end()) 
      _macro_map.erase(macro); // If it should exist, well remove it.
    else {
      std::ostringstream msg;
      msg << "Macro " << macro << " is undefined, unable to #undef it" ;
      throw std::logic_error(msg.str());
    }


  }
}


void Preprocessor::extractMacroPhase(std::string& line)
{
  
  if (preprocessorDirExist(DEFINE, line)) {
  
    std::string macro = GetWord(line);

    std::string value;
    if (line.length()) {
      value = line;
    }
    line = ""; // Remove this line, we have taken the macro definition, we are satisfied with that

    _macro_map.erase(macro); // If it should exist, well remove it.
    _macro_map.insert(MacroMapType(macro, value));
  }

}

//
void Preprocessor::addMacro(const std::string& macro, const std::string& value)
{
  _macro_map.erase(macro); // If it should exist, we remove it.
  _macro_map.insert(MacroMapType(macro, value));
}

void Preprocessor::replacePhase(std::string& line)
{
                                 // Search the line for any matching macros, if found, replace the macro with the associated value
  // from the macro map.
   MacroMapIterator mmi;

   unsigned int no_loops=0;
  // Check all macros
  for(mmi = _macro_map.begin(); mmi != _macro_map.end(); mmi++) {
    std::size_t pos;
    do {
      pos = line.find(mmi->first);
      if (pos != std::string::npos) { // we found it, replace it!
        line = line.substr(0, pos) + (mmi->second) + line.substr(pos+(mmi->first).length(), line.length());
      }
    } while(pos != std::string::npos && (no_loops++ < MAX_LOOPS));
    // Check so that we havent got a recursive replacement (replacing A with A for example)
    if (no_loops >= MAX_LOOPS) {
      std::ostringstream msg;
      msg << "Maximum number of iteratons (" << MAX_LOOPS << " when replacing macro (" << (mmi->second) << "), recursion?" ;
      throw std::logic_error(msg.str());
    }
  }
}

bool Preprocessor::checkMatch() const 
{
  return (!_if_stack.size());
}

/*------------------------------------------

* $Source: /cvsroot/replicantbody/replicantbody/src/rcfg/Preprocessor.cpp,v $
* $Revision: 1.1 $ 
* $Date: 2005/02/09 17:26:57 $
* $Author: deepone $ 
* $Locker:  $

  Description: Implementation of a C stylish preprocessor

* $Log: Preprocessor.cpp,v $
* Revision 1.1  2005/02/09 17:26:57  deepone
* vrutils and cfgscript put into new namespaces
*
* Revision 1.1.1.1  2005/02/06 06:52:59  deepone
* initial sourceforge checkin
*
* Revision 1.10  2003/12/17 09:28:51  andersb
* Made configscript a dll library under windows
* Fixed memory leaks.
* Fixed bug in processing included files in wrong order.
*
* Revision 1.9  2003/12/16 11:34:45  andersb
* *** empty log message ***
*
* Revision 1.8  2003/12/05 10:43:37  deepone
* *** empty log message ***
*
* Revision 1.7  2003/03/03 12:16:51  Anders Backman
* removed more std::ends
*
* Revision 1.6  2003/02/27 14:02:58  andersb
* Works under gcc3.2
*
* Revision 1.5  2002/10/22 12:30:57  Anders Backman
* Moved around include files.
*
* Revision 1.4  2002/09/27 08:01:07  andersb
* Added #undef
* Still bug under linux regarding #include non existing files
*
* Revision 1.3  2002/08/03 09:49:26  andersb
* Now also #include works but not finished and polished.
*
* Revision 1.2  2002/08/01 14:36:31  andersb
* Fixed so that requesting a int casts an expression and a float to an int.
*
* Revision 1.1  2002/07/21 21:55:34  andersb
* Added preprocessor directives:
* #define <macro> <value>
* #ifdef <macroname>
* #else
* #endif
* Also removed MSv6.0 project files.
* Preprocessor.h and Preprocessor.cpp is added.
*

------------------------------------------*/
