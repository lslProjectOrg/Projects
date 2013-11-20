#pragma warning(disable:4786)

#ifndef MULTICASTERTEST_H
#define MULTICASTERTEST_H


#include "TestFramework.h"

#include "Multicaster.h"




class TestCaseMulticaster : public TestCase
{
public:
    class Observer : public MulticastObserver
    {
    public:
        int             m_state;
        string          m_lastAddressReceived;

                        Observer () : m_state (0) {}
                        Observer ( string initialAddress, int state ) 
                            : m_lastAddressReceived ( initialAddress ), m_state ( state ) {}

        virtual void    accept ( string address, Value Value )
        { m_lastAddressReceived = address; m_state++; }

    };

    Multicaster                 *m_multicaster;
    Observer                    *m_o1;
    Observer                    *m_o2;
    Observer                    *m_o3;
    Observer                    *m_o4;


protected:
    


    void                        testSinglePublish				();
    void                        testMultipleHomogenousPublish	();
    void                        testMultipleHeterogenousPublish ();
    void                        testSingleUnsubscribe			();
    void                        testMultipleUnsubscribe			();
    void                        testSimpleUnsubscribeAll		();
    void                        testComplexUnsubscribeAll		();

public:

	void                        setUp							( std::string INFile );
    void                        tearDown						( std::string OUTFile );

	void                        setUpSuite						( std::string INFile );
    void                        tearDownSuite					( void );

	
								TestCaseMulticaster				( std::string name );
    virtual                     ~TestCaseMulticaster			( );

	// Must have unique names
    TestSuite                 *	suite();

};


inline bool operator == ( const TestCaseMulticaster::Observer & o1, const TestCaseMulticaster::Observer & o2)
{ return o1.m_state == o2.m_state && o1.m_lastAddressReceived == o2.m_lastAddressReceived; }


#endif