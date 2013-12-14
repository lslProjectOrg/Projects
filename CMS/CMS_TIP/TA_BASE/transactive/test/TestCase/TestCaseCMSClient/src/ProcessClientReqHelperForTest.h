#ifndef __CLASS_PROCESS_CLIENT_REQ_HELPER_FOR_TEST__HH__
#define __CLASS_PROCESS_CLIENT_REQ_HELPER_FOR_TEST__HH__

#include "UtilityFun.h"
#include "Message.h"

NS_BEGIN(TA_Base_Test) 


class CProcessClientReqHelperForTest
{
public:
	CProcessClientReqHelperForTest(void);
	~CProcessClientReqHelperForTest(void);

public:
	int processMessage(cms::Message::Ptr pMessage);



private:
	int _ProcessCTOSReqUserLogin(cms::Message::Ptr pMessage );
	int _ProcessCTOSReqBeginTest(cms::Message::Ptr pMessage );
	int _ProcessCTOSReqDoTest(cms::Message::Ptr pMessage );
	int _ProcessCTOSReqEndTest(cms::Message::Ptr pMessage );
	int _ProcessCTOSReqUserLogout(cms::Message::Ptr pMessage );
};

NS_END(TA_Base_Test) 


#endif //__CLASS_PROCESS_CLIENT_REQ_HELPER_FOR_TEST__HH__


