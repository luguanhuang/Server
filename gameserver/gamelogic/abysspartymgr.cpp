#include "pch.h"
#include "unit/role.h"
#include "abysspartymgr.h"
#include "util/gametime.h"
#include "define/systemiddef.h"
#include "util/XCommon.h"
#include "util/gametime.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "unit/systeminfo.h"
#include "define/itemdef.h"
#include "bagtransition.h"
#include "stagemgr.h"
#include "item.h"
#include "mail/mailsender.h"
#include "mail/mailconfig.h"

INSTANCE_SINGLETON(AbyssPartyMgr);

AbyssPartyMgr::AbyssPartyMgr(){}

AbyssPartyMgr::~AbyssPartyMgr(){}

bool AbyssPartyMgr::Init()
{
	bool bRet = LoadFile();
	if (bRet == false)
		return false;
	return true;
}

void AbyssPartyMgr::Uninit()
{
	ClearFile();
}

void AbyssPartyMgr::ClearFile()
{
	m_PartyType.Clear();
	m_mapNeedItem.clear();
}

bool AbyssPartyMgr::LoadFile()
{
	if(GSConfig::Instance()->IsCrossGS()==true)
		return true;
	ClearFile();
	bool bRet = m_PartyType.LoadFile("table/AbyssPartyType.txt");
	if (bRet == false)
		return false;
	for (size_t i = 0; i < m_PartyType.Table.size();i++)
	{
		AbyssPartyTypeTable::RowData *pRowData = m_PartyType.Table[i];
		m_mapNeedItem[pRowData->AbyssPartyId] = pRowData->TitanItemID[0];
	}
	return true;
}


void AbyssPartyMgr::GetNeedItem(UINT32 nDiff,std::vector<UINT32> &itemConf)
{
	for (auto iter = m_mapNeedItem.begin(); iter!=m_mapNeedItem.end(); iter++)
	{
		if (iter->first>=nDiff)
		{
			itemConf.push_back(iter->second);
		}
	}
}


bool AbyssPartyMgr::CheckLevel(Role *pRole,UINT32 nDiff)
{
	if (pRole == NULL)
	{
		return false;
	}
	AbyssPartyTypeTable::RowData *pRowData =  m_PartyType.GetByAbyssPartyId(nDiff);
	if (pRowData == NULL)
	{
		return false;
	}
	if(pRole->GetLevel()>=pRowData->OpenLevel)
	{
		return true;
	}
	return false;
}
