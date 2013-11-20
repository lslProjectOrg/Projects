#pragma warning(disable:4786)


#include "TestClassForSample.h"
#include "CommonDef.h"
#include "CommonData.h"
//////////////////////////////////////////////////////////////////////////
//system
#include "pthread.h"
//////////////////////////////////////////////////////////////////////////
//TA
#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/RunParams.h"

//////////////////////////////////////////////////////////////////////////
//project

//////////////////////////////////////////////////////////////////////////
//TestClass




//////////////////////////////////////////////////////////////////////////
using namespace TA_Base_Core;





/////////////////////////////////////////////////////////
// Test Framework Methods
/////////////////////////////////////////////////////////
void CTestClassForSample::setUpSuite( const std::string & inFile )
{

}


void CTestClassForSample::tearDownSuite()
{

}


void CTestClassForSample::setUp( const std::string & INFile , const std::string & OUTFile )
{

}


void CTestClassForSample::tearDown( const std::string & INFile )
{
	//deleteAlarms();
}

TestSuite * CTestClassForSample::suite ()
{
	TestSuite* testSuite = new TestSuite( "CTestClassForGetDataSameTime" );  

	
	testSuite->addTest (
		new TestCaller <CTestClassForSample> (
		"RunThisTestCase", ADDR_OF(&CTestClassForSample::RunThisTestCase ), this ) );

	return testSuite;
}

/////////////////////////////////////////////////////////
// Test Methods implementations
/////////////////////////////////////////////////////////

/*******************************************************
* Test: insert, update, select, delete one alarm row in 
*       the database.
* Expected: all the test operations can work well
********************************************************/


/***********************************************************
* Test: invoke the test thread methods and do the multi-thread
*      
* Expected: all the test operations can work well
***********************************************************/


void CTestClassForSample::RunThisTestCase()
{

	LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, "RunThisTestCase");

}

//////////////////////////////////////////////////////////////////////////




/************************************************************************/
/*                                                                      */
/************************************************************************/




































