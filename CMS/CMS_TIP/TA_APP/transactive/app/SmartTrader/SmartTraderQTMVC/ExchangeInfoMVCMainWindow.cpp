#include <QtGui>

#include "ExchangeInfoMVCMainWindow.h"

#include "ExchangeInfo.h"
#include "TreeItemExchange.h"
#include "TreeModelExchange.h"

#include "ClientLoginParam.h"
#include "MyTradeClient.h"
#include "BoostThread.h"

#include "BoostLogger.h"
USING_BOOST_LOG;

using namespace TA_App_App;


ExchangeInfoMVCMainWindow::ExchangeInfoMVCMainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi(this);

#ifdef Q_WS_MAEMO_5
    // Alternating row colors look bad on Maemo
    ViewExchangeInfo->setAlternatingRowColors(false);
#endif


#if 0
	CTreeItemExchange* pNodeExhangeRoot = NULL;
	QList<QVariant> dataColumn;
	QString strMapKey;
	dataColumn.clear();
	dataColumn.push_back("root");
	pNodeExhangeRoot = new CTreeItemExchange(dataColumn, NULL);

	CExchangeInfo* pExchangeParam = NULL;
	std::string strExchangeName;
	std::string strUnderlyingCode;
	std::string strInstrumentCode;
	pExchangeParam = new CExchangeInfo();

	strExchangeName = "AAAA"; strUnderlyingCode = "AA"; strInstrumentCode = "AAAA_AA_A";
	pExchangeParam->setValue(strExchangeName, strUnderlyingCode, strInstrumentCode);
	pNodeExhangeRoot->appendThreeChild(pExchangeParam);

	strExchangeName = "AAAA"; strUnderlyingCode = "AA"; strInstrumentCode = "AAAA_AA_C";
	pExchangeParam->setValue(strExchangeName, strUnderlyingCode, strInstrumentCode);
	pNodeExhangeRoot->appendThreeChild(pExchangeParam);

	strExchangeName = "AAAA"; strUnderlyingCode = "BB"; strInstrumentCode = "AAAA_AA_B";
	pExchangeParam->setValue(strExchangeName, strUnderlyingCode, strInstrumentCode);
	pNodeExhangeRoot->appendThreeChild(pExchangeParam);


	strExchangeName = "CCCC"; strUnderlyingCode = "CC"; strInstrumentCode = "CCCC_CC_B";
	pExchangeParam->setValue(strExchangeName, strUnderlyingCode, strInstrumentCode);
	pNodeExhangeRoot->appendThreeChild(pExchangeParam);

	strExchangeName = "CCCC"; strUnderlyingCode = "CA"; strInstrumentCode = "CCCC_CC_B";
	pExchangeParam->setValue(strExchangeName, strUnderlyingCode, strInstrumentCode);
	pNodeExhangeRoot->appendThreeChild(pExchangeParam);

	strExchangeName = "CCCC"; strUnderlyingCode = "CA"; strInstrumentCode = "CCCC_CC_C";
	pExchangeParam->setValue(strExchangeName, strUnderlyingCode, strInstrumentCode);
	pNodeExhangeRoot->appendThreeChild(pExchangeParam);


	strExchangeName = "BBBB"; strUnderlyingCode = "BB"; strInstrumentCode = "BBBB_BB_B";
	pExchangeParam->setValue(strExchangeName, strUnderlyingCode, strInstrumentCode);
	pNodeExhangeRoot->appendThreeChild(pExchangeParam);

	if (NULL != pExchangeParam)
	{
		delete pExchangeParam;
		pExchangeParam = NULL;
	}
#endif

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
		TA_App_App::CBoostThread::sleep(1000);
	}


	//
	CTreeModelExchange* pTreeModel = NULL;
	pTreeModel = new CTreeModelExchange(NULL);
	pTreeModel->setRootItem(pMyTradeClient->m_pNodeExhangeRoot);

	//mvc
    ViewExchangeInfo->setModel(pTreeModel);
	//view->setRootIndex(pTreeModel->index(0, 0, QModelIndex()));//not need to set begin 0, 0

    for (int column = 0; column < pTreeModel->columnCount(); ++column)
	{
        ViewExchangeInfo->resizeColumnToContents(column);
	}

	ViewExchangeInfo->setColumnWidth(0, 200);

	connect(ViewExchangeInfo, 
		SIGNAL(doubleClicked ( const QModelIndex)),
		this,
		SLOT(slotTreeViewDoubleClick(const QModelIndex)));

}

void ExchangeInfoMVCMainWindow::slotTreeViewDoubleClick(const QModelIndex & index)
{
	QAbstractItemModel* model = NULL;

	int nRowDoubleClick = 0;
	int nColumnDoubleClick = 0;
	QString strTmp;
	Qt::ItemFlags nFlagsTreeItem;
	Qt::ItemFlags nFlagsTreeItemCheck;

	nFlagsTreeItemCheck = Qt::ItemIsEnabled | Qt::ItemIsSelectable |  Qt::ItemIsEditable;
	model = ViewExchangeInfo->model();

	nRowDoubleClick = index.row();
	nColumnDoubleClick = index.column();

	strTmp = model->data(index, Qt::DisplayRole).toString();
	QMessageBox::about(this, strTmp, strTmp);

	nFlagsTreeItem = model->flags(index);
	if (nFlagsTreeItemCheck== nFlagsTreeItem)
	{
		model->removeRow(index.row(), index.parent());
	}


}

