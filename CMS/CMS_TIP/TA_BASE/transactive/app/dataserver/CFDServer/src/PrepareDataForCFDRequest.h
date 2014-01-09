#ifndef __CLASS_PREPARE_DATA_FOR_CFD_REQUEST_H__
#define __CLASS_PREPARE_DATA_FOR_CFD_REQUEST_H__ 


#include "app/dataserver/cfd_server/src/CFDServerCommonData.h"
#include "BarCalculator.h"
#include "CFDRequest.h"

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
	CPrepareDataForCFDRequest(const CCFDRequest& cfdRequest);	
	~CPrepareDataForCFDRequest(void);
public:
	int getCFDBarInfo(LstBarInfoT& lstCFDBarInfo);

private:
	int _GetCFDBarInfoFromStorager(CCFDInstrumentBarInfoStorager* pCFDStorager, LstBarInfoT& lstCFDBarInfo);
private:
	CCFDServerUtilityFun*    m_pUtilityFun;
	CCFDRequest   m_nCFDRequest;
};


NS_END(TA_Base_App)


#endif // __CLASS_PREPARE_DATA_FOR_CFD_REQUEST_H__









