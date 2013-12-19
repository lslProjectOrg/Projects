#ifndef  __DEF_DATA_ACCESS__COMMON_DEF_HH__
#define  __DEF_DATA_ACCESS__COMMON_DEF_HH__

#include "core/utilities/src/UtilitiesCommonDef.h"

NS_BEGIN(TA_Base_Core) 



#define  defSupportDBCount        3     // the number of database which supported in the system.

#define  defOracleDBName          "ORACLE"
#define  defMysqlDBName           "MYSQL"
#define  defSQLiteDBName          "SQLITE"


#define  DefOCConnectionStringsDelimiter           ','
#define  DefConCharDelimiter					   ':'
#define  DefConStrDelimiter					       ":"



//maybe 200 is too large for first startup, change it to 100
#define  FETCH_MAX_ROWS  110  
#define  FETCH_MAX_COLS  100 


NS_END(TA_Base_Core)

#endif  // __DEF_DATA_ACCESS__COMMON_DEF_HH__











