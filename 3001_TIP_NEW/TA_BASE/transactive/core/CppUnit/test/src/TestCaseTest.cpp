#include <string>
#include "TestCaseTest.h"


// Initialises variables at the beginning of a test suite
void TestCaseTest::setUpSuite ( std::string const & INFile )
{
    // Use the INFile string to read in data,
    // this can change according the config file.
}


// Initialises variables at the end of a test suite
void TestCaseTest::tearDownSuite ()
{


}



void TestCaseTest::setUp ( const std::string & INFile , const std::string & OUTFile)
{

    // Use the INFile string to read in data,
    // this can change according the config file.
}



void TestCaseTest::tearDown ( const std::string & OUTFile )
{

    // Use the INFile string to read in data,
    // this can change according the config file.
}


// Test
void TestCaseTest::TestFailure ()
{
    Assert ( false );
}


// Test
void TestCaseTest::TestError ()
{
    Assert( false );
}


// Test
void TestCaseTest::TestSuccess ()
{

    Assert ( true );

}



// The suite method. Returns list of methods.

TestSuite* TestCaseTest::suite ()
{
    // Must have same name as test class.
    TestSuite * testSuite = new TestSuite( "TestCaseTest" );

    testSuite->addTest (
        new TestCaller <TestCaseTest> (
            "TestFailure", ADDR_OF( TestCaseTest::TestFailure ), this ) );
    testSuite->addTest (
        new TestCaller <TestCaseTest> (
            "TestError", ADDR_OF( TestCaseTest::TestError ), this ) );
    testSuite->addTest (
        new TestCaller <TestCaseTest> (
            "TestSuccess", ADDR_OF( TestCaseTest::TestSuccess ), this ) );


    return testSuite;
}
