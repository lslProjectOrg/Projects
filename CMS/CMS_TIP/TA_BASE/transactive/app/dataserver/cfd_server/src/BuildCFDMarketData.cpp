#include "BuildCFDMarketData.h"
#include "CFDServerUtilityFun.h"

#include "core/utilities/src/BoostLogger.h"
USING_BOOST_LOG;

NS_BEGIN(TA_Base_App)


CBuildCFDMarketData::CBuildCFDMarketData(void)
{
	BOOST_LOG_FUNCTION();
	m_pUtilityFun = new CCFDServerUtilityFun();
}

CBuildCFDMarketData::~CBuildCFDMarketData( void )
{
	BOOST_LOG_FUNCTION();
	if (NULL != m_pUtilityFun)
	{
		delete m_pUtilityFun;
		m_pUtilityFun = NULL;
	}
}

int CBuildCFDMarketData::buildCFDMarketData( const Bar &DataFirst, const Bar &DataSecond, Bar &CFDMarketData, int interval)
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	std::string strLogInfo;

	if (DataFirst.Time != DataSecond.Time)
	{
		LOG_ERROR<<"buildCFDMarketData DataFirst.Time != DataSecond.Time"
			<<" ["<<"DataFirst.Time="<<m_pUtilityFun->dataTimeToStr(DataFirst.Time)<<"]"
			<<" ["<<"DataSecond.Time="<<m_pUtilityFun->dataTimeToStr(DataSecond.Time)<<"]"; 
		nFunRes = -1;
		return nFunRes;
	}

	CFDMarketData.Time = DataFirst.Time;
	CFDMarketData.Open = DataFirst.Open - DataSecond.Open;
	CFDMarketData.Close = DataFirst.Close - DataSecond.Close;
	CFDMarketData.High = DataFirst.High - DataSecond.High;
	CFDMarketData.Low = DataFirst.Low - DataSecond.Low;
	CFDMarketData.Volume = DataFirst.Volume - DataSecond.Volume;

	strLogInfo = " buildCFDMarketData ";
	m_pUtilityFun->logBarInfo(strLogInfo, interval, &CFDMarketData);

	return nFunRes;
}



NS_END(TA_Base_App)
















