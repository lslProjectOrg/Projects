
#include "NoSuiteMethodException.h"


// Construct an NoSuiteException
NoSuiteMethodException::NoSuiteMethodException( std::string const s )
throw(): CppUnitException ( std::string("\nEXCEPTION: ") + s + std::string("\n\n") )
{
}

// Construct an NoSuiteMethodException
NoSuiteMethodException::NoSuiteMethodException( const NoSuiteMethodException & rhs )
	: CppUnitException (rhs)
{
}

/*
// Assign an NoSuiteMethodException
NoSuiteMethodException& NoSuiteMethodException::operator = ( const NoSuiteMethodException & rhs )
{
	return *this;
}
*/
