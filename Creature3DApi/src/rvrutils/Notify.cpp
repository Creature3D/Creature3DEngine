
#include <rvrutils/Notify.h>

#include <stdlib.h>

using namespace rvrutils;

Notify::Notify()
{
    m_notify_labels.push_back( "NEVER" );
    m_notify_labels.push_back( "DEBUG_VERBOSE" );
    m_notify_labels.push_back( "DEBUG" );
    m_notify_labels.push_back( "VERBOSE" );
    m_notify_labels.push_back( "INFO" );
    m_notify_labels.push_back( "NOTICE" );
    m_notify_labels.push_back( "WARNING" );
    m_notify_labels.push_back( "ERROR" );
    m_notify_labels.push_back( "FATAL" );
    m_notify_labels.push_back( "ALWAYS" );

#if defined(WIN32) && !defined(__CYGWIN__)
    m_null_stream = new std::ofstream ("nul");
#else
    m_null_stream = new std::ofstream ("/dev/null");
#endif

    char *level_cstr = getenv( "RMIND_NOTIFY_LEVEL" );
    
    m_notify_level = INVALID;
    if ( level_cstr ) {
        std::string level_str = std::string( level_cstr );
        for ( unsigned int i = 0; i < m_notify_labels.size(); i++ ) {
            if ( level_str == m_notify_labels[i] ) {
                m_notify_level = NotifyLevel( i );
                break;
            }
        }
    }

    if ( m_notify_level == INVALID ) {
        m_notify_level = NOTICE;
    }

}

Notify::~Notify()
{
}

std::ostream&
Notify::notify( NotifyLevel level, NotifyClass* nclass )
{
    if ( ( level >= m_notify_level) && ( !nclass || nclass->getEnabled() ) ) {
        std::cerr << m_notify_labels[level] << ": ";
        return std::cerr;
    } else {
        return *m_null_stream;
    }
}

