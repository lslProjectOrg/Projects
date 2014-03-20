#include "TreeModelExchange.h"


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


#include "TreeItemExchange.h"

NS_BEGIN(TA_App_App)

CTreeModelExchange::CTreeModelExchange(QObject *parent)
    : QAbstractItemModel(parent)
{
	m_pRootItemCopy = NULL;
}


CTreeModelExchange::~CTreeModelExchange()
{
	m_pRootItemCopy = NULL;
}



int CTreeModelExchange::columnCount(const QModelIndex &parent) const
{
	int nColumnCount = 0;

    if (parent.isValid())
	{
        nColumnCount = static_cast<CTreeItemExchange*>(parent.internalPointer())->columnCount();
	}
    else
	{
        nColumnCount = m_pRootItemCopy->columnCount();
	}

	return nColumnCount;
}



QVariant CTreeModelExchange::data(const QModelIndex &index, int role) const
{
	CTreeItemExchange* item = NULL;

    if (!index.isValid())
	{
        return QVariant();
	}

    if (role != Qt::DisplayRole)
	{
        return QVariant();
	}

    item = static_cast<CTreeItemExchange*>(index.internalPointer());

    return item->data(index.column());
}



Qt::ItemFlags CTreeModelExchange::flags(const QModelIndex &index) const
{
    if (!index.isValid())
	{
        return 0;
	}

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}



QVariant CTreeModelExchange::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
	{
		//return QVariant();
        //return rootItemCopy->data(section);
		return QVariant("root_column");
	}

    return QVariant();
}



QModelIndex CTreeModelExchange::index(int row, int column, const QModelIndex &parent) const
{
	CTreeItemExchange* parentItem = NULL;
	CTreeItemExchange* childItem = NULL;

    if (!hasIndex(row, column, parent))
	{
        return QModelIndex();
	}

    if (!parent.isValid())
	{
        parentItem = m_pRootItemCopy;
	}
    else
	{
        parentItem = static_cast<CTreeItemExchange*>(parent.internalPointer());
	}

    childItem = parentItem->child(row);
    if (childItem)
	{
        return createIndex(row, column, childItem);
	}
    else
	{
        return QModelIndex();
	}

	return QModelIndex();
}



QModelIndex CTreeModelExchange::parent(const QModelIndex &index) const
{
	CTreeItemExchange* childItem = NULL;
	CTreeItemExchange* parentItem = NULL;

    if (!index.isValid())
	{
        return QModelIndex();
	}

    childItem = static_cast<CTreeItemExchange*>(index.internalPointer());
    parentItem = childItem->parent();

    if (parentItem == m_pRootItemCopy)
	{
        return QModelIndex();
	}

    return createIndex(parentItem->row(), 0, parentItem);
}



int CTreeModelExchange::rowCount(const QModelIndex &parent) const
{
    CTreeItemExchange* parentItem = NULL;
    if (parent.column() > 0)
	{
        return 0;
	}

    if (!parent.isValid())
	{
        parentItem = m_pRootItemCopy;
	}
    else
	{
        parentItem = static_cast<CTreeItemExchange*>(parent.internalPointer());
	}

    return parentItem->childCount();
}

void CTreeModelExchange::setRootItem( CTreeItemExchange* rootItem )
{
	m_pRootItemCopy = rootItem;
	reset();
}


NS_END(TA_App_App)







#if 0



void CClientTestCase::test_build_exchange_info_data()
{
	//Data
	TreeItem* pRoot = NULL;
	TreeItem* pRootColumn = NULL;
	TreeItem* pItemNewTmp = NULL;
	TreeItem* pItemParentTmp = NULL;

	QList<QVariant> dataTmp;
	dataTmp.clear();
	dataTmp.push_back("root");
	pRoot = new TreeItem(dataTmp, NULL);


	dataTmp.clear();
	dataTmp.push_back("root_column");
	pRootColumn = new TreeItem(dataTmp, pRoot);
	pRoot->appendChild(pRootColumn);


	dataTmp.clear();
	dataTmp.push_back("AAAA");
	pItemNewTmp = new TreeItem(dataTmp, pRootColumn);
	pRootColumn->appendChild(pItemNewTmp);
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
	pItemNewTmp = new TreeItem(dataTmp, pRootColumn);
	pRootColumn->appendChild(pItemNewTmp);
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
	treeViewTmp->setRootIndex(pTreeModel->index(0, 0, QModelIndex()));
	splitter->setWindowTitle("mvc tree view test");
	splitter->setWindowTitle(QObject::tr("Simple Tree Model"));

	splitter->show();

}

#endif



















