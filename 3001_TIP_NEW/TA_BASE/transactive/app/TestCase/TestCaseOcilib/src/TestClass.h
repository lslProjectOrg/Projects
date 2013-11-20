#ifndef    __DEF_CLASS_TESTCLASS_HH__
#define    __DEF_CLASS_TESTCLASS_HH__



#include <string>
#include <vector>

#include "ocilib.h"

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
	static void worker(OCI_Thread *thread, void *data);


private:	
	void fun_testclass_fun_one();
	void fun_testclass_fun_two();
	void fun_testclass_fun_OCILIB_ConPool();



};

#endif // !defined(__DEF_CLASS_TESTCLASS_HH__)
