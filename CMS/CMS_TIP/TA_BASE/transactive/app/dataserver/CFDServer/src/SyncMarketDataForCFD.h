#ifndef __CLASS_SYNC_MARKET_DATA_FOR_CFD_H__
#define __CLASS_SYNC_MARKET_DATA_FOR_CFD_H__ 


#include "CFDServerCommonData.h"

#include "BarCalculator.h"
#include "CFDRequest.h"

NS_BEGIN(TA_Base_App)

class CCFDServerUtilityFun;
class CCFDInstrumentBarInfo;

class CSyncMarketDataForCFD
{
public:
	enum enSynType
	{
		SynType_BEGIN,
		SynType_EQUAL,
		SynType_SMALL,
		SynType_BIGGER,
		SynType_END,
	};

	enum enNextWorkType
	{
		NextWorkType_BEGIN,
		NextWorkType_UseNewFirst_UseNewSecond,
		NextWorkType_UseNewFirst_ReUseSecond,
		NextWorkType_ReUseFirst_UseNewSecond,
		NextWorkType_END,
	};
public:
	typedef std::list<CCFDInstrumentBarInfo*>				  LstCFDBarInfoT;
	typedef std::list<CCFDInstrumentBarInfo*>::iterator       LstCFDBarInfoIterT;

	typedef std::list<MarketData*>				  LstBarInfoT;
	typedef std::list<MarketData*>::iterator     LstBarInfoIterT;

public:
	CSyncMarketDataForCFD(const CCFDRequest& cfdRequest);	
	~CSyncMarketDataForCFD(void);
public:
	void setCFDRequest(const CCFDRequest& cfdRequest);

	int syncSingleCFDBarInfo( 
		const MarketData& nBarInfoFirst, 
		const MarketData& nBarInfoSecond, 
		LstCFDBarInfoT& lstCFDbarInfo,
		enNextWorkType& nNextWorkType);
	int clearCFDBarInfoList(LstCFDBarInfoT& lstCFDbarInfo);

private:
	int _SyncSingleBarInfo( 
		const MarketData& nBarInfoFirst, 
		const MarketData& nBarInfoSecond, 
		LstBarInfoT& lstBarInfoFirst, 
		LstBarInfoT& lstBarInfoSecond,
		enNextWorkType& nNextWorkType);
	int _ClearBarInfoList(LstBarInfoT& lstBarInfo);
private:
	int _SyncLstCFDBarInfo(LstBarInfoT& lstBarInfoFirst, LstBarInfoT& lstBarInfoSecond, LstCFDBarInfoT& lstCFDBarInfo);
	int _SyncSingleBarInfoSynType( enSynType nSynType, const MarketData& nBarInfoFirst, const MarketData& nBarInfoSecond, LstBarInfoT& lstBarInfoFirst, LstBarInfoT& lstBarInfoSecond);
	int _SyncSingleBarInfoSynTypeEqual( enSynType nSynType, const MarketData& nBarInfoFirst, const MarketData& nBarInfoSecond, LstBarInfoT& lstBarInfoFirst, LstBarInfoT& lstBarInfoSecond);
	int _SyncSingleBarInfoSynTypeSmall( enSynType nSynType, const MarketData& nBarInfoFirst, const MarketData& nBarInfoSecond, LstBarInfoT& lstBarInfoFirst, LstBarInfoT& lstBarInfoSecond);
private:
	CCFDServerUtilityFun*    m_pUtilityFun;
	CCFDRequest m_CFDRequest;

};


NS_END(TA_Base_App)


#endif // __CLASS_SYNC_MARKET_DATA_FOR_CFD_H__









