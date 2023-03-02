#include "pch.h"
#include "ExcelReader.h"

CExcel::CExcel()
:m_pBuffer(NULL)
,m_nColCount(0)
,m_nRowCount(0)
{
	memset(m_pChunk, 0, sizeof(m_pChunk));
}

CExcel::~CExcel()
{
	free(m_pBuffer);
	for (int i = 0; i < 128; ++i)
	{
		if (NULL != m_pChunk[i])	
			free(m_pChunk[i]);
	}
}

bool CExcel::ReadExcelFile(const char* pszFileName)
{
	///> 打开文件
	FILE* pFile = fopen(pszFileName, "rb");
	if (NULL == pFile)
	{
		return false;
	}
	///> 获取文件大小
	fseek(pFile, 0L, SEEK_END);
	int nLength = (int)ftell(pFile);
	rewind(pFile);

	///> 把文件拷到缓存
	char* pBuffer = (char*)realloc(m_pBuffer, nLength + 1);
	if (NULL == pBuffer)
	{
		return false;
	}
	m_pBuffer = pBuffer;
	int nRead = (int)fread(pBuffer, 1, nLength, pFile);

	///> 关闭文件
	fclose(pFile);

	if (nRead != nLength)
	{
		return false;
	}

	pBuffer[nLength] = 0;
	m_nColCount = 0;
	for (int i = 0; i < nLength; ++i)
	{
		if ('\t' == pBuffer[i])
		{
			++m_nColCount;
		}
		else if (',' == pBuffer[i])
		{
			++m_nColCount;
		}
		else if ('\n' == pBuffer[i])
		{
			++m_nColCount;
			break;
		}
	}

	return AnalyszFile(pBuffer, nLength);
}

bool CExcel::AnalyszFile(char* pBuffer, int nBufferSize)
{
	bool bIsString = false;
	int& n = m_nRowCount;
	int c = 0;
	int i = 0;

	SetCell(n, c, pBuffer);
	while (i < nBufferSize)
	{
		switch (pBuffer[i])
		{
		case 0:
			{
				return true;
			}
			break;
		case '\t':
			{
				if (!bIsString && i > 0)
				{
					pBuffer[i] = 0;
					SetCell(n, ++c, pBuffer + i + 1);
				}
			}
			break;
		case ',':
			{
				if (!bIsString && i > 0)
				{
					pBuffer[i] = 0;
					SetCell(n, ++c, pBuffer + i + 1);
				}
			}
			break;
		case '\r':
			{
				pBuffer[i] = 0;
				c = 0;
			}
			break;
		case '\n':
			{
				if (!bIsString && i > 0)
				{
					pBuffer[i] = 0;
					SetCell(++n, c, pBuffer + i + 1);
				}
			}
			break;
		case '"':
			{
				if (!bIsString)
				{
					SetCell(n, c, pBuffer + i + 1);
				}
				pBuffer[i] = 0;
				bIsString = !bIsString;
			}
			break;
		default:
			break;
		}
		if (c >= m_nColCount)
		{
			return false;
		}
		++i;
	}
	return true;
}

bool CExcel::SetCell(int nRow, int nCol, char* pStr)
{
	int nIdx = (nRow >> 12); // 右移12位，每过4096行，重新分配一次内存
	void* pChunk = m_pChunk[nIdx];
	if (NULL == pChunk)
	{
		m_pChunk[nIdx] = pChunk = (void*)malloc(0x1000 * sizeof(void*) * m_nColCount);
		if (NULL == pChunk)
		{
			return false;
		}
		memset(pChunk, 0, 0x1000 * sizeof(void*) * m_nColCount);
	}

	int nOffset = (nRow & 0x0fff);
	char** pCell = (char**)pChunk;
	pCell[nOffset*m_nColCount + nCol] = pStr;

	return true;
}

const char* CExcel::GetCell(int nRow, int nCol)
{
	if (nCol >= m_nColCount)
	{
		return NULL;
	}
	int nIdx = (nRow >> 12);
	if (nIdx < (int)(sizeof(m_pChunk)/sizeof(m_pChunk[0])))
	{
		void* pChunk = m_pChunk[nIdx];
		if (NULL != pChunk)
		{
			int nOffset = (nRow & 0x0fff);
			char** pCell = (char**)pChunk;
			return pCell[nOffset*m_nColCount + nCol];
		}
	}
	return NULL;
}
