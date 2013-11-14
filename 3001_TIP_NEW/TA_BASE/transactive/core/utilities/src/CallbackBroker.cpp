/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/core/utilities/src/CallbackBroker.cpp $
  * @author:  Ripple
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2012/02/06 16:15:14 $
  * Last modified by:  $Author: haijun.li $
  *
  */
// A very simple observer/subscriber implementation

#if defined( WIN32 )
#pragma warning( disable : 4786 )
#endif // defined( WIN32 )

#include <vector>
#include "CallbackBroker.h"
#include "CallbackUser.h"
#include "CallbackSubmission.h"


// getInstance()
//
// Create the one and only instance...
CallbackBroker& CallbackBroker::getInstance()
{
    static CallbackBroker theBroker;
    return theBroker;
}


// registerCallbackUser()
//
// Add a new user/key pair to the two map members

void CallbackBroker::registerCallbackUser(CallbackUser* user,
                                          const std::string& key)
{
    UserToKeyMap::value_type ukValue(user, key);
    m_userToKeyMap.insert(ukValue);

    KeyToUserMap::value_type kuValue(key, user);
    m_keyToUserMap.insert(kuValue);
}


// deregisterCallbackUser()
//
// Remove all references to the given user from the two map members

void CallbackBroker::deregisterCallbackUser(CallbackUser* user)
{
    // Cycle through each of the entries in the UserToKey map which are
    // associated with the given user and erase them, but remember the
    // associated keys
	if (!user)
		return;

	if (m_userToKeyMap.empty())
		return;

    UserToKeyMap::iterator ukIter = m_userToKeyMap.find(user);
    UserToKeyMap::iterator ukIterTmp;
    std::vector<std::string> keys;
    while (ukIter != m_userToKeyMap.end() && ukIter->first == user)
    {
        // Remember the key
        keys.push_back(ukIter->second);

        // Remember the current iterator, step to the next iterator, then erase
        // the current iterator
        ukIterTmp = ukIter;
        ukIter++;
        m_userToKeyMap.erase(ukIterTmp);
    }

    // Now use the vector of keys to find all the entries in the KeyToUser map
    // which are associated with the given user and erase them
    KeyToUserMap::iterator kuIter;
    for (unsigned int i = 0; i < keys.size(); i++)
    {
        kuIter = m_keyToUserMap.find(keys[i]);
        while (kuIter != m_keyToUserMap.end()
            && kuIter->first == keys[i])
        {
            if (kuIter->second == user)
            {
                m_keyToUserMap.erase(kuIter);   // found it!
                break;
            }
            kuIter++;
        }
    }
}


// submit()
//
// Forward the given data to each interested observer

void CallbackBroker::submit(const CallbackSubmission* submission)
{
    KeyToUserMap::iterator i = m_keyToUserMap.find(submission->getKey());
    while (i != m_keyToUserMap.end() && i->first == submission->getKey())
    {
        i->second->callback(submission);
        i++;
    }
}
