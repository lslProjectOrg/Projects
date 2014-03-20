#ifndef __CLASS_TREE_ITEM_EXCHANGE_HH__
#define __CLASS_TREE_ITEM_EXCHANGE_HH__


#include "ProjectCommonData.h"

//qt sys
#include <QtCore/QChar>
#include <QtCore/QString>
#include <QtCore/QDir>
#include <QtCore/QVariant>
#include <QtCore/QMap>
#include <QtCore/QObject>

NS_BEGIN(TA_App_App)

class CExchangeInfo;

class CTreeItemExchange
{
public:
    CTreeItemExchange(const QList<QVariant> &data, CTreeItemExchange *parent = NULL);
    ~CTreeItemExchange();

public:
	void appendChild(const QString& strMapKey, CTreeItemExchange** ppItem);
	CTreeItemExchange* child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    CTreeItemExchange* parent();
public:
	int indexOfChildren(CTreeItemExchange* pChildrenItem) const;
	void appendThreeChild(const CExchangeInfo* pExchangeInfo);
	void appendChildUnderlyingCode( const CExchangeInfo* pExchangeInfo );
	void appendChildInstrumentCode( const CExchangeInfo* pExchangeInfo );

private:
	void _AppendChildExchangeName( const CExchangeInfo* pExchangeInfo );

private:
	//use map for sort string
	QMap<QString, CTreeItemExchange*> m_MapStringChildren;
    QList<QVariant> m_ItemData;
    CTreeItemExchange* m_pParentItem;
};
NS_END(TA_App_App)


#endif//__CLASS_TREE_ITEM_EXCHANGE_HH__
