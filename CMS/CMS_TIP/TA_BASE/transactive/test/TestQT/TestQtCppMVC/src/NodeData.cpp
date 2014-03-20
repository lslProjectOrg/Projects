#include "NodeData.h"

#include "QtCore/QObject"
#include "BoostLogger.h"
USING_BOOST_LOG;

NS_BEGIN(TA_Base_App)

NodeData::NodeData(enDataType nDataType, const QString &strDataValue)
{
	this->m_nDataType = nDataType;
	this->m_strDataValue = strDataValue;
	this->m_strDataType = _GetDataString(m_nDataType);

 
}

NodeData::~NodeData() 
{ 

}

QString NodeData::_GetDataString( enDataType nDataType )
{
	QString strDataType;

	switch (nDataType) 
	{ 
	case NodeData::Root: 
		strDataType = QObject::tr("Root"); 
		break;
	case NodeData::OrExpression: 
		strDataType =  QObject::tr("OR Expression"); 
		break;
	case NodeData::AndExpression: 
		strDataType =  QObject::tr("AND Expression");
		break;
	case NodeData::NotExpression: 
		strDataType =  QObject::tr("NOT Expression"); 
	case NodeData::Atom: 
		strDataType =  QObject::tr("Atom"); 
		break;
	case NodeData::Identifier: 
		strDataType =  QObject::tr("Identifier");
		break;
	case NodeData::Operator: 
		strDataType =  QObject::tr("Operator"); 
		break;
	case NodeData::Punctuator: 
		strDataType =  QObject::tr("Punctuator"); 
		break;
	default: 
		strDataType =  QObject::tr("Unknown"); 
		break;
	} 

	return strDataType;

}


NS_END(TA_Base_App)












