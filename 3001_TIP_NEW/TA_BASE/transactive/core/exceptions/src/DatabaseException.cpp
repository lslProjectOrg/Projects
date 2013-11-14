/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/core/exceptions/src/DatabaseException.cpp $
  * @author:  Ripple
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2012/02/06 16:15:14 $
  * Last modified by:  $Author: haijun.li $
  *
  * <description>
  *
  */
// DatabaseException.cpp: implementation of the DatabaseException class.
//
//////////////////////////////////////////////////////////////////////

#include "DatabaseException.h"

namespace TA_Base_Core
{

	//////////////////////////////////////////////////////////////////////
	// Construction/Destruction
	//////////////////////////////////////////////////////////////////////

	DatabaseException::DatabaseException(const std::string& msg) throw() :
		  TransactiveException(msg)
	{
	}

} // Closes TA_Base_Core

