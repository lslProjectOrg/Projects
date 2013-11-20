
#include "CommonDef.h"
#include "CommonData.h"

#include "TestOCIEnqDeq.h"

using namespace std;




#define  defDefault_Oracle_DbName                         "TRANSACT"  // TRANSACT 192.168.253.225
#define  defDefault_Oracle_DbUser                         "IT271350_5"
#define  defDefault_Oracle_DbPwd                          "IT271350_5"

#define  defDefault_Oracle_QueueName                      "AUDIT_DATA_QUEUE"
#define  defDefault_Server_AgentName                      "TRANSACT"
#define  defDefault_Client_AgentName                      "TRA_OCC"


#define  defDefault_defConfig_PLType					  "AUDIT_DATA_QUEUE_PAYLOAD_TYPE"

#define  defDefault_defConfig_PLTypeMessageQualifier	  "message_qualifier"
#define  defDefault_defConfig_PLTypeAuditDataMysql	      "audit_data_mysql"
#define  defDefault_defConfig_PLTypeAuditDataOracle       "audit_data_oracle"


#define  defDefault_Printf									200
#define  defDefault_AQ									    20000

#define  def_int_Buf_Size									512
#define  def_int_Deq_Total									100*10000
#define  def_int_Enq_Total									100*10000



/************************************************************************/
 

/////////////////////////////////////////////////////////
// Test Methods implementations
/////////////////////////////////////////////////////////

/*******************************************************
* Test: insert, update, select, delete one alarm row in 
*       the database.
* Expected: all the test operations can work well
********************************************************/


/***********************************************************
* Test: invoke the test thread methods and do the multi-thread
*      
* Expected: all the test operations can work well
***********************************************************/
void CTestOCIEnqDeq::testDaiGetSetValue_Muti_thread()
{

#ifdef WIN32   
// 	HANDLE hThread[5] = {NULL};
// 	hThread[0] = (HANDLE)_beginthread(test_DaiGetSetValue_one_thread, 0, this);
// 	hThread[1] = (HANDLE)_beginthread(test_DaiGetSetValue_two_thread, 0, this);
// 
// 	::WaitForMultipleObjects(2, hThread, true, INFINITE);

#else
	 
#endif	


}






//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

std::string CTestOCIEnqDeq::_GetCurrentTime(struct timeb* pfbtime)
{
	struct tm*			pTm = NULL;
	std::string			strCurrentTime;
	char				szCurTime[256] = {0};

	if (NULL != pfbtime)
	{
		ftime(pfbtime);	
		pTm = localtime(&(pfbtime->time));
		if ( NULL != pTm && NULL != pfbtime)
		{
			sprintf(szCurTime, "[%02d/%02d/%02d %02d:%02d:%02d:%03d]",
				pTm->tm_year + 1900, pTm->tm_mon + 1, pTm->tm_mday,
				pTm->tm_hour, pTm->tm_min, pTm->tm_sec, pfbtime->millitm); 

			strCurrentTime = szCurTime;     
		}	  

	}

	return strCurrentTime;	 
}


std::string CTestOCIEnqDeq::_GetDiffTime(struct timeb* pfbtimeBegin, struct timeb* pfbtimeEnd)
{  
	std::string			strDiffTime;
	char				szCurTime[256] = {0};
	time_t				diffSeconds;
	double				diffAll = 0;	

	if (NULL != pfbtimeBegin && NULL != pfbtimeEnd)
	{
		diffSeconds = pfbtimeEnd->time - pfbtimeBegin->time;
		diffAll = (diffSeconds*1000.0 + pfbtimeEnd->millitm - pfbtimeBegin->millitm);
		sprintf(szCurTime, "diff Begin End is [%10.3f ms]", diffAll); 
		strDiffTime =	szCurTime;
	}

	return strDiffTime;	 
}



/************************************************************************/
/*                                                                      */
/************************************************************************/
  
int CTestOCIEnqDeq::_InitMyOCIHandle(MyOCIHandle *pMyOCIHandle)
{
	std::string strOracleTNSName= defDefault_Oracle_DbName;//
	std::string strOracleUser = defDefault_Oracle_DbUser;	 
	std::string strOraclePwd = defDefault_Oracle_DbPwd;	

	
	std::string strOracleQueueName =	defDefault_Oracle_QueueName;
	std::string strServerAgentName =	defDefault_Server_AgentName;
	std::string strClientAgentName =	defDefault_Client_AgentName;

	std::string strAQPLType =	defDefault_defConfig_PLType;
	
	int nErrNo = 0;
	char szErrBuf[def_int_Buf_Size]={'\0'};
	sb4 nErrCode = 0;


	if (NULL == pMyOCIHandle)
	{
		return -1;
	}
	
	
	pMyOCIHandle->m_pOCIEnv = NULL;
	pMyOCIHandle->m_swResult = OCI_SUCCESS;
	pMyOCIHandle->m_pOCIError = NULL;
	pMyOCIHandle->m_nOCIEnvInitMode = OCI_DEFAULT | OCI_OBJECT | OCI_THREADED;


/*	enq_nFunRes = OCI_Initialize(NULL, NULL, OCI_ENV_DEFAULT);*/
	// 创建或初始化 OCI 环境 , 并设置环境句柄。
	pMyOCIHandle->m_swResult = OCIEnvCreate(&(pMyOCIHandle->m_pOCIEnv), pMyOCIHandle->m_nOCIEnvInitMode, NULL, NULL, NULL, NULL, 0, NULL);  //OCI_OBJECT   OCI_DEFAULT
   	if (pMyOCIHandle->m_swResult != OCI_SUCCESS && pMyOCIHandle->m_swResult != OCI_SUCCESS_WITH_INFO)
	{
		std::cout << "error! Oracle OCIEnvCreate environment initialization error!" << std::endl;
	}

	// 产生一个指定环境的错误句柄
	pMyOCIHandle->m_swResult = OCIHandleAlloc((dvoid *)(pMyOCIHandle->m_pOCIEnv), (dvoid **)&(pMyOCIHandle->m_pOCIError), OCI_HTYPE_ERROR, (size_t)0, (dvoid **)0);
	if (OCI_SUCCESS != pMyOCIHandle->m_swResult)
	{
		std::cout << "error! Oracle OCIHandleAlloc OCI_HTYPE_ERROR error!" << std::endl;
	}
	
	
	return 0;
}

int CTestOCIEnqDeq::_UninitMyOCIHandle(MyOCIHandle *pMyOCIHandle)
{
	int nErrNo = 0;
	char szErrBuf[def_int_Buf_Size]={'\0'};
	sb4 nErrCode = 0;
	
	if (NULL == pMyOCIHandle)
	{
		return -1;
	}
	
	
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/

	pMyOCIHandle->m_swResult = OCIHandleFree((dvoid *)pMyOCIHandle->m_pOCIError, OCI_HTYPE_ERROR);
	pMyOCIHandle->m_pOCIError = NULL;
	if (OCI_SUCCESS != pMyOCIHandle->m_swResult)
	{	
		memset(szErrBuf, 0,def_int_Buf_Size);  
		OCIErrorGet((dvoid *)pMyOCIHandle->m_pOCIError, (ub4)1, (text *)NULL, &nErrCode, (ub1 *)szErrBuf, (ub4)sizeof(szErrBuf), OCI_HTYPE_ERROR);
		nErrNo = nErrCode;
		std::cout << "error! nErrNo=%d szErrBuf=%s" << nErrNo<<szErrBuf << std::endl;
	}	

	pMyOCIHandle->m_swResult = OCIHandleFree((dvoid *)pMyOCIHandle->m_pOCIEnv,OCI_HTYPE_ENV);
	pMyOCIHandle->m_pOCIEnv = NULL; 
	if (OCI_SUCCESS != pMyOCIHandle->m_swResult)
	{	
		memset(szErrBuf, 0,def_int_Buf_Size);  
		OCIErrorGet((dvoid *)pMyOCIHandle->m_pOCIError, (ub4)1, (text *)NULL, &nErrCode, (ub1 *)szErrBuf, (ub4)sizeof(szErrBuf), OCI_HTYPE_ERROR);
		nErrNo = nErrCode;
		std::cout << "error! nErrNo=%d szErrBuf=%s" << nErrNo<<szErrBuf << std::endl;
	}
	
	
	return 0;
}


int CTestOCIEnqDeq::_InitMyOCIHandleEnq(MyOCIHandle *pOCI, MyOCIHandleEnq *pMyOCIHandleEnq)
{
	std::string strOracleTNSName= defDefault_Oracle_DbName;//
	std::string strOracleUser = defDefault_Oracle_DbUser;	 
	std::string strOraclePwd = defDefault_Oracle_DbPwd;	

	
	std::string strOracleQueueName =	defDefault_Oracle_QueueName;
	std::string strServerAgentName =	defDefault_Server_AgentName;
	std::string strClientAgentName =	defDefault_Client_AgentName;

	std::string strAQPLType =	defDefault_defConfig_PLType;
	
	int nErrNo = 0;
	char szErrBuf[def_int_Buf_Size]={'\0'};
	sb4 nErrCode = 0;


	if (NULL == pMyOCIHandleEnq)
	{
		return -1;
	}
	
	
	pMyOCIHandleEnq->m_swResult = OCI_SUCCESS;

	pMyOCIHandleEnq->m_pOCIServerForEnq = NULL;
	pMyOCIHandleEnq->m_pOCISessionForEnq = NULL;
	pMyOCIHandleEnq->m_OCISvcCtxForEnq = NULL;
	pMyOCIHandleEnq->m_OCIDescribeForEnq = NULL;
	pMyOCIHandleEnq->m_OCITypeForEnq = NULL;
	pMyOCIHandleEnq->m_OCIAQAgentArrayForEnq[0]= NULL;
	pMyOCIHandleEnq->m_pOCIAQEnqOptionsForEnq = NULL;
	pMyOCIHandleEnq->m_pOCIAQMsgPropertiesForEnq = NULL;
	
	

	
	/************************************************************************/
	/* prepare for enq   begin                                              */
	/************************************************************************/	
	
	/*	enq_ConHandle = OCI_ConnectionCreate(defDefault_Oracle_DbName, defDefault_Oracle_DbUser, 
							defDefault_Oracle_DbPwd, OCI_SESSION_DEFAULT);*/
	/*OCI_ConnectionAttach*/						
	// 产生一个指定环境的服务句柄
	pMyOCIHandleEnq->m_swResult = OCIHandleAlloc((dvoid *)(pOCI->m_pOCIEnv), (dvoid **)&(pMyOCIHandleEnq->m_pOCIServerForEnq), OCI_HTYPE_SERVER, (size_t)0, (dvoid **)0);
	if (OCI_SUCCESS != pMyOCIHandleEnq->m_swResult)
	{	
		memset(szErrBuf, 0,def_int_Buf_Size);  
		OCIErrorGet((dvoid *)pOCI->m_pOCIError, (ub4)1, (text *)NULL, &nErrCode, (ub1 *)szErrBuf, (ub4)sizeof(szErrBuf), OCI_HTYPE_ERROR);
		nErrNo = nErrCode;
		std::cout << "error! OCIServerAttach Oracle server attach error:" << szErrBuf << std::endl;
	}

	// 连接 | 断开服务器 多用户方式连接 ( 创建一个存取路径到数据源 )	
	pMyOCIHandleEnq->m_swResult = OCIServerAttach(pMyOCIHandleEnq->m_pOCIServerForEnq, pOCI->m_pOCIError, (text *)strOracleTNSName.c_str(), strlen(strOracleTNSName.c_str()), 0);
	if (OCI_SUCCESS != pMyOCIHandleEnq->m_swResult)
	{	
		memset(szErrBuf, 0,def_int_Buf_Size);  
		OCIErrorGet((dvoid *)pOCI->m_pOCIError, (ub4)1, (text *)NULL, &nErrCode, (ub1 *)szErrBuf, (ub4)sizeof(szErrBuf), OCI_HTYPE_ERROR);
		nErrNo = nErrCode;
		std::cout << "error! OCIServerAttach Oracle server attach error:" << szErrBuf << std::endl;
	}
			
	 /*OCI_ConnectionLogon*/
	/***************** 连接数据库 ****************/ 
	// 产生一个指定环境的用户连接句柄
	pMyOCIHandleEnq->m_pOCISessionForEnq = NULL;
	pMyOCIHandleEnq->m_swResult = OCIHandleAlloc((dvoid *)(pOCI->m_pOCIEnv), (dvoid **)&(pMyOCIHandleEnq->m_pOCISessionForEnq), (ub4)OCI_HTYPE_SESSION, (size_t) 0, (dvoid **)0);
	if (OCI_SUCCESS != pMyOCIHandleEnq->m_swResult)
	{	
		memset(szErrBuf, 0,def_int_Buf_Size);  
		OCIErrorGet((dvoid *)pOCI->m_pOCIError, (ub4)1, (text *)NULL, &nErrCode, (ub1 *)szErrBuf, (ub4)sizeof(szErrBuf), OCI_HTYPE_ERROR);
		nErrNo = nErrCode;
		std::cout << "error! OCIServerAttach Oracle server attach error:" << szErrBuf << std::endl;
	}

	
	// 产生一个指定环境的服务上下文句柄
	pMyOCIHandleEnq->m_OCISvcCtxForEnq = NULL;
	pMyOCIHandleEnq->m_swResult = OCIHandleAlloc((dvoid *)(pOCI->m_pOCIEnv), (dvoid **)&(pMyOCIHandleEnq->m_OCISvcCtxForEnq), OCI_HTYPE_SVCCTX, (size_t) 0, (dvoid **)0);
	if (OCI_SUCCESS != pMyOCIHandleEnq->m_swResult)
	{	
		memset(szErrBuf, 0,def_int_Buf_Size);  
		OCIErrorGet((dvoid *)pOCI->m_pOCIError, (ub4)1, (text *)NULL, &nErrCode, (ub1 *)szErrBuf, (ub4)sizeof(szErrBuf), OCI_HTYPE_ERROR);
		nErrNo = nErrCode;
		std::cout << "error! OCIServerAttach Oracle server attach error:" << szErrBuf << std::endl;
	}

	// 为指定的句柄及描述符设置特定的属性 ( 为服务句柄指定服务上下文 )
	pMyOCIHandleEnq->m_swResult = OCIAttrSet((dvoid *)(pMyOCIHandleEnq->m_OCISvcCtxForEnq), OCI_HTYPE_SVCCTX, (dvoid *)(pMyOCIHandleEnq->m_pOCIServerForEnq), (ub4)0, OCI_ATTR_SERVER, (OCIError *)(pOCI->m_pOCIError));
	if (OCI_SUCCESS != pMyOCIHandleEnq->m_swResult)
	{	
		memset(szErrBuf, 0,def_int_Buf_Size);  
		OCIErrorGet((dvoid *)pOCI->m_pOCIError, (ub4)1, (text *)NULL, &nErrCode, (ub1 *)szErrBuf, (ub4)sizeof(szErrBuf), OCI_HTYPE_ERROR);
		nErrNo = nErrCode;
		std::cout << "error! OCIServerAttach Oracle server attach error:" << szErrBuf << std::endl;
	}
	  
	// 为用户连接句柄设置登录名及密码
	pMyOCIHandleEnq->m_swResult = OCIAttrSet((dvoid *)pMyOCIHandleEnq->m_pOCISessionForEnq, (ub4)OCI_HTYPE_SESSION, (dvoid *)strOracleUser.c_str(), (ub4)strlen(strOracleUser.c_str()), (ub4)OCI_ATTR_USERNAME, pOCI->m_pOCIError);
	pMyOCIHandleEnq->m_swResult = OCIAttrSet((dvoid *)pMyOCIHandleEnq->m_pOCISessionForEnq, (ub4)OCI_HTYPE_SESSION, (dvoid *)strOraclePwd.c_str(), (ub4)strlen(strOraclePwd.c_str()), (ub4)OCI_ATTR_PASSWORD, pOCI->m_pOCIError);
	if (OCI_SUCCESS != pMyOCIHandleEnq->m_swResult)
	{	
		memset(szErrBuf, 0,def_int_Buf_Size);  
		OCIErrorGet((dvoid *)pOCI->m_pOCIError, (ub4)1, (text *)NULL, &nErrCode, (ub1 *)szErrBuf, (ub4)sizeof(szErrBuf), OCI_HTYPE_ERROR);
		nErrNo = nErrCode;
		std::cout << "error! OCIServerAttach Oracle server attach error:" << szErrBuf << std::endl;
	}
	
	// 认证用户再建立一个会话连接
	pMyOCIHandleEnq->m_swResult = OCISessionBegin(pMyOCIHandleEnq->m_OCISvcCtxForEnq, pOCI->m_pOCIError, pMyOCIHandleEnq->m_pOCISessionForEnq, OCI_CRED_RDBMS, (ub4)OCI_DEFAULT);
	if (OCI_SUCCESS != pMyOCIHandleEnq->m_swResult)
	{	
		memset(szErrBuf, 0,def_int_Buf_Size);  
		OCIErrorGet((dvoid *)pOCI->m_pOCIError, (ub4)1, (text *)NULL, &nErrCode, (ub1 *)szErrBuf, (ub4)sizeof(szErrBuf), OCI_HTYPE_ERROR);
		nErrNo = nErrCode;
		std::cout << "error! OCIServerAttach Oracle server attach error:" << szErrBuf << std::endl;
	}
	
	
	pMyOCIHandleEnq->m_swResult = OCIAttrSet((dvoid *)pMyOCIHandleEnq->m_OCISvcCtxForEnq, (ub4) OCI_HTYPE_SVCCTX, (dvoid *)pMyOCIHandleEnq->m_pOCISessionForEnq, (ub4)0, (ub4)OCI_ATTR_SESSION, pOCI->m_pOCIError);
	if (OCI_SUCCESS != pMyOCIHandleEnq->m_swResult)
	{	
		memset(szErrBuf, 0,def_int_Buf_Size);  
		OCIErrorGet((dvoid *)pOCI->m_pOCIError, (ub4)1, (text *)NULL, &nErrCode, (ub1 *)szErrBuf, (ub4)sizeof(szErrBuf), OCI_HTYPE_ERROR);
		nErrNo = nErrCode;
		std::cout << "error! OCIServerAttach Oracle server attach error:" << szErrBuf << std::endl;
	}



	//===OCI_TypeInfoGet===================================================================================================
	/*enq_EnqTypeInfoHandle = OCI_TypeInfoGet(enq_ConHandle, defDefault_defConfig_PLType, OCI_TIF_TYPE);*/
	pMyOCIHandleEnq->m_OCIDescribeForEnq = NULL;	
	pMyOCIHandleEnq->m_swResult = OCIHandleAlloc(pOCI->m_pOCIEnv, (dvoid **) (void *)&(pMyOCIHandleEnq->m_OCIDescribeForEnq), OCI_HTYPE_DESCRIBE, (size_t) 0, (dvoid **) NULL);
	if (OCI_SUCCESS != pMyOCIHandleEnq->m_swResult)
	{	
		memset(szErrBuf, 0,def_int_Buf_Size);  
		OCIErrorGet((dvoid *)pOCI->m_pOCIError, (ub4)1, (text *)NULL, &nErrCode, (ub1 *)szErrBuf, (ub4)sizeof(szErrBuf), OCI_HTYPE_ERROR);
		nErrNo = nErrCode;
		std::cout << "error! OCIServerAttach Oracle server attach error:" << szErrBuf << std::endl;
	}
	
	pMyOCIHandleEnq->m_swResult = OCITypeByName(pOCI->m_pOCIEnv, pOCI->m_pOCIError, pMyOCIHandleEnq->m_OCISvcCtxForEnq, (CONST text *)"", (ub4) strlen(""),
		(CONST text *)strAQPLType.c_str(), (ub4) strlen(strAQPLType.c_str()),
		(text *)0, 0, OCI_DURATION_SESSION, OCI_TYPEGET_ALL, &(pMyOCIHandleEnq->m_OCITypeForEnq));
	if (OCI_SUCCESS != pMyOCIHandleEnq->m_swResult)
	{	
		memset(szErrBuf, 0,def_int_Buf_Size);  
		OCIErrorGet((dvoid *)pOCI->m_pOCIError, (ub4)1, (text *)NULL, &nErrCode, (ub1 *)szErrBuf, (ub4)sizeof(szErrBuf), OCI_HTYPE_ERROR);
		nErrNo = nErrCode;
		std::cout << "error! OCIServerAttach Oracle server attach error:" << szErrBuf << std::endl;
	}
		
	pMyOCIHandleEnq->m_swResult = OCIDescribeAny(pMyOCIHandleEnq->m_OCISvcCtxForEnq, pOCI->m_pOCIError, (void *)pMyOCIHandleEnq->m_OCITypeForEnq, 0, OCI_OTYPE_PTR, OCI_DEFAULT, OCI_PTYPE_TYPE, pMyOCIHandleEnq->m_OCIDescribeForEnq);
	if (OCI_SUCCESS != pMyOCIHandleEnq->m_swResult)
	{	
		memset(szErrBuf, 0,def_int_Buf_Size);  
		OCIErrorGet((dvoid *)pOCI->m_pOCIError, (ub4)1, (text *)NULL, &nErrCode, (ub1 *)szErrBuf, (ub4)sizeof(szErrBuf), OCI_HTYPE_ERROR);
		nErrNo = nErrCode;
		std::cout << "error! OCIServerAttach Oracle server attach error:" << szErrBuf << std::endl;
	}

	//==init enq=====================================================
	
	/*enq_EnqAgentArray[0] = OCI_AgentCreate(enq_ConHandle, defDefault_Server_AgentName, 	defDefault_Oracle_QueueName);*/		
	pMyOCIHandleEnq->m_swResult = OCIDescriptorAlloc(pOCI->m_pOCIEnv, (dvoid **)&(pMyOCIHandleEnq->m_OCIAQAgentArrayForEnq[0]), OCI_DTYPE_AQAGENT,	(size_t) 0, (dvoid **) NULL);
	if (OCI_SUCCESS != pMyOCIHandleEnq->m_swResult)
	{	
		memset(szErrBuf, 0,def_int_Buf_Size);  
		OCIErrorGet((dvoid *)pOCI->m_pOCIError, (ub4)1, (text *)NULL, &nErrCode, (ub1 *)szErrBuf, (ub4)sizeof(szErrBuf), OCI_HTYPE_ERROR);
		nErrNo = nErrCode;
		std::cout << "error! OCIServerAttach Oracle server attach error:" << szErrBuf << std::endl;
	}
	
		
	/*Agent name*/
	//swResult = OCIAttrSet((dvoid *)enq_EnqAgentArray[0], (ub4)OCI_DTYPE_AQAGENT, (dvoid *)"TRANSACT", (ub4)strlen("TRANSACT"), (ub4)OCI_ATTR_AGENT_NAME, errhpp);
	pMyOCIHandleEnq->m_swResult = OCIAttrSet((dvoid *)(pMyOCIHandleEnq->m_OCIAQAgentArrayForEnq[0]), (ub4)OCI_DTYPE_AQAGENT, (dvoid *)strServerAgentName.c_str(), (ub4)strServerAgentName.length(), (ub4)OCI_ATTR_AGENT_NAME, pOCI->m_pOCIError);
	if (OCI_SUCCESS != pMyOCIHandleEnq->m_swResult)
	{	
		memset(szErrBuf, 0,def_int_Buf_Size);  
		OCIErrorGet((dvoid *)pOCI->m_pOCIError, (ub4)1, (text *)NULL, &nErrCode, (ub1 *)szErrBuf, (ub4)sizeof(szErrBuf), OCI_HTYPE_ERROR);
		nErrNo = nErrCode;
		std::cout << "error! OCIServerAttach Oracle server attach error:" << szErrBuf << std::endl;
	}
	
	
	/*Agent Address*/	
	pMyOCIHandleEnq->m_swResult = OCIAttrSet((dvoid *)(pMyOCIHandleEnq->m_OCIAQAgentArrayForEnq[0]), (ub4)OCI_DTYPE_AQAGENT, (dvoid *)strOracleQueueName.c_str(),	(ub4)strOracleQueueName.length(), (ub4)OCI_ATTR_AGENT_ADDRESS, pOCI->m_pOCIError);
	if (OCI_SUCCESS != pMyOCIHandleEnq->m_swResult)
	{	
		memset(szErrBuf, 0,def_int_Buf_Size);  
		OCIErrorGet((dvoid *)pOCI->m_pOCIError, (ub4)1, (text *)NULL, &nErrCode, (ub1 *)szErrBuf, (ub4)sizeof(szErrBuf), OCI_HTYPE_ERROR);
		nErrNo = nErrCode;
		std::cout << "error! OCIServerAttach Oracle server attach error:" << szErrBuf << std::endl;
	}
	/*OCI_EnqueueCreate*/
	pMyOCIHandleEnq->m_swResult = OCIDescriptorAlloc(pOCI->m_pOCIEnv, (dvoid **)&(pMyOCIHandleEnq->m_pOCIAQEnqOptionsForEnq), OCI_DTYPE_AQENQ_OPTIONS, (size_t)0, (dvoid **)NULL);
	if (OCI_SUCCESS != pMyOCIHandleEnq->m_swResult)
	{	
		memset(szErrBuf, 0,def_int_Buf_Size);  
		OCIErrorGet((dvoid *)pOCI->m_pOCIError, (ub4)1, (text *)NULL, &nErrCode, (ub1 *)szErrBuf, (ub4)sizeof(szErrBuf), OCI_HTYPE_ERROR);
		nErrNo = nErrCode;
		std::cout << "error! OCIServerAttach Oracle server attach error:" << szErrBuf << std::endl;
	}
	
	
	/*OCI_MsgCreate*/
	pMyOCIHandleEnq->m_swResult = OCIDescriptorAlloc(pOCI->m_pOCIEnv, (dvoid **)&(pMyOCIHandleEnq->m_pOCIAQMsgPropertiesForEnq), OCI_DTYPE_AQMSG_PROPERTIES, (size_t)0, (dvoid **)NULL);
	if (OCI_SUCCESS != pMyOCIHandleEnq->m_swResult)
	{	
		memset(szErrBuf, 0,def_int_Buf_Size);  
		OCIErrorGet((dvoid *)pOCI->m_pOCIError, (ub4)1, (text *)NULL, &nErrCode, (ub1 *)szErrBuf, (ub4)sizeof(szErrBuf), OCI_HTYPE_ERROR);
		nErrNo = nErrCode;
		std::cout << "error! OCIServerAttach Oracle server attach error:" << szErrBuf << std::endl;
	}
	
	
	/*OCI_MsgSetConsumers*/	
	pMyOCIHandleEnq->m_swResult =	OCIAttrSet((dvoid *)(pMyOCIHandleEnq->m_pOCIAQMsgPropertiesForEnq), (ub4)OCI_DTYPE_AQMSG_PROPERTIES, (dvoid *)(pMyOCIHandleEnq->m_OCIAQAgentArrayForEnq), 
		(ub4)1, (ub4)OCI_ATTR_RECIPIENT_LIST, pOCI->m_pOCIError);
	if (OCI_SUCCESS != pMyOCIHandleEnq->m_swResult)
	{	
		memset(szErrBuf, 0,def_int_Buf_Size);  
		OCIErrorGet((dvoid *)pOCI->m_pOCIError, (ub4)1, (text *)NULL, &nErrCode, (ub1 *)szErrBuf, (ub4)sizeof(szErrBuf), OCI_HTYPE_ERROR);
		nErrNo = nErrCode;
		std::cout << "error! OCIServerAttach Oracle server attach error:" << szErrBuf << std::endl;
	}
	
	/************************************************************************/
	/* prepare for enq   begin                                              */
	/************************************************************************/

	
	return 0;
}

int CTestOCIEnqDeq::_UninitMyOCIHandleEnq(MyOCIHandle *pOCI, MyOCIHandleEnq *pMyOCIHandleEnq)
{
	int nErrNo = 0;
	char szErrBuf[def_int_Buf_Size]={'\0'};
	sb4 nErrCode = 0;
	
	if (NULL == pMyOCIHandleEnq)
	{
		return -1;
	}
	
	/************************************************************************/
	/* uninit enq                                                           */
	/************************************************************************/
	
	//====free enq Agent===========
	pMyOCIHandleEnq->m_swResult = OCIDescriptorFree((dvoid *)(pMyOCIHandleEnq->m_OCIAQAgentArrayForEnq[0]), OCI_DTYPE_AQAGENT);
	pMyOCIHandleEnq->m_OCIAQAgentArrayForEnq[0] = NULL;	
	pMyOCIHandleEnq->m_OCITypeForEnq = NULL;				
	if (OCI_SUCCESS != pMyOCIHandleEnq->m_swResult)
	{	
		memset(szErrBuf, 0,def_int_Buf_Size);  
		OCIErrorGet((dvoid *)pOCI->m_pOCIError, (ub4)1, (text *)NULL, &nErrCode, (ub1 *)szErrBuf, (ub4)sizeof(szErrBuf), OCI_HTYPE_ERROR);
		nErrNo = nErrCode;
		std::cout << "error! nErrNo=%d szErrBuf=%s" << nErrNo<<szErrBuf << std::endl;
	}

	//=======free enq======================
	pMyOCIHandleEnq->m_swResult = OCIDescriptorFree(pMyOCIHandleEnq->m_pOCIAQMsgPropertiesForEnq, OCI_DTYPE_AQMSG_PROPERTIES);
	pMyOCIHandleEnq->m_pOCIAQMsgPropertiesForEnq = NULL;
	if (OCI_SUCCESS != pMyOCIHandleEnq->m_swResult)
	{	
		memset(szErrBuf, 0,def_int_Buf_Size);  
		OCIErrorGet((dvoid *)pOCI->m_pOCIError, (ub4)1, (text *)NULL, &nErrCode, (ub1 *)szErrBuf, (ub4)sizeof(szErrBuf), OCI_HTYPE_ERROR);
		nErrNo = nErrCode;
		std::cout << "error! nErrNo=%d szErrBuf=%s" << nErrNo<<szErrBuf << std::endl;
	}
	
	
	pMyOCIHandleEnq->m_swResult = OCIDescriptorFree(pMyOCIHandleEnq->m_pOCIAQEnqOptionsForEnq, OCI_DTYPE_AQENQ_OPTIONS);
	pMyOCIHandleEnq->m_pOCIAQEnqOptionsForEnq = NULL;
	if (OCI_SUCCESS != pMyOCIHandleEnq->m_swResult)
	{	
		memset(szErrBuf, 0,def_int_Buf_Size);  
		OCIErrorGet((dvoid *)pOCI->m_pOCIError, (ub4)1, (text *)NULL, &nErrCode, (ub1 *)szErrBuf, (ub4)sizeof(szErrBuf), OCI_HTYPE_ERROR);
		nErrNo = nErrCode;
		std::cout << "error! nErrNo=%d szErrBuf=%s" << nErrNo<<szErrBuf << std::endl;
	}
	


	pMyOCIHandleEnq->m_swResult = OCIHandleFree(pMyOCIHandleEnq->m_OCIDescribeForEnq, OCI_HTYPE_DESCRIBE);
	pMyOCIHandleEnq->m_OCIDescribeForEnq = NULL;
	if (OCI_SUCCESS != pMyOCIHandleEnq->m_swResult)
	{	
		memset(szErrBuf, 0,def_int_Buf_Size);  
		OCIErrorGet((dvoid *)pOCI->m_pOCIError, (ub4)1, (text *)NULL, &nErrCode, (ub1 *)szErrBuf, (ub4)sizeof(szErrBuf), OCI_HTYPE_ERROR);
		nErrNo = nErrCode;
		std::cout << "error! nErrNo=%d szErrBuf=%s" << nErrNo<<szErrBuf << std::endl;
	}


	pMyOCIHandleEnq->m_swResult = OCISessionEnd(pMyOCIHandleEnq->m_OCISvcCtxForEnq, pOCI->m_pOCIError, pMyOCIHandleEnq->m_pOCISessionForEnq, (ub4) OCI_DEFAULT);
	if (OCI_SUCCESS != pMyOCIHandleEnq->m_swResult)
	{	
		memset(szErrBuf, 0,def_int_Buf_Size);  
		OCIErrorGet((dvoid *)pOCI->m_pOCIError, (ub4)1, (text *)NULL, &nErrCode, (ub1 *)szErrBuf, (ub4)sizeof(szErrBuf), OCI_HTYPE_ERROR);
		nErrNo = nErrCode;
		std::cout << "error! nErrNo=%d szErrBuf=%s" << nErrNo<<szErrBuf << std::endl;
	}

	pMyOCIHandleEnq->m_swResult = OCIHandleFree((dvoid *)pMyOCIHandleEnq->m_OCISvcCtxForEnq, OCI_HTYPE_SVCCTX);
	pMyOCIHandleEnq->m_OCISvcCtxForEnq = NULL;
	if (OCI_SUCCESS != pMyOCIHandleEnq->m_swResult)
	{	
		memset(szErrBuf, 0,def_int_Buf_Size);  
		OCIErrorGet((dvoid *)pOCI->m_pOCIError, (ub4)1, (text *)NULL, &nErrCode, (ub1 *)szErrBuf, (ub4)sizeof(szErrBuf), OCI_HTYPE_ERROR);
		nErrNo = nErrCode;
		std::cout << "error! nErrNo=%d szErrBuf=%s" << nErrNo<<szErrBuf << std::endl;
	}
	

	pMyOCIHandleEnq->m_swResult = OCIServerDetach(pMyOCIHandleEnq->m_pOCIServerForEnq, pOCI->m_pOCIError, OCI_DEFAULT);
	if (OCI_SUCCESS != pMyOCIHandleEnq->m_swResult)
	{	
		memset(szErrBuf, 0,def_int_Buf_Size);  
		OCIErrorGet((dvoid *)pOCI->m_pOCIError, (ub4)1, (text *)NULL, &nErrCode, (ub1 *)szErrBuf, (ub4)sizeof(szErrBuf), OCI_HTYPE_ERROR);
		nErrNo = nErrCode;
		std::cout << "error! nErrNo=%d szErrBuf=%s" << nErrNo<<szErrBuf << std::endl;
	}


	pMyOCIHandleEnq->m_swResult = OCIHandleFree((dvoid *)pMyOCIHandleEnq->m_pOCISessionForEnq,OCI_HTYPE_SESSION);
	pMyOCIHandleEnq->m_pOCISessionForEnq = NULL;
	if (OCI_SUCCESS != pMyOCIHandleEnq->m_swResult)
	{	
		memset(szErrBuf, 0,def_int_Buf_Size);  
		OCIErrorGet((dvoid *)pOCI->m_pOCIError, (ub4)1, (text *)NULL, &nErrCode, (ub1 *)szErrBuf, (ub4)sizeof(szErrBuf), OCI_HTYPE_ERROR);
		nErrNo = nErrCode;
		std::cout << "error! nErrNo=%d szErrBuf=%s" << nErrNo<<szErrBuf << std::endl;
	}

	pMyOCIHandleEnq->m_swResult = OCIHandleFree((dvoid *)pMyOCIHandleEnq->m_pOCIServerForEnq, OCI_HTYPE_SERVER);
	pMyOCIHandleEnq->m_pOCIServerForEnq = NULL;	
	if (OCI_SUCCESS != pMyOCIHandleEnq->m_swResult)
	{	
		memset(szErrBuf, 0,def_int_Buf_Size);  
		OCIErrorGet((dvoid *)pOCI->m_pOCIError, (ub4)1, (text *)NULL, &nErrCode, (ub1 *)szErrBuf, (ub4)sizeof(szErrBuf), OCI_HTYPE_ERROR);
		nErrNo = nErrCode;
		std::cout << "error! nErrNo=%d szErrBuf=%s" << nErrNo<<szErrBuf << std::endl;
	}
	
	
	
	
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	

	
	
	
	return 0;
}



int CTestOCIEnqDeq::_InitMyOCIHandleDeq(MyOCIHandle *pOCI, MyOCIHandleDeq *pMyOCIHandleDeq)
{
	std::string strOracleTNSName= defDefault_Oracle_DbName;//
	std::string strOracleUser = defDefault_Oracle_DbUser;	 
	std::string strOraclePwd = defDefault_Oracle_DbPwd;	

	
	std::string strOracleQueueName =	defDefault_Oracle_QueueName;
	std::string strServerAgentName =	defDefault_Server_AgentName;
	std::string strClientAgentName =	defDefault_Client_AgentName;

	std::string strAQPLType =	defDefault_defConfig_PLType;
	
	int nErrNo = 0;
	char szErrBuf[def_int_Buf_Size]={'\0'};
	sb4 nErrCode = 0;


	if (NULL == pMyOCIHandleDeq)
	{
		return -1;
	}
	
	
	pMyOCIHandleDeq->m_swResult = OCI_SUCCESS;

	pMyOCIHandleDeq->m_pOCIAQDeqOptionsForDeq = NULL;
	
	pMyOCIHandleDeq->m_pOCIServerForDeq = NULL;			
	pMyOCIHandleDeq->m_pOCISessionForDeq = NULL;	
	pMyOCIHandleDeq->m_OCISvcCtxForDeq = NULL;
	pMyOCIHandleDeq->m_OCIDescribeForDeq = NULL;		
	pMyOCIHandleDeq->m_OCITypeForDeq = NULL;

	pMyOCIHandleDeq->m_nDeqWaitTimeForDeq = 2;//OCI_DEQ_NO_WAIT;
	pMyOCIHandleDeq->m_nDeqNavigationForDeq = OCI_DEQ_FIRST_MSG;
	



	/************************************************************************/
	/* prepare for deq                                              */
	/************************************************************************/
	
	pMyOCIHandleDeq->m_swResult = OCIHandleAlloc((dvoid *)(pOCI->m_pOCIEnv), (dvoid **)&(pMyOCIHandleDeq->m_pOCIServerForDeq), OCI_HTYPE_SERVER, (size_t)0, (dvoid **)0);
	if (OCI_SUCCESS != pMyOCIHandleDeq->m_swResult)
	{	
		memset(szErrBuf, 0,def_int_Buf_Size);  
		OCIErrorGet((dvoid *)pOCI->m_pOCIError, (ub4)1, (text *)NULL, &nErrCode, (ub1 *)szErrBuf, (ub4)sizeof(szErrBuf), OCI_HTYPE_ERROR);
		nErrNo = nErrCode;
		std::cout << "error! nErrNo=%d szErrBuf=%s" << nErrNo<<szErrBuf << std::endl;
	}
	
	
	// 连接 | 断开服务器 多用户方式连接 ( 创建一个存取路径到数据源 )	
	pMyOCIHandleDeq->m_swResult = OCIServerAttach(pMyOCIHandleDeq->m_pOCIServerForDeq, pOCI->m_pOCIError, (text *)strOracleTNSName.c_str(), strOracleTNSName.size(), 0);
	if (OCI_SUCCESS != pMyOCIHandleDeq->m_swResult)
	{	
		memset(szErrBuf, 0,def_int_Buf_Size);  
		OCIErrorGet((dvoid *)pOCI->m_pOCIError, (ub4)1, (text *)NULL, &nErrCode, (ub1 *)szErrBuf, (ub4)sizeof(szErrBuf), OCI_HTYPE_ERROR);
		nErrNo = nErrCode;
		std::cout << "error! nErrNo=%d szErrBuf=%s" << nErrNo<<szErrBuf << std::endl;
	}
	
	
	
	/*OCI_ConnectionLogon*/
	pMyOCIHandleDeq->m_pOCISessionForDeq = NULL;
	pMyOCIHandleDeq->m_swResult = OCIHandleAlloc((dvoid *)(pOCI->m_pOCIEnv), (dvoid **)&(pMyOCIHandleDeq->m_pOCISessionForDeq), (ub4)OCI_HTYPE_SESSION, (size_t) 0, (dvoid **)0);
	if (OCI_SUCCESS != pMyOCIHandleDeq->m_swResult)
	{	
		memset(szErrBuf, 0,def_int_Buf_Size);  
		OCIErrorGet((dvoid *)pOCI->m_pOCIError, (ub4)1, (text *)NULL, &nErrCode, (ub1 *)szErrBuf, (ub4)sizeof(szErrBuf), OCI_HTYPE_ERROR);
		nErrNo = nErrCode;
		std::cout << "error! nErrNo=%d szErrBuf=%s" << nErrNo<<szErrBuf << std::endl;
	}



	pMyOCIHandleDeq->m_OCISvcCtxForDeq = NULL;
	pMyOCIHandleDeq->m_swResult = OCIHandleAlloc((dvoid *)(pOCI->m_pOCIEnv), (dvoid **)&(pMyOCIHandleDeq->m_OCISvcCtxForDeq), OCI_HTYPE_SVCCTX, (size_t) 0, (dvoid **)0);
	if (OCI_SUCCESS != pMyOCIHandleDeq->m_swResult)
	{	
		memset(szErrBuf, 0,def_int_Buf_Size);  
		OCIErrorGet((dvoid *)pOCI->m_pOCIError, (ub4)1, (text *)NULL, &nErrCode, (ub1 *)szErrBuf, (ub4)sizeof(szErrBuf), OCI_HTYPE_ERROR);
		nErrNo = nErrCode;
		std::cout << "error! nErrNo=%d szErrBuf=%s" << nErrNo<<szErrBuf << std::endl;
	}
	
	
	
	// 为指定的句柄及描述符设置特定的属性 ( 为服务句柄指定服务上下文 )
	pMyOCIHandleDeq->m_swResult = OCIAttrSet((dvoid *)(pMyOCIHandleDeq->m_OCISvcCtxForDeq), OCI_HTYPE_SVCCTX, (dvoid *)(pMyOCIHandleDeq->m_pOCIServerForDeq), (ub4)0, OCI_ATTR_SERVER, (OCIError *)(pOCI->m_pOCIError));
	if (OCI_SUCCESS != pMyOCIHandleDeq->m_swResult)
	{	
		memset(szErrBuf, 0,def_int_Buf_Size);  
		OCIErrorGet((dvoid *)pOCI->m_pOCIError, (ub4)1, (text *)NULL, &nErrCode, (ub1 *)szErrBuf, (ub4)sizeof(szErrBuf), OCI_HTYPE_ERROR);
		nErrNo = nErrCode;
		std::cout << "error! nErrNo=%d szErrBuf=%s" << nErrNo<<szErrBuf << std::endl;
	}
	
	
	
	// 为用户连接句柄设置登录名及密码
	pMyOCIHandleDeq->m_swResult = OCIAttrSet((dvoid *)pMyOCIHandleDeq->m_pOCISessionForDeq, (ub4)OCI_HTYPE_SESSION, (dvoid *)strOracleUser.c_str(), (ub4)(strOracleUser.length()), (ub4)OCI_ATTR_USERNAME, pOCI->m_pOCIError);
	pMyOCIHandleDeq->m_swResult = OCIAttrSet((dvoid *)pMyOCIHandleDeq->m_pOCISessionForDeq, (ub4)OCI_HTYPE_SESSION, (dvoid *)strOraclePwd.c_str(), (ub4)(strOraclePwd.length()), (ub4)OCI_ATTR_PASSWORD, pOCI->m_pOCIError);
	if (OCI_SUCCESS != pMyOCIHandleDeq->m_swResult)
	{	
		memset(szErrBuf, 0,def_int_Buf_Size);  
		OCIErrorGet((dvoid *)pOCI->m_pOCIError, (ub4)1, (text *)NULL, &nErrCode, (ub1 *)szErrBuf, (ub4)sizeof(szErrBuf), OCI_HTYPE_ERROR);
		nErrNo = nErrCode;
		std::cout << "error! nErrNo=%d szErrBuf=%s" << nErrNo<<szErrBuf << std::endl;
	}
	
	
	
	// 认证用户再建立一个会话连接
	pMyOCIHandleDeq->m_swResult = OCISessionBegin(pMyOCIHandleDeq->m_OCISvcCtxForDeq, pOCI->m_pOCIError, pMyOCIHandleDeq->m_pOCISessionForDeq, OCI_CRED_RDBMS, (ub4)OCI_DEFAULT);
	if (OCI_SUCCESS != pMyOCIHandleDeq->m_swResult)
	{	
		memset(szErrBuf, 0,def_int_Buf_Size);  
		OCIErrorGet((dvoid *)pOCI->m_pOCIError, (ub4)1, (text *)NULL, &nErrCode, (ub1 *)szErrBuf, (ub4)sizeof(szErrBuf), OCI_HTYPE_ERROR);
		nErrNo = nErrCode;
		std::cout << "error! nErrNo=%d szErrBuf=%s" << nErrNo<<szErrBuf << std::endl;
	}
	
	
	
	pMyOCIHandleDeq->m_swResult = OCIAttrSet((dvoid *)pMyOCIHandleDeq->m_OCISvcCtxForDeq, (ub4) OCI_HTYPE_SVCCTX, (dvoid *)pMyOCIHandleDeq->m_pOCISessionForDeq, (ub4)0, (ub4)OCI_ATTR_SESSION, pOCI->m_pOCIError);
	if (OCI_SUCCESS != pMyOCIHandleDeq->m_swResult)
	{	
		// 返回一个错误指针和一个 OCI 错误代码	
		memset(szErrBuf, 0,def_int_Buf_Size);  
		OCIErrorGet((dvoid *)pOCI->m_pOCIError, (ub4)1, (text *)NULL, &nErrCode, (ub1 *)szErrBuf, (ub4)sizeof(szErrBuf), OCI_HTYPE_ERROR);
		nErrNo = nErrCode;
		std::cout << "error! nErrNo=%d szErrBuf=%s" << nErrNo<<szErrBuf << std::endl;
	}




	//===OCI_TypeInfoGet===================================================================================================
	/*enq_EnqTypeInfoHandle = OCI_TypeInfoGet(enq_ConHandle, defDefault_defConfig_PLType, OCI_TIF_TYPE);*/
	pMyOCIHandleDeq->m_OCIDescribeForDeq = NULL;	
	pMyOCIHandleDeq->m_swResult = OCIHandleAlloc(pOCI->m_pOCIEnv, (dvoid **) (void *)&(pMyOCIHandleDeq->m_OCIDescribeForDeq), OCI_HTYPE_DESCRIBE, (size_t) 0, (dvoid **) NULL);
	if (OCI_SUCCESS != pMyOCIHandleDeq->m_swResult)
	{	
		memset(szErrBuf, 0,def_int_Buf_Size);  
		OCIErrorGet((dvoid *)pOCI->m_pOCIError, (ub4)1, (text *)NULL, &nErrCode, (ub1 *)szErrBuf, (ub4)sizeof(szErrBuf), OCI_HTYPE_ERROR);
		nErrNo = nErrCode;
		std::cout << "error! nErrNo=%d szErrBuf=%s" << nErrNo<<szErrBuf << std::endl;
	}

	pMyOCIHandleDeq->m_swResult = OCITypeByName(pOCI->m_pOCIEnv, pOCI->m_pOCIError, pMyOCIHandleDeq->m_OCISvcCtxForDeq, (CONST text *)"", (ub4) strlen(""),
		(CONST text *)strAQPLType.c_str(), (ub4) strlen(strAQPLType.c_str()),
		(text *)0, 0, OCI_DURATION_SESSION, OCI_TYPEGET_ALL, &(pMyOCIHandleDeq->m_OCITypeForDeq));
	if (OCI_SUCCESS != pMyOCIHandleDeq->m_swResult)
	{	
		memset(szErrBuf, 0,def_int_Buf_Size);  
		OCIErrorGet((dvoid *)pOCI->m_pOCIError, (ub4)1, (text *)NULL, &nErrCode, (ub1 *)szErrBuf, (ub4)sizeof(szErrBuf), OCI_HTYPE_ERROR);
		nErrNo = nErrCode;
		std::cout << "error! nErrNo=%d szErrBuf=%s" << nErrNo<<szErrBuf << std::endl;
	}

	pMyOCIHandleDeq->m_swResult = OCIDescribeAny(pMyOCIHandleDeq->m_OCISvcCtxForDeq, pOCI->m_pOCIError, (void *)pMyOCIHandleDeq->m_OCITypeForDeq, 0, OCI_OTYPE_PTR, OCI_DEFAULT, OCI_PTYPE_TYPE, pMyOCIHandleDeq->m_OCIDescribeForDeq);
	if (OCI_SUCCESS != pMyOCIHandleDeq->m_swResult)
	{	
		memset(szErrBuf, 0,def_int_Buf_Size);  
		OCIErrorGet((dvoid *)pOCI->m_pOCIError, (ub4)1, (text *)NULL, &nErrCode, (ub1 *)szErrBuf, (ub4)sizeof(szErrBuf), OCI_HTYPE_ERROR);
		nErrNo = nErrCode;
		std::cout << "error! nErrNo=%d szErrBuf=%s" << nErrNo<<szErrBuf << std::endl;
	}


	//==init deq=====================================================

	/* now dequeue the messages using different consumer names */
	/* allocate dequeue options descriptor to set the dequeue options */
	pMyOCIHandleDeq->m_swResult = OCIDescriptorAlloc(pOCI->m_pOCIEnv, (dvoid **)&(pMyOCIHandleDeq->m_pOCIAQDeqOptionsForDeq), OCI_DTYPE_AQDEQ_OPTIONS, 0, (dvoid **)0);
	if (OCI_SUCCESS != pMyOCIHandleDeq->m_swResult)
	{	
		memset(szErrBuf, 0,def_int_Buf_Size);  
		OCIErrorGet((dvoid *)pOCI->m_pOCIError, (ub4)1, (text *)NULL, &nErrCode, (ub1 *)szErrBuf, (ub4)sizeof(szErrBuf), OCI_HTYPE_ERROR);
		nErrNo = nErrCode;
		std::cout << "error! nErrNo=%d szErrBuf=%s" << nErrNo<<szErrBuf << std::endl;
	} 
	
	
	/* set wait parameter to NO_WAIT so that the dequeue returns immediately */
	pMyOCIHandleDeq->m_swResult = OCIAttrSet(pMyOCIHandleDeq->m_pOCIAQDeqOptionsForDeq, OCI_DTYPE_AQDEQ_OPTIONS, (dvoid *)&(pMyOCIHandleDeq->m_nDeqWaitTimeForDeq), 0,	OCI_ATTR_WAIT, pOCI->m_pOCIError);
	if (OCI_SUCCESS != pMyOCIHandleDeq->m_swResult)
	{	
		memset(szErrBuf, 0,def_int_Buf_Size);  
		OCIErrorGet((dvoid *)pOCI->m_pOCIError, (ub4)1, (text *)NULL, &nErrCode, (ub1 *)szErrBuf, (ub4)sizeof(szErrBuf), OCI_HTYPE_ERROR);
		nErrNo = nErrCode;
		std::cout << "error! nErrNo=%d szErrBuf=%s" << nErrNo<<szErrBuf << std::endl;
	} 
	
	/* set navigation to FIRST_MESSAGE so that the dequeue resets the position */
	/* after a new consumer_name is set in the dequeue options     */
	pMyOCIHandleDeq->m_swResult = OCIAttrSet(pMyOCIHandleDeq->m_pOCIAQDeqOptionsForDeq, OCI_DTYPE_AQDEQ_OPTIONS, (dvoid *)&(pMyOCIHandleDeq->m_nDeqNavigationForDeq), 0, OCI_ATTR_NAVIGATION, pOCI->m_pOCIError);
	if (OCI_SUCCESS != pMyOCIHandleDeq->m_swResult)
	{	
		memset(szErrBuf, 0,def_int_Buf_Size);  
		OCIErrorGet((dvoid *)pOCI->m_pOCIError, (ub4)1, (text *)NULL, &nErrCode, (ub1 *)szErrBuf, (ub4)sizeof(szErrBuf), OCI_HTYPE_ERROR);
		nErrNo = nErrCode;
		std::cout << "error! nErrNo=%d szErrBuf=%s" << nErrNo<<szErrBuf << std::endl;
	} 
	
	
	/* dequeue from the msg_queue_multiple as consumer BLUE */
	pMyOCIHandleDeq->m_swResult = OCIAttrSet(pMyOCIHandleDeq->m_pOCIAQDeqOptionsForDeq, OCI_DTYPE_AQDEQ_OPTIONS, (dvoid *)(strClientAgentName.c_str()), strClientAgentName.length(), OCI_ATTR_CONSUMER_NAME, pOCI->m_pOCIError);
	if (OCI_SUCCESS != pMyOCIHandleDeq->m_swResult)
	{	
		memset(szErrBuf, 0,def_int_Buf_Size);  
		OCIErrorGet((dvoid *)pOCI->m_pOCIError, (ub4)1, (text *)NULL, &nErrCode, (ub1 *)szErrBuf, (ub4)sizeof(szErrBuf), OCI_HTYPE_ERROR);
		nErrNo = nErrCode;
		std::cout << "error! nErrNo=%d szErrBuf=%s" << nErrNo<<szErrBuf << std::endl;
	} 
	/************************************************************************/
	/* prepare for deq                                              */
	/************************************************************************/

	
	
	return 0;
}

int CTestOCIEnqDeq::_UninitMyOCIHandleDeq(MyOCIHandle *pOCI, MyOCIHandleDeq *pMyOCIHandleDeq)
{
	int nErrNo = 0;
	char szErrBuf[def_int_Buf_Size]={'\0'};
	sb4 nErrCode = 0;
	
	if (NULL == pMyOCIHandleDeq)
	{
		return -1;
	}
	
	/************************************************************************/
	/* uninit enq                                                           */
	/************************************************************************/
	pMyOCIHandleDeq->m_OCITypeForDeq = NULL;
	
	//=======free deq======================	
	pMyOCIHandleDeq->m_swResult = OCIDescriptorFree(pMyOCIHandleDeq->m_pOCIAQDeqOptionsForDeq, OCI_DTYPE_AQDEQ_OPTIONS);
	pMyOCIHandleDeq->m_pOCIAQDeqOptionsForDeq = NULL;
	if (OCI_SUCCESS != pMyOCIHandleDeq->m_swResult)
	{	
		memset(szErrBuf, 0,def_int_Buf_Size);  
		OCIErrorGet((dvoid *)pOCI->m_pOCIError, (ub4)1, (text *)NULL, &nErrCode, (ub1 *)szErrBuf, (ub4)sizeof(szErrBuf), OCI_HTYPE_ERROR);
		nErrNo = nErrCode;
		std::cout << "error! nErrNo=%d szErrBuf=%s" << nErrNo<<szErrBuf << std::endl;
	}
	  	
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/ 	

	pMyOCIHandleDeq->m_swResult = OCIHandleFree(pMyOCIHandleDeq->m_OCIDescribeForDeq, OCI_HTYPE_DESCRIBE);
	pMyOCIHandleDeq->m_OCIDescribeForDeq = NULL;
	if (OCI_SUCCESS != pMyOCIHandleDeq->m_swResult)
	{	
		memset(szErrBuf, 0,def_int_Buf_Size);  
		OCIErrorGet((dvoid *)pOCI->m_pOCIError, (ub4)1, (text *)NULL, &nErrCode, (ub1 *)szErrBuf, (ub4)sizeof(szErrBuf), OCI_HTYPE_ERROR);
		nErrNo = nErrCode;
		std::cout << "error! nErrNo=%d szErrBuf=%s" << nErrNo<<szErrBuf << std::endl;
	}

	pMyOCIHandleDeq->m_swResult = OCISessionEnd(pMyOCIHandleDeq->m_OCISvcCtxForDeq, pOCI->m_pOCIError, pMyOCIHandleDeq->m_pOCISessionForDeq, (ub4) OCI_DEFAULT);
	if (OCI_SUCCESS != pMyOCIHandleDeq->m_swResult)
	{	
		memset(szErrBuf, 0,def_int_Buf_Size);  
		OCIErrorGet((dvoid *)pOCI->m_pOCIError, (ub4)1, (text *)NULL, &nErrCode, (ub1 *)szErrBuf, (ub4)sizeof(szErrBuf), OCI_HTYPE_ERROR);
		nErrNo = nErrCode;
		std::cout << "error! nErrNo=%d szErrBuf=%s" << nErrNo<<szErrBuf << std::endl;
	}


	pMyOCIHandleDeq->m_swResult = OCIHandleFree((dvoid *)pMyOCIHandleDeq->m_OCISvcCtxForDeq, OCI_HTYPE_SVCCTX);
	pMyOCIHandleDeq->m_OCISvcCtxForDeq = NULL;
	if (OCI_SUCCESS != pMyOCIHandleDeq->m_swResult)
	{	
		memset(szErrBuf, 0,def_int_Buf_Size);  
		OCIErrorGet((dvoid *)pOCI->m_pOCIError, (ub4)1, (text *)NULL, &nErrCode, (ub1 *)szErrBuf, (ub4)sizeof(szErrBuf), OCI_HTYPE_ERROR);
		nErrNo = nErrCode;
		std::cout << "error! nErrNo=%d szErrBuf=%s" << nErrNo<<szErrBuf << std::endl;
	}
	
	pMyOCIHandleDeq->m_swResult = OCIServerDetach(pMyOCIHandleDeq->m_pOCIServerForDeq, pOCI->m_pOCIError, OCI_DEFAULT);
	if (OCI_SUCCESS != pMyOCIHandleDeq->m_swResult)
	{	
		memset(szErrBuf, 0,def_int_Buf_Size);  
		OCIErrorGet((dvoid *)pOCI->m_pOCIError, (ub4)1, (text *)NULL, &nErrCode, (ub1 *)szErrBuf, (ub4)sizeof(szErrBuf), OCI_HTYPE_ERROR);
		nErrNo = nErrCode;
		std::cout << "error! nErrNo=%d szErrBuf=%s" << nErrNo<<szErrBuf << std::endl;
	}


	pMyOCIHandleDeq->m_swResult = OCIHandleFree((dvoid *)pMyOCIHandleDeq->m_pOCISessionForDeq,OCI_HTYPE_SESSION);
	pMyOCIHandleDeq->m_pOCISessionForDeq = NULL;
	if (OCI_SUCCESS != pMyOCIHandleDeq->m_swResult)
	{	
		memset(szErrBuf, 0,def_int_Buf_Size);  
		OCIErrorGet((dvoid *)pOCI->m_pOCIError, (ub4)1, (text *)NULL, &nErrCode, (ub1 *)szErrBuf, (ub4)sizeof(szErrBuf), OCI_HTYPE_ERROR);
		nErrNo = nErrCode;
		std::cout << "error! nErrNo=%d szErrBuf=%s" << nErrNo<<szErrBuf << std::endl;
	}

	pMyOCIHandleDeq->m_swResult = OCIHandleFree((dvoid *)pMyOCIHandleDeq->m_pOCIServerForDeq, OCI_HTYPE_SERVER);
	pMyOCIHandleDeq->m_pOCIServerForDeq = NULL;	
	if (OCI_SUCCESS != pMyOCIHandleDeq->m_swResult)
	{	
		memset(szErrBuf, 0,def_int_Buf_Size);  
		OCIErrorGet((dvoid *)pOCI->m_pOCIError, (ub4)1, (text *)NULL, &nErrCode, (ub1 *)szErrBuf, (ub4)sizeof(szErrBuf), OCI_HTYPE_ERROR);
		nErrNo = nErrCode;
		std::cout << "error! nErrNo=%d szErrBuf=%s" << nErrNo<<szErrBuf << std::endl;
	}
	
	
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/

	
	return 0;
}

int CTestOCIEnqDeq::test_fun_oci_enq_many()
{

	AQMessage			enqMsg;
	AQMessageNull		enqNullMsg;
	AQMessage			*pAQMsg = &enqMsg;
	AQMessageNull		*pAQNullMsg = &enqNullMsg; 

	std::string			strOracleQueueName =	defDefault_Oracle_QueueName;
	std::string			strMsgFierValue;
	std::string			strDataDmlValueOracle;
	std::string			strDataDmlValueMysql;
	sword				swResult;
	
	int enq_nCountEnqOnce = 0; 
	int enq_nCountEnqTotal = 0;
	int enq_Each_commit= 200;
	int enq_Total_limit = def_int_Deq_Total;
	int One20000Work_begin = 0;
	int One20000Work_end = 0;
	int enq_once_sleep=20000;

	int nErrNo = 0;
	char szErrBuf[def_int_Buf_Size]={'\0'};
	sb4 nErrCode = 0;
	
	
	MyOCIHandle* m_pOCI = new MyOCIHandle();
	_InitMyOCIHandle(m_pOCI);


	MyOCIHandleEnq *pMyOCIHandleEnq = NULL;
	pMyOCIHandleEnq = new MyOCIHandleEnq();
	if (NULL == pMyOCIHandleEnq)
	{
		return -1;
	}


	strMsgFierValue = "INSERT:TRA_OCC";	//"INSERT:TRA_OCC"
	strDataDmlValueOracle = "Oracle76=insert into queue_test_mysql(ID, NAME, TIMESTAMP) values(1, 'NAME', sysdate)";  
	strDataDmlValueMysql = "MySQL78=insert into queue_test_mysql(ID, NAME, TIMESTAMP) values(1, 'NAME', sysdate())";	
	
	_InitMyOCIHandleEnq(m_pOCI, pMyOCIHandleEnq);
	
		
	/*OCI_ObjectCreate*/
	pAQMsg = &enqMsg;  	
	

	One20000Work_begin = 1;
	while (1)
	{	
		/*after enq 200 record then commit to Oracle*/
		if (enq_nCountEnqOnce >= enq_Each_commit)
		{ 
		
			_GetCurrentTime(&m_fStepOneEnqDataEnd);
			m_strStepOneEnqDataTime = _GetDiffTime(&m_fStepOneEnqDataBegin, &m_fStepOneEnqDataEnd);			
// 			LOG_GENERIC(SourceInfo, DebugUtil::DebugDebug, 
// 				"[enq_nCountEnqTotal=%d][m_strStepOneEnqDataTime=%s]", 
// 				enq_nCountEnqTotal, m_strStepOneEnqDataTime.c_str());

		
			/*OCI_Commit*/
			_GetCurrentTime(&m_fStepTwoEnqCommitBegin);
			swResult = OCITransCommit(pMyOCIHandleEnq->m_OCISvcCtxForEnq, m_pOCI->m_pOCIError, (ub4) 0);
			if (OCI_SUCCESS != swResult)
			{	
				memset(szErrBuf, 0,def_int_Buf_Size);  
				OCIErrorGet((dvoid *)m_pOCI->m_pOCIError, (ub4)1, (text *)NULL, &nErrCode, (ub1 *)szErrBuf, (ub4)sizeof(szErrBuf), OCI_HTYPE_ERROR);
				nErrNo = nErrCode;
				std::cout << "error! nErrNo=%d szErrBuf=%s" << nErrNo<<szErrBuf << std::endl;
			}
			_GetCurrentTime(&m_fStepTwoEnqCommitEnd);
			m_strStepTwoEnqCommitTime = _GetDiffTime(&m_fStepTwoEnqCommitBegin, &m_fStepTwoEnqCommitEnd);
// 			LOG_GENERIC(SourceInfo, DebugUtil::DebugDebug, 
// 				"[enq_nCountEnqTotal=%d][m_strStepTwoEnqCommitTime=%s]", 
// 				enq_nCountEnqTotal, m_strStepTwoEnqCommitTime.c_str());
				
				
			_GetCurrentTime(&m_fEach200EnqWorkEnd);
			m_fEach200EnqWorkUse = _GetDiffTime(&m_fEach200EnqWorkBegin, &m_fEach200EnqWorkEnd);
// 			LOG_GENERIC(SourceInfo, DebugUtil::DebugDebug, 
// 				"[enq_nCountEnqTotal=%d[[m_fEach200EnqWorkUse=%s]", 
// 				enq_nCountEnqTotal, m_fEach200EnqWorkUse.c_str()); 

		}

		/*msg free*/


		if (enq_nCountEnqTotal > enq_Total_limit)
		{
			break;
		}

		if (0 == (enq_nCountEnqTotal % enq_once_sleep) && One20000Work_end == 1 )
		{
			One20000Work_begin = 1;
			One20000Work_end = 0;
			printf("test_enq_fun enq_nCountEnqTotal=%d\n", enq_nCountEnqTotal);			

			_GetCurrentTime(&m_fEach20000EnqWorkEnd);
			m_fEach20000EnqWorkUse = _GetDiffTime(&m_fEach20000EnqWorkBegin, &m_fEach20000EnqWorkEnd);
// 			LOG_GENERIC(SourceInfo, DebugUtil::DebugDebug, 
// 				"[enq_nCountEnqTotal=%d][m_fEach20000EnqWorkUse=%s]", 
// 				enq_nCountEnqTotal, m_fEach20000EnqWorkUse.c_str());
// 
// 			Sleep(1000);
		}

		if (0 == (enq_nCountEnqTotal % enq_once_sleep) && One20000Work_begin == 1 )
		{
			One20000Work_begin = 0;
			One20000Work_end = 1;
			_GetCurrentTime(&m_fEach20000EnqWorkBegin);	

		}


		/*create msg*/

		enq_nCountEnqOnce = 0;
		_GetCurrentTime(&m_fStepOneEnqDataBegin);
		_GetCurrentTime(&m_fEach200EnqWorkBegin);

		while (enq_nCountEnqOnce < enq_Each_commit)
		{ 	
		
			/* prepare the message payload */
			pAQMsg->MsgFiter = (OCIString *)0;
			pAQMsg->OracleSql = (OCIString *)0;
			pAQMsg->MysqlSql = (OCIString *)0;

		
			
			/*set string*/
			swResult = OCIStringAssignText(m_pOCI->m_pOCIEnv, m_pOCI->m_pOCIError, (CONST text *)strMsgFierValue.c_str(),
				(ub4) strlen(strMsgFierValue.c_str()), &pAQMsg->MsgFiter);
			swResult = OCIStringAssignText(m_pOCI->m_pOCIEnv, m_pOCI->m_pOCIError,(CONST text *)strDataDmlValueOracle.c_str(),
				(ub4) strlen(strDataDmlValueOracle.c_str()), &pAQMsg->OracleSql);
			swResult = OCIStringAssignText(m_pOCI->m_pOCIEnv, m_pOCI->m_pOCIError,(CONST text *)strDataDmlValueMysql.c_str(),
				(ub4) strlen(strDataDmlValueMysql.c_str()), &pAQMsg->MysqlSql);
			pAQNullMsg->null_adt = pAQNullMsg->null_MsgFiter = pAQNullMsg->null_OracleSql = pAQNullMsg->null_MysqlSql = OCI_IND_NOTNULL;

			/*enq*/
			swResult = (boolean) OCIAQEnq(pMyOCIHandleEnq->m_OCISvcCtxForEnq, m_pOCI->m_pOCIError, (text *)(strOracleQueueName.c_str()), 
				pMyOCIHandleEnq->m_pOCIAQEnqOptionsForEnq, pMyOCIHandleEnq->m_pOCIAQMsgPropertiesForEnq,
				pMyOCIHandleEnq->m_OCITypeForEnq, (void **)&pAQMsg, (dvoid **)&pAQNullMsg, (OCIRaw **)0, OCI_DEFAULT);
			if (OCI_SUCCESS != swResult)
			{	
				memset(szErrBuf, 0,def_int_Buf_Size);  
				OCIErrorGet((dvoid *)m_pOCI->m_pOCIError, (ub4)1, (text *)NULL, &nErrCode, (ub1 *)szErrBuf, (ub4)sizeof(szErrBuf), OCI_HTYPE_ERROR);
				nErrNo = nErrCode;
				std::cout << "error! nErrNo=%d szErrBuf=%s" << nErrNo<<szErrBuf << std::endl;
			}
			
			
			
			//free data		
			swResult = OCIStringResize(m_pOCI->m_pOCIEnv, m_pOCI->m_pOCIError, 0, &pAQMsg->MsgFiter);
			pAQMsg->MsgFiter = NULL;
			swResult = OCIStringResize(m_pOCI->m_pOCIEnv, m_pOCI->m_pOCIError, 0, &pAQMsg->OracleSql);
			pAQMsg->OracleSql = NULL;
			swResult = OCIStringResize(m_pOCI->m_pOCIEnv, m_pOCI->m_pOCIError, 0, &pAQMsg->MysqlSql);
			pAQMsg->MysqlSql = NULL;

			

			enq_nCountEnqOnce++;
			enq_nCountEnqTotal++;

		}//while (nCount != 1000)

	}//while (1)



 	_UninitMyOCIHandleEnq(m_pOCI, pMyOCIHandleEnq);
	if (NULL != pMyOCIHandleEnq)
	{
		delete pMyOCIHandleEnq;
		pMyOCIHandleEnq = NULL;
	}
	
	_UninitMyOCIHandle(m_pOCI);
	DEF_DELETE(m_pOCI);

	
	return 0;
}





int CTestOCIEnqDeq::test_fun_oci_deq_many()
{

	
	AQMessage		*deqmesg = NULL;	
	AQMessageNull	*ndeqmesg = NULL;
	
	std::string		strOracleQueueName =	defDefault_Oracle_QueueName;
	std::string		strMsgFierValue = "";	
	std::string		strDataDmlValueOracle = "";  
	std::string		strDataDmlValueMysql = "";
	
	int nErrNo = 0;
	char szErrBuf[def_int_Buf_Size]={'\0'};
	sb4 nErrCode = 0;
	
	sword swResult = OCI_SUCCESS;	  	
	
	
	int deq_nCountDeqOnce = 0; 
	int deq_nCountDeqTotal = 0;
	int deq_Each_commit= 200;
	int deq_Total_limmit = def_int_Deq_Total;

	MyOCIHandle *m_pOCI = NULL;
	m_pOCI = new MyOCIHandle();
	_InitMyOCIHandle(m_pOCI);
	MyOCIHandleDeq		*m_pMyOCIHandleDeq = NULL; 
	m_pMyOCIHandleDeq = new MyOCIHandleDeq();
	_InitMyOCIHandleDeq(m_pOCI, m_pMyOCIHandleDeq);
	
	if (NULL == m_pMyOCIHandleDeq)
	{
		return -1;
	}	

	while(1)
	{


		deq_nCountDeqOnce = 0;
		_GetCurrentTime(&m_fStepOneDeqDataBegin);
	
		while (OCI_SUCCESS == OCIAQDeq(m_pMyOCIHandleDeq->m_OCISvcCtxForDeq, m_pOCI->m_pOCIError, (OraText *)(strOracleQueueName.c_str()), 
			m_pMyOCIHandleDeq->m_pOCIAQDeqOptionsForDeq, 0,
			m_pMyOCIHandleDeq->m_OCITypeForDeq, (dvoid **)&deqmesg, (dvoid **)&ndeqmesg, 0, 0))
		{

			printf("\n deq_nCountDeqOnce=%d\n", deq_nCountDeqOnce);
			printf("deqmesg->MsgFiter: %s\n", OCIStringPtr(m_pOCI->m_pOCIEnv, deqmesg->MsgFiter));
			printf("deqmesg->OracleSql: %s\n", OCIStringPtr(m_pOCI->m_pOCIEnv, deqmesg->OracleSql));
			printf("deqmesg->MysqlSql: %s\n\n", OCIStringPtr(m_pOCI->m_pOCIEnv, deqmesg->MysqlSql));

			//===free data=============================================================
			if (NULL != deqmesg && NULL != deqmesg->MsgFiter)
			{
				swResult = OCIStringResize(m_pOCI->m_pOCIEnv, m_pOCI->m_pOCIError, 0, &deqmesg->MsgFiter);
				deqmesg->MsgFiter = NULL;
			}
			if (NULL != deqmesg && NULL != deqmesg->OracleSql)
			{
				swResult = OCIStringResize(m_pOCI->m_pOCIEnv, m_pOCI->m_pOCIError, 0, &deqmesg->OracleSql);
				deqmesg->OracleSql = NULL;
			}
			if (NULL != deqmesg && NULL != deqmesg->MysqlSql)
			{
				swResult = OCIStringResize(m_pOCI->m_pOCIEnv, m_pOCI->m_pOCIError, 0, &deqmesg->MysqlSql);
				deqmesg->MysqlSql = NULL;
			}
			if (NULL != deqmesg)
			{
				swResult = OCIRawResize(m_pOCI->m_pOCIEnv, m_pOCI->m_pOCIError, 0, (OCIRaw **)&deqmesg);
				deqmesg = NULL;	
			}	
			if (OCI_SUCCESS != swResult)
			{	
				// 返回一个错误指针和一个 OCI 错误代码	
				memset(szErrBuf, 0,def_int_Buf_Size);  
				OCIErrorGet((dvoid *)m_pOCI->m_pOCIError, (ub4)1, (text *)NULL, &nErrCode, (ub1 *)szErrBuf, (ub4)sizeof(szErrBuf), OCI_HTYPE_ERROR);
				nErrNo = nErrCode;
				std::cout << "error! OCIServerAttach Oracle server attach error:" << szErrBuf << std::endl;
			}


			deq_nCountDeqOnce++;
			deq_nCountDeqTotal++;
			if (deq_nCountDeqOnce > deq_Each_commit)
			{
				break;	// commit
			}

		}//while
		
		
		_GetCurrentTime(&m_fStepOneDeqDataEnd);			
		m_strStepOneDeqDataTime = _GetDiffTime(&m_fStepOneDeqDataBegin, &m_fStepOneDeqDataEnd);			
// 		LOG_GENERIC(SourceInfo, DebugUtil::DebugDebug, 
// 			"[deq_nCountDeqTotal=%d][deq_nCountDeqOnce=%d][m_strStepOneDeqDataTime=%s]", 
// 			deq_nCountDeqTotal, deq_nCountDeqOnce, m_strStepOneDeqDataTime.c_str());



		_GetCurrentTime(&m_fStepTwoDeqCommitBegin);	
		swResult = OCITransCommit(m_pMyOCIHandleDeq->m_OCISvcCtxForDeq, m_pOCI->m_pOCIError, (ub4)0);
		if (OCI_SUCCESS != swResult)
		{	
			memset(szErrBuf, 0,def_int_Buf_Size);  
			OCIErrorGet((dvoid *)m_pOCI->m_pOCIError, (ub4)1, (text *)NULL, &nErrCode, (ub1 *)szErrBuf, (ub4)sizeof(szErrBuf), OCI_HTYPE_ERROR);
			nErrNo = nErrCode;
			std::cout << "error! OCIServerAttach Oracle server attach error:" << szErrBuf << std::endl;
		}
		_GetCurrentTime(&m_fStepTwoDeqCommitEnd);
		m_strStepTwoDeqCommitTime = _GetDiffTime(&m_fStepTwoDeqCommitBegin, &m_fStepTwoDeqCommitEnd);				
// 		LOG_GENERIC(SourceInfo, DebugUtil::DebugDebug, 
// 			"[deq_nCountDeqTotal=%d][deq_nCountDeqOnce=%d][m_strStepTwoDeqCommitTime=%s]", 
// 			deq_nCountDeqTotal, deq_nCountDeqOnce, m_strStepTwoDeqCommitTime.c_str());

		if (deq_nCountDeqTotal > deq_Total_limmit)
		{
			break; // no deq 
		}


	}//while(1)



	
	
	_UninitMyOCIHandleDeq(m_pOCI, m_pMyOCIHandleDeq);
	if (NULL != m_pMyOCIHandleDeq)
	{
		delete m_pMyOCIHandleDeq;
		m_pMyOCIHandleDeq = NULL;
	}
		
	
	return 0;
}








int CTestOCIEnqDeq::test_fun_oci_enq()
{
	
	OCIEnv *m_pOCIEnv = NULL;		//初始化OCI环境句柄指针对应的句柄类型 OCI__HTYPE_ENV
 	OCIServer *m_pOCIServer = NULL;	// 初始化服务器句柄 , 对应的句柄类型 OCI_HTYPE_SERVER
 	OCIError *m_pOCIError = NULL;   // 用于捕获 OCI 错误信息 , 对应的句柄类型 OCI_HTYPE_ERROR
 	OCISession *m_pOCISession = NULL;	 // 初始化用户连接句柄 , 对应的句柄类型 OCI_HTYPE_SESSION
	OCISvcCtx *m_OCISvcCtx = NULL;  // 初始化服务上下文句柄 , 对应的句柄类型 OCI_HTYPE_SVCCTX
	OCIDescribe *m_OCIDescribe   = NULL;   // 初始化句子句柄 ( 创建一个会话 ), 对应的句柄类型 OCI_HTYPE_STMT
	OCIType *m_OCIType = NULL;
	OCIAQAgent		*m_OCIAQAgentArrayForEnq[1];
	OCIAQEnqOptions *m_pOCIAQEnqOptions = NULL;
	OCIAQMsgProperties *m_pOCIAQMsgProperties = NULL;
	ub4 m_nOCIEnvInitMode = OCI_DEFAULT | OCI_OBJECT;
	sword m_swResult;
	
	
	m_nOCIEnvInitMode = OCI_DEFAULT | OCI_OBJECT;
	
	
	AQMessage  msg;
	AQMessageNull nmsg;
	AQMessage *payload = &msg;
	AQMessageNull *nmesg = &nmsg; 

	std::string strOracleTNSName= defDefault_Oracle_DbName;//
	std::string strOracleUser = defDefault_Oracle_DbUser;	 
	std::string strOraclePwd = defDefault_Oracle_DbPwd;
	
	std::string enq_strMsgFierValue = "INSERT:TRA_OCC";	//"INSERT:TRA_OCC"
	std::string enq_strDataDmlValueOracle = "Oracle76=insert into queue_test_mysql(ID, NAME, TIMESTAMP) values(1, 'NAME', sysdate)";  
	std::string enq_strDataDmlValueMysql = "MySQL78=insert into queue_test_mysql(ID, NAME, TIMESTAMP) values(1, 'NAME', sysdate())";	
	
	m_nOCIEnvInitMode = OCI_DEFAULT | OCI_OBJECT;
	
	/*	enq_nFunRes = OCI_Initialize(NULL, NULL, OCI_ENV_DEFAULT);*/
	// 创建或初始化 OCI 环境 , 并设置环境句柄。
	m_swResult = OCIEnvCreate(&m_pOCIEnv, m_nOCIEnvInitMode, NULL, NULL, NULL, NULL, 0, NULL);  //OCI_OBJECT   OCI_DEFAULT
   	if (m_swResult != OCI_SUCCESS && m_swResult != OCI_SUCCESS_WITH_INFO)
	{
		std::cout << "error! Oracle OCIEnvCreate environment initialization error!" << std::endl;
		exit(1);
	}
	std::cout << "Oracle environment initialization success!" << std::endl;

	// 产生一个指定环境的错误句柄
	m_swResult = OCIHandleAlloc((dvoid *)m_pOCIEnv, (dvoid **)&m_pOCIError, OCI_HTYPE_ERROR, (size_t)0, (dvoid **)0);
	if (OCI_SUCCESS != m_swResult)
	{
		std::cout << "error! Oracle OCIHandleAlloc OCI_HTYPE_ERROR error!" << std::endl;
	}
	
	
	/*	enq_ConHandle = OCI_ConnectionCreate(defDefault_Oracle_DbName, defDefault_Oracle_DbUser, 
							defDefault_Oracle_DbPwd, OCI_SESSION_DEFAULT);*/
	/*OCI_ConnectionAttach*/						
	// 产生一个指定环境的服务句柄
	m_swResult = OCIHandleAlloc((dvoid *)m_pOCIEnv, (dvoid **)&m_pOCIServer, OCI_HTYPE_SERVER, (size_t)0, (dvoid **)0);
	if (OCI_SUCCESS != m_swResult)
	{
		std::cout << "error! Oracle OCIHandleAlloc OCI_HTYPE_SERVER error!" << std::endl;
	}

	// 连接 | 断开服务器 多用户方式连接 ( 创建一个存取路径到数据源 )	
	m_swResult = OCIServerAttach(m_pOCIServer, m_pOCIError, (text *)strOracleTNSName.c_str(), strlen(strOracleTNSName.c_str()), 0);
	if (OCI_SUCCESS != m_swResult)
	{
		int errcno;
		char errbuf[512]={'\0'};
		sb4 errcode;

		// 返回一个错误指针和一个 OCI 错误代码
		OCIErrorGet((dvoid *)m_pOCIError, (ub4)1, (text *)NULL, &errcode, (ub1 *)errbuf, (ub4)sizeof(errbuf), OCI_HTYPE_ERROR);
		errcno = errcode;
		std::cout << "error! OCIServerAttach Oracle server attach error:" << errbuf << std::endl;

	}
	cout << "Oracle server attach success!"<< std::endl;
			
	 /*OCI_ConnectionLogon*/
	/***************** 连接数据库 ****************/ 
	// 产生一个指定环境的用户连接句柄
	m_pOCISession = NULL;
	m_swResult = OCIHandleAlloc((dvoid *)m_pOCIEnv, (dvoid **)&m_pOCISession, (ub4)OCI_HTYPE_SESSION, (size_t) 0, (dvoid **)0);
	if (OCI_SUCCESS != m_swResult)
	{
		cout << "error! OCIHandleAlloc OCI_HTYPE_SESSION "<< std::endl;
	}
	
	// 产生一个指定环境的服务上下文句柄
	m_OCISvcCtx = NULL;
	m_swResult = OCIHandleAlloc((dvoid *)m_pOCIEnv, (dvoid **)&m_OCISvcCtx, OCI_HTYPE_SVCCTX, (size_t) 0, (dvoid **)0);
	if (OCI_SUCCESS != m_swResult)
	{
		cout << "error! OCIHandleAlloc OCI_HTYPE_SVCCTX "<< std::endl;
	}

	// 为指定的句柄及描述符设置特定的属性 ( 为服务句柄指定服务上下文 )
	m_swResult = OCIAttrSet((dvoid *)m_OCISvcCtx, OCI_HTYPE_SVCCTX, (dvoid *)m_pOCIServer, (ub4)0, OCI_ATTR_SERVER, (OCIError *)m_pOCIError);
	if (OCI_SUCCESS != m_swResult)
	{
		cout << "error! OCIAttrSet OCI_HTYPE_SVCCTX OCI_ATTR_SERVER"<< std::endl;
	}
	  
	// 为用户连接句柄设置登录名及密码
	(void) OCIAttrSet((dvoid *)m_pOCISession, (ub4)OCI_HTYPE_SESSION, (dvoid *)strOracleUser.c_str(), (ub4)strlen(strOracleUser.c_str()), (ub4)OCI_ATTR_USERNAME, m_pOCIError);
	(void) OCIAttrSet((dvoid *)m_pOCISession, (ub4)OCI_HTYPE_SESSION, (dvoid *)strOraclePwd.c_str(), (ub4)strlen(strOraclePwd.c_str()), (ub4)OCI_ATTR_PASSWORD, m_pOCIError);

	// 认证用户再建立一个会话连接
	if(OCISessionBegin(m_OCISvcCtx, m_pOCIError, m_pOCISession, OCI_CRED_RDBMS, (ub4)OCI_DEFAULT) != OCI_SUCCESS)
	{
		int errcno;
		char errbuf[512]={'\0'};
		sb4 errcode;
		// 返回一个错误指针和一个 OCI 错误代码

		OCIErrorGet((dvoid *)m_pOCIError, (ub4)1, (text *)NULL, &errcode, (ub1 *)errbuf, (ub4)sizeof(errbuf), OCI_HTYPE_ERROR);
		errcno = errcode;
		cout << "User session error:" << errbuf << endl;
	}
	std::cout << "user session success!" << std::endl;
	(void) OCIAttrSet((dvoid *)m_OCISvcCtx, (ub4) OCI_HTYPE_SVCCTX, (dvoid *)m_pOCISession, (ub4)0, (ub4)OCI_ATTR_SESSION, m_pOCIError);


	//======================================================================================================
	/*enq_EnqTypeInfoHandle = OCI_TypeInfoGet(enq_ConHandle, defDefault_defConfig_PLType, OCI_TIF_TYPE);*/
	m_OCIDescribe = NULL;	
	if (OCI_SUCCESS != OCIHandleAlloc(m_pOCIEnv, (dvoid **) (void *) &m_OCIDescribe, OCI_HTYPE_DESCRIBE, (size_t) 0, (dvoid **) NULL))
	{
		std::cout << "Create OCIHandleAlloc OCI_HTYPE_DESCRIBE error !" << std::endl;
	}
	
	m_swResult = OCITypeByName(m_pOCIEnv, m_pOCIError, m_OCISvcCtx, (CONST text *)"", (ub4) strlen(""),
		(CONST text *)defDefault_defConfig_PLType, (ub4) strlen(defDefault_defConfig_PLType),
		(text *)0, 0, OCI_DURATION_SESSION, OCI_TYPEGET_ALL, &m_OCIType);
		
	if (OCI_SUCCESS != m_swResult)
	{
		std::cout << "Create OCITypeByName  error !" << std::endl;
		int errcno;
		char errbuf[512]={'\0'};
		sb4 errcode;
		// 返回一个错误指针和一个 OCI 错误代码

		OCIErrorGet((dvoid *)m_pOCIError, (ub4)1, (text *)NULL, &errcode, (ub1 *)errbuf, (ub4)sizeof(errbuf), OCI_HTYPE_ERROR);
		errcno = errcode;
		cout << "User session error:" << errbuf << endl;

	}
		
	m_swResult = OCIDescribeAny(m_OCISvcCtx, m_pOCIError, (void *)m_OCIType, 0, OCI_OTYPE_PTR, OCI_DEFAULT, OCI_PTYPE_TYPE, m_OCIDescribe);
	if (OCI_SUCCESS != m_swResult)
	{
		std::cout << "error!  OCIDescribeAny  OCI_OTYPE_PTR, OCI_DEFAULT, OCI_PTYPE_TYPE error !" << std::endl;
	}
	

		
	/*enq_EnqAgentArray[0] = OCI_AgentCreate(enq_ConHandle, defDefault_Server_AgentName, 	defDefault_Oracle_QueueName);*/		
	m_swResult = OCIDescriptorAlloc(m_pOCIEnv, (dvoid **) &m_OCIAQAgentArrayForEnq[0], OCI_DTYPE_AQAGENT,	(size_t) 0, (dvoid **) NULL);
	if (OCI_SUCCESS != m_swResult)
	{
		cout << "Create OCIDescriptorAlloc agthandle error !" << endl;
	} 	
	/*Agent name*/
	//swResult = OCIAttrSet((dvoid *)enq_EnqAgentArray[0], (ub4)OCI_DTYPE_AQAGENT, (dvoid *)"TRANSACT", (ub4)strlen("TRANSACT"), (ub4)OCI_ATTR_AGENT_NAME, errhpp);
	m_swResult = OCIAttrSet((dvoid *)m_OCIAQAgentArrayForEnq[0], (ub4)OCI_DTYPE_AQAGENT, (dvoid *)"TRA_OCC", (ub4)strlen("TRA_OCC"), (ub4)OCI_ATTR_AGENT_NAME, m_pOCIError);
	if (OCI_SUCCESS != m_swResult)
	{
		cout << "Create OCIAttrSet OCI_DTYPE_AQAGENT OCI_ATTR_AGENT_NAME error !" << endl;
	}
	/*Agent Address*/	
	m_swResult = OCIAttrSet((dvoid *)m_OCIAQAgentArrayForEnq[0], (ub4)OCI_DTYPE_AQAGENT, (dvoid *)"TEST_AUDIT_DATA_QUEUE",	(ub4)strlen("TEST_AUDIT_DATA_QUEUE"), (ub4)OCI_ATTR_AGENT_ADDRESS, m_pOCIError);
	if (OCI_SUCCESS != m_swResult)
	{
		cout << "Create OCIAttrSet OCI_DTYPE_AQAGENT OCI_ATTR_AGENT_ADDRESS error !" << endl;
	}
	
	/*OCI_EnqueueCreate*/
	if (OCI_SUCCESS != OCIDescriptorAlloc(m_pOCIEnv, (dvoid **)&m_pOCIAQEnqOptions, OCI_DTYPE_AQENQ_OPTIONS, (size_t)0, (dvoid **)NULL))
	{
		cout << "Create OCIDescriptorAlloc OCI_DTYPE_AQENQ_OPTIONS error !" << endl;
	}
	
	/*OCI_ObjectCreate*/
	 payload = &msg;

	
	/*OCI_MsgCreate*/
	if (OCI_SUCCESS != OCIDescriptorAlloc(m_pOCIEnv, (dvoid **) &m_pOCIAQMsgProperties, OCI_DTYPE_AQMSG_PROPERTIES, (size_t)0, (dvoid **)NULL))
	{
		cout << "Create OCIDescriptorAlloc OCI_DTYPE_AQMSG_PROPERTIES error !" << endl;
	}
	
	
	/*OCI_MsgSetConsumers*/	
	m_swResult =	OCIAttrSet((dvoid *)m_pOCIAQMsgProperties, (ub4)OCI_DTYPE_AQMSG_PROPERTIES, (dvoid *)m_OCIAQAgentArrayForEnq, 
		(ub4)1, (ub4)OCI_ATTR_RECIPIENT_LIST, m_pOCIError);
	if (OCI_SUCCESS != m_swResult)
	{
		cout << "Create OCIAttrSet OCI_ATTR_RECIPIENT_LIST error !" << endl;
	}  	


	/*OCI_ObjectSetString*/											 
	/* prepare the message payload */
	payload->MsgFiter = (OCIString *)0;
	payload->OracleSql = (OCIString *)0;
	payload->MysqlSql = (OCIString *)0;
	
	m_swResult = OCIStringAssignText(m_pOCIEnv, m_pOCIError, (CONST text *)enq_strMsgFierValue.c_str(),
		(ub4) strlen(enq_strMsgFierValue.c_str()), &payload->MsgFiter);
	m_swResult = OCIStringAssignText(m_pOCIEnv, m_pOCIError,(CONST text *)enq_strDataDmlValueOracle.c_str(),
		(ub4) strlen(enq_strDataDmlValueOracle.c_str()), &payload->OracleSql);
	m_swResult = OCIStringAssignText(m_pOCIEnv, m_pOCIError,(CONST text *)enq_strDataDmlValueMysql.c_str(),
		(ub4) strlen(enq_strDataDmlValueMysql.c_str()), &payload->MysqlSql);
	
	nmesg->null_adt = nmesg->null_MsgFiter = nmesg->null_OracleSql = nmesg->null_MysqlSql = OCI_IND_NOTNULL;
	
	

	/*OCI_EnqueuePut*/	
	m_swResult = (boolean) OCIAQEnq(m_OCISvcCtx, m_pOCIError, (text *)defDefault_defConfig_PLType, 
		m_pOCIAQEnqOptions, m_pOCIAQMsgProperties,
		m_OCIType, (void **)&payload, (dvoid **)&nmesg, (OCIRaw **)0, OCI_DEFAULT);
	if (OCI_SUCCESS != m_swResult)
	{
		cout << "error!  OCIAQEnq error !" << endl;
	}  
	/*OCI_Commit*/
	m_swResult = OCITransCommit(m_OCISvcCtx, m_pOCIError, (ub4) 0);
	if (OCI_SUCCESS != m_swResult)
	{
		cout << "error!  OCITransCommit error !" << endl;
	} 
		
	//======================================================================================================


	//====free==================================================================================================
	m_swResult = OCIDescriptorFree((dvoid *)m_OCIAQAgentArrayForEnq[0], OCI_DTYPE_AQAGENT);
	m_OCIAQAgentArrayForEnq[0] = NULL;	
	m_OCIType = NULL;				
	m_swResult = OCIStringResize(m_pOCIEnv, m_pOCIError, 0, &payload->MsgFiter);
	payload->MsgFiter = NULL;
	m_swResult = OCIStringResize(m_pOCIEnv, m_pOCIError, 0, &payload->OracleSql);
	payload->OracleSql = NULL;
	m_swResult = OCIStringResize(m_pOCIEnv, m_pOCIError, 0, &payload->MysqlSql);
	payload->MysqlSql = NULL;
	
	
	m_swResult = OCIDescriptorFree(m_pOCIAQMsgProperties, OCI_DTYPE_AQMSG_PROPERTIES);
	m_pOCIAQMsgProperties = NULL;
	
	m_swResult = OCIDescriptorFree(m_pOCIAQEnqOptions, OCI_DTYPE_AQENQ_OPTIONS);
	m_pOCIAQEnqOptions = NULL;
	

	if (OCI_SUCCESS != m_swResult)
	{
		std::cout << "OCIObjectFree payload error !" << std::endl;
		int errcno;
		char errbuf[512]={'\0'};
		sb4 errcode;
		// 返回一个错误指针和一个 OCI 错误代码

		OCIErrorGet((dvoid *)m_pOCIError, (ub4)1, (text *)NULL, &errcode, (ub1 *)errbuf, (ub4)sizeof(errbuf), OCI_HTYPE_ERROR);
		errcno = errcode;
		cout << "User session error:" << errbuf << endl;

	}
			 	
	m_swResult = OCIHandleFree(m_OCIDescribe, OCI_HTYPE_DESCRIBE);
	m_OCIDescribe = NULL; 
	
	
	
	
	m_swResult = OCISessionEnd(m_OCISvcCtx, m_pOCIError, m_pOCISession, (ub4) OCI_DEFAULT);


	m_swResult = OCIHandleFree((dvoid *) m_OCISvcCtx, OCI_HTYPE_SVCCTX);
	m_OCISvcCtx = NULL;

	
	m_swResult = OCIServerDetach(m_pOCIServer, m_pOCIError, OCI_DEFAULT);
	if (OCI_SUCCESS != m_swResult)
	{
		std::cout << "OCIServerDetach  error !" << std::endl;
		int errcno;
		char errbuf[512]={'\0'};
		sb4 errcode;
		// 返回一个错误指针和一个 OCI 错误代码

		OCIErrorGet((dvoid *)m_pOCIError, (ub4)1, (text *)NULL, &errcode, (ub1 *)errbuf, (ub4)sizeof(errbuf), OCI_HTYPE_ERROR);
		errcno = errcode;
		cout << "User session error:" << errbuf << endl;

	}
	

	m_swResult = OCIHandleFree((dvoid *)m_pOCISession,OCI_HTYPE_SESSION);
	m_pOCISession = NULL;

	m_swResult = OCIHandleFree((dvoid *) m_pOCIServer, OCI_HTYPE_SERVER);
	m_pOCIServer = NULL;


	
	m_swResult = OCIHandleFree((dvoid *) m_pOCIError, OCI_HTYPE_ERROR);
	m_pOCIError = NULL;	
			
	m_swResult = OCIHandleFree((dvoid *)m_pOCIEnv,OCI_HTYPE_ENV);
	m_pOCIEnv = NULL; 
	
	return 0;
}







int CTestOCIEnqDeq::test_fun_oci_deq()
{
	OCIEnv *m_pOCIEnv = NULL;		//初始化OCI环境句柄指针对应的句柄类型 OCI__HTYPE_ENV
	OCIServer *m_pOCIServer = NULL;	// 初始化服务器句柄 , 对应的句柄类型 OCI_HTYPE_SERVER
	OCIError *m_pOCIError = NULL;   // 用于捕获 OCI 错误信息 , 对应的句柄类型 OCI_HTYPE_ERROR
	OCISession *m_pOCISession = NULL;	 // 初始化用户连接句柄 , 对应的句柄类型 OCI_HTYPE_SESSION
	OCISvcCtx *m_OCISvcCtx = NULL;  // 初始化服务上下文句柄 , 对应的句柄类型 OCI_HTYPE_SVCCTX
	OCIDescribe *m_OCIDescribe   = NULL;   
	OCIType *m_OCIType = NULL;
	ub4 m_nOCIEnvInitMode = OCI_DEFAULT | OCI_OBJECT;
	sword m_swResult;
	OCIAQDeqOptions *m_pOCIAQDeqOptions = NULL;
	ub4 m_nDeqWaitTime = OCI_DEQ_NO_WAIT;
	ub4 m_nDeqNavigation = OCI_DEQ_FIRST_MSG;

	 


	std::string strOracleTNSName= defDefault_Oracle_DbName;//
	std::string strOracleUser = defDefault_Oracle_DbUser;	 
	std::string strOraclePwd = defDefault_Oracle_DbPwd;	
	
	
	AQMessage			*deqmesg = NULL;	
	AQMessageNull	*ndeqmesg = NULL;



	/*	enq_nFunRes = OCI_Initialize(NULL, NULL, OCI_ENV_DEFAULT);*/
	// 创建或初始化 OCI 环境 , 并设置环境句柄。
	m_swResult = OCIEnvCreate(&m_pOCIEnv, m_nOCIEnvInitMode, NULL, NULL, NULL, NULL, 0, NULL);  //OCI_OBJECT   OCI_DEFAULT
   	if (m_swResult != OCI_SUCCESS && m_swResult != OCI_SUCCESS_WITH_INFO)
	{
		std::cout << "error! Oracle OCIEnvCreate environment initialization error!" << std::endl;
		exit(1);
	}
	std::cout << "Oracle environment initialization success!" << std::endl;

	// 产生一个指定环境的错误句柄
	m_swResult = OCIHandleAlloc((dvoid *)m_pOCIEnv, (dvoid **)&m_pOCIError, OCI_HTYPE_ERROR, (size_t)0, (dvoid **)0);
	if (OCI_SUCCESS != m_swResult)
	{
		std::cout << "error! Oracle OCIHandleAlloc OCI_HTYPE_ERROR error!" << std::endl;
	}
	
	
	/*	enq_ConHandle = OCI_ConnectionCreate(defDefault_Oracle_DbName, defDefault_Oracle_DbUser, 
							defDefault_Oracle_DbPwd, OCI_SESSION_DEFAULT);*/
	/*OCI_ConnectionAttach*/						
	// 产生一个指定环境的服务句柄
	m_swResult = OCIHandleAlloc((dvoid *)m_pOCIEnv, (dvoid **)&m_pOCIServer, OCI_HTYPE_SERVER, (size_t)0, (dvoid **)0);
	if (OCI_SUCCESS != m_swResult)
	{
		std::cout << "error! Oracle OCIHandleAlloc OCI_HTYPE_SERVER error!" << std::endl;
	}

	// 连接 | 断开服务器 多用户方式连接 ( 创建一个存取路径到数据源 )	
	m_swResult = OCIServerAttach(m_pOCIServer, m_pOCIError, (text *)strOracleTNSName.c_str(), strlen(strOracleTNSName.c_str()), 0);
	if (OCI_SUCCESS != m_swResult)
	{
		int errcno;
		char errbuf[512]={'\0'};
		sb4 errcode;

		// 返回一个错误指针和一个 OCI 错误代码
		OCIErrorGet((dvoid *)m_pOCIError, (ub4)1, (text *)NULL, &errcode, (ub1 *)errbuf, (ub4)sizeof(errbuf), OCI_HTYPE_ERROR);
		errcno = errcode;
		std::cout << "error! OCIServerAttach Oracle server attach error:" << errbuf << std::endl;

	}
	cout << "Oracle server attach success!"<< std::endl;
			
	 /*OCI_ConnectionLogon*/
	/***************** 连接数据库 ****************/ 
	// 产生一个指定环境的用户连接句柄
	m_pOCISession = NULL;
	m_swResult = OCIHandleAlloc((dvoid *)m_pOCIEnv, (dvoid **)&m_pOCISession, (ub4)OCI_HTYPE_SESSION, (size_t) 0, (dvoid **)0);
	if (OCI_SUCCESS != m_swResult)
	{
		cout << "error! OCIHandleAlloc OCI_HTYPE_SESSION "<< std::endl;
	}
	
	// 产生一个指定环境的服务上下文句柄
	m_OCISvcCtx = NULL;
	m_swResult = OCIHandleAlloc((dvoid *)m_pOCIEnv, (dvoid **)&m_OCISvcCtx, OCI_HTYPE_SVCCTX, (size_t) 0, (dvoid **)0);
	if (OCI_SUCCESS != m_swResult)
	{
		cout << "error! OCIHandleAlloc OCI_HTYPE_SVCCTX "<< std::endl;
	}

	// 为指定的句柄及描述符设置特定的属性 ( 为服务句柄指定服务上下文 )
	m_swResult = OCIAttrSet((dvoid *)m_OCISvcCtx, OCI_HTYPE_SVCCTX, (dvoid *)m_pOCIServer, (ub4)0, OCI_ATTR_SERVER, (OCIError *)m_pOCIError);
	if (OCI_SUCCESS != m_swResult)
	{
		cout << "error! OCIAttrSet OCI_HTYPE_SVCCTX OCI_ATTR_SERVER"<< std::endl;
	}
	  
	// 为用户连接句柄设置登录名及密码
	(void) OCIAttrSet((dvoid *)m_pOCISession, (ub4)OCI_HTYPE_SESSION, (dvoid *)strOracleUser.c_str(), (ub4)strlen(strOracleUser.c_str()), (ub4)OCI_ATTR_USERNAME, m_pOCIError);
	(void) OCIAttrSet((dvoid *)m_pOCISession, (ub4)OCI_HTYPE_SESSION, (dvoid *)strOraclePwd.c_str(), (ub4)strlen(strOraclePwd.c_str()), (ub4)OCI_ATTR_PASSWORD, m_pOCIError);

	// 认证用户再建立一个会话连接
	if(OCISessionBegin(m_OCISvcCtx, m_pOCIError, m_pOCISession, OCI_CRED_RDBMS, (ub4)OCI_DEFAULT) != OCI_SUCCESS)
	{
		int errcno;
		char errbuf[512]={'\0'};
		sb4 errcode;
		// 返回一个错误指针和一个 OCI 错误代码

		OCIErrorGet((dvoid *)m_pOCIError, (ub4)1, (text *)NULL, &errcode, (ub1 *)errbuf, (ub4)sizeof(errbuf), OCI_HTYPE_ERROR);
		errcno = errcode;
		cout << "User session error:" << errbuf << endl;
	
	}
	std::cout << "user session success!" << std::endl;
	(void) OCIAttrSet((dvoid *)m_OCISvcCtx, (ub4) OCI_HTYPE_SVCCTX, (dvoid *)m_pOCISession, (ub4)0, (ub4)OCI_ATTR_SESSION, m_pOCIError);


	//======================================================================================================
	/*enq_EnqTypeInfoHandle = OCI_TypeInfoGet(enq_ConHandle, defDefault_defConfig_PLType, OCI_TIF_TYPE);*/
	m_OCIDescribe = NULL;	
	if (OCI_SUCCESS != OCIHandleAlloc(m_pOCIEnv, (dvoid **) (void *) &m_OCIDescribe, OCI_HTYPE_DESCRIBE, (size_t) 0, (dvoid **) NULL))
	{
		std::cout << "Create OCIHandleAlloc OCI_HTYPE_DESCRIBE error !" << std::endl;
	}
	
	m_swResult = OCITypeByName(m_pOCIEnv, m_pOCIError, m_OCISvcCtx, (CONST text *)"", (ub4) strlen(""),
		(CONST text *)defDefault_defConfig_PLType, (ub4) strlen(defDefault_defConfig_PLType),
		(text *)0, 0, OCI_DURATION_SESSION, OCI_TYPEGET_ALL, &m_OCIType);
		
	if (OCI_SUCCESS != m_swResult)
	{
		std::cout << "Create OCITypeByName  error !" << std::endl;
		int errcno;
		char errbuf[512]={'\0'};
		sb4 errcode;
		// 返回一个错误指针和一个 OCI 错误代码

		OCIErrorGet((dvoid *)m_pOCIError, (ub4)1, (text *)NULL, &errcode, (ub1 *)errbuf, (ub4)sizeof(errbuf), OCI_HTYPE_ERROR);
		errcno = errcode;
		cout << "User session error:" << errbuf << endl;

	}
		
	m_swResult = OCIDescribeAny(m_OCISvcCtx, m_pOCIError, (void *)m_OCIType, 0, OCI_OTYPE_PTR, OCI_DEFAULT, OCI_PTYPE_TYPE, m_OCIDescribe);
	if (OCI_SUCCESS != m_swResult)
	{
		std::cout << "error!  OCIDescribeAny  OCI_OTYPE_PTR, OCI_DEFAULT, OCI_PTYPE_TYPE error !" << std::endl;
	}
	

	
	
	
	
	//=================================================================
	
	
	
	/* now dequeue the messages using different consumer names */
	/* allocate dequeue options descriptor to set the dequeue options */
	OCIDescriptorAlloc(m_pOCIEnv, (dvoid **)&m_pOCIAQDeqOptions, OCI_DTYPE_AQDEQ_OPTIONS, 0, (dvoid **)0);

	/* set wait parameter to NO_WAIT so that the dequeue returns immediately */
	OCIAttrSet(m_pOCIAQDeqOptions, OCI_DTYPE_AQDEQ_OPTIONS, (dvoid *)&m_nDeqWaitTime, 0,	OCI_ATTR_WAIT, m_pOCIError);

	/* set navigation to FIRST_MESSAGE so that the dequeue resets the position */
	/* after a new consumer_name is set in the dequeue options     */
	OCIAttrSet(m_pOCIAQDeqOptions, OCI_DTYPE_AQDEQ_OPTIONS, (dvoid *)&m_nDeqNavigation, 0, OCI_ATTR_NAVIGATION, m_pOCIError);

	/* dequeue from the msg_queue_multiple as consumer BLUE */
	OCIAttrSet(m_pOCIAQDeqOptions, OCI_DTYPE_AQDEQ_OPTIONS, (dvoid *)defDefault_Client_AgentName, strlen(defDefault_Client_AgentName), OCI_ATTR_CONSUMER_NAME, m_pOCIError);
	while (OCIAQDeq(m_OCISvcCtx, m_pOCIError, (OraText *)defDefault_defConfig_PLType, m_pOCIAQDeqOptions, 0,
		m_OCIType, (dvoid **)&deqmesg, (dvoid **)&ndeqmesg, 0, 0) 
		== OCI_SUCCESS)
	{
		printf("deqmesg->MsgFiter: %s\n", OCIStringPtr(m_pOCIEnv, deqmesg->MsgFiter));
		printf("deqmesg->OracleSql: %s\n", OCIStringPtr(m_pOCIEnv, deqmesg->OracleSql));
		printf("deqmesg->MysqlSql: %s\n", OCIStringPtr(m_pOCIEnv, deqmesg->MysqlSql));

	}
	OCITransCommit(m_OCISvcCtx, m_pOCIError, (ub4) 0);
	if (OCI_SUCCESS != m_swResult)
	{
		cout << "error!  OCITransCommit error !" << endl;
	} 
	//================================================================

	//====free==================================================================================================
		
	m_OCIType = NULL;//do not need to free
	
	m_swResult = OCIStringResize(m_pOCIEnv, m_pOCIError, 0, &deqmesg->MsgFiter);
	deqmesg->MsgFiter = NULL;
	m_swResult = OCIStringResize(m_pOCIEnv, m_pOCIError, 0, &deqmesg->OracleSql);
	deqmesg->OracleSql = NULL;
	m_swResult = OCIStringResize(m_pOCIEnv, m_pOCIError, 0, &deqmesg->MysqlSql);
	deqmesg->MysqlSql = NULL;
	
	m_swResult = OCIRawResize(m_pOCIEnv, m_pOCIError, 0, (OCIRaw **) &deqmesg);

	deqmesg = NULL;
	
	if (OCI_SUCCESS != m_swResult)
	{
		std::cout << "OCIObjectFree payload error !" << std::endl;
		int errcno;
		char errbuf[512]={'\0'};
		sb4 errcode;
		// 返回一个错误指针和一个 OCI 错误代码

		OCIErrorGet((dvoid *)m_pOCIError, (ub4)1, (text *)NULL, &errcode, (ub1 *)errbuf, (ub4)sizeof(errbuf), OCI_HTYPE_ERROR);
		errcno = errcode;
		cout << "User session error:" << errbuf << endl;

	}	
	m_swResult = OCIDescriptorFree(m_pOCIAQDeqOptions, OCI_DTYPE_AQDEQ_OPTIONS);
	m_pOCIAQDeqOptions = NULL;	
			 	
	m_swResult = OCIHandleFree(m_OCIDescribe, OCI_HTYPE_DESCRIBE);
	m_OCIDescribe = NULL;	
	
	m_swResult = OCISessionEnd(m_OCISvcCtx, m_pOCIError, m_pOCISession, (ub4) OCI_DEFAULT);	

	m_swResult = OCIHandleFree((dvoid *) m_OCISvcCtx, OCI_HTYPE_SVCCTX);
	m_OCISvcCtx = NULL;
	
	m_swResult = OCIServerDetach(m_pOCIServer, m_pOCIError, OCI_DEFAULT);
	if (OCI_SUCCESS != m_swResult)
	{
		std::cout << "OCIServerDetach  error !" << std::endl;
		int errcno;
		char errbuf[512]={'\0'};
		sb4 errcode;
		// 返回一个错误指针和一个 OCI 错误代码

		OCIErrorGet((dvoid *)m_pOCIError, (ub4)1, (text *)NULL, &errcode, (ub1 *)errbuf, (ub4)sizeof(errbuf), OCI_HTYPE_ERROR);
		errcno = errcode;
		cout << "User session error:" << errbuf << endl;

	}
	

	m_swResult = OCIHandleFree((dvoid *)m_pOCISession,OCI_HTYPE_SESSION);
	m_pOCISession = NULL;

	m_swResult = OCIHandleFree((dvoid *) m_pOCIServer, OCI_HTYPE_SERVER);
	m_pOCIServer = NULL;


	
	m_swResult = OCIHandleFree((dvoid *) m_pOCIError, OCI_HTYPE_ERROR);
	m_pOCIError = NULL;	
			
	m_swResult = OCIHandleFree((dvoid *)m_pOCIEnv,OCI_HTYPE_ENV);
	m_pOCIEnv = NULL; 
	
	return 0;
}





//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


 
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////





int CTestOCIEnqDeq::test_fun_oci_enq_one_msg()
{  
	AQMessage			enqMsg;
	AQMessageNull		enqNullMsg;
	AQMessage*			pAQMsg = &enqMsg;
	AQMessageNull*		pAQNullMsg = &enqNullMsg; 

	std::string			strOracleQueueName =	defDefault_Oracle_QueueName;
	std::string			strMsgFierValue;
	std::string			strDataDmlValueOracle;
	std::string			strDataDmlValueMysql;
	sword				swResult = 0; 

	int nErrNo = 0;
	char szErrBuf[def_int_Buf_Size]={'\0'};
	sb4 nErrCode = 0;	

	MyOCIHandle* m_pOCI = new MyOCIHandle();
	_InitMyOCIHandle(m_pOCI);


	MyOCIHandleEnq *pMyOCIHandleEnq = NULL;
	pMyOCIHandleEnq = new MyOCIHandleEnq();
	if (NULL == pMyOCIHandleEnq)
	{
		return -1;
	}


	strMsgFierValue = "INSERT:TRA_OCC";	//"INSERT:TRA_OCC"
	strDataDmlValueOracle = "Oracle76=insert into queue_test_mysql(ID, NAME, TIMESTAMP) values(1, 'NAME', sysdate)";  
	strDataDmlValueMysql = "MySQL78=insert into queue_test_mysql(ID, NAME, TIMESTAMP) values(1, 'NAME', sysdate())";	

	_InitMyOCIHandleEnq(m_pOCI, pMyOCIHandleEnq);


	/*OCI_ObjectCreate*/
	pAQMsg = &enqMsg;  	



	/* prepare the message payload */
	pAQMsg->MsgFiter = (OCIString *)0;
	pAQMsg->OracleSql = (OCIString *)0;
	pAQMsg->MysqlSql = (OCIString *)0;



	/*set string*/
	swResult = OCIStringAssignText(m_pOCI->m_pOCIEnv, m_pOCI->m_pOCIError, (CONST text *)strMsgFierValue.c_str(),
		(ub4) strlen(strMsgFierValue.c_str()), &pAQMsg->MsgFiter);
	swResult = OCIStringAssignText(m_pOCI->m_pOCIEnv, m_pOCI->m_pOCIError,(CONST text *)strDataDmlValueOracle.c_str(),
		(ub4) strlen(strDataDmlValueOracle.c_str()), &pAQMsg->OracleSql);
	swResult = OCIStringAssignText(m_pOCI->m_pOCIEnv, m_pOCI->m_pOCIError,(CONST text *)strDataDmlValueMysql.c_str(),
		(ub4) strlen(strDataDmlValueMysql.c_str()), &pAQMsg->MysqlSql);
	pAQNullMsg->null_adt = pAQNullMsg->null_MsgFiter = pAQNullMsg->null_OracleSql = pAQNullMsg->null_MysqlSql = OCI_IND_NOTNULL;

	/*enq*/
	swResult = (boolean) OCIAQEnq(pMyOCIHandleEnq->m_OCISvcCtxForEnq, m_pOCI->m_pOCIError, (text *)(strOracleQueueName.c_str()), 
		pMyOCIHandleEnq->m_pOCIAQEnqOptionsForEnq, pMyOCIHandleEnq->m_pOCIAQMsgPropertiesForEnq,
		pMyOCIHandleEnq->m_OCITypeForEnq, (void **)&pAQMsg, (dvoid **)&pAQNullMsg, (OCIRaw **)0, OCI_DEFAULT);
	if (OCI_SUCCESS != swResult)
	{	
		memset(szErrBuf, 0,def_int_Buf_Size);  
		OCIErrorGet((dvoid *)m_pOCI->m_pOCIError, (ub4)1, (text *)NULL, &nErrCode, (ub1 *)szErrBuf, (ub4)sizeof(szErrBuf), OCI_HTYPE_ERROR);
		nErrNo = nErrCode;
		std::cout << "error! nErrNo=%d szErrBuf=%s" << nErrNo<<szErrBuf << std::endl;
	}







	/*OCI_Commit*/
	swResult = OCITransCommit(pMyOCIHandleEnq->m_OCISvcCtxForEnq, m_pOCI->m_pOCIError, (ub4) 0);
	if (OCI_SUCCESS != swResult)
	{	
		memset(szErrBuf, 0,def_int_Buf_Size);  
		OCIErrorGet((dvoid *)m_pOCI->m_pOCIError, (ub4)1, (text *)NULL, &nErrCode, (ub1 *)szErrBuf, (ub4)sizeof(szErrBuf), OCI_HTYPE_ERROR);
		nErrNo = nErrCode;
		std::cout << "error! nErrNo=%d szErrBuf=%s" << nErrNo<<szErrBuf << std::endl;
	}




	//free data		
	swResult = OCIStringResize(m_pOCI->m_pOCIEnv, m_pOCI->m_pOCIError, 0, &pAQMsg->MsgFiter);
	pAQMsg->MsgFiter = NULL;
	swResult = OCIStringResize(m_pOCI->m_pOCIEnv, m_pOCI->m_pOCIError, 0, &pAQMsg->OracleSql);
	pAQMsg->OracleSql = NULL;
	swResult = OCIStringResize(m_pOCI->m_pOCIEnv, m_pOCI->m_pOCIError, 0, &pAQMsg->MysqlSql);
	pAQMsg->MysqlSql = NULL;



	_UninitMyOCIHandleEnq(m_pOCI, pMyOCIHandleEnq);
	if (NULL != pMyOCIHandleEnq)
	{
		delete pMyOCIHandleEnq;
		pMyOCIHandleEnq = NULL;
	}

	_UninitMyOCIHandle(m_pOCI);
	DEF_DELETE(m_pOCI);


	return 0;
}



int CTestOCIEnqDeq::_PrintOracleFunErrorInfo(const std::string& strAddInfo, MyOCIHandle* pOCI)
{
	int nFunRes = 0;
	
	int nErrNo = 0;
	char* pszErrBuf = NULL;//[def_int_Buf_Size]={'\0'};
	sb4 nErrCode = 0;
	
	if (NULL == pOCI)
	{
		nFunRes = -1;
		return nFunRes;
	}
		
	
	pszErrBuf = new char[def_int_Buf_Size];
	memset(pszErrBuf, 0, def_int_Buf_Size);

		
	OCIErrorGet((dvoid *)pOCI->m_pOCIError, (ub4)1, (text *)NULL, &nErrCode, (ub1 *)pszErrBuf, (ub4)def_int_Buf_Size, OCI_HTYPE_ERROR);
	nErrNo = nErrCode;
	std::cout << strAddInfo << " error! nErrNo=%d szErrBuf=%s" << nErrNo << pszErrBuf << std::endl; 
// 	LOG_GENERIC(SourceInfo, DebugUtil::DebugError, "%s, error! nErrNo=%d szErrBuf=%s",
// 		strAddInfo.c_str(), nErrNo, pszErrBuf);
		
	DEF_DELETE_ARRAY(pszErrBuf);	
	
	return nFunRes;
}




int CTestOCIEnqDeq::test_fun_oci_enq_one_msg_useobject()
{  
	AQMessage			enqMsg;
	AQMessageNull		enqNullMsg;
	AQMessage*			pAQMsg = &enqMsg;
	AQMessageNull*		pAQNullMsg = &enqNullMsg; 
	std::string			strOracleQueueName;
	std::string			strMsgFierValue;
	std::string			strDataDmlValueOracle;
	std::string			strDataDmlValueMysql;
	sword				swResult = 0; 
	std::string         strLogAddInfo;	
	MyOCIHandle*		m_pOCI = NULL;
	MyOCIHandleEnq*     pMyOCIHandleEnq = NULL;

	m_pOCI = new MyOCIHandle();
	_InitMyOCIHandle(m_pOCI);	
	
	pMyOCIHandleEnq = new MyOCIHandleEnq();
	_InitMyOCIHandleEnq(m_pOCI, pMyOCIHandleEnq);

	strOracleQueueName =	defDefault_Oracle_QueueName;
	strMsgFierValue = "INSERT:TRA_OCC";	//"INSERT:TRA_OCC"
	strDataDmlValueOracle = "Oracle76=insert into queue_test_mysql(ID, NAME, TIMESTAMP) values(1, 'NAME', sysdate)";  
	strDataDmlValueMysql = "MySQL78=insert into queue_test_mysql(ID, NAME, TIMESTAMP) values(1, 'NAME', sysdate())";	
	 
	/*OCI_ObjectCreate*/
	pAQMsg = &enqMsg; 

	/* prepare the message payload */
	pAQMsg->MsgFiter = (OCIString *)0;
	pAQMsg->OracleSql = (OCIString *)0;
	pAQMsg->MysqlSql = (OCIString *)0; 

	/*set string*/
	swResult = OCIStringAssignText(m_pOCI->m_pOCIEnv, m_pOCI->m_pOCIError, (CONST text *)strMsgFierValue.c_str(),
		(ub4) strlen(strMsgFierValue.c_str()), &pAQMsg->MsgFiter);
	if (OCI_SUCCESS != swResult)
	{
		strLogAddInfo = "OCIStringAssignText()";
		_PrintOracleFunErrorInfo(strLogAddInfo, m_pOCI);
	}
		
	swResult = OCIStringAssignText(m_pOCI->m_pOCIEnv, m_pOCI->m_pOCIError,(CONST text *)strDataDmlValueOracle.c_str(),
		(ub4) strlen(strDataDmlValueOracle.c_str()), &pAQMsg->OracleSql);
	if (OCI_SUCCESS != swResult)
	{
		strLogAddInfo = "OCIStringAssignText()";
		_PrintOracleFunErrorInfo(strLogAddInfo, m_pOCI);
	}
		
	swResult = OCIStringAssignText(m_pOCI->m_pOCIEnv, m_pOCI->m_pOCIError,(CONST text *)strDataDmlValueMysql.c_str(),
		(ub4) strlen(strDataDmlValueMysql.c_str()), &pAQMsg->MysqlSql);
	if (OCI_SUCCESS != swResult)
	{
		strLogAddInfo = "OCIStringAssignText()";
		_PrintOracleFunErrorInfo(strLogAddInfo, m_pOCI);
	}
	
		
	pAQNullMsg->null_adt = OCI_IND_NOTNULL; 
	pAQNullMsg->null_MsgFiter = OCI_IND_NOTNULL;
	pAQNullMsg->null_OracleSql = OCI_IND_NOTNULL;
	pAQNullMsg->null_MysqlSql = OCI_IND_NOTNULL;

	/*enq*/
	swResult = (boolean) OCIAQEnq(pMyOCIHandleEnq->m_OCISvcCtxForEnq, m_pOCI->m_pOCIError, (text *)(strOracleQueueName.c_str()), 
		pMyOCIHandleEnq->m_pOCIAQEnqOptionsForEnq, pMyOCIHandleEnq->m_pOCIAQMsgPropertiesForEnq,
		pMyOCIHandleEnq->m_OCITypeForEnq, (void **)&pAQMsg, (dvoid **)&pAQNullMsg, (OCIRaw **)0, OCI_DEFAULT);
	if (OCI_SUCCESS != swResult)
	{	
		strLogAddInfo = "OCIAQEnq()";
		_PrintOracleFunErrorInfo(strLogAddInfo, m_pOCI);
	}
	
	/*OCI_Commit*/
	swResult = OCITransCommit(pMyOCIHandleEnq->m_OCISvcCtxForEnq, m_pOCI->m_pOCIError, (ub4) 0);
	if (OCI_SUCCESS != swResult)
	{	
		strLogAddInfo = "OCITransCommit()";
		_PrintOracleFunErrorInfo(strLogAddInfo, m_pOCI);
	}
	
	//free data		
	if (NULL != pAQMsg->MsgFiter)
	{
		swResult = OCIStringResize(m_pOCI->m_pOCIEnv, m_pOCI->m_pOCIError, 0, &pAQMsg->MsgFiter);
		pAQMsg->MsgFiter = NULL;
	}
	
	if (NULL != pAQMsg->OracleSql)
	{
		swResult = OCIStringResize(m_pOCI->m_pOCIEnv, m_pOCI->m_pOCIError, 0, &pAQMsg->OracleSql);
		pAQMsg->OracleSql = NULL;
	}
	
	if (NULL != pAQMsg->MysqlSql)
	{
		swResult = OCIStringResize(m_pOCI->m_pOCIEnv, m_pOCI->m_pOCIError, 0, &pAQMsg->MysqlSql);
		pAQMsg->MysqlSql = NULL;
	}
	
	_UninitMyOCIHandleEnq(m_pOCI, pMyOCIHandleEnq);
	if (NULL != pMyOCIHandleEnq)
	{
		DEF_DELETE(pMyOCIHandleEnq);
	}

	_UninitMyOCIHandle(m_pOCI);
	DEF_DELETE(m_pOCI);


	return 0;
}




int CTestOCIEnqDeq::test_fun_oci_enq_one_msg_useobject_T2()
{  
	int nFunRes = 0;

	return nFunRes;
}














//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
























































































