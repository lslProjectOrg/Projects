#include "TreeItemExchange.h"


//qt sys
#include <QtCore/QChar>
#include <QtCore/QString>
#include <QtCore/QDir>
#include <QtCore/QVariant>
#include <QtCore/QMap>
#include <QtCore/QObject>


#include "ExchangeInfo.h"

#include "BoostLogger.h"
USING_BOOST_LOG;

NS_BEGIN(TA_App_App)

CTreeItemExchange::CTreeItemExchange(const QList<QVariant> &data, CTreeItemExchange *parent /*= 0*/ )
{
	m_pParentItem = NULL;
	m_ItemData.clear();
	m_MapStringChildren.clear();

	m_pParentItem = parent;
	m_ItemData = data;
}


CTreeItemExchange::~CTreeItemExchange()
{
	CTreeItemExchange* pTreeItem = NULL;
	QMap<QString, CTreeItemExchange*>::iterator iterMap;

	iterMap = m_MapStringChildren.begin();
	while (iterMap != m_MapStringChildren.end())
	{
		pTreeItem = iterMap.value();

		if (NULL != pTreeItem)
		{
			delete pTreeItem;
			pTreeItem = NULL;
		}

		iterMap++;
	}
	m_MapStringChildren.clear();


	m_pParentItem = NULL;
	m_ItemData.clear();

}

void CTreeItemExchange::appendChild(const QString& strMapKey, CTreeItemExchange** ppItem)
{
	CTreeItemExchange* pItem = NULL;
	if (NULL == ppItem || NULL == *ppItem || strMapKey.isEmpty())
	{
		return;
	}
	pItem = *ppItem;

	if (m_MapStringChildren.contains(strMapKey))
	{
		delete pItem;
		pItem = NULL;
	}
	else
	{
		m_MapStringChildren.insert(strMapKey, pItem);
	}
	

}

CTreeItemExchange* CTreeItemExchange::child(int row)
{
  //  return m_LstChildItems.value(row);

	int nIndex = 0;
	CTreeItemExchange* pTreeItem = NULL;
	QMap<QString, CTreeItemExchange*>::iterator iterMap;

	iterMap = m_MapStringChildren.begin();
	nIndex = 0;
	while (iterMap != m_MapStringChildren.end())
	{
		if (nIndex == row)
		{
			pTreeItem = iterMap.value();
			break;
		}
		nIndex++;
		iterMap++;
	}

	return pTreeItem;
}

int CTreeItemExchange::childCount() const
{
    //return m_LstChildItems.count();
	return m_MapStringChildren.size();
}

int CTreeItemExchange::columnCount() const
{
    return m_ItemData.count();
}

QVariant CTreeItemExchange::data(int column) const
{
    return m_ItemData.value(column);
}

CTreeItemExchange* CTreeItemExchange::parent()
{
    return m_pParentItem;
}

int CTreeItemExchange::row() const
{
	int nIndex = 0;

    if (NULL != m_pParentItem)
	{
		//nIndex = m_pParentItem->m_LstChildItems.indexOf(const_cast<TreeItem*>(this));
		nIndex = m_pParentItem->indexOfChildren(const_cast<CTreeItemExchange*>(this));
        return nIndex;
	}

    return nIndex;
}

int CTreeItemExchange::indexOfChildren(CTreeItemExchange* pChildrenItem) const
{
	int nIndex = 0;
	CTreeItemExchange* pTreeItemTmp = NULL;
	QMap<QString,CTreeItemExchange*>::const_iterator iterMap;
	
	iterMap = m_MapStringChildren.constBegin();
	nIndex = 0;
	while (iterMap != m_MapStringChildren.constEnd())
	{
		pTreeItemTmp = iterMap.value();

		if (pChildrenItem == pTreeItemTmp)
		{
			break;
		}
		nIndex++;
		iterMap++;
	}

	return nIndex;
}

void CTreeItemExchange::appendThreeChild(const CExchangeInfo* pExchangeInfo)
{
	_AppendChildExchangeName(pExchangeInfo);
}

void CTreeItemExchange::_AppendChildExchangeName( const CExchangeInfo* pExchangeInfo )
{
	QString strItemValue;
	QList<QVariant> dataTreeItem;
	CTreeItemExchange* pTreeItemExchangeName = NULL;
	QMap<QString, CTreeItemExchange*>::iterator iterFind;
	strItemValue = pExchangeInfo->getExchangeName();

	dataTreeItem.clear();
	dataTreeItem.push_back(strItemValue);
	pTreeItemExchangeName = new CTreeItemExchange(dataTreeItem, this);

	this->appendChild(strItemValue, &pTreeItemExchangeName);
	pTreeItemExchangeName = NULL;


	iterFind = m_MapStringChildren.find(strItemValue);
	if (iterFind != m_MapStringChildren.end())
	{
		pTreeItemExchangeName = iterFind.value();
		pTreeItemExchangeName->appendChildUnderlyingCode(pExchangeInfo);
	}


}
void CTreeItemExchange::appendChildUnderlyingCode( const CExchangeInfo* pExchangeInfo )
{
	QString strItemValue;
	QList<QVariant> dataTreeItem;
	CTreeItemExchange* pTreeItemUnderlyingCode = NULL;
	QMap<QString, CTreeItemExchange*>::iterator iterFind;

	strItemValue = pExchangeInfo->getUnderlyingCode();

	dataTreeItem.clear();
	dataTreeItem.push_back(strItemValue);
	pTreeItemUnderlyingCode = new CTreeItemExchange(dataTreeItem, this);

	this->appendChild(strItemValue, &pTreeItemUnderlyingCode);
	pTreeItemUnderlyingCode = NULL;


	iterFind = m_MapStringChildren.find(strItemValue);
	if (iterFind != m_MapStringChildren.end())
	{
		pTreeItemUnderlyingCode = iterFind.value();
		pTreeItemUnderlyingCode->appendChildInstrumentCode(pExchangeInfo);
	}



}

void CTreeItemExchange::appendChildInstrumentCode( const CExchangeInfo* pExchangeInfo )
{	

	QString strItemValue;
	QList<QVariant> dataTreeItem;
	CTreeItemExchange* pTreeItemInstrumentCode = NULL;
	QMap<QString, CTreeItemExchange*>::iterator iterFind;

	strItemValue = pExchangeInfo->getUnderlyingCode();

	dataTreeItem.clear();
	dataTreeItem.push_back(strItemValue);
	pTreeItemInstrumentCode = new CTreeItemExchange(dataTreeItem, this);

	this->appendChild(strItemValue, &pTreeItemInstrumentCode);
	pTreeItemInstrumentCode = NULL;

}

NS_END(TA_App_App)

