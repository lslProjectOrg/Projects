#ifndef  __DEF__HISTORY_DATA_SERVER_COMMON_DEF_H_
#define  __DEF__HISTORY_DATA_SERVER_COMMON_DEF_H_

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




#define  DEF_VALUE_STRING_DB_ORACLE          "ORACLE"
#define  DEF_VALUE_STRING_DB_MYSQL           "MYSQL"
#define  DEF_VALUE_STRING_DB_SQLITE          "SQLITE"

#define  DEF_VALUE_STRING_QT_SUPPORT_DB_QMYSQL           "QMYSQL"
#define  DEF_VALUE_STRING_QT_SUPPORT_DB_QSQLITE          "QSQLITE"


NS_END(TA_Base_App)

#endif  // __DEF__HISTORY_DATA_SERVER_COMMON_DEF_H_











