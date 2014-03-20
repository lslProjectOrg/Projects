#ifndef TREEMODEL_H
#define TREEMODEL_H

#include "ProjectCommonData.h"


#include <QtCore/QAbstractItemModel>
#include <QtCore/QModelIndex>
#include <QtCore/QVariant>

NS_BEGIN(TA_App_App)

class TreeItem;

class TreeModel : public QAbstractItemModel
{
    //Q_OBJECT

public:
    TreeModel(QObject* parent = 0);
    ~TreeModel();
public:
    QVariant data(const QModelIndex &index, int role) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const;
    
	QModelIndex parent(const QModelIndex& index) const;
    
	int rowCount(const QModelIndex& parent = QModelIndex()) const;
    
	int columnCount(const QModelIndex& parent = QModelIndex()) const;
	
	Qt::ItemFlags flags(const QModelIndex& index) const;

public:	
	bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

	bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole);

	bool insertColumns(int position, int columns, const QModelIndex &parent = QModelIndex());
	
	bool removeColumns(int position, int columns, const QModelIndex &parent = QModelIndex());
	
	bool insertRows(int position, int rows, const QModelIndex &parent = QModelIndex());
	
	bool removeRows(int position, int rows, const QModelIndex &parent = QModelIndex());

public:
	void setRootItem(TreeItem* rootItem);
private:
	TreeItem* getItem(const QModelIndex& index) const;

    TreeItem* m_pRootItemCopy;
};
NS_END(TA_App_App)

#endif
