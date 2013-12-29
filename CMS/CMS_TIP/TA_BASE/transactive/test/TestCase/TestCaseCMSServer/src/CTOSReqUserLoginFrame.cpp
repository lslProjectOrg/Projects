#include "CTOSReqUserLoginFrame.h"


#include "BoostLogger.h"
USING_BOOST_LOG;


NS_BEGIN(TA_Base_Test) 




CTOSReqUserLoginFrame::CTOSReqUserLoginFrame(void)
{
	BOOST_LOG_FUNCTION();

	m_nFrameType_SampleValue = C_TO_S_REQ_USER_LOGIN;
	m_nClientType_SampleValue = ClientIdentity_TestClient;
	m_strUserName_SampleValue = "m_strUserName";
	m_strPwd_SampleValue = "m_strPwd";
	m_strReqInfo_SampleValue = "Client want to login!";

	setSampleValue();
}

CTOSReqUserLoginFrame::~CTOSReqUserLoginFrame(void)
{
	BOOST_LOG_FUNCTION();

}


int CTOSReqUserLoginFrame::setSampleValue()
{
	int nFunRes = 0;

	m_nFrameType = m_nFrameType_SampleValue;
	m_nClientType = m_nClientType_SampleValue;
	m_strUserName = m_strUserName_SampleValue;
	m_strPwd = m_strPwd_SampleValue;
	m_strReqInfo = m_strReqInfo_SampleValue;

	return nFunRes;
}


int CTOSReqUserLoginFrame::setDataWithMessage(cms::Message::Ptr pMessage)
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


int CTOSReqUserLoginFrame::checkValue()
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


cms::Message::Ptr CTOSReqUserLoginFrame::getMessage()
{
	cms::Message::Ptr pMessage(new cms::Message());
	pMessage->write(m_nFrameType);
	pMessage->write(m_nClientType);
	pMessage->write(m_strUserName);
	pMessage->write(m_strPwd);
	pMessage->write(m_strReqInfo);
	return pMessage;
}


void CTOSReqUserLoginFrame::logFrameInfo()
{
	LOG_TRACE<<"FrameInfo:"
		<<" "<<"m_nFrameType="<<m_nFrameType
		<<" "<<"m_nClientType="<<m_nClientType
		<<" "<<"m_strUserName="<<m_strUserName
		<<" "<<"m_strPwd="<<m_strPwd
		<<" "<<"m_strReqInfo="<<m_strReqInfo;
		
}


NS_END(TA_Base_Test) 



