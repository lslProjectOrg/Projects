#if !defined(_MATRIXEXCEPTION_H__A7A12810_888E_48F1_A959_D1E6DCAFFB9B__INCLUDED_)
#define _MATRIXEXCEPTION_H__A7A12810_888E_48F1_A959_D1E6DCAFFB9B__INCLUDED_

/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/core/utilities/src/MatrixException.h $
  * @author:  Bart Golab
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2012/02/06 16:15:14 $
  * Last modified by:  $Author: haijun.li $
  * 
  * Thrown by Matrix when an attempt is made to access a nonexisting element.
  *
  */

#include "core/exceptions/src/TransactiveException.h"


namespace TA_Base_Core
{
    class MatrixException : public TransactiveException  
    {
    public:
        MatrixException() throw();
        MatrixException(const std::string& msg) throw();
        virtual ~MatrixException() throw();
    };
}

#endif // !defined(_MATRIXEXCEPTION_H__A7A12810_888E_48F1_A959_D1E6DCAFFB9B__INCLUDED_)
