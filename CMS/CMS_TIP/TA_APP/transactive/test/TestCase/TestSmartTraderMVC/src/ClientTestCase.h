#ifndef __CLASS_CLIENT_TEST_CASE_HH__
#define __CLASS_CLIENT_TEST_CASE_HH__

#include "ProjectCommonData.h"


NS_BEGIN(TA_App_App)

class CClientTestCase 
{ 
public: 
	CClientTestCase(); 
	virtual ~CClientTestCase(); 
public:
	void test_get_exchange_info();
	void test_TreeModel();
	void test_TreeModelExchange();

}; 



NS_END(TA_App_App)




#endif//__CLASS_CLIENT_TEST_CASE_HH__



