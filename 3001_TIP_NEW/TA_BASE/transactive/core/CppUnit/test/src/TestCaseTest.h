#ifndef CPP_UNIT_TESTCASETEST_H
#define CPP_UNIT_TESTCASETEST_H

#include <string>
#include "TestCase.h"
#include "TestFramework.h"

class TestCaseTest : public TestCase
{
public:

    TestSuite * suite();

    TestCaseTest ( const std::string & name )
        : TestCase ( name )
        {
        }

    ~TestCaseTest()
        {
        }

    void TestAnother();
    void TestMethod();
    void TestFailure();
    void TestError();
    void TestSuccess();

    virtual void setUp( const std::string & INFile, const std::string & OUTFile );
    virtual void tearDown( const std::string & OUTFile );

    virtual void setUpSuite ( const std::string & INFile );
    virtual void tearDownSuite();


};


#endif

