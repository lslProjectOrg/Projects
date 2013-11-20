#ifndef __CLASS_TEST_POOL__HH__
#define __CLASS_TEST_POOL__HH__

#include "ConnPool.h"
#include "SQLFunction.h"

class CTestPool
{
public:
	CTestPool();
	~CTestPool();

public:
	int CTestPool::Test_pool_main();


private:
	CConnPool* m_pConnPool;
	CSQLFunction* m_SQLFunction;
};


#endif//__CLASS_TEST_POOL__HH__
