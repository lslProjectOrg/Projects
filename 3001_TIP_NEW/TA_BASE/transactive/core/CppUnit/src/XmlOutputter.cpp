#include "CppUnitException.h"
#include "Test.h"
#include "TestFailure.h"
#include "TestResult.h"
#include "XmlOutputter.h"
#include <map>
#include <stdlib.h>

#include <sstream>
typedef std::ostringstream OStringStream;


// XmlOutputter::Node
// //////////////////////////////////////////////////////////////////


XmlOutputter::Node::Node( std::string elementName,
                          std::string content ) :
    m_name( elementName ),
    m_content( content )
{
}

    
XmlOutputter::Node::Node( std::string elementName,
                          int numericContent ) :
    m_name( elementName )
{
  m_content = asString( numericContent );
}


XmlOutputter::Node::~Node()
{
  Nodes::iterator itNode = m_nodes.begin();
  while ( itNode != m_nodes.end() )
    delete *itNode++;
}


void 
XmlOutputter::Node::addAttribute( std::string attributeName,
                                  std::string value  )
{
  m_attributes.push_back( Attribute( attributeName, value ) );
}


void 
XmlOutputter::Node::addAttribute( std::string attributeName,
                                  int numericValue )
{
  addAttribute( attributeName, asString( numericValue ) );
}


void 
XmlOutputter::Node::addNode( Node *node )
{
  m_nodes.push_back( node );
}


std::string 
XmlOutputter::Node::toString() const
{
  std::string element = "<";
  element += m_name;
  element += " ";
  element += attributesAsString();
  element += " >";

  element += m_content;

  Nodes::const_iterator itNode = m_nodes.begin();
  while ( itNode != m_nodes.end() )
  {
    const Node *node = *itNode++;
    element += node->toString();
  }

  //element += m_content;

  element += "</";
  element += m_name;
  element += ">\n";

  return element;
}


std::string 
XmlOutputter::Node::attributesAsString() const
{
  std::string attributes;
  Attributes::const_iterator itAttribute = m_attributes.begin();
  while ( itAttribute != m_attributes.end() )
  {
    const Attribute &attribute = *itAttribute++;
    attributes += attribute.first;
    attributes += "=\"";
    attributes += escape( attribute.second );
    attributes += "\"";
  }
  return attributes;
}


std::string 
XmlOutputter::Node::escape( std::string value ) const
{
  std::string escaped;
  for ( unsigned int index =0; index < value.length(); ++index )
  {
    char c = value[index ];
    switch ( c )    // escape all predefined XML entity (safe?)
    {
    case '<': 
      escaped += "&lt;";
      break;
    case '>': 
      escaped += "&gt;";
      break;
    case '&': 
      escaped += "&amp;";
      break;
    case '\'': 
      escaped += "&apos;";
      break;
    case '"': 
      escaped += "&quot;";
      break;
    default:
      escaped += c;
    }
  }
  
  return escaped;
}

// should be somewhere else... Future CppUnit::String ?    
std::string 
XmlOutputter::Node::asString( int value )
{
  OStringStream stream;
  stream << value;
  return stream.str();
}




// XmlOutputter
// //////////////////////////////////////////////////////////////////

XmlOutputter::XmlOutputter( TestResult *result,
                            std::ostream &stream,
                            std::string encoding ) :
    m_result( result ),
    m_stream( stream ),
    m_encoding( encoding )
{
}


XmlOutputter::~XmlOutputter()
{
}


void 
XmlOutputter::write()
{
  //writeProlog();
  writeTestsResult();
}


void 
XmlOutputter::writeProlog()
{
  m_stream  <<  "<?xml version=\"1.0\" "
                "encoding='"  <<  m_encoding  << "' standalone='yes' ?>"
            <<  std::endl;
}


void 
XmlOutputter::writeTestsResult()
{
  Node *rootNode = makeRootNode();
  m_stream  <<  rootNode->toString();
  delete rootNode;
}


XmlOutputter::Node *
XmlOutputter::makeRootNode()
{
  Node *rootNode = new Node( "TestRun" );
  rootNode->addAttribute( "TestClass", m_result->getTestClassName() );

  addFailedTests( rootNode );
  addSuccessfulTests( rootNode );

  return rootNode;
}

/**
 * This will record all test failures and test errors.
 * It will first iterate through all test failures and add them.
 * Then it will iterate through all test errors and add them.
 */
void
XmlOutputter::addFailedTests( Node *rootNode )
{
  Node *testsNode = new Node( "FailedTests" );
  rootNode->addNode( testsNode );

  const TestFailures &testFailures = m_result->failures();
  unsigned int testNumber;
  for ( testNumber = 0; testNumber < testFailures.size(); ++testNumber )
  {
     Test *test = testFailures[testNumber]->failedTest();
     addFailedTest( test, testFailures[testNumber], testNumber+1, testsNode );
  }
  const TestFailures &testErrors = m_result->errors();
  for ( testNumber = 0; testNumber < testErrors.size(); ++testNumber )
  {
     Test *test = testErrors[testNumber]->failedTest();
     addFailedTest( test, testErrors[testNumber], testNumber+1, testsNode );
  }
}


void
XmlOutputter::addSuccessfulTests( Node *rootNode )
{
  Node *testsNode = new Node( "SuccessfulTests" );
  rootNode->addNode( testsNode );

  const Tests &tests = m_result->successes();
  for ( unsigned int testNumber = 0; testNumber < tests.size(); ++testNumber )
  {
    addSuccessfulTest( tests[testNumber], testNumber+1, testsNode );
  }
}


void
XmlOutputter::addFailedTest( Test *test,
                             TestFailure *failure,
                             int testNumber,
                             Node *testsNode )
{
  CppUnitException *thrownException = failure->thrownException();
  
  //Node *testNode = new Node( "Test", thrownException->what() );
  Node *testNode = new Node( "Test");	
  testsNode->addNode( testNode );
  testNode->addAttribute( "id", testNumber );
  testNode->addNode( new Node( "Name", test->name() ) );
  testNode->addNode( new Node( "FailureType", "Assertion" ) );

  addFailureLocation( failure, testNode );
}


void
XmlOutputter::addFailureLocation( TestFailure *failure,
                                  Node *testNode )
{
  AssertionFailedError* assertion =
      dynamic_cast<AssertionFailedError*>( failure->thrownException() );
  if( assertion == NULL )
      return;

  Node *locationNode = new Node( "Location" );
  testNode->addNode( locationNode );
  locationNode->addNode( new Node( "File", assertion->fileName() ) );
  locationNode->addNode( new Node( "Line", assertion->lineNumber() ) );
}


void
XmlOutputter::addTestError( Test *test,
                            TestFailure *failure,
                            int testNumber,
                            Node *testsNode )
{
  CppUnitException *thrownException = failure->thrownException();
  
  Node *testNode = new Node( "TestError", thrownException->what() );
  testsNode->addNode( testNode );
  testNode->addAttribute( "id", testNumber );
  testNode->addNode( new Node( "Name", test->name() ) );
  testNode->addNode( new Node( "FailureType", "Error" ) );
}


void
XmlOutputter::addSuccessfulTest( Test *test, 
                                 int testNumber,
                                 Node *testsNode )
{
  Node *testNode = new Node( "Test" );
  testsNode->addNode( testNode );
  testNode->addAttribute( "id", testNumber );
  testNode->addNode( new Node( "Name", test->name() ) );
}
