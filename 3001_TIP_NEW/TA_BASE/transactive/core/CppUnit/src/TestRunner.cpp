#include <fstream>
#include <iostream>
#include <time.h>
#include <stdio.h>

#include "TestRunner.h"
#include "TestSuite.h"
#include "ReadINIFile.h"
#include "ConfigData.h"
#include "NoSuiteMethodException.h"
#include "TestCase.h"

using namespace std;

#include "XmlOutputter.h"
#include "TextOutputter.h"

std::string getTag();

// Constructor
TestRunner::TestRunner()
{
    m_TotalTime = 0;
}

/*
 * Clean up
 */
TestRunner::~TestRunner ()
{
    std::vector< TestSuite * >::iterator iter;

    // Delete the original tests passed to the TestRunner
    for ( iter = m_suites.begin (); iter != m_suites.end (); iter++ )
    {
        ( *iter )->deleteContents();
        delete *iter;
    }
}

// Erases the new suites that were created
void TestRunner::eraseNewSuites()
{
    std::vector< TestSuite * >::iterator iter;

    // Delete the temporary suites
    for ( iter = m_newSuites.begin (); iter != m_newSuites.end (); iter++ )
    {
        delete *iter;
    }

    m_newSuites.erase(m_newSuites.begin(), m_newSuites.end());
}

/*
 * Run all m_tests and collect their results.
 *
 * n - Configuraion file
 */
void TestRunner::begin ( const std::string & configFile )
{
    // Show the time that this thing was run.
    time_t timeVal = time( NULL );
    std::cout << "Test run on " << ctime( &timeVal ) << "\n";

    // Open the file
    std::string tag = getTag();
    this->openOutputFile( tag, timeVal );

    ReadINIFile * readINIFile = new ReadINIFile ( configFile );
    std::vector< ConfigData* > data;
    data = readINIFile->readCases();

    // Iterate through configuration data and construct Test Suites accordingly
    for ( std::vector<ConfigData*>::iterator it1 = data.begin ();
          it1 != data.end (); it1++ )
    {
        ConfigData * c_d = (*it1);
        std::vector< MethodsInputs > methods = c_d->getBoth();
        std::string className  = c_d->getTestClass();
        std::string outputFile = c_d->getOutPutFile();

        TestSuite * testSuite = 0;
        try
        {
            // Construct a suite
            testSuite = getTests( methods, className, outputFile );
        }
        catch ( NoSuiteMethodException e )
        {
            std::cout << e.what() << std::endl;
            std::ostringstream line;
            line << e.what();
            line << "\n";
            this->outputFile.close();
            exit(1);
        }

        // Add new suite
        addNewSuite ( testSuite );
    }

    // Run all new test suites
    // But catch any exceptions so that we can close the output file.
    try
    {
        // Add XML root
        this->outputToFile( "<TestModule Tool=\"CppUnit 1.3\" " );

        // Determine the operatoring system
#ifdef WIN32
        this->outputToFile( "os=\"Windows\" " );
#else
#ifdef SOLARIS // From the common makefile
        this->outputToFile( "os=\"Solaris\" " );
#else
#ifdef FREEBSD // From the common makefile
        this->outputToFile( "os=\"FreeBSD\" " );
#else
#ifdef LINUX // From the common makefile
        this->outputToFile( "os=\"LINUX\" " );
#else
#error Operating system not supported
#endif
#endif
#endif
#endif

        // Date the test was executed
        this->outputToFile( std::string( "date=\"" ) +
                            ctime( &timeVal ) + std::string( "\" " ) );
        this->outputToFile( "tag=\"" + tag + "\">\n" );

        TestSuite * testToRun = 0;
        std::vector< ConfigData* >::iterator it_data;
        std::vector< TestSuite* >::iterator it;
        for (it = m_newSuites.begin (), it_data = data.begin();
             it != m_newSuites.end ();
             it++, it_data++)
        {
            testToRun = (*it);
            run( testToRun , *it_data);
        }

        // Terminate XML root
        this->outputToFile( "</TestModule>" );
    }
    catch( ... )
    {
        if( outputFile )
        {
            // Terminate XML root  The file may be useless at this point.
            this->outputToFile( "</TestModule>" );
            outputFile.flush();
            outputFile.close();
        }
        throw;
    }

    // Print out time taken to run tests
    cout << std::endl
         << std::endl
         << "Total Time for config file: "
         << configFile.c_str();
    printf( " %2.2f seconds.\n", m_TotalTime );
    cout << std::endl
         << std::endl
         << std::endl
         << std::endl
         << std::endl;

    //
    // Provide some feedback to indicate any tests that were not run.
    //
    for( std::vector< TestSuite * >::const_iterator lvIt = m_suites.begin();
         m_suites.end() != lvIt;
         lvIt++)
    {
        const TestSuite *lvSuite = *lvIt;
        std::cout << "Summary of executed tests for class "
                  << lvSuite->name().c_str()
                  << std::endl;

        std::vector< Test * > lvTests = lvSuite->getM_Tests();
        for( std::vector< Test * >::const_iterator lvTestIt = lvTests.begin();
             lvTests.end() != lvTestIt;
             lvTestIt++)
        {
            TestCase * lvTest = static_cast< TestCase * >( *lvTestIt );
            cout << "\tMethod "
                 << lvTest->name().c_str()
                 << " ";
            if ( true == lvTest->didRun() )
            {
                cout << "ran";
            }
            else
            {
                cout << "did not run";
            }
            cout << std::endl;
        }
    }

    // Erase new suites. Prepare for next configuration file.
    eraseNewSuites();

    // Remove new test suites for the next configuration file.
    delete readINIFile;

    if( outputFile )
        outputFile.close();
}

// Adds a suite to the test runner
void TestRunner::addSuite ( TestSuite * suite )
{
    m_suites.push_back ( suite );
}

// Adds a newly created suite to the test runner
void TestRunner::addNewSuite ( TestSuite * suite )
{
    m_newSuites.push_back ( suite );
}

/*
 * Runs a single test suite and collects its results.
 */
void TestRunner::run ( TestSuite * test, ConfigData* cd)
{
    TestResult result;
    result.setTestClassName( test->name() );
    result.addConfigData( cd );

    // Start clock
    clock_t start = clock();

    test->run ( & result , "", "", 1);

    // Stop clock
    clock_t finish = clock();

    double duration = (double)(finish - start) / CLOCKS_PER_SEC;

    // print results from running tests
    XmlOutputter xmlOutput( &result, outputFile );
    xmlOutput.write();
    TextOutputter textOutput( &result, std::cout, duration );
    textOutput.write();

    if ( m_TestsNotFound.size() != 0 )
    {
        cout << std::endl
             << std::endl
             << "COULD NOT FIND THE FOLLOWING TESTS. THIS COULD PROBABLY BE DUE TO SPELLING MISTAKES."
             << " CHECK FOR SPACING AND TABS:"
             << std::endl
             << std::endl;

        // Write out all tests that were not found
        std::vector< std::string >::iterator iter_notFoundTests;
        for ( iter_notFoundTests = m_TestsNotFound.begin(); iter_notFoundTests != m_TestsNotFound.end(); iter_notFoundTests++ )
        {
            std::string notFoundTest = ( *iter_notFoundTests );
            cout << "'"
                 << notFoundTest.c_str()
                 <<"'"
                 << std::endl;
        }
    }

    m_TotalTime = m_TotalTime + duration;

}

// Gets all 'm_tests' associated by the class name.
TestSuite * TestRunner::getAllTests( const std::string & test_class)
{
    TestSuite * the_suite = new TestSuite("");

    std::vector< TestSuite* >::iterator iter;

    for ( iter = m_suites.begin(); iter != m_suites.end(); iter++ )
    {
        TestSuite * t = (*iter);

        if ( t->name() == test_class )
        {
            std::vector<Test*> testMethods = t->getM_Tests();

            // Iterate through each test in the test suites
            for ( vector<Test*>::iterator it_t = testMethods.begin (); it_t != testMethods.end (); it_t++ )
                {
                    Test * test = *it_t;
                    the_suite->addTest ( test );
                }

            break;
        }
    }

    return the_suite;
}

// Returns a suite of m_tests that conform to the vector<MethodsInputs> (list of methods) and
// the class name
TestSuite * TestRunner::getTests(
    std::vector< MethodsInputs > & pMethods,
    const std::string & pClassName,
    const std::string & pOutputFile )
{
    // If there is one element and it is equal to the class name it suggests that the tester
    // wishes to run all m_tests in the test class.

    if ( ( pMethods.front().getMethod () == pClassName ) )
    {

        // Get all tests using the class name
        TestSuite * suite = getAllTests( pMethods.front().getMethod () );

        if ( suite == 0 )
        {
            throw NoSuiteMethodException(" No Suite was returned. The Test Class could have been spelt wrong");
        }

        suite->changeName( pOutputFile );
        return suite;
    }

    // For each 'TestSuite' we have
    TestSuite * the_suite = new TestSuite( pClassName );
    std::vector<TestSuite*>::iterator iter;
    for ( iter = m_suites.begin (); iter != m_suites.end (); iter++ )
    {
        TestSuite * t = (*iter);

        // Found class name
        if ( t->name() == pClassName )
        {
            std::vector<MethodsInputs>::iterator it_s;

            // Search for the test methods in the suite we have found
            for ( it_s = pMethods.begin (); it_s != pMethods.end (); it_s++ )
            {
                bool foundTest = false;
                std::string s = (*it_s).getMethod ();
                std::string file = (*it_s).getINFile ();
                std::vector<Test*> testMethods = t->getM_Tests();

                // Iterate through each test in the test suites
                for ( std::vector<Test*>::iterator it_t = testMethods.begin();
                      it_t != testMethods.end ();
                      it_t++ )
                {
                    Test * test = *it_t;
                    if ( test->name() == s )
                    {
                        the_suite->addTest ( test );
                        foundTest = true;
                        break;
                    }
                }

                if ( foundTest == false )
                {
                    m_TestsNotFound.push_back(s);
                }
            }

            // Change the name as this corresponds to the file we will write results to.
            the_suite->changeName ( pOutputFile );
            if ( the_suite->getM_Tests().size() == 0 )
            {
                std::string msg (" Empty suite returned. Methods may be spelt wrong: " );
                    msg += pClassName;
                throw NoSuiteMethodException(msg.c_str());
            }

            return the_suite;
        }
    }
    std::string msg (" No suite returned, can't find Test Class, may be spelt wrong: " );
    msg += pClassName;
    throw NoSuiteMethodException(" No suite returned, can't find Test Class, may be spelt wrong");
}


void TestRunner::outputToFile( std::string output )
{
    if( outputFile )
        outputFile << output;
}

void TestRunner::openOutputFile( std::string tag, time_t timeVal )
{
    try
    {
        // Here we are opening the output file.
        struct tm* newtime = localtime( &timeVal ); 
        std::ostringstream outputFileName;
            
        // Prepend directory name
        outputFileName << "output/";

        outputFileName << tag;
        outputFileName << "-";

        // We may need to add a leading 0.  The -100 is to account for  a
        // Y2K bug in localtime.
        outputFileName << ( ((newtime->tm_year - 100)>10)?"":"0" );
        outputFileName << (newtime->tm_year - 100);  // Y2K bug

        // We may need to add a leading 0.  The +1 is because tm_mon
        // starts at 0 for Jan.
        outputFileName << ( ((newtime->tm_mon + 1)>10)?"":"0" );
        outputFileName << (newtime->tm_mon + 1);

        outputFileName << ( ((newtime->tm_mday)>10)?"":"0" );
        outputFileName << newtime->tm_mday;
        outputFileName << ".xml";
        outputFile.open( outputFileName.str().c_str() );
    }
    catch( ... )
    {
        std::cout << "Output file not created\n";
    }
}


    
// Gets the tag that this code comes from.
// Open the CVS/tag file, read the tag line, chomp first letter and then
// output it in a pretty way.  Display a nice error message if user stuffs
// up.
//
// We will also open the outputFile here because we will be using the tag
// as part of the name.
std::string getTag()
{
    try
    {
        ifstream tagFile( "CVS/Tag" );

        if( !tagFile )
        {
            std::cerr << "Unable to provide tag info.  This must be run with CVS "
                      << "intact.\n";
            return std::string( "NONE" );
        }
        else
        {
            std::string line;
            getline( tagFile, line );
            line = line.substr( 1, line.size() - 1 );
            std::cout << "The tag of the testing code is " << line << "\n\n";
            return line;
        }
        tagFile.close();
    }
    catch( ... )
    {
        // Should we do anything?
        std::cerr << "Tag information not available due to I/O error.\n\n";
        return std::string( "NONE" );
    }
}
