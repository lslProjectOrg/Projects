#include "MyTick.h"

#include "BoostLogger.h"
USING_BOOST_LOG;

NS_BEGIN(TA_Base_App)

CMyTick::CMyTick(void)
{
	BOOST_LOG_FUNCTION();
}

CMyTick::~CMyTick(void)
{
	BOOST_LOG_FUNCTION();
}


CMyTick& CMyTick::operator=(const CMyTick& myTick )
{
	BOOST_LOG_FUNCTION();

	Time = myTick.Time;
	BidPx = myTick.BidPx;
	AskPx = myTick.AskPx;
	LastPx = myTick.LastPx;
	BidVol = myTick.BidVol;
	AskVol = myTick.AskVol;
	LastVol = myTick.LastVol;

	return *this;
}




void CMyTick::logInfo()
{
	BOOST_LOG_FUNCTION();

	return;
}


NS_END(TA_Base_App)
















