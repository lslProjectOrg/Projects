#ifndef TEXTTESTRESULT_H
#define TEXTTESTRESULT_H

/*
 * TextTestResult, used to hold and display results to tests.
 */

#if defined( WIN32 )
#pragma warning(disable:4786)
#endif // defined( WIN32 )

#include "TestResult.h"

#include <string>
#include <fstream>

class TextTestResult : public TestResult
{
public:


    // Add an exception to this object
    virtual void addError( Test * test, CppUnitException     * e );


    // Add a failure to this object
    virtual void addFailure( Test * test, AssertionFailedError * e );

    // Start the test
    virtual void startTest( Test * test );


    // Print results.
    virtual void print( std::ostream & stream, const double & time );

    // Print exceptions
    virtual void printErrors( std::ostream & stream );

    // Print failures
    virtual void printFailures( std::ostream & stream );

    // Print header information
    virtual void printHeader( std::ostream & stream );



    // Adds the ConfigData object that represents the data
    // read in from the configuration file.
    void addConfigData( ConfigData * cd);


    // Changes the name of this object. Used for writing results to the
    // correct file
    void addName( const std::string & n );


    // Returns config data associated to this object.
    ConfigData * getConfigData();

private:

    // Name of object. Used to open the log file to
    // write results to.
    std::string        m_name;


    // Configuration data
    ConfigData *    m_cData;
};


#endif


