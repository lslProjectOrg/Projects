/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/core/sockets/src/ICMPMessageCodec.h $
  * @author:  Ripple
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2012/02/06 16:15:14 $
  * Last modified by:  $Author: haijun.li $
  *
  */

// a bunch of, well, functions that can construct and decode
// ICMP messages into Datagrams.  Before decoding, make sure
// to strip the header from the payload when receiving data
// from the RawDatagramSocket.  You can easily do this by setting
// stripHeader to true in the RawDatagramSocket constructor.
// In other words, this class doesn't understand IP headers.


#ifndef ICMP_MESSAGE_CODEC_H
#define ICMP_MESSAGE_CODEC_H

#include "core/sockets/src/Datagram.h"

namespace TA_Base_Core
{
	class ICMPMessageCodec
	{
	public:
		static ICMPMessageCodec *getInstance();

		// codec for ICMP_ECHO (ping) messages
		void constructECHO(Datagram &datagram, unsigned short id, unsigned short seq);
		bool decodeECHO(Datagram &datagram, unsigned short &idOut, unsigned short &seqOut);

	private:
		static unsigned short checksum16(char *addr, unsigned int len);
		static ICMPMessageCodec *singleton;
	};
}

#endif
