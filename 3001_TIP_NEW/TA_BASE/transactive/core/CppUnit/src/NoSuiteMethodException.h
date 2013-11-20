#ifndef NOSUITEMETHODEXCEPTION_H
#define NOSUITEMETHODEXCEPTION_H

#include "CppUnitException.h"

/*
 * NoSuiteMethodException is thrown when a tester has received an
 * empty suite or a null pointer to a suite
 */


class NoSuiteMethodException : public CppUnitException
{
public:

    // Constructor
    NoSuiteMethodException( const std::string message ) throw();

    // Copy constructor
    NoSuiteMethodException( const NoSuiteMethodException & rhs );

    // Destructor
    virtual ~NoSuiteMethodException() throw() {}

    // Assignment operator
    NoSuiteMethodException & operator=( const NoSuiteMethodException & rhs );
};

#endif
