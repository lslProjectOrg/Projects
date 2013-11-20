#ifndef    __DEF_CLASS_TESTCLASS_HH__
#define    __DEF_CLASS_TESTCLASS_HH__



#include <string>
#include <vector>

#include "ocl.h"


#ifdef __WIN32__
typedef void* threadrounte;
#else
typedef void* threadrounte;
#endif

class CTestClass 
{
public:
	CTestClass ( const std::string & strTestClassName );
	virtual ~CTestClass();

public:	
	void RunTestCase();
	
protected:
	static threadrounte runThread(void* pParam);


private:	
	void fillDataBuffer(ocl::OraConnection *pconnection, ocl::OraQuery* pQuery, int numColumns, long lMaxRows );
	void fun_testclass_fun_two();
	void fetchInitial(ocl::OraConnection *pconnection, ocl::OraQuery* pQuery, int numColumns, long lMaxRows );
	void exec(ocl::OraConnection *pconnection, const std::string& strOracleSQL, bool isQuery);
};
#endif // !defined(__DEF_CLASS_TESTCLASS_HH__)
