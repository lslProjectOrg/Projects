#ifndef __CLASS_PREPARE_DATA_FOR_CFD_REQUEST_H__
#define __CLASS_PREPARE_DATA_FOR_CFD_REQUEST_H__ 


#include "app/dataserver/cfd_server/src/CFDServerCommonData.h"
#include "BarCalculator.h"


NS_BEGIN(TA_Base_App)

class CCFDServerUtilityFun;
class CCreateCFDInstrumentBarInfo;
class CCFDInstrumentBarInfoStorager;

class CPrepareDataForCFDRequest
{
public:
	typedef std::list<Bar>				LstBarInfoT;
	typedef std::list<Bar>::iterator	LstBarInfoIterT;
public:
	CPrepareDataForCFDRequest(unsigned int nInstrumentIDFirst, unsigned int nInstrumentIDSecond);	
	~CPrepareDataForCFDRequest(void);
public:
	//get CFD 1mins bar Info
	void setCFDBarInterval(int nCFDBarInterval);
	void setInstrumentBarInterval(int nInstrumentBarInterval);
	int getCFDBarInfo(LstBarInfoT& lstCFDBarInfo);

private:
	int _GetCFDBarInfoFromStorager(CCFDInstrumentBarInfoStorager* pCFDStorager, LstBarInfoT& lstCFDBarInfo);
private:
	CCFDServerUtilityFun*    m_pUtilityFun;
	unsigned int m_nInstrumentIDFirst;
	unsigned int m_nInstrumentIDSecond;
	int m_nCFDBarInterval;
	int m_nInstrumentBarInterval;
};


NS_END(TA_Base_App)


#endif // __CLASS_PREPARE_DATA_FOR_CFD_REQUEST_H__









