#ifndef  __DEF_DATA_ACCESS__COMMON_DATA_HH__
#define  __DEF_DATA_ACCESS__COMMON_DATA_HH__

#include "core/utilities/src/UtilitiesCommonData.h"
#include "core/DataAccess/src/DataAccessCommonDef.h"

NS_BEGIN(TA_Base_Core)

#include <cstdlib>
#include <iostream>
#include <sstream>


// Data will be retrieved in Rows into a Buffer.
typedef std::list<std::string> Row;
typedef std::list<Row> Buffer;

typedef std::list<unsigned char> DataClob;
typedef std::list<DataClob> DataClobArray;



// the database server type support in the system
enum DbServerType
{
	enumBeginDbType = 0,    // the begin of the supported database type.

	enumOracleDb = 1,   // Oracle database  (Support) //defOracleDBName
	enumMysqlDb  = 2,   // Mysql database   (Support) //defMysqlDBName
	enumSqliteDb = 3,   // SQLiet database  (support) //defSqliteDBName

	enumEndDbType,          // the end tag of supported database type, all supported database should list above this line.

	enumReserve         // For other database system should add below
};


enum DB_ERR_CODE
{
	ER_DB_ERR_UNKNOWN			 = 0,

	ER_DB_ERR_SQLFileHelper,		 // "the SQLKey cannot match in Hash table"
	ER_DB_ERR_SQLCode,		     //"the PrepareStatement parameter count error"
	ER_DB_ERR_SQLTypeAdapter,  //"Not enough chars in SQLTypeAdapter"
};


NS_END(TA_Base_Core)

#endif  // __DEF_DATA_ACCESS__COMMON_DATA_HH__








