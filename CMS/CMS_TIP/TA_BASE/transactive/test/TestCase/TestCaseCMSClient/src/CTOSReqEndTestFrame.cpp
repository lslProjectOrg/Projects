#include "CTOSReqEndTestFrame.h"


#include "BoostLogger.h"
USING_BOOST_LOG;


NS_BEGIN(TA_Base_Test) 




CTOSReqEndTestFrame::CTOSReqEndTestFrame(void)
{
	BOOST_LOG_FUNCTION();

	m_nFrameType_SampleValue = C_TO_S_REQ_END_TEST;
	m_strReqInfo_SampleValue = "client want to end Test!";

	setSampleValue();
}

CTOSReqEndTestFrame::~CTOSReqEndTestFrame(void)
{
	BOOST_LOG_FUNCTION();

}


int CTOSReqEndTestFrame::setSampleValue()
{
	int nFunRes = 0;

	m_nFrameType = m_nFrameType_SampleValue;
	m_strReqInfo = m_strReqInfo_SampleValue;

	return nFunRes;
}


int CTOSReqEndTestFrame::setDataWithMessage(cms::Message::Ptr pMessage)
{
	int nFunRes = 0;

	{
		m_nFrameType = m_nFrameType_SampleValue;
		pMessage->read(m_strReqInfo);
	}

	return nFunRes;
}


int CTOSReqEndTestFrame::checkValue()
{
	int nFunRes = 0;

	if (m_nFrameType_SampleValue != m_nFrameType)
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


cms::Message::Ptr CTOSReqEndTestFrame::getMessage()
{
	cms::Message::Ptr pMessage(new cms::Message());
	pMessage->write(m_nFrameType);
	pMessage->write(m_strReqInfo);
	return pMessage;
}


void CTOSReqEndTestFrame::logFrameInfo()
{
	LOG_TRACE<<"FrameInfo:"
		<<" "<<"m_nFrameType="<<m_nFrameType
		<<" "<<"m_strReqInfo="<<m_strReqInfo;
}




NS_END(TA_Base_Test) 



