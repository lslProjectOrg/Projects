#include "ClientTestCase.h"



//////////////////////////////////////////////////////////////////////////


//qt sys
#include <QtCore/QChar>
#include <QtCore/QString>
#include <QtCore/QDir>
#include <QtCore/QVariant>
#include <QtCore/QMap>
#include <QtCore/QObject>
//
#include <QtCore/QAbstractItemModel>
#include <QtCore/QModelIndex>

//gui
#include <QtGui/QHBoxLayout>
#include <QtGui/QSlider>
#include <QtGui/QSpinBox>
#include <QtGui/QSplitter>

#include <QtGui/QListView>
#include <QtGui/QTableView>
#include <QtGui/QTreeView>
//////////////////////////////////////////////////////////////////////////

#include "ClientLoginParam.h"
#include "MyTradeClient.h"

#include "TreeItem.h"
#include "TreeModel.h"


#include "TreeItemExchange.h"
#include "TreeModelExchange.h"


#include "ExchangeInfo.h"

#include "BoostThread.h"
#include "BoostLogger.h"
USING_BOOST_LOG;

NS_BEGIN(TA_App_App)






CClientTestCase::CClientTestCase()
{

}

CClientTestCase::~CClientTestCase()
{

}

void CClientTestCase::test_get_exchange_info()
{
	CClientLoginParam* pClientLoginParam = NULL;
	CMyTradeClient* pMyTradeClient = NULL;

	pClientLoginParam = new CClientLoginParam();
	pClientLoginParam->setDefaultValue();

	pMyTradeClient = new CMyTradeClient(*pClientLoginParam);

	pMyTradeClient->logonToServer();


	while (1)
	{
		if (pMyTradeClient->m_nExhangeNum >= 578)
		{
			break;
		}
		printf("pMyTradeClient->m_nExhangeNum=%d\n", pMyTradeClient->m_nExhangeNum);
		CBoostThread::sleep(1000);
	}


	//
	CTreeModelExchange* pTreeModel = NULL;
	pTreeModel = new CTreeModelExchange(NULL);
	pTreeModel->setRootItem(pMyTradeClient->m_pNodeExhangeRoot);


	//mvc view
	QSplitter* splitter = new QSplitter();//two widget
	QTreeView* treeViewTmp = new QTreeView(splitter);//

	//mvc
	treeViewTmp->setModel((QAbstractItemModel *)pTreeModel);
	//treeViewTmp->setRootIndex(pTreeModel->index(0, 0, QModelIndex()));
	splitter->setWindowTitle("mvc CTreeModelExchange tree view test");
	splitter->setWindowTitle(QObject::tr("Simple CTreeModelExchange"));

	splitter->show();



	if (NULL != pClientLoginParam)
	{
		delete pClientLoginParam;
		pClientLoginParam = NULL;
	}


// 	if (NULL != pMyTradeClient)
// 	{
// 		delete pMyTradeClient;
// 		pMyTradeClient = NULL;
// 	}

}

void CClientTestCase::test_TreeModel()
{
	//Data
	TreeItem* pRoot = NULL;
	TreeItem* pItemNewTmp = NULL;
	TreeItem* pItemParentTmp = NULL;

	QList<QVariant> dataTmp;
	dataTmp.clear();
	dataTmp.push_back("root");
	pRoot = new TreeItem(dataTmp, NULL);


	dataTmp.clear();
	dataTmp.push_back("AAAA");
	pItemNewTmp = new TreeItem(dataTmp, pRoot);
	pRoot->appendChild(pItemNewTmp);
	pItemParentTmp = pItemNewTmp;


	dataTmp.clear();
	dataTmp.push_back("AAAA_A");
	pItemNewTmp = new TreeItem(dataTmp, pItemParentTmp);
	pItemParentTmp->appendChild(pItemNewTmp);
	pItemParentTmp = pItemNewTmp;


	dataTmp.clear();
	dataTmp.push_back("AAAA_A_A");
	pItemNewTmp = new TreeItem(dataTmp, pItemParentTmp);
	pItemParentTmp->appendChild(pItemNewTmp);
	pItemParentTmp = pItemNewTmp;
	
	dataTmp.clear();
	dataTmp.push_back("BBBB");
	pItemNewTmp = new TreeItem(dataTmp, pRoot);
	pRoot->appendChild(pItemNewTmp);
	pItemParentTmp = pItemNewTmp;

	dataTmp.clear();
	dataTmp.push_back("BBBB_B");
	pItemNewTmp = new TreeItem(dataTmp, pItemParentTmp);
	pItemParentTmp->appendChild(pItemNewTmp);
	pItemParentTmp = pItemNewTmp;


	dataTmp.clear();
	dataTmp.push_back("BBBB_B_B");
	pItemNewTmp = new TreeItem(dataTmp, pItemParentTmp);
	pItemParentTmp->appendChild(pItemNewTmp);
	pItemParentTmp = pItemNewTmp;


	//
	TreeModel* pTreeModel = NULL;
	pTreeModel = new TreeModel(NULL);
	pTreeModel->setRootItem(pRoot);


	//mvc view
	QSplitter* splitter = new QSplitter();//two widget
	QTreeView* treeViewTmp = new QTreeView(splitter);//

	//mvc
	treeViewTmp->setModel((QAbstractItemModel *)pTreeModel);
	//treeViewTmp->setRootIndex(pTreeModel->index(0, 0, QModelIndex()));
	splitter->setWindowTitle("mvc tree view test");
	splitter->setWindowTitle(QObject::tr("Simple Tree Model"));

	splitter->show();

}




void CClientTestCase::test_TreeModelExchange()
{
	CTreeItemExchange* m_pNodeExhangeRoot = NULL;
	QList<QVariant> dataColumn;
	QString strMapKey;
	dataColumn.clear();
	dataColumn.push_back("root");
	m_pNodeExhangeRoot = new CTreeItemExchange(dataColumn, NULL);

	CExchangeInfo* pExchangeParam = NULL;
	std::string strExchangeName;
	std::string strUnderlyingCode;
	std::string strInstrumentCode;
	pExchangeParam = new CExchangeInfo();

	strExchangeName = "AAAA"; strUnderlyingCode = "AA"; strInstrumentCode = "AAAA_AA_A";
	pExchangeParam->setValue(strExchangeName, strUnderlyingCode, strInstrumentCode);
	m_pNodeExhangeRoot->appendThreeChild(pExchangeParam);

	strExchangeName = "AAAA"; strUnderlyingCode = "AA"; strInstrumentCode = "AAAA_AA_C";
	pExchangeParam->setValue(strExchangeName, strUnderlyingCode, strInstrumentCode);
	m_pNodeExhangeRoot->appendThreeChild(pExchangeParam);

	strExchangeName = "AAAA"; strUnderlyingCode = "BB"; strInstrumentCode = "AAAA_AA_B";
	pExchangeParam->setValue(strExchangeName, strUnderlyingCode, strInstrumentCode);
	m_pNodeExhangeRoot->appendThreeChild(pExchangeParam);


	strExchangeName = "CCCC"; strUnderlyingCode = "CC"; strInstrumentCode = "CCCC_CC_B";
	pExchangeParam->setValue(strExchangeName, strUnderlyingCode, strInstrumentCode);
	m_pNodeExhangeRoot->appendThreeChild(pExchangeParam);

	strExchangeName = "CCCC"; strUnderlyingCode = "CA"; strInstrumentCode = "CCCC_CC_B";
	pExchangeParam->setValue(strExchangeName, strUnderlyingCode, strInstrumentCode);
	m_pNodeExhangeRoot->appendThreeChild(pExchangeParam);

	strExchangeName = "CCCC"; strUnderlyingCode = "CA"; strInstrumentCode = "CCCC_CC_C";
	pExchangeParam->setValue(strExchangeName, strUnderlyingCode, strInstrumentCode);
	m_pNodeExhangeRoot->appendThreeChild(pExchangeParam);


	strExchangeName = "BBBB"; strUnderlyingCode = "BB"; strInstrumentCode = "BBBB_BB_B";
	pExchangeParam->setValue(strExchangeName, strUnderlyingCode, strInstrumentCode);
	m_pNodeExhangeRoot->appendThreeChild(pExchangeParam);

	if (NULL != pExchangeParam)
	{
		delete pExchangeParam;
		pExchangeParam = NULL;
	}



	//
	CTreeModelExchange* pTreeModel = NULL;
	pTreeModel = new CTreeModelExchange(NULL);
	pTreeModel->setRootItem(m_pNodeExhangeRoot);


	//mvc view
	QSplitter* splitter = new QSplitter();//two widget
	QTreeView* treeViewTmp = new QTreeView(splitter);//

	//mvc
	treeViewTmp->setModel((QAbstractItemModel *)pTreeModel);
	//treeViewTmp->setRootIndex(pTreeModel->index(0, 0, QModelIndex()));
	splitter->setWindowTitle("mvc CTreeModelExchange tree view test");
	splitter->setWindowTitle(QObject::tr("Simple CTreeModelExchange"));

	splitter->show();

}

NS_END(TA_App_App)












