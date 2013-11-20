
#ifndef __DEF__UTILITY_FUN_H__
#define __DEF__UTILITY_FUN_H__ 

#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <functional>

#include "CommonDef.h"
#include "CommonData.h"

#include "core/synchronisation/src/NonReEntrantThreadLockable.h"


NS_BEGIN(TA_Base_App)


class CUtilityFun
{

public:
	static CUtilityFun& getInstance();
	static void removeInstance();

private:
	static CUtilityFun*                      m_pInstance;
 	static TA_Base_Core::NonReEntrantThreadLockable	     m_instanceLock;
private:
	CUtilityFun(void);	
	~CUtilityFun(void);

	DEF_DISABLE_COPY_AND_ASSIGNMENT(CUtilityFun);



public:
	/// \brief Converts anything you can insert into a C++ stream to a
	/// \c std::string via \c std::ostringstream.
	template <class T>
	inline std::string stream2string(const T& object)
	{
		std::ostringstream str;
		str << object;
		return str.str();
	}

	// trim from start(left)
	std::string& mysys_ltrim(std::string& s);

	// trim from end(right)
	std::string& mysys_rtrim(std::string& s);

	// trim from both end
	std::string& mysys_trim(std::string& s);



public:
	char*			mysys_strlwr(char* pszSrc);
	char*			mysys_strtolwr(char* pszSrc);
	char*			mysys_strupper(char* pszSrc);
	char*			mysys_strtoupper(char* pszSrc);
	int				mysys_StdStrToUpper(std::string &strSrc);
	int				mysys_StdStrToLwr(std::string &strSrc);



	std::string		mysys_int2str(unsigned nVal);
	void			mysys_int2str(int uVal, char sz[]);
	int				mysys_str2Int32(const std::string& strInt32);
	int				mysys_stricmp(const char* pzLeft, const char* pzRight);
	int				mysys_strnicmp(const char *pzLeft, const char *pzRight, size_t count);	

	std::string		mysys_sysDecToHex(int i);
	int				mysys_sysHexToDec(char cLeft, char cRight);


public:
	char*			mysys_strncpy(char* dest, const char* src, size_t length);
	void*			mysys_malloc(size_t nSize);
	void			mysys_free(void *);
	int				mysys_getVisableStdStr(std::string& strSrc);
	int				mysys_getVisableStr(char* Str);


public:
	int				readAllLineFromFile(const std::string& strFileName, VrtLinesConT& vrtAllLines);
	int				removeCommentInAllLine(VrtLinesConT& vrtAllLines);	 
	int				removeNoDataLineInAllLine(VrtLinesConT& vrtAllLines);

#ifdef  SYS_USE_BOOST

public:
	std::string		mysys_boost_GetTimeStr(unsigned int uTimeVale);
	double			mysys_boost_strTodouble(std::string& strNumber);
	int				mysys_boost_strToint(std::string& strNumber);
	std::string		mysys_boost_intTostr(int nVal);
	std::string     mysys_boost_unsignedintTostr(unsigned int nVal);
	int				mysys_boost_stricmp(std::string& strLeft, std::string& strRight);
	int				mysys_boost_trim(std::string& strSrc);
	int				mysys_boost_ToUpper(std::string &strSrc);
	int				mysys_boost_ToLwr(std::string &strSrc);
	void			mysys_boost_sleep(int sec, int nsec);

#endif

};


NS_END(TA_Base_App)


#endif // __DEF__UTILITY_FUN_H__









