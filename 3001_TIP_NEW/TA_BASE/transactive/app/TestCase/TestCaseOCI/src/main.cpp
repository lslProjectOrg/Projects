//#include "vld.h"
#include <iostream>

#include "TestNoPool.h"
#include "TestPool.h"


#include "ConnPool.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int main()
{
	/*debug working dir "C:\mytestpro7\DaiTestGetSetValuePro\bin"*/
	

	/*
	CTestNoPool* pTestNoPool = NULL;
	pTestNoPool = new CTestNoPool();
	pTestNoPool->test_noPool_main();

	if (NULL != pTestNoPool)
	{
		delete pTestNoPool;
		pTestNoPool = NULL;
	}
	*/

	CTestPool* pTestPool = NULL;
	pTestPool = new CTestPool();
	pTestPool->Test_pool_main();


	struct timespec delayVal; 

	long  nSleepMilliseconds = 0; 
	long  nSleepMicroseconds = 0;
	long  nSleepNanoseconds = 0; 
	long  nSleepSeconds = 0;

	nSleepSeconds =  5 / 1000;

	nSleepMilliseconds = 1500 % 1000;
	nSleepMicroseconds = nSleepMilliseconds * 1000;
	nSleepNanoseconds = nSleepMicroseconds * 1000;

	delayVal.tv_sec = nSleepSeconds;
	delayVal.tv_nsec = nSleepNanoseconds;	




	while(1)
	{
		//select(0,NULL, NULL, NULL, &tv); 
		//printf("check once.\n");
		pthread_delay_np (&delayVal);
	}








	if (NULL != pTestPool)
	{
		delete pTestPool;
		pTestPool = NULL;
	}

	
	return 0;
}

























