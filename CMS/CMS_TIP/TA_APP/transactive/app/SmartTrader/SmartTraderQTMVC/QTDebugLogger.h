#ifndef __CLASS_QT_DEBUG_LOGGER_HH__
#define __CLASS_QT_DEBUG_LOGGER_HH__ 

#include "ProjectCommonData.h"

//Log4Qt http://log4qt.sourceforge.net/
//set qDebug() info to boost log
#include "Qtcore/QGlobalStatic"
#include "QtCore/QtDebug"
#include "QtCore/QFile"
#include "QtCore/QTextStream"
#include "QtCore/QDateTime"

#include <boost/chrono.hpp>
#include <boost/thread.hpp>


NS_BEGIN(TA_App_App)

class CQTDebugLogger
{
public:
	static CQTDebugLogger& getInstance();
	static void removeInstance();

private:
	static CQTDebugLogger* m_pInstance;
	static boost::mutex m_mutexInstance;

private:
	CQTDebugLogger(void);	
	~CQTDebugLogger(void);
public:
	//qglobal.h  qglobal.cpp
	//enum QtMsgType { QtDebugMsg, QtWarningMsg, QtCriticalMsg, QtFatalMsg, QtSystemMsg = QtCriticalMsg };
    static void logMsgHandler(QtMsgType type, const char *msg);

private:
	int _LogInit();
	int _TestLog();
	void _CheckQTSupportDBType();
};//class


NS_END(TA_App_App)


#endif // __CLASS_QT_DEBUG_LOGGER_HH__









