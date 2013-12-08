#ifndef  __DEF__COMMON_DATA_H__
#define  __DEF__COMMON_DATA_H__


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


#include "CommonDef.h"

NS_BEGIN(TA_Base_Test)


//==============================================================
/*
	declare some time constant for easy to use
	Ns
	Microseconds
	Millisecond
	Second
*/
enum TimeConstantMacNS
{
	TIME_BASE_NS		    = 1,
	TIME_BASE_NS_10		    = 10 * TIME_BASE_NS,
	TIME_BASE_NS_100        = 10 * TIME_BASE_NS_10,

	TIME_BASE_MICROS        = 10 * TIME_BASE_NS_100,
	TIME_BASE_MICROS_10     = 10 * TIME_BASE_MICROS,
	TIME_BASE_MICROS_100    = 10 * TIME_BASE_MICROS_10,


	TIME_BASE_SECOND_NS		= 10 * TIME_BASE_MICROS_100,
};
enum TimeConstantMacMicroseconds
{	
	TIME_BASE_SECOND		= 1000,

	TIME_BASE_MIN			= 60   * TIME_BASE_SECOND,
	TIME_BASE_HOUR			= 60   * TIME_BASE_MIN,
	TIME_BASE_DAY			= 24   * TIME_BASE_HOUR,

	TIME_SECOND_TWO         = 2    * TIME_BASE_SECOND,
	TIME_SECOND_THREE       = 3    * TIME_BASE_SECOND,
	TIME_SECOND_FOUR        = 4    * TIME_BASE_SECOND,
	TIME_SECOND_FIVE		= 5    * TIME_BASE_SECOND,
	TIME_SECOND_TEN         = 10   * TIME_BASE_SECOND,
	TIME_SECOND_THIRTY      = 30   * TIME_BASE_SECOND,
            
	TIME_MIN_TWO			= 2  * TIME_BASE_MIN,
	TIME_MIN_THREE			= 3  * TIME_BASE_MIN,
	TIME_MIN_FIVE           = 5  * TIME_BASE_MIN,
	TIME_MIN_EIGHT          = 8  * TIME_BASE_MIN,
	TIME_MIN_TEN            = 10 * TIME_BASE_MIN,
	TIME_MIN_FIFTEEN        = 15 * TIME_BASE_MIN,
	TIME_MIN_TWENTY         = 20 * TIME_BASE_MIN,
	TIME_MIN_THIRTY         = 30 * TIME_BASE_MIN,
	TIME_MIN_FOURTY         = 40 * TIME_BASE_MIN,
	TIME_MIN_FIFTY          = 50 * TIME_BASE_MIN,

	TIME_HOUR_TWO           = 2 * TIME_BASE_HOUR
};


//==============================================================





enum def_int_Code_Value 
{
	DEF_INT_MaxLogMesgSize			=			1024*1024,
	DEF_INT_MinLogMesgSize			=			1024*200,
	DEF_INT_MinMesgBuffSize			=			512,	

	DEF_INT_BUFFER_LEN_ADDEX		=           512,
	DEF_INT_BUFFER_LEN_READ			=			0xFFFF,//65535 64K
	DEF_INT_BUFFER_LEN_MAX			=			0xFFFFFFF,//268M

	DEF_INT_SingleLineMaxSize		=			1024*8,
	DEF_INT_NormalThreadSleep		=			100,
	DEF_INT_MonitorThreadSleep		=           1000,
	DEF_INT_LongMonitorThreadSleep		=           1000*30,

	DEF_INT_LIMIT_TCP_TIMEOUT       =			500,

};


//==============================================================
enum  ECOMFRAMETYPE
{
	ComFrameType_TestNomalData = 1,
	ComFrameType_TestMinData,
	ComFrameType_TestMaxData,
	ComFrameType_NULL,
};




NS_END(TA_Base_Test)

#endif  // __DEF__COMMON_DATA_H__








