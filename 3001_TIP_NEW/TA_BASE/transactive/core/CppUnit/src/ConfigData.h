#ifndef CPP_CONFIGDATA_H
#define CPP_CONFIGDATA_H


/*
 * Holds data for one test case read in from a config file. The ReadINIFile object will
 * have a vector of these for the many test cases. For example the following two test cases
 * will each have a ConfigData object to store it's contents.
 *
 * [TestClass1]
 * TestClassName=TestCoverageAndPurify
 * SuiteInputFile=   SuiteFile
 * TestMethod1= TestTrueAssertEquals
 * InputInformation1=firstInput.log
 * TestOutputFile=FirstFile

 * [TestClass2]
 * TestClassName=TestCoverageAndPurify
 * TestOutputFile=SecondFile
 * AppendFile=true
 *
 */

#if defined( WIN32 )
#pragma warning(disable:4786)
#endif // defined( WIN32 )


#include <string>
#include <vector>
#include <fstream>

#include "MethodsInputs.h"


class ConfigData
{
public:

    // Constructors
    ConfigData ()
    {
        m_append = false;
    }

    ConfigData ( const std::string & c_name,
                 const std::vector< MethodsInputs > & mi,
                 const std::string & ofile,
                 const std::string & ifile ,
                 const bool append );

    std::string    getTestClass    () const;              // Returns test class that methods are associated by.
    std::string    getOutPutFile    () const;             // Returns file name of file to report errors to.
    std::string    getSuiteFile    () const;              // Returns file to be used when setting up and tearing down suites.


    std::vector< MethodsInputs > getBoth    () const;     // Returns methods and associated input files

    bool                        getAppend    () const;    // Returns append aattribute


    void print        ( std::ostream & stream );          // Prints config data for a test case to a stream
    void showBoth    ( std::ostream & stream );           // Prints methods to a stream
    void outPutFile ( std::ostream & stream );            // Prints output file to a stream
    void TestClass    ( std::ostream & stream );          // Prints the class name to a stream
    void suiteFile    ( std::ostream & stream );          // Prints the 'SuiteInputFile' attribute to a stream


private:
    std::string                        m_testClass;       // The test class

    std::vector< MethodsInputs >    m_methsANDins;        // Vector that holds the: Methods, The number of times
                                                          // to loop each method, the input files & output files

    std::string                        m_outputfile;      // File to log results to
    std::string                        m_inputfile;       // The 'SuiteInputFile' attribute in the configuration file


    bool                            m_append;             // Whether to append or over-write to the output file.
};

#endif
