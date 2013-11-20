

#include "TestCaseExample.h"
#include "TestCaseTest.h"
#include "Multicaster.h"


int main(int argc, char** argv)
{



    TestRunner* runner = new TestRunner();


    TestCaseExample * testcase1 = new TestCaseExample    ("TestCaseExample");
    TestCaseTest    * testcase2 = new TestCaseTest        ("TestCaseTest");

    runner->addSuite( testcase1->suite () );                // Adds all test methods to the framework
    runner->addSuite( testcase2->suite () );                // Adds all test methods to the framework


    // Default, ie no parameters passed
    if (argc == 1)
    {
        // Run config file. Make sure it is in your WINNT directory.


        runner->begin( "./config.ini" );
    }


    else
    {
        runner->begin ( argv[1] );

    }


    delete testcase1;
    delete testcase2;

    delete runner;


    return 0;

}
