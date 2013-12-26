#pragma once
#include <string>
#include <boost/function.hpp>

#include "MarketData.h"

class MarketDataFeedImp;

class CTS_PRICE MarketDataFeed
{
	MarketDataFeedImp * feed;
public:
	MarketDataFeed();
	~MarketDataFeed(void);

	void connect(const std::string &hubAddress);
	void disconnect();

	boost::function<void (const MarketData &)> onMarketDataUpate;
};
