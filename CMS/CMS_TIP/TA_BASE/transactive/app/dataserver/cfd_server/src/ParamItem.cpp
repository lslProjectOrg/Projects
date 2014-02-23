#include "ParamItem.h"
#include "CFDServerUtilityFun.h"
#include <sstream>
#include <boost/algorithm/string.hpp>

#include "BoostLogger.h"
USING_BOOST_LOG;

NS_BEGIN(TA_Base_App)

CParamItem::CParamItem( const std::string& strParamLine )
{
	m_strParamLine.clear();
	m_strParam.clear();
	m_strParamValue.clear();

	m_strParamLine = strParamLine;
	m_pUtilityFun = new CCFDServerUtilityFun();

	_ProcessParamLine();
}

CParamItem::~CParamItem( void )
{
	m_strParamLine.clear();
	m_strParam.clear();
	m_strParamValue.clear();

	if (NULL != m_pUtilityFun)
	{
		delete m_pUtilityFun;
		m_pUtilityFun = NULL;
	}
}
int CParamItem::_ProcessParamLine()
{
	int nFunRes = 0;

	typedef boost::tokenizer<boost::char_separator<char> > tokenizer; 
	boost::char_separator<char> sep("="); 
	tokenizer tok(m_strParamLine, sep); 
	for (tokenizer::iterator it = tok.begin(); it != tok.end(); ++it) 
	{
		if (m_strParam.empty())
		{
			m_strParam = (*it);
			boost::algorithm::trim(m_strParam);
			boost::algorithm::to_upper(m_strParam);
		}
		else
		{
			m_strParamValue = (*it);
		}
	}

	return nFunRes;
}
std::string CParamItem::getParamKey()
{
	return m_strParam;//upper
}

std::string CParamItem::getStringParamValue()
{
	return m_strParamValue;
}



NS_END(TA_Base_App)
















