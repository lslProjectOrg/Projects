#include "ConfigInfoHelper.h"
#include "FileInfo.h"
#include "ParamInfo.h"
#include <boost/algorithm/string.hpp>

#include "BoostLogger.h"
USING_BOOST_LOG;

NS_BEGIN(TA_Base_App)



CConfigInfoHelper::CConfigInfoHelper( const std::string& strConfigFileName )
{
	BOOST_LOG_FUNCTION();

	m_strConfigFileName = strConfigFileName;
	m_pConfigFileInfo = new CFileInfo(m_strConfigFileName);

	_GetConifgFileInfo();
}

CConfigInfoHelper::~CConfigInfoHelper(void)
{
	BOOST_LOG_FUNCTION();

	_FreeData(m_MapParamKeyParamItem);
	m_MapParamKeyParamItem.clear();

	if (NULL != m_pConfigFileInfo)
	{
		delete m_pConfigFileInfo;
		m_pConfigFileInfo = NULL;
	}
}



void CConfigInfoHelper::_GetConifgFileInfo()
{
	BOOST_LOG_FUNCTION();

	CFileInfo::LstLineT  lstLine;
	CFileInfo::LstLineIterT  iterLst;

	m_pConfigFileInfo->getAllLinesInFile(lstLine);

	iterLst = lstLine.begin();
	while (iterLst != lstLine.end())
	{
		std::string strLineTmp = *iterLst;
		std::string strParamKey;

		CParamInfo* pParamItem = new CParamInfo(strLineTmp);
		strParamKey = pParamItem->getParamKey();
		m_MapParamKeyParamItem.insert(MapParamKeyParamItemValueTypeT(strParamKey, pParamItem));
		pParamItem = NULL;

		iterLst++;
	}//while


	lstLine.clear();

	return;
}

void CConfigInfoHelper::_FreeData(MapParamKeyParamItemT& mapParamKeyParamItemT)
{
	BOOST_LOG_FUNCTION();
	MapParamKeyParamItemIterT iterMap;
	CParamInfo* pParamItem = NULL;

	iterMap = mapParamKeyParamItemT.begin();
	while (iterMap != mapParamKeyParamItemT.end())
	{
		pParamItem = (iterMap->second);
		
		delete pParamItem;
		pParamItem = NULL;

		iterMap++;
	}

	mapParamKeyParamItemT.clear();

	return;
}


void CConfigInfoHelper::logInfo()
{
	BOOST_LOG_FUNCTION();



	return;
}

std::string CConfigInfoHelper::getStringParamValue( const std::string& strParamKey )
{
	std::string strParamValue;
	BOOST_LOG_FUNCTION();

	MapParamKeyParamItemIterT iterFind;
	CParamInfo* pParamItem = NULL;
	std::string strParamKeyUpper = strParamKey;
	boost::algorithm::trim(strParamKeyUpper);
	boost::algorithm::to_upper(strParamKeyUpper);

	iterFind = m_MapParamKeyParamItem.find(strParamKeyUpper);
	if (iterFind != m_MapParamKeyParamItem.end())
	{
		//find ok
		pParamItem = (iterFind->second);

		strParamValue = pParamItem->getStringParamValue();
	}

	return strParamValue;
}


NS_END(TA_Base_App)
















