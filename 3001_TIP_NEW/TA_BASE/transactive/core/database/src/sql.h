//[sqlcode.txt file time=2013/11/16 17:07:36]
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// This file is generated by the sql header file tool sqlgen.exe. Please do 
// not modify this file directly. Change the sqlcode.txt file instead.
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 


#ifndef SQL_H          
#define SQL_H           



#if defined(_UNICODE) || defined(UNICODE)       
typedef wchar_t  CharT;	    
#else					 
typedef char     CharT;		
#endif						




struct st_sql_data	
{
	const CharT*    pSQLID;
	const CharT*    pCommonSQL;
	const CharT*    pOracleSQL;
	const CharT*    pMySQLSQL;
	const CharT*    pSQLiteSQL;
};



static const st_sql_data  s_SQLArray[]=	
{   


//[0][TABLE_TEST_DAITEST]
	{
		/*SQLID*/("TABLETESTDAITEST_SELECT_1000"),
		/*CommonSQL*/("SELECT STR_COLUMN FROM TABLE_TEST_DAITEST WHERE LINE_ID = 1000"),
		/*OracleSQL*/(""),
		/*MySQLSQL*/(""),
		/*SQLiteSQL*/("")	
	},
	{
		/*SQLID*/("TABLETESTDAITEST_UPDATE_3000"),
		/*CommonSQL*/("UPDATE TABLE_TEST_DAITEST SET INT_COLUMN=5000, STR_COLUMN='SQL_UPDATE_OK' WHERE LINE_ID = 1000"),
		/*OracleSQL*/(""),
		/*MySQLSQL*/(""),
		/*SQLiteSQL*/("")	
	},
	{
		/*SQLID*/("TABLETESTDAITEST_DELETE_4000"),
		/*CommonSQL*/("DELETE FROM TABLE_TEST_DAITEST WHERE LINE_ID = 1000"),
		/*OracleSQL*/(""),
		/*MySQLSQL*/(""),
		/*SQLiteSQL*/("")	
	},
	{
		/*SQLID*/("TABLETESTDAITEST_DROP_5000"),
		/*CommonSQL*/("DROP TABLE TABLE_TEST_DAITEST"),
		/*OracleSQL*/(""),
		/*MySQLSQL*/(""),
		/*SQLiteSQL*/("")	
	},

//[0][TABLE_TEST_DAITEST]
	{
		/*SQLID*/("TABLETESTDAITEST_INSERT_2000"),
		/*CommonSQL*/(""),
		/*OracleSQL*/("INSERT INTO TABLE_TEST_DAITEST(LINE_ID, TEST_TYPE, INT_COLUMN, STR_COLUMN, BOOL_COLUMN, REAL_COLUMN,\
 DATE_COLUMN, TIMESTAMP_COLUMN) VALUES (1000, 'Test_Type_NormalValue', 1000000, 'str_normal_value', 1, 10000000.10001, sysdate, sysdate)"),
		/*MySQLSQL*/("INSERT INTO TABLE_TEST_DAITEST(LINE_ID, TEST_TYPE, INT_COLUMN, STR_COLUMN, BOOL_COLUMN, REAL_COLUMN,\
 DATE_COLUMN, TIMESTAMP_COLUMN) VALUES (1000, 'Test_Type_NormalValue', 1000000, 'str_normal_value', 1, 10000000.10001, sysdate(), sysdate())"),
		/*SQLiteSQL*/("INSERT INTO TABLE_TEST_DAITEST(LINE_ID, TEST_TYPE, INT_COLUMN, STR_COLUMN, BOOL_COLUMN, REAL_COLUMN\
 ) VALUES (1000, 'Test_Type_NormalValue', 1000000, 'str_normal_value', 1, 10000000.10001)")	
	},
	{
		/*SQLID*/("TABLETESTDAITEST_CREATE_6000"),
		/*CommonSQL*/(""),
		/*OracleSQL*/("CREATE TABLE TABLE_TEST_DAITEST(LINE_ID NUMBER, TEST_TYPE VARCHAR2(200), INT_COLUMN NUMBER, STR_COLUMN VARCHAR2(200), BOOL_COLUMN NUMBER, REAL_COLUMN NUMBER(38,8),\
 DATE_COLUMN DATE, TIMESTAMP_COLUMN TIMESTAMP)"),
		/*MySQLSQL*/("CREATE TABLE TABLE_TEST_DAITEST(LINE_ID int, TEST_TYPE VARCHAR(200), INT_COLUMN int, STR_COLUMN VARCHAR(200), BOOL_COLUMN int, REAL_COLUMN double,\
 DATE_COLUMN datetime, TIMESTAMP_COLUMN TIMESTAMP)"),
		/*SQLiteSQL*/("CREATE TABLE TABLE_TEST_DAITEST(LINE_ID int, TEST_TYPE VARCHAR(200), INT_COLUMN int, STR_COLUMN VARCHAR(200), BOOL_COLUMN int, REAL_COLUMN double,\
 DATE_COLUMN DateTime, TIMESTAMP_COLUMN DateTime)")	
	},

//[1][AlarmRule]
	{
		/*SQLID*/("ALARMRULE_SELECT_1001"),
		/*CommonSQL*/("SELECT pkey, ruleType FROM alarmrule"),
		/*OracleSQL*/(""),
		/*MySQLSQL*/(""),
		/*SQLiteSQL*/("")	
	},

//[1][AlarmRule]
	{
		/*SQLID*/("ALARMRULE_SELECT_1002"),
		/*CommonSQL*/(""),
		/*OracleSQL*/("SELECT TO_CHAR(DATE_CREATED,'YYYYMMDDHH24MISS') from alarmrule where pkey = %s"),
		/*MySQLSQL*/("SELECT DATE_FORMAT(DATE_CREATED,'%%Y%%m%%d%%H%%i%%S') from alarmrule where pkey = %s"),
		/*SQLiteSQL*/("SELECT TO_CHAR(DATE_CREATED,'YYYYMMDDHH24MISS') from alarmrule where pkey = %s")	
	},

//[2][AlarmRuleType]
	{
		/*SQLID*/("ALARMRULETYPE_SELECT_1001"),
		/*CommonSQL*/("SELECT pkey, ruleType FROM alarmruletype"),
		/*OracleSQL*/(""),
		/*MySQLSQL*/(""),
		/*SQLiteSQL*/("")	
	},

//[2][AlarmRuleType]
	{
		/*SQLID*/("ALARMRULETYPE_SELECT_1002"),
		/*CommonSQL*/(""),
		/*OracleSQL*/("SELECT TO_CHAR(DATE_CREATED,'YYYYMMDDHH24MISS') from alarmruletype where pkey = %s"),
		/*MySQLSQL*/("SELECT DATE_FORMAT(DATE_CREATED,'%%Y%%m%%d%%H%%i%%S') from alarmruletype where pkey = %s"),
		/*SQLiteSQL*/("SELECT TO_CHAR(DATE_CREATED,'YYYYMMDDHH24MISS') from alarmruletype where pkey = %s")	
	},


}; //static const st_sql_data  s_SQLArray[]  


 #endif//SQL_H           

