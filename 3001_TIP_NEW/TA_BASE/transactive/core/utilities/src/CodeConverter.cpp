#include "CodeConverter.h"
#include <memory.h>
#define UTFLEN(x)  utf8_look_for_table[(x)]

namespace TA_Base_Core
{
	unsigned char utf8_look_for_table[] = 
	{
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
		3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
		4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 1, 1
	};

#if defined(WIN32)
	UINT CodePage(const char * charset)
	{
		if(0 == strcmp(TA_UTF8, charset))
			return CP_UTF8;
		else if(0 == strcmp(TA_UTF7, charset))
			return CP_UTF7;
		else
			return CP_ACP;
	}
#endif

	CodeConverter::CodeConverter( const char *from_charset,const char *to_charset )
	{
#if !defined(WIN32)
		cd = iconv_open(to_charset,from_charset);
#else
		m_from_charset = CodePage(from_charset);
		m_to_charset = CodePage(to_charset);
#endif
	}

#if !defined(WIN32)
	int CodeConverter::convert( const char *inbuf,int& inlenleft,char *outbuf,int& outlen )
	{
		const char **pin = &inbuf;
		char **pout = &outbuf;
		int outlenLeft = outlen;
		memset(outbuf,0,outlen);
		int ret = iconv(cd, pin,(size_t *)&inlenleft,pout,(size_t *)&outlenLeft);
		outlen = outlen - outlenLeft;
		return ret;
	}
#else
	int CodeConverter::convert(const char *inbuf,int& inlenleft,char *outbuf,int& outlen)
	{     
		int ret = 0;
		size_t unicodeLen=MultiByteToWideChar(m_from_charset,0,inbuf,inlenleft,NULL,0);       

		/*if(unicodeLen <=0)
		return -1;*/

		wchar_t* pUnicode = NULL;  
		pUnicode=new wchar_t[unicodeLen+1]; 
		memset(pUnicode,0,(unicodeLen+1)*sizeof(wchar_t));       
		ret = MultiByteToWideChar(m_from_charset,0,inbuf,inlenleft,(LPWSTR)pUnicode,(int)unicodeLen);       

		BYTE* pTargetData = NULL;       
		int targetLen=WideCharToMultiByte(m_to_charset,0,(LPWSTR)pUnicode,-1, (char *)pTargetData,0,NULL,NULL);       

		/*if(targetLen <=0)
		return -2;*/

		pTargetData=new BYTE[targetLen + 1];       
		memset(pTargetData,0,targetLen + 1);       

		ret = WideCharToMultiByte(m_to_charset,0,(LPWSTR)pUnicode,-1, (char*)pTargetData,targetLen,NULL,NULL);       

		memcpy(outbuf, pTargetData, targetLen);
		outlen = targetLen - 1;

		delete [] pUnicode;       
		delete [] pTargetData;       

		return 0;
	}  

#endif

	CodeConverter::~CodeConverter()
	{
#if !defined(WIN32)
		iconv_close(cd);
#endif
	}

	int CodeConverter::getUtf8Length( char *str, int strLen /*= 0*/ )
	{
		int clen = strLen;
		if(clen <=0)
		{
			clen = strlen(str);
		}
		int len = 0;

		for(char *ptr = str; 
			*ptr!=0&&len<clen; 
			len++, ptr+=UTFLEN((unsigned char)*ptr));

		return len;
	}

#if defined(WIN32)

	std::wstring ANSIToUnicode ( const std::string& str )
	{
		int len = 0;
		len = str .length();
		int unicodeLen = ::MultiByteToWideChar( CP_ACP, 0, str.c_str(), -1, NULL, 0 );
		wchar_t * pUnicode ;
		pUnicode = new wchar_t[unicodeLen+1];
		memset(pUnicode ,0,(unicodeLen+1)* sizeof(wchar_t ));
		:: MultiByteToWideChar( CP_ACP , 0, str.c_str(), -1, pUnicode , unicodeLen );
		std::wstring rt ;
		rt = ( wchar_t *)pUnicode;
		delete pUnicode ;
		return rt ;
	}

	std::string UnicodeToANSI ( const std::wstring& str )
	{
		char* pElementText ;
		int iTextLen ;
		iTextLen = WideCharToMultiByte ( CP_ACP, 0, str.c_str (), -1, NULL, 0, NULL, NULL );
		pElementText = new char[ iTextLen + 1];
		memset( ( void * )pElementText, 0, sizeof( char ) * ( iTextLen + 1 ) );
		:: WideCharToMultiByte( CP_ACP , 0, str.c_str (), -1, pElementText, iTextLen, NULL, NULL);
		std::string strText ;
		strText = pElementText ;
		delete[] pElementText ;
		return strText ;
	}

	std::wstring UTF8ToUnicode ( const std::string& str )
	{
		int len = 0;
		len = str .length();
		int unicodeLen = ::MultiByteToWideChar( CP_UTF8, 0, str.c_str(), -1, NULL, 0 );
		wchar_t * pUnicode ;
		pUnicode = new wchar_t[ unicodeLen+1 ];
		memset(pUnicode ,0,(unicodeLen+1)* sizeof(wchar_t ));
		:: MultiByteToWideChar( CP_UTF8 , 0, str.c_str (), -1, pUnicode, unicodeLen );
		std::wstring rt ;
		rt = ( wchar_t * )pUnicode;
		delete pUnicode ;
		return rt ;
	}

	std::string UnicodeToUTF8 ( const std::wstring& str )
	{
		char* pElementText ;
		int iTextLen ;
		iTextLen = WideCharToMultiByte ( CP_UTF8, 0, str.c_str (), -1, NULL, 0, NULL, NULL );
		pElementText = new char[ iTextLen + 1];
		memset( ( void * )pElementText, 0, sizeof( char ) * ( iTextLen + 1 ) );
		:: WideCharToMultiByte( CP_UTF8, 0, str.c_str(), -1, pElementText, iTextLen, NULL, NULL );
		std::string strText ;
		strText = pElementText ;
		delete[] pElementText ;
		return strText ;
	}

#endif

}