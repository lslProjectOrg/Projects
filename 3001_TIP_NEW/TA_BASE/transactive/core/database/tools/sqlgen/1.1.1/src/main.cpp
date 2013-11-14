//#include "vld.h"
#include "GenerateSQLFile.h"

///export/home/tabuild/3001_TIP_NEW/Base_3001/Base/code/cots/sqlgen/1.1.0/bin/SOLARIS_5.10_i86pc/sqlgen --Inputfile=./src/sqlcode.txt --Outpath=./src
   
int main(int argc,char *argv[])
{
	int nFunRes = 0;
	CGenerateSQLFile *pGenerateSQLFile = NULL;
	
	pGenerateSQLFile = new CGenerateSQLFile();
	
	if (NULL != pGenerateSQLFile)
	{
		nFunRes = pGenerateSQLFile->setParam(argc, argv);
		nFunRes = pGenerateSQLFile->generalFiles();
	}

	if (NULL != pGenerateSQLFile)
	{
		delete pGenerateSQLFile;
		pGenerateSQLFile = NULL;
	}
	
	return 0;
}










