
#include "TestCaseMulticaster.h"


TestCaseMulticaster::TestCaseMulticaster ( std::string name )
: TestCase ( name )
{
}


TestCaseMulticaster::~TestCaseMulticaster()
{
}


void TestCaseMulticaster::setUp ( std::string INFile )
{
    m_multicaster = new Multicaster;
    m_o1   = new Observer;
    m_o2   = new Observer;
    m_o3   = new Observer;
    m_o4   = new Observer;

}


void TestCaseMulticaster::tearDown ( std::string OUTFile )
{
    delete m_multicaster;
    delete m_o1;
    delete m_o2;
    delete m_o3;
    delete m_o4;
}



void TestCaseMulticaster::testSinglePublish ()
{
    // Make sure we can subscribe and publish to an address
    Value value;

    Assert ( m_multicaster->subscribe ( m_o1, "alpha" ) );
    Assert ( m_multicaster->publish ( NULL, "alpha", value ) );

    Assert ( *m_o1 == Observer ( "alpha", 1 ) );

}


void TestCaseMulticaster::testMultipleHomogenousPublish ()
{
    // Make sure we can multicast to an address
    Value value;

    Assert ( m_multicaster->subscribe ( m_o1, "alpha" ) );
    Assert ( m_multicaster->subscribe ( m_o2, "alpha" ) );
    Assert ( m_multicaster->subscribe ( m_o3, "alpha" ) );
    Assert ( m_multicaster->subscribe ( m_o4, "alpha" ) );
    Assert ( m_multicaster->publish (NULL, "alpha", value) );

    Assert ( *m_o1 == Observer ( "alpha", 1 ) );
    Assert ( *m_o2 == Observer ( "alpha", 1 ) );
    Assert ( *m_o3 == Observer ( "alpha", 1 ) );
    Assert ( *m_o4 == Observer ( "alpha", 1 ) );


}

void TestCaseMulticaster::testMultipleHeterogenousPublish ()
{
    // Make sure we can multicast to several addresses at once
    Value value;

    Assert ( m_multicaster->subscribe ( m_o1, "alpha" ) );
    Assert ( m_multicaster->subscribe ( m_o2, "beta" ) );
    Assert ( m_multicaster->subscribe ( m_o3, "alpha" ) );
    Assert ( m_multicaster->subscribe ( m_o4, "beta" ) );
    Assert ( m_multicaster->publish (NULL, "alpha", value) );

    Assert ( *m_o1 == Observer ( "alpha", 1 ) );
    Assert ( *m_o2 == Observer () );
    Assert ( *m_o3 == Observer ( "alpha", 1 ) );
    Assert ( *m_o4 == Observer () );

}



void TestCaseMulticaster::testSingleUnsubscribe ()
{
    // Make sure we can unsubscribe one of two observers on the same address
    Value value;

    Assert ( m_multicaster->subscribe ( m_o1, "alpha" ) );
    Assert ( m_multicaster->subscribe ( m_o2, "alpha" ) );
    Assert ( m_multicaster->unsubscribe ( m_o1, "alpha" ) );
    Assert ( m_multicaster->publish ( NULL, "alpha", value ) );

    Assert ( *m_o1 == Observer () );
    Assert ( *m_o2 == Observer ( "alpha", 1 ) );

}


void TestCaseMulticaster::testMultipleUnsubscribe ()
{
    // Make sure we unsubscribe all occurrences of an observer on the same address
    Value value;

    Assert ( m_multicaster->subscribe ( m_o1, "alpha ") );
    Assert ( m_multicaster->subscribe ( m_o1, "alpha ") );
    Assert ( m_multicaster->unsubscribe ( m_o1, "alpha ") );
    Assert ( m_multicaster->publish ( NULL, "alpha", value ) );
    Assert ( *m_o1 == Observer () );

}


void TestCaseMulticaster::testSimpleUnsubscribeAll ()
{
    // Make sure we unsubscribe all occurrences of an observer on all addresses
    Value value;

    Assert ( m_multicaster->subscribe ( m_o1, "alpha" ) );
    Assert ( m_multicaster->subscribe ( m_o1, "beck" ) );
    Assert ( m_multicaster->subscribe ( m_o1, "gamma" ) );

    m_multicaster->unsubscribeFromAll ( m_o1);

    Assert ( m_multicaster->publish ( NULL, "alpha", value ) );
    Assert ( m_multicaster->publish ( NULL, "beck", value ) );
    Assert ( m_multicaster->publish ( NULL, "gamma", value ) );
    Assert ( *m_o1 == Observer ());

}


void TestCaseMulticaster::testComplexUnsubscribeAll ()
{
    // Make sure we unsubscribe all occurrences of an observer on all addresses
    // in the presence of many observers
    Value value;

    Assert ( m_multicaster->subscribe ( m_o1, "alpha" ) );
    Assert ( m_multicaster->subscribe ( m_o1, "beck" ) );
    Assert ( m_multicaster->subscribe ( m_o1, "gamma" ) );
    Assert ( m_multicaster->subscribe ( m_o2, "beck" ) );
    Assert ( m_multicaster->subscribe ( m_o2, "gamma" ) );
    Assert ( m_multicaster->subscribe ( m_o2, "demeter" ) );
    m_multicaster->unsubscribeFromAll ( m_o2 );

    Assert ( m_multicaster->publish ( NULL, "alpha", value) );
    Assert ( m_multicaster->publish ( NULL, "beck", value) );
    Assert ( m_multicaster->publish ( NULL, "gamma", value) );
    Assert ( m_multicaster->publish ( NULL, "demeter", value) );
    Assert ( *m_o1 == Observer ( "gamma", 3 ) );
    Assert ( *m_o2 == Observer () );

}


TestSuite *TestCaseMulticaster::suite ()
{
	// Must have same name as test class.
    TestSuite *	suite = new TestSuite ( "TestCaseMulticaster" );

    suite->addTest ( new TestCaller<TestCaseMulticaster> ("testSinglePublish", testSinglePublish , this) );
	suite->addTest ( new TestCaller<TestCaseMulticaster> ("testMultipleHomogenousPublish", testMultipleHomogenousPublish, this ) );
	suite->addTest ( new TestCaller<TestCaseMulticaster> ("testMultipleHeterogenousPublish", testMultipleHeterogenousPublish, this ) );
	suite->addTest ( new TestCaller<TestCaseMulticaster> ("testSingleUnsubscribe", testSingleUnsubscribe, this ) );
	suite->addTest ( new TestCaller<TestCaseMulticaster> ("testMultipleUnsubscribe", testMultipleUnsubscribe, this ) );
	suite->addTest ( new TestCaller<TestCaseMulticaster> ("testSimpleUnsubscribeAll", testSimpleUnsubscribeAll, this ) );
	suite->addTest ( new TestCaller<TestCaseMulticaster> ("testComplexUnsubscribeAll", testComplexUnsubscribeAll, this ) );

    return suite;

}

