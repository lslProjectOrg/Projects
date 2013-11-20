#ifndef FLAG_H
#define FLAG_H

#include <string>

/*
 *  A technique for running a section of code after another section has been run in another process
 *
 *  For example:    If we have two processes A & B, where one wants to run a section of code
 *                    before the other, Flags should be used.
 *
 *
 *
 *            A                                                            B
 *
 *
 *            cout    << "I need to run this code first "                    checkFlag(name) // This waits for the flag to be set
 *                    << endl;                                            cout    << "I need to run this after "
 *                                                                                << "the code on process A is run"
 *            setFlag(name)
 *
 *
 * As you can see the code on the right is only executed after the code on the left.
 *
 * Ofcourse there needs to be communication between the two proceses for this to happen.
 *
 * The class TstFwkListener is in charge of such communication
 *
 */


class Flag
{

public:
    // Constructor
    Flag( const std::string & name );


    // Destructor
    virtual ~Flag( void );


    // Name of flag
    std::string getName( void );


    // get state of flag
    bool getState();


    // Reset state of flag
    void resetFlag( void );


    // Set state of flag
    void setState( bool state );


protected:


private:

    // state of flag
    bool m_state;

    // Name of flag
    std::string m_name;

};


#endif
