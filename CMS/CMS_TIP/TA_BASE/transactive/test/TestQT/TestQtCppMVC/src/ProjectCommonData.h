#ifndef  __DEF__HISTORY_DATA_SERVER_COMMON_DATA_H_
#define  __DEF__HISTORY_DATA_SERVER_COMMON_DATA_H_

#include "ProjectCommonDef.h"



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

NS_BEGIN(TA_Base_App)




// the database server type support in the system
enum DbServerType
{
	enumBeginDbType = 0,    // the begin of the supported database type.

	enumOracleDb = 1,   // Oracle database  (Support) //defOracleDBName
	enumMysqlDb  = 2,   // MYSQL database   (Support) //defMysqlDBName
	enumSqliteDb = 3,   // SQLITE database  (support) //defSqliteDBName

	enumEndDbType,          // the end tag of supported database type, all supported database should list above this line.

	enumReserve         // For other database system should add below
};


// the BarInfo type support in the system
enum BarInfoType
{
	enumBeginBarInfoType = 0,

	enumBarInfoType_5second,
	enumBarInfoType_1min,
	enumBarInfoType_5min,
	enumBarInfoType_15min,
	enumBarInfoType_30min,
	enumBarInfoType_60min,
	enumBarInfoType_1day,
	enumBarInfoType_30day,
	enumBarInfoType_1year,


	enumEndBarInfoTypeEnd,
	enumBarInfoTypeReserve
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
	TIME_BASE_S_1YEAR		= 365   * TIME_BASE_S_1DAY,

};


NS_END(TA_Base_App)

#endif  // __DEF__HISTORY_DATA_SERVER_COMMON_DATA_H_








