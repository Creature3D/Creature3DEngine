#ifndef _CACTIONCONFLICTS_H
#define _CACTIONCONFLICTS_H

#include "rbody/global.h"
#include <string>
#include <map>
#include <list>

namespace rbody {

/// Contains conflicting actions that cannot be executed at the same time
class REPLICANTBODY_API ActionConflicts {

    public:

        /// Constructor
        ActionConflicts();

        /// Destructor
        virtual ~ActionConflicts();

        /// Add two conflicting actions
        /**
         * \param action1 is name of action that conflicts with action 2
         * \param action2 is name of action that conflicts with action 1
         **/
        void addConflictingActions(const std::string& action1, const std::string& action2);

        /// Get conflicting actions to given action
        /**
         * \param action is name of action which to find conflicting actions for
         * \param conflicting_actions is reference list were conflicting actions are placed
         **/
        void getConflictingActions(const std::string& action, std::list<std::string>& conflicting_actions);

    private:

        /// Action multi map with conflicting actions
        std::multimap<std::string, std::string> m_act_conflicts_mmap; 

};

} // namespace end

#endif // _CACTIONCONFLICTS_H

/*------------------------------------------

* $Source: /cvsroot/replicantbody/replicantbody/include/rbody/ActionConflicts.h,v $
* $Revision: 1.1.1.1 $ 
* $Date: 2005/02/06 06:52:59 $
* $Author: deepone $ 
* $Locker:  $

* Description: 

 Copyright (C) Peter Sunna, VRlab, Ume?University 2001

 Created: 2001-12-12

* $Log: ActionConflicts.h,v $
* Revision 1.1.1.1  2005/02/06 06:52:59  deepone
* initial sourceforge checkin
*
* Revision 1.3  2003/10/31 13:21:45  andersb
* Added ReplicantBodyMgr class to handle instances of CreBody
* Added .char files for specifying individual characters (CreBody)
* Added OSG plugin for reading char files
*
* Revision 1.2  2002/11/01 09:26:28  sunna
* no message
*
* Revision 1.1  2002/06/20 12:51:50  sunna
* Restructured directories
*
* Revision 1.2  2002/03/15 08:55:07  andersb
* no message
*
* Revision 1.1.1.1  2002/03/14 20:48:40  andersb
* First import as ReplicantBody
*
* Revision 1.1.1.1  2002/01/04 08:22:34  andersb
* Initial version
*
----------------------------------------------------------------------------*/
