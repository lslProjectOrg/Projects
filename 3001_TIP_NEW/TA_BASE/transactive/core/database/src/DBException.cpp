/**
* The source code in this file is the property of
* Ripple Systems and is not for redistribution
* in any form.
*
* Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/core/database/src/DBException.cpp $
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
#include "DBException.h"
#include <sstream>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DBException::DBException( const std::string status, const std::string reason, 
						 long nativeCode )
						 throw() : m_status( status ),
						 m_reason( reason ),
						 m_nativeCode( nativeCode )
{
	// Do nothing. 
}

DBException::~DBException() throw()
{
	// Do nothing. 
}

std::string DBException::getSummary() const
{

	std::ostringstream ostr;

	ostr << "Status: " << m_status.c_str() << " Reason: " << m_reason.c_str()
		<< " NativeCode: " << m_nativeCode;
	return ostr.str();
}

std::string DBException::getStatus() const
{
	return m_status;
}

std::string DBException::getReason() const
{
	return m_reason;
}

long DBException::getNativeCode() const
{
	return m_nativeCode;
}

