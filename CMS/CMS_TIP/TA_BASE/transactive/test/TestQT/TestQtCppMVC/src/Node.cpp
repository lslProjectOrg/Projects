#include "Node.h"

#include "BoostLogger.h"
USING_BOOST_LOG;

NS_BEGIN(TA_Base_App)

Node::Node(NodeData::enDataType nDataType, const QString &strDataValue)
{
	m_pNodeData = NULL;
	m_pNodeData = new NodeData(nDataType, strDataValue);
	m_pParentNode = NULL;
	m_pChildrenNodeLst.clear();
}

Node::~Node() 
{ 
	if (NULL != m_pNodeData)
	{
		delete m_pNodeData;
		m_pNodeData = NULL;
	}
	qDeleteAll(m_pChildrenNodeLst); 
	m_pChildrenNodeLst.clear();

	/*
	while (begin != end) 
	{
		delete *begin;
		++begin;
	}
	*/
}


NS_END(TA_Base_App)












