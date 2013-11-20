#ifndef TESTCALLER_H
#define TESTCALLER_H

/*
 * A test caller provides access to a test case method
 * on a test case class.  Test callers are useful when
 * you want to run an individual test or add it to a
 * suite.
 *
 * Here is an example:
 *
 * class MathTest : public TestCase {
 *         ...
 *     public:
 *         void         setUp ();
 *         void         tearDown ();
 *
 *         void         testAdd ();
 *         void         testSubtract ();
 * };
 *
 * Test *MathTest::suite () {
 *     TestSuite *suite = new TestSuite;
 *
 *     suite->addTest (new TestCaller<MathTest> ("testAdd", testAdd));
 *     return suite;
 * }
 *
 * You can use a TestCaller to bind any test method on a TestCase
 * class, as long as it returns accepts void and returns void.
 *
 * See TestCase
 */

#if defined( WIN32 )
#pragma warning(disable:4786)
#endif // defined( WIN32 )

#include <string>

#include "TestCase.h"

/**
  * This macro is used to take the address of a method.  It is required
  * because the Microsoft C++ compiler automatically takes the address of a
  * method rather than requiring the developer to do it.  The downside of
  * the compiler automatically taking the address is that it can be
  * incorrectly used and not picked up until porting to Solaris.
  */
#if defined( WIN32 )
#define ADDR_OF(m) (m)
#else // !defined( WIN32 )
#define ADDR_OF(m) &(m)
#endif // !defined( WIN32 )

template < class Fixture > class TestCaller : public TestCase
{
public:

    // Pointer to a method of type 'Fixture'
    typedef void (Fixture::*TestMethod)();

private:
    // Pointer to the template object
    Fixture *    m_object;

    // Pointer to the method
    TestMethod    m_test;


public:


    // Constructor
    TestCaller ( const std::string & name, TestMethod test, Fixture * obj )
        :  TestCase ( name ), m_object ( obj ), m_test ( test )
        {
        }

protected:


    // Call method. This runs the test method.
    void runTest()
        {
            ( m_object->*m_test )();
        }


    // Run 'setUpSuite' in the test class.
    void setUpSuite( const std::string & INFile )
        {
            m_object->setUpSuite( INFile );
        }


    // Run 'tearDownSuite' in the test class.
    void tearDownSuite()
        {
            m_object->tearDownSuite();
        }


    // Run 'setUp' in the test class.
    void setUp( const std::string & INFile , const std::string & OUTFile)
        {
            m_object->setUp( INFile , OUTFile);
        }


    // Run 'tearDown' in the test class.
    void tearDown( const std::string & OUTFile )
        {
            m_object->tearDown( OUTFile );
        }
};

#endif
