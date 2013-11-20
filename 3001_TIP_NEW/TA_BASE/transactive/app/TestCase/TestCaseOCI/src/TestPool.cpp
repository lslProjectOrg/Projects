//test.c

#include "stdio.h"
#include "SQLFunction.h"
#include "stdlib.h"
#include "TestPool.h"
#include "ConnPool.h"

CTestPool::CTestPool()
{
	m_pConnPool = new CConnPool();
	m_SQLFunction = new CSQLFunction();
}

CTestPool::~CTestPool()
{
	if (NULL != m_pConnPool)
	{
		delete m_pConnPool;
		m_pConnPool = NULL;
	}

	if (NULL != m_SQLFunction)
	{
		delete m_SQLFunction;
		m_SQLFunction = NULL;
	}
}

int CTestPool::Test_pool_main()
{
	int i,rdm,port=0;
	char szNAME[255];
	char szHOSTNAME[255];
	Connection *conn;

	*((int*)szNAME) = sizeof(szNAME);
	*((int*)szHOSTNAME) = sizeof(szHOSTNAME);

	printf("test start.\n");
	//m_pConnPool->CreateConnectionPool(10,"IT271350_5","IT271350_5","(DESCRIPTION00=(ADDRESS_LIST=(ADDRESS=(PROTOCOL=TCP)(HOST=192.168.1.109)(PORT=1522)))(CONNECT_DATA=(SID=dbgentel)(SERVER=DEDICATED)))");
	m_pConnPool->CreateConnectionPool(10,"IT271350_5","IT271350_5","TRANSACT");

	conn = m_pConnPool->GetConnection();
	//ExecateSQL(conn,"insert into phone (mid,username,passwd,ip,port,isonline) values ('005','15810721095','ssssdefeas','192.168.1.109',2511,1)");
	//if(!ExecateSQL(conn,"update phone set username = 'xxl' where mid='001'"))
	// printf("执行更新出错.\n");
	//"select mid,username from phone"
	//select NAME,HOSTNAME from recipient_info;
	m_SQLFunction->SingleQuerySQL(conn,"select NAME,HOSTNAME from recipient_info","%s%s",&szNAME,&szHOSTNAME);
	printf("mid %s\n",szNAME);
	printf("username %s\n",szHOSTNAME);
	//printf("port is %d\n",port);
	return 0;
}