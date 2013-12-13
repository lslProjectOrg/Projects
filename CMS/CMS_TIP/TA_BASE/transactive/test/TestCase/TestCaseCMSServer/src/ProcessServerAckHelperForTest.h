#ifndef __CLASS_PROCESS_SERVER_ACK_HELPER_FOR_TEST__HH__
#define __CLASS_PROCESS_SERVER_ACK_HELPER_FOR_TEST__HH__

#include "UtilityFun.h"
#include "Message.h"
#include "ClientWorkerForTest.h"

NS_BEGIN(TA_Base_Test) 


class CProcessServerAckHekperForTest
{
public:
	CProcessServerAckHekperForTest(void);
	~CProcessServerAckHekperForTest(void);

public:
	CClientWorkerForTest::ProcessedFrameRes  processMessage(cms::Message::Ptr pMessage);


private:
	CClientWorkerForTest::ProcessedFrameRes _ProcessSTOCAckUserLogin(cms::Message::Ptr pMessage );
	CClientWorkerForTest::ProcessedFrameRes _ProcessSTOCAckBeginTest(cms::Message::Ptr pMessage );
	CClientWorkerForTest::ProcessedFrameRes _ProcessSTOCAckDoTest(cms::Message::Ptr pMessage );
	CClientWorkerForTest::ProcessedFrameRes _ProcessSTOCAckEndTest(cms::Message::Ptr pMessage );
	CClientWorkerForTest::ProcessedFrameRes _ProcessSTOCAckUserLogOut(cms::Message::Ptr pMessage );
};

NS_END(TA_Base_Test) 


#endif //__CLASS_PROCESS_CLIENT_REQ_HELPER_FOR_TEST__HH__


