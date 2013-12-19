#ifndef SQLVARPARMS_H
#define SQLVARPARMS_H
#include <vector>
#include "SQLTypeAdapter.h"

NS_BEGIN(TA_Base_Core)

class SQLVarParms : public std::vector<SQLTypeAdapter>
{
public:
	SQLVarParms();
	~SQLVarParms();
	void clear();
	SQLTypeAdapter& operator [](size_type n);
	const SQLTypeAdapter& operator [](size_type n) const;
	SQLVarParms& operator <<(const SQLTypeAdapter& str);
	SQLVarParms& operator +=(const SQLTypeAdapter& str);
	SQLVarParms operator +(const SQLVarParms& other) const;
};

NS_END(TA_Base_Core)

#endif  /// SQLVARPARMS_H




