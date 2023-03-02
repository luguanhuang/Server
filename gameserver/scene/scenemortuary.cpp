#include "pch.h"
#include <time.h>
#include "scenemortuary.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "battle/ptcg2c_revivenotify.h"
#include "battle/ptcg2c_revivecountdown.h"
#include "unit/unit.h"
#include "scene/scene.h"
#include "gamelogic/takecostmgr.h"
#include "foreach.h"
#include "table/globalconfig.h"
#include "define/itemdef.h"
#include "entity/XObject.h"
#include "component/XBuffComponent.h"
#include "unit/attributewatcher.h"
#include "unit/combatattribute.h"
#include "gamelogic/spriterecord.h"

SceneMortuary::SceneMortuary(Scene *pScene) : m_pScene(pScene)
{
	m_isInOnTimer = false;
	m_ReviveFreeInterval = 30;
	m_ReviveAutoInterval = 2;
	m_ReviveFreeMaxCount = 2;

	m_handler = CTimerMgr::Instance()->SetTimer(this, 0, 1000, -1, __FILE__, __LINE__);
}

SceneMortuary::~SceneMortuary()
{
	CTimerMgr::Instance()->KillTimer(m_handler);
}

void SceneMortuary::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	m_isInOnTimer = true;
	time_t now = time(NULL);
	auto it = m_DeathTime.begin();
	while(it != m_DeathTime.end())
	{
		const time_t &preTime = it->first;
		if (preTime <= now)
		{
			UINT64 qwRoleID = it->second;
			
			Role* pRole = RoleManager::Instance()->FindByRoleID(qwRoleID);
			if (pRole == NULL)
			{
				m_DeathTime.erase(it++);
				continue;
			}

			int revivetype = 0;
			bool islimit = false;
			auto i = m_roleid2revivetype.find(pRole->GetID());
			if (i != m_roleid2revivetype.end())
			{
				revivetype = i->second.revivetype;
				islimit = i->second.islimit;
			}
			if (pRole->GetCurrScene() != m_pScene)
			{
				m_DeathTime.erase(it++);
				m_roleid2revivetype.erase(pRole->GetID());
				continue;
			}
			if (!pRole->IsDead())
			{
				m_DeathTime.erase(it++);
				m_roleid2revivetype.erase(pRole->GetID());
				continue;
			}

			IncReviveCount(pRole->GetID(), revivetype, islimit);

			pRole->Revive();

			SSInfo << "revive " << pRole << " type:"<< revivetype<< END;

			m_DeathTime.erase(it++);
			m_roleid2revivetype.erase(pRole->GetID());
		}
		else
		{
			break;
		}
	}

	m_isInOnTimer = false;
}

void SceneMortuary::SetReviveConfig(int interval, int maxcount, std::vector<Sequence<UINT32, 2>>& buffs)
{
	m_ReviveFreeInterval = interval;
	m_ReviveFreeMaxCount = maxcount;
	for (auto i = buffs.begin(); i != buffs.end(); ++i)
	{
		BuffDesc buff;
		buff.buffID = i->seq[0];
		buff.BuffLevel = i->seq[1];
		m_revivebuffs.push_back(buff);
	}
}

void SceneMortuary::AddDeathRole(Role *pRole)
{
	SSInfo << "death " << pRole << END;
	// special handle
	PtcG2C_ReviveCountdown oPtc;
	oPtc.m_Data.set_countdowntime(m_ReviveFreeInterval);

	SceneInfo* pSceneInfo = SceneConfig::Instance()->FindSceneInfo(m_pScene->GetSceneTemplateID());
	UINT32 vecSize = pSceneInfo->m_pConf->ReviveCost.size();
	if(0 == vecSize)
	{
		oPtc.m_Data.set_revivecosttype(0);
		oPtc.m_Data.set_revivecost(0);
	}
	else
	{
		UINT32 reviveCount = GetReviveLimitCount(pRole->GetID());
		UINT32 index = reviveCount >= vecSize - 1 ? vecSize - 1 : reviveCount;
		UINT32 costItemID = pSceneInfo->m_pConf->ReviveCost[index][0];
		UINT32 costNum = pSceneInfo->m_pConf->ReviveCost[index][1];

		oPtc.m_Data.set_revivecosttype(costItemID);
		oPtc.m_Data.set_revivecost(costNum);
	}
	pRole->Send(oPtc);

	///> common handle
	DeathRecord *pRecord = IncDeathCount(pRole->GetID());

	const SReviveInfo& reviveinfo = pRole->GetReviveInfo();
	if (reviveinfo.isreviving)
	{
		///> 其他系统置标记复活
		time_t reviveTime = time(NULL) + m_ReviveAutoInterval;
		m_DeathTime.insert(std::make_pair(reviveTime, pRole->GetID()));
		AutoReviveInfo info;
		info.revivetime = reviveTime;
		info.islimit = reviveinfo.islimit;
		info.revivetype = reviveinfo.type;
		m_roleid2revivetype[pRole->GetID()] = info;
	}
	else if (pRecord->nReviveCount[KKSG::ReviveFree] < m_ReviveFreeMaxCount)
	{
		// free revive
		time_t reviveTime = time(NULL) + m_ReviveFreeInterval;
		m_DeathTime.insert(std::make_pair(reviveTime, pRole->GetID()));
		AutoReviveInfo info;
		info.islimit = false;
		info.revivetime = reviveTime;
		info.revivetype = KKSG::ReviveFree;
		m_roleid2revivetype[pRole->GetID()] = info;

		pRole->SetIsReviving(true);
		pRole->SetReviveType(KKSG::ReviveFree);
	}
	else
	{
		// item revive
		pRole->SetIsReviving(false);
		pRole->SetReviveType(KKSG::ReviveItem);
	}
}

void SceneMortuary::DeleteDeathRoleInTimer(Role* role)
{
	if (m_isInOnTimer)
	{
		return;
	}

	role->GetReviveInfo().Reset();

	UINT64 qwRoleID = role->GetID();
	if(m_roleid2revivetype.find(qwRoleID) == m_roleid2revivetype.end())
	{
		return;
	}
	time_t preDeathTime = m_roleid2revivetype[qwRoleID].revivetime;
	auto it = m_DeathTime.find(make_pair(preDeathTime, qwRoleID));
	if(it != m_DeathTime.end())
	{
		m_DeathTime.erase(it);
	}
}

DeathRecord* SceneMortuary::IncReviveCount(UINT64 qwRoleID, int type, bool islimit)
{
	if (type >= KKSG::ReviveMax)
	{
		SSError<<"type is out of range:"<<type<<END;
		return NULL;
	}
	auto i = m_RoleDeathCount.find(qwRoleID);
	if(i == m_RoleDeathCount.end())
	{
		DeathRecord record;
		record.qwRoleID = qwRoleID;
		record.nReviveCount[type] = 1;
		if (islimit)
		{
			record.nReviveLimitCount = 1;
		}
		record.isaddbuff = (type == (int)KKSG::ReviveItem || type == (int)KKSG::ReviveMoney);
		m_RoleDeathCount[qwRoleID] = record;
		return &m_RoleDeathCount[qwRoleID];
	}
	else
	{
		++i->second.nReviveCount[type];
		if (islimit)
		{
			++i->second.nReviveLimitCount;
		}
		i->second.isaddbuff = (type == (int)KKSG::ReviveItem || type == (int)KKSG::ReviveMoney);
		return &i->second;
	}
}

int SceneMortuary::GetReviveCount(UINT64 qwRoleID, int type)
{
	auto i = m_RoleDeathCount.find(qwRoleID);
	if(i == m_RoleDeathCount.end())
	{
		return 0;
	}
	return i->second.nReviveCount[type];
}

void SceneMortuary::ClearDeathInfo()
{
	m_RoleDeathCount.clear();
	m_DeathTime.clear();
}

DeathRecord *SceneMortuary::GetDeathRecord(UINT64 qwRoleID)
{	
	auto i = m_RoleDeathCount.find(qwRoleID);
	if(i == m_RoleDeathCount.end())
	{
		return NULL;
	}
	else
	{
		return &i->second;
	}
}

DeathRecord * SceneMortuary::IncDeathCount(UINT64 qwRoleID)
{
	auto i = m_RoleDeathCount.find(qwRoleID);
	if(i == m_RoleDeathCount.end())
	{
		DeathRecord record;
		record.qwRoleID = qwRoleID;
		record.nDeathCount = 1;
		m_RoleDeathCount[qwRoleID] = record;
		return &m_RoleDeathCount[qwRoleID];
	}
	else
	{
		++i->second.nDeathCount;
		return &i->second;
	}
}

int SceneMortuary::GetDeathCount(UINT64 qwRoleID)
{
	auto i = m_RoleDeathCount.find(qwRoleID);
	if(i == m_RoleDeathCount.end())
	{
		return 0;
	}
	return i->second.nDeathCount;
}

int SceneMortuary::GetReviveLimitCount(UINT64 roleid)
{
	auto i = m_RoleDeathCount.find(roleid);
	if(i == m_RoleDeathCount.end())
	{
		return 0;
	}
	return i->second.nReviveLimitCount;
}

int SceneMortuary::GetItemReviveCount(UINT64 roleid)
{
	auto i = m_RoleDeathCount.find(roleid);
	if(i == m_RoleDeathCount.end())
	{
		return 0;
	}
	return (i->second.nReviveCount[KKSG::ReviveItem] + i->second.nReviveCount[KKSG::ReviveMoney]);
}

int SceneMortuary::GetReviveCount(UINT64 qwRoleID)
{
	auto i = m_RoleDeathCount.find(qwRoleID);
	if(i == m_RoleDeathCount.end())
	{
		return 0;
	}
	int n = 0;
	for (int j = 0; j < KKSG::ReviveMax; ++j)
	{
		n += i->second.nReviveCount[j];
	}
	return n;
}

bool SceneMortuary::CanVipRevive(UINT64 qwRoleID)
{
	auto i = m_RoleDeathCount.find(qwRoleID);
	if(i == m_RoleDeathCount.end())
	{
		return true;
	}
	UINT32 count = i->second.nReviveCount[KKSG::ReviveVIP];
	UINT32 limit = m_pScene->GetSceneInfo()->m_pConf->VipReviveLimit;
	return limit > count;
}
