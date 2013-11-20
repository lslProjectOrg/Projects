//ConnPool.c

#include "ConnPool.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <strings.h>
//#include <sys/select.h>

#ifndef OCI_ORACLE
#include <oci.h>
#endif


static Connection *pool = NULL;
static char p_userName[33];
static char p_password[33];
static char p_connStr[255];
static int nowPoolSize = 0;
static int lastError = 0;
static int open = 0;




CConnPool::CConnPool()
{

}
CConnPool::~CConnPool()
{

}

int CConnPool::CreateConnectionPool(int connNum,char *userName,char *password,char *connStr)
{
	int minNum,maxNum;
	int i,ret=FALSE,connSize;
	if(0>=connNum)
		return FALSE;
	minNum = connNum;
	maxNum = connNum;
	connSize = sizeof(Connection) * minNum;
	pool = (Connection *)malloc(connSize);
	strcpy(p_userName,userName);
	strcpy(p_password,password);
	if(connStr!=NULL)
		strcpy(p_connStr,connStr);
	else strcpy(p_connStr,"");
	if(!pool)return FALSE;
	nowPoolSize = minNum;
	//bzero((void*)pool,(size_t)connSize);
	memset(pool, 0, connSize);
	for(i=0;i<minNum;i++)
	{
		int tmp = createOneConnection(userName,password,&pool[i],connStr);
		ret |= tmp;
		if(!tmp)
		{
			printf("%d connection create faild.\n",i+1);
		}
		else
		{
			printf("%d connection create success.\n",i+1);
		}
		pool[i].ID = i+1;
	}
	if(ret)open = 1;
	if(ret)startPoolMan();
	return ret;
}

int CConnPool::createOneConnection(char *username,char *password,Connection *outConn,char *connStr)
{
	if (OCIEnvCreate((OCIEnv **) &outConn->environmentHandle,
		(ub4)OCI_THREADED|OCI_OBJECT, (dvoid *)0,
		(dvoid * (*)(dvoid *, size_t)) 0,
		(dvoid * (*)(dvoid *, dvoid *, size_t))0,
		(void (*)(dvoid *, dvoid *)) 0,
		(size_t) 0, (dvoid **) 0 ))
	{
		printf("FAILED: OCIEnvCreate()\n");
		return FALSE;
	}

	if (OCIHandleAlloc((dvoid *) outConn->environmentHandle, (dvoid **) &outConn->errorHandle,
		(ub4) OCI_HTYPE_ERROR, (size_t) 0, (dvoid **) 0))
	{
		printf("FAILED: OCIHandleAlloc() on ctxptr->errhp\n");
		return FALSE;
	}

	if (OCIHandleAlloc((dvoid *) outConn->environmentHandle, (dvoid **) &outConn->serverHandle,
		(ub4) OCI_HTYPE_SERVER, (size_t) 0, (dvoid **) 0))
	{
		printf("FAILED: OCIHandleAlloc() on ctxptr->srvhp\n");
		return FALSE;
	}

	if (OCIHandleAlloc((dvoid *) outConn->environmentHandle, (dvoid **) &outConn->serviceHandle,
		(ub4) OCI_HTYPE_SVCCTX, (size_t) 0, (dvoid **) 0))
	{
		printf("FAILED: OCIHandleAlloc() on ctxptr->svchp\n");
		return FALSE;
	}


	if (OCIHandleAlloc((dvoid *) outConn->environmentHandle, (dvoid **) &outConn->sessionHandle,
		(ub4) OCI_HTYPE_SESSION, (size_t) 0, (dvoid **) 0))
	{
		printf("FAILED: OCIHandleAlloc() on ctxptr->authp\n"); 
		return FALSE;
	}

	//////////////////////////////////////////////////////////////////////////////////
	if(connStr!=NULL)
	{
		if (OCIServerAttach(outConn->serverHandle, outConn->errorHandle, (text *) connStr,
			(sb4) strlen((char *) connStr), (ub4) OCI_DEFAULT))
		{
			printf("FAILED: OCIServerAttach()\n");
			return FALSE;
		}
	}
	else
	{
		if (OCIServerAttach(outConn->serverHandle, outConn->errorHandle, (text *) "",
			(sb4) strlen((char *) ""), (ub4) OCI_DEFAULT))
		{
			printf("FAILED: OCIServerAttach()\n");
			return FALSE;
		}
	}

	if (OCIAttrSet((dvoid *) outConn->serviceHandle, (ub4) OCI_HTYPE_SVCCTX,
		(dvoid *) outConn->serverHandle, (ub4) 0, (ub4) OCI_ATTR_SERVER,
		outConn->errorHandle))
	{
		printf("FAILED: OCIAttrSet() server attribute\n");
		return FALSE;
	}

	/*begin log_on part */

	if (OCIAttrSet((dvoid *) outConn->sessionHandle, (ub4) OCI_HTYPE_SESSION,
		(dvoid *) username, (ub4) strlen((char *) username),
		(ub4) OCI_ATTR_USERNAME, outConn->errorHandle))
	{
		printf("FAILED: OCIAttrSet() userid\n"); 
		return FALSE;
	}

	if (OCIAttrSet((dvoid *) outConn->sessionHandle, (ub4) OCI_HTYPE_SESSION,
		(dvoid *) password, (ub4) strlen((char *) password),
		(ub4) OCI_ATTR_PASSWORD, outConn->errorHandle))
	{
		printf("FAILED: OCIAttrSet() passwd\n");                                
		return FALSE;
	}

	if(!checkerr(outConn->errorHandle, OCISessionBegin((OCISvcCtx *)outConn->serviceHandle,
		outConn->errorHandle,outConn->sessionHandle, 
		(ub4) OCI_CRED_RDBMS,(ub4) OCI_DEFAULT )))
	{   
		printf("FAILED: OCISessionBegin()\n");
		return FALSE;
	}

	if (OCIAttrSet((dvoid *) outConn->serviceHandle, (ub4) OCI_HTYPE_SVCCTX,   
		(dvoid *) outConn->sessionHandle, (ub4) 0, (ub4) OCI_ATTR_SESSION,
		outConn->errorHandle))
	{
		printf("FAILED: OCIAttrSet() session\n");
		return FALSE;
	}

	/* end log_on part */

	/* alocate stmt handle for sql queries */    
	if (OCIHandleAlloc((dvoid *)outConn->environmentHandle, (dvoid **) &outConn->stmtementHandle,
		(ub4)OCI_HTYPE_STMT, (CONST size_t) 0, (dvoid **) 0))
	{
		printf("FAILED: alloc statement handle\n");
		return FALSE;
	}

	outConn->isEnable = TRUE;
	outConn->isNotUsing = TRUE;

	return TRUE;
}


int CConnPool::checkerr(OCIError *errhp,sword status )
{
	text errbuf[512];
	sb4 errcode = 0;

	switch (status)
	{
	case OCI_SUCCESS:
		return TRUE;
		break;
	case OCI_NO_DATA:
		lastError = NO_DATA;
		break;
	case OCI_SUCCESS_WITH_INFO:
		lastError = OPERATE_ERROR;
		printf("Error - OCI_SUCCESS_WITH_INFO\n");
		break;
	case OCI_NEED_DATA:
		lastError = OPERATE_ERROR;
		printf("Error - OCI_NEED_DATA\n");
		break;
	case OCI_ERROR:
		lastError = OPERATE_ERROR;
		OCIErrorGet((dvoid *)errhp, (ub4) 1, (text *) NULL, &errcode,
			errbuf, (ub4) sizeof(errbuf), OCI_HTYPE_ERROR);
		printf("Error - %.*s\n", 512, errbuf);
		break;
	case OCI_STILL_EXECUTING:
		lastError = OPERATE_ERROR;
		printf("Error - OCI_STILL_EXECUTE\n");
		break;
	case OCI_INVALID_HANDLE:
		lastError = CONN_FAILED;
		printf("Error - OCI_INVALID_HANDLE\n");
		break;
	case OCI_CONTINUE:
		lastError = CONN_FAILED;
		printf("Error - OCI_CONTINUE\n");
		break;
	default:
		break;
	}
	return FALSE;
} /* end checkerr() */


int CConnPool::GetLastError()
{
	return lastError;
}


Connection* CConnPool::GetConnection()
{
	int i;
	for(i=0;i<nowPoolSize;i++)
	{
		if(pool[i].isEnable && pool[i].isNotUsing)
		{
			pool[i].isNotUsing = FALSE;
			return &pool[i];
		}
	}
	return NULL;
}
void CConnPool::PutConnection(Connection **conn)
{
	(*conn)->isNotUsing = TRUE;
	(*conn) = NULL;
}
void CConnPool::SetConnDisable(Connection **conn)
{
	(*conn)->isEnable = FALSE;
	(*conn) = NULL;
}
void CConnPool::DeletePool()
{
	int i;

	open = 0;
	closePoolMan();
	for(i=0;i<nowPoolSize;i++)
	{
		pool[i].isEnable = FALSE;

		if (pool[i].stmtementHandle)
			OCIHandleFree((dvoid *) pool[i].stmtementHandle, (ub4) OCI_HTYPE_STMT);
		if (pool[i].errorHandle )
			OCIHandleFree((dvoid *) pool[i].errorHandle, (ub4) OCI_HTYPE_ERROR);
		if (pool[i].serverHandle)
			OCIHandleFree((dvoid *) pool[i].serverHandle, (ub4) OCI_HTYPE_SERVER);
		if (pool[i].serviceHandle)
			OCIHandleFree((dvoid *) pool[i].serviceHandle, (ub4) OCI_HTYPE_SVCCTX);
		if (pool[i].sessionHandle)
			OCIHandleFree((dvoid *) pool[i].sessionHandle, (ub4) OCI_HTYPE_SESSION);
		if (pool[i].environmentHandle)
			OCIHandleFree((dvoid *) pool[i].environmentHandle, (ub4) OCI_HTYPE_ENV);
	}
	free(pool);
	pool = NULL;
	nowPoolSize = 0;
}
int    CConnPool::ConnectinPoolisOpen()
{
	return open;
}


/////////////////////////////////////////////////////
void CConnPool::startPoolMan()
{
	int ret;
	pthread_t id;
	ret=pthread_create(&id, NULL, poolManThread, NULL);
	if(ret!=0){
		printf ("Create PoolMan thread error!\n");
	}
}
void CConnPool::closePoolMan()
{
}
void* CConnPool::poolManThread(void* args)
{
	int i;

	struct timespec delayVal; 

	long  nSleepMilliseconds = 0; 
	long  nSleepMicroseconds = 0;
	long  nSleepNanoseconds = 0; 
	long  nSleepSeconds = 0;

	nSleepSeconds =  1 / 1000;

	nSleepMilliseconds = 500 % 1000;
	nSleepMicroseconds = nSleepMilliseconds * 1000;
	nSleepNanoseconds = nSleepMicroseconds * 1000;

	delayVal.tv_sec = nSleepSeconds;
	delayVal.tv_nsec = nSleepNanoseconds;	




	printf("Man start.\n");
	while(open)
	{
		//select(0,NULL, NULL, NULL, &tv); 
		//printf("check once.\n");
		pthread_delay_np (&delayVal);


		for(i=0;i<nowPoolSize;i++)
		{
			if(!pool[i].isEnable && open)
			{
				if (pool[i].stmtementHandle)
					OCIHandleFree((dvoid *) pool[i].stmtementHandle, (ub4) OCI_HTYPE_STMT);
				if (pool[i].errorHandle )
					OCIHandleFree((dvoid *) pool[i].errorHandle, (ub4) OCI_HTYPE_ERROR);
				if (pool[i].serverHandle)
					OCIHandleFree((dvoid *) pool[i].serverHandle, (ub4) OCI_HTYPE_SERVER);
				if (pool[i].serviceHandle)
					OCIHandleFree((dvoid *) pool[i].serviceHandle, (ub4) OCI_HTYPE_SVCCTX);
				if (pool[i].sessionHandle)
					OCIHandleFree((dvoid *) pool[i].sessionHandle, (ub4) OCI_HTYPE_SESSION);
				if (pool[i].environmentHandle)
					OCIHandleFree((dvoid *) pool[i].environmentHandle, (ub4) OCI_HTYPE_ENV);
				if(createOneConnection(p_userName,p_password,&pool[i],p_connStr))
				{
					pool[i].isEnable = TRUE;
					pool[i].isNotUsing = TRUE;
				}
			}
		}
	}

	return NULL;
}