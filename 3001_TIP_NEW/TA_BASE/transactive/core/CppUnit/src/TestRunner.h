#ifndef TESTRUNNER_H
#define TESTRUNNER_H

/*
 * An object which runs tests given a configuration file
 */


#include "MethodsInputs.h"

#include <string>
#include <vector>
#include <fstream>

class TestSuite;
class ConfigData;


class TestRunner
{
public:

    // Constructor
    TestRunner();

    // Destructor
    virtual ~TestRunner();

    // Begin a test given the config file name
    void begin( const std::string & configFile );

    // Add a suite of tests to this object
    void addSuite( TestSuite * suite );



protected:


    // Vector of suites added to the test runner
    std::vector< TestSuite * >    m_suites;


    // Vector of newly created suites, created
    // from the configuration file.
    std::vector< TestSuite * >    m_newSuites;


    // A vector of tests not found.
    std::vector< std::string >  m_TestsNotFound;


private:

    // Used to calculate the total time to complete tests in a config file.
    double m_TotalTime;

    // Adds a new suite to the TestRunner
    void addNewSuite( TestSuite * suite );



    // Run a suite given the config data
    void run( TestSuite * test , ConfigData * cd);


    // Return the tests to run
    TestSuite * getTests( std::vector< MethodsInputs > & meths,
                          const std::string & o_file,
                          const std::string & i_file );


    // Return all tests in a test class given it's name
    TestSuite * getAllTests( const std::string & nameClass );


    // Erases the new suites that were created
    void eraseNewSuites();

private:
    // The file stream that we will write everything that is outputted to
    // std::cout
    std::ofstream outputFile;

    // Writes line to the outputFile.
    void outputToFile( std::string output );

    // Opens the output file.
    void openOutputFile( std::string tag, time_t timeVal );
};

#endif
