
#include "TestResult.h"
#include "TestFailure.h"
#include "AssertionFailedError.h"

// Construct a TestResult
TestResult::TestResult()
    : m_syncObject ( new SynchronizationObject () )
    , m_runTests( 0 )
    , m_stop( false )
    , m_cData( NULL )
{
}




// TestResult destructor. Deletes errors and failures
TestResult::~TestResult ()
{
    std::vector< TestFailure * >::iterator it;

    for ( it = m_errors.begin (); it != m_errors.end (); it++ )
    {
        delete *it;
    }

    for (it = m_failures.begin (); it != m_failures.end (); it++)
    {
        delete *it;
    }

    delete m_syncObject;
}

void TestResult::addSuccess( Test * test )
{
    ExclusiveZone zone ( m_syncObject );
    m_successes.push_back ( test );
}

// Adds an error to the list of errors. The passed in exception
// caused the error
void TestResult::addError ( Test * test, CppUnitException * e )
{
    ExclusiveZone zone ( m_syncObject );
    m_errors.push_back ( new TestFailure (test, e ) );
}


// Adds a failure to the list of failures. The passed in exception
// caused the failure.
void TestResult::addFailure ( Test * test, AssertionFailedError * e )
{
    ExclusiveZone zone ( m_syncObject );
    m_failures.push_back ( new TestFailure ( test, e ) );
}

// Informs the result that a test will be started.
void TestResult::startTest ( Test * test )
{
    ExclusiveZone zone ( m_syncObject );
    m_runTests++;
}


// Informs the result that a test was completed.
void TestResult::endTest ( Test * test )
{
    ExclusiveZone zone ( m_syncObject );
}


// Gets the number of run tests.
int TestResult::runTests ()
{
    ExclusiveZone zone ( m_syncObject );
    return m_runTests;
}

// Gets the number of detected errors.
int TestResult::testErrors ()
{
    ExclusiveZone zone ( m_syncObject );
    return m_errors.size ();
}

// Gets the number of detected failures.
int TestResult::testFailures ()
{
    ExclusiveZone zone ( m_syncObject );
    return m_failures.size ();
}

// Returns whether the entire test was successful or not.
bool TestResult::wasSuccessful ()
{
    ExclusiveZone zone ( m_syncObject );
    return m_failures.size () == 0 && m_errors.size () == 0;
}

// Returns a vector of the errors.
std::vector<TestFailure *>& TestResult::errors ()
{
    ExclusiveZone zone ( m_syncObject );
    return m_errors;
}

// Returns a vector of the failures.
std::vector<TestFailure *>& TestResult::failures ()
{
    ExclusiveZone zone ( m_syncObject );
    return m_failures;
}

// Returns a vector of the successes.
std::vector<Test *>& TestResult::successes ()
{
    ExclusiveZone zone ( m_syncObject );
    return m_successes;
}

// Returns whether testing should be stopped
bool TestResult::shouldStop ()
{
    ExclusiveZone zone ( m_syncObject );
    return m_stop;
}

// Stop testing
void TestResult::stop ()
{
    ExclusiveZone zone ( m_syncObject );
    m_stop = true;
}

// Accept a new synchronization object for protection of this instance
// TestResult assumes ownership of the object
void TestResult::setSynchronizationObject ( SynchronizationObject * syncObject )
{
    delete m_syncObject;
    m_syncObject = syncObject;
}



// Returns ConfigData read in from the configuration file for these tests.
ConfigData * TestResult::getConfigData()
{
    return m_cData;
}



// Add config data to this object
void TestResult::addConfigData( ConfigData * cd )
{
    m_cData = cd;
}


// Sets the tests class name
// For the XML Extension
void TestResult::setTestClassName( const std::string name )
{
    this->m_name = name;
}

std::string TestResult::getTestClassName()
{
    return this->m_name;
}