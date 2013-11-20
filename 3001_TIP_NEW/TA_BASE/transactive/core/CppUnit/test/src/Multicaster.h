
#ifndef MULTICASTER_H
#define MULTICASTER_H

#if defined( WIN32 )
#pragma warning(disable:4786)
#endif // defined( WIN32 )

#include <map>
#include <vector>
using namespace std;




class Value
{
};


class MulticastObserver
{
public:
    virtual void accept ( std::string address, Value value ) = 0;

};


typedef vector < MulticastObserver * > Subscriptions;
typedef map < std::string, Subscriptions > AddressSpace;



class Multicaster
{
private:
    AddressSpace    m_addresses;

public:
    virtual        ~Multicaster            () {}
    virtual bool    subscribe            ( MulticastObserver * observer, std::string address );
    virtual bool    unsubscribe            ( MulticastObserver * observer, std::string address );
    virtual bool    publish                ( MulticastObserver * observer, std::string address, Value value );
    virtual void    unsubscribeFromAll    ( MulticastObserver * observer );

private:
    void            unsubscribe            ( Subscriptions & subscriptions, MulticastObserver * observerToRemove );

};


#endif
