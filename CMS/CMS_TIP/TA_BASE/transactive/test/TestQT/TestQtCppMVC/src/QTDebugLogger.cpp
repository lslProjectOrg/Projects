#include "QTDebugLogger.h"

#include "Qtcore/QGlobalStatic"
#include "QtCore/QtDebug"
#include "QtCore/QFile"
#include "QtCore/QTextStream"
#include "QtCore/QDateTime"

//qt sys
#include <QtCore/QChar>
#include <QtCore/QString>
#include <QtCore/QDir>
#include <QtCore/QVariant>

//qt db
#include <QtSql/QtSql>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlDatabase>

//#include <boost/foreach.hpp>

#include "BoostLogger.h"
USING_BOOST_LOG;


NS_BEGIN(TA_Base_App)

CQTDebugLogger* CQTDebugLogger::m_pInstance = 0;
boost::mutex CQTDebugLogger::m_mutexInstance;

CQTDebugLogger& CQTDebugLogger::getInstance()
{	
	boost::mutex::scoped_lock lock(m_mutexInstance);	
	if (NULL == m_pInstance)
	{
		m_pInstance = new CQTDebugLogger();
	}
	return (*m_pInstance);
}

void CQTDebugLogger::removeInstance()
{
	boost::mutex::scoped_lock lock(m_mutexInstance);	
	delete m_pInstance;
	m_pInstance = NULL;
	
}


CQTDebugLogger::CQTDebugLogger( void )
{
	_LogInit();
	_TestLog();
	_CheckQTSupportDBType();
}

CQTDebugLogger::~CQTDebugLogger( void )
{

}

int CQTDebugLogger::_LogInit()
{
	//QTextCodec::setCodecForTr(QTextCodec::codecForLocale());

	/**< install qDebug function handler.*/
	qInstallMsgHandler(logMsgHandler);

	return 0;
}

int CQTDebugLogger::_TestLog()
{
	//test
	qDebug() << "test for debug";
	qDebug("This is a debug message ");
	qWarning("This is a warning message  ");
	qCritical("This is a critical message ");
	//qFatal("This is a fatal message ");
	return 0;
}




/**
//typedef void (*QtMsgHandler)(QtMsgType, const char *);
//Q_CORE_EXPORT QtMsgHandler qInstallMsgHandler(QtMsgHandler);
* @brief  custom message handler
* @param type [QtMsgType]
* @param msg [const char *]
* @return void
*/
void CQTDebugLogger::logMsgHandler( QtMsgType type, const char *msg )
{
	QString strLog;

	strLog = "---qDebug--";

	switch (type) 
	{
	case QtDebugMsg:
		//program msg
		//strLog += QString(QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss ")+"[Debug] %1").arg(QObject::tr(msg));
		strLog += QObject::tr(msg);
		LOG_DEBUG<<strLog.toStdString();
		break;
	case QtWarningMsg:
		//program warning
		//strLog += QString(QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss ")+"[Warn] %1").arg(QObject::tr(msg));
		strLog += QObject::tr(msg);
		LOG_WARNING<<strLog.toStdString();
		break;
	case QtCriticalMsg:
		//program error
		//strLog += QString(QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss ")+"[Critical] %1").arg(QObject::tr(msg));
		strLog += QObject::tr(msg);
		LOG_ERROR<<strLog.toStdString();
		break;
	case QtFatalMsg:
		//program  crash error
		//strLog = QString(QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss ")+"[Fatal] %1").arg(QObject::tr(msg));
		strLog += QObject::tr(msg);
		LOG_FATAL<<strLog.toStdString();
		//abort();
	default:
		break;
	}//switch

// 	QFile outFile(".debug.log");
// 	outFile.open(QIODevice::WriteOnly | QIODevice::Append);
// 	/**< the max size of log.txt.*/
// 	if(outFile.size()/1000>10000000)
// 	{
// 		outFile.close();
// 		outFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Truncate);
// 		outFile.close();
// 		outFile.open(QIODevice::WriteOnly | QIODevice::Append);
// 	}
// 	QTextStream ts(&outFile);
// 	ts << strLog << endl;
}



void CQTDebugLogger::_CheckQTSupportDBType()
{
	qDebug() << "Available drivers:";
	QStringList drivers = QSqlDatabase::drivers();
	foreach(QString driver, drivers)
	{
		qDebug() << "\t" << driver;
	}
}



NS_END(TA_Base_App)















