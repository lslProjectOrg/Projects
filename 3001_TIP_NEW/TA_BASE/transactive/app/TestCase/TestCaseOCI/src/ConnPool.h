//ConnPool.h

#ifndef _CONNPOOL_H_
#define _CONNPOOL_H_




#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef OCI_ORACLE
#include <oci.h>
#endif

typedef struct Connection
{
	OCIEnv *environmentHandle;

	OCIError *errorHandle;
	OCIServer *serverHandle;
	OCISvcCtx *serviceHandle;

	OCISession *sessionHandle;
	OCIStmt *stmtementHandle;

	//π‹¿Ì Ù–‘
	int isNotUsing; 
	int isEnable;
	int ID;

}Connection;


#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

#define CONN_FAILED     10
#define NO_DATA      13
#define OPERATE_ERROR    12




class CConnPool
{
public:
	CConnPool();
	~CConnPool();

public:


	static int checkerr(OCIError *errhp, sword status);
	int GetLastError();

	int CreateConnectionPool(int connNum,char *userName,char *password,char *connStr);
	Connection* GetConnection();
	void PutConnection(Connection **conn);
	void SetConnDisable(Connection **conn);
	void DeletePool();
	int    ConnectinPoolisOpen();

	static int CConnPool::createOneConnection(char *username,char *password,Connection *outConn,char *connStr);
	void CConnPool::startPoolMan();
	void CConnPool::closePoolMan();
	static void* CConnPool::poolManThread(void* args);

};



#endif