#if !defined(ALARMACCESSFACTORYTEST_H)
#define ALARMACCESSFACTORYTEST_H

#include "CommonDef.h"
#include "CommonData.h"

#include <string>
#include <vector>

//#include "C:/TransactiveWS/shenglonglin_Lenovo/COTS/oracle/10.2.0/winnt/inc/oci.h"
#include "oci.h"


//===========================================================================
typedef struct tag_AQMessage
{
	OCIString   *MsgFiter;
	OCIString   *OracleSql;
	OCIString   *MysqlSql;

}AQMessage;


typedef struct tag_AQMessageNull
{
	OCIInd    null_adt;//must have this member
	OCIInd    null_MsgFiter;
	OCIInd    null_OracleSql;
	OCIInd    null_MysqlSql;
}AQMessageNull;


/*
CREATE OR REPLACE
TYPE "IT271350_5"."AUDIT_DATA_QUEUE_PAYLOAD_TYPE" as object (
	message_qualifier  VARCHAR2(100),
	db_srv_id  NUMBER,
	central_sid  NUMBER,
	audit_data_oracle VARCHAR2(4000),
	audit_data_mysql VARCHAR2(4000)
	)
*/

//===========================================================================

typedef struct tag_AQMessageNEW
{

	OCIString	*message_qualifier;//  VARCHAR2(100),
	OCINumber	*db_srv_id;//  NUMBER,
	OCINumber	*central_sid;//  NUMBER,
	OCIString	*audit_data_oracle;// VARCHAR2(4000),
	OCIString	*audit_data_mysql;// VARCHAR2(4000)
}AQMessageNEW;





//===========================================================================






//===========================================================================


typedef struct tag_MyOCIHandleEnq
{
	sword				m_swResult;

	OCIServer			*m_pOCIServerForEnq;		
	OCISession			*m_pOCISessionForEnq;		
	OCISvcCtx			*m_OCISvcCtxForEnq;			
	OCIDescribe			*m_OCIDescribeForEnq;			
	OCIType				*m_OCITypeForEnq;


	OCIAQAgent			*m_OCIAQAgentArrayForEnq[1];
	OCIAQEnqOptions		*m_pOCIAQEnqOptionsForEnq;
	OCIAQMsgProperties  *m_pOCIAQMsgPropertiesForEnq;


}MyOCIHandleEnq;





typedef struct tag_MyOCIHandleDeq
{
	sword				m_swResult;	

	OCIServer			*m_pOCIServerForDeq;			
	OCISession			*m_pOCISessionForDeq;			
	OCISvcCtx			*m_OCISvcCtxForDeq;		
	OCIDescribe			*m_OCIDescribeForDeq;			
	OCIType				*m_OCITypeForDeq;  

	OCIAQDeqOptions		*m_pOCIAQDeqOptionsForDeq;
	ub4					m_nDeqWaitTimeForDeq; 
	ub4					m_nDeqNavigationForDeq;

}MyOCIHandleDeq; 



typedef struct tag_MyOCIHandle
{
	OCIEnv				*m_pOCIEnv;				//初始化OCI环境句柄指针对应的句柄类型 OCI__HTYPE_ENV
	OCIError			*m_pOCIError;			// 用于捕获 OCI 错误信息 , 对应的句柄类型 OCI_HTYPE_ERROR
	ub4					m_nOCIEnvInitMode;
	sword				m_swResult;

}MyOCIHandle;








typedef long  clock_t;

#ifdef __WIN32__
typedef void threadrounte;
#else
typedef void* threadrounte;
#endif

class CTestOCIEnqDeq 
{
public:
	CTestOCIEnqDeq ( const std::string & name ) 
	{
	}

	virtual ~CTestOCIEnqDeq()
	{
	}


private:
	struct timeb                                    m_fStepOneEnqDataBegin;	
	struct timeb                                    m_fStepOneEnqDataEnd;
	std::string										m_strStepOneEnqDataTime;

	struct timeb                                    m_fStepTwoEnqCommitBegin;	
	struct timeb                                    m_fStepTwoEnqCommitEnd;
	std::string										m_strStepTwoEnqCommitTime;

	struct timeb                                    m_fEach200EnqWorkBegin;//200
	struct timeb                                    m_fEach200EnqWorkEnd;		
	std::string                                     m_fEach200EnqWorkUse;		

	struct timeb                                    m_fEach20000EnqWorkBegin;
	struct timeb                                    m_fEach20000EnqWorkEnd;		
	std::string                                     m_fEach20000EnqWorkUse;		


private:
	struct timeb                                    m_fStepOneDeqDataBegin;	
	struct timeb                                    m_fStepOneDeqDataEnd;
	std::string										m_strStepOneDeqDataTime;

	struct timeb                                    m_fStepTwoDeqCommitBegin;	
	struct timeb                                    m_fStepTwoDeqCommitEnd;
	std::string										m_strStepTwoDeqCommitTime;

	struct timeb                                    m_fEach200DeqWorkBegin;//200
	struct timeb                                    m_fEach200DeqWorkEnd;		
	std::string                                     m_fEach200DeqWorkUse;		

	struct timeb                                    m_fEach20000DeqWorkBegin;
	struct timeb                                    m_fEach20000DeqWorkEnd;		
	std::string                                     m_fEach20000DeqWorkUse;		




protected:

	static threadrounte test_DaiGetSetValue_one_thread(void* pParam);
	static threadrounte test_DaiGetSetValue_two_thread(void* pParam);


private:	
	void testDaiGetSetValue_Muti_thread();
	
private:
	std::string _GetCurrentTime(struct timeb* pfbtime);
	std::string _GetDiffTime(struct timeb* pfbtimeBegin, struct timeb* pfbtimeEnd);
	

public:
	int _InitMyOCIHandle(MyOCIHandle *pMyOCIHandle);
	int _UninitMyOCIHandle(MyOCIHandle *pMyOCIHandle);

public:
	int test_fun_oci_enq();
	int test_fun_oci_deq();
	int test_fun_oci_enq_many();
	int test_fun_oci_deq_many();
	int test_fun_oci_enq_one_msg();

	int _PrintOracleFunErrorInfo(const std::string& strAddInfo,MyOCIHandle* pOCI);
	int test_fun_oci_enq_one_msg_useobject();
	int test_fun_oci_enq_one_msg_useobject_T2();

	
	int _InitMyOCIHandleEnq(MyOCIHandle *pOCI, MyOCIHandleEnq *pMyOCIHandleEnq);
	int _UninitMyOCIHandleEnq(MyOCIHandle *pOCI, MyOCIHandleEnq *pMyOCIHandleEnq);
	int _InitMyOCIHandleDeq(MyOCIHandle *pOCI, MyOCIHandleDeq *pMyOCIHandleDeq);
	int _UninitMyOCIHandleDeq(MyOCIHandle *pOCI, MyOCIHandleDeq *pMyOCIHandleDeq);
};

#endif // !defined(ALARMRULEACCESSFACTORYTEST_H)










#if 0
CREATE OR REPLACE
	TYPE "IT271350_5"."TEST_AUDIT_DATA_TYPE" as object (
	message_qualifier  VARCHAR2(100),
	audit_data_oracle VARCHAR2(4000),
	audit_data_mysql VARCHAR2(4000)
	)
	/





	BEGIN
	sys.dbms_aqadm.stop_queue 
	(queue_name => 'IT271350_5.TEST_AUDIT_DATA_QUEUE');
sys.dbms_aqadm.drop_queue 
	(queue_name => 'IT271350_5.TEST_AUDIT_DATA_QUEUE');
END;
/

	BEGIN
	sys.dbms_aqadm.stop_queue 
	(queue_name => 'IT271350_5.AQ$_TEST_AUDIT_DATA_QUEUE_T_E');
sys.dbms_aqadm.drop_queue 
	(queue_name => 'IT271350_5.AQ$_TEST_AUDIT_DATA_QUEUE_T_E');
END;
/

	BEGIN
	sys.dbms_aqadm.drop_queue_table 
	(queue_table => 'IT271350_5.TEST_AUDIT_DATA_QUEUE_T');
END;
/
	BEGIN
	DBMS_AQADM.CREATE_QUEUE_TABLE
	(
	QUEUE_TABLE           =>        'TEST_AUDIT_DATA_QUEUE_T'
	,QUEUE_PAYLOAD_TYPE    =>        'TEST_AUDIT_DATA_QUEUE_TYPE'
	,COMPATIBLE            =>        '8.1'
	,STORAGE_CLAUSE        =>        '
	TABLESPACE CONFIG_DATA
	PCTUSED    0
	PCTFREE    10
	INITRANS   1
	MAXTRANS   255
	STORAGE    (
	INITIAL          200K
	NEXT             200K
	MINEXTENTS       1
	MAXEXTENTS       2147483645
	PCTINCREASE      0
	BUFFER_POOL      DEFAULT
	)'
	,SORT_LIST             =>        'ENQ_TIME'
	,MULTIPLE_CONSUMERS    =>         TRUE
	,MESSAGE_GROUPING      =>         0
	,COMMENT               =>         'Queue table for database distribution'
	,SECURE                =>         FALSE
	);
End;
/
	BEGIN
	SYS.DBMS_AQADM.CREATE_QUEUE
	(
	QUEUE_NAME          =>   'TEST_AUDIT_DATA_QUEUE'
	,QUEUE_TABLE         =>   'TEST_AUDIT_DATA_QUEUE_T'
	,QUEUE_TYPE          =>   SYS.DBMS_AQADM.NORMAL_QUEUE
	,MAX_RETRIES         =>   5
	,RETRY_DELAY         =>   0
	,RETENTION_TIME      =>   0
	,COMMENT             =>   'Queue for distribution'
	);
END;
/

	BEGIN
	SYS.DBMS_AQADM.START_QUEUE
	(
	QUEUE_NAME => 'TEST_AUDIT_DATA_QUEUE'
	,ENQUEUE => TRUE 
	,DEQUEUE => TRUE 
	);
END;
/

#endif






