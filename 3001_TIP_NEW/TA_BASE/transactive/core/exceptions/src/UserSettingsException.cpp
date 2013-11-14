/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/core/exceptions/src/UserSettingsException.cpp $
  * @author:  Ripple
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2012/02/06 16:15:14 $
  * Last modified by:  $Author: haijun.li $
  *
  * <description>
  *
  */
#include "core\exceptions\src\UserSettingsException.h"

namespace TA_Base_Core
{

    UserSettingsException::UserSettingsException(const char* msg) 
        : TransactiveException(msg)
    {

    }	

    UserSettingsException::~UserSettingsException()
    {

    }



} // End TA_Base_Core
