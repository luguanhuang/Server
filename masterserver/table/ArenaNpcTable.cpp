#include "pch.h"
#include "ArenaNpcTable.h"
#include "ExcelReader.h"
#include "config.h"

#define EnemyMask ((UINT64)KKSG::Category_Enemy << 60)

ArenaNpcTable::ArenaNpcTable()
{

}

ArenaNpcTable::~ArenaNpcTable()
{
	Clear();
}

void ArenaNpcTable::Clear()
{
	for (auto i = m_vecNpcInfo.begin(); i != m_vecNpcInfo.end(); ++i)
	{
		delete *i;
		*i = NULL;
	}
	m_vecNpcInfo.clear();
}

bool ArenaNpcTable::LoadFile(const char* pszFile)
{
	CExcel oExcel;
	if (!oExcel.ReadExcelFile(MSConfig::Instance()->GetFilePath(pszFile).c_str()))
	{
		return false;
	}
	int nRow = 0;
	int nCol = 0;
	int counts = oExcel.GetRowCount();
	if (counts != (int)ARENANPC_COUNT)
	{
		assert(false);
		return false;
	}
	for (; nRow < (int)ARENANPC_COUNT; ++nRow)
	{
		ArenaNpcInfo* stNpcInfo = new ArenaNpcInfo();
		for (; nCol < oExcel.GetColCount(); ++nCol)
		{
			const char* pCell = oExcel.GetCell(nRow, nCol);
			if (NULL == pCell)
			{
				continue;
			}
			if (0 == nCol)
			{
				stNpcInfo->qwNpcId = (UINT64)atoi(pCell) + EnemyMask;
			}
			else if (1 == nCol)
			{
				if (strlen(pCell) >= ARENANPC_NAME_LENGTH)
				{
					return false;
				}
				memcpy(stNpcInfo->szNpcName, pCell, strlen(pCell));
			}
			else if (2 == nCol)
			{
				stNpcInfo->dwNpcProfession = (UINT32)atoi(pCell);
			}
		}
		m_vecNpcInfo.push_back(stNpcInfo);
		m_uid2info[stNpcInfo->qwNpcId] = stNpcInfo;
		nCol = 0;
	}
	return true;
}

bool ArenaNpcTable::InTable(UINT64 uid)
{
	return m_uid2info.find(uid) != m_uid2info.end();
}

const ArenaNpcInfo* ArenaNpcTable::GetInfo(UINT64 uid)
{
	auto i = m_uid2info.find(uid);
	if (i == m_uid2info.end())
	{
		return NULL;
	}
	return i->second;
}
