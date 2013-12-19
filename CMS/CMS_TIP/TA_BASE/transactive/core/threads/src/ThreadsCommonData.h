#ifndef  __DEF__THREADS_COMMON_DATA_H_
#define  __DEF__THREADS_COMMON_DATA_H_

#include "core/utilities/src/UtilitiesCommonData.h"
#include "core/threads/src/ThreadsCommonDef.h"


NS_BEGIN(TA_Base_Core)



enum EThreadState
{
	THREAD_STATE_NEW,

	THREAD_STATE_RUNNING,
	THREAD_STATE_TERMINATED,

	THREAD_STATE_FINISH,
	THREAD_STATE_UNKNOWN                                     
};

/*
enum STATE {
uninitialized,
starting,
started,
stopping,
stopped
};

*/

NS_END(TA_Base_Core)

#endif  // __DEF__THREADS_COMMON_DATA_H_








