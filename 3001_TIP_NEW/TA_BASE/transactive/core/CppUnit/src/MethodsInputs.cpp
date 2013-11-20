
// MethodsInputs.cpp

#include "MethodsInputs.h"

// Constructor
MethodsInputs::MethodsInputs (
    const std::string & method,
    const std::string & inFile ,
    const int loop,
    const std::string & outFile)
{
    m_method    = method;
    m_inFile    = inFile;
    m_loop        = loop;
    m_outFile    = outFile;
}




// Returns name of method
std::string MethodsInputs::getMethod() const
{
    return m_method;
}



// Returns name of input file
std::string MethodsInputs::getINFile() const
{
    return m_inFile;
}



// Returns name of output file
std::string MethodsInputs::getOUTFile() const
{
    return m_outFile;
}



// Returns loop attribute.
int MethodsInputs::getLoop() const
{
    return m_loop;
}
