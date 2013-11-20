
#include "CppUnitException.h"



// Basic constructor
CppUnitException::CppUnitException() throw()
{
}



// Copy constructor
CppUnitException::CppUnitException ( const CppUnitException& rhs )
{
    m_message = rhs.m_message;
}




// Constructor
CppUnitException::CppUnitException( const std::string & message ) throw()
    : m_message ( message )
{
}




// Destruct the exception
CppUnitException::~CppUnitException () throw()
{
}

// Perform an assignment
CppUnitException & CppUnitException::operator = ( const CppUnitException& rhs )
{

    if ( & rhs != this )
    {
        m_message = rhs.m_message;
    }

    return * this;

}




// Return descriptive test
const char * CppUnitException::what() const throw()
{
    return m_message.c_str ();
}


