#ifndef __rvrutils__CODELINE_EXCEPTION_H
#define __rvrutils__CODELINE_EXCEPTION_H


#include <string>
#include <stdexcept>
#include <sstream>
#include <iostream>
#include <cassert>

#include "rvrutils/export.h"

namespace rvrutils {

/// This class is for sharing static members
class VRUTILS_API BaseCodeLineException
{
    protected:
        static bool sm_notify_on_exception;
        static bool sm_dump_on_exception;
        static bool sm_print_file;
        static bool sm_print_line;
        static bool sm_short_file;
        static unsigned int sm_max_file_length;
    public:
        static void setNotifyOnException( bool value )
        { sm_notify_on_exception = value; };
        static void setDumpOnException( bool value )
        { sm_dump_on_exception = value; };
        static void setPrintFile( bool value )
        { sm_print_file = value; };
        static void setPrintLine( bool value )
        { sm_print_line = value; };
        static void setShortFile( bool value )
        { sm_short_file = value; };
        static void setMaxFileLength( int value )
        { sm_max_file_length = value; };
};

template <typename EXCEPTION>
class  CodeLineException : public EXCEPTION, public BaseCodeLineException {
    protected:
        std::string m_file;
        int m_line;
    public:
        /// Constructor
        CodeLineException( EXCEPTION e, const std::string& file, int line )
            : EXCEPTION( e ), m_file( file ), m_line( line )
        {
            if ( sm_notify_on_exception ) {
                std::cerr << "Exception: " << what() << std::endl;
            }
            if ( sm_dump_on_exception ) {
                assert( false );
            }
        };
        
        virtual ~CodeLineException() throw() {};

        virtual const char *what() const throw()
        {
            std::ostringstream s;
            std::string file = m_file;
            if ( sm_short_file ) {
                unsigned int pathend = file.find_last_of( "\\/" );
                if ( pathend != std::string::npos ) {
                    file = file.substr( pathend );
                }
                if ( file.size() > sm_max_file_length ) {
                    file = file.substr( 0, sm_max_file_length - 3 ) + std::string( "..." );
                }
            }
            if ( sm_print_file ) {
                s << file << " ";
            }
            if ( sm_print_line ) {
                s << m_line << " ";
            }
            if ( sm_print_file || sm_print_line ) {
                s << ": ";
            }
            s << EXCEPTION::what();
            return s.str().c_str();
        }
};

} // namespace rvrutils

#define CODELINE_EXCEPTION(T,e)   rvrutils::CodeLineException<T>( e, __FILE__, __LINE__ )
#define CODELINE_RTEXCEPTION(what)   rvrutils::CodeLineException<std::runtime_error>( std::runtime_error( what ), __FILE__, __LINE__ )
#define EXCEPTION_IF_NULL( var )   if ( var == NULL ) throw rvrutils::CodeLineException<std::runtime_error>( std::runtime_error( "NULL: " #var ), __FILE__, __LINE__ )
#define EXCEPTION_IF_TRUE( expr )   if ( expr ) throw rvrutils::CodeLineException<std::runtime_error>( std::runtime_error( "TRUE: " #expr ), __FILE__, __LINE__ )
#define EXCEPTION_IF_FALSE( expr )   if ( !(expr) ) throw rvrutils::CodeLineException<std::runtime_error>( std::runtime_error( "FALSE: " #expr ), __FILE__, __LINE__ )

#endif // __rvrutils__CODELINE_EXCEPTION_H
