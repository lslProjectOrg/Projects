#include <QtCore/QStringList>

#include "TreeItem.h"

NS_BEGIN(TA_App_App)

TreeItem::TreeItem(const QList<QVariant> &data, TreeItem *parent /*= 0*/ )
{
	m_pParentItem = parent;
	m_ItemData = data;
}


TreeItem::~TreeItem()
{
    qDeleteAll(m_LstChildItems);
	m_LstChildItems.clear();
}

void TreeItem::appendChild(TreeItem *item)
{
    m_LstChildItems.append(item);
}

TreeItem* TreeItem::child(int row)
{
    return m_LstChildItems.value(row);
}

int TreeItem::childCount() const
{
    return m_LstChildItems.count();
}

int TreeItem::columnCount() const
{
    return m_ItemData.count();
}

QVariant TreeItem::data(int column) const
{
    return m_ItemData.value(column);
}

TreeItem* TreeItem::parent()
{
    return m_pParentItem;
}

int TreeItem::childNumber() const
{
	int nIndex = 0;

    if (NULL != m_pParentItem)
	{
		//nIndex = m_pParentItem->m_LstChildItems.indexOf(const_cast<TreeItem*>(this));
		nIndex = m_pParentItem->indexOfChildren(const_cast<TreeItem*>(this));
        return nIndex;
	}

    return nIndex;
}

int TreeItem::indexOfChildren(TreeItem* pChildrenItem) const
{
	int nIndex = 0;

	if (m_LstChildItems.empty())
	{
		//error
		nIndex = -1;
		return nIndex;
	}

	nIndex = m_LstChildItems.indexOf(const_cast<TreeItem*>(pChildrenItem));

	return nIndex;
}


bool TreeItem::insertChildren(int position, int count, int columns)
{
	if (position < 0 || position > m_LstChildItems.size())
	{
		return false;
	}

	for (int row = 0; row < count; ++row) 
	{
		QList<QVariant> data;

		for (int nColumn = 0; nColumn < columns; nColumn++)
		{
			data.push_back("");
		}
		TreeItem *item = new TreeItem(data, this);
		m_LstChildItems.insert(position, item);
	}

	return true;
}


bool TreeItem::insertColumns(int position, int columns)
{
	if (position < 0 || position > m_ItemData.size())
	{
		return false;
	}

	for (int column = 0; column < columns; ++column)
	{
		m_ItemData.insert(position, QVariant());
	}

	foreach (TreeItem *child, m_LstChildItems)
	{
		child->insertColumns(position, columns);
	}

	return true;
}

bool TreeItem::removeChildren(int position, int count)
{
	if (position < 0 || position + count > m_LstChildItems.size())
	{
		return false;
	}

	for (int row = 0; row < count; ++row)
	{
		delete m_LstChildItems.takeAt(position);
	}

	return true;
}


bool TreeItem::removeColumns(int position, int columns)
{
	QList<QVariant>::Iterator iterList;

	if (position < 0 || position + columns > m_ItemData.size())
	{
		return false;
	}

	for (int column = 0; column < columns; ++column)
	{
		//m_ItemData.remove(position);

		iterList = m_ItemData.begin();
		iterList += position;
		m_ItemData.erase(iterList);
	}

	foreach (TreeItem *child, m_LstChildItems)
	{
		child->removeColumns(position, columns);
	}

	return true;
}

bool TreeItem::setData(int column, const QVariant &dataValue)
{
	if (column < 0 || column >= m_ItemData.size())
	{
		return false;
	}

	m_ItemData[column] = dataValue;

	return true;
}
NS_END(TA_App_App)

