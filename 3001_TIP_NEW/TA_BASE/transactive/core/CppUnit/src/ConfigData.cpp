#include "ConfigData.h"


// Constructor.
ConfigData::ConfigData( const std::string & testClass,
                        const std::vector< MethodsInputs >&  methods,
                        const std::string & outfile,
                        const std::string &infile,
                        const bool append )
{
    m_testClass  = testClass;
    m_outputfile = outfile;
    m_methsANDins = methods;
    m_inputfile = infile;
    m_append = append;
}



// Returns the 'SuiteInputFile' attribute specified
// in the configuration file
std::string ConfigData::getSuiteFile() const
{
    return m_inputfile;
}



// Returns the name of the test class
std::string ConfigData::getTestClass() const
{
    return m_testClass;
}



// Returns the file name to log results to
std::string ConfigData::getOutPutFile() const
{
    return m_outputfile;
}




// Returns the 'AppendFile' attribute specified
// in the configuration file
bool ConfigData::getAppend() const
{
    return m_append;
}



// Returns the m_method and associated input files
std::vector<MethodsInputs> ConfigData::getBoth() const
{
    return m_methsANDins;
}




// Prints config data read in from a file to a stream.
void ConfigData::print ( std::ostream & stream )
{


    stream << "**********************************************************************************************************************" << std::endl;
    TestClass    ( stream );
    outPutFile    ( stream );
    suiteFile    ( stream );
    showBoth    ( stream );
    stream << "**********************************************************************************************************************" << std::endl << std::endl;
}




// Input file for the setUpSuite method.
void ConfigData::suiteFile ( std::ostream & stream)
{
    stream << "Suite File:\t\t"
           << "'" << std::string ( m_inputfile )
           << "'" << std::endl;
}



// Name of the test class that the tests are associated to
void ConfigData::TestClass ( std::ostream & stream)
{
    stream << "Running the following methods from the Test Class:\t"
           << "'"
           << std::string ( m_testClass )
           << "'"
           << std::endl;
}



// Output file to log results to.
void ConfigData::outPutFile ( std::ostream & stream)
{
    stream << "Results File:\t"
           << "'"
           << std::string ( m_outputfile )
           << "'"
           << std::endl;
}



// Methods, input files, output files,  and the number of times to run each method.
void ConfigData::showBoth ( std::ostream & stream)
{
    for ( std::vector< MethodsInputs >::iterator it = m_methsANDins.begin();
          it != m_methsANDins.end ();
          it++)
    {
        stream << "\nTesting method:\t"
               << "'" << (*it).getMethod()
               << "'"
               << "\nInput data file:\t"
               << "'"
               << (*it).getINFile()
               << "'"
               << "\nOuput data file:\t"
               << "'" << (*it).getOUTFile()
               << "'"
               << "\nLooping:\t\t"
               << "'"
               << (*it).getLoop()
               << "'"
               << std::endl << std::endl;
    }


}


