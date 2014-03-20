#ifndef __CLASS_NODE_HH__
#define __CLASS_NODE_HH__

#include "ProjectCommonData.h"

//qt sys
#include <QtCore/QChar>
#include <QtCore/QString>
#include <QtCore/QDir>
#include <QtCore/QVariant>
#include <QtCore/QMap>

#include "NodeData.h"

NS_BEGIN(TA_Base_App)

class Node 
{ 
public: 
	Node(NodeData::enDataType nDataType, const QString &strDataValue); 
	virtual ~Node(); 
public:
	NodeData* m_pNodeData;
public:
	Node *m_pParentNode; 
	QList<Node *> m_pChildrenNodeLst; 
}; 


#if 0
class Node
{

public:
	Node();

	QString m_strCurrency;//"CHY"
	double m_fCurrencyValue;//6//1USD = 6CHY  
};
#endif


NS_END(TA_Base_App)




#endif//__CLASS_NODE_HH__



