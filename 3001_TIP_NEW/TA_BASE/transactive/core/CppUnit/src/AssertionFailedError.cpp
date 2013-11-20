#include "AssertionFailedError.h"
#include "TestFrameworkBase.h"

// Constrcutor. Used when the filename and the line number is unknown.
AssertionFailedError::AssertionFailedError (
    const std::string & message )
    throw (): CppUnitException ( message )
{
    m_lineNumber    = TestFrameworkBase::UNKNOWNLINENUMBER;
    m_fileName        = TestFrameworkBase::UNKNOWNFILENAME;
}



// Constructor. This is used when a string needs to be appended to the original string located in
// the rhs object.
AssertionFailedError::AssertionFailedError(
    const AssertionFailedError & rhs,
    const std::string & message )
throw(): CppUnitException ( message )
{
    m_lineNumber    = rhs.m_lineNumber;
    m_fileName        = rhs.m_fileName;
}



/// Construct an AssertionFailedError
AssertionFailedError::AssertionFailedError (
    const AssertionFailedError & rhs )
    : CppUnitException ( rhs )
{
    m_lineNumber    = rhs.m_lineNumber;
    m_fileName        = rhs.m_fileName;
}




// Copy constructor
AssertionFailedError::AssertionFailedError (
    const std::string & message,
    const long lineNumber,
    const std::string & fileName )
    throw():CppUnitException ( message ), m_lineNumber ( lineNumber ), m_fileName ( fileName )
{
}


// Retuns the line on which the error occurred
long AssertionFailedError::lineNumber () const
{
    return m_lineNumber;
}


// Returns the file in which the error occurred
std::string AssertionFailedError::fileName () const
{
    return m_fileName;
}
