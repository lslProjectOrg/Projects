#ifndef __CLASS_CONFIG_INFO_HELPER_H__
#define __CLASS_CONFIG_INFO_HELPER_H__ 


#include "ProjectCommonData.h"


#include <boost/filesystem.hpp>
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"
#include "boost/filesystem/fstream.hpp"
#include "boost/filesystem/exception.hpp"
#include "boost/tokenizer.hpp"

NS_BEGIN(TA_Base_App)

class CFileInfo;
class CParamInfo;


class CConfigInfoHelper
{
private:
	typedef std::map<std::string, CParamInfo*>                      MapParamKeyParamItemT;
	typedef std::map<std::string, CParamInfo*>::iterator            MapParamKeyParamItemIterT;
	typedef std::map<std::string, CParamInfo*>::value_type          MapParamKeyParamItemValueTypeT;
public:
	CConfigInfoHelper(const std::string& strConfigFileName);	
	~CConfigInfoHelper(void);

public:
	void logInfo();
	std::string getStringParamValue(const std::string& strParamKey);
private:
	void _FreeData(MapParamKeyParamItemT& mapParamKeyParamItemT);
	void _GetConifgFileInfo();
private:
	std::string m_strConfigFileName;
	CFileInfo* m_pConfigFileInfo;
	MapParamKeyParamItemT  m_MapParamKeyParamItem;

};


NS_END(TA_Base_App)


#endif // __CLASS_CONFIG_INFO_HELPER_H__









