#ifndef CMS_H_
#define CMS_H_

#include <string>
#include <stdlib.h>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

#if _WIN32
	#ifdef CMS_EXPORTS
		#define CMSAPI __declspec (dllexport)
	#else
		#define CMSAPI __declspec (dllimport)
	#endif
#else
	#define CMSAPI
#endif

namespace cms
{
	using std::string;
	using boost::function;
	using boost::shared_ptr;

	typedef unsigned int TopicID;
	typedef unsigned int ClientID;
	typedef unsigned int SessionID;

	enum 
	{
		INVALID_TOPIC_ID = 0,
		INVALID_CLIENT_ID = 0,
		INVALID_SESSION_ID = 0, 
		UNKNOWN_MESSAGE_TYPE = 0,
	};
}
#endif
