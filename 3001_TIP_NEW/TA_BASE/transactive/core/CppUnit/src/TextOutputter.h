#ifndef TEXTOUTPUTTER_H
#define TEXTOUTPUTTER_H

/*
 * TextOutputter, used to display results for tests.
 */

#if defined( WIN32 )
#pragma warning(disable:4786)
#endif // defined( WIN32 )

#include "Outputter.h"
#include "TestResult.h"

#include <string>
#include <fstream>

class TextOutputter : public Outputter
{
public:

    /** Constructor
     * \param result Result of the test run.
     * \param stream Stream used to output the text output.
     * \param time The time it took to execute the test.
     */
    TextOutputter( TestResult *result,
                   std::ostream &stream,
                   const double & time );

    virtual ~TextOutputter();

    // Print results.
    virtual void write();

protected:
    TestResult *m_result;
    std::ostream &m_stream;
    const double & time;

private:
    /// Prevents the use of the copy constructor.
    TextOutputter( const TextOutputter &copy );

    /// Prevents the use of the copy operator.
    void operator =( const TextOutputter &copy );


    // Print results.
    virtual void print( std::ostream & stream, const double & time );

    // Print exceptions
    virtual void printErrors( std::ostream & stream );

    // Print failures
    virtual void printFailures( std::ostream & stream );

    // Print header information
    virtual void printHeader( std::ostream & stream );
};


#endif


