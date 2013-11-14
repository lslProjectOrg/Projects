/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/core/sockets/src/ICMPMessageCodec.cpp $
  * @author:  Ripple
  * @version: $Revision: #3 $
  *
  * Last modification: $DateTime: 2013/10/14 14:57:41 $
  * Last modified by:  $Author: shurui.liu $
  *
  */

// for convenience, might as well use the pre-defined ICMP
// structures and definitions.

#ifndef WIN32
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#else
#include <winsock2.h>
#include <core/sockets/src/netinet/in_systm.h>
#include <core/sockets/src/netinet/ip.h>
#include <core/sockets/src/netinet/ip_icmp.h>
#endif

#include "ICMPMessageCodec.h"
#include <string.h>

#include "core/synchronisation/src/ReEntrantThreadLockable.h" //++RenQiang
#include "core/synchronisation/src/ThreadGuard.h" //++RenQiang

namespace TA_Base_Core
{

    ICMPMessageCodec* ICMPMessageCodec::singleton = 0;
 /*   
    ICMPMessageCodec *
    ICMPMessageCodec::getInstance()
    {
        if (singleton == 0)
            singleton = new ICMPMessageCodec();
    
        return singleton;
    }
*///--RenQiang
//++RenQiang
    ICMPMessageCodec *
	ICMPMessageCodec::getInstance()
    {
		static TA_Base_Core::ReEntrantThreadLockable lock;
		TA_Base_Core::ThreadGuard guard(lock);
		static ICMPMessageCodec codec;
		if(!singleton)
			singleton = &codec;
        return singleton;
    }
//RenQiang++

    void
    ICMPMessageCodec::constructECHO(Datagram &datagram, unsigned short id, unsigned short seq)
    {
        // initialise datagram to right size.  Actually it's a bit big because
        // sizeof(icmp) is bigger than what's required for an echo message if
        // you read through RFC792.  However, it is conventional to use sizeof(icmp)
        // as the payload size and may be more portable.  Who knows?
        datagram.SetBuffer(0, sizeof(icmp));
        char* buf;
        datagram.GetBuffer(&buf);
    
        // myurghh...  Apparently, this is kosher...
        icmp* echo = (icmp *) buf;
    
        memset(buf, 0, sizeof(icmp));	
    
        // Store the ping information in the icmp structure
        echo->icmp_type = ICMP_ECHO;
        echo->icmp_code = 0;
        echo->icmp_id = id; 
        echo->icmp_seq = seq; 
        echo->icmp_cksum = checksum16(buf, sizeof(icmp));
    }
    
    bool
    ICMPMessageCodec::decodeECHO(Datagram &datagram, 
                                 unsigned short &idOut,
                                 unsigned short &seqOut)
    {
        char* buf;
        unsigned int bufSize = datagram.GetBuffer(&buf);
    
        if (bufSize < sizeof(icmp))
            return false;
    
        icmp* echo = (icmp *) buf;
    
        // snaffle the packet's checksum
        unsigned short check = echo->icmp_cksum;
    
        // recalculate
        echo->icmp_cksum = 0;
        echo->icmp_cksum = checksum16(buf, sizeof(icmp));
    
        if (check != echo->icmp_cksum)
            return false;
    
        // w00t
        idOut = echo->icmp_id;
        seqOut = echo->icmp_seq;
        return true;
    
    }
    
    unsigned short
    ICMPMessageCodec::checksum16(char *addr, unsigned int len)
    {
        // based on Mike Muuss' original Ping code...  but better.
        unsigned int numLeft = len;
        unsigned short *ptr = (unsigned short *) addr;
        unsigned int sum = 0;
    
        while (numLeft > 1)
        {
            sum += *ptr;
            ptr++;
            numLeft -= 2;
        }
    
        if (numLeft == 1)
        {
            // how legal is this?!?  Is it endian safe?
            unsigned short u = 0;
            *(unsigned char *)(&u) = *(unsigned char *)ptr;
            sum += u;
        }
    
        sum = (sum >> 16) + (sum & 0xffff);
        sum += (sum >> 16);
    
        return (unsigned short) ~sum;
    }
}
