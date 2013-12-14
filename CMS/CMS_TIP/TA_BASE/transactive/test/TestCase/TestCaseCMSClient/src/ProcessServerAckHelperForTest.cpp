#include "ProcessServerAckHelperForTest.h"
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


CProcessServerAckHekperForTest::CProcessServerAckHekperForTest(void)
{
	BOOST_LOG_FUNCTION();

	
}

CProcessServerAckHekperForTest::~CProcessServerAckHekperForTest(void)
{
	BOOST_LOG_FUNCTION();

}

CClientWorkerForTest::ProcessedFrameRes CProcessServerAckHekperForTest::processMessage( cms::Message::Ptr pMessage )
{
	CClientWorkerForTest::ProcessedFrameRes nFunRes = CClientWorkerForTest::ProcessedFrameRes_Begin;

	int nFrameType = 0;	 

	pMessage->read(nFrameType);//not read again

	switch (nFrameType)
	{
	case FRAME_TYPE_CLIENT_SERVER_BEGIN:
		break;
	case S_TO_C_ACK_USER_LOGIN:
		nFunRes = _ProcessSTOCAckUserLogin(pMessage);
		break;
	case S_TO_C_ACK_BEGIN_TEST:
		nFunRes = _ProcessSTOCAckBeginTest(pMessage);
		break;
	case S_TO_C_ACK_DO_TEST:
		nFunRes = _ProcessSTOCAckDoTest(pMessage);
		break;
	case S_TO_C_ACK_END_TEST:
		nFunRes = _ProcessSTOCAckEndTest(pMessage);
		break;
	case S_TO_C_ACK_USER_LOGOUT:
		nFunRes = _ProcessSTOCAckUserLogOut(pMessage);
		break;
	
	case FRAME_TYPE_CLIENT_SERVER_END:
		nFunRes = CClientWorkerForTest::ProcessedFrameRes_End;
		break;
	default:
		break;
	}

	return nFunRes;
}

CClientWorkerForTest::ProcessedFrameRes  CProcessServerAckHekperForTest::_ProcessSTOCAckUserLogin(cms::Message::Ptr pMessage )
{
	CClientWorkerForTest::ProcessedFrameRes  nFunRes = CClientWorkerForTest::ProcessedFrameRes_Begin;
	cms::SessionID nSessionID = 0;
	STOCAckUserLoginFrame STOCAckUserLoginFrameTmp;
	int nCheckRes = 0;

	nSessionID = pMessage->getSessionID();
	STOCAckUserLoginFrameTmp.setDataWithMessage(pMessage);
	STOCAckUserLoginFrameTmp.logFrameInfo();
	nCheckRes = STOCAckUserLoginFrameTmp.checkValue();

	if (0 == nCheckRes)
	{
		nFunRes = CClientWorkerForTest::FrameRes_LoginAck_CheckOK;
	}
	else
	{
		nFunRes = CClientWorkerForTest::FrameRes_LoginAck_CheckError;
	}

	return nFunRes;
}



CClientWorkerForTest::ProcessedFrameRes  CProcessServerAckHekperForTest::_ProcessSTOCAckBeginTest(cms::Message::Ptr pMessage )
{
	//S_TO_C_ACK_BEGIN_TEST
	CClientWorkerForTest::ProcessedFrameRes  nFunRes = CClientWorkerForTest::ProcessedFrameRes_Begin;
	cms::SessionID nSessionID = 0;
	STOCAckBeginTestFrame STOCAckBeginTestFrameTmp;
	int nCheckRes = 0;

	nSessionID = pMessage->getSessionID();
	STOCAckBeginTestFrameTmp.setDataWithMessage(pMessage);
	STOCAckBeginTestFrameTmp.logFrameInfo();
	nCheckRes = STOCAckBeginTestFrameTmp.checkValue();

	if (0 == nCheckRes)
	{
		nFunRes = CClientWorkerForTest::FrameRes_BeginTestAck_CheckOK;
	}
	else
	{
		nFunRes = CClientWorkerForTest::FrameRes_BeginTestAck_CheckError;
	}

	return nFunRes;
}




CClientWorkerForTest::ProcessedFrameRes  CProcessServerAckHekperForTest::_ProcessSTOCAckDoTest(cms::Message::Ptr pMessage )
{
	//S_TO_C_ACK_DO_TEST
	CClientWorkerForTest::ProcessedFrameRes  nFunRes = CClientWorkerForTest::ProcessedFrameRes_Begin;
	cms::SessionID nSessionID = 0;
	STOCAckDoTestFrame STOCAckDoTestFrameTmp;
	int nCheckRes = 0;

	nSessionID = pMessage->getSessionID();
	STOCAckDoTestFrameTmp.setDataWithMessage(pMessage);
	STOCAckDoTestFrameTmp.logFrameInfo();
	nCheckRes = STOCAckDoTestFrameTmp.checkValue();

	if (0 == nCheckRes)
	{
		nFunRes = CClientWorkerForTest::FrameRes_DoTestAck_CheckOK;
	}
	else
	{
		nFunRes = CClientWorkerForTest::FrameRes_DoTestAck_CheckError;
	}

	return nFunRes;
}




CClientWorkerForTest::ProcessedFrameRes  CProcessServerAckHekperForTest::_ProcessSTOCAckEndTest(cms::Message::Ptr pMessage )
{
	//S_TO_C_ACK_END_TEST
	CClientWorkerForTest::ProcessedFrameRes  nFunRes = CClientWorkerForTest::ProcessedFrameRes_Begin;
	cms::SessionID nSessionID = 0;
	STOCAckEndTestFrame STOCAckEndTestFrameTmp;
	int nCheckRes = 0;

	nSessionID = pMessage->getSessionID();
	STOCAckEndTestFrameTmp.setDataWithMessage(pMessage);
	STOCAckEndTestFrameTmp.logFrameInfo();
	nCheckRes = STOCAckEndTestFrameTmp.checkValue();

	if (0 == nCheckRes)
	{
		nFunRes = CClientWorkerForTest::FrameRes_EndTestAck_CheckOK;
	}
	else
	{
		nFunRes = CClientWorkerForTest::FrameRes_EndTestAck_CheckError;
	}

	return nFunRes;
}





CClientWorkerForTest::ProcessedFrameRes  CProcessServerAckHekperForTest::_ProcessSTOCAckUserLogOut(cms::Message::Ptr pMessage )
{
	//S_TO_C_ACK_USER_LOGOUT
	CClientWorkerForTest::ProcessedFrameRes  nFunRes = CClientWorkerForTest::ProcessedFrameRes_Begin;
	cms::SessionID nSessionID = 0;
	STOCAckUserLogOutFrame STOCAckUserLogOutFrameTmp;
	int nCheckRes = 0;

	nSessionID = pMessage->getSessionID();
	STOCAckUserLogOutFrameTmp.setDataWithMessage(pMessage);
	STOCAckUserLogOutFrameTmp.logFrameInfo();
	nCheckRes = STOCAckUserLogOutFrameTmp.checkValue();

	if (0 == nCheckRes)
	{
		nFunRes = CClientWorkerForTest::FrameRes_LogOutAck_CheckOK;
	}
	else
	{
		nFunRes = CClientWorkerForTest::FrameRes_LogOutAck_CheckError;
	}

	return nFunRes;
}



NS_END(TA_Base_Test) 



