#ifndef CPPUNIT_OUTPUTTER_H
#define CPPUNIT_OUTPUTTER_H

//#include <cppunit/Portability.h>


/*! \brief Abstract outputter to print test result summary.
 * \ingroup WritingTestResult
 */
class Outputter
{
public:
  /// Destructor.
  virtual ~Outputter() {}

  virtual void write() =0;
};



#endif  // CPPUNIT_OUTPUTTER_H
