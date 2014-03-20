
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

//#include <Qt/qapplication.h>
#include <QtGui/QHBoxLayout>
#include <QtGui/QSlider>
#include <QtGui/QSpinBox>
#include <QtGui/QSplitter>
#include <QtGui/QFileSystemModel>
#include <QtGui/QTreeView>
#include <QtGui/QListView>



#include <iostream>
#include <fstream>
#include <sstream>
#include <list> 
#include <time.h>


// #include <boost/foreach.hpp>
// #include <boost/test/unit_test.hpp>
// 
// #include "core/utilities/src/BoostLogger.h"
// USING_BOOST_LOG;


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
	QApplication app(argc, argv);

	QSplitter* splitter = new QSplitter;//two widget
	QFileSystemModel* model = new QFileSystemModel;
	QTreeView* tree = new QTreeView(splitter);//left tree view
	QListView* list = new QListView(splitter);//right list view

	splitter->setWindowTitle("Two vies onto the same file system model");

	model->setRootPath(QDir::currentPath());
	tree->setModel(model);//set model for view
	tree->setRootIndex(model->index(QDir::currentPath()));
	list->setModel(model);
	list->setRootIndex(model->index(QDir::currentPath()));

	list->setSelectionModel(tree->selectionModel());
	splitter->show();

	return app.exec();
}


