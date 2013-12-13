#ifndef __SERVER_TO_CLIENT_ACK_BEGIN_TEST_FRAME__HH__
#define __SERVER_TO_CLIENT_ACK_BEGIN_TEST_FRAME__HH__



#include "UtilityFun.h"
#include "Message.h"

NS_BEGIN(TA_Base_Test) 


class STOCAckBeginTestFrame
{
public:
	STOCAckBeginTestFrame(void);
	~STOCAckBeginTestFrame(void);

public:
	cms::Message::Ptr getMessage();
	int setDataWithMessage(cms::Message::Ptr pMessage);
	int setSampleValue();
	int checkValue();
public:
	int m_nFrameType;//frame type  //enClientServerCommunicationFrameType//FrameType
	std::string  m_strACKInfo;//clientIP, port, TestClientIndex, 

private:
	int m_nFrameType_SampleValue;
	std::string  m_strACKInfo_SampleValue;//clientIP, port, TestClientIndex, 
};

NS_END(TA_Base_Test) 


#endif //__SERVER_TO_CLIENT_ACK_BEGIN_TEST_FRAME__HH__


