#ifndef __CLASS_TREE_MODEL_EXCHANGE_HH__
#define __CLASS_TREE_MODEL_EXCHANGE_HH__

#include "ProjectCommonData.h"


#include <QtCore/QAbstractItemModel>
#include <QtCore/QModelIndex>
#include <QtCore/QVariant>


NS_BEGIN(TA_App_App)

class CTreeItemExchange;

class CTreeModelExchange : public QAbstractItemModel
{
    //Q_OBJECT

public:
    CTreeModelExchange(QObject* parent = 0);
    ~CTreeModelExchange();
public:
    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
public:
	bool removeRows( int position, int rows, const QModelIndex &parent /*= QModelIndex()*/ );

public:
	void setRootItem(CTreeItemExchange* rootItem);
private:
	CTreeItemExchange* _GetItem( const QModelIndex& index ) const;
private:
    CTreeItemExchange* m_pRootItemCopy;
};
NS_END(TA_App_App)

#endif//__CLASS_TREE_MODEL_EXCHANGE_HH__
