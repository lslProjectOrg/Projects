#include "STOCAckUserLoginFrame.h"


#include "BoostLogger.h"
USING_BOOST_LOG;


NS_BEGIN(TA_Base_Test) 




STOCAckUserLoginFrame::STOCAckUserLoginFrame(void)
{
	BOOST_LOG_FUNCTION();

	m_nFrameType_SampleValue = S_TO_C_ACK_USER_LOGIN;
	m_nClientType_SampleValue = ClientIdentity_TestClient;
	m_strUserName_SampleValue = "m_strUserName";
	m_strPwd_SampleValue = "m_strPwd";
	m_strACKInfo_SampleValue = "login ok!";

	setSampleValue();
}

STOCAckUserLoginFrame::~STOCAckUserLoginFrame(void)
{
	BOOST_LOG_FUNCTION();

}


int STOCAckUserLoginFrame::setSampleValue()
{
	int nFunRes = 0;

	m_nFrameType = m_nFrameType_SampleValue;
	m_nClientType = m_nClientType_SampleValue;
	m_strUserName = m_strUserName_SampleValue;
	m_strPwd = m_strPwd_SampleValue;
	m_strACKInfo = m_strACKInfo_SampleValue;

	return nFunRes;
}


int STOCAckUserLoginFrame::setDataWithMessage(const int& nFrameType, Message::Ptr pMessage)
{
	int nFunRes = 0;

	if (m_nFrameType_SampleValue == nFrameType)
	{
		m_nFrameType = m_nFrameType_SampleValue;

		pMessage->read(m_nClientType);
		pMessage->read(m_strUserName);

		pMessage->read(m_strPwd);
		pMessage->read(m_strACKInfo);
	}
	else
	{

		LOG_ERROR<<"Error: not support Frame"
			<<" "<<"nFrameType="<<nFrameType;
		nFunRes = -1;
	}

	return nFunRes;
}


int STOCAckUserLoginFrame::checkValue()
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

	if (m_strACKInfo_SampleValue != m_strACKInfo)
	{
		LOG_ERROR<<"checkValue Error!";
		nFunRes = -1;
		return nFunRes;
	}

	return nFunRes;
}

NS_END(TA_Base_Test) 


