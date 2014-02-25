#ifndef __CLASS_INSTRUMENT_TICK_INFO_H__
#define __CLASS_INSTRUMENT_TICK_INFO_H__ 


#include "CFDServerCommonData.h"
#include "BarCalculator.h"
#include "MyTick.h"

NS_BEGIN(TA_Base_App)


class CInstrumentTickInfo
{
public:
	CInstrumentTickInfo(CMyTick& tickInfo);	
	~CInstrumentTickInfo(void);

public:
	CInstrumentTickInfo& operator=(const CInstrumentTickInfo& instrumentTickInfo);
public:
	void logInfo();
public:
	CMyTick m_TickInfo;
};


NS_END(TA_Base_App)


#endif // __CLASS_INSTRUMENT_BAR_INFO_H__









