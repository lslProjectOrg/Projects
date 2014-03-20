#ifndef __CLASS_CONFIG_INFO_H__
#define __CLASS_CONFIG_INFO_H__ 


#include "ProjectCommonData.h"

#include <boost/chrono.hpp>
#include <boost/thread.hpp>


NS_BEGIN(TA_Base_App)

class CConfigInfo
{
public:
	static CConfigInfo& getInstance();
	static void removeInstance();

private:
	static CConfigInfo* m_pInstance;
	static boost::mutex m_mutexInstance;

private:
	CConfigInfo(void);	
	~CConfigInfo(void);

public:
	void loadDefaultConfigInfo();
	void logInfo();
	std::string getStartTime();
private:
	std::string m_strConfigFileName;
	std::string m_strStartTime;


};//class


NS_END(TA_Base_App)


#endif // __CLASS_CONFIG_INFO_H__



















