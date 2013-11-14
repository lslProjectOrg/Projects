/**
  * The source code in this file is the property of 
  * Ripple Systems Pty Ltd and is not for redistribution
  * in any form.
  *
  * Source: $File: //depot/3001_TIP_NEW/TA_BASE/transactive/core/exceptions/src/DbUniqueConstraintViolated.h $
  * @author Gregg Kirkpatrick
  * @version $Revision: #2 $
  * Last modification: $DateTime: 2012/02/06 16:15:14 $
  * Last modified by:  $Author: haijun.li $
  * 
  * Raised by the db code when a unique constraint is violated.
  */


#if !defined(DBUNIQUECONSTRAINTVIOLATED_H_INCLUDED)
#define DBUNIQUECONSTRAINTVIOLATED_H_INCLUDED

#include "DatabaseException.h"
#include <string>

namespace TA_Base_Core
{
	class DbUniqueConstraintViolated : public DatabaseException  
	{
	public:

		DbUniqueConstraintViolated(const std::string& msg) throw();
	};
} // Closes TA_Base_Core

#endif // !defined(DBUNIQUECONSTRAINTVIOLATED_H_INCLUDED)

