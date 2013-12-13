#ifndef __CLIENT_TO_SERVER_REQ_END_TEST_FRAME__HH__
#define __CLIENT_TO_SERVER_REQ_END_TEST_FRAME__HH__

#include "UtilityFun.h"
#include "Message.h"

NS_BEGIN(TA_Base_Test) 


class CTOSReqEndTestFrame
{
public:
	CTOSReqEndTestFrame(void);
	~CTOSReqEndTestFrame(void);

public:
	cms::Message::Ptr getMessage();
	int setDataWithMessage(cms::Message::Ptr pMessage);
	int setSampleValue();
	int checkValue();
public:
	int m_nFrameType;//frame type  //enClientServerCommunicationFrameType//FrameType
	std::string  m_strReqInfo;//clientIP, port, TestClientIndex, 

private:
	int m_nFrameType_SampleValue;
	std::string  m_strReqInfo_SampleValue;//clientIP, port, TestClientIndex, 
};

NS_END(TA_Base_Test) 


#endif //__CLIENT_TO_SERVER_REQ_END_TEST_FRAME__HH__


