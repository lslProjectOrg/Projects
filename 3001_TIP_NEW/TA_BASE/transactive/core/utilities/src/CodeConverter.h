#ifndef CODECONVERTER_H_INCLUDED
#define CODECONVERTER_H_INCLUDED


#if !defined(WIN32)
#include <iconv.h>
#else
#include <windows.h>
#endif

#include <cstring>
#include <string>

#define TA_GB2312 "gb2312"
#define TA_UTF7 "utf-7"
#define TA_UTF8 "utf-8"
#define TA_UTF16 "utf-16"
#define TA_UTF16BE "utf-16be"
#define TA_UTF16LE "utf-16le"
#define TA_UTF32 "utf-32"
#define TA_UTF32BE "utf-32be"
#define TA_UTF32LE "utf-32le"

namespace TA_Base_Core
{


	class CodeConverter {

	public:
		CodeConverter(const char *from_charset,const char *to_charset);

		~CodeConverter();

		int convert(const char *inbuf,int& inlenleft,char *outbuf,int& outlenleft);

#if defined(WIN32)

		std::wstring ANSIToUnicode ( const std::string& str );

		std::string UnicodeToANSI ( const std::wstring& str );

		std::wstring UTF8ToUnicode ( const std::string& str );

		std::string UnicodeToUTF8 ( const std::wstring& str );

#endif
		static int getUtf8Length(char *str, int strLen = 0);

	private:

#if !defined(WIN32)
		iconv_t cd;
#else
		UINT m_from_charset;
		UINT m_to_charset;
#endif
	};
}

#endif
