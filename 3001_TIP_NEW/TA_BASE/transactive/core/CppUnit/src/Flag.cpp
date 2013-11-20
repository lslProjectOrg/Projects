#include "Flag.h"


// Constructor
Flag::Flag ( const std::string & name )
{
    m_name    = name;
    m_state    = true;
}


// Destructor
Flag::~Flag( void ){}



// Returns name of flag
std::string Flag::getName( void )
{
    return m_name;
}



// Returns state
bool Flag::getState( void )
{
    return m_state;
}



// Resets flags state
void Flag::resetFlag ( void )
{
    m_state = false;
}


// Sets a flags state
void Flag::setState( bool state )
{
    m_state = state;
}
