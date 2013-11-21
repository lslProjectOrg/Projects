#ifndef __CLASS_DATA_BUFFER__HH__
#define __CLASS_DATA_BUFFER__HH__

#include "CommonData.h"

NS_BEGIN(TA_Base_App)

class CDataBuffer  
{
private:
	int		m_nBufferSize;			
	int		m_nBufferSizeLimit;
	int		m_nRemainDataLen;			
	char*	m_pBuffer;			
	char*	m_pWritePos;		
	char*	m_pReadPos;		
public:
	CDataBuffer(int nBufferSize = DEF_INT_BUFFER_LEN_READ);
	~CDataBuffer();
public:
	char* getWritePos(); 
	char* getReadPos();
	int getRemainDataLength();
	int getBlankSize();
	int getBufferSize();
	int getLimit();
	void print();

public:
	bool write(const void* src, int nLen);
	bool moveWritePos(int nLen);

	bool read(void* dest, int nLen);
	bool moveReadPos(int nLen);

	void clear();		

public:
	void trim();
	bool resize(int nSize);
	bool resizeFitData();
	void prepareSize(int nSize);	

};


NS_END(TA_Base_App)


#endif //__CLASS_DATA_BUFFER__HH__






