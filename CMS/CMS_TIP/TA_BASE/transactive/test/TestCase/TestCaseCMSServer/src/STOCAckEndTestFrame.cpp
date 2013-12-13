#include "STOCAckEndTestFrame.h"


#include "BoostLogger.h"
USING_BOOST_LOG;


NS_BEGIN(TA_Base_Test) 




STOCAckEndTestFrame::STOCAckEndTestFrame(void)
{
	BOOST_LOG_FUNCTION();

	m_nFrameType_SampleValue = S_TO_C_ACK_END_TEST;
	m_strAckInfo_SampleValue = "you can end Test now!";

	setSampleValue();
}

STOCAckEndTestFrame::~STOCAckEndTestFrame(void)
{
	BOOST_LOG_FUNCTION();

}


int STOCAckEndTestFrame::setSampleValue()
{
	int nFunRes = 0;

	m_nFrameType = m_nFrameType_SampleValue;
	m_strAckInfo = m_strAckInfo_SampleValue;

	return nFunRes;
}


int STOCAckEndTestFrame::setDataWithMessage(cms::Message::Ptr pMessage)
{
	int nFunRes = 0;

	{
		m_nFrameType = m_nFrameType_SampleValue;
		pMessage->read(m_strAckInfo);
	}

	return nFunRes;
}


int STOCAckEndTestFrame::checkValue()
{
	int nFunRes = 0;

	if (m_nFrameType_SampleValue != m_nFrameType)
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


cms::Message::Ptr STOCAckEndTestFrame::getMessage()
{
	cms::Message::Ptr pMessage(new cms::Message());
	pMessage->write(m_nFrameType);
	pMessage->write(m_strAckInfo);
	return pMessage;
}

NS_END(TA_Base_Test) 



