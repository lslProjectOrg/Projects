#pragma once
#include <map>
#include <memory>
#include "MarketData.h"
#include <boost/function.hpp>

class ErrorDetector;

class CTS_PRICE Tick
{
public:
	Tick();
	unsigned int Time;

	float BidPx;
	float AskPx;
	float LastPx;

	int BidVol;
	int AskVol;
	int LastVol;
};

class CTS_PRICE Bar
{
public:
	Bar();
	float Open;
	float Close;
	float High;
	float Low;
	int Volume;
	unsigned int Time;

	void update(unsigned int timestamp, float price, int volume);
};

class CTS_PRICE BarCalculator
{
public:
	BarCalculator(unsigned int instrumentID, bool calcLastVol = true);
	~BarCalculator(void);

	unsigned int getInstrumentID() const;

	bool addBar(int interval /*Seconds*/);

	void onMarketDataUpdate(const MarketData &marketData);
	void onMarketDataUpdate(unsigned int timestamp, float price, int volume);

	boost::function<void (int, const Bar &) > onNewBar;
	boost::function<void (int, const Bar &) > onBarUpdate;
private:
	bool m_calcLastVol;
	bool m_changedFlag;
	float m_lastPrice;
	int m_totalVolume;
	unsigned int m_instrumentID;
	std::map<int, Bar> m_bars;
	std::auto_ptr<ErrorDetector> m_errorDetector;
};