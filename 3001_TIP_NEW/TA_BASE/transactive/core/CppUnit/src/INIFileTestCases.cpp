#include <fstream>
#include <iostream>
#include <stdio.h>
#include "INIFileTestCases.h"
#include "INIFileException.h"
#include "ConfigData.h"

using namespace std;

INIFileTestCases::INIFileTestCases()
{
    mALLMethods = false;
}

// Adds a line to this object
void INIFileTestCases::add( const std::string & line )
{
    elements.push_back( line );
}

// Searches for a given element.
// The element passed will be in the form of 'TestClassName', 'InputInformationFile', 'TestMethod'
// and so on.
std::string INIFileTestCases::search( const std::string & element )
{
    std::string foundElement = "";

    std::vector<std::string>::iterator iter_cases;

    // For all the lines that represent the test case.
    for (iter_cases = elements.begin (); iter_cases != elements.end(); iter_cases++)
    {
        std::string line = * iter_cases;

        // We have found the element
        if ( line.find( element ) != -1 )
        {
            // Ignore it if '/' is the first character.
            if ( line.find_first_of("/") != 0 )
            {
                foundElement = line;
                break;
            }
        }
    }

    // Find the "=" character.
    foundElement = foundElement.substr( foundElement.find("=") + 1 );

    // Ignore any white spaces & tabs until a non-white space is found.
    while ( ( foundElement.find_first_of ( " " ) == 0 ) || ( foundElement.find_first_of ( "\t" ) == 0 ) )
    {
        foundElement = foundElement.substr( 1 );
    }

    return foundElement;
}

// Checks if the specified TestMethod was defined in the config file. i.e. call checkTestMethod(3) to check
// if 'TestMethod3 = someName' was defined.
bool INIFileTestCases::checkTestMethod( const int sectionNumber )
{
    bool methodDefined = true;

    std::vector<int>::iterator iter_NoMethods;

    for ( iter_NoMethods = noMethods.begin (); iter_NoMethods != noMethods.end(); iter_NoMethods++ )
    {
        int found = *iter_NoMethods;

        if ( found == sectionNumber )
        {
            methodDefined = false;
            break;
        }
    }

    return methodDefined;
}

// Gets all the elements associated by a subject.
// For example if 'TestMethod1=   test1' was defined in the configuration file, call
// getStringData("TestMethod1") to retrieve 'test1'
std::vector< std::string >    INIFileTestCases::getStringData(
    const std::string & subject )
{
    std::vector<std::string> data;
    std::vector<std::string>::iterator iter_cases;
    const std::string subjectData    = subject;
    int sectionNumber = 1;

    // For all lines
    for (iter_cases = elements.begin (); iter_cases != elements.end(); iter_cases++)
    {

        std::string method = search ( subjectData + num_String( sectionNumber ) );

        // If we are looking for 'TestMethod[i]=' and we do not find any
        // add the 'i' to the vector 'noMethods', so that we know that there wasn't one defined.
        if ( ( method == "" ) && ( subject == "TestMethod" ) )
        {
            noMethods.push_back( sectionNumber );
        }

        // If there was a 'TestMethod[i]=' defined add it to the vector of methods.
        if ( ( method != "" ) && ( subject == "TestMethod" ) )
        {
            data.push_back ( method );
        }

        // If data was not defined for any subject apart from 'TestMethod=' add it anyway.
        if ( ( subject != "TestMethod" ) && ( checkTestMethod ( sectionNumber ) ) )
        {
            data.push_back ( method );
        }
        sectionNumber++;
    }

    // If no methods were defined run all methods in the test class. I.e. add class name.
    if ( ( data.size() == 0 ) && ( subject == "TestMethod" ) )
    {
        cout << "No elements for 'TestMethod=' were provided ( Check spelling for 'TestMethod'). Running all tests in "
             << ( char *)getClassName().c_str()
             << endl;

        data.push_back ( getClassName() );

        // We need to know if all test methods are to be run
        mALLMethods = true;
    }

    return data;
}

// As above but the subject is Loop. i.e. retrieves the 'Loop' attribute in the configuration file.
std::vector< int > INIFileTestCases::getLoops( void )
{
    std::vector<int> loops;
    std::vector<std::string>::iterator iter_cases;
    const std::string Loop    = "Loop";
    int sectionNumber = 1;

    // For all lines
    for (iter_cases = elements.begin (); iter_cases != elements.end(); iter_cases++)
    {
        string loop = search ( Loop + num_String( sectionNumber ) );
        int num = 1;

        // If a method has been defined for the given sectionNumber add the data
        // associated by the 'Loop=' attribute.
        if ( checkTestMethod ( sectionNumber ) || mALLMethods )
        {
            mALLMethods = false;
            const char    * newInput = loop.c_str();
            num = atoi ( newInput );

            if ( num == 0 )
            {
                num = 1;
            }

            loops.push_back ( num );
        }

        sectionNumber++;
    }

    return loops;
}

// Returns the class name of the given test case.
// i.e. in 'TestClassName=testClass1', testClass1 will be returned.
std::string INIFileTestCases::getClassName( void )
{
    std::string className = search ( "TestClassName" );

    if ( className == "")
    {
        throw INIFileException ( "Could not find 'TestClassName='\n" );
    }

    return className;
}

// Returns the append file attribute
bool INIFileTestCases::getAppend( void )
{
    std::string append = search ( "AppendFile" );
    bool result = false;

    if ( append == "true")
    {
        result = true;
    }

    return result;
}

// As above except TestOutputFile is searched for.
std::string INIFileTestCases::getLogFile( void )
{
    std::string logFile = search ( "TestOutputFile" );

    if ( logFile == "")
    {
        throw INIFileException ( "Could not find 'TestOutputFile='\n" );
    }

    // If the file name does not
    if ( logFile.find('.') == -1 )
    {
        logFile = string(logFile + ".log ");
    }

    return logFile;
}

// Return the 'SuiteInputFile' element
std::string INIFileTestCases::getSuiteInputFile( void )
{
    std::string className = search ( "SuiteInputFile" );
    return className;
}

// Turns an integer into a string
std::string INIFileTestCases::num_String ( const int number ) const
{
    char buffer [50];
    sprintf (buffer, "%d", number);
    return std::string (buffer);
}

// Return config data for the test case.
ConfigData* INIFileTestCases::getConfigData()
{
    // Vector of TestMethods, InputInformation, OutputInformation & Loop.
    std::vector<MethodsInputs> methodsAndData;

    // Get methods
    std::vector<std::string> methods = getStringData ("TestMethod");

    // Get InputInformation
    std::vector<std::string> inputs = getStringData ("InputInformation" );

    // Get OutputInformation
    std::vector<std::string> outputs = getStringData ("OutputInformation" );

    // Get loops
    std::vector<int> loops = getLoops();

    // All methods, loops, input files and output files need to be equal. Hence make them so.
    if ( methods.size() > inputs.size() )
    {
        int cnt = methods.size() - inputs.size();
        for (int i = 0; i < cnt; i++ )
        {
            inputs.push_back ( "" );
        }
    }

    if ( methods.size() > loops.size() )
    {
        int cnt = methods.size() - loops.size();
        for (int i = 0; i < cnt; i++ )
        {
            loops.push_back ( 1 );
        }
    }

    if ( methods.size() > outputs.size() )
    {
        int cnt = methods.size() - outputs.size();
        for (int i = 0; i < cnt; i++ )
        {
            outputs.push_back ( "" );
        }
    }

    std::vector<std::string>::iterator    it_meth;
    std::vector<std::string>::iterator    it_ifile;
    std::vector<std::string>::iterator    it_ofile;
    std::vector<int>::iterator        it_loops;

    // Construct the MethodsInputs object.
    for (it_meth = methods.begin (), it_ifile = inputs.begin (), it_loops = loops.begin (), it_ofile = outputs.begin ();
                it_meth != methods.end ();
                it_meth ++, it_ifile ++, it_loops ++, it_ofile ++)
        {

            MethodsInputs m_i( * it_meth, * it_ifile , * it_loops, * it_ofile);

            methodsAndData.push_back ( m_i );
        }

    // Construct a new configuration object for the test case
    ConfigData * aTestCase    = new ConfigData ( getClassName(), methodsAndData, getLogFile() , getSuiteInputFile() , getAppend() );

    return aTestCase;
}
