

#include <stdio.h>
#include <fstream>

#include "TextTestResult.h"
#include "TestCase.h"
#include "ConfigData.h"
#include "TestFailure.h"
#include "AssertionFailedError.h"

// Add error.
void TextTestResult::addError ( Test * test, CppUnitException * e )
{
    TestResult::addError ( test, e );
}



// Add failure
void TextTestResult::addFailure ( Test * test, AssertionFailedError * e )
{
    TestResult::addFailure ( test, e );
}




// Start a test
void TextTestResult::startTest ( Test * test )
{
    TestResult::startTest ( test );
}




// Print errors to a stream
void TextTestResult::printErrors ( std::ostream & stream )
{
    if ( testErrors () == 0 )
    {
      stream << "There were no Test Errors\n";
    }
    else if ( testErrors () != 0 )
    {
        stream << "There were " << testErrors () << " Exception(s): " << std::endl;

        int i = 1;

        for ( std::vector<TestFailure *>::iterator it = errors ().begin (); it != errors ().end (); it++ )
        {
            TestFailure             * failure    = *it;
            AssertionFailedError    * e          = ( AssertionFailedError * ) ( failure->thrownException () );

            stream << i
                   << ")\t"
                   << "line: ";

            if ( e )
            {
                stream << e->lineNumber();
            }
            else
            {
                stream << "";
            }

            stream << "\n\tFilename:\t"
                   << (e ? e->fileName() : std::string(""))
                   << "\n\tTestMethod:\t"
                   << ((TestCase *) failure->failedTest())->name()
                   << "\n\tException:\t"
                   << failure->thrownException()->what()
                   << std::endl
                   << std::endl;

            i++;
        }
        stream << std::endl;
    }
}




// Prints failures to a stream
void TextTestResult::printFailures ( std::ostream & stream)
{
    if (testFailures()==0)
    {
        stream << "There were no Test Failures\n";
    }
    else
    {
        stream << "There were " << testFailures () << " Assertion(s): " << std::endl;
        int i = 1;

        for (std::vector<TestFailure *>::iterator it = failures ().begin (); it != failures ().end (); it++)
        {
            TestFailure             *failure    = *it;
            AssertionFailedError    *e          = (AssertionFailedError *)(failure->thrownException ());

            stream    << i
                    << ")\t"
                    << "line: ";
            if ( e )
            {
                stream << e->lineNumber();
            }
            else
            {
                stream << "";
            }

            stream    << "\n\tFilename:\t"
                    << (e ? e->fileName () : "")
                    << "\n\tTestMethod:\t"
                    << ((TestCase*) failure->failedTest())->name()
                    << "\n\tAssertion:\t"
                    << failure->thrownException ()->what ()
                    << std::endl
                    << std::endl;
            i++;
        }
    }


}




// Print results to a stream
void TextTestResult::print ( std::ostream & stream, const double & time )
{
    // Get file name to print results to
    std::string f_name = m_name;
    const char* n = f_name.c_str();

    std::ofstream logFile;


    // If we want to append to the file
    if ( m_cData->getAppend () )
    {
        // Open file to output results to
        logFile.open( n, std::ios::app );
    }


    // else we do not want to append to the file
    else
    {
        logFile.open( n );
    }



    // Display and write results.
    m_cData -> print( logFile );

    printHeader        ( stream );
    printHeader        ( logFile );

    printErrors        ( stream );
    printErrors        ( logFile );

    printFailures    ( stream );
    printFailures    ( logFile );


    char buffer [50];
    sprintf (buffer, "%2.2f seconds.\n", time );


    stream << std::endl << "Time Taken For Test Case: " << buffer << std::endl;


    logFile << std::endl << "Time Taken For Test Case: " << buffer << std::endl;


    stream << "**********************************************************************************************************************\n\n\n";
    logFile << "**********************************************************************************************************************\n\n\n";
    logFile.close    ();
}




// Prints header information
void TextTestResult::printHeader ( std::ostream & stream )
{
    stream    << "**********************************************************************************************************************\n"
            << "Test Results for: "
            << std::string( m_name )
            << std::endl
            << std::endl;

    if (wasSuccessful ())
    {
        stream << "OK ("
               << runTests ()
               << " tests)"
               << std::endl;
    }

    stream << "Tests Ran: "
           << runTests ()
           << "\t\tFailures: "
           << testFailures ()
           << "\t\tExceptions: "
           << testErrors ()
           << std::endl
           << std::endl;
}

// Add name to results
void TextTestResult::addName( const std::string & n )
{
    m_name = n;
}



// Returns ConfigData read in from the configuration file for these tests.
ConfigData * TextTestResult::getConfigData()
{
    return m_cData;
}



// Add config data to this object
void TextTestResult::addConfigData( ConfigData * cd )
{
    m_cData = cd;
}
