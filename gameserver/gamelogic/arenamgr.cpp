#include "pch.h"
#include "time.h"
#include "arenamgr.h"
#include "rolesummarymgr.h"
#include "tablemgr/XEntityInfoLibrary.h"
#include "table/XEntityStatistics.h"
#include "unit/role.h"
#include "bagtransition.h"
#include "unit/enemymanager.h"
#include "pb/project.pb.h"
#include "globalconfig.h"
#include "arenarecord.h"

INSTANCE_SINGLETON(CArenaMgr);

CArenaMgr::CArenaMgr()
{
	Reset();
}

CArenaMgr::~CArenaMgr()
{

}

bool CArenaMgr::Init()
{
	if (!LoadFile())
	{
		return false;
	}
	return true;
}

void CArenaMgr::Uninit()
{

}

void CArenaMgr::Reset()
{
	m_oTable.Clear();
}

bool CArenaMgr::LoadFile()
{
	if (!m_oTable.LoadFile("table/Arena.txt"))
	{
		SSWarn<<"Load table table/Arena.txt failed!"<<END;
		return false;
	}
	return true;
}

bool CArenaMgr::Reload()
{
	Reset();
	return LoadFile();
}

void CArenaMgr::PushBattleInfo(UINT64 roleid, const KKSG::ArenaBattleInfo& info)
{
	m_roleid2battle[roleid] = info;	
}

void CArenaMgr::PopBattleInfo(UINT64 roleid)
{
	auto i = m_roleid2battle.find(roleid);
	if (i != m_roleid2battle.end())
	{
		m_roleid2battle.erase(i);
	}
}

const KKSG::ArenaBattleInfo* CArenaMgr::GetBattleInfo(UINT64 roleid)
{
	auto i = m_roleid2battle.find(roleid);
	if (i != m_roleid2battle.end())
	{
		return &i->second;
	}
	return NULL;
}

void CArenaMgr::FillBattleInfo(Role* role, KKSG::UnitAppearance* pUnitAppearance)
{
	const KKSG::ArenaBattleInfo* info = GetBattleInfo(role->GetID());
	if (NULL == info)
	{
		SSWarn<<"not find arena battle info, roleid:"<<role->GetID()<<END;
		return;
	}
	if (info->isnpc())
	{
		pUnitAppearance->set_uid(info->challengid());
		pUnitAppearance->set_unittype(info->enemyid());
		pUnitAppearance->set_unitname(info->name());
		pUnitAppearance->set_level(info->level());
		pUnitAppearance->set_isservercontrol(false);
		pUnitAppearance->set_fightgroup(KKSG::FightEnemy);
		pUnitAppearance->set_category(KKSG::Category_Enemy);

		float scale = 1.0f;
		if (0 !=  GetGlobalConfig().ArenaScale)
		{
			scale = (float)(info->ppt())/(float)role->GetAttr(TOTAL_POWERPOINT);
		}

		CombatAttribute combatAttr;
		EnemyManager::LoadEnemyAttribute(info->enemyid(), &combatAttr);
		EnemyManager::LoadRoleAttribute(role, &combatAttr, info->profession(), scale);
		combatAttr.SetAttr(BASIC_POWERPOINT, info->ppt());
		combatAttr.SaveToAttributes(*pUnitAppearance->mutable_attributes());
	}
	else
	{
		//RoleSummary* pRoleSummary = RoleSummaryMgr::Instance()->FindRoleSummary(info->challengid());
		RoleSummary* pRoleSummary = NULL;
		if (NULL == pRoleSummary)
		{
			SSWarn<<"pRoleSummary is NULL, role id:"<<info->challengid()<<END;
			return;
		}
		//pRoleSummary->SetUnitAppearnce(pUnitAppearance);
		pUnitAppearance->set_isservercontrol(false);
		pUnitAppearance->set_fightgroup(KKSG::FightEnemy);
		pUnitAppearance->set_category(KKSG::Category_Role);
		pUnitAppearance->set_level(pRoleSummary->GetLevel());
	}
}

void CArenaMgr::OnEnd(Role* pRole, bool iswin, bool isnpc, UINT64 challengeid)
{
	CArenaRecord* record = pRole->Get<CArenaRecord>();
	record->Update();
	record->AddPoint(iswin ? 2 : 1);

	if (!isnpc)
	{
		//RoleSummary* challengedsummary = RoleSummaryMgr::Instance()->FindRoleSummary(challengeid);
		RoleSummary* challengedsummary = NULL; 
		if (NULL == challengedsummary)
		{
			SSWarn<<"challengedsummary is NULL, role id:"<<challengeid<<END;
		}
		else
		{
			//challengedsummary->stRoleArenaInfo.PushRecorded(1, !iswin, pRole->GetID());
		}
	}
	//RoleSummary* rolesummary = RoleSummaryMgr::Instance()->FindRoleSummary(pRole->GetID());
	RoleSummary* rolesummary = NULL;
	if (NULL == rolesummary)
	{
		SSWarn<<"rolesummary is NULL, role id:"<<pRole->GetID()<<END;		
	}
	else
	{
		//rolesummary->stRoleArenaInfo.PushRecorded(0, iswin, challengeid);
	}

	return;
}

int CArenaMgr::GetUpReward(UINT32 lastrank, UINT32 nowrank)
{
	UINT32 count = 0;
	const ArenaTable::RowData* lastdata = GetRowData(lastrank);
	const ArenaTable::RowData* nowdata = GetRowData(nowrank);
	if (lastdata == nowdata)
	{
		count = (UINT32)((lastrank - nowrank)*nowdata->uprewardcoef);
	}
	else
	{
		UINT32 begin = 0;
		UINT32 end = 0;
		bool isin = false;
		for (auto i = m_oTable.Table.rbegin(); i != m_oTable.Table.rend(); ++i)
		{
			ArenaTable::RowData* data = *i;
			if (isin)
			{
				if (nowrank >= data->ArenaRank[0] && nowrank <= data->ArenaRank[1])
				{
					end = (UINT32)((data->ArenaRank[1] - nowrank + 1)*data->uprewardcoef);
					break;
				}
				else
				{
					count += (UINT32)((data->ArenaRank[1] - data->ArenaRank[0] + 1)*data->uprewardcoef);
				}
			}
			else
			{
				if (lastrank >= data->ArenaRank[0] && lastrank <= data->ArenaRank[1])
				{
					isin = true;
					begin = (UINT32)((lastrank - data->ArenaRank[0])*data->uprewardcoef);
				}
			}
		}
		count += (begin + end);
	}
	return (int)count;
}


const ArenaTable::RowData* CArenaMgr::GetRowData(UINT32 rank)
{
	for (auto i = m_oTable.Table.rbegin(); i != m_oTable.Table.rend(); ++i)
	{
		ArenaTable::RowData* data = *i;
		if (rank >= data->ArenaRank[0] && rank <= data->ArenaRank[1])
		{
			return data;
		}
	}
	return NULL;
}
