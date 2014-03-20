#ifndef __CLASS_NODE_DATA_HH__
#define __CLASS_NODE_DATA_HH__

#include "ProjectCommonData.h"

//qt sys
#include <QtCore/QChar>
#include <QtCore/QString>
#include <QtCore/QDir>
#include <QtCore/QVariant>
#include <QtCore/QMap>

NS_BEGIN(TA_Base_App)

class NodeData 
{ 
public: 
	enum enDataType 
	{ 
		Root, 
		OrExpression, 
		AndExpression, 
		NotExpression, 
		Atom, 
		Identifier, 
		Operator, 
		Punctuator 
	}; 
public: 
	NodeData(enDataType nDataType, const QString &strDataValue); 
	virtual ~NodeData(); 
public:
	QString _GetDataString(enDataType nDataType);
public:
	enDataType    m_nDataType; 
	QString		  m_strDataType;
	QString		  m_strDataValue;
}; 


NS_END(TA_Base_App)




#endif//__CLASS_NODE_HH__



