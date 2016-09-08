// $Id: ConfigIO.cpp,v 1.1 2005/02/09 17:26:57 deepone Exp $

#include "rcfg/ConfigIO.h"
#include "rcfg/utils.h"
#include <sys/stat.h>


#include <vector>
#include <list>

#include "rcfg/Item.h"

#include <sstream>
#include <stdio.h>

using namespace rcfg;

#if HAVE_NEW_IOSTREAM
#else
class Eof {
public:
  Eof(const std::string& m) : m_message(m) {}

  std::string what() { return m_message; }
private:
  std::string m_message;
};
#endif


const std::string  COMMENT_STRING="//";
//
ConfigIO::ConfigIO( void ): _comment_string(COMMENT_STRING),  
 MAX_LINES(1000000), _have_header(false), _item_map(0L), _current_scope(0L), MAX_INCLUDE_DEPTH(15)
{

    // Add platform macros
#ifdef _WIN32
    addMacro("WIN32", "1");
#else
    addMacro("UNIX", "1");
#endif

}

//
ConfigIO::ConfigIO( const std::string& fileName, const std::string& header ) : 
  MAX_LINES(1000000), _item_map(0L), _current_scope(0L), MAX_INCLUDE_DEPTH(15)

{
  ConfigIO();
  OpenRead(fileName, header);
}

//
void ConfigIO::Close(  ) 
{
  // Close all opened config scripts
  // Now handled by destructors
}


//
ConfigIO::~ConfigIO( void )
{
  Close();
}


void ConfigIO::ReadString( const std::string& title, const std::string& data, const std::string& header )
{
  // Create a new IOState
  pushIOState(new IOState(title, data));

  // Get the total include path for this file
  currIOState()->setIncludePath( getIncludePath() );

  _header = header;

  checkHeader();

  // Now read the whole file
  ReadFile();  

  currIOState()->finish();
        
  // Push the state back to the work queue for later parsing
  pushWorkQueue(currIOState());
        
  // Pop the state stack
  popIOState();

  if (!_preprocessor.checkMatch()) {
    std::ostringstream msg;
    msg << "IOError: String " << title << " contains an error "  << ": #ifdef missing matching #endif";
    throw std::runtime_error(msg.str());
//    IOError(currIOState(), "#ifdef missing matching #endif");
  }

}


void ConfigIO::OpenRead( const std::string& fileName, const std::string& header )
{
  _openRead( fileName, header );
  if (!_preprocessor.checkMatch()) {
    std::ostringstream msg;
    msg << "IOError: File " <<  fileName << ": #ifdef missing matching #endif";
    throw std::runtime_error(msg.str());
//    IOError(currIOState(), "#ifdef missing matching #endif");
  }

}

//
void ConfigIO::_openRead( const std::string& fileName, const std::string& header )
{

  // Create a new IOState
  pushIOState(new IOState(fileName));

  // Get the total include path for this file
  currIOState()->setIncludePath( getIncludePath() );

  _header = header;

  checkHeader();

  // Push the state back to the work queue for later parsing
  pushWorkQueue(currIOState());


  // Now read the whole file
  ReadFile();  

  currIOState()->finish();
        
        
  // Pop the state stack
  popIOState();

}

void ConfigIO::checkHeader( void ) 
{
  // If a header is given, check so it matches the first line in the file
  if (_header.length()) {
    _have_header = true;
    std::string line;
    try {      
      line = ReadLine();
    }
#if HAVE_NEW_IOSTREAM
    catch (std::ios_base::failure)  {
#else
    catch (Eof& e)  {
#endif
      throw IOError(currIOState(), "error during reading.");
    }

    //line = Trim(line);
    // compare line and header
    //if (line != _header)
    //  throw IOError(currIOState(), "contains an invalid header");
  }
}

//
ConfigValue *ConfigIO::GetItem(std::string &key, int start_line) 
{
  ConfigValue *value =0;

  workQueue()->lineDB.set(start_line);

  if (!workQueue()->lineDB.valid(start_line))
    return 0;

  // Skip empty lines
  while(!workQueue()->lineDB.ref().size() && workQueue()->lineDB.next())
    ;

  if (workQueue()->lineDB.end())
    return 0;

  start_line = workQueue()->lineDB.currLineNo();
//  workQueue()->curr_line = start_line;

  workQueue()->item_start = start_line; //workQueue()->curr_line; // Save the line number on which this item started

  std::string& line = workQueue()->lineDB.ref();

  // Extract the keyword from the current line
  key = GetWord(line);

  // Check so that it is a valid key.
  try {
    ValidKey(key);
  }
  catch(std::string message) {
    throw ParseError(workQueue(), start_line, workQueue()->lineDB.currLineNo(), message);
  }

  // Now found out what start delimiter the value has, one of {, [, " or nothing (a scalar)

  // The line is emptym try next line
  if (!line.length()) {
    if (!workQueue()->lineDB.next()) {
      std::ostringstream msg_str;
      msg_str << "Key (" << key << "), missing datavalue";
      throw ParseError(workQueue(), start_line, workQueue()->lineDB.currLineNo(), msg_str.str());
    }
  }

  // Get the value for this key
  value = GetValue(start_line); //, end_delimiter, type);

  return value;
}

//
ConfigValue *ConfigIO::ExtractValue(std::string& line, int start_line)
{
  ConfigValue::ValueType type;
  char end_delimiter;


  ConfigIO::Value *val=0;

  if (!line.length())
    throw ParseError(workQueue(), start_line, workQueue()->lineDB.currLineNo(), "Empty data string found");

  char start_delimiter = line.at(0);
  type = ConfigValue::VALUE_NONE;

  switch (start_delimiter) {

    // Looks like a struct
    case '{':
      end_delimiter = '}';
      type = ConfigValue::VALUE_STRUCT;
      val = GetStruct(start_line);

      break;
    
    // Looks like an array
    case '[':
      end_delimiter = ']';
      type = ConfigValue::VALUE_FLOAT_ARRAY;
      val = GetArray(start_line, end_delimiter);
      break;
    
      // Looks like a string
    case '"': {
      end_delimiter = '"';
      type = ConfigValue::VALUE_STRING;
      val = GetString(line, start_line, end_delimiter);
      break;
    }

    // Looks like a number or expression
    default: 
      {
        std::string s_val = GetWord(line);
        val = new SingleValue(s_val, start_line, start_line);
        type = ConfigValue::VALUE_EXPRESSION;
      }
      break;
  }
  
  // Was it a valid value?
  try {
    ValidValue(val, type);
  }
  catch (std::string message) {
    throw ParseError(workQueue(), start_line, workQueue()->lineDB.currLineNo(), message);
  }

  ConfigValue *value=0;
  value = ParseValue(val, type);

  return value;  
}

// Extracts the value from line
ConfigValue *ConfigIO::GetValue(int start_line) 
{
  // Find first non empty line
  
  while(!workQueue()->lineDB.ref().size() && workQueue()->lineDB.next())
    ;

  std::string& line = workQueue()->lineDB.ref();
  if (!line.length())
    throw ParseError(workQueue(), start_line, workQueue()->lineDB.currLineNo(), "No value found");

  return ExtractValue(line, start_line);

}


// Find the start and end line for the next valid struct
ConfigIO::Value *ConfigIO::GetStruct(int start_line)
{
  
  // remove the first delimiter {
  std::string& line = workQueue()->lineDB.ref();
  line = line.substr(1, line.length());

  int end_line = start_line;

  bool inside_string=false;
  int parenthis_stack=1;

  unsigned int i=0;
  while(parenthis_stack) {
    std::string& line = workQueue()->lineDB.ref();

    for(i=0; i < line.size();i++) {
      switch(line[i]) {
      
      case '"':
        inside_string = !inside_string;
        break;

      case '{':
        if (!inside_string)
          parenthis_stack++;
        break;

      case '}':
        if (!inside_string)
          parenthis_stack--;

        if (parenthis_stack < 0) {
          if (inside_string)
            throw ParseError(workQueue(), start_line, workQueue()->lineDB.currLineNo(), "Unbalanced \" ");
          else
            throw ParseError(workQueue(), start_line, workQueue()->lineDB.currLineNo(), "Unbalanced {} ");
        }
        break;
      } // switch
    } // for

    if (!parenthis_stack)
      break;

    // Still not balanced {}. Read another line from the file
    if (!workQueue()->lineDB.next()) {
      if (inside_string)
        throw ParseError(workQueue(), start_line, workQueue()->lineDB.currLineNo(), "Unbalanced \" while parsing a struct");
      else
        throw ParseError(workQueue(), start_line, workQueue()->lineDB.currLineNo(), "Unbalanced {} while parsing a struct");
    }
  } // while

  end_line = workQueue()->lineDB.currLineNo();
  int delimiter_pos = i;
   
  return new StructValue(start_line, end_line, delimiter_pos);
}

// Find the next delimiter in the file.
ConfigIO::Value *ConfigIO::GetString(std::string& line, int start_line, char delimiter)
{

  std::string string_value;
  
  // remove the first delimiter
//  line = workQueue()->lineDB.ref();
  line = line.substr(1, line.length());
  
  // now find the next delimiter that ends the string
  bool found = false;
  int end_line=0;
  char str_delim[3];
  sprintf(str_delim, "%c\0", delimiter);

  //string_value += line;
  std::size_t pos = line.find(str_delim);
  // Did we find it on this line?
  if (pos != std::string::npos) {
    string_value += line.substr(0, pos);
      
    // Remove end-delimiter
    line = line.erase(0, pos+1); //substr(0, string_value.length()-1);
    found = true;
    end_line = workQueue()->lineDB.currLineNo();
  }
  
  if (!found) {
    std::ostringstream msg_str;
    msg_str << "string is missing end delimiter: " << str_delim;
    throw ParseError(workQueue(), start_line, workQueue()->lineDB.currLineNo(), msg_str.str());
  }

  // We found it!
  return new SingleValue(string_value, start_line, end_line);
}


// Find the next delimiter in the file.
ConfigIO::Value *ConfigIO::GetArray(int start_line, char delimiter)
{

  std::string string_value;
  int end_line = start_line;
  
  // remove the first delimiter
  std::string& line = workQueue()->lineDB.ref();
  line = line.substr(1, line.length());
  
  // now find the next ] that ends the array
  bool found = false;
  char str_delim[3];
  sprintf(str_delim, "%c\0", delimiter);

  do {
    std::string& line = workQueue()->lineDB.ref();
    //string_value += line;
    std::size_t pos = line.find(str_delim);
    // Did we find it on this line?
    if (pos != std::string::npos) {
      string_value += line.substr(0, pos);
      
      // Remove end-delimiter
      line = line.erase(0, pos+1); //substr(0, string_value.length()-1);
      end_line = workQueue()->lineDB.currLineNo();
      found = true;
    }
    else {// New line found
      string_value += line;
      line = ""; // Eat the whole line
      if (delimiter == '"') // If string, add new line char
        string_value += std::string("\n");
      else 
        string_value += std::string(" "); // if array add space


    }

  } while (!found && workQueue()->lineDB.next()); // Continue with next line 
  
  if (!found) {
    std::ostringstream msg_str;
    msg_str << "string is missing end delimiter: " << str_delim;
    throw ParseError(workQueue(), start_line, workQueue()->lineDB.currLineNo(), msg_str.str());
  }

  // We found it!
  return new SingleValue(string_value, start_line, end_line);
}


// value will be deallocated!
ConfigValue *ConfigIO::ParseValue( ConfigIO::Value *value, ConfigValue::ValueType type) 
{

    ConfigValue *val=0;

  switch (type) {
    case (ConfigValue::VALUE_STRING):
      val =  new StringItem(value->val());
      break;

    case (ConfigValue::VALUE_INT):
      val =   new IntItem(atoi(value->val().c_str()));
      break;

    case (ConfigValue::VALUE_FLOAT):
      val =  new FloatItem((float)atof(value->val().c_str()));
      break;

    case (ConfigValue::VALUE_EXPRESSION):
      val = parseExpression(value);
      break;

    case (ConfigValue::VALUE_FLOAT_ARRAY):
      val =  ParseArray(value);
      //      return new Item<int>(value);
      break;
    case (ConfigValue::VALUE_STRUCT):
      val =  ParseStruct(value);
      //      return new Item<int>(value);
      break;
    default:
      throw ParseError(workQueue(), value->start(), workQueue()->lineDB.currLineNo(), "Unknown value type");
  }

  // Done using this value
  delete value;

  // Check so that there are no trailing garbage on the line
  if (workQueue()->lineDB.ref().size()) {
    std::ostringstream msg_str;
    msg_str << "Extra item following data: \"" << workQueue()->lineDB.val() << "\"";
    throw ParseError(workQueue(), value->start(), workQueue()->lineDB.currLineNo(), msg_str.str());
  }
  workQueue()->curr_line = workQueue()->lineDB.currLineNo()+1;
  return val;
}

// Parse a mathematical expression
ConfigValue *ConfigIO::parseExpression( ConfigIO::Value *value )
{

  ConfigExpressionSolver solver(this);
  solver.enableAssignments(false);

  std::string e = value->val();
  ConfigExpressionSolver::ExpressionType result;

  if (solver.evaluate(e, result))
    throw ParseError(workQueue(), value->start(), workQueue()->lineDB.currLineNo(), solver.getLastError());

  return new ExpressionItem(Expression(e, result));
}


// Parse a value like: [ val1 val2 ... valn]
ConfigValue *ConfigIO::ParseArray( ConfigIO::Value *value )
{
  Array vec;

  std::string word;
  std::string line = value->val();
  
  // By default it is a VALUE_FLOAT_ARRAY (float int, expressions whatever)
  // However, it could turn out that it is a string array later on

  // The first item in an array determines what kind of array it is

  ConfigValue::ValueType type = ConfigValue::VALUE_FLOAT_ARRAY;
  bool first = true;
  // For each word (value) transform it to a float and insert into the vec vector
  do {
    word = GetWord( line );

    std::string item = word;

    ConfigValue *v;

    v = ExtractValue(word, value->start());

//    if (!StringToFloat(word, fval)) 
//      throw ParseError(_filename, value->start(), _lineDB.currLineNo(), "Invalid number: " + word);

    // First item in the array, the type of this determines the type of the whole array
    // String or literal array
    if (first) {
      first = false;
      if (v->Type() == ConfigValue::VALUE_STRING)
        type = ConfigValue::VALUE_STRING_ARRAY;
    }

    // Only strings in VALUE_STRING_ARRAYS is valid
    if (type == ConfigValue::VALUE_STRING_ARRAY && v->Type() != ConfigValue::VALUE_STRING)  {
      delete v;
      throw ParseError(workQueue(), value->start(), workQueue()->lineDB.currLineNo(), 
        std::string("Invalid item found in string array: ") +  item);
    }
    
    // Only floats ints and expressions is valid in a VALUE_FLOAT_ARRAY arrays
    if (type == ConfigValue::VALUE_FLOAT_ARRAY && (v->Type() != ConfigValue::VALUE_FLOAT  && v->Type() != ConfigValue::VALUE_EXPRESSION &&
      v->Type() != ConfigValue::VALUE_INT)) {
      delete v;
      throw ParseError(workQueue(), value->start(), workQueue()->lineDB.currLineNo(), std::string("Invalid item found in array: ") +  item);
    }
    vec.push_back( v ); // Insert the valid value

  } while(line.find_first_of(" ") != std::string::npos || line.length() );

  if (!vec.size())
    throw ParseError(workQueue(), value->start(), workQueue()->lineDB.currLineNo(), "No valid values found in vector " + value->val());
/*  else if (vec.size() < 1)
    throw ParseError(_filename, _start_line_no, "Less than two values found in vector " + value);    
*/
  return new FloatVectorItem(vec, type);
}

// When we get here line contains a struct definition.
// It could a recursive structure i.e. a struct within a struct
ConfigValue *ConfigIO::ParseStruct( ConfigIO::Value *val )
{  
  std::string key;
  ConfigValue *value;
  StructMap *struct_map = new StructMap;
  if (_current_scope)
    struct_map->SetParent(_current_scope);
  
  _current_scope = struct_map;
  
  int curr_line=0;
  std::string item;

  try {
    for(curr_line = val->start(); curr_line <= val->end(); curr_line++) 
    {

      workQueue()->lineDB.set(curr_line);

      // Skip empty lines
      while(!workQueue()->lineDB.ref().size() && workQueue()->lineDB.next())
        ;

      curr_line = workQueue()->lineDB.currLineNo();
      if (curr_line > val->end()) {
        throw ParseError(workQueue(), curr_line, curr_line, "Hm confused, empty lines found for struct");
      }

      std::string& line = workQueue()->lineDB.ref();
      if (line[0] == '}')
        break;

      value = GetItem(key, val->start());
      
      // If the value is a struct, then set the parent pointer of that struct
      // to be the current struct, this helps when solving scopes for variables in expression later on.
#if 0
      if (value->Type() == ConfigValue::VALUE_STRUCT) {
        StructMap  *smptr = ((StructItem *)value)->GetVal();
        smptr->SetParent(struct_map);
          // Set the current scope to be this struct
        _current_scope = struct_map;
      }
#endif        
      struct_map->insert(std::make_pair(key, value));

    } // for
    if (curr_line == val->end()) {
      if (workQueue()->lineDB.currLineNo() == curr_line) {
        std::string& line = workQueue()->lineDB.ref();
        std::size_t pos = line.find("}");
        if (pos != std::string::npos)
          line.erase(pos,1);
        else {
          std::cerr << "Opps: " << __FILE__ << ":" << __LINE__ << std::endl;
        }
      }
    }
    // Now find the } and eat it

  }
  catch(std::exception& e) {
    delete struct_map;
    throw std::runtime_error(e.what());
  }

  // Clear the current scope
  _current_scope = struct_map->GetParent();

  return new StructMapItem(struct_map);
}


// Opens the configfile in write mode, also writes the header to the file
void ConfigIO::OpenWrite( const std::string& fileName, const std::string& header )
{
  _out_state.m_filename = fileName;

  Close();

  // Open the file
#if HAVE_NEW_IOSTREAM

  _out_state.out.open(_out_state.m_filename.c_str(), std::ios_base::out | std::ios_base::trunc);
#else
  _out_state.out.open(_out_state.m_filename.c_str(), std::ios::out | std::ios::trunc);
#endif
  if (!_out_state.out.is_open()) throw IOError(&_out_state, "Unable to open file for writing");
  
  // Write the header as the first line
  if (header.length())
    PutLine(header);
}



// Write one line to out-stream
void ConfigIO::PutLine( const std::string& line )
{
 
  if (!_out_state.out.is_open())  throw IOError(&_out_state, "File is not opened for writing");

  _out_state.out << line.c_str() << std::endl;
  
}



// Writes an item to the opened configfile
void ConfigIO::PutItem(const std::string& key, const ConfigValue *value)
{
  if (!_out_state.out.is_open())  throw IOError(&_out_state, "File is not opened for writing");

    _out_state.out << key << *value << std::endl;
}




//
void ConfigIO::PutMap( const ItemMap& item_map )
{
  if (!_out_state.out.is_open())  throw IOError(&_out_state, "File is not opened for writing");

  _out_state.out << item_map << std::endl;
}


//
void ConfigIO::GetMap( ItemMap& item_map )
{
  std::string key;
  ConfigValue *value;

  // Make the item map known by all methods in this class
  // useful for expression parsing
  _item_map = &item_map;

  while(workLoad()) {
    if (!workQueue()->ok())  throw IOError(workQueue(), "File has not been read correctly");

    //std::cerr << "Processing: " << workQueue()->m_filename << std::endl;
    // As long we can, read all the items from the file.
    // Insert them into the map
    while((value = GetItem(key, workQueue()->curr_line))) {
      item_map.insert(std::make_pair(key, value));
    }

    popWorkQueue();
  }

}


// Read one line from in-stream
std::string ConfigIO::ReadLine( void )
{
  std::string line;
 

#if HAVE_NEW_IOSTREAM
  std::ios_base::iostate old_state = currIOState()->getInStream().exceptions();
#else
  std::ios::iostate old_state = currIOState()->getInStream().exceptions();
#endif
  try {

    // Do not throw an exception on eof
#if HAVE_NEW_IOSTREAM
    currIOState()->getInStream().exceptions(std::ios_base::eofbit);
#else
    currIOState()->getInStream().exceptions(std::ios::eofbit);
#endif
  
    // Did we reach eof last time? Then catch that here
    if (currIOState()->getInStream().eof()) {
#if HAVE_NEW_IOSTREAM
      throw std::ios_base::failure("end of file reached");
#else
      throw Eof("End of file reached");

#endif
    }

    std::getline(currIOState()->getInStream(), line); 
    //line = tmp_line;
    if (currIOState()->getInStream().eof())
#if HAVE_NEW_IOSTREAM
      throw std::ios_base::failure("end of file reached");
#else
      throw Eof("End of file reached");
#endif


    // Remove all non printable characters
    RemoveCntrlChar(line);

    currIOState()->getInStream().exceptions(old_state);
  }
  
  // Catch the eof exception
#if HAVE_NEW_IOSTREAM
  catch (std::ios_base::failure) { 
#else
  catch (Eof& e) {
#endif
    currIOState()->getInStream().exceptions(old_state); // reset the exception state
    
    // We reached the eof, but did we get anything into the line?
    // If so, do not throw an error, catch it the next time
    if (!line.length()) {
#if HAVE_NEW_IOSTREAM
      throw std::ios_base::failure("oops");   // throw another eof file exception 
                                              // to signal it to callee
#else
      throw Eof("Endoffile");
#endif
    }
  }
  return line;
}


// Read the whole file
void ConfigIO::ReadFile( void )
{
  std::string line;

  // Remove any previous content from a file
  currIOState()->lineDB.clear();

  if (_have_header) {
    currIOState()->lineDB.push_back("Header dummy");
    currIOState()->curr_line = 1;
  }
  else
    currIOState()->curr_line = 0;

  // Now read the whole file into the _file_content array

  try { 

    while(currIOState()->lineDB.size() <= MAX_LINES) {
      line = ReadLineNoComments();

      try {
        _preprocessor.processLine(line);
      }
      catch(std::exception& e) {
        throw ParseError(currIOState(), (int)currIOState()->lineDB.size(), (int)currIOState()->lineDB.size(), e.what());
      }

      std::string include_path = _preprocessor.getIncludePath();
      if (include_path.length()) {
        if (sizeIOStateStack() > MAX_INCLUDE_DEPTH) {
          std::ostringstream msg;
          msg << "Maximum depth (" << MAX_INCLUDE_DEPTH << ") for including files is reached, recursive include directives perhaps?";
          throw IOError(currIOState(), msg.str());
        }
        
        // Open the included file
        _openRead(include_path, _header);
      }

      currIOState()->lineDB.push_back(line);
    }
    // Check so that we havent exeeded the maximum number of lines
    if (currIOState()->lineDB.size() >= MAX_LINES) {
      std::ostringstream msg_str;
      msg_str << "Maximum lines (" << MAX_LINES << ") exeeded, file parsing aborted.";
      Close();
      throw IOError(currIOState(), msg_str.str());
    }

  }

  // Catch the eof exception
#if HAVE_NEW_IOSTREAM
  catch (std::ios_base::failure) { 
#else
  catch(Eof &e) {
#endif
  }


 catch(std::exception& e) {
   throw IOError(currIOState(), e.what());

 }
 catch(...) {
   throw IOError(currIOState(), "Undefined exception in ReadLine: ");
 }
 

}




// Reads a line and removes everything after the comments
std::string ConfigIO::ReadLineNoComments( void )
{
  std::string line;
  
  line = ReadLine();

  // Find the position of the comment string
  // Remove the rest of the line
  std::size_t comment_pos = line.find(_comment_string); 
  if (comment_pos != std::string::npos) {
    std::size_t p1 = line.find_first_of('"');
    if (p1 != std::string::npos && p1 < comment_pos) { // The comment sign is after a ", could be inside a string
      std::string tmp(line);
      tmp.replace(p1, 1, "!"); // Remove the " sign
      std::size_t p2 = tmp.find_first_of('"');
      if (p2 != std::string::npos && p2 > comment_pos)
        return line; // The comment sign is within a string, ignore it
    }
    line.resize(comment_pos);
  }

  // Remove leading and trailing blanks
  line = Trim(line);
  return line;


}

bool ConfigIO::setSearchPathEnvironmentVariable(const std::string& env_variable)
{
  return FilePathContainer().setEnvironmentVariable(env_variable);
}

void ConfigIO::setSearchPathString(const std::string& path)
{
  FilePathContainer().setFilePathList(path);
}

//
void ConfigIO::ValidValue( ConfigIO::Value *value, ConfigValue::ValueType& type )
{
  
  // If it is an empty value, then it could be an empty string
  // which is of course valid
 // if (!value.length() && type != ConfigValue::VALUE_STRING)
  //  throw std::string("invalid value, empty!");
  
  if (type == ConfigValue::VALUE_NONE)
    throw "Invalid value: " + value->val();

  if (type == ConfigValue::VALUE_FLOAT) {

    // Is it an integer? (no decimalpoint or Exponent E found)
    //if (value->val().find_first_of(".E") == std::string::npos)
    //  type = ConfigValue::VALUE_INT;

    float val;
    if (!StringToFloat(value->val(), val))
      throw "invalid number: " + value->val(); // It must be an invalid number
  }
  // If it is an expression the parsing later on will take care of checking the syntax
  // Unless it is just a literal
  else if (type == ConfigValue::VALUE_EXPRESSION) {
    float val;
    // Is this expression just a numeric value? Then test it for a FLOAT  
    if (StringToFloat(value->val(), val)) {
      type = ConfigValue::VALUE_FLOAT;
      ValidValue(value, type);
    }
  }
  else
    return;
}



ConfigValue *ConfigExpressionSolver::recursiveSearchValue(StructMap *struct_map, ConfigValue::ValueType type, std::vector<std::string>& key_vector, bool up)
{
  // Search in the item map and find the variable name, it should be an integer or an float or another expression.
  
  ConfigValue *value=0L;

  // Create a list of keys from a string containing .operator for accessing structure members
  // A string with no .:s will be a vector containing one element
  //std::vector<std::string> key_vector;
  //int len = rcfg::ConfigScript::CreateKeyVector( name, key_vector);

  //StructMap *sm=getCIO()->getCurrentScope();

  //int idx = 0; // Start looking for the first key

  std::string key = key_vector[0];

  // First look in the current scope
  value = struct_map->findConfigValue(key);
 
  if (value) { // We found the key we are looking for
    if ( key_vector.size() == 1 )  { // Is it the last key? (Final destination?)
      if (value->Type() && type) { // Does it have the right type?
        //sm->GetVal(key, v);
        //value = (ConfigExpressionSolver::ExpressionType)v;
        return value;
      } else {
        return 0L; // We found it, but it wasnt of the right type
      }
    }
    else  { // We found an item, but we still have a few keys left
      // Then if we still havent found the final destination and we have keys left.
      // THen the found value have to be a struct, otherwise this is a dead end
      if (value->Type() != ConfigValue::VALUE_STRUCT)
        return 0L;

      StructMap *sm=0;

      // Ok its a Struct, go down      
      key_vector.erase(key_vector.begin()); // Erase the first key as we have found it
      bool flag;
      flag = ((StructItem *)value)->GetVal(sm);
      if (!flag)
        return 0L;

      value = recursiveSearchValue(sm, type, key_vector, false);
      return value;
    }
  }

  // We didnt find the key here
  if (!up) {
    // As we are moving down, and we didnt find the key, we wont be able to find it at all.
    return 0L;
  }

  // We will now try in the parents scope to find it
  StructMap *sm;
  sm = struct_map->GetParent();
  if (!sm) 
    return 0L; // No parent, havent found it, sorry    

  return value = recursiveSearchValue(sm, type, key_vector, true);

}

bool ConfigExpressionSolver::getVariableValue(const std::string& name, ExpressionSolver::ExpressionType& value)
{

  ConfigValue *val=0L;
  float v;
  bool flag=false;

  std::vector<std::string> key_vector;
  std::size_t len = rcfg::ConfigScript::CreateKeyVector( name, key_vector);
  if (len==0)
    return false;

  // Is the current scope set? Well then we are inside a struct, try to
  // search for the variable name from this scope and outward.

  StructMap *sm = getCIO()->getCurrentScope();
  if (sm) {
    val = recursiveSearchValue(sm, ConfigValue::VALUE_NUMBER, key_vector);
    if (val) {
      flag = val->GetLiteralValue(v);
      if (flag) {
        value = (ExpressionSolver::ExpressionType)v;
        return flag;
      }
    }
  }

  
  // If the variable contains a . then it is a direct reference to a member of a struct
  flag = getCIO()->getConfigScript().Get(name, v);
  if (flag)
    value = (ConfigExpressionSolver::ExpressionType)v;

  return flag;

  // Search in the global scope
  /*if (!found) {
    ItemMap *item_map = getCIO()->getItemMap();

    assert(item_map && "_item_map pointer is null");
    found = item_map->getLiteral(name, v);
    if (found)
      value = (ConfigExpressionSolver::ExpressionType)v;
 
  }
  return found;*/
}

ConfigIO::IOStateStack::~IOStateStack()
{
  while(size()) {
    pop();
  }
}
void ConfigIO::IOStateStack::pop()
{
  if (size()) {
    m_vec.resize(size()-1);
    //std::cerr << "IOStateStack::pop(): Size: " << size() << std::endl;
  }
}

std::string ConfigIO::IOStateStack::getIncludePath()
{
  

  std::string path;
  std::string space;

  int level=0;
  for(unsigned int i=0; i < size()-1; i++) { // Dont use the last
    space.resize(level, ' ');
    level +=3;
    path += space + m_vec[i]->m_filename + "\n";
  }    
  path += "\n";

  return path;    
}


std::ostream& operator<<(std::ostream& os, const ConfigIO::IOState* s)  {
  if (s->haveSStream())
    os << "in stream named: " << s->m_title << " ";
  else
    os << "in file: " << s->m_filename << " ";

  if (s->getIncludePath().length()) 
    os << std::endl << "included from:\n\n" << s->getIncludePath();
  
  return os;
}


IOError::IOError(const ConfigIO::IOState *s, const std::string& m )
{  
  std::ostringstream msg;
  msg << "IOError: File " << s << m;
  throw std::runtime_error(msg.str());
}



ConfigIO::IOState::IOState(const std::string& filename) : 
  item_start(0), curr_line(0), m_filename(filename), m_title(""), 
  m_finished(false),  m_istrstream(""), m_use_sstream(false) 
{

  // Use the list of directories to search for the file in
  std::string filepath = FilePathContainer().findFile(m_filename);

  if (filepath.length())
    m_filename = filepath;

  // Get the status of the file.
  struct stat buf;
  if (stat( m_filename.c_str(), &buf ) == -1) 
    throw IOError(this, "Does not exist.");

  // Is it a directory?
  if ((buf.st_mode & S_IFDIR)) throw IOError(this, "is a directory.");

  if (!(buf.st_mode & S_IFREG)) throw IOError(this, "is not a regular file.");
  
  // Is it empty?
  if (!buf.st_size) throw IOError(this, "Is empty.");
  
  // Can we read the file?
  if (!(buf.st_mode & S_IREAD ))  throw IOError(this, "is missing read permission.");
    

  // Open the file
  m_ifstream.open(m_filename.c_str());
  if (!m_ifstream || !m_ifstream.is_open()) throw IOError(this, "error opening for reading.");
}

//
ConfigIO::IOState::IOState(const std::string& title, const std::string& in_data) : 
  item_start(0), curr_line(0), m_title(title),
  m_finished(false),  m_istrstream(in_data.c_str()), m_use_sstream(true)
 
{
}

//
ConfigIO::IOState::IOState() : item_start(0), curr_line(0), 
  m_finished(false), m_istrstream(""), m_use_sstream(false)
{
}

ConfigIO::FilePathSingleton m_filepathSingleton;
ConfigIO::FilePathSingleton* ConfigIO::FilePathSingleton::ms_Singleton=0;


/*------------------------------------------

* $Source: /cvsroot/replicantbody/replicantbody/src/rcfg/ConfigIO.cpp,v $
* $Revision: 1.1 $ 
* $Date: 2005/02/09 17:26:57 $
* $Author: deepone $ 
* $Locker:  $

* $Log: ConfigIO.cpp,v $
* Revision 1.1  2005/02/09 17:26:57  deepone
* vrutils and cfgscript put into new namespaces
*
* Revision 1.1.1.1  2005/02/06 06:52:59  deepone
* initial sourceforge checkin
*
* Revision 1.9  2004/01/08 08:12:06  andersb
* Changed the parsing order so that initial file is read first then included files.
* This is a major problem, but we have to live with it for now.
*
* Revision 1.8  2003/12/17 09:28:51  andersb
* Made configscript a dll library under windows
* Fixed memory leaks.
* Fixed bug in processing included files in wrong order.
*
* Revision 1.7  2003/12/16 11:34:45  andersb
* *** empty log message ***
*
* Revision 1.6  2003/12/05 10:43:37  deepone
* *** empty log message ***
*
* Revision 1.5  2003/12/04 13:30:51  andersb
* Fixed a few compiler warnings.
*
* Revision 1.4  2003/12/02 15:40:14  andersb
* Added scoped struct access within the scripts.
*
* Revision 1.3  2003/11/19 19:14:33  andersb
* Added GetType method for ConfigScript. Returns the type of a named item
*
* Revision 1.2  2003/10/29 15:10:33  andersb
* *** empty log message ***
*
* Revision 1.1  2003/10/27 13:45:47  andersb
* Moved .cxx to .cpp
*
* Revision 1.46  2003/09/10 18:11:12  andersb
* Added WIN32 and UNIX macros that is available from scripts.
*
* Revision 1.45  2003/04/24 06:23:04  andersb
* Added support for Array of Strings
*
* Revision 1.44  2003/03/04 11:19:53  Anders Backman
* Fixed bug that required data beginning on the same line as the key,
* Added a search path feature. Default the environment variable CFG_FILE_PATH is enquired
* for a list of directories to search config files in.
*
* Revision 1.43  2003/03/04 09:23:09  sunna
* *** empty log message ***
*
* Revision 1.42  2003/03/03 16:05:05  deepone
* *** empty log message ***
*
* Revision 1.41  2003/02/27 13:27:54  andersb
* Moved over to sstream instead of strstream
*
* Revision 1.40  2003/02/27 13:08:58  andersb
* *** empty log message ***
*
* Revision 1.39  2003/01/28 14:58:29  Anders Backman
* no message
*
* Revision 1.38  2003/01/27 13:43:51  andersb
* Added ReadString for parsing a string instead of a file.
*
* Revision 1.37  2003/01/07 13:49:55  andersb
* no message
*
* Revision 1.36  2002/12/17 07:25:46  Anders Backman
* Minor bugfix getting assert when not locating a #include file.
*
* Revision 1.35  2002/10/29 16:02:22  andersb
* Fixed the bug the bugfix introduced!
*
*
* Revision 1.33  2002/10/22 12:30:56  Anders Backman
* Moved around include files.
*
* Revision 1.32  2002/09/23 06:33:35  andersb
* no message
*
* Revision 1.31  2002/08/07 14:48:58  andersb
* no message
*
* Revision 1.30  2002/08/06 11:34:50  andersb
* Rewrote Array class so it hides the std::vector. This solved problem
* when using method at(i) with overloaded operator[] under unix.
*
* Revision 1.29  2002/08/03 09:49:26  andersb
* Now also #include works but not finished and polished.
*
* Revision 1.28  2002/07/21 21:55:34  andersb
* Added preprocessor directives:
* #define <macro> <value>
* #ifdef <macroname>
* #else
* #endif
* Also removed MSv6.0 project files.
* Preprocessor.h and Preprocessor.cpp is added.
*
* Revision 1.27  2002/07/09 21:28:57  andersb
* Fixed case bug in Expressions allowing only lowercase variables.
* Added expressions in arrays too.
* Added randInterval(min,max) function in Expressions allowing expressions like:
*
* Position [randInterval(-10,10) randInterval(-10,10) randInterval(0,3)]
*
* Revision 1.26  2002/07/04 23:08:08  andersb
* Added expressions.
* This helps alot writing scripts, where math expressions is available.
* Most standard functions: ceil, floor, sin, cos, etc is available.
* Also pi (3.1415...) and e ( 2.718 ) as constants.
* Scope is also implemented. (a variable name is backtraced upwards from the current
* scope.
*
* Revision 1.25  2002/07/03 18:55:06  andersb
* no message
*
* Revision 1.22  2002/05/21 22:05:08  andersb
* Fixed bug when reading a line that ends with a comment.
*
* Revision 1.21  2002/05/03 09:41:33  andersb
* Now using CommonC++ 2.0.93 with STLPort 4.5.3
*
* Revision 1.20  2002/04/18 06:23:37  andersb
* Minor error in GCC version check.
*
* Revision 1.19  2002/04/18 06:16:31  andersb
* Finally got into the mood to rewrite the ConfigIO so it supports both
* gcc of versions 2.x and 3.x. No more need of g++-3.0!!
*
* Revision 1.18  2002/04/15 07:04:40  andersb
* Peter found one more bug in ConfigScriptOO.cxx regarding "Get":ing numerous
* items with same name. Fixed.
*
* Revision 1.17  2002/04/12 08:49:57  andersb
* Peter found a bug in ConfigScriptOO.cxx made it crash when searching for
* indexed items with Get and Return methods.
*
* Revision 1.16  2002/04/10 14:01:04  andersb
* Fixed some compiler warnings.
*
* Revision 1.15  2002/04/09 14:02:09  andersb
* Some platform issues under Linux!
*
* Revision 1.14  2002/04/09 09:04:56  andersb
* Major rewrite of ConfigIO.cxx.
* Now supports multiline strings and arrays.
*
* Revision 1.13  2002/04/04 20:44:12  andersb
* Allowed arrays of only one value [2.3]
*
* Revision 1.12  2002/02/01 13:35:29  andersb
* Now fully updated to support recursive structs.
* Still problems with case sensitivity.
*
* Revision 1.2  2002/01/08 11:20:37  andersb
* Removed code from default constructor. Did nothing useful.
*
* Revision 1.1.1.1  2002/01/02 13:45:31  andersb
* A new version of Configscript using namespace rcfg.
* Also supporting recursive structures.
*
* Revision 1.11  2001/11/14 13:48:03  andersb
* *** empty log message ***
*
* Revision 1.10  2001/11/14 13:17:00  andersb
* no message
*
* Revision 1.9  2001/11/14 13:12:53  andersb
* New features added:
* Hiearchial structures supported.
* Possible to use a mapID when opening to
* support multiple open configscripts.
*
* Revision 1.8  2001/10/02 13:12:11  andersb
* Allowed empty strings "" for strings.
*
* Revision 1.7  2001/09/19 13:15:47  andersb
* Minur bug in output detected and fixed.
*
* Revision 1.6  2001/09/05 09:03:22  andersb
* Working under SGI.Working under SGI.
*
* Revision 1.5  2001/08/29 09:23:37  andersb
* Now using the old iostream.h library to get it to work together with
* CommonC++.
*
* Revision 1.4  2001/08/10 14:13:57  andersb
* no message
*
* Revision 1.3  2001/08/01 07:11:22  andersb
* Major update
*
* Revision 1.2  2001/04/25 12:45:03  andersb
* Added makefiles for the IRIX platform.
*
* Revision 1.1.1.1  2001/04/25 10:56:28  andersb
* no message
*
* Revision 1.8  2000/11/23 09:06:36  andersb
* *** empty log message ***
*
* Revision 1.7  2000/11/03 14:47:59  andersb
* no message
*
* Revision 1.6  2000/10/10 15:04:25  andersb
* no message
*
* Revision 1.5  2000/10/05 13:59:58  andersb
* no message
*
* Revision 1.4  2000/10/05 10:27:06  andersb
* no message
*
* Revision 1.3  2000/10/03 09:21:23  andersb
* no message
*
* Revision 1.2  2000/10/03 07:44:36  andersb
* Using DOC++
*
* Revision 1.1  2000/10/03 07:02:39  andersb
* no message
*
* Revision 1.4  2000/10/02 14:04:14  andersb
* no message
*
* Revision 1.3  2000/10/02 09:08:30  andersb
* no message
*
* Revision 1.2  2000/09/26 14:14:32  andersb
* no message
*
* Revision 1.1.1.1  2000/09/26 09:05:46  andersb
* no message
*

--------------------------------------------*/
