#ifndef __CLASS_INSTRUMENT_BAR_INFO_H__
#define __CLASS_INSTRUMENT_BAR_INFO_H__ 


#include "CFDServerCommonData.h"
#include "BarCalculator.h"

NS_BEGIN(TA_Base_App)


class CInstrumentBarInfo
{
public:
	CInstrumentBarInfo(int interval, Bar& barInfo);	
	~CInstrumentBarInfo(void);

public:
	CInstrumentBarInfo& operator=(const CInstrumentBarInfo& instrumentBarInfo);
public:
	void logInfo();
public:
	int m_nInterval;
	Bar m_BarInfo;
};


NS_END(TA_Base_App)


#endif // __CLASS_INSTRUMENT_BAR_INFO_H__









