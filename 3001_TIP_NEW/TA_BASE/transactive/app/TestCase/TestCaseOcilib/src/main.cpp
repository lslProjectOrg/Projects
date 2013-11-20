#include "vld.h"

///export/home/tabuild/3001_TIP_NEW/Base_3001/Base/code/cots/sqlgen/1.1.0/bin/SOLARIS_5.10_i86pc/sqlgen --Inputfile=./src/sqlcode.txt --Outpath=./src




#include "CommonDef.h"
#include "CommonData.h"

#include "TestClass.h"
#include "Logger.h"





int main(int argc,char *argv[])
{
	int nFunRes = 0;
	std::string strLogFileName = "TestOcilibLog.log";
	EDebugLevel nLoglevel = DebugDebug;
	std::string strTestClassName;
	CTestClass* pTestClase = NULL;


	CLogger::getInstance();
	CLogger::getInstance().setlogFile(strLogFileName);
	CLogger::getInstance().setlogLevel(nLoglevel);
	_SysLog(SourceFLInfo, DebugDebug, " start main");

	pTestClase = new CTestClass(strTestClassName);

	pTestClase->RunTestCase();

	DEF_DELETE(pTestClase);


	_SysLog(SourceFLInfo, DebugDebug, " end main");
	CLogger::removeInstance();	

	return 0;
}



