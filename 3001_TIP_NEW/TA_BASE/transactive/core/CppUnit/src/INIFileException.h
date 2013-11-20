#ifndef INIFILEEXCEPTION_H
#define INIFILEEXCEPTION_H

#include "CppUnitException.h"

/*
 * INIFileException is thrown when an error has ocurred when
 * reading the configuration file.
 */

class INIFileException : public CppUnitException
{
public:

    // Constructor
    INIFileException( const std::string message ) throw();

    // Copy constructor
    INIFileException( const INIFileException & rhs );

    // Destructor
    virtual ~INIFileException() throw() {}


    // Assignment operator
    INIFileException &    operator= ( const INIFileException & rhs );
};

#endif
