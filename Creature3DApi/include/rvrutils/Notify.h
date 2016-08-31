#ifndef __rvrutils_Notify_h__
#define __rvrutils_Notify_h__

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

#include <CRCore/ref_ptr.h>
#include <CRCore/Referenced.h>

#include "rvrutils/export.h"

namespace rvrutils {

// forward declaration
class Notify;

/// Allow messages of different classes to be turned on and of independently
/** This can be used eg to enable/disable messages from task code,
 * action code, core core, or whatever you might want. **/
class VRUTILS_API NotifyClass : public CRCore::Referenced {
    /// Notify is the only class that can create notify classes
    friend class Notify;
    private:
        /// Name of the notify class
        const std::string m_name;
        /// Status of notify class
        bool m_enabled;
    private:
        /// Constructor - only to be called from Notify
        NotifyClass( const std::string& name ) : m_name(name),m_enabled(true){};
        /// Enable
        void enable() { m_enabled = true; };
        /// Disable
        void disable() { m_enabled = false; };
        /// Disable
        void toggle() { m_enabled = !m_enabled; };
        /// Get enabled status
        bool getEnabled() const { return m_enabled; };
    public:
        /// Get name of notify class
        const std::string& getName() { return m_name; };
};
#ifdef _WIN32 
  #ifdef ERROR
    #undef ERROR
  #endif
#endif

/// Notify class containing everything to control console output
class VRUTILS_API Notify : public CRCore::Referenced {
    public:
        
        /// Notify levels
        enum NotifyLevel {
            NEVER,
            DEBUG_VERBOSE,
            DEBUG,
            VERBOSE,
            INFO,
            NOTICE,
            WARNING,
            ERROR,
            FATAL,
            ALWAYS,
            INVALID
        };
        
    private:
        /// Notify classes map
        typedef
            std::map<std::string,CRCore::ref_ptr<NotifyClass>,std::less<std::string> >
            NotifyClassMap;
        /// Notify classes status map
        typedef
            std::map<NotifyClass*,bool,std::less<NotifyClass*> >
            NotifyClassStatusMap;
        
        /// Notify level labels
        std::vector<std::string> m_notify_labels;
        /// NULL output stream
        std::ofstream *m_null_stream;
        /// Lowest level that should be output
        NotifyLevel m_notify_level;
        /// Notify classes
        NotifyClassMap m_notify_classes;

    protected:

        /// Destructor
        virtual ~Notify();

        /// Constructor
        Notify();

    public:

        /// Get instance
        static Notify* instance()
        {
            static CRCore::ref_ptr<Notify> n_instance = new Notify();
            return n_instance.get();
        };

        /// Print notify message
        std::ostream& notify(
                NotifyLevel level = NOTICE, NotifyClass* nclass = NULL );

        /// Get null stream
        std::ofstream& getNullStream() { return *m_null_stream; };

        /// Get (and create if necessary) notify class
        NotifyClass* getNotifyClass( const std::string& name ) {
            if ( m_notify_classes.find( name ) == m_notify_classes.end() ) {
                m_notify_classes[name] = new NotifyClass( name );
            }
            return m_notify_classes[name].get();
        };

        /// Get single (created) notify class
        /** You take care of this one **/
        NotifyClass* getSingleNotifyClass() {
            return new NotifyClass( "single" );
        };

        /// Set notify level
        void setNotifyLevel( NotifyLevel level )
        { m_notify_level = level; };

        /// Get notify level
        NotifyLevel getNotifyLevel()
        { return m_notify_level; };
            
        /// Get label for notify level
        const std::string& getNotifyLabel( NotifyLevel level )
        { return m_notify_labels[level]; };

        /// Toggle notify class
        void toggleNotifyClass( const std::string& name ) {
            getNotifyClass( name )->toggle();
        };

        /// Enable notify class
        void enableNotifyClass( const std::string& name ) {
            getNotifyClass( name )->enable();
        };

        /// Disable notify class
        void disableNotifyClass( const std::string& name ) {
            getNotifyClass( name )->disable();
        };

        /// Get notify class enabled status
        bool getNotifyClassEnabled( const std::string& name ) {
            return getNotifyClass( name )->getEnabled();
        };

};

inline std::ostream&
notify( Notify::NotifyLevel level = Notify::NOTICE, NotifyClass* nclass = NULL )
{
    return Notify::instance()->notify( level, nclass );
}

} // namespace end

using rvrutils::notify;


#define NOTIFY(level)   ::rvrutils::Notify::instance()->notify( level ) << __FILE__":" << __LINE__ << " "

#define NOTIFY_NEVER    ::rvrutils::Notify::instance()->notify( ::rvrutils::Notify::NEVER ) << __FILE__":" << __LINE__ << " "
#define NOTIFY_DEBUG_VERBOSE    ::rvrutils::Notify::instance()->notify( ::rvrutils::Notify::DEBUG_VERBOSE ) << __FILE__":" << __LINE__ << " "
#define NOTIFY_DEBUG    ::rvrutils::Notify::instance()->notify( ::rvrutils::Notify::DEBUG ) << __FILE__":" << __LINE__ << " "
#define NOTIFY_VERBOSE  ::rvrutils::Notify::instance()->notify( ::rvrutils::Notify::VERBOSE ) << __FILE__":" << __LINE__ << " "
#define NOTIFY_INFO     ::rvrutils::Notify::instance()->notify( ::rvrutils::Notify::INFO ) << __FILE__":" << __LINE__ << " "
#define NOTIFY_NOTICE   ::rvrutils::Notify::instance()->notify( ::rvrutils::Notify::NOTICE ) << __FILE__":" << __LINE__ << " "
#define NOTIFY_WARNING  ::rvrutils::Notify::instance()->notify( ::rvrutils::Notify::WARNING ) << __FILE__":" << __LINE__ << " "
#define NOTIFY_ERROR    ::rvrutils::Notify::instance()->notify( ::rvrutils::Notify::ERROR ) << __FILE__":" << __LINE__ << " "
#define NOTIFY_FATAL    ::rvrutils::Notify::instance()->notify( ::rvrutils::Notify::FATAL ) << __FILE__":" << __LINE__ << " "
#define NOTIFY_ALWAYS   ::rvrutils::Notify::instance()->notify( ::rvrutils::Notify::ALWAYS ) << __FILE__":" << __LINE__ << " "

#endif // __rvrutils_Notify_h__
