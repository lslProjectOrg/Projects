#ifndef __CLASS_CppSQLite3_TABLE_H__
#define __CLASS_CppSQLite3_TABLE_H__


#include "core/CppSQLiteLib/src/CppSQLiteLibCommonData.h"


NS_BEGIN(TA_Base_Core)

class CppSQLite3Table
{
public:

    CppSQLite3Table();

    CppSQLite3Table(const CppSQLite3Table& rTable);

    CppSQLite3Table(char** paszResults, int nRows, int nCols);

    virtual ~CppSQLite3Table();

    CppSQLite3Table& operator=(const CppSQLite3Table& rTable);

    int numFields();

    int numRows();

    const char* fieldName(int nCol);

    const char* fieldValue(int nField);
    const char* fieldValue(const char* szField);

    int getIntField(int nField, int nNullValue=0);
    int getIntField(const char* szField, int nNullValue=0);

    double getFloatField(int nField, double fNullValue=0.0);
    double getFloatField(const char* szField, double fNullValue=0.0);

    const char* getStringField(int nField, const char* szNullValue="");
    const char* getStringField(const char* szField, const char* szNullValue="");

    bool fieldIsNull(int nField);
    bool fieldIsNull(const char* szField);

    void setRow(int nRow);

    void finalize();

private:

    void checkResults();

    int mnCols;
    int mnRows;
    int mnCurrentRow;
    char** mpaszResults;
};


NS_END(TA_Base_Core)

#endif  //__CLASS_CppSQLite3_TABLE_H__
