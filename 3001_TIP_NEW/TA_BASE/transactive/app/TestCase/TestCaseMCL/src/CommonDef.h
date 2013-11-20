/**
  *
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source: $File: //COTS/sqlgen/1.1.0/src/CommonDef.h $
  * @author Shenglong.Lin
  * @version $Revision: #1 $
  * Last modification: $DateTime: 2013/06/21 12:00:00 $
  * Last modified by: Shenglong.Lin $
  * 
  * to define some common use macro or constant static variables used in the sqlgen
  *
  */
  
  
#ifndef __DEF_COMMONDEF_HH__
#define __DEF_COMMONDEF_HH__




//==================================================================================
#define  LINE_ADD					  '+'
#define  LINE_COMMONT				  ';'
#define  LINE_END				      '\r'

#define  DB_NUM						  3
#define	 DB_COMMON					  "[[common]]"
#define	 DB_ORACLE			          "[[oracle]]"
#define	 DB_MYSQL			          "[[mysql]]"



#define  defSQLFilename_OUT_ERRORREPORT				"LogFileName.log"  /*OUT*/

//==================================================================================


#define	 DEF_INT_BUFFSIZE				  512
#define	 DEF_INT_MAXLINELEN				  8192



//=========================================================

#define DEF_DELETE_ARRAY(pArray)    if (pArray) { delete [](pArray); (pArray) = NULL; }
#define DEF_DELETE(p)   if (p) { delete (p); (p) = NULL; }
//=========================================================


#endif  // __DEF_COMMONDEF_HH__














