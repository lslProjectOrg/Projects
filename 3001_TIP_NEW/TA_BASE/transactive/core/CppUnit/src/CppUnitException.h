#ifndef CPP_UNIT_EXCEPTION_H
#define CPP_UNIT_EXCEPTION_H

/*
 * CppUnitException is an exception that serves
 * descriptive strings through its what () method
 *
 */

#if defined( WIN32 )
#pragma warning(disable:4786)
#endif // defined( WIN32 )

#include <string>
#include <exception>

class CppUnitException : public std::exception
{
public:

    // Constructor
    CppUnitException() throw();

    // Copy constructor
    CppUnitException( const CppUnitException& rhs );

    // Constructor
    CppUnitException( const std::string & message ) throw();

    // Destructor
    virtual ~CppUnitException() throw();


    // Assignment operator
    CppUnitException & operator=( const CppUnitException& rhs );

    // Error meesage
    const char *    what() const throw();

protected:
    std::string    m_message;
};

#endif

