#ifndef READINIFILE_H
#define READINIFILE_H

/*
 * An Object to read in a config file.
 */

#if defined( WIN32 )
#pragma warning(disable:4786)
#endif // defined( WIN32 )

#include <string>
#include <vector>

#include "MethodsInputs.h"
#include "INIFileTestCases.h"
#include "INIFileException.h"
#include "ConfigData.h"


/*
 * A class to read fom a configuration file.
 */
class ReadINIFile
{
public:


    // Constructor
    ReadINIFile( const std::string & Name = "config.ini" );



    // Destructor
    virtual    ~ReadINIFile();


                                    // Produces config data
    std::vector<ConfigData *>        readCases();



protected:


private:

                                    // Writes the test cases to a file to check. Used when debugging
    void                            checkCases                ();


                                    // Reads in all test cases
    void                            getTestCases            ();


    // Filename of config data
    std::string m_fileName;


    // Test Cases
    std::vector<INIFileTestCases *> m_iniCases;



    // Test Cases
    std::vector<ConfigData *> m_cases;



    // All lines read in from the config file
    std::vector< std::string > m_allLines;
};


#endif
