#include <boost/test/unit_test.hpp>
//#include "../MarketData/BarCalculator.h"
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

BOOST_AUTO_TEST_CASE(BarTest)
{
	{
		minBar.update(timestamp, 100, 5);
		BOOST_CHECK(minBar.Open == 100);
		BOOST_CHECK(minBar.Close == 100);
		BOOST_CHECK(minBar.High == 100);
		BOOST_CHECK(minBar.Low == 100);
		BOOST_CHECK(minBar.Volume == 5);
		BOOST_CHECK(minBar.Time == timestamp);

		minBar.update(timestamp, 99, 7);
		BOOST_CHECK(minBar.Open == 100);
		BOOST_CHECK(minBar.Close == 99);
		BOOST_CHECK(minBar.High == 100);
		BOOST_CHECK(minBar.Low == 99);
		BOOST_CHECK(minBar.Volume == 12);
		BOOST_CHECK(minBar.Time == timestamp);

		minBar.update(timestamp, 101, 4);
		BOOST_CHECK(minBar.Open == 100);
		BOOST_CHECK(minBar.Close == 101);
		BOOST_CHECK(minBar.High == 101);
		BOOST_CHECK(minBar.Low == 99);
		BOOST_CHECK(minBar.Volume == 16);
		BOOST_CHECK(minBar.Time == timestamp);

		timestamp += 60;
		minBar.update(timestamp, 102, 1);
		BOOST_CHECK(minBar.Open == 102);
		BOOST_CHECK(minBar.Close == 102);
		BOOST_CHECK(minBar.High == 102);
		BOOST_CHECK(minBar.Low == 102);
		BOOST_CHECK(minBar.Volume == 1);
		BOOST_CHECK(minBar.Time == timestamp);
	}

	{
		minBar.update(0, 0, 0);
		BarCalculator barCalc(1);
		barCalc.onNewBar = updateBar;
		barCalc.onBarUpdate = updateBar;
		barCalc.addBar(60); // One minute
		barCalc.addBar(300); // Five Minute

		barCalc.onMarketDataUpdate(timestamp, 100, 5);
		BOOST_CHECK(minBar.Open == 100);
		BOOST_CHECK(minBar.Close == 100);
		BOOST_CHECK(minBar.High == 100);
		BOOST_CHECK(minBar.Low == 100);
		BOOST_CHECK(minBar.Volume == 5);
		BOOST_CHECK(minBar.Time == timestamp);

		barCalc.onMarketDataUpdate(timestamp, 99, 7);
		BOOST_CHECK(minBar.Open == 100);
		BOOST_CHECK(minBar.Close == 99);
		BOOST_CHECK(minBar.High == 100);
		BOOST_CHECK(minBar.Low == 99);
		BOOST_CHECK(minBar.Volume == 12);
		BOOST_CHECK(minBar.Time == timestamp);

		barCalc.onMarketDataUpdate(timestamp, 101, 4);
		BOOST_CHECK(minBar.Open == 100);
		BOOST_CHECK(minBar.Close == 101);
		BOOST_CHECK(minBar.High == 101);
		BOOST_CHECK(minBar.Low == 99);
		BOOST_CHECK(minBar.Volume == 16);
		BOOST_CHECK(minBar.Time == timestamp);

		timestamp += 60;
		barCalc.onMarketDataUpdate(timestamp, 102, 1);
		BOOST_CHECK(minBar.Open == 102);
		BOOST_CHECK(minBar.Close == 102);
		BOOST_CHECK(minBar.High == 102);
		BOOST_CHECK(minBar.Low == 102);
		BOOST_CHECK(minBar.Volume == 1);
		BOOST_CHECK(minBar.Time == timestamp);

		BOOST_CHECK(fiveMinBar.Open == 100);
		BOOST_CHECK(fiveMinBar.Close == 102);
		BOOST_CHECK(fiveMinBar.High == 102);
		BOOST_CHECK(fiveMinBar.Low == 99);
		BOOST_CHECK(fiveMinBar.Volume == 17);
	}
}