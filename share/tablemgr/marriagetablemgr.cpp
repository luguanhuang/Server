#include "pch.h"
#include "marriagetablemgr.h"

bool MarriageTableMgr::Init()
{
	return LoadFile();
}

void MarriageTableMgr::Uninit()
{

}

bool MarriageTableMgr::DoLoadFile()
{
	if (!m_liveness.LoadFile("table/WeddingLoverLiveness.txt"))
	{
		LogError("load table/WeddingLoverLiveness.txt failed");
		return false;
	}
	return true;
}

void MarriageTableMgr::ClearFile()
{
	m_liveness.Clear();
}

WeddingLoverLiveness::RowData* MarriageTableMgr::GetLivenessData(UINT32 index)
{
	return m_liveness.GetByindex(index);
}

bool MarriageTableMgr::HasNewChest(UINT32 oldValue, UINT32 newValue)
{
	for (auto it = m_liveness.Table.begin(); it != m_liveness.Table.end(); ++it)
	{
		auto pData = *it;
		if (oldValue < pData->liveness && newValue >= pData->liveness)
		{
			return true;
		}
	}
	return false;
}

