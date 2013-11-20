#include <fstream>
#include <iostream>
#include <stdio.h>

#include "ReadINIFile.h"
#include "ConfigData.h"
#include "INIFileException.h"

using namespace std;

// Constructor. Given the file name of the configuration file
ReadINIFile::ReadINIFile ( const std::string & Name )
{
    try
    {
        const int lineSize = 1024;
        m_fileName = Name;

        // Open file.
        std::ifstream file( m_fileName.c_str() );

        // Throw exception if it does not exist.
        if ( ! file )
        {
            throw INIFileException ( "Could not open config file: '" + Name + "'\n" );
        }

        char inBuf[lineSize];

        // Read in all lines and add them to a vector
        while ( file.getline ( inBuf, lineSize ) )
        {
            char * crPtr = strchr( inBuf, '\r' );
            if( 0 != crPtr )
            {
                *crPtr = '\0';
            }
            std::string line = std::string( inBuf );
            m_allLines.push_back( line );
        }

        // close file
        file.close();

        // Read test cases from file
        getTestCases();
    }
    catch ( INIFileException e  )
    {
        printf( e.what() );
        printf( "\n" );
        exit(0);
    }
}

// Search for test cases. i.e. lines that begin with TestClass[i]
void ReadINIFile::getTestCases ()
{
    std::vector< std::string >::iterator iter_allLines = m_allLines.begin ();
    INIFileTestCases * aTestCase = 0;
    std::string line = *iter_allLines;

    // found "[....]". So we have found "[TestClass[i]]"
    while ( (line.find ("[" ) != std::string::npos ) &&
            (line.find ("]" ) != std::string::npos ) &&
            ( iter_allLines != m_allLines.end() ) )
    {
        aTestCase = new INIFileTestCases();
        iter_allLines++;
        line = *iter_allLines;

        // while we haven't found "[...]", i.e. another testcase.
        while ( (line.find ("[" ) == std::string::npos ) &&
                (line.find ("]" ) == std::string::npos ) )
        {
            if ( line != "")
            {
                aTestCase->add( line );
            }

            if ( iter_allLines != ( m_allLines.end() - 1 ))
            {
                iter_allLines++;
                line = *iter_allLines;
            }
            else
            {
                break;
            }
        }

        m_iniCases.push_back( aTestCase );
    }
}

// Return all the test cases read in from the file.
std::vector< ConfigData * > ReadINIFile::readCases()
{
    try
    {
        std::vector< INIFileTestCases * >::iterator iter_cases;

        for (iter_cases = m_iniCases.begin ();
             iter_cases != m_iniCases.end();
             iter_cases++)
        {
            INIFileTestCases * aCase = * iter_cases;
            ConfigData* data = aCase->getConfigData();
            m_cases.push_back( data );
        }
    }
    catch ( INIFileException e  )
    {
        printf( e.what() );
        printf( "\n" );
        exit(0);
    }

    // Used for checking
    // checkCases();
    return m_cases;
}

// Write test cases to a file for checking.
void ReadINIFile::checkCases()
{
    /*
    ofstream check("checking.txt");

    vector<INIFileTestCases*>::iterator iter_cases;

    for (iter_cases = iniCases.begin (); iter_cases != iniCases.end(); iter_cases++)
    {
        INIFileTestCases * aCase = * iter_cases;
        aCase->getConfigData()->print( check );
    }
    */
}

// Destructor
ReadINIFile::~ReadINIFile ()
{
    std::vector< INIFileTestCases * >::iterator iter_cases;
    std::vector< ConfigData * >::iterator       iter_configs;

    // Delete data
    for ( iter_cases = m_iniCases.begin ();
          iter_cases != m_iniCases.end(); iter_cases++ )
    {
        delete * iter_cases;
    }

    // Delete data
    for ( iter_configs = m_cases.begin ();
          iter_configs != m_cases.end();
          iter_configs++ )
    {
        delete * iter_configs;
    }
}
