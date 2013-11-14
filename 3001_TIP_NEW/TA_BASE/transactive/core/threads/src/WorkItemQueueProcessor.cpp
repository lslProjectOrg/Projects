/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/core/threads/src/WorkItemQueueProcessor.cpp $
  * @author:  Ripple
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2012/02/06 16:15:14 $
  * Last modified by:  $Author: haijun.li $
  *
  * This is a queue processor which processes work items.
  * It provides a means of having a queue which you can put anything in
  * and the items you insert will do their own work sequentially
  *
  */

#include "core/threads/src/WorkItemQueueProcessor.h"

namespace TA_Base_Core
{
    WorkItemQueueProcessor::WorkItemQueueProcessor (unsigned long maxQueueSize)
      : QueueProcessor<IWorkItem>(maxQueueSize)
    {}

    WorkItemQueueProcessor::~WorkItemQueueProcessor()
    {
    }

	void WorkItemQueueProcessor::processEvent( boost::shared_ptr<IWorkItem> newEvent )
    {
        newEvent->executeWorkItem();
    }
};

