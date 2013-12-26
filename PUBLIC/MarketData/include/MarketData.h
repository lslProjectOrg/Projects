#pragma  once
#include "MarketDataAPI.h"

class MarketDataImp;
#include <iostream>

class CTS_PRICE MarketData
{
public:
	enum MarketStatus
	{
		PRE_OPEN = 48,
		OPENED,
		PRE_CLOSE,
		CLOSED,
		SUSPENDED,
	};

	enum PriceType
	{
		OPENNING_PRICE = 0,
		SESSION_HIGH,
		SESSION_LOW,
		REFERENCE_PRICE,
		YESTERDAY_SETTLMENT_PRICE,
		LAST_TRADED_PRICE,
		SETTLEMENT_PRICE,
	};

	enum VolumeType
	{
		OPEN_INTEREST = 0,
		OPENING_VOLUME,
		LAST_TRADED_VOLUME,
		TOTAL_TRADED_VOLUME,
	};

	MarketData();
	MarketData(unsigned int secID);
	~MarketData();

	MarketData(const MarketData &);
	MarketData & operator= (const MarketData &);

	void setSecurityID(unsigned int secID);
	unsigned int getSecurityID() const;

	int getMarketStatus() const;
	unsigned int getTime() const;

	int getVolume(int type) const;
	int getBidVol(int level) const;
	int getAskVol(int level) const;

	float getPrice(int type) const;
	float getBidPx(int level) const;
	float getAskPx(int level) const;

	unsigned int getDataBits() const;
	unsigned int getChangeBits() const;

	MarketDataImp * imp;
};

CTS_PRICE std::istream & operator >>(std::istream &in, MarketData &price);
CTS_PRICE std::ostream & operator <<(std::ostream &out, const MarketData &price);
