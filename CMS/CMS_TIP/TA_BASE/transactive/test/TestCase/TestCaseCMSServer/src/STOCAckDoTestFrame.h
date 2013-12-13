#ifndef __SERVER_TO_CLIENT_ACK_DO_TEST_FRAME__HH__
#define __SERVER_TO_CLIENT_ACK_DO_TEST_FRAME__HH__

#include "UtilityFun.h"
#include "Message.h"

NS_BEGIN(TA_Base_Test) 


class STOCAckDoTestFrame
{
public:
	STOCAckDoTestFrame(void);
	~STOCAckDoTestFrame(void);

public:
	cms::Message::Ptr getMessage();
	int setDataWithMessage(cms::Message::Ptr pMessage);
	int setSampleValue();
	int checkValue();

private:
	std::string _InitMaxLenString();
	//biger than 100 bites
public:
	int m_nFrameType;//frame type  //enClientServerCommunicationFrameType//FrameType
	int m_nDoTestType;//DoTestType
	std::string  m_strReqInfo;//clientIP, port, TestClientIndex, 

	unsigned int m_id;//4
	char m_securityType;//1
	std::string m_code;//80
	std::string m_symbol;//80
	std::string m_securityId;//80
	std::string m_securityExchange;//80
	double m_tickSize;//8
	double m_tickValue;//8
	int m_decimalPlace;//4
	int m_maturityDate;//4

private:
	int m_nFrameType_SampleValue;
	int m_nDoTestType_SampleValue;
	std::string  m_strReqInfo_SampleValue;

	unsigned int m_id_SampleValue;//4
	char m_securityType_SampleValue;//1
	std::string m_code_SampleValue;//80
	std::string m_symbol_SampleValue;//80
	std::string m_securityId_SampleValue;//80
	std::string m_securityExchange_SampleValue;//80
	double m_tickSize_SampleValue;//8
	double m_tickValue_SampleValue;//8
	int m_decimalPlace_SampleValue;//4
	int m_maturityDate_SampleValue;//4
};

NS_END(TA_Base_Test) 


#endif //__SERVER_TO_CLIENT_ACK_DO_TEST_FRAME__HH__


