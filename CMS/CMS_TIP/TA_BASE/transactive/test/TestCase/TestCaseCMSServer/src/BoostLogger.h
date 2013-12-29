
#ifndef __CLASS_BOOST_LOGGER_HH__
#define __CLASS_BOOST_LOGGER_HH__ 

#include "UtilityFun.h"



#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/common.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/expressions.hpp>

#include <boost/log/utility/string_literal.hpp>
#include <boost/log/utility/type_info_wrapper.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

#include <boost/log/attributes/timer.hpp>
#include <boost/log/attributes/named_scope.hpp>

#include <boost/log/sources/logger.hpp>

#include <boost/log/support/date_time.hpp>
#include <boost/log/detail/default_attribute_names.hpp>



namespace logging = boost::log;
namespace sinks = boost::log::sinks;
namespace attrs = boost::log::attributes;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;
namespace trivial = boost::log::trivial;
namespace default_attribute_names = boost::log::aux::default_attribute_names;


using boost::shared_ptr;








#define SourceFLInfo    " ["<<__FILE__<<":"<<__LINE__<<"] "

#define  LOG_TRACE      BOOST_LOG_SEV(g_logHandleFile, trivial::trace)<<SourceFLInfo
#define  LOG_DEBUG      BOOST_LOG_SEV(g_logHandleFile, trivial::debug)<<SourceFLInfo
#define  LOG_INFO         BOOST_LOG_SEV(g_logHandleFile, trivial::info)<<SourceFLInfo
#define  LOG_WARNING      BOOST_LOG_SEV(g_logHandleFile, trivial::warning)<<SourceFLInfo
#define  LOG_ERROR         BOOST_LOG_SEV(g_logHandleFile, trivial::error)<<SourceFLInfo
#define  LOG_FATAL         BOOST_LOG_SEV(g_logHandleFile, trivial::fatal)<<SourceFLInfo


#define  USING_BOOST_LOG   extern boost::log::sources::severity_logger< boost::log::trivial::severity_level > g_logHandleFile;

NS_BEGIN(TA_Base_Test)


class CBoostLogger
{
public:
	CBoostLogger(void);	
	~CBoostLogger(void);

private:
	int _BoostLogInit();
	int _TestBoostLog();
	int _BoostLogInit_ex();
private:

#if 0
	//! Trivial severity levels
	// 	enum severity_level
	// 	{
	// 		trace,
	// 		debug,
	// 		info,
	// 		warning,
	// 		error,
	// 		fatal
	// 	};
#endif



};//class


NS_END(TA_Base_Test)


#endif // __CLASS_BOOST_LOGGER_HH__









