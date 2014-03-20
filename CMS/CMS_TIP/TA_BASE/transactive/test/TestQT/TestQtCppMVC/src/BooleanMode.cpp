#include "BooleanMode.h"

#include "Node.h"


#include "BoostLogger.h"
USING_BOOST_LOG;

NS_BEGIN(TA_Base_App)

BooleanModel::BooleanModel(QObject *parent) 
: QAbstractItemModel(parent) 
{ 
	m_pRootNode = NULL; 
} 

BooleanModel::~BooleanModel() 
{ 
	if (NULL != m_pRootNode)
	{
		delete m_pRootNode; 
		m_pRootNode = NULL;
	}

} 

void BooleanModel::setRootNode(Node* pRootNode) 
{ 
	if (NULL != m_pRootNode)
	{
		delete m_pRootNode; 
		m_pRootNode = NULL;
	}

	m_pRootNode = pRootNode; 

	reset(); 
} 

QModelIndex BooleanModel::index( int row, int column, const QModelIndex &parent /*= QModelIndex()*/ ) const
{ 

	if (NULL == m_pRootNode || row < 0 || column < 0) 
	{
		return QModelIndex(); 
	}

	Node* parentNode = NULL;
	Node* childNode = NULL;


	if (true != parent.isValid()) 
	{ 
		return createIndex(row, column, m_pRootNode); 
	}

	parentNode = _GetNodeByIndex(parent); 

	if (NULL != parentNode)
	{
		childNode = parentNode->m_pChildrenNodeLst.value(row); 
	}

	if (NULL == childNode) 
	{
		return QModelIndex(); 
	}

	return createIndex(row, column, childNode); 
} 



Node *BooleanModel::_GetNodeByIndex(const QModelIndex &nIndex) const 
{ 
	if (true == nIndex.isValid()) 
	{ 
		return static_cast<Node *>(nIndex.internalPointer());// r c p   row column pData
	}
	else 
	{ 
		return m_pRootNode; 
	} 
} 

int BooleanModel::rowCount(const QModelIndex &parent) const 
{ 
	if (parent.column() > 0) 
	{
		return 0; 
	}
	Node *parentNode = NULL;
	parentNode = _GetNodeByIndex(parent); 
	if (NULL == parentNode) 
	{
		return 0; 
	}
	return parentNode->m_pChildrenNodeLst.count(); 
} 


int BooleanModel::columnCount(const QModelIndex& parent) const 
{ 
	return 2; 
} 

QModelIndex BooleanModel::parent(const QModelIndex &child) const 
{ 
	Node *node = _GetNodeByIndex(child); 
	if (!node) 
	{
		return QModelIndex(); 
	}

	Node *parentNode = node->m_pParentNode; 
	if (!parentNode) 
	{
		return QModelIndex(); 
	}

	Node *grandparentNode = parentNode->m_pParentNode; 
	if (!grandparentNode) 
	{
		return QModelIndex(); 
	}

	int row = grandparentNode->m_pChildrenNodeLst.indexOf(parentNode); 
	return createIndex(row, 0, parentNode); 
} 

QVariant BooleanModel::data(const QModelIndex &nIndex, int role) const 
{ 
	Node* pNodeTmp = NULL;
	Node *pNodeTmp2 = NULL;

	if (true == nIndex.isValid()) 
	{ 
		pNodeTmp = static_cast<Node *>(nIndex.internalPointer());// r c p   row column pData
	}

	if (role != Qt::DisplayRole) 
	{
		pNodeTmp->m_pNodeData->m_strDataType;
		pNodeTmp->m_pNodeData->m_strDataValue;

		LOG_DEBUG<<"role="<<role
			<<" "<<"strDataType="<<pNodeTmp->m_pNodeData->m_strDataType.toStdString()
			<<" "<<"strDataValue="<<pNodeTmp->m_pNodeData->m_strDataValue.toStdString();

		return QVariant(); 
	}

	pNodeTmp2 = _GetNodeByIndex(nIndex); 

	LOG_DEBUG<<"role="<<role
		<<" "<<"strDataType="<<pNodeTmp2->m_pNodeData->m_strDataType.toStdString()
		<<" "<<"strDataValue="<<pNodeTmp2->m_pNodeData->m_strDataValue.toStdString();

	if (!pNodeTmp2) 
	{
		return QVariant(); 
	}

	if (nIndex.column() == 0) 
	{ 
		return pNodeTmp2->m_pNodeData->m_strDataType;		
	} 
	else if (nIndex.column() == 1) 
	{ 
		return pNodeTmp2->m_pNodeData->m_strDataValue; 
	} 
	return QVariant(); 
} 

QVariant BooleanModel::headerData(int section, 
								  Qt::Orientation orientation, 
								  int role) const 
{ 
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole) 
	{ 
		if (section == 0)
		{ 
			return tr("Node"); 
		} 
		else if (section == 1)
		{ 
			return tr("Value"); 
		} 
	} 
	return QVariant(); 
}


NS_END(TA_Base_App)










#if 0

//sys
#include <iostream>
#include <fstream>
#include <sstream>
#include <list> 
#include <time.h>
#include <cstdlib>

//qt app
#include <QtGui/QApplication>

//qt sys
#include <QtCore/QChar>
#include <QtCore/QString>
#include <QtCore/QDir>
#include <QtCore/QVariant>
#include <QtCore/QMap>

//qt db
#include <QtSql/QtSql>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlDatabase>

//model
#include <QtGui/QFileSystemModel>
#include "Node.h"
#include "BooleanMode.h"

//gui
#include <QtGui/QHBoxLayout>
#include <QtGui/QSlider>
#include <QtGui/QSpinBox>
#include <QtGui/QSplitter>

#include <QtGui/QListView>
#include <QtGui/QTableView>
#include <QtGui/QTreeView>

#include "QTDebugLogger.h"

#include "BoostLogger.h"
USING_BOOST_LOG;


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


void Test_FileSystem_MVC()
{
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

}

void Test_TreeView_MVC()
{
	//mvc data
	TA_Base_App::Node* pNodeRoot = NULL;
	TA_Base_App::Node* pNodeRootRoot = NULL;
	TA_Base_App::Node* pNodeOrExpression = NULL;
	TA_Base_App::Node* pNodeAndExpression = NULL;
	TA_Base_App::Node* pNodeNotExpression = NULL;
	TA_Base_App::Node* pNodeOperator = NULL;

	pNodeRoot = new TA_Base_App::Node(TA_Base_App::NodeData::Root, "root");
	pNodeRootRoot = new TA_Base_App::Node(TA_Base_App::NodeData::Root, "rootroot");
	pNodeOrExpression = new TA_Base_App::Node(TA_Base_App::NodeData::OrExpression, "A");
	pNodeAndExpression = new TA_Base_App::Node(TA_Base_App::NodeData::AndExpression, "A_A");
	pNodeNotExpression = new TA_Base_App::Node(TA_Base_App::NodeData::NotExpression, "A_A_A");
	pNodeOperator = new TA_Base_App::Node(TA_Base_App::NodeData::Operator, "A_A_A_A");


	pNodeRoot->m_pParentNode = NULL;
	pNodeRoot->m_pChildrenNodeLst.push_back(pNodeRootRoot);

	pNodeRootRoot->m_pParentNode = pNodeRoot;
	pNodeRootRoot->m_pChildrenNodeLst.push_back(pNodeOrExpression);

	pNodeOrExpression->m_pParentNode = pNodeRootRoot;
	pNodeOrExpression->m_pChildrenNodeLst.push_back(pNodeAndExpression);

	pNodeAndExpression->m_pParentNode = pNodeOrExpression;
	pNodeAndExpression->m_pChildrenNodeLst.push_back(pNodeNotExpression);

	pNodeNotExpression->m_pParentNode = pNodeAndExpression;
	pNodeNotExpression->m_pChildrenNodeLst.push_back(pNodeOperator);

	pNodeOperator->m_pParentNode = pNodeNotExpression;
	pNodeOperator->m_pChildrenNodeLst.clear();

	//model
	TA_Base_App::BooleanModel* booleanModel = new TA_Base_App::BooleanModel();
	booleanModel->setRootNode(pNodeRoot);
	pNodeRoot = NULL;


	//mvc view
	QSplitter* splitter = new QSplitter();//two widget
	QTreeView* treeViewTmp = new QTreeView(splitter);//

	//mvc
	treeViewTmp->setModel((QAbstractItemModel *)booleanModel);
	//treeViewTmp->setAlternatingRowColors(true);
	treeViewTmp->setRootIndex(booleanModel->index(0, 0, QModelIndex()));
	splitter->setWindowTitle("mvc tree view test");
	splitter->show();

}

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	logInit();
	TA_Base_App::CQTDebugLogger::getInstance();

	Test_TableView_MVC();
	//Test_TreeView_MVC();

	return app.exec();
}

#endif
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////





