#ifndef __CLASS_TEST_FRAME__HH__
#define __CLASS_TEST_FRAME__HH__

#include "UtilityFun.h"
#include "MessageBroker.h"
#include "Message.h"

using namespace cms;
NS_BEGIN(TA_Base_Test) 


class CTestFrame
{
public:
	CTestFrame(void);
	~CTestFrame(void);

public:
	typedef shared_ptr<CTestFrame> Ptr;

public:
	int checkFrameData();
	void logFrameInfo(const std::string& strLogInfo);

	void setFrameMaxValue();
	void setFrameMinValue();
	void setFrameNomalValue();
	int setDataWithMessage(Message::Ptr pMessage);

private:

	int _CheckFrameDataMax();
	int _CheckFrameDataMin();
	int _CheckFrameDataNomal();
public:
	char m_nFrameType;

	SessionID m_nSessionIDSrc;

	char m_nCharValue;
	unsigned char m_nUCharValue;

	short m_nShortValue;
	unsigned short m_nUShortValue;

	int m_nIntValue;
	unsigned int m_nUIntValue;

	BigInt32 m_nBitIntValue;
	UBigInt32 m_nUBitIntValue;

	float m_fFloatValue;
	double m_fDoubleValue;
	std::string m_strStringValue;



};

NS_END(TA_Base_Test) 


#endif //__CLASS_TEST_FRAME__HH__


