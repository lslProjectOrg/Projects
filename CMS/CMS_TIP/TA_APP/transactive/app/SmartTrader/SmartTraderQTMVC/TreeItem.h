#ifndef TREEITEM_H
#define TREEITEM_H


#include "ProjectCommonData.h"

#include <QtCore/QList>
#include <QtCore/QVariant>

NS_BEGIN(TA_App_App)

class TreeItem
{
public:
    TreeItem(const QList<QVariant>& data, TreeItem *parent = NULL);
    ~TreeItem();

    void appendChild(TreeItem *child);

    TreeItem* child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int childNumber() const;
    TreeItem* parent();

public:
	bool insertChildren(int position, int count, int columns);
	bool insertColumns(int position, int columns);
	bool removeChildren(int position, int count);
	bool removeColumns(int position, int columns);
	bool setData(int column, const QVariant &dataValue);

public:
	int indexOfChildren(TreeItem* pChildrenItem) const;
private:
    QList<TreeItem*> m_LstChildItems;
    QList<QVariant> m_ItemData;
    TreeItem* m_pParentItem;
};
NS_END(TA_App_App)


#endif
