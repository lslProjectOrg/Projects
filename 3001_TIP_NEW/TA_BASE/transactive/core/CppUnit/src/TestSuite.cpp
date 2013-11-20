#include "TestSuite.h"
#include "TestResult.h"
#include "ConfigData.h"
#include "TestCaller.h"

// Default constructor
TestSuite::TestSuite ( const std::string  & name )
    : m_name (name)
{
}

// Destructor
TestSuite::~TestSuite ()
{
}
// Deletes all tests in the suite.
void TestSuite::deleteContents ()
{
    for ( std::vector<Test *>::iterator iter = m_tests.begin ();
          iter != m_tests.end ();
          iter++)
    {
        delete *iter;
    }
}

// Runs the tests and collects their result in a TestResult.
// Calls setUpSuite() & tearDownSuite() at the beginning and end respectively.
void TestSuite::run (
    TestResult * result,
    const std::string & INFile,
    const std::string & OUTFile,
    const int loops)
{
    ConfigData * cd = result->getConfigData();
    Test * t = m_tests.front();

    // Call 'setUpSuite' at the beginning of running the suite of tests
    t->setUpSuite ( cd->getSuiteFile() );

    // Retrieve the the InputInformation, OutputInformation & Loop attributes
    std::vector< MethodsInputs > v = cd->getBoth();
    std::vector< Test * >::iterator iter;
    std::vector< MethodsInputs >::iterator it_inputs;

    int loop;
    std::string inputFile;
    std::string outputFile;

    for ( iter = m_tests.begin (), it_inputs = v.begin();
          iter != m_tests.end ();
          iter++)
    {
        if (result->shouldStop ())
            break;

        Test * test = *iter;
        if (it_inputs != v.end())
        {
            // Get input and output files for setup and teardown
            MethodsInputs mi = (*it_inputs);
            inputFile = std::string(mi.getINFile());
            outputFile = std::string(mi.getOUTFile());
            loop = mi.getLoop ();
            it_inputs++;
        }

        // Run the test the specified number of times
        for (int i = 0; i < loop; i++)
        {
            test->run( result, inputFile, outputFile, i+1);
        }
    }

    // After running all tests call tearDownSuite()
    t->tearDownSuite();
}

// Counts the number of test cases that will be run by this test.
int TestSuite::countTestCases ()
{
    int count = 0;
    std::vector<Test *>::iterator iter;
    for ( iter = m_tests.begin (); iter != m_tests.end (); iter++)
    {
        count += (*iter)->countTestCases ();
    }
    return count;
}

// Adds a test to the suite.
void TestSuite::addTest ( Test * test )
{
    m_tests.push_back (test);
}

// Returns a string representation of the test suite.
std::string TestSuite::toString () const
{
    return "suite " + m_name;
}

// Returns a string representation of the test suite.
std::string TestSuite::name () const
{
    return m_name;
}

// Changes name.
void TestSuite::changeName ( const std::string & m )
{
    if (m!="")
    {
        f_name = m;
    }
}

// Returns the file name of the file that we will log results to.
std::string TestSuite::file_name ()
{
    return f_name;
}

// Returns a string representation of the test suite.
std::vector< Test* > TestSuite::getM_Tests() const
{
    return m_tests;
}





