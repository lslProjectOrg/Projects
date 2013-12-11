#ifndef __CLIENT_TO_SERVER_REQ_USER_LOGIN__HH__
#define __CLIENT_TO_SERVER_REQ_USER_LOGIN__HH__

#include "UtilityFun.h"

NS_BEGIN(TA_Base_Test) 


class CTOSReqUserLoginFrame
{
public:
	CTOSReqUserLoginFrame(void);
	~CTOSReqUserLoginFrame(void);

public:
	int m_nFrameType;
	SessionID m_nSessionIDSrc;


};

NS_END(TA_Base_Test) 


#endif //__CLIENT_TO_SERVER_REQ_USER_LOGIN__HH__


