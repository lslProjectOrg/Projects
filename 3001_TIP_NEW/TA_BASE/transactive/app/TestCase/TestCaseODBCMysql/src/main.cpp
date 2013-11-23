//#include "vld.h"
//#include <odbcinst.h>
//SqlConfigDataSource odbcinst.h  odbccp32.lib ODBCCP32.DLL
//#import "C:\LSL\SVNWork\MT\3001_TIP_NEW\bin\win32_nd\MSADO15.DLL" no_namespace rename("EOF","adoEOF")


//SQLConfigDataSource(NULL,ODBC_ADD_DSN,"MySQL ODBC 5.2 Unicode Driver","DSN=odbctest\0 UID=root\0 PWD=\0 SERVER=localhost\0 DATABASE=odbc\0\0");


#include <windows.h> 
#include "stdio.h"
#include <sql.h>
#include <sqlext.h>
#include <odbcinst.h>

#pragma comment(lib,"odbc32.lib")
#pragma comment(lib,"odbccp32.lib")


int test_fun_odbc_api_getdata();
int test_fun_odbc_api_dynDNS();
int test_fun_odbc_api_DBPOOL();
int test_fun_odbc_api_DBPOOL_Sample();


int main()
{
	int nFunRes = 0;
	//test_fun_odbc_api_getdata();

	//test_fun_odbc_api_dynDNS();

	test_fun_odbc_api_DBPOOL_Sample();

	return nFunRes;
}




int test_fun_odbc_api_DBPOOL_Sample()
{
// #include <windows.h>
// #include <stdio.h>
// #include <sql.h>
// #include <sqlext.h>
	SQLHENV   henv;
	SQLHDBC   hdbc;
	int       i=0;

	if (!SQL_SUCCEEDED(SQLSetEnvAttr(
		NULL,  // make process level cursor pooling
		SQL_ATTR_CONNECTION_POOLING,
		(SQLPOINTER)SQL_CP_ONE_PER_DRIVER,
		SQL_IS_INTEGER)))
		printf("SQLSetEnvAttr/SQL_ATTR_CONNECTION_POOLING error\n");

	if (!SQL_SUCCEEDED(SQLAllocHandle(SQL_HANDLE_ENV, NULL, &henv)))
		printf("SQLAllocHandle error\n");

	// set the ODBC behavior version.
	if (!SQL_SUCCEEDED(SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION,
		(SQLPOINTER) SQL_OV_ODBC2, SQL_IS_INTEGER)))
		printf("SQLSetEnvAttr/SQL_ATTR_ODBC_VERSION error\n");

	//set the matching condition for using an existing connection in the pool
		if (!SQL_SUCCEEDED(SQLSetEnvAttr(henv, SQL_ATTR_CP_MATCH,
			(SQLPOINTER) SQL_CP_RELAXED_MATCH, SQL_IS_INTEGER)))
			printf("SQLSetEnvAttr/SQL_ATTR_CP_MATCH error\n");

	while (i < 10) 
	{
		if (!SQL_SUCCEEDED(SQLAllocHandle(SQL_HANDLE_DBC,
			henv, &hdbc)))
			printf("SQLAllocHandle error\n");


		if (!SQL_SUCCEEDED(SQLConnect(hdbc,
			(unsigned char*)"Mysql12343DYN\0", SQL_NTS,
			(unsigned char*)"IT271350_5\0", SQL_NTS,
			(unsigned char*)"IT271350_5\0", SQL_NTS)))
			printf("SQLConnect error\n");
		else
			printf("Connect successfully %d times\n", i);
		//When the ODBC application calls SQLDisconnect the first time, the connection is saved to the pool. 
		//Any subsequent SQLConnect/SQLDisconnect that matches the required condition will reuse the first connection.
		
		
		
		//the first time, the application calls SQLDisconenct, it will return
		//the connection to the //pool
		SQLDisconnect(hdbc);

		if (!SQL_SUCCEEDED(SQLFreeHandle(SQL_HANDLE_DBC, hdbc)))
			printf("SQLFreeHandle error\n");
		i++;
	}
	SQLFreeHandle(SQL_HANDLE_ENV, henv);

	return 0;
}

int test_fun_odbc_api_DBPOOL()
{
	int nFunRes = 0;

	SQLConfigDataSource(NULL, ODBC_ADD_DSN, "MySQL ODBC 5.2 Unicode Driver",
		"DSN=Mysql12343DYN;UID=IT271350_5;PWD=IT271350_5;SERVER=192.168.123.43;DATABASE=tra_occ");


	SQLHENV m_hEnviroment;
	SQLHDBC m_hDatabaseConnection;
	SQLHSTMT m_hStatement;
	SQLRETURN retcode;

	////http://support.microsoft.com/kb/164221/en-us

	////http://www.informixchina.net/club/thread-5924-1-1.html

	/*Allocate environment handle */
	retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_hEnviroment);
	if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) 
	{
		return nFunRes;
	}
	// set the ODBC behavior version.
	/* Set the ODBC version environment attribute */
	retcode = SQLSetEnvAttr(m_hEnviroment, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);
	if (!SQL_SUCCEEDED(retcode))  
	{
		retcode = SQLFreeHandle(SQL_HANDLE_ENV, m_hEnviroment);
		return nFunRes;
	}


	// make process level cursor pooling
	retcode = SQLSetEnvAttr(m_hEnviroment, SQL_ATTR_CONNECTION_POOLING,	(SQLPOINTER)SQL_CP_ONE_PER_DRIVER, SQL_IS_UINTEGER);
	if (!SQL_SUCCEEDED(retcode)) 
	{
		retcode = SQLFreeHandle(SQL_HANDLE_ENV, m_hEnviroment);
		return nFunRes;
	}




	//set the matching condition for using an existing connection in the pool
	retcode = SQLSetEnvAttr(m_hEnviroment, SQL_ATTR_CP_MATCH, (SQLPOINTER)SQL_CP_RELAXED_MATCH, 0);//SQL_IS_INTEGER
	if (!SQL_SUCCEEDED(retcode))  
	{
		printf("SQLSetEnvAttr/SQL_ATTR_CP_MATCH error\n");
		retcode = SQLFreeHandle(SQL_HANDLE_ENV, m_hEnviroment);
		return nFunRes;
	}


	int nIndex = 0;
	while (nIndex < 10) 
	{
		/* Allocate connection handle */
		retcode = SQLAllocHandle(SQL_HANDLE_DBC, m_hEnviroment, &m_hDatabaseConnection);
		if (!SQL_SUCCEEDED(retcode))  
		{
			printf("SQLAllocHandle error\n");
			retcode = SQLFreeHandle(SQL_HANDLE_ENV, m_hEnviroment);
			return nFunRes;
		}



		/* Set login timeout to 5 seconds. */
		//SQLSetConnectAttr(hdbc, SQL_LOGIN_TIMEOUT, 5, 0);

		/* Connect to data source */
		retcode = SQLConnect(m_hDatabaseConnection, 
			(SQLCHAR*) "Mysql12343DYN", SQL_NTS,
			(SQLCHAR*) "IT271350_5", SQL_NTS,
			(SQLCHAR*) "IT271350_5", SQL_NTS);
		if (!SQL_SUCCEEDED(retcode))  
		{
			printf("SQLConnect error\n");
			retcode = SQLFreeHandle(SQL_HANDLE_DBC, m_hDatabaseConnection);
			retcode = SQLFreeHandle(SQL_HANDLE_ENV, m_hEnviroment);
			return nFunRes;
		}
		else
		{
			printf("Connect successfully %d times\n", nIndex);
		}


		//////////////////////////////////////////////////////////////////////////
		
		/* Allocate statement handle */
		retcode = SQLAllocHandle(SQL_HANDLE_STMT, m_hDatabaseConnection, &m_hStatement);
		if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) 
		{
			retcode = SQLDisconnect(m_hDatabaseConnection);
			retcode = SQLFreeHandle(SQL_HANDLE_DBC, m_hDatabaseConnection);
			retcode = SQLFreeHandle(SQL_HANDLE_ENV, m_hEnviroment);
			return nFunRes;
		}

		retcode = SQLExecDirect(m_hStatement,(SQLCHAR*)"select pkey from recipient_info", SQL_NTS);
		if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
		{
			retcode = SQLFreeHandle(SQL_HANDLE_STMT, m_hStatement);
			retcode = SQLDisconnect(m_hDatabaseConnection);
			retcode = SQLFreeHandle(SQL_HANDLE_DBC, m_hDatabaseConnection);
			retcode = SQLFreeHandle(SQL_HANDLE_ENV, m_hEnviroment);
			return nFunRes;
		}


		/* get return data Row count */
		SQLINTEGER l_siIdCount = 0;
		retcode = SQLRowCount(m_hStatement, &l_siIdCount);
		if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
		{
			retcode = SQLFreeHandle(SQL_HANDLE_STMT, m_hStatement);
			retcode = SQLDisconnect(m_hDatabaseConnection);
			retcode = SQLFreeHandle(SQL_HANDLE_DBC, m_hDatabaseConnection);
			retcode = SQLFreeHandle(SQL_HANDLE_ENV, m_hEnviroment);
			return nFunRes;
		}

		while (true)
		{
			//fecth data use SQLFetch() return SQL_SUCCESS  then no data
			retcode = SQLFetch(m_hStatement);
			if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
			{
				break;
			}

			SQLINTEGER l_siID;
			SQLINTEGER l_siIDLength = 0;

			retcode = SQLGetData(m_hStatement, 1, SQL_C_ULONG, &l_siID, 0, &l_siIDLength);
			if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
		}//while (true)


		//the first time, the application calls SQLDisconenct, it will return
		//the connection to the //pool
		retcode = SQLFreeHandle(SQL_HANDLE_STMT, m_hStatement);
		retcode = SQLDisconnect(m_hDatabaseConnection);
		retcode = SQLFreeHandle(SQL_HANDLE_DBC, m_hDatabaseConnection);

		//////////////////////////////////////////////////////////////////////////

		nIndex++;
	}//while (nIndex < 10) 
	
	retcode = SQLFreeHandle(SQL_HANDLE_ENV, m_hEnviroment);


	SQLConfigDataSource(NULL, ODBC_REMOVE_DSN, "MySQL ODBC 5.2 Unicode Driver",
		"DSN=Mysql12343DYN;UID=IT271350_5;PWD=IT271350_5;SERVER=192.168.123.43;DATABASE=tra_occ");

	return nFunRes;
}


int test_fun_odbc_api_dynDNS()
{
	int nFunRes = 0;

	SQLConfigDataSource(NULL, ODBC_ADD_DSN, "MySQL ODBC 5.2 Unicode Driver",
		"DSN=Mysql12343DYN;UID=IT271350_5;PWD=IT271350_5;SERVER=192.168.123.43;DATABASE=tra_occ");


	SQLHENV m_hEnviroment;
	SQLHDBC m_hDatabaseConnection;
	SQLHSTMT m_hStatement;
	SQLRETURN retcode;

	/*Allocate environment handle */
	retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_hEnviroment);
	if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) 
	{
		return nFunRes;
	}

	/* Set the ODBC version environment attribute */
	retcode = SQLSetEnvAttr(m_hEnviroment, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
	if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) 
	{
		retcode = SQLFreeHandle(SQL_HANDLE_ENV, m_hEnviroment);
		return nFunRes;
	}

	/* Allocate connection handle */
	retcode = SQLAllocHandle(SQL_HANDLE_DBC, m_hEnviroment, &m_hDatabaseConnection);
	if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) 
	{
		retcode = SQLFreeHandle(SQL_HANDLE_ENV, m_hEnviroment);
		return nFunRes;
	}


	/* Set login timeout to 5 seconds. */
	//SQLSetConnectAttr(hdbc, SQL_LOGIN_TIMEOUT, 5, 0);

	/* Connect to data source */
	retcode = SQLConnect(m_hDatabaseConnection, 
		(SQLCHAR*) "Mysql12343DYN", SQL_NTS,
		(SQLCHAR*) "IT271350_5", SQL_NTS,
		(SQLCHAR*) "IT271350_5", SQL_NTS);
	if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) 
	{
		retcode = SQLFreeHandle(SQL_HANDLE_DBC, m_hDatabaseConnection);
		retcode = SQLFreeHandle(SQL_HANDLE_ENV, m_hEnviroment);
		return nFunRes;
	}


	/* Allocate statement handle */
	retcode = SQLAllocHandle(SQL_HANDLE_STMT, m_hDatabaseConnection, &m_hStatement);
	if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) 
	{
		retcode = SQLDisconnect(m_hDatabaseConnection);
		retcode = SQLFreeHandle(SQL_HANDLE_DBC, m_hDatabaseConnection);
		retcode = SQLFreeHandle(SQL_HANDLE_ENV, m_hEnviroment);
		return nFunRes;
	}

	retcode = SQLExecDirect(m_hStatement,(SQLCHAR*)"select pkey from recipient_info", SQL_NTS);
	if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
	{
		retcode = SQLFreeHandle(SQL_HANDLE_STMT, m_hStatement);
		retcode = SQLDisconnect(m_hDatabaseConnection);
		retcode = SQLFreeHandle(SQL_HANDLE_DBC, m_hDatabaseConnection);
		retcode = SQLFreeHandle(SQL_HANDLE_ENV, m_hEnviroment);
		return nFunRes;
	}


	/* get return data Row count */
	SQLINTEGER l_siIdCount = 0;
	retcode = SQLRowCount(m_hStatement, &l_siIdCount);
	if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
	{
		retcode = SQLFreeHandle(SQL_HANDLE_STMT, m_hStatement);
		retcode = SQLDisconnect(m_hDatabaseConnection);
		retcode = SQLFreeHandle(SQL_HANDLE_DBC, m_hDatabaseConnection);
		retcode = SQLFreeHandle(SQL_HANDLE_ENV, m_hEnviroment);
		return nFunRes;
	}

	while (true)
	{
		//fecth data use SQLFetch() return SQL_SUCCESS  then no data
		retcode = SQLFetch(m_hStatement);
		if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
		{
			break;
		}

		SQLINTEGER l_siID;
		SQLINTEGER l_siIDLength = 0;

		retcode = SQLGetData(m_hStatement, 1, SQL_C_ULONG, &l_siID, 0, &l_siIDLength);
		if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
		{
			break;
		}
	}//while (true)


	/* Process data */
	retcode = SQLFreeHandle(SQL_HANDLE_STMT, m_hStatement);
	retcode = SQLDisconnect(m_hDatabaseConnection);
	retcode = SQLFreeHandle(SQL_HANDLE_DBC, m_hDatabaseConnection);
	retcode = SQLFreeHandle(SQL_HANDLE_ENV, m_hEnviroment);



	SQLConfigDataSource(NULL, ODBC_REMOVE_DSN, "MySQL ODBC 5.2 Unicode Driver",
		"DSN=Mysql12343DYN;UID=IT271350_5;PWD=IT271350_5;SERVER=192.168.123.43;DATABASE=tra_occ");

	return nFunRes;
}



int test_fun_odbc_api_getdata()
{
	int nFunRes = 0;

	SQLHENV m_hEnviroment;
	SQLHDBC m_hDatabaseConnection;
	SQLHSTMT m_hStatement;
	SQLRETURN retcode;

	/*Allocate environment handle */
	retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_hEnviroment);
	if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) 
	{
		return nFunRes;
	}

	/* Set the ODBC version environment attribute */
	retcode = SQLSetEnvAttr(m_hEnviroment, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
	if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) 
	{
		retcode = SQLFreeHandle(SQL_HANDLE_ENV, m_hEnviroment);
		return nFunRes;
	}

	/* Allocate connection handle */
	retcode = SQLAllocHandle(SQL_HANDLE_DBC, m_hEnviroment, &m_hDatabaseConnection);
	if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) 
	{
		retcode = SQLFreeHandle(SQL_HANDLE_ENV, m_hEnviroment);
		return nFunRes;
	}


	/* Set login timeout to 5 seconds. */
	//SQLSetConnectAttr(hdbc, SQL_LOGIN_TIMEOUT, 5, 0);

	/* Connect to data source */
	retcode = SQLConnect(m_hDatabaseConnection, 
		(SQLCHAR*) "Mysql12343", SQL_NTS,
		(SQLCHAR*) "IT271350_5", SQL_NTS,
		(SQLCHAR*) "IT271350_5", SQL_NTS);
	if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) 
	{
		retcode = SQLFreeHandle(SQL_HANDLE_DBC, m_hDatabaseConnection);
		retcode = SQLFreeHandle(SQL_HANDLE_ENV, m_hEnviroment);
		return nFunRes;
	}


	/* Allocate statement handle */
	retcode = SQLAllocHandle(SQL_HANDLE_STMT, m_hDatabaseConnection, &m_hStatement);
	if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) 
	{
		retcode = SQLDisconnect(m_hDatabaseConnection);
		retcode = SQLFreeHandle(SQL_HANDLE_DBC, m_hDatabaseConnection);
		retcode = SQLFreeHandle(SQL_HANDLE_ENV, m_hEnviroment);
		return nFunRes;
	}

	retcode = SQLExecDirect(m_hStatement,(SQLCHAR*)"select pkey from recipient_info", SQL_NTS);
	if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
	{
		retcode = SQLFreeHandle(SQL_HANDLE_STMT, m_hStatement);
		retcode = SQLDisconnect(m_hDatabaseConnection);
		retcode = SQLFreeHandle(SQL_HANDLE_DBC, m_hDatabaseConnection);
		retcode = SQLFreeHandle(SQL_HANDLE_ENV, m_hEnviroment);
		return nFunRes;
	}


	/* get return data Row count */
	SQLINTEGER l_siIdCount = 0;
	retcode = SQLRowCount(m_hStatement, &l_siIdCount);
	if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
	{
		retcode = SQLFreeHandle(SQL_HANDLE_STMT, m_hStatement);
		retcode = SQLDisconnect(m_hDatabaseConnection);
		retcode = SQLFreeHandle(SQL_HANDLE_DBC, m_hDatabaseConnection);
		retcode = SQLFreeHandle(SQL_HANDLE_ENV, m_hEnviroment);
		return nFunRes;
	}

	while (true)
	{
		//fecth data use SQLFetch() return SQL_SUCCESS  then no data
		retcode = SQLFetch(m_hStatement);
		if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
		{
			break;
		}

		SQLINTEGER l_siID;
		SQLINTEGER l_siIDLength = 0;

		retcode = SQLGetData(m_hStatement, 1, SQL_C_ULONG, &l_siID, 0, &l_siIDLength);
		if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
		{
			break;
		}
	}//while (true)


	/* Process data */
	retcode = SQLFreeHandle(SQL_HANDLE_STMT, m_hStatement);
	retcode = SQLDisconnect(m_hDatabaseConnection);
	retcode = SQLFreeHandle(SQL_HANDLE_DBC, m_hDatabaseConnection);
	retcode = SQLFreeHandle(SQL_HANDLE_ENV, m_hEnviroment);

	return nFunRes;
}


#define  MAXBUFLEN 512
#define  MAXNAME 512

void ProcessLogMessages(
	SQLSMALLINT plm_handle_type,//出现错误时所使用的ODBC句柄类型，取值为：SQL_HANDLE_ENV ，SQL_HANDLE_DBC ，SQL_HANDLE_STMT
	SQLHANDLE plm_handle, //出现错误时所使用的ODBC句柄
	char *logstring, //标题字符串
	int ConnInd //指明句柄是否为DBC句柄
	)
{
	RETCODE      plm_retcode = SQL_SUCCESS;
	UCHAR      plm_szSqlState[MAXBUFLEN] = "";
	UCHAR	   plm_szErrorMsg[MAXBUFLEN] = "";
	SDWORD      plm_pfNativeError = 0L;
	SWORD      plm_pcbErrorMsg = 0;
	SQLSMALLINT   plm_cRecNmbr = 1;
	SDWORD      plm_SS_MsgState = 0;
	SDWORD      plm_SS_Severity = 0;
	SQLINTEGER   plm_Rownumber = 0;
	USHORT      plm_SS_Line;
	SQLSMALLINT   plm_cbSS_Procname, plm_cbSS_Srvname;
	SQLCHAR      plm_SS_Procname[MAXNAME], plm_SS_Srvname[MAXNAME];

	printf(logstring);

	while (plm_retcode != SQL_NO_DATA_FOUND) 
	{
		plm_retcode = SQLGetDiagRec(plm_handle_type, plm_handle,
			plm_cRecNmbr, plm_szSqlState, &plm_pfNativeError,
			plm_szErrorMsg, MAXBUFLEN - 1, &plm_pcbErrorMsg);

		// Note that if the application has not yet made a
		// successful connection, the SQLGetDiagField
		// information has not yet been cached by ODBC
		// Driver Manager and these calls to SQLGetDiagField
		// will fail.

		if (plm_retcode != SQL_NO_DATA_FOUND)
		{
			if (ConnInd) 
			{
				plm_retcode = SQLGetDiagField(
					plm_handle_type, plm_handle, plm_cRecNmbr,
					SQL_DIAG_ROW_NUMBER, &plm_Rownumber,
					SQL_IS_INTEGER,
					NULL);
			}

			printf("szSqlState = %s/n",plm_szSqlState);
			printf("pfNativeError = %d/n",plm_pfNativeError);
			printf("szErrorMsg = %s/n",plm_szErrorMsg);
			printf("pcbErrorMsg = %d/n/n",plm_pcbErrorMsg);

			if (ConnInd) 
			{
				printf("ODBCRowNumber = %d/n", plm_Rownumber);
				printf("SSrvrLine = %d/n", plm_Rownumber);
				printf("SSrvrMsgState = %d/n",plm_SS_MsgState);
				printf("SSrvrSeverity = %d/n",plm_SS_Severity);
				printf("SSrvrProcname = %s/n",plm_SS_Procname);
				printf("SSrvrSrvname = %s/n/n",plm_SS_Srvname);
			}

		}

		plm_cRecNmbr++; //Increment to next diagnostic record.

	} // End while.

}







