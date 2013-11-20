#ifndef TEST_H
#define TEST_H

/*
 * A Test can be run and collect its results.
 * See TestResult.
 */

#if defined( WIN32 )
#pragma warning(disable:4786)
#endif // defined( WIN32 )

#include <string>

class TestResult;

class Test
{
public:
    virtual ~Test()
    {
    }

    virtual int countTestCases() = 0;
    virtual std::string toString() = 0;
    virtual std::string name() = 0;
    virtual void run( TestResult * result,
                      const std::string & INFile,
                      const std::string & OUTFile,
                      const int loops) = 0;

    // Sets up any variables b4 each suite of tests.
    virtual void setUpSuite( const std::string & inFile ){}

    // Destroys variables after each suite of tests.
    virtual void tearDownSuite(){}


};

#endif

