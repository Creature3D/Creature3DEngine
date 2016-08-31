// $Id: ConfigScriptOO.h,v 1.1 2005/02/09 17:26:54 deepone Exp $

#ifndef __CONFIGSCRIPTOO_H__
#define __CONFIGSCRIPTOO_H__


#ifdef _WIN32
  #pragma warning(disable:4786) // Disable warnings about long names
  #include "rvrutils/os/memory_debug.h"
#endif

#include <string>
#include <stack>
#include <vector>
#include "rcfg/ItemMap.h"
#include "rcfg/Item.h"
#include <sstream>
#include <stdexcept>

#ifdef _THREAD_SAFE
#include <cc++/thread.h>
#endif

#include "rcfg/export.h"

//Modified by Îâ²Æ»ª For Creature3D Use
const std::string CreScriptParserHeader = "# Creature3D Script Parser v1.0";
//const std::string CreScriptParserHeader = "";
namespace rcfg {
/// Class for parsing and accessing data in configuration scripts. 

/*! 
 * Instead of setting parameters for applications explicitly this class helps you
 * by letting you setting all parameters in a configuration script and then parsing the 
 * script, and later letting the application accessing the data in the script in a typesafe way.
 * The fileformat of the script is:

 <key> <item>

 Where <key> can contain alphanumering letters including _ (underscore).
 <key> is the "hook" for accessing the data <item> later on.
 There can be many <key> with the same name. All Get and Return methods has a index for accessing 
 the nth <key>.

<item> Is describing the data. Valid types are: 
integer, float, string, struct and vector

integer and float is obvious, just make sure you think of the type for default parameters.

string is declared as "a string" and must start and end on the same line.
vector is declared as [float or int ... ] can be as many as you want, but must start and 
end on the same line.

struct is declared as { <key> <item> ... } and can span over as many lines as you want. Example:

Sven {
  age 23
  Children {
    child {
      name "john"
      age 3
      sex "male"
    }
    child {
      name "Eva"
      age 2
      sex "femal"
    }
  }
}
*/
class  CONFIGSCRIPT_API ConfigScript {

  public:

    /// The available types of items
    enum Type {INT, FLOAT, FLOAT_ARRAY, STRUCT, STRING, STRING_ARRAY, NO_TYPE };

    
    /*!
    * Arguments to ConfigStructPop.
    * ONE - Pop only one level
    * ALL - Pops all levels and empties the stack of pushed structs
    */
    enum StructSelection { ONE=0, ALL=1};

    /*!
    * Constructor
    * Opens the ConfigScript file filename.
    * Checks that the first line in the file contains header
    * Parses the file for configuration data.
    * Throws and std::runtime_error exception in case of an error
    * \param filename - The file to open
    * \param header - The header of the file that will be opened. This header must match the one in the file
    * \param map_id - Associates an id with the opened configfile for later retrieval with ConfigSetActive
    * \return bool - true if opening and parsing is successful.
    */
    ConfigScript(const std::string& filename, const std::string& header=std::string(""));

    /*!
      Uses the stream in data to parse and create items for later access.
      Can be used instead of opening a file and parsing that.
      Will append the data read from the the data string to the current map

      \param title - The title of the stream of data to be parsed. Can be used to get better information
      from error messages.
      \param data - A stream of data which is the script to be parsed.
      \param header - The first line in any script to be parsed, including the stream of data.
    */
    void ReadString(const std::string& title, const std::string& data, const std::string& header=std::string(""));
    
    /*!
      Constructor
      Uses the parameter item_map to search for data.
      Dont parses any files, that is assumed to be done when creating the item_map.
      \param item_map - The Item map containing data that will be parsed
    */
    ConfigScript(ItemMap* item_map);

    /*! 
    * Constructor Does almost nothing. Requires that  Open is called later on.
    */
    ConfigScript();

    /*!
    * Opens the ConfigScript file filename.
    * Checks that the first line in the file contains header
    * Parses the file for configuration data
    * \param filename - The file to open
    * \param header - The header of the file that will be opened. This header must match the one in the file
    * \param map_id - Associates an id with the opened configfile for later retrieval with ConfigSetActive
    * \return bool - true if opening and parsing is successful.
    */
    bool Open(const std::string& filename, const std::string& header=CreScriptParserHeader);

    
    /*!
      Uses an existing ItemMap for parsing.


    * \param item_map - The map of keys and data (items) that will be used for this instance of ConfigScript
    * \return bool - always true 
    */
    bool Open(ItemMap *item_map,bool allocat = false) { Close(); _allocated_item_map = allocat; _item_map = item_map; return true;}
    
    /*!
      This method takes the existing data and writes it to a named file as a structured
      configscript file.

      \param fileName - Name of the file the data will be written to.
      \param header - The header (first line) that will be written to the file.
      \return bool - true if writing to the file was successful.
    */
    bool Write( const std::string& fileName, const std::string& header=CreScriptParserHeader );    

    /*!
     * Explicitly closes the script. The file is really closed earlier the mearly deallocates any 
     * allocated memory.
     * \return bool - true if it can be closed (is open).
     */
    bool Close();

    /*!
     * \return bool - true if script is opened and parsed with no problems.
    */
    bool IsOpen();
    /*! 
    * Destructor. 
    */
    ~ConfigScript() { if (Check()) Close(); };

    /*!
    * Any error message set during a call to any configfunctions can be retreived with ConfigGetLastError
    * \return std::string - A message containing the last set error message.
    */
    std::string GetLastError( void );

    /*!
    * Pops the stack of opened structs
    * \param selection - If == ONE only one level is popped from the stack.
    * If == ALL all pushed structs are popped
    * \return bool - true if stack before the call to ConfigStructPop was not empty otherwise false.
    */
    bool Pop( StructSelection selection=ONE ); 

    
    /*!
    * Pushes a given (in key) struct to the opened stack. This will cause all successive data retrieves to 
    * work from the opened struct and beneath.
    * i.e. PushOpen("aaa"); Will work on the struct aaa { ... }
    * \param key - The name of the struct to be opened
    * \param index - If the key exists several times (multiple key) index selects which one to open (1 = first)
    * \return true if the struct key was found.
    */
    bool Push(const std::string& key, int index=1); 


    /*!
    * Returns true if the given key exists
    * \param key - The name of the key to search for
    * \param index - If the key exists several times (multiple key) index selects which one to open (1 = first)
    * \return true if the key was found.
    */
    bool Exist( std::string key, Type type, int index=1); 

    /*! Overloaded function to retrieve data associated to key read from configfile
    * Returns false when key is either missing or of wrong type
    */
    bool Get(const std::string& key, int& data, int index=1); 
 
    /*! Overloaded function to retrieve data associated to key read from configfile
        Returns false when key is either missing or of wrong type
    */
    bool Get(const std::string& key, float& data, int index=1); 
    
    /*! Overloaded function to retrieve data associated to key read from configfile
        Returns false when key is either missing or of wrong type
    */
    bool Get(const std::string& key, std::string&, int index=1); 
    
    /*! Overloaded function to retrieve data associated to key read from configfile
        Returns false when key is either missing or of wrong type
    */
    bool Get(const std::string& key, std::vector<float> &data, int index=1); 

    /*! Overloaded function to retrieve data associated to key read from configfile
        Returns false when key is either missing or of wrong type
    */
    bool Get(const std::string& key, std::vector<std::string> &data, int index=1); 

    /*! Overloaded function to retrieve data associated to key read from configfile
        Returns false when key is either missing or of wrong type
    */
    bool Get(const std::string& key, Expression &data, int index=1); 
    
    /*! Overloaded function to retrieve data associated to key read from configfile
        Returns the value if found, otherwise the specified default value will be returned.
    */
    std::string Return(const std::string& key, const std::string& def, int index=1); 
  
    /*! Overloaded function to retrieve data associated to key read from configfile
        Returns the value if found, otherwise the specified default value will be returned.
    */
    int Return(const std::string& key, int def, int index=1); 
  
    /*! Overloaded function to retrieve data associated to key read from configfile
        Returns the value if found, otherwise the specified default value will be returned.
    */
    float Return(const std::string& key, float def, int index=1); 
    
    /*! Overloaded function to retrieve data associated to key read from configfile
        Returns the value if found, otherwise the specified default value will be returned.
    */
    std::vector<float> Return(const std::string& key, const std::vector<float> &def, int index=1); 

    /*! Overloaded function to retrieve data associated to key read from configfile
        Returns the value if found, otherwise the specified default value will be returned.
    */
    std::vector<std::string> Return(const std::string& key, const std::vector<std::string> &def, int index=1); 

    /*! Overloaded function to retrieve data associated to key read from configfile
        Returns the value if found, otherwise the specified default value will be returned.
    */
    Expression Return(const std::string& key, const Expression& def, int index=1);

    /*!
      Returns the type of a named key of a given index.
      If the key does not exist, it returns NO_TYPE
    */
    Type GetType(const std::string& key, int index=1);

    /*!
      Add a macro definition to the list of macros. This macro can then be used when parsing 
      configscripts and tested for with #ifdef and also used in expressions.
      It has to be called before the Open method, otherwise it is useless, the parsing is already been done.
      It cant be used with the Constructor that calls Open ConfigScript(const std::string& filename)
      because that constructor calls Open.
    */
    void AddMacro(const std::string& macro, const std::string& value);

    
    /*! 
      Add a key with its associated data (int) to the current scope
      \return the index of the newly added key
    */
    int Add(const std::string& key, const int& data); 
 
    /*! 
      Add a key with its associated data (int) to the current scope
      \return the index of the newly added key
    */
    int Add(const std::string& key, const float& data); 
    
    /*! 
      Add a key with its associated data (int) to the current scope
      \return the index of the newly added key
    */
    int Add(const std::string& key, const std::string&); 
    
    /*! 
      Add a key with its associated data (int) to the current scope
      \return the index of the newly added key
    */
    int Add(const std::string& key, const std::vector<float> &data); 

    /*! 
      Add a key with its associated data (int) to the current scope
      \return the index of the newly added key
    */
    int Add(const std::string& key, const std::vector<std::string> &data); 

    /*! 
      Add a key with its associated data (int) to the current scope
      \return the index of the newly added key
    */
    int Add(const std::string& key, const Expression &data); 

    /*!
      Add an empty struct named key to the map at the current position
      \return the index of the newly added key
    */
    int Add(const std::string& key);


    /*!
      Returns the top of the stack of pushed structures.
      This is the scope in which any Return and get Call would operate.
    */
    std::string CurrentScope();

    /*!
      Specify the environment variable used for extracing the path to scripts.
      \param env_variable - The name of the environmentvariable containing the list of directories.
      \return bool - true if the environment variable can be found, otherwise false.
    */
    static bool setSearchPathEnvironmentVariable(const std::string& env_variable); 

    /*!
      Set the list of directories to search for scripts in.
      Under win32 a semicolon separated list, under unix a colon ':' separated list.
    */
    static void setSearchPathString(const std::string& path_string); 

	/////////ÐÞ¸Ä
	/*! Overloaded function to retrieve data associated to key read from configfile
	* Returns false when key is either missing or of wrong type
	*/
	bool Modify(const std::string& key, const int& data, int index=1); 

	/*! Overloaded function to retrieve data associated to key read from configfile
	Returns false when key is either missing or of wrong type
	*/
	bool Modify(const std::string& key, const float& data, int index=1); 

	/*! Overloaded function to retrieve data associated to key read from configfile
	Returns false when key is either missing or of wrong type
	*/
	bool Modify(const std::string& key, const std::string&, int index=1); 

	/*! Overloaded function to retrieve data associated to key read from configfile
	Returns false when key is either missing or of wrong type
	*/
	bool Modify(const std::string& key, const std::vector<float> &data, int index=1); 

	/*! Overloaded function to retrieve data associated to key read from configfile
	Returns false when key is either missing or of wrong type
	*/
	bool Modify(const std::string& key, const std::vector<std::string> &data, int index=1); 

	/*! Overloaded function to retrieve data associated to key read from configfile
	Returns false when key is either missing or of wrong type
	*/
	bool Modify(const std::string& key, const Expression &data, int index=1); 

	////Delete
	//bool Delete(const std::string& key,int index=1);
	////Replace
	//bool Replace(const std::string& key,const int& data,int index=1);
	//bool Replace(const std::string& key,const float& data,int index=1);
	//bool Replace(const std::string& key,const std::string& data,int index=1);
	//bool Replace(const std::string& key,const std::vector<float>& data,int index=1);
	//bool Replace(const std::string& key,const std::vector<std::string>& data,int index=1);
	//bool Replace(const std::string& key,const Expression& data,int index=1);
	ItemMap* GetItemMap();
  private: 
      typedef std::pair<std::string, StructMapItem *> StructStackContent;
  public:
  
    // Class for storing the current Push/Pop scope
    class Scope : private std::deque<StructStackContent> {
    public:
      friend class ConfigScript;
      Scope() : std::deque<StructStackContent>() {}
      Scope(const Scope& s) {
        if (this == &s)
          return;

        *this = s;
      }

      Scope& operator=(const Scope& s) {
        if (this == &s)
          return *this;
    
        this->clear();
        Scope::const_iterator it;
        for(it=s.begin(); it != s.end(); it++) {
          this->push_back(*it);
        }
        return *this;
      }

      private:
        StructStackContent top() { return std::deque<StructStackContent>::front(); }
        void push(StructStackContent& s) { std::deque<StructStackContent>::push_front(s); }
        void pop() { std::deque<StructStackContent>::erase(std::deque<StructStackContent>::begin()); }

      }; // Scope class

  public:

   
    /*! Return the current Push/Pop scope
      By storing the scope externally it is possible to manipulate it
      and restore it later on.
      \returns Scope
    */
    const Scope GetScope() const { return _struct_stack; }

    /*!
      Set scope to be the current Push()/Pop() scope.
    */
    void RestoreScope(const Scope& scope) { _struct_stack = scope; }

    /*!
      From a point separated key string (a.b.c.d) this method generates a vector of strings where each
      item in the vector contains the keys i.e. [a, b, c, d].
      \returns the size of the vector. 1 means that no . were found at all
    */
    static std::size_t CreateKeyVector( std::string str, std::vector<std::string>& vec);

  private:

    ConfigValue::ValueType ToInternalType(Type type) const;

    Type ConfigScript::FromInternalType(ConfigValue::ValueType type) const;



      typedef std::map<std::string, std::string> MacroMap;
      typedef MacroMap::const_iterator MacroMapIterator;
      typedef MacroMap::value_type MacroMapType;

      MacroMap _macro_map;

      enum Status { OK, INVALID_MAP_ID, INVALID_KEY, INVALID_TYPE, INVALID_MEMBER, MISSING_KEY, INTERNAL_ERROR };

      Scope _struct_stack;

      /// Contains the last error message
      std::string _last_error;

      std::ostringstream msg_str_;

      ItemMap *_item_map;
      bool _allocated_item_map;

      /// Mutex used for secure access from several threads
#ifdef _THREAD_SAFE
      ost::Mutex _io_mutex;
#endif

      ConfigScript::Status StructAccess(const std::string& str, std::string& key, std::string& struct_key,
                    ConfigValue* &result, unsigned int nth=1); 
	  /*ConfigScript::Status StructAccess_I(const std::string& str, std::string& key, std::string& struct_key,
		  ItemMapIterator& result, unsigned int nth=1); */

      bool Check();

    template <class T>
    int getNumberofItems(const std::string& key, T *data)
    {
      int idx = 0;

      std::pair<typename T::iterator, typename T::iterator> range;
      range = data->equal_range(key);

      for(typename T::iterator it=range.first; it != range.second; it++)
        idx++;

      return idx;
    }
    
    template <class T>
    int addItem(const std::string& key, T *value)
    {
      int idx=0;

      // Are we somewhere deep into the tree of the data?
      if (_struct_stack.size()) {
      // Use the current scope
    
        StructMap *smptr=0; 
        smptr = ((StructItem *)_struct_stack.top().second)->GetVal();

        idx = getNumberofItems<StructMap>(key, smptr);

        smptr->insert(std::make_pair(key, value));

      } // No we are at the root
      else {
        idx = getNumberofItems<ItemMap>(key, _item_map);
        _item_map->insert(std::make_pair(key, value));
      }
      return idx+1;
    }

  }; // class ConfigScript

  inline bool ConfigScript::Check() {   
    if (!_item_map) { 
      _last_error = "Class is unitialized. The Open method must be executed";
      return false;
    }
    return true;
  }

  /// The constructor does a Push() and the destructor automatically cals Pop()
  
  /*!
    The purpose of this class is to automatically manage a pop of a ConfigScript object
    when the scope of a ScopedPush is ended. In this way an exception can be thrown anywhere 
    in the scope, and the pop method will still be managed by this class, not leaving a
    dangling state of a ConfigScript object.
  */
  class CONFIGSCRIPT_API  ScopedPush {
  public:
    
    /*!
      Constructor.
      \param rcfg - The ConfigScript object that will be managed
      \param id - The id of the struct that will be pushed
      \param n - The level of the struct (n:th struct)
      \param will_throw - If true this constructor will throw a runtime_error if the struct is not found.
    */
    ScopedPush(ConfigScript& rcfg, const std::string& id, unsigned int n=1, bool will_throw=false) : m_cfg(rcfg), m_pushed(false) {
      if (!m_cfg.Push(id, n)) {
        if (will_throw)      
          throw std::runtime_error(m_cfg.GetLastError());
        else
          return;
      }
      m_pushed = true;
    }

    /// Returns true if the constructor managed to successfully push the named struct
    bool valid() { return m_pushed; }

    /// Destructor, will pop the pushed struct
    virtual ~ScopedPush() {
      if (m_pushed)
        m_cfg.Pop();
    }
  
  private:
    ConfigScript &m_cfg;
    bool m_pushed;
 

  }; // class ScopedPush 
  
 
  /// Class that stores the current state of the Scope at construction and restores it at destruction
  
  /*!
    The purpose of this class is to automatically store the current scope of a ConfigScript object
    and to restore it at destruction of this class.
  */
  class CONFIGSCRIPT_API  ScopeRestore {
  public:
    
    /*!
      Constructor.
      \param rcfg - The ConfigScript object that will be managed
      \param id - The id of the struct that will be pushed
      \param n - The level of the struct (n:th struct)
      \param will_throw - If true this constructor will throw a runtime_error if the struct is not found.
    */
    ScopeRestore(ConfigScript& rcfg) : m_cfg(rcfg) {
      m_scope = m_cfg.GetScope();
    }

    /// Destructor, will pop the pushed struct
    virtual ~ScopeRestore() {
      m_cfg.RestoreScope(m_scope);
    }
  
  private:
    ConfigScript &m_cfg;
    ConfigScript::Scope m_scope;
  }; // Class ScopeRestorer

}; //namespace rcfg

#endif // __CONFIGSCRIPTOO_H__

/*------------------------------------------

* $Source: /cvsroot/replicantbody/replicantbody/include/rcfg/ConfigScriptOO.h,v $
* $Revision: 1.1 $ 
* $Date: 2005/02/09 17:26:54 $
* $Author: deepone $ 
* $Locker:  $

* Description: C Wrapper to simplify the use of Config script utility

  VRlab, Ume?University, 2001, Anders Backman


* $Log: ConfigScriptOO.h,v $
* Revision 1.1  2005/02/09 17:26:54  deepone
* vrutils and cfgscript put into new namespaces
*
* Revision 1.1.1.1  2005/02/06 06:53:00  deepone
* initial sourceforge checkin
*
* Revision 1.22  2004/03/15 13:42:29  andersb
* Minor cleanup
*
* Revision 1.21  2004/03/14 12:50:19  andersb
* Added ScopeRestore class.
* Added ConfigScript::GetScope and ::RestoreScope methods
*
* Revision 1.20  2003/12/17 15:04:46  deepone
* *** empty log message ***
*
* Revision 1.19  2003/12/17 09:28:50  andersb
* Made configscript a dll library under windows
* Fixed memory leaks.
* Fixed bug in processing included files in wrong order.
*
* Revision 1.18  2003/12/16 11:34:44  andersb
* *** empty log message ***
*
* Revision 1.17  2003/12/15 15:31:11  andersb
* Adding write to configscript
*
* Revision 1.16  2003/12/05 10:43:37  deepone
* *** empty log message ***
*
* Revision 1.15  2003/12/02 15:40:13  andersb
* Added scoped struct access within the scripts.
*
* Revision 1.14  2003/11/19 19:14:31  andersb
* Added GetType method for ConfigScript. Returns the type of a named item
*
* Revision 1.13  2003/10/31 13:24:37  andersb
* no message
*
* Revision 1.12  2003/09/10 12:37:50  andersb
* Changed Pusher to ScopePush (better name)
*
* Revision 1.11  2003/05/16 11:59:01  andersb
* *** empty log message ***
*
* Revision 1.10  2003/05/12 06:59:09  Anders Backman
* Added Pusher class for automatic Push/Pop of a structure.
*
* Revision 1.9  2003/04/24 06:20:58  andersb
* Added support for Array of Strings
*
* Revision 1.8  2003/03/04 11:19:53  Anders Backman
* Fixed bug that required data beginning on the same line as the key,
* Added a search path feature. Default the environment variable CFG_FILE_PATH is enquired
* for a list of directories to search config files in.
*
* Revision 1.7  2003/02/27 14:02:56  andersb
* Works under gcc3.2
*
* Revision 1.6  2003/02/27 13:27:53  andersb
* Moved over to sstream instead of strstream
*
* Revision 1.5  2003/02/11 07:11:56  andersb
* no message
*
* Revision 1.4  2003/01/27 13:43:50  andersb
* Added ReadString for parsing a string instead of a file.
*
* Revision 1.3  2002/12/03 09:50:13  Anders Backman
* no message
*
* Revision 1.1  2002/10/22 12:30:55  Anders Backman
* Moved around include files.
*
* Revision 1.13  2002/08/03 09:49:12  andersb
* Now also #include works but not finished and polished.
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
* Revision 1.10  2002/07/03 10:38:14  andersb
* no message
*
* Revision 1.9  2002/04/12 08:49:46  andersb
* Peter found a bug in ConfigScriptOO.cxx made it crash when searching for
* indexed items with Get and Return methods.
*
* Revision 1.8  2002/04/10 14:01:04  andersb
* Fixed some compiler warnings.
*
* Revision 1.7  2002/04/09 09:04:48  andersb
* Major rewrite of ConfigIO.cxx.
* Now supports multiline strings and arrays.
*
* Revision 1.6  2002/04/06 21:05:00  andersb
* Found one huge bug for the Struct parser, not solved yet.
* Should rewrite the whole ConfigIO.cxx
*
* Revision 1.5  2002/04/05 18:04:26  andersb
* Added Exists method.
*
* Revision 1.4  2002/03/13 14:08:19  andersb
* no message
*
* Revision 1.3  2002/03/13 09:03:54  andersb
* Compile with -D_THREAD_SAFE if thread safety is required. This also requires
* CommonC++ version 1.9.3 or higher.
*
* Also now Case sensitive for all items.
*
* Revision 1.2  2002/02/22 11:04:14  andersb
* Also working under linux
*
* Revision 1.1  2002/02/22 10:42:05  andersb
* ConfigScriptOO.h added which now implements a OO version of ConfigScript.
* Please use this in favour from the old C-version.
* The old ConfigScriptGetData-kind of version is now only a wrapper ontop of the OO version.
*

--------------------------------------------*/
