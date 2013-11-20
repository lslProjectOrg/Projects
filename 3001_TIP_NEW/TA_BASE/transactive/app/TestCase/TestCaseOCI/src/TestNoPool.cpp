#include <oci.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include "TestNoPool.h"

#define MAXTHREAD 2

static OCIError   *errhp;
static OCIEnv     *envhp;
static OCICPool   *poolhp;

static int employeeNum[MAXTHREAD];

static OraText *poolName;
static sb4 poolNameLen;
static text *database = (text *)"TRANSACT";
static text *username =(text *)"IT271350_5";
static text *password =(text *)"IT271350_5";
static text *appusername =(text *)"IT271350_5";
static text *apppassword =(text *)"IT271350_5";

static ub4 conMin = 5;
static ub4 conMax = 15;
static ub4 conIncr = 1;

// static void checkerr (OCIError *errhp, sword status);
// static void threadFunction (dvoid *arg);
// static int test_noPool_main (void);

CTestNoPool::CTestNoPool()
{

}
CTestNoPool::~CTestNoPool()
{

}

int CTestNoPool::test_noPool_main (void)
{
	int i = 0;

	//OCIEnvCreate (&envhp, OCI_THREADED, (dvoid *)0,  (dvoid * (*)()) 0, (dvoid * (*)()) 0, (dvoid (*)()) 0, 0, (dvoid *)0);
	OCIEnvCreate (&envhp, OCI_THREADED, 0,  0, 0, 0, 0, 0);

	(void) OCIHandleAlloc((dvoid *) envhp, (dvoid **) &errhp, OCI_HTYPE_ERROR,
		(size_t) 0, (dvoid **) 0);


	(void) OCIHandleAlloc((dvoid *) envhp, (dvoid **) &poolhp, OCI_HTYPE_CPOOL,
		(size_t) 0, (dvoid **) 0);

	/* CREATE THE CONNECTION POOL */
	checkerr (errhp, OCIConnectionPoolCreate(envhp, 
		errhp,poolhp, &poolName, &poolNameLen,
		database, strlen((const char*)database),
		conMin, conMax, conIncr,
		appusername,strlen((const char*)appusername),
		apppassword,strlen((const char*)apppassword),OCI_DEFAULT));

	/* Multiple threads using the connection pool */
	{
		OCIThreadId     *thrid[MAXTHREAD];
		OCIThreadHandle *thrhp[MAXTHREAD];

		OCIThreadProcessInit ();
		checkerr (errhp, OCIThreadInit (envhp, errhp));
		for (i = 0; i < MAXTHREAD; ++i)
		{
			checkerr (errhp, OCIThreadIdInit (envhp, errhp, &thrid[i]));
			checkerr (errhp, OCIThreadHndInit (envhp, errhp, &thrhp[i]));
		}
		for (i = 0; i < MAXTHREAD; ++i)
		{
			employeeNum[i]=i;
			checkerr (errhp, OCIThreadCreate (envhp, errhp, threadFunction, 
				(dvoid *) &employeeNum[i], thrid[i], thrhp[i]));
		}
		for (i = 0; i < MAXTHREAD; ++i)
		{
			checkerr (errhp, OCIThreadJoin (envhp, errhp, thrhp[i]));
			checkerr (errhp, OCIThreadClose (envhp, errhp, thrhp[i]));
			checkerr (errhp, OCIThreadIdDestroy (envhp, errhp, &(thrid[i])));
			checkerr (errhp, OCIThreadHndDestroy (envhp, errhp, &(thrhp[i])));
		}
		checkerr (errhp, OCIThreadTerm (envhp, errhp));
	} /* ALL THE THREADS ARE COMPLETE */

	checkerr(errhp, OCIConnectionPoolDestroy(poolhp, errhp, OCI_DEFAULT));
	checkerr(errhp, OCIHandleFree((dvoid *)poolhp, OCI_HTYPE_CPOOL));
	checkerr(errhp, OCIHandleFree((dvoid *)errhp, OCI_HTYPE_ERROR));

	return 0;
} /* end of main () */

void CTestNoPool::threadFunction (dvoid *arg)
{ 
	int empno = *(int *)arg;
	OCISvcCtx *svchp = (OCISvcCtx *) arg;
	char insertst1[512];
	char delete1[256];

	OCIStmt *stmthp = (OCIStmt *)0;

	checkerr(errhp,OCILogon2(envhp, errhp, &svchp, 
		(CONST OraText *)username, strlen((const char*)username), 
		(CONST OraText *)password, strlen((const char*)password), 
		(CONST OraText *)poolName, poolNameLen,
		OCI_CPOOL));

	//insert
	sprintf((char*)insertst1,
		"insert into AQ_ENQ_DUPLICATE_CHECK(LOCATION, SRV_ID, Q_NAME, MSG_SID) values(UPPER('%s'), %s, UPPER('%s'), %s)",
		"LOCATION", "1", "Q_NAME", "1000000");

	OCIHandleAlloc(envhp, (dvoid **)&stmthp, OCI_HTYPE_STMT, (size_t)0,	(dvoid **)0);

	checkerr(errhp, OCIStmtPrepare (stmthp, errhp, (text *)insertst1,
		(ub4)strlen((const char*)insertst1), OCI_NTV_SYNTAX, OCI_DEFAULT));

	checkerr(errhp, OCIStmtExecute (svchp, stmthp, errhp, (ub4)1, (ub4)0,
		(OCISnapshot *)0, (OCISnapshot *)0, OCI_DEFAULT ));

	checkerr(errhp, OCITransCommit(svchp,errhp,(ub4)0));

	checkerr(errhp, OCIHandleFree((dvoid *) stmthp, OCI_HTYPE_STMT));


	//delete
	sprintf((char*)delete1,
		"delete from AQ_ENQ_DUPLICATE_CHECK where UPPER(LOCATION)=UPPER('%s')",
		"LOCATION");

	OCIHandleAlloc(envhp, (dvoid **)&stmthp, OCI_HTYPE_STMT, (size_t)0,	(dvoid **)0);

	checkerr(errhp, OCIStmtPrepare (stmthp, errhp, (text *)delete1,
		(ub4)strlen((const char*)delete1), OCI_NTV_SYNTAX, OCI_DEFAULT));

	checkerr(errhp, OCIStmtExecute (svchp, stmthp, errhp, (ub4)1, (ub4)0,
		(OCISnapshot *)0, (OCISnapshot *)0, OCI_DEFAULT ));

	checkerr(errhp, OCITransCommit(svchp,errhp,(ub4)0));

	checkerr(errhp, OCIHandleFree((dvoid *) stmthp, OCI_HTYPE_STMT));

	//logoff
	checkerr(errhp, OCILogoff((OCISvcCtx *) svchp, errhp));
} /* end of threadFunction (dvoid *) */

void CTestNoPool::checkerr(OCIError *errhp, sword status)
{
	text errbuf[512];
	sb4 errcode = 0;

	switch (status)
	{
	case OCI_SUCCESS:
		break;
	case OCI_SUCCESS_WITH_INFO:
		(void) printf("Error - OCI_SUCCESS_WITH_INFO\n");
		break;
	case OCI_NEED_DATA:
		(void) printf("Error - OCI_NEED_DATA\n");
		break;
	case OCI_NO_DATA:
		(void) printf("Error - OCI_NODATA\n");
		break;
	case OCI_ERROR:
		(void) OCIErrorGet((dvoid *)errhp, (ub4) 1, (text *) NULL, &errcode,
			errbuf, (ub4) sizeof(errbuf), OCI_HTYPE_ERROR);
		(void) printf("Error - %.*s\n", 512, errbuf);
		break;
	case OCI_INVALID_HANDLE:
		(void) printf("Error - OCI_INVALID_HANDLE\n");
		break;
	case OCI_STILL_EXECUTING:
		(void) printf("Error - OCI_STILL_EXECUTE\n");
		break;
	case OCI_CONTINUE:
		(void) printf("Error - OCI_CONTINUE\n");
		break;
	default:
		break;
	}
}