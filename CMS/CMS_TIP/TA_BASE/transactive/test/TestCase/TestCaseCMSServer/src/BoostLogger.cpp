#include "BoostLogger.h"


boost::log::sources::severity_logger< boost::log::trivial::severity_level > g_logHandleFile;


NS_BEGIN(TA_Base_Test)


CBoostLogger::CBoostLogger( void )
{
	_BoostLogInit_ex();
}

CBoostLogger::~CBoostLogger( void )
{

}





int CBoostLogger::_BoostLogInit()
{
	//2013-11-27, 16:18:22.148378 [int __cdecl main(int,char *[]) (c:\lsl\svnwork\cms\cms_tip\ta_base\transactive\test\testcase\testcasecmsserver\src\main.cpp:102)] <   normal> :A normal severity message, will not pass to the file


	// The first thing we have to do to get using the library is
	// to set up the logging sinks - i.e. where the logs will be written to.

	// One can also use lambda expressions to setup filters and formatters
	shared_ptr< sinks::synchronous_sink< sinks::text_file_backend > > file_sink =
		logging::add_file_log
		(
		keywords::file_name="LOG_%Y-%m-%d_%H_%M_%S_%f_%4N.log",      //log file name  eg.sample_2013-11-27_17_26_49_750319_0001.log
		keywords::filter = expr::attr< trivial::severity_level >("Severity") >= trivial::trace,//log level
		keywords::rotation_size=20*1024*1024,  //log file size
		keywords::time_based_rotation=sinks::file::rotation_at_time_point(0,0,0)//create log file everyday
		);


	logging::formatter formatterLog = expr::stream
		<<" "<<"["<<expr::attr< unsigned int >("RecordID")<<"]"
		<<" "<<"["<<expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d, %H:%M:%S.%f")<<"]"
		//<<" "<<expr::format_named_scope("Scope", keywords::format = "[%f:%l] [%n]") 
		<<" "<<"["<<expr::attr< attrs::current_thread_id::value_type >("ThreadID")<<"]"
		//<<" "<< default_attribute_names::line_id<<" "
		<<" "<<"<" << expr::attr< trivial::severity_level >("Severity")<< ">"
		<<" " <<expr::message;

	file_sink->set_formatter(formatterLog);

	//set all log path
	file_sink->locked_backend()->set_file_collector(
		sinks::file::make_collector(
		keywords::target="ALL_LOG",   //path     
		keywords::max_size=100*1024*1024,    //maxth log size
		keywords::min_free_space=100*1024*1024  //min free size
		));


	//logging::core::get()->set_filter(expr::attr< trivial::severity_level >("Severity") >= trivial::debug);//TODO, Test this

	file_sink->set_filter(expr::attr< trivial::severity_level >("Severity") >= trivial::debug);   //log level trivial::debug
	file_sink->locked_backend()->scan_for_files();
	file_sink->locked_backend()->auto_flush(true);


	logging::core::get()->add_global_attribute("Scope",attrs::named_scope());
	logging::core::get()->add_global_attribute("TimeStamp", attrs::local_clock());
	logging::core::get()->add_global_attribute("ThreadID", attrs::current_thread_id());
	logging::core::get()->add_global_attribute("RecordID", attrs::counter< unsigned int >());


	// Also let's add some commonly used attributes, like timestamp and record counter.
	logging::add_common_attributes();

	BOOST_LOG_FUNCTION();//

	// Now, let's try logging with severity
	//src::severity_logger< trivial::severity_level > g_logHandleFile;

	// Let's pretend we also want to profile our code, so add a special timer attribute.
	g_logHandleFile.add_attribute("Uptime", attrs::timer());

	//Test
	_TestBoostLog();

	return 0;
}

int CBoostLogger::_BoostLogInit_ex()
{
	std::string strLogPath = "ALL_Server_LOG";
	std::string strLogFileName = "Server_LOG_%Y-%m-%d_%H_%M_%S_%f_%4N.log";
	int runAsServer = 0;
	int runAsClient = 1;

	if (1 == runAsServer)
	{
		strLogPath = "ALL_Server_LOG";
		strLogFileName = "Server_LOG_%Y-%m-%d_%H_%M_%S_%f_%4N.log";
	}

	if (1 == runAsServer)
	{
		strLogPath = "ALL_Client_LOG";
		strLogFileName = "Client_LOG_%Y-%m-%d_%H_%M_%S_%f_%4N.log";
	}

	//2013-11-27, 16:18:22.148378 [int __cdecl main(int,char *[]) (c:\lsl\svnwork\cms\cms_tip\ta_base\transactive\test\testcase\testcasecmsserver\src\main.cpp:102)] <   normal> :A normal severity message, will not pass to the file


	// The first thing we have to do to get using the library is
	// to set up the logging sinks - i.e. where the logs will be written to.

	// One can also use lambda expressions to setup filters and formatters
	shared_ptr< sinks::synchronous_sink< sinks::text_file_backend > > file_sink =
		logging::add_file_log
		(
		keywords::file_name=strLogFileName,
		//keywords::file_name="Server_LOG_%Y-%m-%d_%H_%M_%S_%f_%4N.log",      //log file name  eg.sample_2013-11-27_17_26_49_750319_0001.log
		keywords::filter = expr::attr< trivial::severity_level >("Severity") >= trivial::trace,//log level
		keywords::rotation_size=20*1024*1024,  //log file size
		keywords::time_based_rotation=sinks::file::rotation_at_time_point(0,0,0)//create log file everyday
		);


	logging::formatter formatterLog = expr::stream
		<<" "<<"["<<expr::attr< unsigned int >("RecordID")<<"]"
		<<" "<<"["<<expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d, %H:%M:%S.%f")<<"]"
		//<<" "<<expr::format_named_scope("Scope", keywords::format = "[%f:%l] [%n]") 
		<<" "<<"["<<expr::attr< attrs::current_thread_id::value_type >("ThreadID")<<"]"
		//<<" "<< default_attribute_names::line_id<<" "
		<<" "<<"<" << expr::attr< trivial::severity_level >("Severity")<< ">"
		<<" " <<expr::message;

	file_sink->set_formatter(formatterLog);

	//set all log path
	file_sink->locked_backend()->set_file_collector(
		sinks::file::make_collector(
		keywords::target=strLogPath,   //path  		
		//keywords::target="ALL_Server_LOG",   //path     
		keywords::max_size=100*1024*1024,    //maxth log size
		keywords::min_free_space=100*1024*1024  //min free size
		));


	//logging::core::get()->set_filter(expr::attr< trivial::severity_level >("Severity") >= trivial::debug);//TODO, Test this

	file_sink->set_filter(expr::attr< trivial::severity_level >("Severity") >= trivial::debug);   //log level trivial::debug
	file_sink->locked_backend()->scan_for_files();
	file_sink->locked_backend()->auto_flush(true);


	logging::core::get()->add_global_attribute("Scope",attrs::named_scope());
	logging::core::get()->add_global_attribute("TimeStamp", attrs::local_clock());
	logging::core::get()->add_global_attribute("ThreadID", attrs::current_thread_id());
	logging::core::get()->add_global_attribute("RecordID", attrs::counter< unsigned int >());
	

	// Also let's add some commonly used attributes, like timestamp and record counter.
	logging::add_common_attributes();

	BOOST_LOG_FUNCTION();//

	// Now, let's try logging with severity
	//src::severity_logger< trivial::severity_level > g_logHandleFile;

	// Let's pretend we also want to profile our code, so add a special timer attribute.
	g_logHandleFile.add_attribute("Uptime", attrs::timer());

	//Test
	_TestBoostLog();

	return 0;
}
int CBoostLogger::_TestBoostLog()
{
	BOOST_LOG_FUNCTION(); //

	LOG_TRACE<<"Test LOG_TRACE";
	LOG_DEBUG<<"Test LOG_DEBUG";
	LOG_INFO<<"Test LOG_INFO";
	LOG_WARNING<<"Test LOG_WARNING";
	LOG_ERROR<<"Test LOG_ERROR";
	LOG_FATAL<<"Test LOG_FATAL";


	return 0;
}



NS_END(TA_Base_Test)
















