#ifndef CPP_INIFILETESTCASES_H
#define CPP_INIFILETESTCASES_H


#if defined( WIN32 )
#pragma warning(disable:4786)
#endif // defined( WIN32 )

#include <vector>
#include <string>
#include "ConfigData.h"

// A class that uses the lines read in from a configuration that represent a
// testcase, and returns the configuration data for that test case.
class INIFileTestCases
{

public:
    // Constructor
    INIFileTestCases();

    // Return config data
    ConfigData * getConfigData();

    // Adds a string representing a line in a test case to this object.
    void add( const std::string & line );

private:

    // Specifies if all methods should be run.
    bool mALLMethods;

    // Converts an int to an std::string.
    std::string num_String( const int number ) const;

    // Search for an element
    std::string search( const std::string & element );

    // Return the elements associated by the string passed.
    std::vector< std::string > getStringData( const std::string & subject );

    // Return the number of times to run each method
    std::vector< int > getLoops( void );

    // Return class name.
    std::string getClassName( void );

    // Return file to log results to.
    std::string getLogFile( void );

    // Return the suite input file.
    std::string getSuiteInputFile( void );

    // Return the append flag
    bool getAppend( void );

    // Check if the specified TestMethod was defined
    // in the config file. i.e. call checkTestMethod(3) to check if
    // 'TestMethod3 = someName' was defined.
    bool checkTestMethod( const int sectionNumber );

    // All elements associated to the test case. i.e. all lines taken from
    // the configuration file.
    std::vector< std::string > elements;

    // The i'th method that doess not exist
    std::vector< int > noMethods;
};


#endif
