

#include <stdio.h>
#include <fstream>

#include "TextOutputter.h"
#include "TestCase.h"
#include "ConfigData.h"
#include "TestFailure.h"
#include "AssertionFailedError.h"

// Print errors to a stream
void TextOutputter::printErrors ( std::ostream & stream )
{
    if ( this->m_result->errors().size() == 0 )
    {
      stream << "There were no Test Errors\n";
    }
    else if ( this->m_result->errors().size() != 0 )
    {
        stream << "There were " << this->m_result->errors().size() << " Exception(s): " << std::endl;

        int i = 1;

        for ( std::vector<TestFailure *>::iterator it = this->m_result->errors().begin (); it != this->m_result->errors().end (); it++ )
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
void TextOutputter::printFailures ( std::ostream & stream)
{
    if (this->m_result->failures().size()==0)
    {
        stream << "There were no Test Failures\n";
    }
    else
    {
        stream << "There were " << this->m_result->failures().size() << " Assertion(s): " << std::endl;
        int i = 1;

        for (std::vector<TestFailure *>::iterator it = this->m_result->failures().begin (); it != this->m_result->failures().end (); it++)
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
void TextOutputter::print ( std::ostream & stream, const double & time )
{
    // Get file name to print results to
    std::string f_name = this->m_result->getConfigData()->getOutPutFile(); //m_result->getTestClassName() + ".log";
    const char* n = f_name.c_str();

    std::ofstream logFile;


    // If we want to append to the file
    if ( this->m_result->getConfigData()->getAppend () )
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
    this->m_result->getConfigData() -> print( logFile );

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
void TextOutputter::printHeader ( std::ostream & stream )
{
    stream    << "**********************************************************************************************************************\n"
            << "Test Results for: "
            << std::string( this->m_result->getTestClassName() )
            << std::endl
            << std::endl;

    if ( this->m_result->wasSuccessful())
    {
        stream << "OK ("
               << this->m_result->runTests()
               << " tests)"
               << std::endl;
    }

    stream << "Tests Ran: "
           << this->m_result->runTests()
           << "\t\tFailures: "
           << this->m_result->failures().size()
           << "\t\tExceptions: "
           << this->m_result->errors().size()
           << std::endl
           << std::endl;
}


TextOutputter::TextOutputter( TestResult *result,
                              std::ostream &stream,
                              const double & time )
    : m_result( result )
    , m_stream( stream )
    , time( time )
{
}

TextOutputter::~TextOutputter()
{
}

void TextOutputter::write()
{
    this->print( m_stream, time );
}