#include "rbody/ActionConflicts.h"

using namespace rbody;


/*----------------------------------------------------------------------------
  Constructor
  ----------------------------------------------------------------------------*/
ActionConflicts::ActionConflicts()
{

}


/*----------------------------------------------------------------------------
  Destructor
  ----------------------------------------------------------------------------*/
ActionConflicts::~ActionConflicts()
{

    m_act_conflicts_mmap.clear();

}


/*----------------------------------------------------------------------------
  Add two conflicting actions
  ----------------------------------------------------------------------------*/
void ActionConflicts::addConflictingActions(const std::string& action1, const std::string& action2)
{

    // Insert conflicting actions into multi map
    // Make both action1 and action2 a key which makes both searchable (sp?)
    m_act_conflicts_mmap.insert(std::make_pair(action1, action2));
    m_act_conflicts_mmap.insert(std::make_pair(action2, action1));

}


/*----------------------------------------------------------------------------
  Get actions that conflict with given action
  ----------------------------------------------------------------------------*/
void ActionConflicts::getConflictingActions(const std::string& action, std::list<std::string>& conflicting_actions)
{


    // Take the fast way out if multi map is empty
    if (m_act_conflicts_mmap.empty())
        return;

    // Get number of conflicting actions
    int nr_of_conflicting_actions = m_act_conflicts_mmap.count(action);

    // Take fast way out if no there are no conflicting actions to given action
    if (nr_of_conflicting_actions == 0)
        return;

    std::multimap<std::string, std::string>::iterator act_conflicts_iter;

    // If only one conflicting action, add it to conflicting actions list and return
    if (nr_of_conflicting_actions == 1) {
        act_conflicts_iter = m_act_conflicts_mmap.find(action);
        conflicting_actions.push_back(act_conflicts_iter->second);
        return;
    }

    // If we get here there are more than one conflicting action

    // Pair of act_conflicts_mmap iterators
    std::pair<std::multimap<std::string, std::string>::iterator, std::multimap<std::string, std::string>::iterator> iter_pair;

    // Get pair with range
    iter_pair = m_act_conflicts_mmap.equal_range(action);

    // Loop through all conflicting actions and add them to list
    for (act_conflicts_iter = iter_pair.first; act_conflicts_iter != iter_pair.second; act_conflicts_iter++)
        conflicting_actions.push_back(act_conflicts_iter->second);


}

/*------------------------------------------

 * $Source: /cvsroot/replicantbody/replicantbody/src/rbody/ActionConflicts.cpp,v $
 * $Revision: 1.1.1.1 $ 
 * $Date: 2005/02/06 06:52:59 $
 * $Author: deepone $ 
 * $Locker:  $

 * Description: 

 Copyright (C) Peter Sunna, VRlab, Ume?University 2001

Created: 2001-12-12

 * $Log: ActionConflicts.cpp,v $
 * Revision 1.1.1.1  2005/02/06 06:52:59  deepone
 * initial sourceforge checkin
 *
 * Revision 1.1  2002/06/20 13:23:11  sunna
 * no message
 *
 * Revision 1.3  2002/04/16 15:44:23  sunna
 * no message
 *
 * Revision 1.2  2002/03/15 08:55:08  andersb
 * no message
 *
 * Revision 1.1.1.1  2002/03/14 20:48:40  andersb
 * First import as ReplicantBody
 *
 * Revision 1.1.1.1  2002/01/04 08:22:40  andersb
 * Initial version
 *
 ----------------------------------------------------------------------------*/


