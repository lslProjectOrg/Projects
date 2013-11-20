
#include "TstFwkListener.h"
#include "FlagHandle.h"
#include "PipeClient.h"
#include "Flag.h"
#include <iostream.h>


// A Thread that listens for data received from the UTFMSServer.
void threadFunctionReceive ( LPVOID thisInstance )
{


    TstFwkListener * listen = (TstFwkListener*) thisInstance;

    listen->connectToServer();

    string message  = "";




    // While we are connected
    while ( listen->connected() )
    {
        message = "";

        char messageToken[ MAX_MESSAGE_LENGTH ];



        // Receive message
        listen->receiveMessage( message ) ;
        strcpy( messageToken, message.c_str( ) );



        // If we received a new message.
        if ( ( message != "" ) )
        {

            const char * delim = ":";

            string answer;
            string subject;
            string fromWhichProcess;
            string toWhichProcess;


            char * token;


            token                = strtok( messageToken, delim );
               fromWhichProcess    = string (token );


            // While there are tokens, get "subject"
            token                = strtok( NULL, delim );
            toWhichProcess        = string (token );


            // While there are tokens, get "subject"
            token                = strtok( NULL, delim );
            subject                = string (token );


            // Get next token
            token                = strtok( NULL, delim );
            answer                = string ( token );




            // If we didn't send the message received, or the message has been broadcasted to
            // everyone, or the message has been sent to us.

            if  ( ( toWhichProcess == "BROADCAST" ) ||
                ( toWhichProcess == listen->name() ) )

            {
                // Need to set a flag
                if ( subject == "SETFLAG" )
                {
                    listen->setFlagState( answer );
                }


                // Need to reset a flag
                if ( subject == "RESETFLAG" )
                {
                    listen->resetFlagHere( answer );
                }



                // Need to set a flag to true
                if ( subject == "SETFLAGTRUE" )
                {
                    listen->setTrueHere( answer );
                }


                // Need to kill a process
                if ( subject == "KILL" )
                {
                    if ( answer == listen->name() )
                    {
                        if ( listen->m_debug )
                        {
                            cout << "killing me" << endl;
                        }
                        OSLayer::_exit(1);
                    }
                }


                // A message has been sent
                if ( ( subject == "MESSAGE" ) && ( toWhichProcess == listen->name() ) )
                {
                    listen->setMessage ( answer );
                }

            }



        }


    }

}



