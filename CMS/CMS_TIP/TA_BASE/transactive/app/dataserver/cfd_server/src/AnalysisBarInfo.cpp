#include "AnalysisBarInfo.h"


#include "MarketData.h"
#include "BarCalculator.h"

#include "CFDServerUtilityFun.h"

#include "core/utilities/src/BoostLogger.h"
USING_BOOST_LOG;



NS_BEGIN(TA_Base_App) 

//////////////////////////////////////////////////////////////////////////
CAnalysisBarInfo::CAnalysisBarInfo(unsigned int nInstrumentID, const std::string& strHistoryDataFileName)
{	
	BOOST_LOG_FUNCTION();	

	m_pUtilityFun = new CCFDServerUtilityFun();
	m_pNewBarFor_60_Seconds = NULL;
	//m_nInstrumentID = 3620;
	m_nInstrumentID = nInstrumentID;
	//m_strHistoryDataFileName = "C:\\LSL\\SVNWork\\CMS\\PUBLIC\\MarketData\\sample\\sample.csv";
	m_strHistoryDataFileName = strHistoryDataFileName;

	_InitAnalysisBarInfo();

}

CAnalysisBarInfo::~CAnalysisBarInfo(void)
{
	BOOST_LOG_FUNCTION();

	if (NULL != m_pUtilityFun)
	{
		delete m_pUtilityFun;
		m_pUtilityFun = NULL;
	}

	if (NULL != m_pNewBarFor_60_Seconds)
	{
		delete m_pNewBarFor_60_Seconds;
		m_pNewBarFor_60_Seconds = NULL;
	}


}



void CAnalysisBarInfo::HandleUpdateBar(int interval, const Bar &bar)
{
	std::ostringstream sreaamTmp;
	std::string strLogInfo;
	sreaamTmp.str("");
	sreaamTmp<<"UpdateBar interval="<<interval;
	strLogInfo = sreaamTmp.str();

	if (interval == 60)
	{
		if (NULL != m_pNewBarFor_60_Seconds)
		{
			//update bar
			(*m_pNewBarFor_60_Seconds) = bar;
			m_pUtilityFun.logBarInfo(strLogInfo, *m_pNewBarFor_60_Seconds);
		}
	}
	
}

void CAnalysisBarInfo::HandleNewBar(int interval, const Bar &bar)
{
	std::ostringstream sreaamTmp;
	std::string strLogInfo;

	if (interval == 60)
	{
		if (NULL != m_pNewBarFor_60_Seconds)
		{
			//save bar
			sreaamTmp.str("");
			sreaamTmp<<"save old bar interval="<<interval;
			strLogInfo = sreaamTmp.str();
			m_pUtilityFun.logBarInfo(strLogInfo, *m_pNewBarFor_60_Seconds);

			//reset bar
			m_pNewBarFor_60_Seconds->update(0, 0, 0);
		}
		if (NULL == m_pNewBarFor_60_Seconds)
		{
			m_pNewBarFor_60_Seconds = new Bar();
			m_pNewBarFor_60_Seconds->update(0, 0, 0);
		}
		m_pNewBarFor_60_Seconds->update(0, 0, 0);
		(*m_pNewBarFor_60_Seconds) = bar;

		sreaamTmp.str("");
		sreaamTmp<<"new bar interval="<<interval;
		strLogInfo = sreaamTmp.str();
		m_pUtilityFun.logBarInfo(strLogInfo, *m_pNewBarFor_60_Seconds);

	}
	
}




int CAnalysisBarInfo::_InitAnalysisBarInfo()
{

	//"C:\\LSL\\SVNWork\\CMS\\PUBLIC\\MarketData\\sample\\sample.csv"
	//std::string filename = "C:\\LSL\\SVNWork\\CMS\\PUBLIC\\MarketData\\sample\\sample.csv";
	std::string filename = m_strHistoryDataFileName;
	std::ifstream priceFile(filename.c_str());

	//
	MarketData::PriceType nPriceType = MarketData::LAST_TRADED_PRICE;
	MarketData::VolumeType nVolumeType = MarketData::LAST_TRADED_VOLUME;
	unsigned int SecurityID_Value = 0;
	MarketData::MarketStatus MarkerStatus_Value = (MarketData::MarketStatus)MarketData::PRE_OPEN;
	unsigned int Time_Value = 0;
	std::string  strTime_Value = "";
	int Volume_Value = 0;
	int BidVol_Value = 0;
	int AskVol_Value = 0;
	float Price_Value = 0;
	float BidPx_Value = 0;
	float AskPx_Value = 0;
	unsigned int DataBits_Value = 0;
	unsigned int ChangeBits_Value = 0;
	//



	BarCalculator barCalc(m_nInstrumentID);
	barCalc.onNewBar = HandleNewBar;
	barCalc.onBarUpdate = HandleUpdateBar;


	barCalc.addBar(60); // One minute
	//barCalc.addBar(300); // Five Minute

	while (!priceFile.eof())
	{
		MarketData marketDataTmp(0);
		priceFile >> marketDataTmp;
		//process price ....

		//
		SecurityID_Value = marketDataTmp.getSecurityID();
		MarkerStatus_Value = (MarketData::MarketStatus)(marketDataTmp.getMarketStatus());
		Time_Value = marketDataTmp.getTime();
		strTime_Value = m_pUtilityFun.getTimeString(Time_Value);
		Volume_Value = marketDataTmp.getVolume(nVolumeType);
		BidVol_Value = marketDataTmp.getBidVol(0);
		AskVol_Value = marketDataTmp.getAskVol(0);
		Price_Value = marketDataTmp.getPrice(nPriceType);
		BidPx_Value = marketDataTmp.getBidPx(0);
		AskPx_Value = marketDataTmp.getAskPx(0);
		DataBits_Value = marketDataTmp.getDataBits();
		ChangeBits_Value = marketDataTmp.getChangeBits();
		//
		LOG_DEBUG<<"SecurityID_Value="<<SecurityID_Value;
		LOG_DEBUG<<"MarkerStatus_Value="<<MarkerStatus_Value;
		LOG_DEBUG<<"Time_Value="<<Time_Value;
		LOG_DEBUG<<"strTime_Value="<<strTime_Value;
		LOG_DEBUG<<"Volume_Value="<<Volume_Value;
		LOG_DEBUG<<"Price_Value="<<Price_Value;

		//
		barCalc.onMarketDataUpdate(marketDataTmp);
	}

	return 0;
}




NS_END(TA_Base_App) 



