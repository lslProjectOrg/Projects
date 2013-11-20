#ifndef    __DEF_CLASS_TESTCLASS___FOR_SAMPLE_HH__
#define    __DEF_CLASS_TESTCLASS___FOR_SAMPLE_HH__

#include "CommonDef.h"
#include "CommonData.h"

#include "TestCase.h"
#include "TestFramework.h"



class CTestClassForSample : public TestCase  
{
public:
	CTestClassForSample ( const std::string & name ) : TestCase ( name )
	{
	}

	virtual ~CTestClassForSample()
	{
	}

 public:
    TestSuite * suite();    
    void setUp( const std::string & INFile , const std::string & OUTFile );
    void tearDown( const std::string & INFile );
    virtual void setUpSuite( const std::string & inFile );
    void tearDownSuite();



private:	
	void RunThisTestCase();
	



};

#endif // !defined(__DEF_CLASS_TESTCLASS___FOR_SAMPLE_HH__)

