#ifndef __CLASS_ANALYSIS_BAR_INFO__HH__
#define __CLASS_ANALYSIS_BAR_INFO__HH__

#include "app/dataserver/cfd_server/src/CFDServerCommonData.h"

#include <boost/chrono.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>



using namespace cms;


NS_BEGIN(TA_Base_App) 

class CCFDServerUtilityFun;

class CAnalysisBarInfo 
{
public:
	CAnalysisBarInfo(void);
	~CAnalysisBarInfo(void);
private:
	CCFDServerUtilityFun   m_pUtilityFun;

private:
	unsigned int m_nInstrumentID;
	std::string m_strHistoryDataFileName;
	Bar*  m_pNewBarFor_60_Seconds;
};

NS_END(TA_Base_App) 


#endif //__CLASS_ANALYSIS_BAR_INFO__HH__


