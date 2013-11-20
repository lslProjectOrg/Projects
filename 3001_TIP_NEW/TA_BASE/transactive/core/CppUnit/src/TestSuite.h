#ifndef TESTSUITE_H
#define TESTSUITE_H

/*
 * A TestSuite is a Composite of Tests.
 * It runs a collection of test cases. Here is an example.
 *
 * TestSuite *suite= new TestSuite();
 * suite->addTest(new TestCaller<MathTest> ("testAdd", testAdd));
 * suite->addTest(new TestCaller<MathTest> ("testDivideByZero", testDivideByZero));
 *
 * Note that TestSuites assume lifetime
 * control for any tests added to them.
 *
 * see Test and TestCaller
 */

#include <string>
#include <vector>

#include "Test.h"
#include "TestResult.h"

class TestSuite //: public Test
{
public:

    // Constructor
    TestSuite( const std::string & name );


    // Destructor
    virtual ~TestSuite();


    // Run a test
    void run( TestResult * result ,
              const std::string & INFile,
              const std::string & OUTFile,
              const int loops);


    // change name of the suite
    void changeName( const std::string & m );

    // Add a test to a suite
    void addTest( Test * test );


    // Returns the suite name.
    std::string toString() const;
    std::string name() const;


    // Returns config file name
    std::string file_name();


    // Returns all tests in a suite
    std::vector<Test *> getM_Tests() const;

    // Count tests
    int countTestCases();

    // Deletes contents
    virtual    void deleteContents();

private:

    // Vector of tests
    std::vector<Test *> m_tests;

    // Name of suite
    std::string m_name;

    // File name to log results to
    std::string f_name;
};

#endif
