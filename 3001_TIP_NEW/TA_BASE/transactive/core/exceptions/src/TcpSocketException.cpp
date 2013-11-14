/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/core/exceptions/src/TcpSocketException.cpp $
  * @author:  J. Welton
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2012/02/06 16:15:14 $
  * Last modified by:  $Author: haijun.li $
  * 
  * Exception class thrown by the TcpSocket subsystem
  *
  */

#include "TcpSocketException.h"

namespace TA_Base_Core
{

    TcpSocketException::TcpSocketException() throw()
    {}

    TcpSocketException::TcpSocketException(const std::string& msg) 
        throw(): TransactiveException(msg)
    {}

    TcpSocketException::~TcpSocketException() throw()
    {}
}
