#include "CTOSReqDoTestFrame.h"


#include "BoostLogger.h"
USING_BOOST_LOG;


NS_BEGIN(TA_Base_Test) 




CTOSReqDoTestFrame::CTOSReqDoTestFrame(void)
{
	BOOST_LOG_FUNCTION();

	m_nFrameType_SampleValue = C_TO_S_REQ_DO_TEST;
	m_nDoTestType_SampleValue = DoTestType_TestStable;
	m_strReqInfo_SampleValue = "Test Stable Req";

	m_id_SampleValue = 4;
	m_securityType_SampleValue = 4;
	m_code_SampleValue = _InitMaxLenString();
	m_symbol_SampleValue = m_code_SampleValue;
	m_securityId_SampleValue = m_code_SampleValue;
	m_securityExchange_SampleValue = m_code_SampleValue;
	m_tickSize_SampleValue = 8;
	m_tickValue_SampleValue = 8;
	m_decimalPlace_SampleValue = 4;
	m_maturityDate_SampleValue = 4;

	setSampleValue();
}

CTOSReqDoTestFrame::~CTOSReqDoTestFrame(void)
{
	BOOST_LOG_FUNCTION();

}


int CTOSReqDoTestFrame::setSampleValue()
{
	int nFunRes = 0;

	m_nFrameType = m_nFrameType_SampleValue;
	m_nDoTestType = m_nDoTestType_SampleValue;
	m_strReqInfo = m_strReqInfo_SampleValue;
	m_id = m_id_SampleValue;
	m_securityType = m_securityType_SampleValue;
	m_code = m_code_SampleValue;
	m_symbol = m_symbol_SampleValue;
	m_securityId = m_securityId_SampleValue;
	m_securityExchange = m_securityExchange_SampleValue;
	m_tickSize = m_tickSize_SampleValue;
	m_tickValue = m_tickValue_SampleValue;
	m_decimalPlace = m_decimalPlace_SampleValue;
	m_maturityDate = m_maturityDate_SampleValue;

	return nFunRes;
}


int CTOSReqDoTestFrame::setDataWithMessage(cms::Message::Ptr pMessage)
{
	int nFunRes = 0;

	{
		m_nFrameType = m_nFrameType_SampleValue;
		pMessage->read(m_nDoTestType);
		pMessage->read(m_strReqInfo);

		pMessage->read(m_id);
		pMessage->read(m_securityType);
		pMessage->read(m_code);
		pMessage->read(m_symbol);
		pMessage->read(m_securityId);
		pMessage->read(m_securityExchange);
		pMessage->read(m_tickSize);
		pMessage->read(m_tickValue);
		pMessage->read(m_decimalPlace);
		pMessage->read(m_maturityDate);

	}

	return nFunRes;
}


int CTOSReqDoTestFrame::checkValue()
{
	int nFunRes = 0;

	if (m_nFrameType_SampleValue != m_nFrameType)
	{
		LOG_ERROR<<"checkValue Error!";
		nFunRes = -1;
		return nFunRes;
	}

	if (m_nDoTestType_SampleValue != m_nDoTestType)
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

	if (m_id_SampleValue != m_id)
	{
		LOG_ERROR<<"checkValue Error!";
		nFunRes = -1;
		return nFunRes;
	}

	if (m_securityType_SampleValue != m_securityType)
	{
		LOG_ERROR<<"checkValue Error!";
		nFunRes = -1;
		return nFunRes;
	}

	if (m_code_SampleValue != m_code)
	{
		LOG_ERROR<<"checkValue Error!";
		nFunRes = -1;
		return nFunRes;
	}

	if (m_symbol_SampleValue != m_symbol)
	{
		LOG_ERROR<<"checkValue Error!";
		nFunRes = -1;
		return nFunRes;
	}

	if (m_securityId_SampleValue != m_securityId)
	{
		LOG_ERROR<<"checkValue Error!";
		nFunRes = -1;
		return nFunRes;
	}

	if (m_securityExchange_SampleValue != m_securityExchange)
	{
		LOG_ERROR<<"checkValue Error!";
		nFunRes = -1;
		return nFunRes;
	}


	if (m_tickSize_SampleValue != m_tickSize)
	{
		LOG_ERROR<<"checkValue Error!";
		nFunRes = -1;
		return nFunRes;
	}

	if (m_tickValue_SampleValue != m_tickValue)
	{
		LOG_ERROR<<"checkValue Error!";
		nFunRes = -1;
		return nFunRes;
	}

	if (m_decimalPlace_SampleValue != m_decimalPlace)
	{
		LOG_ERROR<<"checkValue Error!";
		nFunRes = -1;
		return nFunRes;
	}

	if (m_maturityDate_SampleValue != m_maturityDate)
	{
		LOG_ERROR<<"checkValue Error!";
		nFunRes = -1;
		return nFunRes;
	}


	return nFunRes;
}


cms::Message::Ptr CTOSReqDoTestFrame::getMessage()
{
	cms::Message::Ptr pMessage(new cms::Message());
	pMessage->write(m_nFrameType);
	pMessage->write(m_nDoTestType);
	pMessage->write(m_strReqInfo);
	pMessage->write(m_id);
	pMessage->write(m_securityType);
	pMessage->write(m_code);
	pMessage->write(m_symbol);
	pMessage->write(m_securityId);
	pMessage->write(m_securityExchange);
	pMessage->write(m_tickSize);
	pMessage->write(m_tickValue);
	pMessage->write(m_decimalPlace);
	pMessage->write(m_maturityDate);

	return pMessage;
}



std::string CTOSReqDoTestFrame::_InitMaxLenString()
{
	BOOST_LOG_FUNCTION();

	//1024*64
	std::string strTemp1024;

	strTemp1024.clear();
	//128<std::string<256
	for (int nIndex = 0; nIndex < 80; nIndex++)
	{
		strTemp1024 += "T";
	}
	return strTemp1024;
}

NS_END(TA_Base_Test) 



