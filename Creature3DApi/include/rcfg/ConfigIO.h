// $Id: ConfigIO.h,v 1.1 2005/02/09 17:26:54 deepone Exp $

#ifndef __CONFIGIO_H__
#define __CONFIGIO_H__

#ifdef _WIN32
#pragma warning(disable:4786) // Disable warnings about long names

#include "rvrutils/os/memory_debug.h"
#endif


#include <fstream>
#include <string>
#include <queue>
#include "rcfg/utils.h"
#include <sstream>
#include <assert.h>
#include <queue>
#include <stack>

#include "rcfg/ConfigValue.h"
#include "rcfg/ItemMap.h"
#include "rcfg/ExpressionSolver.h"
#include "rcfg/Item.h"
#include "rcfg/ConfigScriptOO.h"
#include <stdexcept>
#include "rcfg/Preprocessor.h"
#include "rvrutils/os/FilePathContainer.h"

#include "rcfg/export.h"

/// A class for indicating parsing error
class CONFIGSCRIPT_API InternalError : public std::exception  {
public:
  ///
  InternalError(const std::string& fname, int line_no, const std::string& m) 
  { 
    std::ostringstream msg;
    msg << "InternalError: " << fname << ": " << line_no << ": " << m;
    std::runtime_error(msg.str());
  }
};


class ConfigIO;

class CONFIGSCRIPT_API ConfigExpressionSolver : public ExpressionSolver {
public:
  ConfigExpressionSolver(ConfigIO *cio) : ExpressionSolver(), m_cio(cio) {}
  virtual ~ConfigExpressionSolver() {}

  virtual bool getVariableValue(const std::string& name, ExpressionSolver::ExpressionType& value);
  ConfigIO *getCIO() { return m_cio; }
private:
  ConfigValue *recursiveSearchValue(StructMap *struct_map, ConfigValue::ValueType type, std::vector<std::string>& key_vector, bool up=true);
  ConfigIO *m_cio;
};

/// Class for reading/writing configuration files
class CONFIGSCRIPT_API ConfigIO {

public:
  friend class ConfigExpressionSolver;

  /// Opens a config file
  ConfigIO( const std::string& fileName, const std::string& header=std::string("") );

  /// Constructor
  ConfigIO( void );

  ///
  ~ConfigIO( );
  
  /// Closes a configuration file
  void Close( );

  /// Opens a config file for reading, checks so the first line contains header
  void OpenRead( const std::string& filename, const std::string& header=std::string("") );

  /// Reads data from the string data instead of a file.
  void ReadString( const std::string& title, const std::string& data, const std::string& header="" );

  /// Opens a config file for writing (makes it empty), Writes the header to the first line
  void OpenWrite( const std::string& fileName, const std::string& header=std::string("") );

  /// Parses the opened file for an item, throws exceptions on error
  ConfigValue *GetItem(std::string &key, int start_line);

  /// Writes an item to the config file
  void PutItem( const std::string &key, const ConfigValue* value);

  /// Writes a whole item map to the config file
  void PutMap( const ItemMap& item_map );

  /// Parses the whole file and returns a item map as the result
  void GetMap( ItemMap& item_map );

  
  /// Changes the default commentstring "//"
  void SetCommentString( std::string cstr ) { _comment_string = cstr; };

  /*!
  Add a macro definition to the preprocessors map of macros. This macro can then be used when parsing 
  configscripts and tested for with #ifdef and also used in expressions.
  It has to be called before the OpenRead method, otherwise it is useless, the parsing is already been done.
  It cant be used with the Constructor that calls Open ConfigIO(const std::string& filename, ...
  because that constructor calls OpenRead.
 */
  void addMacro(const std::string& macro, const std::string& value) { _preprocessor.addMacro(macro, value); }


  /*!
    Set the list of directories to search for scripts in.
    Under win32 a semicolon separated list, under unix a colon ':' separated list.
  */
  static void setSearchPathString(const std::string& path);

  /*!
    Specify the environment variable used for extracing the path to scripts.
    \param env_variable - The name of the environmentvariable containing the list of directories.
    \return bool - true if the environment variable can be found, otherwise false.
  */
  static bool setSearchPathEnvironmentVariable(const std::string& env_variable);

private:

  /*! Opens a config file for reading, checks so the first line contains header
      also handles the IOState stack and the work queue of IOStates
  */
  void _openRead( const std::string& filename, const std::string& header );


  ///
  ItemMap *getItemMap() {return _item_map; }
  ///
  StructMap *getCurrentScope() {return _current_scope; }

  rcfg::ConfigScript& getConfigScript() { _config_script.Open(_item_map); return _config_script; }

  class Value {
  public:
    enum Type { SINGLE, STRUCT };

    Type type() { return type_; }

    int start() {return start_; }
    int end() {return end_; }

    std::string val() { assert(type_ == SINGLE); return value_; }
    int pos() { assert(type_ == STRUCT); return pos_; }

  protected:
    Value(Type t, int start, int end, std::string str=std::string(""), int pos=0) : type_(t), start_(start), 
        end_(end), value_(str), pos_(pos){};

  private:

    Type type_;
    int start_, end_;
    std::string value_;
    int pos_;
  };

  class SingleValue : public Value{
  public:
    SingleValue(std::string& str, int start, int end) : Value(SINGLE, start, end, str) {}
  };

  class StructValue : public Value {
  public:
    StructValue(int start, int end, int pos) : Value(STRUCT,start,end, std::string(""), pos) {}
 
   };


private:

  /// Throws an exception if the header does not match
  void checkHeader( void );
  
  /// Do a preprocessing parsing of the key
  void preprocessKey(const std::string& key);

  /// Reads a line from the stream, removes any non-printable letters
  std::string ReadLine(  );

  /// Reads the whole file into the _file_content
  void ReadFile( void );

  /// Reads a line, removes any non-printable letters
  void PutLine( const std::string& line );

  //void BuildLineVector( std::string& line, std::queue<std::string>& line_queue);

  /// Reads a line and removes all comments
  std::string ReadLineNoComments( );

  /// Continues to read the file until the delimiter is found
  std::string GetLineUntilDelimiter( char delimiter );

  /// Parses a string for values, returns a ConfigValue
  ConfigValue *ParseValue( Value *val, ConfigValue::ValueType type );

  /// Parses an array value
  ConfigValue *ParseArray( Value *val );

  /// Parses a structure value
  ConfigValue *ParseStruct( Value *val );

  ConfigValue *parseExpression(Value *val);

  
  ConfigValue *GetValue(int start_line);


/*! Extracts the value from line
  If the type of the value seems to be a struct then it will
  continue to read and try to find balancing {}
  */
  ConfigValue *ExtractValue(std::string& line, int start_line);

  ConfigIO::Value *GetString(std::string& line, int start_line, char delimiter);

  Value *GetArray(int start_line, char delimiter);
  ///
  Value *GetStruct(int start_line);

  /*! Checks so value is of the same type indicated by type
  * If not it trows a char *message
  */
  void ValidValue( Value *value, ConfigValue::ValueType& type );

  void preProcessLine(std::string& line);

  class LineDB {
  public: 
    LineDB() : last_accessed_(0), end_(true) {};
    void push_back(const std::string& line) { end_ = false; content_.push_back(line); }
    bool next() 
    { 
      assert(content_.size()); 

      if (last_accessed_ < ( content_.size()-1)) {
        last_accessed_++; 
        end_ = false;
      }
      else  {
        end_ = true;
      }
      return !end_;
    }
    bool end() { return end_; }

    bool valid(int i) { return (i >=0 && i < (int)content_.size()); }
    std::string& ref() 
    { 
      assert(content_.size()); 
      assert(last_accessed_ < content_.size()); 
      return  content_[last_accessed_];
    }
    std::string val() { assert(content_.size()); return  content_[last_accessed_];}
    bool set(unsigned pos) 
    { 
      //assert(content_.size()); 
      if (!content_.size())
        return false;

      if (pos < content_.size()) {
        last_accessed_=pos; 
        return true;
      }
      else
        return false;
    }
    void clear() { last_accessed_=0; content_.clear(); }
    std::size_t size() {return content_.size(); }

    int currLineNo() {return last_accessed_; }

  private:
    unsigned int last_accessed_;
    std::vector<std::string> content_;
    bool end_;
  };

  public:

  class IOState {
  public:

    /// Default constructor
    IOState();

    /*!
      Constructor.
      Opens filename for reading, throws an IOError exception on error
      \param filename - path to file to open for reading
    */
    IOState(const std::string& filename);
    
    /*!
      Constructor
      Uses in_data to initialize a strstream used for reading later on.
      \param title - Title of data, used in error messages
      \param in_data - String containing the data that will be parsed
    */
    IOState(const std::string& title, const std::string& in_data);

    /// Closes any opened files
    void finish() { if (!m_use_sstream) m_ifstream.close(); }

    bool haveSStream() const { return m_use_sstream; }

    int item_start; /// The last item started at line 
    int curr_line; /// Current line
  
    ///
    std::string m_filename; /// Name of the file
    std::string m_title;

    /// Return the active stream.
    std::istream& getInStream() 
    {
      if (haveSStream())
        return m_istrstream;
      else
        return m_ifstream;
    }
    std::ifstream m_ifstream;

    ///
    std::ofstream out;

  

    LineDB lineDB;

    /// Return the include path used when opening included files
    std::string getIncludePath() const { return m_include_path; }
    void setIncludePath(const std::string& path) { m_include_path = path; m_finished = true; }

    bool ok() { return m_finished; }
  private:
    bool m_finished;

    std::istringstream m_istrstream;
    bool m_use_sstream;

    std::string m_include_path;


  };

private:

	typedef std::vector<IOState *> IOStateVector_t;
  
  class IOStateStack { //: private  IOStateVector_t {
  public:
    ~IOStateStack();

    void pop();
    IOState *top() { if (size()) return *(m_vec.end()-1); //[size()-1]; 
      else throw InternalError(__FILE__, __LINE__, "Getting top of empty stack, serious stuff"); 
    return 0L; // Satisfies the compiler
    }
  
    void push(IOState *s) { m_vec.push_back(s); }
    size_t size() { return m_vec.size(); }
    std::string getIncludePath();
  private:
    IOStateVector_t m_vec;

  };


  IOStateStack _iostate_stack;
  typedef std::queue<IOState *> IOStateQueue_t;

  class IOStateQueue : public  IOStateQueue_t {
  public:
    void pop() { if (size()) delete top(); m_queue.pop(); }
    bool empty() const { return m_queue.empty(); }
    IOStateQueue_t::size_type size() const
		{	// test length of stack
		return (m_queue.size());
		}

    const IOStateQueue_t::value_type& top() const { return m_queue.front(); }
    void push(const IOStateQueue_t::value_type& v) { m_queue.push(v); }

    ~IOStateQueue() { while(!empty()) pop(); }

  private:
    IOStateQueue_t m_queue;

  };

  IOStateQueue _iostate_queue;
    
  inline IOState *currIOState() { return _iostate_stack.top();  }

  IOState _out_state;

  void popIOState() {  _iostate_stack.pop();  }
  void pushIOState(IOState *s) { _iostate_stack.push(s); }

  IOState *topIOState() { return _iostate_stack.top();  }
  size_t sizeIOStateStack() { return _iostate_stack.size(); }

  inline IOState *workQueue() { return _iostate_queue.top(); }
  void popWorkQueue() { _iostate_queue.pop(); }
  void pushWorkQueue(IOState *s) { _iostate_queue.push(s); }

  bool workLoad() { return sizeWorkQueue() !=0; }
  size_t sizeWorkQueue() { return _iostate_queue.size(); }

  std::string getIncludePath() { return _iostate_stack.getIncludePath(); }


 /// The string used for comments
  std::string _comment_string;

  /// The maximum number of lines a configfile can contain (just for precaution)  
  const unsigned int MAX_LINES;

  bool _have_header;

  ItemMap *_item_map;
  StructMap *_current_scope;

  /// The maximum number of included files (just to avoid recursive includes)
  const unsigned int MAX_INCLUDE_DEPTH;

  std::string _header;


  rcfg::ConfigScript _config_script;
  
  Preprocessor _preprocessor;

  #define CONFIGSCRIPT_FILE_PATH "CFG_FILE_PATH"

  public:
    class FilePathSingleton
    {
    public:
      /// Constructor  
      FilePathSingleton( void ) : m_filepathcontainer(CONFIGSCRIPT_FILE_PATH) {
        assert( !ms_Singleton && "Singleton already instantiated" );
        ms_Singleton = this;
      }

      // Destructor
      ~FilePathSingleton( void ) { 
        assert( ms_Singleton && "No singleton instantiated" );  
        ms_Singleton = 0;  
      }
   
      /// Return the singleton object
      static FilePathSingleton& getSingleton( void ) {  
        assert( ms_Singleton && "Singleton not instantiated" );  
        return ( *ms_Singleton );  
      }

      /// Return a pointer to the singleton object
      static FilePathSingleton* getSingletonPtr( void )  {  
        assert( ms_Singleton && "Singleton not instantiated" ); 
        return ( ms_Singleton );  
      }

      static rvrutils::FilePathContainer& getFilePath() { return getSingleton().m_filepathcontainer; }
    protected:

      ///
      static FilePathSingleton* ms_Singleton;
      rvrutils::FilePathContainer m_filepathcontainer;

    };
    #define FilePathContainer() ConfigIO::FilePathSingleton::getFilePath()
};

std::ostream&  operator<<(std::ostream& os, const ConfigIO::IOState* s);



/// A class for indicating an io error (EOF)
class CONFIGSCRIPT_API IOError : public std::exception {
public:
  ///
  IOError(const ConfigIO::IOState *s, const std::string& m ); 
  
  ///
  //virtual ~IOError() {};

};

/// A class for indicating parsing error
class CONFIGSCRIPT_API ParseError : public std::exception  {
public:
  ///
  ParseError(const ConfigIO::IOState *s, int start_line, int curr_line, const std::string& m) 
  { 
    std::ostringstream msg;
    msg << "ParseError: " << m << " (" << curr_line+1 << "), starting at (" << start_line+1 << ") " << s ;
    throw std::runtime_error(msg.str());
  }
};



#endif

/*------------------------------------------

* $Source: /cvsroot/replicantbody/replicantbody/include/rcfg/ConfigIO.h,v $
* $Revision: 1.1 $ 
* $Date: 2005/02/09 17:26:54 $
* $Author: deepone $ 
* $Locker:  $

* $Log: ConfigIO.h,v $
* Revision 1.1  2005/02/09 17:26:54  deepone
* vrutils and cfgscript put into new namespaces
*
* Revision 1.1.1.1  2005/02/06 06:53:00  deepone
* initial sourceforge checkin
*
* Revision 1.14  2003/12/17 09:28:50  andersb
* Made configscript a dll library under windows
* Fixed memory leaks.
* Fixed bug in processing included files in wrong order.
*
* Revision 1.13  2003/12/16 11:34:43  andersb
* *** empty log message ***
*
* Revision 1.12  2003/12/05 10:43:37  deepone
* *** empty log message ***
*
* Revision 1.11  2003/12/02 15:40:12  andersb
* Added scoped struct access within the scripts.
*
* Revision 1.10  2003/10/28 19:38:15  andersb
* Modified the use of stack<> to make it work under .NET2003
*
* Revision 1.9  2003/04/24 06:20:58  andersb
* Added support for Array of Strings
*
* Revision 1.8  2003/03/04 11:19:53  Anders Backman
* Fixed bug that required data beginning on the same line as the key,
* Added a search path feature. Default the environment variable CFG_FILE_PATH is enquired
* for a list of directories to search config files in.
*
* Revision 1.7  2003/03/03 12:16:51  Anders Backman
* removed more std::ends
*
* Revision 1.6  2003/02/27 13:27:52  andersb
* Moved over to sstream instead of strstream
*
* Revision 1.5  2003/02/27 13:08:56  andersb
* *** empty log message ***
*
* Revision 1.4  2003/01/27 13:43:49  andersb
* Added ReadString for parsing a string instead of a file.
*
* Revision 1.3  2002/12/17 07:25:46  Anders Backman
* Minor bugfix getting assert when not locating a #include file.
*
* Revision 1.2  2002/10/28 21:47:48  andersb
* Fixed include bug under linux
*
* Revision 1.1  2002/10/22 12:30:54  Anders Backman
* Moved around include files.
*
* Revision 1.17  2002/10/14 14:22:31  Anders Backman
* Added VC6 project files again,
*
* Revision 1.16  2002/09/23 06:33:33  andersb
* no message
*
* Revision 1.15  2002/08/06 11:34:49  andersb
* Rewrote Array class so it hides the std::vector. This solved problem
* when using method at(i) with overloaded operator[] under unix.
*
* Revision 1.14  2002/08/03 09:49:12  andersb
* Now also #include works but not finished and polished.
*
* Revision 1.13  2002/07/21 21:55:15  andersb
* Added preprocessor directives:
* #define <macro> <value>
* #ifdef <macroname>
* #else
* #endif
* Also removed MSv6.0 project files.
* Preprocessor.h and Preprocessor.cpp is added.
*
* Revision 1.12  2002/07/09 21:28:46  andersb
* Fixed case bug in Expressions allowing only lowercase variables.
* Added expressions in arrays too.
* Added randInterval(min,max) function in Expressions allowing expressions like:
*
* Position [randInterval(-10,10) randInterval(-10,10) randInterval(0,3)]
*
* Revision 1.11  2002/07/04 23:08:47  andersb
* Added expressions.
* This helps alot writing scripts, where math expressions is available.
* Most standard functions: ceil, floor, sin, cos, etc is available.
* Also pi (3.1415...) and e ( 2.718 ) as constants.
* Scope is also implemented. (a variable name is backtraced upwards from the current
* scope.
*
* Revision 1.10  2002/07/03 18:54:58  andersb
* no message
*
* Revision 1.9  2002/07/03 10:38:14  andersb
* no message
*
* Revision 1.8  2002/04/10 14:01:04  andersb
* Fixed some compiler warnings.
*
* Revision 1.7  2002/04/09 09:04:48  andersb
* Major rewrite of ConfigIO.cxx.
* Now supports multiline strings and arrays.
*
* Revision 1.6  2002/02/01 13:35:20  andersb
* Now fully updated to support recursive structs.
* Still problems with case sensitivity.
*
* Revision 1.1.1.1  2002/01/02 13:45:29  andersb
* A new version of Configscript using namespace rcfg.
* Also supporting recursive structures.
*
* Revision 1.5  2001/11/14 13:12:46  andersb
* New features added:
* Hiearchial structures supported.
* Possible to use a mapID when opening to
* support multiple open configscripts.
*
* Revision 1.4  2001/08/29 09:23:35  andersb
* Now using the old iostream.h library to get it to work together with
* CommonC++.
*
* Revision 1.3  2001/08/01 07:11:19  andersb
* Major update
*
* Revision 1.2  2001/04/25 12:45:00  andersb
* Added makefiles for the IRIX platform.
*
* Revision 1.1.1.1  2001/04/25 10:56:27  andersb
* no message
*
* Revision 1.8  2000/11/14 16:04:25  andersb
* Getting there...
*
* Revision 1.7  2000/11/06 16:04:18  andersb
* Load scenario + load view is working
*
* Revision 1.6  2000/11/03 14:48:00  andersb
* no message
*
* Revision 1.5  2000/10/05 13:59:58  andersb
* no message
*
* Revision 1.4  2000/10/05 10:27:07  andersb
* no message
*
* Revision 1.3  2000/10/03 09:21:23  andersb
* no message
*
* Revision 1.2  2000/10/03 07:44:37  andersb
* Using DOC++
*
* Revision 1.1  2000/10/03 07:02:39  andersb
* no message
*
* Revision 1.4  2000/10/02 09:31:13  andersb
* A Working SGI version of the ConfigIO files.
*
* Revision 1.3  2000/10/02 09:08:31  andersb
* no message
*
* Revision 1.2  2000/09/26 14:14:32  andersb
* no message
*
* Revision 1.1.1.1  2000/09/26 09:05:46  andersb
* no message
*

--------------------------------------------*/
