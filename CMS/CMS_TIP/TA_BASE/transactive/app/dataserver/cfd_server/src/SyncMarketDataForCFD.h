#ifndef __CLASS_SYNC_MARKET_DATA_FOR_CFD_H__
#define __CLASS_SYNC_MARKET_DATA_FOR_CFD_H__ 


#include "app/dataserver/cfd_server/src/CFDServerCommonData.h"
#include "BarCalculator.h"
NS_BEGIN(TA_Base_App)

class CCFDServerUtilityFun;
class CCFDInstrumentBarInfo;

class CSyncMarketDataForCFD
{
public:
	typedef std::list<CCFDInstrumentBarInfo*>				  LstCFDBarInfoT;
	typedef std::list<CCFDInstrumentBarInfo*>::iterator       LstCFDBarInfoIterT;

	typedef std::list<Bar*>				  LstBarInfoT;
	typedef std::list<Bar*>::iterator     LstBarInfoIterT;

public:
	CSyncMarketDataForCFD(void);	
	~CSyncMarketDataForCFD(void);
public:
	void setInterval(int nInterval);
	void setCFDInstrumentID(int nCFDInstrumentID);

	int syncSingleCFDBarInfo( const Bar& nBarInfoFirst, const Bar& nBarInfoSecond, LstCFDBarInfoT& lstCFDbarInfo);
	int clearCFDBarInfoList(LstCFDBarInfoT& lstCFDbarInfo);

private:
	int _SyncSingleBarInfo( const Bar& nBarInfoFirst, const Bar& nBarInfoSecond, 
		LstBarInfoT& lstBarInfoFirst, LstBarInfoT& lstBarInfoSecond);
	int _ClearBarInfoList(LstBarInfoT& lstBarInfo);
	int _SyncLstCFDBarInfo(LstBarInfoT& lstBarInfoFirst, LstBarInfoT& lstBarInfoSecond, LstCFDBarInfoT& lstCFDBarInfo);
private:
	CCFDServerUtilityFun*    m_pUtilityFun;
	int m_nInterval;// = 5;//second
	int m_nCFDInstrumentID;// = 3620*3521;

};


NS_END(TA_Base_App)


#endif // __CLASS_SYNC_MARKET_DATA_FOR_CFD_H__









