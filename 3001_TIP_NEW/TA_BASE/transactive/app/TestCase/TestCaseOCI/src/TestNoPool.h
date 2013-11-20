#ifndef __CLASS_TESTNOPOOL__HH__
#define __CLASS_TESTNOPOOL__HH__


#include <oci.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

class CTestNoPool
{
public:
	CTestNoPool();
	~CTestNoPool();
public:
	static void checkerr (OCIError *errhp, sword status);
	static void threadFunction (dvoid *arg);
	int CTestNoPool::test_noPool_main (void);

};

#endif //__CLASS_TESTNOPOOL__HH__