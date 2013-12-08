#include "TestFrame.h"
#include "CommonData.h"
#include "Logger.h"

// extern boost::condition_variable g_conditionMainRun;

NS_BEGIN(TA_Base_Test) 

extern std::string g_string_strServerAddress;
extern int g_n_EachClientSendFrameNUM;
extern int g_n_ClientNUM;
extern int g_n_TotalClientSendFrameNUM;







//////global Data init////////////////////////////////////////////////////////////////////

extern   char g_TestData_nomal_Frame;// = ComFrameType_TestNomalData;
extern   char g_TestData_min_Frame;// = ComFrameType_TestMinData;
extern   char g_TestData_max_Frame;// = ComFrameType_TestMaxData;

extern   char g_TestData_min_nCharValue;// = SCHAR_MIN;//SCHAR_MAX 
extern   char g_TestData_nomal_nCharValue;// = 'A';
extern   char g_TestData_max_nCharValue;// = SCHAR_MAX;//SCHAR_MAX

extern   unsigned char g_TestData_min_nUCharValue;// = 0;//UCHAR_MAX
extern   unsigned char g_TestData_nomal_nUCharValue;// = 240;
extern   unsigned char g_TestData_max_nUCharValue;// = UCHAR_MAX;//UCHAR_MAX

extern   short g_TestData_min_nShortIntValue;// = SHRT_MIN;//SHRT_MAX
extern   short g_TestData_nomal_nShortIntValue;// = 1;//SHRT_MAX
extern   short g_TestData_max_nShortIntValue;// = SHRT_MAX;//SHRT_MAX

extern   unsigned short g_TestData_min_nUShortIntValue;// = 0;//USHRT_MAX
extern   unsigned short g_TestData_nomal_nUShortIntValue;// = 1;//INT_MAX
extern   unsigned short g_TestData_max_nUShortIntValue;// = USHRT_MAX;//USHRT_MAX

extern   int g_TestData_min_nIntValue;// = INT_MIN;//INT_MAX
extern   int g_TestData_nomal_nIntValue;// = -1111;
extern   int g_TestData_max_nIntValue;// = INT_MAX;//INT_MAX

extern   unsigned int g_TestData_min_nUIntValue;// = 0;//UINT_MAX
extern   unsigned int g_TestData_nomal_nUIntValue;// = 1111;
extern   unsigned int g_TestData_max_nUIntValue;// = UINT_MAX;//UINT_MAX



#ifndef __linux__
extern   BigInt32 g_TestData_min_nBitIntValue;// = LLONG_MIN;//LLONG_MAX
extern   UBigInt32 g_TestData_min_nUBitIntValue;// = 0;//ULONG_MAX
#else 
extern   BigInt32 g_TestData_min_nBitIntValue;// = LONG_MIN;//LONG_MAX
extern   UBigInt32 g_TestData_min_nUBitIntValue;// = 0;//ULONG_MAX
#endif

extern   BigInt32 g_TestData_nomal_nBitIntValue;// = -222222222;
extern   UBigInt32 g_TestData_nomal_nUBitIntValue;// = 222222222; 

#ifndef __linux__
extern   BigInt32 g_TestData_max_nBitIntValue;// = LLONG_MAX;//LLONG_MIN
extern   UBigInt32 g_TestData_max_nUBitIntValue;// = ULLONG_MAX;//0
#else 
extern   BigInt32 g_TestData_max_nBitIntValue;// = LONG_MAX;
extern   UBigInt32 g_TestData_max_nUBitIntValue;// = ULONG_MAX;
#endif

extern   float g_TestData_min_fFloatValue;// = FLT_MIN;//FLT_MAX ;
extern   float g_TestData_nomal_fFloatValue;// = 3.1416f;
extern   float g_TestData_max_fFloatValue;// = FLT_MAX;//FLT_MAX ;

extern   double g_TestData_min_fDoubleValue;// = DBL_MIN;//DBL_MAX
extern   double g_TestData_nomal_fDoubleValue;// = 3.14159;
extern   double g_TestData_max_fDoubleValue;// = DBL_MAX;//DBL_MAX

extern   std::string g_TestData_min_strStringValue;// = "T";
extern   std::string g_TestData_nomal_strStringValue;// = "Hello World!!!";
extern   std::string g_TestData_max_strStringValue;// = "255";//sms support string max len 256
extern   int DEF_CMS_MAX_STESTRING_LEN;//       255

//////global Data init end////////////////////////////////////////////////////////////////////




CTestFrame::CTestFrame(void)
{
	//
	m_nFrameType = ComFrameType_NULL;
	m_nSessionIDSrc = 0;
	m_nCharValue = 0;
	m_nUCharValue = 0;
	m_nShortValue = 0;
	m_nUShortValue = 0;
	m_nIntValue = 0;
	m_nUIntValue = 0;
	m_nBitIntValue = 0;
	m_nUBitIntValue = 0;
	m_fFloatValue = 0;
	m_fDoubleValue = 0;
	m_strStringValue.clear();
	//

}

CTestFrame::~CTestFrame(void)
{

}



void CTestFrame::logFrameInfo(const std::string& strLogInfo)
{
	_SysLog(LogSourceFLInfo, TA_Base_Test::DebugDebug, 
		"strLogInfo=%s, nFrameType = %d", strLogInfo.c_str(), m_nFrameType);

	
	_SysLog(LogSourceFLInfo, TA_Base_Test::DebugDebug, 
		"strLogInfo=%s, m_nSessionIDSrc = %u", strLogInfo.c_str(), m_nSessionIDSrc);

	_SysLog(LogSourceFLInfo, TA_Base_Test::DebugDebug, 
		"strLogInfo=%s, nCharValue = %d", strLogInfo.c_str(), m_nCharValue);
	_SysLog(LogSourceFLInfo, TA_Base_Test::DebugDebug, 
		"strLogInfo=%s, nUCharValue = %u", strLogInfo.c_str(), m_nUCharValue);

	_SysLog(LogSourceFLInfo, TA_Base_Test::DebugDebug, 
		"strLogInfo=%s, nShortValue = %hd", strLogInfo.c_str(), m_nShortValue);
	_SysLog(LogSourceFLInfo, TA_Base_Test::DebugDebug, 
		"strLogInfo=%s, nUShortValue = %u", strLogInfo.c_str(), m_nUShortValue);

	_SysLog(LogSourceFLInfo, TA_Base_Test::DebugDebug, 
		"strLogInfo=%s, nIntValue = %d", strLogInfo.c_str(), m_nIntValue);
	_SysLog(LogSourceFLInfo, TA_Base_Test::DebugDebug, 
		"strLogInfo=%s, nUIntValue = %u", strLogInfo.c_str(), m_nUIntValue);

	_SysLog(LogSourceFLInfo, TA_Base_Test::DebugDebug, 
		"strLogInfo=%s, nBitIntValue = %lld", strLogInfo.c_str(), m_nBitIntValue);
	_SysLog(LogSourceFLInfo, TA_Base_Test::DebugDebug, 
		"strLogInfo=%s, nUBitIntValue = %llu", strLogInfo.c_str(), m_nUBitIntValue);

	//_SysLog(LogSourceFLInfo, TA_Base_Test::DebugDebug, 
	//	"m_nRecvMsgCount=%d, fGetFloatValue = %lf", m_nRecvMsgCount, fGetFloatValue);
	_SysLog(LogSourceFLInfo, TA_Base_Test::DebugDebug, 
		"strLogInfo=%s, fFloatValue = %e", strLogInfo.c_str(), m_fFloatValue);
	_SysLog(LogSourceFLInfo, TA_Base_Test::DebugDebug, 
		"strLogInfo=%s, fDoubleValue = %e", strLogInfo.c_str(), m_fDoubleValue);

	_SysLog(LogSourceFLInfo, TA_Base_Test::DebugDebug, 
		"strLogInfo=%s, strStringValue = %s", strLogInfo.c_str(), m_strStringValue.c_str());

}

void CTestFrame::setFrameNomalValue()
{
	m_nFrameType = g_TestData_nomal_Frame;
	m_nSessionIDSrc = 0;
	m_nCharValue = g_TestData_nomal_nCharValue;
	m_nUCharValue = g_TestData_nomal_nUCharValue;
	m_nShortValue = g_TestData_nomal_nShortIntValue;
	m_nUShortValue = g_TestData_nomal_nUShortIntValue;
	m_nIntValue = g_TestData_nomal_nIntValue;
	m_nUIntValue = g_TestData_nomal_nUIntValue;
	m_nBitIntValue = g_TestData_nomal_nBitIntValue;
	m_nUBitIntValue = g_TestData_nomal_nUBitIntValue;
	m_fFloatValue = g_TestData_nomal_fFloatValue;
	m_fDoubleValue = g_TestData_nomal_fDoubleValue;
	m_strStringValue = g_TestData_nomal_strStringValue;

}


void CTestFrame::setFrameMinValue()
{
	m_nFrameType = g_TestData_min_Frame;
	m_nSessionIDSrc = 0;
	m_nCharValue = g_TestData_min_nCharValue;
	m_nUCharValue = g_TestData_min_nUCharValue;
	m_nShortValue = g_TestData_min_nShortIntValue;
	m_nUShortValue = g_TestData_min_nUShortIntValue;
	m_nIntValue = g_TestData_min_nIntValue;
	m_nUIntValue = g_TestData_min_nUIntValue;
	m_nBitIntValue = g_TestData_min_nBitIntValue;
	m_nUBitIntValue = g_TestData_min_nUBitIntValue;
	m_fFloatValue = g_TestData_min_fFloatValue;
	m_fDoubleValue = g_TestData_min_fDoubleValue;
	m_strStringValue = g_TestData_min_strStringValue;

}
void CTestFrame::setFrameMaxValue()
{
	m_nFrameType = g_TestData_max_Frame;
	m_nSessionIDSrc = 0;
	m_nCharValue = g_TestData_max_nCharValue;
	m_nUCharValue = g_TestData_max_nUCharValue;
	m_nShortValue = g_TestData_max_nShortIntValue;
	m_nUShortValue = g_TestData_max_nUShortIntValue;
	m_nIntValue = g_TestData_max_nIntValue;
	m_nUIntValue = g_TestData_max_nUIntValue;
	m_nBitIntValue = g_TestData_max_nBitIntValue;
	m_nUBitIntValue = g_TestData_max_nUBitIntValue;
	m_fFloatValue = g_TestData_max_fFloatValue;
	m_fDoubleValue = g_TestData_max_fDoubleValue;
	m_strStringValue = g_TestData_max_strStringValue;
}


int CTestFrame::setDataWithMessage(Message::Ptr pMessage)
{
	int nFunRes = 0;

	pMessage->read(m_nFrameType);
	pMessage->read(m_nSessionIDSrc);

	if (ComFrameType_TestNomalData == m_nFrameType
		|| ComFrameType_TestMinData == m_nFrameType 
		|| ComFrameType_TestMaxData == m_nFrameType)
	{
		pMessage->read(m_nCharValue);
		pMessage->read(m_nUCharValue);

		pMessage->read(m_nShortValue);
		pMessage->read(m_nUShortValue);

		pMessage->read(m_nIntValue);
		pMessage->read(m_nUIntValue);

		pMessage->read(m_nBitIntValue);
		pMessage->read(m_nUBitIntValue);

		pMessage->read(m_fFloatValue);
		pMessage->read(m_fDoubleValue);

		pMessage->read(m_strStringValue);
	}
	else
	{
		_SysLog(LogSourceFLInfo, TA_Base_Test::DebugError, 
			"DebugError: not support Frame nGetFrameType=%d", m_nFrameType);
		nFunRes = -1;
	}

	return nFunRes;
}

int CTestFrame::_CheckFrameDataNomal()
{
	int nFunRes = 0;
	bool bCheckDataRes = false;

	if (ComFrameType_TestNomalData != m_nFrameType)
	{
		_SysLog(LogSourceFLInfo, TA_Base_Test::DebugError, "DebugError: ComFrameType_TestNomalData != nFrameType");
		nFunRes = -1;
		return nFunRes;
	}


	bCheckDataRes = (m_nCharValue == g_TestData_nomal_nCharValue);// = 'A';
	if (!bCheckDataRes)
	{
		_SysLog(LogSourceFLInfo, TA_Base_Test::DebugError, "DebugError: check Data Error! nCharValue=%d, g_TestData_nomal_nCharValue=%d", m_nCharValue, g_TestData_nomal_nCharValue);
		nFunRes = -1;
		return nFunRes;

	}

	bCheckDataRes = (m_nUCharValue == g_TestData_nomal_nUCharValue);// = 'A';
	if (!bCheckDataRes)
	{
		_SysLog(LogSourceFLInfo, TA_Base_Test::DebugError, "DebugError: check Data Error");
		nFunRes = -1;
		return nFunRes;

	}

	bCheckDataRes = (m_nShortValue == g_TestData_nomal_nShortIntValue);// = 'A';
	if (!bCheckDataRes)
	{
		_SysLog(LogSourceFLInfo, TA_Base_Test::DebugError, "DebugError: check Data Error");
		nFunRes = -1;
		return nFunRes;

	}

	bCheckDataRes = (m_nUShortValue == g_TestData_nomal_nUShortIntValue);// = 'A';
	if (!bCheckDataRes)
	{
		_SysLog(LogSourceFLInfo, TA_Base_Test::DebugError, "DebugError: check Data Error");
		nFunRes = -1;
		return nFunRes;

	}

	bCheckDataRes = (m_nIntValue == g_TestData_nomal_nIntValue);// = 'A';
	if (!bCheckDataRes)
	{
		_SysLog(LogSourceFLInfo, TA_Base_Test::DebugError, "DebugError: check Data Error");
		nFunRes = -1;
		return nFunRes;

	}

	bCheckDataRes = (m_nUIntValue == g_TestData_nomal_nUIntValue);// = 'A';
	if (!bCheckDataRes)
	{
		_SysLog(LogSourceFLInfo, TA_Base_Test::DebugError, "DebugError: check Data Error");
		nFunRes = -1;
		return nFunRes;

	}

	bCheckDataRes = (m_nBitIntValue == g_TestData_nomal_nBitIntValue);// = 'A';
	if (!bCheckDataRes)
	{
		_SysLog(LogSourceFLInfo, TA_Base_Test::DebugError, "DebugError: check Data Error");
		nFunRes = -1;
		return nFunRes;

	}

	bCheckDataRes = (m_nUBitIntValue == g_TestData_nomal_nUBitIntValue);// = 'A';
	if (!bCheckDataRes)
	{
		_SysLog(LogSourceFLInfo, TA_Base_Test::DebugError, "DebugError: check Data Error");
		nFunRes = -1;
		return nFunRes;

	}

	bCheckDataRes = ((m_fFloatValue - g_TestData_nomal_fFloatValue) < 0.0001);// = 'A';
	if (!bCheckDataRes)
	{
		_SysLog(LogSourceFLInfo, TA_Base_Test::DebugError, "DebugError: check Data Error");
		nFunRes = -1;
		return nFunRes;

	}

	bCheckDataRes = ((m_fDoubleValue - g_TestData_nomal_fDoubleValue) < 0.0001);// = 'A';
	if (!bCheckDataRes)
	{
		_SysLog(LogSourceFLInfo, TA_Base_Test::DebugError, "DebugError: check Data Error");
		nFunRes = -1;
		return nFunRes;

	}

	bCheckDataRes = (m_strStringValue == g_TestData_nomal_strStringValue);// = 'A';
	if (!bCheckDataRes)
	{
		_SysLog(LogSourceFLInfo, TA_Base_Test::DebugError, "DebugError: check Data Error");
		nFunRes = -1;
		return nFunRes;

	}


	_SysLog(LogSourceFLInfo, TA_Base_Test::DebugDebug, "check one frame ok! type = ComFrameType_TestNomalData");



	return nFunRes;
}

int CTestFrame::_CheckFrameDataMin()
{
	int nFunRes = 0;
	bool bCheckDataRes = false;
	if (ComFrameType_TestMinData != m_nFrameType)
	{
		_SysLog(LogSourceFLInfo, TA_Base_Test::DebugError, "DebugError:Frame Type Error! ComFrameType_TestMinData != m_nFrameType");
		nFunRes = -1;
		return nFunRes;
	}


	bCheckDataRes = (m_nCharValue == g_TestData_min_nCharValue);// = 'A';
	if (!bCheckDataRes)
	{
		_SysLog(LogSourceFLInfo, TA_Base_Test::DebugError, "DebugError: check Data Error");
		nFunRes = -1;
		return nFunRes;

	}

	bCheckDataRes = (m_nUCharValue == g_TestData_min_nUCharValue);// = 'A';
	if (!bCheckDataRes)
	{
		_SysLog(LogSourceFLInfo, TA_Base_Test::DebugError, "DebugError: check Data Error");
		nFunRes = -1;
		return nFunRes;

	}

	bCheckDataRes = (m_nShortValue == g_TestData_min_nShortIntValue);// = 'A';
	if (!bCheckDataRes)
	{
		_SysLog(LogSourceFLInfo, TA_Base_Test::DebugError, "DebugError: check Data Error");
		nFunRes = -1;
		return nFunRes;

	}

	bCheckDataRes = (m_nUShortValue == g_TestData_min_nUShortIntValue);// = 'A';
	if (!bCheckDataRes)
	{
		_SysLog(LogSourceFLInfo, TA_Base_Test::DebugError, "DebugError: check Data Error");
		nFunRes = -1;
		return nFunRes;

	}

	bCheckDataRes = (m_nIntValue == g_TestData_min_nIntValue);// = 'A';
	if (!bCheckDataRes)
	{
		_SysLog(LogSourceFLInfo, TA_Base_Test::DebugError, "DebugError: check Data Error");
		nFunRes = -1;
		return nFunRes;

	}

	bCheckDataRes = (m_nUIntValue == g_TestData_min_nUIntValue);// = 'A';
	if (!bCheckDataRes)
	{
		_SysLog(LogSourceFLInfo, TA_Base_Test::DebugError, "DebugError: check Data Error");
		nFunRes = -1;
		return nFunRes;

	}

	bCheckDataRes = (m_nBitIntValue == g_TestData_min_nBitIntValue);// = 'A';
	if (!bCheckDataRes)
	{
		_SysLog(LogSourceFLInfo, TA_Base_Test::DebugError, "DebugError: check Data Error");
		nFunRes = -1;
		return nFunRes;

	}

	bCheckDataRes = (m_nUBitIntValue == g_TestData_min_nUBitIntValue);// = 'A';
	if (!bCheckDataRes)
	{
		_SysLog(LogSourceFLInfo, TA_Base_Test::DebugError, "DebugError: check Data Error");
		nFunRes = -1;
		return nFunRes;

	}

	bCheckDataRes = ((m_fFloatValue - g_TestData_min_fFloatValue) < 0.0001);// = 'A';
	if (!bCheckDataRes)
	{
		_SysLog(LogSourceFLInfo, TA_Base_Test::DebugError, "DebugError: check Data Error");
		nFunRes = -1;
		return nFunRes;

	}

	bCheckDataRes = ((m_fDoubleValue - g_TestData_min_fDoubleValue) < 0.0001);// = 'A';
	if (!bCheckDataRes)
	{
		_SysLog(LogSourceFLInfo, TA_Base_Test::DebugError, "DebugError: check Data Error");
		nFunRes = -1;
		return nFunRes;

	}

	bCheckDataRes = (m_strStringValue == g_TestData_min_strStringValue);// = 'A';
	if (!bCheckDataRes)
	{
		_SysLog(LogSourceFLInfo, TA_Base_Test::DebugError, "DebugError: check Data Error");
		nFunRes = -1;
		return nFunRes;

	}
	_SysLog(LogSourceFLInfo, TA_Base_Test::DebugDebug, "check one frame ok! type = ComFrameType_TestMinData");




	return nFunRes;
}

int CTestFrame::_CheckFrameDataMax()
{
	int nFunRes = 0;
	bool bCheckDataRes = false;


	if (ComFrameType_TestMaxData != m_nFrameType)
	{
		_SysLog(LogSourceFLInfo, TA_Base_Test::DebugError, "DebugError: Frame Type Error! ComFrameType_TestMaxData != m_nFrameType");
		nFunRes = -1;
		return nFunRes;
	}


	bCheckDataRes = (m_nCharValue == g_TestData_max_nCharValue);// = 'A';
	if (!bCheckDataRes)
	{
		_SysLog(LogSourceFLInfo, TA_Base_Test::DebugError, "DebugError: check Data Error");
		nFunRes = -1;
		return nFunRes;

	}

	bCheckDataRes = (m_nUCharValue == g_TestData_max_nUCharValue);// = 'A';
	if (!bCheckDataRes)
	{
		_SysLog(LogSourceFLInfo, TA_Base_Test::DebugError, "DebugError: check Data Error");
		nFunRes = -1;
		return nFunRes;

	}

	bCheckDataRes = (m_nShortValue == g_TestData_max_nShortIntValue);// = 'A';
	if (!bCheckDataRes)
	{
		_SysLog(LogSourceFLInfo, TA_Base_Test::DebugError, "DebugError: check Data Error");
		nFunRes = -1;
		return nFunRes;

	}

	bCheckDataRes = (m_nUShortValue == g_TestData_max_nUShortIntValue);// = 'A';
	if (!bCheckDataRes)
	{
		_SysLog(LogSourceFLInfo, TA_Base_Test::DebugError, "DebugError: check Data Error");
		nFunRes = -1;
		return nFunRes;

	}

	bCheckDataRes = (m_nIntValue == g_TestData_max_nIntValue);// = 'A';
	if (!bCheckDataRes)
	{
		_SysLog(LogSourceFLInfo, TA_Base_Test::DebugError, "DebugError: check Data Error");
		nFunRes = -1;
		return nFunRes;

	}

	bCheckDataRes = (m_nUIntValue == g_TestData_max_nUIntValue);// = 'A';
	if (!bCheckDataRes)
	{
		_SysLog(LogSourceFLInfo, TA_Base_Test::DebugError, "DebugError: check Data Error");
		nFunRes = -1;
		return nFunRes;

	}

	bCheckDataRes = (m_nBitIntValue == g_TestData_max_nBitIntValue);// = 'A';
	if (!bCheckDataRes)
	{
		_SysLog(LogSourceFLInfo, TA_Base_Test::DebugError, "DebugError: check Data Error");
		nFunRes = -1;
		return nFunRes;

	}

	bCheckDataRes = (m_nUBitIntValue == g_TestData_max_nUBitIntValue);// = 'A';
	if (!bCheckDataRes)
	{
		_SysLog(LogSourceFLInfo, TA_Base_Test::DebugError, "DebugError: check Data Error");
		nFunRes = -1;
		return nFunRes;

	}

	bCheckDataRes = ((m_fFloatValue - g_TestData_max_fFloatValue) < 0.0001);// = 'A';
	if (!bCheckDataRes)
	{
		_SysLog(LogSourceFLInfo, TA_Base_Test::DebugError, "DebugError: check Data Error");
		nFunRes = -1;
		return nFunRes;

	}

	bCheckDataRes = ((m_fDoubleValue - g_TestData_max_fDoubleValue) < 0.0001);// = 'A';
	if (!bCheckDataRes)
	{
		_SysLog(LogSourceFLInfo, TA_Base_Test::DebugError, "DebugError: check Data Error");
		nFunRes = -1;
		return nFunRes;

	}

	bCheckDataRes = (m_strStringValue == g_TestData_max_strStringValue);// = 'A';
	if (!bCheckDataRes)
	{
		_SysLog(LogSourceFLInfo, TA_Base_Test::DebugError, "DebugError: check Data Error");
		nFunRes = -1;
		return nFunRes;

	}
	_SysLog(LogSourceFLInfo, TA_Base_Test::DebugDebug, "check one frame ok! type = ComFrameType_TestMaxData");



	return nFunRes;
}

int CTestFrame::checkFrameData()
{
	int nFunRes = 0;
	bool bCheckDataRes = false;

	switch(m_nFrameType)
	{
	case ComFrameType_TestNomalData:
		nFunRes = _CheckFrameDataNomal();
		break;
	case ComFrameType_TestMinData:
		nFunRes = _CheckFrameDataMin();
		break;
	case ComFrameType_TestMaxData:
		nFunRes = _CheckFrameDataMax();
		break;
	case ComFrameType_NULL:
		_SysLog(LogSourceFLInfo, TA_Base_Test::DebugError, "DebugError: FrameType Error! nFrameType=ComFrameType_NULL");
		break;
	default:
		_SysLog(LogSourceFLInfo, TA_Base_Test::DebugError, 
			"DebugError: not support check Frame nFrameType=%d", m_nFrameType);
		nFunRes = -1;
		break;
	}
	return nFunRes;



}

NS_END(TA_Base_Test) 



