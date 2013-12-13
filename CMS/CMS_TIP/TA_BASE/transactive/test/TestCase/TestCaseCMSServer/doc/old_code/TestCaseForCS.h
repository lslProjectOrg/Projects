#ifndef __CLASS_TESTCASE_CLIENT_SERVER__HH__
#define __CLASS_TESTCASE_CLIENT_SERVER__HH__

#include "UtilityFun.h"



NS_BEGIN(TA_Base_Test) 


class TestCaseForCS
{
public:
	TestCaseForCS(void);
	~TestCaseForCS(void);

public:
	void runTestCase_ForServer();
	void runTestCase_ForClient();

private:
	void _InitGlobalData();
	void _InitMaxLenString();


};

NS_END(TA_Base_Test) 


#endif //__CLASS_TESTCASE_CLIENT_SERVER__HH__


