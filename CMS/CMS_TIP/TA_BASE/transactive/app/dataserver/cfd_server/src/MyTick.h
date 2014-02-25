#ifndef __CLASS_MY_TICK_H__
#define __CLASS_MY_TICK_H__ 


#include "CFDServerCommonData.h"
#include "BarCalculator.h"

NS_BEGIN(TA_Base_App)


class CMyTick
{
public:
	CMyTick(void);	
	~CMyTick(void);

public:
	CMyTick& operator=(const CMyTick& myTick);
public:
	void logInfo();
public:
	unsigned int Time;

	float BidPx;
	float AskPx;
	float LastPx;

	int BidVol;
	int AskVol;
	int LastVol;
};


NS_END(TA_Base_App)


#endif // __CLASS_INSTRUMENT_BAR_INFO_H__









