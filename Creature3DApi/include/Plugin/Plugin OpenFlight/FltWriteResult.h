/*
 * This library is open source and may be redistributed and/or modified under
 * the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or (at
 * your option) any later version. The full license is in the LICENSE file
 * included with this distribution, and on the openscenegraph.org website.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * OpenSceneGraph Public License for more details.
*/

//
// Copyright(c) 2008 Skew crMatrix Software LLC.
//

#ifndef __FLT_WRITE_RESULT_H__
#define __FLT_WRITE_RESULT_H__ 1

#include <string>
#include <utility>
#include <vector>

#include <CRCore/crNode.h>
#include <CRCore/crNotify.h>
#include <CRIOManager/crReaderWriter.h>
#include <CRIOManager/crFileNameUtils.h>


namespace flt
{


/*!
   Custom WriteResult to support proxy/validation ("validate" Option).
   If the application is able to #include this header and obtain the Writeresult
   from CRIOManager, then the app can query this class for warning or error
   conditions due to scene graph incompatibility with FLT.
 */
class FltWriteResult : public CRIOManager::crReaderWriter::WriteResult
{
public:
    FltWriteResult( WriteResult::WriteStatus status=WriteResult::FILE_SAVED )
      : WriteResult( status )
        {}

    void setNumErrors( int n );
    int getNumErrors() const;

    void setNumWarnings( int n );
    int getNumWarnings() const;

    typedef std::pair< CRCore::NotifySeverity, std::string > MessagePair;
    typedef std::vector< MessagePair > MessageVector;

    void warn( const std::string &ss )
    {
        messages_.push_back( std::make_pair( CRCore::WARN, ss ) );
    }

    void error( const std::string &ss )
    {
        messages_.push_back( std::make_pair( CRCore::FATAL, ss ) );
    }

protected:
    MessageVector messages_;
};


}

#endif /* __OPEN_FLIGHT_WRITER_H__ */
