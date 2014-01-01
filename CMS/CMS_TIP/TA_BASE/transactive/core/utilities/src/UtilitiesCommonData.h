#ifndef  __DEF__UTILITIES_COMMON_DATA_H_
#define  __DEF__UTILITIES_COMMON_DATA_H_


//////////////////////////////////////////////////////////////////////////
///include
//////////////////////////////////////////////////////////////////////////

#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <typeinfo.h>
#include <time.h>
#include <sys/timeb.h>
#include <limits.h>
#include <float.h>
#include <memory.h>
#include <signal.h>
#include <direct.h>

#include <iostream>	
#include <string>
#include <sstream>
#include <fstream>
#include <vector>	
#include <ctime>
#include <map> 
#include <set>
#include <list>
#include <deque>
#include <utility> // for pair
#include <algorithm> // for lexicographical_compare
#include <iosfwd>    // needed by basic_path inserter and extractor
#include <stdexcept>
#include <cassert>
#include <cstdlib>      // std::rand, std::srand random_shuffle
#include <cctype>
#include <functional>


#include "core/utilities/src/UtilitiesCommonDef.h"

NS_BEGIN(TA_Base_Core)


//==============================================================
/*
	declare some time constant for easy to use
	1s=1000ms=1000 000us=1000 000 000ns=1000 000 000 000ps
	1 Second=1000 Millisecond=1000 000 Microsecond=1000 000 000 Nanosecond=1000 000 000 000 Picosecond
*/
enum TimeBaseMicrosecond
{
	TIME_BASE_US_1US		    = 1,
	TIME_BASE_US_1MS        = 1000 * TIME_BASE_US_1US,
	TIME_BASE_US_1S		   = 1000 * TIME_BASE_US_1MS,
};
enum TimeBaseMillisecond
{	
	TIME_BASE_MS_1MS        = 1,
	TIME_BASE_MS_1S		    = 1000 * TIME_BASE_MS_1MS,
	TIME_BASE_MS_1MIN		= 60   * TIME_BASE_MS_1S,
	TIME_BASE_MS_1HOUR		= 60   * TIME_BASE_MS_1MIN,
	TIME_BASE_MS_1DAY		= 24   * TIME_BASE_MS_1HOUR,
};
enum TimeBaseSecond
{	
	TIME_BASE_S_1S          = 1,
	TIME_BASE_S_5S          = 5 * TIME_BASE_S_1S,

	TIME_BASE_S_1MIN		= 60   * TIME_BASE_S_1S,
	TIME_BASE_S_5MIN		= 5   * TIME_BASE_S_1MIN,
	TIME_BASE_S_15MIN		= 15   * TIME_BASE_S_1MIN,
	TIME_BASE_S_30MIN		= 30   * TIME_BASE_S_1MIN,

	TIME_BASE_S_1HOUR		= 60   * TIME_BASE_S_1MIN,
	TIME_BASE_S_1DAY		= 24   * TIME_BASE_S_1HOUR,
	TIME_BASE_S_1MON		= 30   * TIME_BASE_S_1DAY,
	TIME_BASE_S_1YEAR		= 12   * TIME_BASE_S_1MON,

};

NS_END(TA_Base_Core)

#endif  // __DEF__UTILITIES_COMMON_DATA_H_








