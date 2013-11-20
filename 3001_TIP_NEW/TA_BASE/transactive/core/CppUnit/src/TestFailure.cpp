

#include "TestFailure.h"
#include "Test.h"
#include "CppUnitException.h"

// Constructs a TestFailure with the given test and exception.
TestFailure::TestFailure ( Test * failedTest, CppUnitException * thrownException )
	: m_failedTest ( failedTest ), m_thrownException (thrownException) 
{
}

// Deletes the owned exception.
TestFailure::~TestFailure ()
{
	delete m_thrownException;
}

// Returns a short description of the failure.
std::string TestFailure::toString () 
{ 
    return m_failedTest->toString () + ": " + m_thrownException->what ();
}

// Gets the failed test.
Test *TestFailure::failedTest ()
{
	return m_failedTest;
}

// Gets the thrown exception.
CppUnitException * TestFailure::thrownException ()
{
	return m_thrownException;
}

