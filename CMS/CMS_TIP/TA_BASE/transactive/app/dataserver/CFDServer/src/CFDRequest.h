#ifndef __CLASS_CFD_REQUEST_H__
#define __CLASS_CFD_REQUEST_H__ 


#include "app/dataserver/cfd_server/src/CFDServerCommonData.h"
#include "BarCalculator.h"


NS_BEGIN(TA_Base_App)


class CCFDRequest
{
public:
	CCFDRequest(void);	
	~CCFDRequest(void);

public:
	CCFDRequest& operator=(const CCFDRequest& cfdRequest);

public:
	void logInfo();


public:
	unsigned int m_nCFDInstrumentIDFirst;// = 3620
	unsigned int m_nCFDInstrumentIDSecond;// = 3521;
	int m_nXValue;
	int m_nYValue;
	int m_nCFDInterval;//CFD bar info


};


NS_END(TA_Base_App)


#endif // __CLASS_CFD_REQUEST_H__









