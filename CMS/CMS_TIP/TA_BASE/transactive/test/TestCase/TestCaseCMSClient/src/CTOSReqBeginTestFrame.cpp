#include "CTOSReqBeginTestFrame.h"


#include "BoostLogger.h"
USING_BOOST_LOG;


NS_BEGIN(TA_Base_Test) 




CTOSReqBeginTestFrame::CTOSReqBeginTestFrame(void)
{
	BOOST_LOG_FUNCTION();

	m_nFrameType_SampleValue = C_TO_S_REQ_BEGIN_TEST;
	m_strReqInfo_SampleValue = "Client want to beign Test!";

	setSampleValue();
}

CTOSReqBeginTestFrame::~CTOSReqBeginTestFrame(void)
{
	BOOST_LOG_FUNCTION();

}


int CTOSReqBeginTestFrame::setSampleValue()
{
	int nFunRes = 0;

	m_nFrameType = m_nFrameType_SampleValue;
	m_strReqInfo = m_strReqInfo_SampleValue;

	return nFunRes;
}


int CTOSReqBeginTestFrame::setDataWithMessage(cms::Message::Ptr pMessage)
{
	int nFunRes = 0;

	{
		m_nFrameType = m_nFrameType_SampleValue;
		pMessage->read(m_strReqInfo);
	}

	return nFunRes;
}


int CTOSReqBeginTestFrame::checkValue()
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


cms::Message::Ptr CTOSReqBeginTestFrame::getMessage()
{
	cms::Message::Ptr pMessage(new cms::Message());
	pMessage->write(m_nFrameType);
	pMessage->write(m_strReqInfo);
	return pMessage;
}

void CTOSReqBeginTestFrame::logFrameInfo()
{
	LOG_TRACE<<"FrameInfo:"
		<<" "<<"m_nFrameType="<<m_nFrameType
		<<" "<<"m_strReqInfo="<<m_strReqInfo;
}



NS_END(TA_Base_Test) 



