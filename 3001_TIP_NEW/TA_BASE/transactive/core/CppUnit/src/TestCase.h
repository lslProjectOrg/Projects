#ifndef TESTCASE_H
#define TESTCASE_H

/*
 * A test case defines the fixture to run multiple tests. To define a test case
 * 1) implement a subclass of TestCase
 * 2) define instance variables that store the state of the fixture
 * 3) initialize the fixture state by overriding setUp
 * 4) clean-up after a test by overriding tearDown.
 *
 * Each test runs in its own fixture so there
 * can be no side effects among test runs.
 * Here is an example:
 *
 * class MathTest : public TestCase {
 *     protected: int m_value1;
 *     protected: int m_value2;
 *
 *     public: MathTest (string name)
 *                 : TestCase (name) {
 *     }
 *
 *     protected: void setUp () {
 *         m_value1 = 2;
 *         m_value2 = 3;
 *     }
 * }
 *
 *
 * For each test implement a method which interacts
 * with the fixture. Verify the expected results with assertions specified
 * by calling assert on the expression you want to test:
 *
 *    protected: void testAdd () {
 *        int result = value1 + value2;
 *        assert (result == 5);
 *    }
 *
 * Once the methods are defined you can run them. To do this, use
 * a TestCaller.
 *
 * Test *test = new TestCaller<MathTest>("testAdd", MathTest::testAdd);
 * test->run ();
 *
 *
 * The tests to be run can be collected into a TestSuite. CppUnit provides
 * different test runners which can run a test suite and collect the results.
 * The test runners expect a static method suite as the entry
 * point to get a test to run.
 *
 * public: static MathTest::suite () {
 *      TestSuite *suiteOfTests = new TestSuite;
 *      suiteOfTests->addTest(new TestCaller<MathTest>("testAdd", testAdd));
 *      suiteOfTests->addTest(new TestCaller<MathTest>("testDivideByZero", testDivideByZero));
 *      return suiteOfTests;
 *  }
 *
 * Note that the caller of suite assumes lifetime control
 * for the returned suite.
 *
 * see TestResult, TestSuite and TestCaller
 *
 */

#include <sstream>
#include "Test.h"
#include "TestFrameworkBase.h"


class  TestCase : public Test
{
public:

    // Constructor
    TestCase( const std::string & Name );
    virtual ~TestCase();


    // Runs a test
    virtual void run( TestResult * result,
                      const std::string & INFile,
                      const std::string & OUTFile ,
                      const int loops);

    virtual TestResult * run();


    // Counts tests
    virtual int countTestCases();

    std::string name();                                         // Name of test case
    std::string toString();

    // Sets up any variables b4 each test method
    virtual void setUp( const std::string & INFile , const std::string & OUTFile) = 0;

    // Destroys variables after each test method
    virtual void tearDown( const std::string & OUTFile ) = 0;

    // Sets up any variables b4 each suite of tests.
    virtual void setUpSuite( const std::string & inFile ) = 0;

    // Destroys variables after each suite of tests.
    virtual void tearDownSuite() = 0;

    // this is overridden
    virtual void runTest();
    TestResult * defaultResult();


    // Call this when an Assert fails.
    void assertImplementation ( const bool condition,
                                const std::string & conditionExpression = "",
                                const long lineNumber = TestFrameworkBase::UNKNOWNLINENUMBER,
                                const std::string & fileName = TestFrameworkBase::UNKNOWNFILENAME );

    inline bool didRun() const
        {
            return m_wasRun;
        }

protected:




    // Checks if expected and acuals values are equal.
    template < class T > void assertEquals ( const T & expected,
                                             const T & actual,
                                             const long lineNumber,
                                             const std::string & fileName,
                                             const std::string & ex,
                                             const std::string & ac)
        {
            std::stringstream ss;
            ss << " ("
               << ex
               << ","
               << ac
               << ")  Was passed.";

            std::string message = ss.str();

            if ( ! (expected == actual ) )
            {
                assertImplementation ( false, message, lineNumber, fileName );
            }
        }




    // Check for a failed range check
    template <class T> void assertRange( const T & expected,
                                         const T & actual,
                                         const T & range,
                                         const long lineNumber,
                                         const std::string & fileName )
        {
            std::stringstream ss;
            ss << " ("
               /*<< ex*/
               << " , "
               /*<< ac*/
               << " , "
               << ")  Was passed.  Results are not in the range of "
               << range;

            std::string message = ss.str();

            if ( fabs ( expected - actual ) > range )
            {
                assertImplementation ( false, message, lineNumber, fileName );
            }
        }




private:

    // Name of test class
    const std::string m_name;


    // Converts an int into an std::String
    std::string num_String ( const int number ) const;

    bool m_wasRun;

};



// A set of macros which allow us to get the line number
// and file name at the point of an error.
// Just goes to show that preprocessors do have some
// redeeming qualities.

#ifdef CPP_UNIT_SOURCEANNOT

    #define Assert(condition) \
    (this->assertImplementation ((condition),(#condition), \
        __LINE__, __FILE__))

#else

    #define Assert(condition) (this->assertImplementation ((condition),(#condition), __LINE__, __FILE__))

#endif


#define AssertRange(expected,actual,delta) (this->assertRange ((expected), (actual),(delta),__LINE__,__FILE__))

#define AssertEquals(expected,actual) (this->assertEquals ((expected), (actual),__LINE__,__FILE__,(#expected),(#actual)))


#endif
