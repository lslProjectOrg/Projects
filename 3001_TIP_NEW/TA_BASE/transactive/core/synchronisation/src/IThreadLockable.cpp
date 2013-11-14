/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/core/synchronisation/src/IThreadLockable.cpp $
  * @author:  Robert Young
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2012/02/06 16:15:14 $
  * Last modified by:  $Author: haijun.li $
  *
  * Implements those functions that may not be supported by all ThreadLockable
  * implementations. In general they should just assert.
  *
  */

#include "core/utilities/src/TAAssert.h"
#include "IThreadLockable.h"

namespace TA_Base_Core
{

    void IThreadLockable::acquireRead()
    {
        TA_ASSERT(false, "Read locks not supported");
    }

    void IThreadLockable::releaseRead()
    {
        TA_ASSERT(false, "Read locks not supported");
    }

}