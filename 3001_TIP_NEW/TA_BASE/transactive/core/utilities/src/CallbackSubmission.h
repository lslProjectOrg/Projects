/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/core/utilities/src/CallbackSubmission.h $
  * @author:  Ripple
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2012/02/06 16:15:14 $
  * Last modified by:  $Author: haijun.li $
  *
  * <description>
  *
  * Base class for submissions to the CallbackBroker
  */

#ifndef CALLBACK_SUBMISSION_H
#define CALLBACK_SUBMISSION_H

#if defined( WIN32 )
#pragma warning( disable : 4786 )
#endif /* defined( WIN32 ) */

#include <string>

class CallbackSubmission
{
    /** This is the base class for data submitted to the CallbackBroker. If you
      * wish to provide more data than the key then you can derive a subclass
      * which includes the extra data members plus accessors. The CallbackUser
      * can access your derived class using a dynamic_cast. The broker doesn't
      * care. Your derived class can contain additional data by reference -
      * passing more data doesn't necessarily mean copying swags of stuff into
      * the submission.
      *
      * For example, the following class passes an additional structure member
      * by reference:
      *
      * class MyStructSubmission: public CallbackSubmission
      * {
      * public:
      *     virtual const MyStruct* getData() const { return m_data; }
      *     virtual void setData(const MyStruct* data) { m_data = data; }
      * private:
      *     const MyStruct* m_data;
      * };
      */
public:

    /**
      * Constructor
      * 
      * @param key [IN]:
      *     optionally set the m_key member on construction
      */
    CallbackSubmission() {};

    CallbackSubmission(std::string key)
        : m_key(key)
    {
    }

    /**
      * Destructor
      */
    virtual ~CallbackSubmission() {};

    /**
      * getKey
      *
      * accessor for the m_key member
      *
      * @return a copy of the m_key string
      */
    virtual std::string getKey() const { return m_key; }

    /**
      * setKey
      *
      * accessor for the m_key member
      *
      * @param key [IN]:
      *     new value for the m_key member
      */
    virtual void setKey(std::string key) { m_key = key; };

private:
    // The minimum data associated with a submission
    std::string m_key;
};

#endif // CALLBACK_SUBMISSION_H
