#include "TestCaseExample.h"


// A test
void TestCaseExample::TestExample ()
{

    Assert( 1 == 0 );
    Assert( 1 == 1 );
}



// A test
void TestCaseExample::TestAnotherExample ()
{
    Assert( 1 == 2 );
    Assert( 1 == 2 );
    Assert( 1 == 2 );
    Assert( 1 == 2 );
    Assert( 1 == 2 );
}

// A test
void TestCaseExample::setUpSuite ( const std::string & INFile )
{

    // Use the INFile CString to read in data,
    // this can change according the config file.

    m_value1 = 1;
    m_value2 = 2;


}



// A test
void TestCaseExample::tearDownSuite ()
{

}




// Initialises variables
void TestCaseExample::setUp (
    const std::string & INFile ,
    const std::string & OUTFile)
{
    m_value1 = "111";
    m_value2 = "999";

    //Use the INFile CString to read in data,
    //this can change according the config file.
}



// destroys variables
void TestCaseExample::tearDown ( const std::string & OUTFile )
{
    // Use the string to open and write to a file

}



// Test
void TestCaseExample::TestAdd ()
{

    std::string result = "Test";
    Assert ( result == "Test" );

    throw std::exception();

}



// Test
void TestCaseExample::TestAdd2 ()
{

    std::string actual        = "102";
    std::string expected    = "200";

    AssertEquals ( actual , expected );

}



// Test
void TestCaseExample::TestDivideByZero ()
{
  Assert( false );
}



// Test
void TestCaseExample::TestEquals ()
{

    int    test        = 100;
    int anotherTest = 101;


    AssertEquals    ( test, test );
    AssertEquals    ( anotherTest, anotherTest );


    Assert            ( test == anotherTest );
    AssertEquals    ( test, anotherTest );




}




// Suite method, returns all methods.
TestSuite * TestCaseExample::suite ()
{
    TestCaseExample x ( "" );

    // Must have same name as test class.
    TestSuite * testSuite = new TestSuite( "TestCaseExample" );



    testSuite->addTest(
        new TestCaller< TestCaseExample >(
            std::string( "TestExample" ),
            ADDR_OF( TestCaseExample::TestExample ), this ) );
    testSuite->addTest( new TestCaller< TestCaseExample >(
        "TestAnotherExample", ADDR_OF( TestCaseExample::TestAnotherExample ), this ) );
    testSuite->addTest( new TestCaller< TestCaseExample >(
        "TestAdd", ADDR_OF( TestCaseExample::TestAdd ), this ) );
    testSuite->addTest( new TestCaller< TestCaseExample >(
        "TestAdd2", ADDR_OF( TestCaseExample::TestAdd2 ), this ) );
    testSuite->addTest( new TestCaller< TestCaseExample >(
        "TestDivideByZero", ADDR_OF( TestCaseExample::TestDivideByZero ), this ) );
    testSuite->addTest( new TestCaller< TestCaseExample >(
        "TestEquals", ADDR_OF( TestCaseExample::TestEquals ), this ) );




    return testSuite;
}
