#include "FrameUserLoginREQ.h"


#include "BoostLogger.h"
USING_BOOST_LOG;


NS_BEGIN(TA_Base_Test) 


CTOSReqUserLoginFrame::CTOSReqUserLoginFrame(void)
{
	BOOST_LOG_FUNCTION();

	m_nFrameType = C_TO_S_REQ_USER_LOGIN;
}

CTOSReqUserLoginFrame::~CTOSReqUserLoginFrame(void)
{
	BOOST_LOG_FUNCTION();

}


NS_END(TA_Base_Test) 



