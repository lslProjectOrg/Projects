#ifndef TESTFAILURE_H
#define TESTFAILURE_H

/*
 * A TestFailure collects a failed test together with
 * the caught exception.
 *
 * TestFailure assumes lifetime control for any exception
 * passed to it.  The lifetime of tests is handled by
 * their TestSuite (if they have been added to one) or
 * whomever creates them.
 *
 * see TestResult
 * see TestSuite
 *
 */

#if defined( WIN32 )
#pragma warning(disable:4786)
#endif // defined)( WIN32 )

#include <string>
#include "Test.h"
#include "CppUnitException.h"

class TestFailure
{
public:

    // Constructor
    TestFailure ( Test * failedTest, CppUnitException * thrownException );

    // Destructor
    virtual ~TestFailure ();

    // Returns failed test
    Test *  failedTest();

    // Returns exception
    CppUnitException * thrownException();

    // Returns string
    std::string toString();

protected:

    // Failed test
    Test * m_failedTest;


    // Exception thrown
    CppUnitException * m_thrownException;
};

#endif
