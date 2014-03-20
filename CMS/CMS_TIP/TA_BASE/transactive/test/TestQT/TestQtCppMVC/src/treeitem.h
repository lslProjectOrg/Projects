#ifndef TREEITEM_H
#define TREEITEM_H


#include "ProjectCommonData.h"

#include <QtCore/QList>
#include <QtCore/QVariant>

NS_BEGIN(TA_Base_App)

//! [0]
class TreeItem
{
public:
    TreeItem(const QList<QVariant> &data, TreeItem *parent = 0);
    ~TreeItem();

    void appendChild(TreeItem *child);

    TreeItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    TreeItem *parent();

private:
    QList<TreeItem*> childItems;
    QList<QVariant> itemData;
    TreeItem *parentItem;
};
NS_END(TA_Base_App)


#endif
