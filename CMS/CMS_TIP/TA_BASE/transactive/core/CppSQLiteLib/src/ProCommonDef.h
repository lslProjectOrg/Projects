#ifndef  __DEF_PROJECT_COMMON_DEF_HH__
#define  __DEF_PROJECT_COMMON_DEF_HH__

#include "core/utilities/src/CommonDef.h"

#include "sqlite3.h"
#include <stdio.h>
#include <string.h>
#include <cstdlib>


NS_BEGIN(TA_Base_Core) 


#define CPPSQLITE_ERROR 1000

// Named constant for passing to CppSQLite3Exception when passing it a string
// that cannot be deleted.
const bool DONT_DELETE_MSG=false;







NS_END(TA_Base_Core)

#endif  // __DEF_PROJECT_COMMON_DEF_HH__











