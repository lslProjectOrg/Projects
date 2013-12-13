#include "ProcessClientReqHelperForTest.h"
#include "ServerManage.h"
#include "Message.h"
#include "MessageBroker.h"

#include "CTOSReqUserLoginFrame.h"
#include "STOCAckUserLoginFrame.h"
#include "CTOSReqBeginTestFrame.h"
#include "STOCAckBeginTestFrame.h"
#include "CTOSReqDoTestFrame.h"
#include "STOCAckDoTestFrame.h"
#include "CTOSReqEndTestFrame.h"
#include "STOCAckEndTestFrame.h"
#include "CTOSReqUserLogOutFrame.h"
#include "STOCAckUserLogOutFrame.h"

#include "BoostLogger.h"
USING_BOOST_LOG;


NS_BEGIN(TA_Base_Test) 


CProcessClientReqHelperForTest::CProcessClientReqHelperForTest(void)
{
	BOOST_LOG_FUNCTION();


}

CProcessClientReqHelperForTest::~CProcessClientReqHelperForTest(void)
{
	BOOST_LOG_FUNCTION();

}

int CProcessClientReqHelperForTest::processMessage( cms::Message::Ptr pMessage )
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

int CProcessClientReqHelperForTest::_ProcessCTOSReqUserLogin(cms::Message::Ptr pMessage )
{
	int nFunRes = 0;
	cms::SessionID nSessionID = 0;
	CTOSReqUserLoginFrame CTOSReqUserLoginFrameTmp;
	STOCAckUserLoginFrame STOCAckUserLoginFrameTmp;

	nSessionID = pMessage->getSessionID();
	CTOSReqUserLoginFrameTmp.setDataWithMessage(pMessage);
	nFunRes = CTOSReqUserLoginFrameTmp.checkValue();

	STOCAckUserLoginFrameTmp.setSampleValue();
	cms::Message::Ptr pMessageSend = STOCAckUserLoginFrameTmp.getMessage();

	if (0 == nFunRes)
	{
		CServerManager::getInstance().sendData(nSessionID, pMessageSend);
	}

	return nFunRes;
}

int CProcessClientReqHelperForTest::_ProcessCTOSReqBeginTest(cms::Message::Ptr pMessage )
{
	int nFunRes = 0;
	cms::SessionID nSessionID = 0;
	CTOSReqBeginTestFrame CTOSReqBeginTestFrameTmp;
	STOCAckBeginTestFrame STOCAckBeginTestFrameTmp;

	nSessionID = pMessage->getSessionID();
	CTOSReqBeginTestFrameTmp.setDataWithMessage(pMessage);
	nFunRes = CTOSReqBeginTestFrameTmp.checkValue();

	STOCAckBeginTestFrameTmp.setSampleValue();
	cms::Message::Ptr pMessageSend = STOCAckBeginTestFrameTmp.getMessage();

	if (0 == nFunRes)
	{
		CServerManager::getInstance().sendData(nSessionID, pMessageSend);
	}

	return nFunRes;
}

int CProcessClientReqHelperForTest::_ProcessCTOSReqDoTest(cms::Message::Ptr pMessage )
{
	int nFunRes = 0;
	cms::SessionID nSessionID = 0;
	CTOSReqDoTestFrame CTOSReqDoTestFrameTmp;
	STOCAckDoTestFrame STOCAckDoTestFrameTmp;

	nSessionID = pMessage->getSessionID();
	CTOSReqDoTestFrameTmp.setDataWithMessage(pMessage);
	nFunRes = CTOSReqDoTestFrameTmp.checkValue();

	STOCAckDoTestFrameTmp.setSampleValue();
	cms::Message::Ptr pMessageSend = STOCAckDoTestFrameTmp.getMessage();

	if (0 == nFunRes)
	{
		CServerManager::getInstance().sendData(nSessionID, pMessageSend);
	}

	return nFunRes;
}

int CProcessClientReqHelperForTest::_ProcessCTOSReqEndTest(cms::Message::Ptr pMessage )
{
	int nFunRes = 0;
	cms::SessionID nSessionID = 0;
	CTOSReqEndTestFrame CTOSReqEndTestFrameTmp;
	STOCAckEndTestFrame STOCAckEndTestFrameTmp;

	nSessionID = pMessage->getSessionID();
	CTOSReqEndTestFrameTmp.setDataWithMessage(pMessage);
	nFunRes = CTOSReqEndTestFrameTmp.checkValue();

	STOCAckEndTestFrameTmp.setSampleValue();
	cms::Message::Ptr pMessageSend = STOCAckEndTestFrameTmp.getMessage();

	if (0 == nFunRes)
	{
		CServerManager::getInstance().sendData(nSessionID, pMessageSend);
	}

	return nFunRes;
}

int CProcessClientReqHelperForTest::_ProcessCTOSReqUserLogout(cms::Message::Ptr pMessage )
{
	int nFunRes = 0;
	cms::SessionID nSessionID = 0;

	CTOSReqUserLogOutFrame CTOSReqUserLogOutFrameTmp;
	STOCAckUserLogOutFrame STOCAckUserLogOutFrameTmp;

	nSessionID = pMessage->getSessionID();
	CTOSReqUserLogOutFrameTmp.setDataWithMessage(pMessage);
	nFunRes = CTOSReqUserLogOutFrameTmp.checkValue();

	STOCAckUserLogOutFrameTmp.setSampleValue();
	cms::Message::Ptr pMessageSend = STOCAckUserLogOutFrameTmp.getMessage();

	if (0 == nFunRes)
	{
		CServerManager::getInstance().sendData(nSessionID, pMessageSend);
	}

	return nFunRes;
}




NS_END(TA_Base_Test) 



