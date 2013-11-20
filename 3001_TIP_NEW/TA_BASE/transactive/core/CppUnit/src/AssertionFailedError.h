#ifndef ASSERTIONFAILEDERROR_H
#define ASSERTIONFAILEDERROR_H

/*
 * AssertionFailedError is thrown when an assertion fails. i.e. Assert(false)
 */
#if defined( WIN32 )
#pragma warning(disable:4786)
#endif // defined( WIN32 )

#include <string>
#include "CppUnitException.h"

class AssertionFailedError : public CppUnitException
{
public:

    // Constructor
    AssertionFailedError( const std::string & message ) throw();


    // Constructor
    AssertionFailedError( const AssertionFailedError & rhs,
                          const std::string & message ) throw();

    // Constructor
    AssertionFailedError( const std::string & message,
                          const long lineNumber, const std::string & fileName ) throw();

    // Copy constructor
    AssertionFailedError( const AssertionFailedError & rhs );


    // Destructor
    virtual ~AssertionFailedError() throw()
        {
        }


    // Assignment operator
    AssertionFailedError & operator=( const AssertionFailedError & rhs );



    // Returns line number of where the Assert failed
    long        lineNumber () const;


    // Returns file name of where the Assert failed
    std::string    fileName () const;



protected:

    // Line number of where Assert failed
    long            m_lineNumber;


    // File name of where the Assert failed
    std::string        m_fileName;
};
#endif
