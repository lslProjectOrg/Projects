/**
  *
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source: $File: //COTS/sqlgen/1.1.0/src/UtilityFun.h $
  * @author Shenglong.Lin
  * @version $Revision: #1 $
  * Last modification: $DateTime: 2013/06/21 12:00:00 $
  * Last modified by: Shenglong.Lin	$
  * 
  * to declare some useful methods used in sqlgen
  *
  */


#ifndef __DEF_UTILITYFUN_HH__
#define __DEF_UTILITYFUN_HH__

#include <string>
#include "CommonDef.h"
#include "CommonData.h"


class CUtilityFun
{
public:
	static CUtilityFun& getInstance();
	static void removeInstance();

private:
	static CUtilityFun* m_pInstance;
	
private:
	CUtilityFun();
	~CUtilityFun();
	
public:
	static char* sysStrLwr(char* pszSrc);
	static char* sysStrUpper(char* pszSrc);
	static std::string int2str(int nVal);
	static int sysStricmp(const char* pzLeft, const char* pzRight);
	
	// trim from start(left)
	static std::string& ltrim(std::string& s);
	// trim from end(right)
	static std::string& rtrim(std::string& s);
	// trim from both end
	static std::string& trim(std::string& s);

	//Remove UnVisable char in string, except ' '  0x09
	//remove '/r/n'
	int removeUnVisableStr(std::string& strSrc);

};




#endif // __DEF_UTILITYFUN_HH__















