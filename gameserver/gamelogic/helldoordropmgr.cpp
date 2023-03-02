#include "pch.h"
#include "helldoordropmgr.h"
#include "util/gametime.h"
#include "util/XCommon.h"
#include "unit/role.h"
#include "battlefieldrecord.h"
#include "config/itemdropconfig.h"
#include "define/itemdef.h"


INSTANCE_SINGLETON(HellDoorDropMgr);



HellDoorDropMgr::HellDoorDropMgr()
{

}

HellDoorDropMgr::~HellDoorDropMgr()
{

}

bool HellDoorDropMgr::Init()
{
	bool bRet =CheckFile();
	if(!bRet)
		return false;
	LoadFile();
	return true;
}

void HellDoorDropMgr::Uninit()
{
	ClearFile();
}

bool HellDoorDropMgr::LoadFile()
{
	bool bRet = CheckFile();
	if (!bRet)
		return false;
	ClearFile();

	if( !m_Table.LoadFile("table/HellDoorDrop.txt"))
	{
		SSWarn<<"load file HellDoorDrop.txt failed"<<END;
		return false;
	}
	return true;

}


void HellDoorDropMgr::ClearFile()
{
	m_Table.Clear();
}

bool HellDoorDropMgr::CheckFile()
{
	HellDoorDropTable tmpTable;
	bool bRet = tmpTable.LoadFile("table/HellDoorDrop.txt");
	return bRet;
}


void  HellDoorDropMgr::GetItem(Role *pRole,UINT32 nId,std::vector<const ItemDesc*> &refItemList)
{
	HellDoorDropTable::RowData *pRowData =   GetData(nId);
	if (pRowData==NULL)
	{
		return;
	}
	CBattleFieldRecord* pRecord = pRole->Get<CBattleFieldRecord>();
	KKSG::HellDropInfo* pDropInfo =  pRecord->GetDropInfo(nId);
	std::vector<UINT32> vecDropId;
	LogDebug("GetItem----cur=%u worse=%u worst=%u",pDropInfo->count(),pDropInfo->worse(),pDropInfo->worst());
	if (pDropInfo->count() < pDropInfo->worse())//common
	{
		vecDropId = pRowData->CommonDropID;

	}else if (pDropInfo->count() < pDropInfo->worst())//worse
	{
		vecDropId = pRowData->WorseDropID;
	}else if (pDropInfo->count() >= pDropInfo->worst())//worst
	{
		vecDropId = pRowData->WorstDropID;
	}
	if (!vecDropId.empty())
	{
		pRecord->SetDropInfo(nId, pDropInfo->count()+1);
	}
	for (auto  iter = vecDropId.begin(); iter!=vecDropId.end(); iter++)
	{
		const ItemDesc* pItem = ItemDropConfig::Instance()->GetRandomDrop(*iter, pRole->GetLevel(), CommonRandom);
		refItemList.push_back(pItem);
	}
}

HellDoorDropTable::RowData * HellDoorDropMgr::GetData(UINT32 nID)
{
	return m_Table.GetByHellDoorDropID(nID);
}

HellDoorDropTable* HellDoorDropMgr::GetTable()
{
	return &m_Table;
}

