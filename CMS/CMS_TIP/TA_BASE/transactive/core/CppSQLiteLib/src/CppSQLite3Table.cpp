#include "CppSQLite3Table.h"
#include "core/CppSQLiteLib/src/CppSQLite3Execption.h"

NS_BEGIN(TA_Base_Core)

////////////////////////////////////////////////////////////////////////////////

CppSQLite3Table::CppSQLite3Table()
{
	mpaszResults = 0;
	mnRows = 0;
	mnCols = 0;
	mnCurrentRow = 0;
}


CppSQLite3Table::CppSQLite3Table(const CppSQLite3Table& rTable)
{
	mpaszResults = rTable.mpaszResults;
	// Only one object can own the results
	const_cast<CppSQLite3Table&>(rTable).mpaszResults = 0;
	mnRows = rTable.mnRows;
	mnCols = rTable.mnCols;
	mnCurrentRow = rTable.mnCurrentRow;
}


CppSQLite3Table::CppSQLite3Table(char** paszResults, int nRows, int nCols)
{
	mpaszResults = paszResults;
	mnRows = nRows;
	mnCols = nCols;
	mnCurrentRow = 0;
}


CppSQLite3Table::~CppSQLite3Table()
{
	try
	{
		finalize();
	}
	catch (...)
	{
	}
}


CppSQLite3Table& CppSQLite3Table::operator=(const CppSQLite3Table& rTable)
{
	try
	{
		finalize();
	}
	catch (...)
	{
	}
	mpaszResults = rTable.mpaszResults;
	// Only one object can own the results
	const_cast<CppSQLite3Table&>(rTable).mpaszResults = 0;
	mnRows = rTable.mnRows;
	mnCols = rTable.mnCols;
	mnCurrentRow = rTable.mnCurrentRow;
	return *this;
}


void CppSQLite3Table::finalize()
{
	if (mpaszResults)
	{
		sqlite3_free_table(mpaszResults);
		mpaszResults = 0;
	}
}


int CppSQLite3Table::numFields()
{
	checkResults();
	return mnCols;
}


int CppSQLite3Table::numRows()
{
	checkResults();
	return mnRows;
}


const char* CppSQLite3Table::fieldValue(int nField)
{
	checkResults();

	if (nField < 0 || nField > mnCols-1)
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR,
			"Invalid field index requested",
			DONT_DELETE_MSG);
	}

	int nIndex = (mnCurrentRow*mnCols) + mnCols + nField;
	return mpaszResults[nIndex];
}


const char* CppSQLite3Table::fieldValue(const char* szField)
{
	checkResults();

	if (szField)
	{
		for (int nField = 0; nField < mnCols; nField++)
		{
			if (strcmp(szField, mpaszResults[nField]) == 0)
			{
				int nIndex = (mnCurrentRow*mnCols) + mnCols + nField;
				return mpaszResults[nIndex];
			}
		}
	}

	throw CppSQLite3Exception(CPPSQLITE_ERROR,
		"Invalid field name requested",
		DONT_DELETE_MSG);
}


int CppSQLite3Table::getIntField(int nField, int nNullValue/*=0*/)
{
	if (fieldIsNull(nField))
	{
		return nNullValue;
	}
	else
	{
		return atoi(fieldValue(nField));
	}
}


int CppSQLite3Table::getIntField(const char* szField, int nNullValue/*=0*/)
{
	if (fieldIsNull(szField))
	{
		return nNullValue;
	}
	else
	{
		return atoi(fieldValue(szField));
	}
}


double CppSQLite3Table::getFloatField(int nField, double fNullValue/*=0.0*/)
{
	if (fieldIsNull(nField))
	{
		return fNullValue;
	}
	else
	{
		return atof(fieldValue(nField));
	}
}


double CppSQLite3Table::getFloatField(const char* szField, double fNullValue/*=0.0*/)
{
	if (fieldIsNull(szField))
	{
		return fNullValue;
	}
	else
	{
		return atof(fieldValue(szField));
	}
}


const char* CppSQLite3Table::getStringField(int nField, const char* szNullValue/*=""*/)
{
	if (fieldIsNull(nField))
	{
		return szNullValue;
	}
	else
	{
		return fieldValue(nField);
	}
}


const char* CppSQLite3Table::getStringField(const char* szField, const char* szNullValue/*=""*/)
{
	if (fieldIsNull(szField))
	{
		return szNullValue;
	}
	else
	{
		return fieldValue(szField);
	}
}


bool CppSQLite3Table::fieldIsNull(int nField)
{
	checkResults();
	return (fieldValue(nField) == 0);
}


bool CppSQLite3Table::fieldIsNull(const char* szField)
{
	checkResults();
	return (fieldValue(szField) == 0);
}


const char* CppSQLite3Table::fieldName(int nCol)
{
	checkResults();

	if (nCol < 0 || nCol > mnCols-1)
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR,
			"Invalid field index requested",
			DONT_DELETE_MSG);
	}

	return mpaszResults[nCol];
}


void CppSQLite3Table::setRow(int nRow)
{
	checkResults();

	if (nRow < 0 || nRow > mnRows-1)
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR,
			"Invalid row index requested",
			DONT_DELETE_MSG);
	}

	mnCurrentRow = nRow;
}


void CppSQLite3Table::checkResults()
{
	if (mpaszResults == 0)
	{
		throw CppSQLite3Exception(CPPSQLITE_ERROR,
			"Null Results pointer",
			DONT_DELETE_MSG);
	}
}



NS_END(TA_Base_Core)



