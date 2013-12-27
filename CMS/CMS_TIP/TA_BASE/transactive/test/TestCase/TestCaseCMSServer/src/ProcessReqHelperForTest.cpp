#include "ProcessReqHelperForTest.h"
#include "ServerManage.h"
#include "Message.h"
#include "MessageBroker.h"

#include "CTOSReqUserLoginFrame.h"
#include "STOCAckUserLoginFrame.h"

#include "BoostLogger.h"
USING_BOOST_LOG;


NS_BEGIN(TA_Base_Test) 


CProcessReqHelperForTest::CProcessReqHelperForTest(void)
{
	BOOST_LOG_FUNCTION();

	
}

CProcessReqHelperForTest::~CProcessReqHelperForTest(void)
{
	BOOST_LOG_FUNCTION();

}

int CProcessReqHelperForTest::processMessage( cms::Message::Ptr pMessage )
{
	int nFunRes = 0;

	int nFrameType = 0;

	pMessage->read(nFrameType);//not read again


	switch (nFrameType)
	{
	case FRAME_TYPE_CLIENT_SERVER_BEGIN:
		break;
	case C_TO_S_REQ_USER_LOGIN:
		nFunRes = _ProcessCTOSReqUserLogin(pMessage);
		break;
	case C_TO_S_REQ_BEGIN_TEST:
		nFunRes = _ProcessCTOSReqBeginTest(pMessage);
		break;
	case C_TO_S_REQ_DO_TEST:
		nFunRes = _ProcessCTOSReqDoTest(pMessage);
		break;
	case C_TO_S_REQ_END_TEST:
		nFunRes = _ProcessCTOSReqEndTest(pMessage);
		break;
	case C_TO_S_REQ_USER_LOGOUT:
		nFunRes = _ProcessCTOSReqUserLogout(pMessage);
		break;
	case FRAME_TYPE_CLIENT_SERVER_END:
		break;
	default:
		break;
	}


	return nFunRes;
}

int CProcessReqHelperForTest::_ProcessCTOSReqUserLogin(cms::Message::Ptr pMessage )
{
	int nFunRes = 0;

	cms::SessionID nSessionID = pMessage->getSessionID();
	CTOSReqUserLoginFrame CTOSReqUserLoginFrameTmp;
	CTOSReqUserLoginFrameTmp.setDataWithMessage(pMessage);
	nFunRes = CTOSReqUserLoginFrameTmp.checkValue();


	cms::Message::Ptr pMessageSend; 

	if (0 == nFunRes)
	{
		CServerManager::getInstance().sendData(nSessionID, pMessageSend);
	}

	return nFunRes;
}

int CProcessReqHelperForTest::_ProcessCTOSReqBeginTest(cms::Message::Ptr pMessage )
{
	int nFunRes = 0;
	return nFunRes;
}

int CProcessReqHelperForTest::_ProcessCTOSReqDoTest(cms::Message::Ptr pMessage )
{
	int nFunRes = 0;
	return nFunRes;
}

int CProcessReqHelperForTest::_ProcessCTOSReqEndTest(cms::Message::Ptr pMessage )
{
	int nFunRes = 0;
	return nFunRes;
}

int CProcessReqHelperForTest::_ProcessCTOSReqUserLogout(cms::Message::Ptr pMessage )
{
	int nFunRes = 0;
	return nFunRes;
}




NS_END(TA_Base_Test) 



