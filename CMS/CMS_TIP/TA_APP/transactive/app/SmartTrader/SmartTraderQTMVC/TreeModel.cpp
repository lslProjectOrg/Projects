#include "TreeModel.h"


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
#include <QtCore/QVariant>

//gui
#include <QtGui/QHBoxLayout>
#include <QtGui/QSlider>
#include <QtGui/QSpinBox>
#include <QtGui/QSplitter>

#include <QtGui/QListView>
#include <QtGui/QTableView>
#include <QtGui/QTreeView>

#include "TreeItem.h"

NS_BEGIN(TA_App_App)

TreeModel::TreeModel(QObject *parent)
    : QAbstractItemModel(parent)
{
	m_pRootItemCopy = NULL;
}


TreeModel::~TreeModel()
{
	m_pRootItemCopy = NULL;
}



int TreeModel::columnCount(const QModelIndex &parent) const
{
	int nColumnCount = 0;

    if (parent.isValid())
	{
        nColumnCount = static_cast<TreeItem*>(parent.internalPointer())->columnCount();
	}
    else
	{
        nColumnCount = m_pRootItemCopy->columnCount();
	}

	return nColumnCount;
}



QVariant TreeModel::data(const QModelIndex &index, int role) const
{
	TreeItem* item = NULL;

    if (!index.isValid())
	{
        return QVariant();
	}

    if (role != Qt::DisplayRole)
	{
        return QVariant();
	}

    item = static_cast<TreeItem*>(index.internalPointer());

    return item->data(index.column());
}



Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return 0;

	return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}



QVariant TreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
	{
		//return QVariant();
        //return rootItemCopy->data(section);
		return QVariant("root_column");
	}

    return QVariant();
}



QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
	TreeItem* parentItem = NULL;
	TreeItem* childItem = NULL;

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
        parentItem = static_cast<TreeItem*>(parent.internalPointer());
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



QModelIndex TreeModel::parent(const QModelIndex &index) const
{
	TreeItem* childItem = NULL;
	TreeItem* parentItem = NULL;

    if (!index.isValid())
	{
        return QModelIndex();
	}

    childItem = static_cast<TreeItem*>(index.internalPointer());
    parentItem = childItem->parent();

    if (parentItem == m_pRootItemCopy)
	{
        return QModelIndex();
	}

    return createIndex(parentItem->childNumber(), 0, parentItem);
}



int TreeModel::rowCount(const QModelIndex &parent) const
{
    TreeItem* parentItem = NULL;
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
        parentItem = static_cast<TreeItem*>(parent.internalPointer());
	}

    return parentItem->childCount();
}

void TreeModel::setRootItem( TreeItem* rootItem )
{
	m_pRootItemCopy = rootItem;
	reset();
}

TreeItem* TreeModel::getItem( const QModelIndex& index ) const
{
	if (index.isValid()) 
	{
		TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
		if (item) 
		{
			return item;
		}
	}

	return m_pRootItemCopy;
}

bool TreeModel::setData( const QModelIndex &index, const QVariant &value, int role /*= Qt::EditRole*/ )
{
	if (role != Qt::EditRole)
		return false;

	TreeItem *item = getItem(index);
	bool result = item->setData(index.column(), value);

	if (result)
		emit dataChanged(index, index);

	return result;
}

bool TreeModel::setHeaderData( int section, Qt::Orientation orientation, const QVariant &value, int role /*= Qt::EditRole*/ )
{
	if (role != Qt::EditRole || orientation != Qt::Horizontal)
		return false;

	bool result = m_pRootItemCopy->setData(section, value);

	if (result)
		emit headerDataChanged(orientation, section, section);

	return result;
}

bool TreeModel::insertColumns( int position, int columns, const QModelIndex &parent /*= QModelIndex()*/ )
{
	bool success;

	beginInsertColumns(parent, position, position + columns - 1);
	success = m_pRootItemCopy->insertColumns(position, columns);
	endInsertColumns();

	return success;
}

bool TreeModel::removeColumns( int position, int columns, const QModelIndex &parent /*= QModelIndex()*/ )
{
	bool success;

	beginRemoveColumns(parent, position, position + columns - 1);
	success = m_pRootItemCopy->removeColumns(position, columns);
	endRemoveColumns();

	if (m_pRootItemCopy->columnCount() == 0)
		removeRows(0, rowCount());

	return success;
}

bool TreeModel::insertRows( int position, int rows, const QModelIndex &parent /*= QModelIndex()*/ )
{
	TreeItem *parentItem = getItem(parent);
	bool success;

	beginInsertRows(parent, position, position + rows - 1);
	success = parentItem->insertChildren(position, rows, m_pRootItemCopy->columnCount());
	endInsertRows();

	return success;
}

bool TreeModel::removeRows( int position, int rows, const QModelIndex &parent /*= QModelIndex()*/ )
{
	TreeItem *parentItem = getItem(parent);
	bool success = true;

	beginRemoveRows(parent, position, position + rows - 1);
	success = parentItem->removeChildren(position, rows);
	endRemoveRows();

	return success;
}



NS_END(TA_App_App)















