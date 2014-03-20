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
	Qt::ItemFlags nFlagsTreeItem;
	CTreeItemExchange* pTreeItem = NULL;
	enDataTypeExchange nDataTypeExchange = DataType_InstrumentCode;

    if (!index.isValid())
	{
		nFlagsTreeItem = Qt::NoItemFlags;
		return nFlagsTreeItem;
	}

    //return Qt::ItemIsEnabled | Qt::ItemIsSelectable;

	pTreeItem = static_cast<CTreeItemExchange*>(index.internalPointer());

	if (NULL == pTreeItem)
	{
		nFlagsTreeItem = Qt::NoItemFlags;
		return nFlagsTreeItem;
	}

	nDataTypeExchange = pTreeItem->getDataType();
	if (DataType_InstrumentCode == nDataTypeExchange)
	{
		//nFlagsTreeItem = Qt::ItemIsEditable;
		nFlagsTreeItem = Qt::ItemIsEnabled | Qt::ItemIsSelectable |  Qt::ItemIsEditable;
		return nFlagsTreeItem;
	}

	nFlagsTreeItem = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
	return nFlagsTreeItem;
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


bool CTreeModelExchange::removeRows( int position, int rows, const QModelIndex &parent /*= QModelIndex()*/ )
{
	CTreeItemExchange* parentItem = _GetItem(parent);
	bool success = true;
	int nRemoveRowNum = 1;

	beginRemoveRows(parent, position, position + nRemoveRowNum - 1);
	success = parentItem->removeChildren(position);
	endRemoveRows();

	return success;
}


CTreeItemExchange* CTreeModelExchange::_GetItem( const QModelIndex& index ) const
{
	CTreeItemExchange *item = NULL;

	if (index.isValid()) 
	{
		item = static_cast<CTreeItemExchange*>(index.internalPointer());
		if (NULL != item) 
		{
			return item;
		}
	}

	return m_pRootItemCopy;
}


NS_END(TA_App_App)



















