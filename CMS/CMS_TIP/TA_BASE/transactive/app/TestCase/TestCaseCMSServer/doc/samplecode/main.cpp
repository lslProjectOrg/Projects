#include <iostream>
#include "Security.h"
#include <boost/foreach.hpp>

int main(int argc, char * * argv)
{
	const Security::Table & securities =  Security::loadFromDatabase("");
	BOOST_FOREACH(const Security::Table::value_type &entry, securities)
	{
		const Security::Ptr security = entry.second;
		security->saveMetaData();
	}
	return 0;
}
