#ifndef _RBODY_ACTIONREQUEST_H_
#define _RBODY_ACTIONREQUEST_H_

#include <typeinfo>

#include <map>
#include <list>
#include <CRCore/ref_ptr.h>

#include "rvrutils/Property.h"
#include "rbody/export.h"

namespace rbody {


#ifdef SUCCESS 
#undef SUCCESS
#endif
/// Request result type
enum RequestResult {
    UNKNOWN, /**< means the body does not recognize this action */
    INACTIVE, /**< means the action is not active */
    STARTING, /**< means the action is activating */
    ACTIVE, /**< means the action is active */
    STOPPING, /**< means the action is deactivating */
    QUEUED, /**< means the action is soon to be active */
    CONFLICTS, /**< means the action conflicts with active ones */
    SUCCESS /**< means everything went well */
};

/// Body action
class REPLICANTBODY_API ActionRequest  : public rvrutils::PropertyContainer {
    protected:
        /// Name of action
        const std::string m_name;
        /// Is this action prioritized?
        bool m_prioritized;
		virtual ~ActionRequest(){}
    public:
        /// Constructor
        ActionRequest( const std::string& name ) : m_prioritized(false),m_name(name) {};

        /// Constructor
        ActionRequest( const ActionRequest& action )
            : rvrutils::PropertyContainer(action),
              m_name(action.m_name),
              m_prioritized(action.m_prioritized) {};

        /// Clone this body action object
        ActionRequest* clone() { return new ActionRequest( *this ); };

        /// Get name
        const std::string& getName() const { return m_name; };

        /// Get prioritized status
        bool getPrioritized() { return m_prioritized; };

        /// Set prioritized
        void setPrioritized( bool prioritized ) { m_prioritized = prioritized;};
};

} // namespace rbody

#endif // _RBODY_ACTIONREQUEST_H_
