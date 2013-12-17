/**
* The source code in this file is the property of
* Ripple Systems and is not for redistribution
* in any form.
*
* Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/core/database/src/DBException.h $
* @author:  Ripple
* @version: $Revision: #3 $
*
* Last modification: $DateTime: 2012/02/10 14:06:39 $
* Last modified by:  $Author: huirong.luo $
*
* This is a simple exception class that stores the error status code for
* the result of an OCI function call and the reason that the call failed.
* This class can then be queried for a summary of the error.
* 
*/

#ifndef DB_EXCEPTION_H
#define DB_EXCEPTION_H

#include <string>
#include <exception>

class DBException : public std::exception
{
public:
	DBException( const std::string status, const std::string reason, 
		long nativeCode ) throw();
	virtual ~DBException() throw();

	std::string getSummary() const;
	std::string getStatus() const;
	std::string getReason() const;
	long getNativeCode() const;
private:
	std::string    m_status;
	std::string    m_reason;
	long	       m_nativeCode;
};

#endif // DB_EXCEPTION_H

