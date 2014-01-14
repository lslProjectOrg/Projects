

#include <iostream>
#include <fstream>
#include <sstream>
#include <list> 
#include <time.h>


#include <boost/foreach.hpp>
#include <boost/test/unit_test.hpp>
#include "MarketData.h"
#include "BarCalculator.h"

#include "core/utilities/src/BoostLogger.h"
USING_BOOST_LOG;





Bar* g_pNewBarFor_60_Seconds = NULL;
Bar* g_pNewBarFor_300_Seconds = NULL;

std::string getTimeString(unsigned int nTimeValue);

void logBarInfo(const std::string& strInfo, const Bar &bar)
{
	std::string strBarTime;

	strBarTime = getTimeString(bar.Time);
	LOG_DEBUG<<strInfo<<" "<<"bar.Time="<<bar.Time;
	LOG_DEBUG<<strInfo<<" "<<"strBarTime="<<strBarTime;
	LOG_DEBUG<<strInfo<<" "<<"bar.Open="<<bar.Open;
	LOG_DEBUG<<strInfo<<" "<<"bar.Close="<<bar.Close;
	LOG_DEBUG<<strInfo<<" "<<"bar.High="<<bar.High;
	LOG_DEBUG<<strInfo<<" "<<"bar.Low="<<bar.Low;
	LOG_DEBUG<<strInfo<<" "<<"bar.Volume="<<bar.Volume;


}
void HandleUpdateBar(int interval, const Bar &bar)
{
	std::ostringstream sreaamTmp;
	std::string strLogInfo;
	sreaamTmp.str("");
	sreaamTmp<<"UpdateBar interval="<<interval;
	strLogInfo = sreaamTmp.str();

	if (interval == 60)
	{
		if (NULL != g_pNewBarFor_60_Seconds)
		{
			//update bar
			(*g_pNewBarFor_60_Seconds) = bar;
			logBarInfo(strLogInfo, *g_pNewBarFor_60_Seconds);
		}
	}
	else if (interval == 300) 
	{
		if (NULL != g_pNewBarFor_300_Seconds)
		{
			//update bar
			(*g_pNewBarFor_300_Seconds) = bar;
			logBarInfo(strLogInfo, *g_pNewBarFor_300_Seconds);
		}
	}
}

void HandleNewBar(int interval, const Bar &bar)
{
	std::ostringstream sreaamTmp;
	std::string strLogInfo;

	if (interval == 60)
	{
		if (NULL != g_pNewBarFor_60_Seconds)
		{
			//save bar
			sreaamTmp.str("");
			sreaamTmp<<"save old bar interval="<<interval;
			strLogInfo = sreaamTmp.str();
			logBarInfo(strLogInfo, *g_pNewBarFor_60_Seconds);

			//freee bar
			delete g_pNewBarFor_60_Seconds;
			g_pNewBarFor_60_Seconds = NULL;
		}
		g_pNewBarFor_60_Seconds = new Bar();
		g_pNewBarFor_60_Seconds->update(0, 0, 0);
		(*g_pNewBarFor_60_Seconds) = bar;

		sreaamTmp.str("");
		sreaamTmp<<"new bar interval="<<interval;
		strLogInfo = sreaamTmp.str();
		logBarInfo(strLogInfo, *g_pNewBarFor_60_Seconds);

	}
	else if (interval == 300) 
	{
		if (NULL != g_pNewBarFor_300_Seconds)
		{
			//save bar
			sreaamTmp.str("");
			sreaamTmp<<"save old bar interval="<<interval;
			strLogInfo = sreaamTmp.str();

			logBarInfo(strLogInfo, *g_pNewBarFor_300_Seconds);

			//freee bar
			delete g_pNewBarFor_300_Seconds;
			g_pNewBarFor_300_Seconds = NULL;
		}
		g_pNewBarFor_300_Seconds = new Bar();
		g_pNewBarFor_300_Seconds->update(0, 0, 0);
		(*g_pNewBarFor_300_Seconds) = bar;

		sreaamTmp.str("");
		sreaamTmp<<"new bar interval="<<interval;
		strLogInfo = sreaamTmp.str();
		logBarInfo(strLogInfo, *g_pNewBarFor_300_Seconds);

	}
}



void Test_BarTest()
{

	unsigned int timestamp = 1000 * 300;

	{
		BarCalculator barCalc(3620);
		barCalc.onNewBar = HandleNewBar;
		barCalc.onBarUpdate = HandleUpdateBar;
		barCalc.addBar(60); // One minute
		//strBarTime=[1970-01-04 19:21:00] strBarTime=[1970-01-04 19:22:00]
		//barCalc.addBar(300); // Five Minute
		//strBarTime=[1970-01-04 19:20:00] strBarTime=[1970-01-04 19:25:00]

		barCalc.onMarketDataUpdate(timestamp, 100, 5);
		barCalc.onMarketDataUpdate(timestamp, 99, 7);
		barCalc.onMarketDataUpdate(timestamp, 101, 4);
		timestamp += 60;
		barCalc.onMarketDataUpdate(timestamp, 102, 1);
	}
}



std::string getTimeString(unsigned int nTimeValue)
{
	std::string	strTimeString;
	char* pszCurTime = NULL;
	int nBufferSize = 256;
	struct tm* pTM = NULL;
	time_t time_Value = (time_t)nTimeValue;

	pszCurTime =new char[nBufferSize];
	memset(pszCurTime, 0, nBufferSize);

	pTM = localtime(&(time_Value));

	//format to string
	if ( NULL != pTM )
	{
		sprintf(pszCurTime, "[%04d-%02d-%02d %02d:%02d:%02d]",
			pTM->tm_year + 1900, pTM->tm_mon + 1, pTM->tm_mday,
			pTM->tm_hour, pTM->tm_min, pTM->tm_sec); 

		strTimeString = pszCurTime;     
	}

	delete pszCurTime;
	pszCurTime = NULL;
	pTM = NULL;

	return strTimeString;
}

void logInit()
{
	TA_Base_Core::CBoostLogger::getInstance();

	std::string strLogPath = "ALL_LOG_PATH";
	std::string strLogFileName = "Sample_Log_%Y-%m-%d_%H_%M_%S_%f_%4N.log";
	boost::log::trivial::severity_level nLogLevel = boost::log::trivial::trace;


	TA_Base_Core::CBoostLogger::getInstance().setLogPath(strLogPath);
	TA_Base_Core::CBoostLogger::getInstance().setLogFileName(strLogFileName);
	TA_Base_Core::CBoostLogger::getInstance().setLogLevel(nLogLevel);
	TA_Base_Core::CBoostLogger::getInstance().logInit();
	TA_Base_Core::CBoostLogger::getInstance().testBoostLog();
}

int Test_MarketData()
{
	logInit();

	//C:\LSL\SVNWork\CMS\PUBLIC\MarketData\sample
	std::string filename = "C:\\LSL\\SVNWork\\CMS\\PUBLIC\\MarketData\\sample\\sample.csv";
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


	//unsigned int ninstrumentID = 1;
	unsigned int ninstrumentID = 3620;
	BarCalculator barCalc(ninstrumentID);
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
		strTime_Value = getTimeString(Time_Value);
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



		barCalc.onMarketDataUpdate(marketDataTmp);

		//getPrice
		//LAST_TRADED_PRICE
		//LAST_TRADED_VOLUME

	}

	return 0;
}


int main(int argc, char * * argv)
{

	Test_MarketData();
	return 0;
}


#if 0
#include <cstdlib>
#include <QtGui/QApplication>
#include <QtSql/QtSql>
#include <QtCore/QChar>
#include <QtCore/QString>
#include <QtCore/QDir>
#include <QtCore/QVariant>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlDatabase>


bool createConnection()
{
	qDebug() << "Available drivers:";
	QStringList drivers = QSqlDatabase::drivers();
	foreach(QString driver, drivers)
	{
		qDebug() << "\t" << driver;
	}
	QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
	qDebug() << "MYSQL driver valid?" << db.isValid();

	return true;
}


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	if (!createConnection())
	{
		return 0;
	}
	return 0;
}
#endif

