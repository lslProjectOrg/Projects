#include "SQLVarParms.h"

NS_BEGIN(TA_Base_Core)


SQLVarParms::SQLVarParms()
{

}

SQLVarParms::~SQLVarParms()
{

}


SQLVarParms SQLVarParms::operator +(const SQLVarParms& other) const
{
	if (other.size() <= size()) 
	{
		return *this;
	}

	SQLVarParms New = *this;

	for (size_t nIndex = size(); nIndex < other.size(); nIndex++) 
	{
		New.push_back(other[nIndex]);
	}

	return New;
}

void SQLVarParms::clear()
{
	erase(begin(), end());
}

SQLTypeAdapter& SQLVarParms::operator [](size_type n)
{
	if (n >= size()) {
		insert(end(), (n + 1) - size(), "");
	}
	return std::vector<SQLTypeAdapter>::operator [](n);
}

const SQLTypeAdapter& SQLVarParms::operator [](size_type n) const
{ 
	return std::vector<SQLTypeAdapter>::operator [](n); 
}

SQLVarParms& SQLVarParms::operator <<(const SQLTypeAdapter& str)
{
	push_back(str);
	return *this;
}
SQLVarParms& SQLVarParms::operator +=(const SQLTypeAdapter& str)
{
	push_back(str);
	return *this;
}

NS_END(TA_Base_Core)


