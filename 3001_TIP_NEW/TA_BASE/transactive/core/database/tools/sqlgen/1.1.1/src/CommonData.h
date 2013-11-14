/**
  *
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source: $File: //COTS/sqlgen/1.1.0/src/CommonData.h $
  * @author Shenglong.Lin
  * @version $Revision: #1 $
  * Last modification: $DateTime: 2013/06/21 12:00:00 $
  * Last modified by: Shenglong.Lin $
  * 
  * to define some common use data or structure which used in the sqlgen
  *
  */
 


#ifndef __DEF_COMMONDATA_HH__
#define __DEF_COMMONDATA_HH__


//////////////////////////////////////////////////////////////////////////
///include
//////////////////////////////////////////////////////////////////////////


#include <stdio.h> 
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <sys/timeb.h>
#include <ctime>
#include <limits.h>
#include <float.h>
#include <iostream>	
#include <string>
#include <sstream>
#include <fstream>
#include <vector>	
#include <map> 
#include <list>



//======================================================================================
//log
// Debug levels are sequentially indexed starting at 0.  It is possible to iterate 
// through all debug levels by iterating through the integral sequence [ 0, countDebugLevels() ).
// The current debug level determines the highest debugging level that will be logged.
enum EDebugLevel
{
	DebugFatal		= 0L,	// fatal error conditions
	DebugError,		    // error conditions
	DebugWarn,		        // warning conditions
	DebugNorm,		        // normal conditions
	DebugInfo,		        // informational messages
	DebugDebug,		    // debug-level messages
	DebugSQL,              // SQL query strings
	DebugCORBA,            // for CORBA debug messages (only if other flags are enabled - see CorbaUtil)
	DebugMsg,              // for Messaging library debug messages
	DebugMsgPub,           // for Messaging library message sending and receiving debug messages
	DebugPeriodic,         // messages logged periodically by keepalives, etc
	DebugDatabase,      	// data access (SimpleDB) messages (other than SQL query strings)
	DebugExternal,      	// 3rd party (non-transactive) messages
	DebugTrace,		    // Function Entry/Exit Messages
};



struct DebugLevelNameValue
{
	const char*   pLevelName;
	EDebugLevel   nLevelValue;
};

//======================================================================================


typedef std::map<std::string, std::string>				mapSqlIdLineConT;
typedef std::map<std::string, std::string>::iterator    mapSqlIdLineConIterT;
typedef std::map<std::string, std::string>::value_type	mapSqlIdLineConValueT;

typedef std::vector<std::string>						VrtLinesConT;	
typedef std::vector<std::string>::iterator				VrtLinesConIterT;


#endif  // __DEF_COMMONDATA_HH__





