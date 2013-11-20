#include <iostream>
#include <exception>
#include <math.h>
#include <typeinfo>
#include <stdio.h>

#include "TestCase.h"
#include "TestResult.h"
#include "AssertionFailedError.h"

// Constructor.
TestCase::TestCase ( const std::string & name )
    : m_name ( name ), m_wasRun( false )
{
}

// Destructor
TestCase::~TestCase ()
{
}

// Check for a failed general assertion
void TestCase::assertImplementation ( const bool condition,
                                      const std::string & conditionExpression,
                                      const long lineNumber,
                                      const std::string & fileName )
{
    // Throw an AssertionFailedError exception if the Assert failed
    if ( !condition )
    {
        throw AssertionFailedError ( conditionExpression, lineNumber, fileName );
    }
}

// Turns an integer into a string
std::string TestCase::num_String ( const int number ) const
{
    char buffer [50];
    sprintf (buffer, "%d", number);
    return std::string (buffer);
}

// Run the test and catch any exceptions that are triggered by it
void TestCase::run (
    TestResult * result,
    const std::string & INFile ,
    const std::string & OUTFile ,
    const int loops )
{
    // Adds the relevant bit of text to the call on which the method failed.
    std::string spelling = "";
    if ( ( loops == 1) || ( ( loops-1 ) % 10 == 0 ) )
    {
        spelling = "st";
    }
    else
        if ( ( loops == 2) || ( ( loops-2 ) % 10 == 0 ) )
        {
            spelling = "nd";
        }
        else
            if ( ( loops == 3) || ( ( loops-3 ) % 10 == 0 ) )
            {
                spelling = "rd";
            }
            else
            {
                spelling = "th";
            }
    result->startTest ( this );

    // Call setup before the method is run
    setUp ( INFile , OUTFile);

    // Try to run test, i.e. call the method
    try
    {
        m_wasRun = true;
        runTest ();
        result->addSuccess( this ); // XML Extension
    }

	// -------------------------------------------------
	// Added by JE, 31 March 2003

	// Catch CppUnitException
	catch (AssertionFailedError& e)
	{
		AssertionFailedError* myCopy = new AssertionFailedError(e);
		result->addFailure(this, myCopy);
	}

	// -------------------------------------------------

    // Catch exception
    catch ( const std::exception & e )
    {
        result->addError ( this,
			new AssertionFailedError ( "\"" + std::string (e.what()) + "\"" + "\n\tException was thrown on the " + num_String( loops ) + spelling  + " call, with INFile: " + INFile + " passed to setUp() ")  );
    }

    // Catch unknown exception
    catch (...)
    {
        AssertionFailedError * e = new AssertionFailedError ( std::string("unknown exception")  + "\n\tException thrown on the " + num_String( loops ) + spelling  + " call, with INFile: " + INFile + " passed to setUp() ");
        result->addError ( this, e );
    }

    // Call tearDown at the end of the test
    tearDown ( OUTFile );
    result->endTest ( this );
}

// A default run method
TestResult * TestCase::run ()
{
    TestResult * result = defaultResult ();
    m_wasRun = true;
    run (result, "", "" , 1);
    return result;
}

// All the work for runTest is deferred to subclasses
void TestCase::runTest ()
{
}

// This will be over-ridden
int TestCase::countTestCases ()
{
    return 1;
}

// Returns name of test
std::string TestCase::name ()
{
    return m_name;
}

std::string TestCase::toString ()
{
    const std::type_info& thisClass = typeid (*this);
    return std::string ( thisClass.name () ) + "." + name ();
}

TestResult *TestCase::defaultResult ()
{
    return new TestResult;
}

