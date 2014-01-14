
//#include <QtGui>
#include <cstdlib>
#include <QtGui/QApplication>
#include <QtSql/QtSql>
#include <QtCore/QChar>
#include <QtCore/QString>
#include <QtCore/QDir>
#include <QtCore/QVariant>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlDatabase>



#include <iostream>
#include <fstream>
#include <sstream>
#include <list> 
#include <time.h>


#include <boost/foreach.hpp>
#include <boost/test/unit_test.hpp>

#include "core/utilities/src/BoostLogger.h"
USING_BOOST_LOG;


bool createConnection()
{
	qDebug() << "Available drivers:";
	QStringList drivers = QSqlDatabase::drivers();
	foreach(QString driver, drivers)
	{
		qDebug() << "\t" << driver;
	}
	QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
	qDebug() << "MYSQL driver valid?" << db.isValid();

	return true;
}


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	if (!createConnection())
	{
		return 0;
	}
	return 0;
}


