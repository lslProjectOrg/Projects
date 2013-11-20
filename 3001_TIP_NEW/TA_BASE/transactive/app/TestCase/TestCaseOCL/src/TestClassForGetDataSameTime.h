#ifndef    __DEF_CLASS_TESTCLASS___FOR_GETDATASAMETIME_HH__
#define    __DEF_CLASS_TESTCLASS___FOR_GETDATASAMETIME_HH__

#include "CommonDef.h"
#include "CommonData.h"

#include "core/CppUnit/src/TestCase.h"
#include "core/CppUnit/src/TestFramework.h"

#include "core/synchronisation/src/ReEntrantThreadLockable.h"
#include "core/ocl/src/ocl.h"


#ifdef __WIN32__
typedef void* threadrounte;
#else
typedef void* threadrounte;
#endif




class CTestClassForGetDataSameTime : public TestCase  
{
public:
	CTestClassForGetDataSameTime ( const std::string & name );
	
	virtual ~CTestClassForGetDataSameTime();

 public:
    TestSuite * suite();    
    void setUp( const std::string & INFile , const std::string & OUTFile );
    void tearDown( const std::string & INFile );
    virtual void setUpSuite( const std::string & inFile );
    void tearDownSuite();



private:	
	void RunThisTestCase();
	

protected:
	static threadrounte runThread(void* pParam);
	static threadrounte runThread_2(void* pParam);

private:
	TA_Base_Core::ReEntrantThreadLockable m_lockConnection;	//TA_THREADGUARD(m_lock);
	ocl::OraConnection *m_pConnection;

	int              m_nThread1Thread1Conditon;

	std::string m_connectionStr;
	std::string m_databaseName;
	std::string m_userName;
	std::string m_password;
	std::string m_hostName;

private:
	void CTestClassForGetDataSameTime::_InitTestData();
	void CTestClassForGetDataSameTime::_UnInitTestData();
	void CTestClassForGetDataSameTime::_Test_Exec_GetData_1();

private:

	void CTestClassForGetDataSameTime::_GetColumnNames(std::vector<std::string>& lstColumns);
	void exec(ocl::OraConnection* pConnection, const std::string& strSql, bool isQuery, ocl::OraQuery** ppQueryGet);
	void fetchInitial(ocl::OraConnection* pconnection, ocl::OraQuery* pQuery, int numColumns, long lMaxRows );
	void fillDataBuffer(ocl::OraConnection* pconnection, ocl::OraQuery* pQuery, int numColumns, long lMaxRows );
	void _InitTestTable();
	void _UnInitTestTable();

};

#endif // !defined(__DEF_CLASS_TESTCLASS___FOR_GETDATASAMETIME_HH__)

