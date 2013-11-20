
#include "INIFileException.h"

// Construct an INIFileException
INIFileException::INIFileException( std::string const s )
	throw(): CppUnitException ( std::string("\nCONFIG FILE EXCEPTION: ") + s + std::string("\n\n") )
{
}

// Construct an INIFileException
INIFileException::INIFileException( const INIFileException & rhs )
	: CppUnitException (rhs)
{
}

/*
// Assign an INIFileException
INIFileException & INIFileException::operator= ( const INIFileException & rhs )
{
	return *this;
}
*/
