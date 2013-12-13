#include "STOCAckBeginTestFrame.h"


#include "BoostLogger.h"
USING_BOOST_LOG;


NS_BEGIN(TA_Base_Test) 




STOCAckBeginTestFrame::STOCAckBeginTestFrame(void)
{
	BOOST_LOG_FUNCTION();

	m_nFrameType_SampleValue = S_TO_C_ACK_BEGIN_TEST;
	m_strACKInfo_SampleValue = "I am ready, you can begin to do Test Now!";

	setSampleValue();
}

STOCAckBeginTestFrame::~STOCAckBeginTestFrame(void)
{
	BOOST_LOG_FUNCTION();

}


int STOCAckBeginTestFrame::setSampleValue()
{
	int nFunRes = 0;

	m_nFrameType = m_nFrameType_SampleValue;
	m_strACKInfo = m_strACKInfo_SampleValue;

	return nFunRes;
}


int STOCAckBeginTestFrame::setDataWithMessage(cms::Message::Ptr pMessage)
{
	int nFunRes = 0;

	{
		m_nFrameType = m_nFrameType_SampleValue;

		pMessage->read(m_strACKInfo);
	}

	return nFunRes;
}


int STOCAckBeginTestFrame::checkValue()
{
	int nFunRes = 0;

	if (m_nFrameType_SampleValue != m_nFrameType)
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

cms::Message::Ptr STOCAckBeginTestFrame::getMessage()
{
	cms::Message::Ptr pMessage(new cms::Message());
	pMessage->write(m_nFrameType);
	pMessage->write(m_strACKInfo);
	return pMessage;
}

NS_END(TA_Base_Test) 



