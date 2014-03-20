#ifndef __CLASS_PARAM_INFO_H__
#define __CLASS_PARAM_INFO_H__ 


#include "ProjectCommonData.h"

#include "boost/tokenizer.hpp"

NS_BEGIN(TA_Base_App)

class CProjectUtilityFun;

class CParamInfo
{
public:
	//"Param=ParamValue"
	CParamInfo(const std::string& strParamLine);	
	~CParamInfo(void);
private:
	int _ProcessParamLine();

public:
	std::string getParamKey();
	std::string getStringParamValue();
private:
	std::string m_strParamLine;
	std::string m_strParam;
	std::string m_strParamValue;
private:
	CProjectUtilityFun* m_pUtilityFun;

};


NS_END(TA_Base_App)


#endif // __CLASS_PARAM_INFO_H__









