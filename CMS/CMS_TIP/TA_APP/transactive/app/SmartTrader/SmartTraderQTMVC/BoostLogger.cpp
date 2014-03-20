#include "BoostLogger.h"


boost::log::sources::severity_logger< boost::log::trivial::severity_level > g_logHandleFile;




NS_BEGIN(TA_App_App)

CBoostLogger* CBoostLogger::m_pInstance = 0;
boost::mutex CBoostLogger::m_mutexInstance;

CBoostLogger& CBoostLogger::getInstance()
{	
	boost::mutex::scoped_lock lock(m_mutexInstance);	
	if (NULL == m_pInstance)
	{
		m_pInstance = new CBoostLogger();
	}
	return (*m_pInstance);
}

void CBoostLogger::removeInstance()
{
	boost::mutex::scoped_lock lock(m_mutexInstance);	
	delete m_pInstance;
	m_pInstance = NULL;
	
}


CBoostLogger::CBoostLogger( void )
{
	m_strLogPath = "ALL_LOG_PATH";
	m_strLogFileName = "Sample_Log_%Y-%m-%d_%H_%M_%S_%f_%4N.log";
	m_nLogLevel = boost::log::trivial::trace;
}

CBoostLogger::~CBoostLogger( void )
{

}


void CBoostLogger::setLogPath(const std::string& strLogPath)
{
	m_strLogPath = strLogPath;
}
void CBoostLogger::setLogFileName(const std::string& strLogFileName)
{
	m_strLogFileName = strLogFileName;
}
void CBoostLogger::setLogLevel(const boost::log::trivial::severity_level& nLogLevel)
{
	m_nLogLevel = nLogLevel;
	logging::core::get()->set_filter(expr::attr< trivial::severity_level >("Severity") >= m_nLogLevel);
}



int CBoostLogger::logInit()
{
	std::string strLogPath = m_strLogPath;
	std::string strLogFileName = m_strLogFileName;
	int nMaxLogSize = 1*1024*1024*1024;   //maxth log size 10GB
	int nMinFreeSpace = 100*1024*1024;  //min free size


	shared_ptr< sinks::synchronous_sink< sinks::text_file_backend > > file_sink =
		logging::add_file_log
		(
		keywords::file_name=strLogFileName,
		//keywords::file_name="Server_LOG_%Y-%m-%d_%H_%M_%S_%f_%4N.log",      //log file name  eg.sample_2013-11-27_17_26_49_750319_0001.log
		keywords::filter = expr::attr< trivial::severity_level >("Severity") >= m_nLogLevel,//log level
		keywords::rotation_size=20*1024*1024,  //log file size
		keywords::time_based_rotation=sinks::file::rotation_at_time_point(0,0,0)//create log file everyday
		);

	//[RecordID] [TimeStamp] [ThreadID] <Severity> [file:line] msg
	logging::formatter formatterLog = expr::stream
		<<" "<<"["<<expr::attr< unsigned int >("RecordID")<<"]"
		<<" "<<"["<<expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d, %H:%M:%S.%f")<<"]"
		//<<" "<<expr::format_named_scope("Scope", keywords::format = "[%f:%l] [%n]") 
		<<" "<<expr::format_named_scope("Scope", keywords::format = "") 
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
		keywords::max_size = nMaxLogSize,    //maxth log size 10GB
		keywords::min_free_space = nMinFreeSpace  //min free size
		));


	//logging::core::get()->set_filter(expr::attr< trivial::severity_level >("Severity") >= trivial::debug);//TODO, Test this

	file_sink->set_filter(expr::attr< trivial::severity_level >("Severity") >= m_nLogLevel);   //log level trivial::debug
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

	return 0;
}

int CBoostLogger::testBoostLog()
{
	BOOST_LOG_FUNCTION(); //

	LOG_FATAL<<"===================================================================================";
	//	//[RecordID] [TimeStamp] [ThreadID] <Severity> [file:line] msg
	LOG_FATAL<<"m_strLogPath="<<m_strLogPath
		<<" "<<"m_strLogFileName="<<m_strLogFileName
		<<" "<<"m_nLogLevel="<<m_nLogLevel		
		<<" "<<"format="<<"[RecordID] [TimeStamp] [ThreadID] <Severity> [file:line] msg";
	LOG_FATAL<<"===================================================================================";


	LOG_TRACE<<"Test LOG_TRACE";
	LOG_DEBUG<<"Test LOG_DEBUG";
	LOG_INFO<<"Test LOG_INFO";
	LOG_WARNING<<"Test LOG_WARNING";
	LOG_ERROR<<"Test LOG_ERROR";
	LOG_FATAL<<"Test LOG_FATAL";


	return 0;
}


#if 0
void logInit()
{
	TA_Base_App::CBoostLogger::getInstance();

	std::string strLogPath = "ALL_LOG_PATH";
	//std::string strLogFileName = "Test_Log_%Y-%m-%d_%H_%M_%S_%f_%4N.log";
	std::string strLogFileName = "Test_Log_%Y-%m-%d_%H_%M_%S.log";
	boost::log::trivial::severity_level nLogLevel = boost::log::trivial::trace;
	//boost::log::trivial::severity_level nLogLevel = boost::log::trivial::info;
	//boost::log::trivial::severity_level nLogLevel = boost::log::trivial::debug;


	TA_Base_App::CBoostLogger::getInstance().setLogPath(strLogPath);
	TA_Base_App::CBoostLogger::getInstance().setLogFileName(strLogFileName);
	TA_Base_App::CBoostLogger::getInstance().setLogLevel(nLogLevel);
	TA_Base_App::CBoostLogger::getInstance().logInit();
	TA_Base_App::CBoostLogger::getInstance().testBoostLog();
}
#endif



NS_END(TA_App_App)
















