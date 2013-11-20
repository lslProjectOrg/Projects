#ifndef CPP_UNIT_TESTCASEEXAMPLE_H
#define CPP_UNIT_TESTCASEEXAMPLE_H

#include "TestCase.h"
#include "TestFramework.h"



/*
 * A test case that uses the framework
 */



class TestCaseExample : public TestCase
{


public:



    virtual ~TestCaseExample(){}

    TestCaseExample ( const std::string & name ) : TestCase ( name ){}


    TestSuite * suite();                                // Reuturns all the methods packaged into
    // the TestSuite object.

    // Initialises variables before each method is run.
    void setUp( const std::string & INFile , const std::string & OUTFile);

    void tearDown( const std::string & INFile );    // Destroys variables after each method is run.

    virtual void setUpSuite( const std::string & inFile );
    void tearDownSuite();                            // Destroys variables after a suite of tests are run.



    void TestExample();
    void TestAnotherExample();
    void TestAdd();
    void TestAdd2();                                // Tests
    void TestDivideByZero();
    void TestEquals();


protected:

    std::string m_value1;
    std::string m_value2;

private:

};


#endif
