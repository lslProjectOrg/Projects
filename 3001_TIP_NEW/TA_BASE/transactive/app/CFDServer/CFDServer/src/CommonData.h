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
#include <ctime>

#include <limits.h>
#include <float.h>
#include <memory.h>
//#include <memory>	 

#include <iostream>	
#include <string>
#include <sstream>
#include <vector>	
#include <map> 
#include <set>
#include <list>
#include <deque>
#include <utility> // for pair
#include <algorithm> // for lexicographical_compare
#include <iosfwd>    // needed by basic_path inserter and extractor
#include <stdexcept>
#include <cassert>

// random_shuffle example
#include <cstdlib>      // std::rand, std::srand

#include <fstream>
#include <signal.h>


#ifdef WIN32
#include <process.h>
#else
#include "pthread.h"
#endif
#include "pthread.h"

#include <direct.h>


#include "CommonDef.h"

NS_BEGIN(TA_Base_App)


//==============================================================
/*
	declare some time constant for easy to use
*/
enum TimeConstantMac
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






//==============================================================

//user  use
enum EThreadWorkState
{ 	
	WorkState_Begin,
	WorkState_Ready,

	WorkState_End,
};


//==============================================================





//==============================================================


typedef std::vector<std::string>							VrtLinesConT;	
typedef std::vector<std::string>::iterator					VrtLinesConIterT;

typedef std::map<unsigned int, std::string>					mapIDCodeConT;	
typedef std::map<unsigned int, std::string>::iterator		mapIDCodeConIterT;
typedef std::map<unsigned int, std::string>::value_type		mapIDCodeValueTypeT;

//==============================================================



NS_END(TA_Base_App)

#endif  // __DEF__COMMON_DATA_H__








