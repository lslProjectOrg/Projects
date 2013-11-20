#ifndef CPPUNIT_XMLTESTRESULTOUTPUTTER_H
#define CPPUNIT_XMLTESTRESULTOUTPUTTER_H

// Changes:
// 1. Removed reference to the portability header file (does not exist in 1.3).
// 2. Changed TestResultCollector to TestResult (TestResult has been modified
//    to suit).
// 3. Removed defintion for addStatistics( Node* rootNode ).  This is not
//    needed.
// 4. Modified the logic of addFailedTests so that it now differentiates
//    between failures and errors and invoked the correct recording method.
// 5. Added addTestError( Test*, TestFailure *failure, int testNumber, Node *testsNode );

#if CPPUNIT_NEED_DLL_DECL
#pragma warning( push )
#pragma warning( disable: 4251 )  // X needs to have dll-interface to be used by clients of class Z
#endif

#include "Outputter.h"
#include <deque>
#include <iostream>
#include <map>
#include <string>
#include <utility>


class Test;
class TestFailure;


/*! \brief Outputs a TestResultCollector in XML format.
 * \ingroup WritingTestResult
 */
class XmlOutputter : public Outputter
{
public:
  /*! Constructs a XmlOutputter object.
   * \param result Result of the test run.
   * \param stream Stream used to output the XML output.
   * \param encoding Encoding used in the XML file (default is Latin-1). 
   */
  XmlOutputter( TestResult *result,
                std::ostream &stream,
                std::string encoding = "ISO-8859-1" );

  /// Destructor.
  virtual ~XmlOutputter();

  /*! Writes the specified result as an XML document to the stream.
   *
   * Refer to examples/cppunittest/XmlOutputterTest.cpp for example
   * of use and XML document structure.
   */
  virtual void write();

  /*! \brief An XML Element.
   * \warning This class will probably be replaced with an abstract
   * builder in future version.
   */
  class Node
  {
  public:
    Node( std::string elementName,
          std::string content ="" );
    Node( std::string elementName,
          int numericContent );
    virtual ~Node();

    void addAttribute( std::string attributeName,
                       std::string value );
    void addAttribute( std::string attributeName,
                       int numericValue );
    void addNode( Node *node );

    std::string toString() const;

  private:
    typedef std::pair<std::string,std::string> Attribute;

    std::string attributesAsString() const;
    std::string escape( std::string value ) const;
    static std::string asString( int value );

  private:
    std::string m_name;
    std::string m_content;
    typedef std::deque<Attribute> Attributes;
    Attributes m_attributes;
    typedef std::deque<Node *> Nodes;
    Nodes m_nodes;
  };


  virtual void writeProlog();
  virtual void writeTestsResult();

  typedef std::map<Test *,TestFailure*> FailedTests;
  virtual Node *makeRootNode();
  virtual void addFailedTests( Node *rootNode );
  virtual void addSuccessfulTests( Node *rootNode );
  virtual void addFailedTest( Test *test,
                              TestFailure *failure,
                              int testNumber,
                              Node *testsNode );
  virtual void addTestError( Test*,
                             TestFailure *failure,
                             int testNumber,
                             Node *testsNode );
  virtual void addFailureLocation( TestFailure *failure,
                                   Node *testNode );
  virtual void addSuccessfulTest( Test *test, 
                                 int testNumber,
                                 Node *testsNode );
protected:

protected:
  TestResult *m_result;
  std::ostream &m_stream;
  std::string m_encoding;

private:
  /// Prevents the use of the copy constructor.
  XmlOutputter( const XmlOutputter &copy );

  /// Prevents the use of the copy operator.
  void operator =( const XmlOutputter &copy );

private:
};




#if CPPUNIT_NEED_DLL_DECL
#pragma warning( pop )
#endif

#endif  // CPPUNIT_XMLTESTRESULTOUTPUTTER_H
