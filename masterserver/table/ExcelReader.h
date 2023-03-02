#ifndef __EXCELREADER_H__
#define __EXCELREADER_H__

class CExcel
{
public:
	CExcel();
	~CExcel();

	bool ReadExcelFile(const char* pszFileName);
	int GetRowCount()
	{
		return m_nRowCount;
	}
	int GetColCount()
	{
		return m_nColCount;
	}
	const char* GetCell(int nRow, int nCol);

private:
	bool AnalyszFile(char* pBuffer, int nBufferSize);

	bool SetCell(int nRow, int nCol, char* pStr);

private:
	char* m_pBuffer;

	int m_nColCount;
	int m_nRowCount;
	void* m_pChunk[128];
};

#endif
