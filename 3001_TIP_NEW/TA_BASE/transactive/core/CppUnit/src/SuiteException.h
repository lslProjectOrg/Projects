#ifndef CPP_UNIT_SUITENOTADDEDEXCEPTION_H
#define CPP_UNIT_SUITENOTADDEDEXCEPTION_H



/*
 * SuiteNotAddedException is thrown when a tester wants to run
 * methods in a test class that have not benn added to the TestRunner.
 */

class SuiteException : public CppUnitException
{

public:
	SuiteException ();
                           
                            ~SuiteException () {}

                            SuiteException(const SuiteException& rhs);
    SuiteException&   operator=(const SuiteException& rhs);

   
};



/// Construct an SuiteNotAddedException
inline SuiteException::SuiteException()
: CppUnitException ("EXCEPTION: Could not find suite, suite may not exist in the test runner. A possible solution is to add the class's suite method to the test runner\n\n")
{}



/// Construct an SuiteNotAddedException
inline SuiteException::SuiteException(const SuiteException& rhs)
: CppUnitException (rhs)
{}


// Assign an SuiteNotAddedException
inline SuiteException& SuiteException::operator=(const SuiteException& rhs)
{   return *this; }


#endif