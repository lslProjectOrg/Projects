#include "STOCAckUserLogOutFrame.h"


#include "BoostLogger.h"
USING_BOOST_LOG;


NS_BEGIN(TA_Base_Test) 




STOCAckUserLogOutFrame::STOCAckUserLogOutFrame(void)
{
	BOOST_LOG_FUNCTION();

	m_nFrameType_SampleValue = S_TO_C_ACK_USER_LOGOUT;
	m_nClientType_SampleValue = ClientIdentity_TestClient;
	m_strUserName_SampleValue = "m_strUserName";
	m_strPwd_SampleValue = "m_strPwd";
	m_strAckInfo_SampleValue = "you can to logOut Now!";

	setSampleValue();
}

STOCAckUserLogOutFrame::~STOCAckUserLogOutFrame(void)
{
	BOOST_LOG_FUNCTION();

}


int STOCAckUserLogOutFrame::setSampleValue()
{
	int nFunRes = 0;

	m_nFrameType = m_nFrameType_SampleValue;
	m_nClientType = m_nClientType_SampleValue;
	m_strUserName = m_strUserName_SampleValue;
	m_strPwd = m_strPwd_SampleValue;
	m_strAckInfo = m_strAckInfo_SampleValue;

	return nFunRes;
}


int STOCAckUserLogOutFrame::setDataWithMessage(cms::Message::Ptr pMessage)
{
	int nFunRes = 0;

	{
		m_nFrameType = m_nFrameType_SampleValue;

		pMessage->read(m_nClientType);
		pMessage->read(m_strUserName);

		pMessage->read(m_strPwd);
		pMessage->read(m_strAckInfo);
	}

	return nFunRes;
}


int STOCAckUserLogOutFrame::checkValue()
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

	if (m_strAckInfo_SampleValue != m_strAckInfo)
	{
		LOG_ERROR<<"checkValue Error!";
		nFunRes = -1;
		return nFunRes;
	}

	return nFunRes;
}


cms::Message::Ptr STOCAckUserLogOutFrame::getMessage()
{
	cms::Message::Ptr pMessage(new cms::Message());
	pMessage->write(m_nFrameType);
	pMessage->write(m_nClientType);
	pMessage->write(m_strUserName);
	pMessage->write(m_strPwd);
	pMessage->write(m_strAckInfo);
	return pMessage;
}

NS_END(TA_Base_Test) 



