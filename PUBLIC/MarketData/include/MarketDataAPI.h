#pragma  once
#ifdef _WIN32
#ifdef MARKETDATA_EXPORTS
#define CTS_PRICE __declspec (dllexport)
#else
#define CTS_PRICE __declspec (dllimport)
#endif
#else 
#define CTS_PRICE
#endif

