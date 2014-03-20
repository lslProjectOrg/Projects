#include "TestCaseMVC.h"

//sys
#include <iostream>
#include <fstream>
#include <sstream>
#include <list> 
#include <time.h>
#include <cstdlib>


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

//gui
#include <QtGui/QHBoxLayout>
#include <QtGui/QSlider>
#include <QtGui/QSpinBox>
#include <QtGui/QSplitter>

#include <QtGui/QListView>
#include <QtGui/QTableView>
#include <QtGui/QTreeView>

/////
#include "Node.h"
#include "NodeData.h"
#include "BooleanMode.h"

/////
#include "treeitem.h"
#include "treemodel.h"
////


#include "BoostLogger.h"
USING_BOOST_LOG;

NS_BEGIN(TA_Base_App)



CTestCaseMVC::CTestCaseMVC()
{

}

CTestCaseMVC::~CTestCaseMVC()
{

}

void CTestCaseMVC::test_TreeMode_BooleanMode()
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

void CTestCaseMVC::test_TreeMode_QTDemo()
{
	//data
	TreeItem* rootItem = NULL;
	TreeItem* rootRootItem = NULL;

	TreeModel* pTreeModel = NULL;

	QList<QVariant> rootItemData;
	QList<QVariant> tmpData;
	TreeItem* tmpItemNew = NULL;
	TreeItem* tmpItemParent = NULL;


	rootItemData.clear();
	rootItemData.push_back("Column_1");
	rootItemData.push_back("Column_2");
	rootItem = new TreeItem(rootItemData, NULL);

	rootItemData.clear();
	rootItemData.push_back("Column_1_Value");
	rootItemData.push_back("Column_2_Value");
	rootRootItem = new TreeItem(rootItemData, rootItem);
	rootItem->appendChild(rootRootItem);

	tmpData.clear();
	tmpData.push_back("A1");
	tmpData.push_back("A2");
	tmpItemParent = rootRootItem;//1
	tmpItemNew = new TreeItem(tmpData, tmpItemParent);
	tmpItemParent->appendChild(tmpItemNew);


	tmpItemParent = tmpItemNew;//2
	tmpItemNew = NULL;
	tmpData.clear();
	tmpData.push_back("A1_A1");
	tmpData.push_back("A2_A2");
	tmpItemNew = new TreeItem(tmpData, tmpItemParent);
	tmpItemParent->appendChild(tmpItemNew);

	tmpItemParent = tmpItemNew;//3
	tmpItemNew = NULL;
	tmpData.clear();
	tmpData.push_back("A1_A1_A1");
	tmpData.push_back("A2_A2_A2");
	tmpItemNew = new TreeItem(tmpData, tmpItemParent);
	tmpItemParent->appendChild(tmpItemNew);



	tmpData.clear();
	tmpData.push_back("B1");
	tmpData.push_back("B2");
	tmpItemParent = rootRootItem;//1
	tmpItemNew = new TreeItem(tmpData, tmpItemParent);
	tmpItemParent->appendChild(tmpItemNew);


	tmpItemParent = tmpItemNew;//2
	tmpItemNew = NULL;
	tmpData.clear();
	tmpData.push_back("B1_B1");
	tmpData.push_back("B2_B2");
	tmpItemNew = new TreeItem(tmpData, tmpItemParent);
	tmpItemParent->appendChild(tmpItemNew);

	

	pTreeModel = new TreeModel(NULL);
	pTreeModel->setRootItem(rootItem);




	//mvc view
	QSplitter* splitter = new QSplitter();//two widget
	QTreeView* treeViewTmp = new QTreeView(splitter);//

	//mvc
	treeViewTmp->setModel((QAbstractItemModel *)pTreeModel);
	treeViewTmp->setRootIndex(pTreeModel->index(0, 0, QModelIndex()));
	splitter->setWindowTitle("mvc tree view test");
	splitter->setWindowTitle(QObject::tr("Simple Tree Model"));

	splitter->show();

}

NS_END(TA_Base_App)












