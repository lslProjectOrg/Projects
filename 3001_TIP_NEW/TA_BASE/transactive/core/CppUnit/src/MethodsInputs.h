#ifndef METHODSINPUTS_H
#define METHODSINPUTS_H

/*
 * An object to hold a name of a test method and the associated input file, output file,
 * and number of times to run the method.
 */

#if defined( WIN32 )
#pragma warning(disable:4786)
#endif // defined( WIN32 )

#include <string>

class MethodsInputs
{
public:

    // Constructor
    MethodsInputs(
        const std::string & method,
        const std::string & inputFile ,
        const int loop,
        const std::string & outpuFile );



    std::string        getMethod()        const;        // Return name of method
    std::string        getINFile()        const;        // Return namd of input file
    std::string        getOUTFile()    const;        // Return name of output file
    int                getLoop()        const;        // Return loop

private:
    std::string        m_method;                    // Method name
    std::string        m_inFile;                    // File name of associated input file
    std::string        m_outFile;                    // File name of associated output file
    int                m_loop;                        // Number of times to run the method.
};

#endif
