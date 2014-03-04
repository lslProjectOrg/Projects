#ifndef  __DEF__CFDSEVER_COMMON_DEF_H_
#define  __DEF__CFDSEVER_COMMON_DEF_H_

//---------------------------------------------------------------
// easy usage of the namespace identifier
#define  NS_BEGIN(name)			namespace name {
#define  NS_END(name)			};
//---------------------------------------------------------------




NS_BEGIN(TA_Base_App) 

#if defined(_MSC_VER) || defined(__BORLANDC__)
typedef unsigned __int64		BigInt64;//%I64d
typedef signed __int64			UBigInt64;// %I64u ¡¢ %I64o ¡¢ %I64x
#else
typedef unsigned long long		BigInt64;
typedef signed long long		UBigInt64;
#endif




#define  defOracleDBName          "ORACLE"
#define  defMysqlDBName           "MYSQL"
#define  defSQLiteDBName          "SQLITE"

#define  defQTMysqlDBTypeName           "QMYSQL"
#define  defQTSQLiteDBTypeName          "QSQLITE"


NS_END(TA_Base_App)

#endif  // __DEF__CFDSEVER_COMMON_DEF_H_











