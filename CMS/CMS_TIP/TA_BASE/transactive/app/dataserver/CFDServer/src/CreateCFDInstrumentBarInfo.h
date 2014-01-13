#ifndef __CLASS_CREATE_CFD_INSTRUMENT_BAR_INFO_H__
#define __CLASS_CREATE_CFD_INSTRUMENT_BAR_INFO_H__ 


#include "CFDServerCommonData.h"
#include "BarCalculator.h"
#include "SyncMarketDataForCFD.h"
#include "CFDRequest.h"

NS_BEGIN(TA_Base_App)

class CCFDServerUtilityFun;
class CCFDInstrumentBarInfo;
class CCFDInstrumentBarInfoCalculator;
class CInstrumentBarInfoStorager;

class CCreateCFDInstrumentBarInfo
{
public:
	typedef std::list<Bar>				LstBarInfoT;
	typedef std::list<Bar>::iterator	LstBarInfoIterT;

	typedef std::list<MarketData>				LstMarketDataT;
	typedef std::list<MarketData>::iterator	    LstMarketDataIterT;
public:
	CCreateCFDInstrumentBarInfo(const CCFDRequest& cfdRequest);	
	~CCreateCFDInstrumentBarInfo(void);
public:
	int createCFDbarInfoByMarketData();
	void createCFDbarInfoByLst(LstMarketDataT& lstBarFirst, LstMarketDataT& lstBarSecond);

private:
	int _UpdateCFDMarketData(CSyncMarketDataForCFD::LstCFDBarInfoT& lstCFDBarInfo);
	int _InitInstrumentDataSource(unsigned int nInstrumentID, std::ifstream** ppStorager);
	int _UnInitInstrumentDataSource(unsigned int nInstrumentID, std::ifstream** ppStorager);
	int _GetNextBarInfo( std::ifstream* pStorager, bool& bIsLstEnd, MarketData& BarInfoGet);
private:
	std::string _GetInstrumentPriceFileName(unsigned int nInstrumentID);
private:
	CCFDServerUtilityFun*    m_pUtilityFun;
	CSyncMarketDataForCFD*   m_CSyncMarketDataForCFD;
	CCFDInstrumentBarInfoCalculator*   m_pCFDBarInfoCalculator;
	CCFDRequest m_CFDRequest;
};


NS_END(TA_Base_App)


#endif // __CLASS_CREATE_CFD_INSTRUMENT_BAR_INFO_H__









