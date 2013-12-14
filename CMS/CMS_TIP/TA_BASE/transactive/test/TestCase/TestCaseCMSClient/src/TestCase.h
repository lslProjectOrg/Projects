#ifndef __CLASS_TESTCASE_HH__
#define __CLASS_TESTCASE_HH__

#include "UtilityFun.h"



NS_BEGIN(TA_Base_Test) 


class CTestCase
{
public:
	CTestCase(void);
	~CTestCase(void);

public:
	void runTestCase_ForServer();
	void runTestCase_ForClient();




};

NS_END(TA_Base_Test) 


#endif //__CLASS_TESTCASE_CLIENT_SERVER__HH__


