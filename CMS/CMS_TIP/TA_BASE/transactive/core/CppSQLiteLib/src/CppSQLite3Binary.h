#ifndef __CLASS_CppSQLite3_BINARY_H__
#define __CLASS_CppSQLite3_BINARY_H__

#include "core/CppSQLiteLib/src/ProCommonDef.h"


NS_BEGIN(TA_Base_Core)


class CppSQLite3Binary
{
public:

    CppSQLite3Binary();

    ~CppSQLite3Binary();

    void setBinary(const unsigned char* pBuf, int nLen);
    void setEncoded(const unsigned char* pBuf);

    const unsigned char* getEncoded();
    const unsigned char* getBinary();

    int getBinaryLength();

    unsigned char* allocBuffer(int nLen);

    void clear();


public:
	////////////////////////////////////////////////////////////////////////////////
	// Prototypes for SQLite functions not included in SQLite DLL, but copied below
	// from SQLite encode.c
	////////////////////////////////////////////////////////////////////////////////
	int sqlite3_encode_binary(const unsigned char *in, int n, unsigned char *out);
	int sqlite3_decode_binary(const unsigned char *in, unsigned char *out);

	////////////////////////////////////////////////////////////////////////////////
private:

    unsigned char* mpBuf;
    int mnBinaryLen;
    int mnBufferLen;
    int mnEncodedLen;
    bool mbEncoded;
};



NS_END(TA_Base_Core)

#endif//__CLASS_CppSQLite3_BINARY_H__
