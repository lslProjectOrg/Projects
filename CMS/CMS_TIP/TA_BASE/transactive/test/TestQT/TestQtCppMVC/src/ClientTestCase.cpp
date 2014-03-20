#include "ClientTestCase.h"

#include "ClientLoginParam.h"
#include "MyTradeClient.h"



#include "BoostThread.h"
#include "BoostLogger.h"
USING_BOOST_LOG;

NS_BEGIN(TA_Base_App)






CClientTestCase::CClientTestCase()
{

}

CClientTestCase::~CClientTestCase()
{

}

void CClientTestCase::test_get_exchange_info()
{
	CClientLoginParam* pClientLoginParam = NULL;
	CMyTradeClient* pMyTradeClient = NULL;

	pClientLoginParam = new CClientLoginParam();
	pClientLoginParam->setDefaultValue();
	
	pMyTradeClient = new CMyTradeClient(*pClientLoginParam);

	pMyTradeClient->logonToServer();


	while (1)
	{
		CBoostThread::sleep(10000);
	}


	if (NULL != pClientLoginParam)
	{
		delete pClientLoginParam;
		pClientLoginParam = NULL;
	}


	if (NULL != pMyTradeClient)
	{
		delete pMyTradeClient;
		pMyTradeClient = NULL;
	}

}

NS_END(TA_Base_App)












