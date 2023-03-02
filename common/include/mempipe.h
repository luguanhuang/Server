#ifndef _Mem_Pipe_H__
#define _Mem_Pipe_H__


class CMemPipe
{
public: 
	CMemPipe();
	CMemPipe(size_t nSize);
	~CMemPipe();

	bool Init(size_t nSize);
	void UnInit();

	bool Write(const char* pData, size_t nLen);
	bool Read(char* pBuf, size_t nLen);

	bool Discard(size_t nLen);

private:
	char* m_pBuffer;
	char* volatile m_pHead;
	char* volatile m_pTail;
	size_t	m_nSize;
};

#endif