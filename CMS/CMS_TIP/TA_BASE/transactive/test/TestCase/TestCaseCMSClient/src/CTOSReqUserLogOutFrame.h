#ifndef __CLIENT_TO_SERVER_REQ_USER_LOGOUT_FRAME__HH__
#define __CLIENT_TO_SERVER_REQ_USER_LOGOUT_FRAME__HH__

#include "UtilityFun.h"
#include "Message.h"

NS_BEGIN(TA_Base_Test) 


class CTOSReqUserLogOutFrame
{
public:
	CTOSReqUserLogOutFrame(void);
	~CTOSReqUserLogOutFrame(void);

public:
	cms::Message::Ptr getMessage();
	int setDataWithMessage(cms::Message::Ptr pMessage);
	int setSampleValue();
	int checkValue();
	void logFrameInfo();
public:
	int m_nFrameType;//frame type  //enClientServerCommunicationFrameType//FrameType
	int m_nClientType;//client type //ClientIdentity
	std::string  m_strUserName;
	std::string  m_strPwd;
	std::string  m_strReqInfo;//clientIP, port, TestClientIndex, 

private:
	int m_nFrameType_SampleValue;
	int m_nClientType_SampleValue;//client type //ClientIdentity
	std::string  m_strUserName_SampleValue;
	std::string  m_strPwd_SampleValue;
	std::string  m_strReqInfo_SampleValue;//clientIP, port, TestClientIndex, 
};

NS_END(TA_Base_Test) 


#endif //__CLIENT_TO_SERVER_REQ_USER_LOGOUT_FRAME__HH__


