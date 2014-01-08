#ifndef __CLASS_CREATE_CFD_INSTRUMENT_BAR_INFO_H__
#define __CLASS_CREATE_CFD_INSTRUMENT_BAR_INFO_H__ 


#include "app/dataserver/cfd_server/src/CFDServerCommonData.h"
#include "BarCalculator.h"
#include "SyncMarketDataForCFD.h"

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
public:
	CCreateCFDInstrumentBarInfo(unsigned int nInstrumentIDFirst, unsigned int nInstrumentIDSecond);	
	~CCreateCFDInstrumentBarInfo(void);
public:
	//use Instrument 5seconds bar info to calc  CFD 1mins bar Info
	void setInterval(int nInterval);
	int createCFDbarInfoByDB();
	void createCFDbarInfoByLst(LstBarInfoT& lstBarFirst, LstBarInfoT& lstBarSecond);
public:
	void doOneTest();

private:
	int _UpdateCFDMarketData(CSyncMarketDataForCFD::LstCFDBarInfoT& lstCFDBarInfo);
	int _InitInstrumentDataSource(unsigned int nInstrumentID, int nInterval, CInstrumentBarInfoStorager** ppStorager);
	int _UnInitInstrumentDataSource(unsigned int nInstrumentID, int nInterval, CInstrumentBarInfoStorager** ppStorager);
	int _GetNextBarInfo( CInstrumentBarInfoStorager* pStorager, bool& bIsLstEnd, Bar& BarInfoGet);
private:
	CCFDServerUtilityFun*    m_pUtilityFun;
	CSyncMarketDataForCFD*   m_CSyncMarketDataForCFD;
	CCFDInstrumentBarInfoCalculator*   m_pCFDBarInfoCalculator;
	unsigned int m_nInstrumentIDFirst;
	unsigned int m_nInstrumentIDSecond;
	int m_nInterval;
};


NS_END(TA_Base_App)


#endif // __CLASS_CREATE_CFD_INSTRUMENT_BAR_INFO_H__









