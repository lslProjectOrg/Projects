#ifndef TREEMODEL_H
#define TREEMODEL_H

#include "ProjectCommonData.h"


#include <QtCore/QAbstractItemModel>
#include <QtCore/QModelIndex>
#include <QtCore/QVariant>


NS_BEGIN(TA_Base_App)

class TreeItem;

class TreeModel : public QAbstractItemModel
{
    //Q_OBJECT

public:
    TreeModel(QObject* parent = 0);
    ~TreeModel();

    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
public:
	void setRootItem(TreeItem* rootItem);
private:
    TreeItem *rootItemCopy;
};
NS_END(TA_Base_App)

#endif
