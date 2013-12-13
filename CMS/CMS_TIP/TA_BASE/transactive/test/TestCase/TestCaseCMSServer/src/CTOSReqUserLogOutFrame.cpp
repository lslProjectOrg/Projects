#include "CTOSReqUserLogOutFrame.h"


#include "BoostLogger.h"
USING_BOOST_LOG;


NS_BEGIN(TA_Base_Test) 




CTOSReqUserLogOutFrame::CTOSReqUserLogOutFrame(void)
{
	BOOST_LOG_FUNCTION();

	m_nFrameType_SampleValue = C_TO_S_REQ_USER_LOGOUT;
	m_nClientType_SampleValue = ClientIdentity_TestClient;
	m_strUserName_SampleValue = "m_strUserName";
	m_strPwd_SampleValue = "m_strPwd";
	m_strReqInfo_SampleValue = "I want to logOut!";

	setSampleValue();
}

CTOSReqUserLogOutFrame::~CTOSReqUserLogOutFrame(void)
{
	BOOST_LOG_FUNCTION();

}


int CTOSReqUserLogOutFrame::setSampleValue()
{
	int nFunRes = 0;

	m_nFrameType = m_nFrameType_SampleValue;
	m_nClientType = m_nClientType_SampleValue;
	m_strUserName = m_strUserName_SampleValue;
	m_strPwd = m_strPwd_SampleValue;
	m_strReqInfo = m_strReqInfo_SampleValue;

	return nFunRes;
}


int CTOSReqUserLogOutFrame::setDataWithMessage(cms::Message::Ptr pMessage)
{
	int nFunRes = 0;

	{
		m_nFrameType = m_nFrameType_SampleValue;

		pMessage->read(m_nClientType);
		pMessage->read(m_strUserName);

		pMessage->read(m_strPwd);
		pMessage->read(m_strReqInfo);
	}

	return nFunRes;
}


int CTOSReqUserLogOutFrame::checkValue()
{
	int nFunRes = 0;

	if (m_nFrameType_SampleValue != m_nFrameType)
	{
		LOG_ERROR<<"checkValue Error!";
		nFunRes = -1;
		return nFunRes;
	}

	if (m_nClientType_SampleValue != m_nClientType)
	{
		LOG_ERROR<<"checkValue Error!";
		nFunRes = -1;
		return nFunRes;
	}

	if (m_strUserName_SampleValue != m_strUserName)
	{
		LOG_ERROR<<"checkValue Error!";
		nFunRes = -1;
		return nFunRes;
	}

	if (m_strPwd_SampleValue != m_strPwd)
	{
		LOG_ERROR<<"checkValue Error!";
		nFunRes = -1;
		return nFunRes;
	}

	if (m_strReqInfo_SampleValue != m_strReqInfo)
	{
		LOG_ERROR<<"checkValue Error!";
		nFunRes = -1;
		return nFunRes;
	}

	return nFunRes;
}


cms::Message::Ptr CTOSReqUserLogOutFrame::getMessage()
{
	cms::Message::Ptr pMessage(new cms::Message());
	pMessage->write(m_nFrameType);
	pMessage->write(m_nClientType);
	pMessage->write(m_strUserName);
	pMessage->write(m_strPwd);
	pMessage->write(m_strReqInfo);
	return pMessage;
}

NS_END(TA_Base_Test) 



