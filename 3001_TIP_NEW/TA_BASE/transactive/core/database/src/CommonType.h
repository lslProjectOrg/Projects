

#ifndef _COMMON_TYPE_H
#define _COMMON_TYPE_H

#include <string>
#include <vector>
#include <map>

namespace TA_Base_Core
{
	// Data will be retrieved in Rows into a Buffer.
	typedef std::vector<std::string> Row;
	typedef std::vector<Row> Buffer;

	typedef std::vector<unsigned char> DataClob;
	typedef std::vector<DataClob> DataClobArray;

	//const unsigned long MAX_FIELDSIZE  = 4000;
	const unsigned long FETCH_MAX_ROWS = 110;//maybe 200 is too large for first startup, change it to 100
	const unsigned long FETCH_MAX_COLS = 100; 
	const unsigned int  CONN_STR_PARAM_COUNT = 5;  // the number of Db connection string parameters
	const unsigned long DEFMAXLOGLEN = 4000;       // the max number of characters for log module

	enum DB_ERR_CODE
	{
		ER_MYSQL_READ_ONLY			 = 1290, // MySQL server is read-only
		ER_MYSQL_CANNT_CONNECT		 = 2003, // Can't connect to MySQL server 
		ER_MYSQL_GONE_AWAY			 = 2006, // MySQL server has gone away

		ER_ORA_IMMIDATE_SHUTDOWN     = 1089, // immediate shutdown in progress - no operations are permitted
		ER_ORA_INITORSHUTDOWN        = 1033, // ORACLE initialization or shutdown in progress
		ER_ORA_NOT_AVALIABLE         = 1034, // ORACLE not available
		ER_ORA_TNS_CLOSE             = 12537 // TNS:connection closed
	};

	//typedef struct st_sql_tag
	//{
	//	int nSQLID;                                // the SQL ID map in the sqlcode.txt
	//	std::string strSQLID;                      // the SQL ID map in the sqlcode.txt
	//	std::string strCommonSQL;                     // the common SQL which can run at all supported db
	//	std::string strOracleSQL;                     // the oracle SQL statement
	//	std::string strMySQLSQL;                      // the mysql SQL statement
	//	std::string strSQLiteSQL;                     // the SQLite SQL statement

	//}SQLStatement, *PSQLStatement;



	class SQLStatement
	{
	public:
		SQLStatement()
		{
			clear();
		}
		~SQLStatement()
		{
			clear();
		}
	public:
		int nSQLID;                                // the SQL ID map in the sqlcode.txt
		std::string strSQLID;                      // the SQL ID map in the sqlcode.txt
		std::string strCommonSQL;                     // the common SQL which can run at all supported db
		std::string strOracleSQL;                     // the oracle SQL statement
		std::string strMySQLSQL;                      // the mysql SQL statement
		std::string strSQLiteSQL;                     // the SQLite SQL statement

	public:
		void clear()
		{
			this->nSQLID = 0;
			this->strSQLID.clear();
			this->strCommonSQL.clear();
			this->strOracleSQL.clear();
			this->strMySQLSQL.clear();
			this->strSQLiteSQL.clear();
		}
	};//class SQLStatement

}





// the database server type support in the system

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




//define dbType sting
#define  defOracleDBName          "oracle"
#define  defMysqlDBName           "mysql"
#define  defSqliteDBName          "sqlite"



#define  MAXLOGMESGSIZE           2000  // the max length of message for log module to log
#define  MAX_SQLSTRING_LEN        2047  // max length of one SQL statement 2048 (2047 + 1 NULL)
#define  defSQL_MODE_TXT          0     // the define of the SQL statements (text file or header file mode)
#define  defMINPARAMSIZE          1     // the min require parameter count in SQL contruction
#define  defMINSTATICAQPARAMSIZE  2     // the min require parameters count in SQL construction for static AQ Statement.
#define  defMINDYNAMICAQPARAMSIZE 3     // the min require parameters count in SQL construction for dynamic AQ statement.
#define  defAQSQL_Demit           0x2c  // ','
#define  defAQSQL_EndTag          0x29  // ')'
#define  defSupportDBCount        3     // the number of database which supported in the system.




//define namespace Begin end
#define  NS_BEGIN(name) namespace name {
#define  NS_END(name)   };




#endif