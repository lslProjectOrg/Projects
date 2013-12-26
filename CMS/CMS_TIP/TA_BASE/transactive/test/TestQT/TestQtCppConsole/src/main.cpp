#include <iostream>
#include <fstream>
#include <list> 

#include <stdio.h>
#include <sys/timeb.h>
#include <stdlib.h>
#include <time.h>

#include <sstream>
#include <fstream>
#include <boost/foreach.hpp>
#include <boost/test/unit_test.hpp>
#include "MarketData.h"
#include "BarCalculator.h"



Bar minBar;
Bar fiveMinBar;
unsigned int timestamp = 1000 * 300;

void updateBar(int interval, const Bar &bar)
{
	if (interval == 60)
	{
		minBar = bar;
	}
	else if (interval == 300) 
	{
		fiveMinBar = bar;
	}
}


void Test_BarTest()
{
	{
		minBar.update(timestamp, 100, 5);
		//BOOST_CHECK(minBar.Open == 100);
		//BOOST_CHECK(minBar.Close == 100);
		//BOOST_CHECK(minBar.High == 100);
		//BOOST_CHECK(minBar.Low == 100);
		//BOOST_CHECK(minBar.Volume == 5);
		//BOOST_CHECK(minBar.Time == timestamp);

		minBar.update(timestamp, 99, 7);
		//BOOST_CHECK(minBar.Open == 100);
		//BOOST_CHECK(minBar.Close == 99);
		//BOOST_CHECK(minBar.High == 100);
		//BOOST_CHECK(minBar.Low == 99);
		//BOOST_CHECK(minBar.Volume == 12);
		//BOOST_CHECK(minBar.Time == timestamp);

		minBar.update(timestamp, 101, 4);
		//BOOST_CHECK(minBar.Open == 100);
		//BOOST_CHECK(minBar.Close == 101);
		//BOOST_CHECK(minBar.High == 101);
		//BOOST_CHECK(minBar.Low == 99);
		//BOOST_CHECK(minBar.Volume == 16);
		//BOOST_CHECK(minBar.Time == timestamp);

		timestamp += 60;
		minBar.update(timestamp, 102, 1);
		//BOOST_CHECK(minBar.Open == 102);
		//BOOST_CHECK(minBar.Close == 102);
		//BOOST_CHECK(minBar.High == 102);
		//BOOST_CHECK(minBar.Low == 102);
		//BOOST_CHECK(minBar.Volume == 1);
		//BOOST_CHECK(minBar.Time == timestamp);
	}

	{
		minBar.update(0, 0, 0);
		BarCalculator barCalc(1);
		barCalc.onNewBar = updateBar;
		barCalc.onBarUpdate = updateBar;
		barCalc.addBar(60); // One minute
		barCalc.addBar(300); // Five Minute

		barCalc.onMarketDataUpdate(timestamp, 100, 5);
		//BOOST_CHECK(minBar.Open == 100);
		//BOOST_CHECK(minBar.Close == 100);
		//BOOST_CHECK(minBar.High == 100);
		//BOOST_CHECK(minBar.Low == 100);
		//BOOST_CHECK(minBar.Volume == 5);
		//BOOST_CHECK(minBar.Time == timestamp);

		barCalc.onMarketDataUpdate(timestamp, 99, 7);
		//BOOST_CHECK(minBar.Open == 100);
		//BOOST_CHECK(minBar.Close == 99);
		//BOOST_CHECK(minBar.High == 100);
		//BOOST_CHECK(minBar.Low == 99);
		//BOOST_CHECK(minBar.Volume == 12);
		//BOOST_CHECK(minBar.Time == timestamp);

		barCalc.onMarketDataUpdate(timestamp, 101, 4);
		//BOOST_CHECK(minBar.Open == 100);
		//BOOST_CHECK(minBar.Close == 101);
		//BOOST_CHECK(minBar.High == 101);
		//BOOST_CHECK(minBar.Low == 99);
		//BOOST_CHECK(minBar.Volume == 16);
		//BOOST_CHECK(minBar.Time == timestamp);

		timestamp += 60;
		barCalc.onMarketDataUpdate(timestamp, 102, 1);
		//BOOST_CHECK(minBar.Open == 102);
		//BOOST_CHECK(minBar.Close == 102);
		//BOOST_CHECK(minBar.High == 102);
		//BOOST_CHECK(minBar.Low == 102);
		//BOOST_CHECK(minBar.Volume == 1);
		//BOOST_CHECK(minBar.Time == timestamp);

		//BOOST_CHECK(fiveMinBar.Open == 100);
		//BOOST_CHECK(fiveMinBar.Close == 102);
		//BOOST_CHECK(fiveMinBar.High == 102);
		//BOOST_CHECK(fiveMinBar.Low == 99);
		//BOOST_CHECK(fiveMinBar.Volume == 17);
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

int main(int argc, char * * argv)
{
	std::string filename = "G:\\LSL\\LSL_Code\\Svn_Work\\PUBLIC\\MarketData\\sample\\sample.csv";
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
	barCalc.onNewBar = updateBar;
	barCalc.onBarUpdate = updateBar;

	minBar.update(0, 0, 0);
	fiveMinBar.update(0, 0 ,0);

	barCalc.addBar(60); // One minute
	barCalc.addBar(300); // Five Minute

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

		barCalc.onMarketDataUpdate(marketDataTmp);

		//getPrice
		//LAST_TRADED_PRICE
		//LAST_TRADED_VOLUME

	}

	Test_BarTest();
	return 0;
}


