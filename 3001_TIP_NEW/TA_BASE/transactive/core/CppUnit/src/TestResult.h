#ifndef TESTRESULT_H
#define TESTRESULT_H

/*
 * A TestResult collects the results of executing a test case. It is an
 * instance of the Collecting Parameter pattern.
 *
 * The test framework distinguishes between failures and errors.
 * A failure is anticipated and checked for with assertions. Errors are
 * unanticipated problems signified by exceptions that are not generated
 * by the framework.
 *
 * TestResult supplies a template method 'setSynchronizationObject ()'
 * so that subclasses can provide mutual exclusion in the face of multiple
 * threads.  This can be useful when tests execute in one thread and
 * they fill a subclass of TestResult which effects change in another
 * thread.  To have mutual exclusion, override setSynchronizationObject ()
 * and make sure that you create an instance of ExclusiveZone at the
 * beginning of each method.
 *
 * see Test
 */

#if defined( WIN32 )
#pragma warning(disable:4786)
#endif // defined( WIN32 )

#include <vector>
#include "Test.h"
#include "TestFailure.h"
#include "CppUnitException.h"
#include "AssertionFailedError.h"
#include "ConfigData.h"

typedef std::vector<TestFailure *> TestFailures; // XML extension
typedef std::vector<Test *> Tests;               // XML extension

class TestResult
{
public:
    TestResult();
    virtual ~TestResult();

    virtual void addError ( Test * test, CppUnitException * e );
    virtual void addFailure ( Test * test, AssertionFailedError * e );
    virtual void addSuccess( Test * test );  // XML extension
    virtual void startTest( Test * test );
    virtual void endTest( Test * test );
    virtual int runTests();
    virtual int testErrors();
    virtual int testFailures();
    virtual bool wasSuccessful();
    virtual bool shouldStop();
    virtual void stop();

    // Sets the tests class name
    // For the XML Extension
    virtual void setTestClassName( const std::string name );

    // Gets the tests class name
    // For the XML Extension
    virtual std::string getTestClassName();

    // Returns vector of errors/exceptions
    virtual TestFailures & errors();

    // Returns a vector of failures.
    virtual TestFailures & failures();

    // Returns a vector of successes
    virtual Tests & successes();  // XML extension

    virtual ConfigData * getConfigData();
    
    // Adds the ConfigData object that represents the data
    // read in from the configuration file.
    void addConfigData( ConfigData * cd);

    class SynchronizationObject
    {
    public:
        SynchronizationObject()
            {
            }
        virtual ~SynchronizationObject()
            {
            }
        virtual void lock()
            {
            }
        virtual void unlock()
            {
            }
    };

    class ExclusiveZone
    {
        SynchronizationObject * m_syncObject;

    public:
        ExclusiveZone ( SynchronizationObject * syncObject )
            : m_syncObject ( syncObject )
            {
                m_syncObject->lock ();
            }

        ~ExclusiveZone ()
            {
                m_syncObject->unlock ();
            }
    };

protected:

    virtual void setSynchronizationObject ( SynchronizationObject * syncObject );


    // Number of tests ran
    int                     m_runTests;
    bool                    m_stop;
    SynchronizationObject * m_syncObject;


    // Vector of errors
    std::vector< TestFailure * > m_errors;


    // Vector of failures
    std::vector< TestFailure * > m_failures;


    // Vector of successes
    std::vector< Test * > m_successes;

    std::string m_name;

    // Configuration data
    ConfigData *    m_cData;
};
#endif


