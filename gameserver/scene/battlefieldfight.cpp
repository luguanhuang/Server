#include "pch.h"
#include "battlefieldfight.h"
#include "scene/scene.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "util/gametime.h"
#include "network/worldlink.h"
#include "utility/tlogger.h"
#include "unit/enemy.h"
#include "config/itemdropconfig.h"
#include "tablemgr/XEntityInfoLibrary.h"
#include "battlefield/ptcg2c_battlefieldreliventf.h"
#include "battlefield/ptcg2c_bffighttimentf.h"
#include "scene/battleresultlistener.h"
#include "gamelogic/battlefieldrecord.h"

SceneBFFight::SceneBFFight(Scene* scene):SceneVsBase(scene),m_nTime(0),m_nEndTime(0),m_bLoaded(false)
{
	m_scene			= scene;
	m_nEndTime		= GameTime::GetTime()+ GetGlobalConfig().BattleFieldFightTime;
	m_nLastSynTime	= 0;
	m_nLvIdx		= 0;
	m_bFinal		= 0;

}

SceneBFFight::~SceneBFFight()
{
}


bool SceneBFFight::Init(const KKSG::CreateBattleParam& roData)
{
	m_nLvIdx = roData.bffight().lvindx();
	m_bFinal = roData.bffight().end();
	for (int i = 0; i < roData.bffight().roleinfo_size(); i++)
	{	
		stBFRoleInfo roleInfo;
		roleInfo.nRoleID = roData.bffight().roleinfo(i).roleid();
		roleInfo.szName = roData.bffight().roleinfo(i).name();
		roleInfo.svrname = roData.bffight().roleinfo(i).svrname();
		roleInfo.job = roData.bffight().roleinfo(i).job();
		m_units.push_back(roleInfo);
		LogInfo("battlefield roleid=%llu name=%s",roleInfo.nRoleID,roleInfo.szName.c_str());
	}
	InitState();	
	return true;
}

void SceneBFFight::InitState()
{
	m_statemgr.Init(this, &VsWaitLoad::GlobalVsWaitLoad);
	m_statemgr.GetCurrentState()->OnEnter(this);
}

VsStateType SceneBFFight::IsAllLoad()
{
	for(size_t i = 0; i < m_units.size(); ++i)
	{
		if(!m_loadmgr.HaveRole(m_units[i].nRoleID))
		{
			return VS_STATE_DUMMY;
		}
		else if(m_loadmgr.LoadedTime(m_units[i].nRoleID) < GetGlobalConfig().BattleFieldAnimTime)
		{
			return VS_STATE_DUMMY;
		}
	}
	return VS_STATE_PREPARE321;
}



UINT32 SceneBFFight::GetGroup(UINT64 roleid)
{
	std::vector<stBFRoleInfo>::iterator iter  = std::find_if(m_units.begin(), m_units.end(), BFRoleFind(roleid));
	return iter-m_units.begin();
}

bool SceneBFFight::OnEvent(const VsEvent& event)
{
	Role* pRole = RoleManager::Instance()->FindByRoleID(event.roleid);	
	if(pRole==NULL ||(pRole&&pRole->IsWatcher()))
	{
		return false;
	}
	switch(event.type)
	{
	case VS_EVENT_ROLE_LOADED:
		{
			OnRoleEnter(pRole);
			break;
		}
	case VS_EVENT_ROLE_QUIT:
		{
			OnRoleLeave(event.roleid);	
			break;
		}
	case VS_EVENT_ROLE_DIE:
		{
			OnRoleDie(event.roleid);
			break;
		}
	default:
		break;
	}
	return true;
}




void SceneBFFight::Update()
{
	m_nTime = GameTime::GetTime();
	if( m_nTime  >= m_nLastSynTime)
	{
		m_nLastSynTime = GetGlobalConfig().BattleFieldSyncTime + m_nTime;
		SyncTime();
	}
	SyncHangTime();
	_CheckRoleRevive();
	m_statemgr.Update();
	if (m_nEndTime&& m_nTime >= m_nEndTime)
	{
		BattleEnd();
	}
}

void SceneBFFight::BattleEnd()
{
	m_nEndTime = 0;
	SetEnd();
	GetScene()->SetDontDestory(false);
}


VsStateType SceneBFFight::CheckFightEnd()
{
	return VS_STATE_DUMMY;
}

VsStateType SceneBFFight::CheckAfterFightEnd()
{
	return VS_STATE_DUMMY;
}

void SceneBFFight::OnRoleReconnect(Role* pRole)
{

}

void SceneBFFight::OnEnemyDead(Enemy *pEnemy)
{
	UINT64 nRoleID = pEnemy->GetKillerID();
	Role *pRole =  GetRole(nRoleID);
	const XEntityStatistics::RowData* pRow = XEntityInfoLibrary::Instance()->TemplateID2Data_Enemy(pEnemy->GetTemplateID());
	if (pEnemy != NULL&&pRole!=NULL)
	{
		LogInfo("role kill boss roleid =%llu point=%u",nRoleID,pRow->BigMeleePoints);
		AddPoint(nRoleID, pRow->BigMeleePoints);
		SortRoleBattle();
		if (pEnemy->IsBoss())
		{
			BattleEnd();
		}
	}
}

void SceneBFFight::AddPoint(UINT64 nRoleID,UINT32 nPoint)
{
	std::vector<stBFRoleInfo>::iterator iter  = std::find_if(m_units.begin(), m_units.end(), BFRoleFind(nRoleID));
	if (iter!=m_units.end())
	{
		(*iter).nScore += nPoint;
		Role* pRole = GetRole(iter->nRoleID);
		if (pRole)
		{
			CBattleFieldRecord* pRecord = pRole->Get<CBattleFieldRecord>();
			pRecord->GivePointAward(iter->nScore);
		}
	}
}

bool SceneBFFight::OnRoleLeave(UINT64 roleid)
{
	m_loadmgr.DelRole(roleid);
	return true;
}

bool SceneBFFight::OnRoleDie(UINT64 roleid)
{
	VsKillNotify(roleid, GetGlobalConfig().BattleFieldReviveTime, 0);
	Role* pDeadRole = GetRole(roleid);
	if (pDeadRole)
	{
		PtcG2C_BattleFieldReliveNtf ntf;
		ntf.m_Data.set_time(GetGlobalConfig().BattleFieldReviveTime);
		pDeadRole->Send(ntf);

		stBFRoleInfo* pDeadRoleInfo = GetBFFightRole(roleid);
		if (pDeadRoleInfo==NULL)
			return false;
		pDeadRoleInfo->nKillStreak = 0;
		pDeadRoleInfo->nDead++;
		pDeadRoleInfo->nReliveTime = m_nTime + GetGlobalConfig().BattleFieldReviveTime;
		UINT64 killer =	pDeadRole->GetKillerID();
		stBFRoleInfo* pKillerRoleInfo = GetBFFightRole(killer);
		if (pKillerRoleInfo==NULL)
			return false;

		Role* pKillerRole = GetRole(killer);
		if (pKillerRole)
		{
			CBattleFieldRecord* pRecord = pKillerRole->Get<CBattleFieldRecord>();
			pRecord->AddTotalKillCount();
			UINT32 nExtraPoint = 0;
			for (size_t i = 0; i < GetGlobalConfig().BattleFieldKillerLvPoint.size(); i++)
			{
				if (pDeadRoleInfo->nRank >= GetGlobalConfig().BattleFieldKillerLvPoint[i][0] && pDeadRoleInfo->nRank <= GetGlobalConfig().BattleFieldKillerLvPoint[i][1])
				{
					nExtraPoint = GetGlobalConfig().BattleFieldKillerLvPoint[i][2];
				}
			}
			pKillerRoleInfo->nKiller++;
			pKillerRoleInfo->nScore+=nExtraPoint;
			pKillerRoleInfo->nKillStreak++;
		}
		SortRoleBattle();
	}
	return true;
}


void SceneBFFight::OnEnterP321()
{
	Set321State();
}

void SceneBFFight::_CheckRoleRevive()
{
	for (size_t  i = 0; i < m_units.size(); i++)
	{
		if (m_units[i].nReliveTime && m_nTime >= m_units[i].nReliveTime)
		{
			Role* pRole = GetRole(m_units[i].nRoleID);
			if (pRole)
			{
				XEntity* entity = dynamic_cast<XEntity*>(pRole->GetXObject());
				if (entity)
				{
					EnterPosition postion = GetInitPos(pRole->GetID());
					entity->TransferLocation(postion.position, postion.face);
					pRole->Revive();
					m_units[i].nReliveTime = 0;
				}
			}
		}
	}
}

bool SceneBFFight::OnRoleEnter(Role *pRole)
{
	CBattleFieldRecord* pRecord = pRole->Get<CBattleFieldRecord>();
	pRecord->ClearBattleReward();
	m_loadmgr.AddRole(pRole->GetID());
	if (m_units.size()==1)
		Set321State();
	if(IsInWaitLoaded())
		MakePuppet(pRole, true);
	return true;
}


void SceneBFFight::Set321State()
{
	m_nEndTime = GameTime::GetTime() + GetGlobalConfig().BattleFieldFightTime;
	m_bLoaded = true;
}

stBFRoleInfo* SceneBFFight::GetBFFightRole(UINT64 roleid)
{
	auto iter = std::find_if(m_units.begin(),m_units.end(),BFRoleFind(roleid));
	if (iter == m_units.end())
	{
		return NULL;
	}
	return &(*iter);
}

void SceneBFFight::GetAginstInfo(KKSG::BattleFieldRoleAgainst &refInfo)
{
	for (size_t i = 0; i < m_units.size(); i++)
	{
		KKSG::BattleFieldRoleSimpleInfo* pInfo = refInfo.add_roles();
		pInfo->set_name(m_units[i].szName);
		pInfo->set_roleid(m_units[i].nRoleID);
	}
}

void SceneBFFight::GetRank(KKSG::BattleFieldRankRes &refInfo)
{
	for (size_t i = 0; i < m_units.size(); i++)
	{
		KKSG::BattleFieldRank * pInfo = refInfo.add_ranks();
		pInfo->set_name(m_units[i].szName);
		pInfo->set_roleid(m_units[i].nRoleID);
		pInfo->set_point(m_units[i].nScore);
	}
}

void SceneBFFight::SyncTime()
{
	if (m_bLoaded)
	{
		PtcG2C_BFFightTimeNtf ntf;
		if(m_nTime <= m_nEndTime)
		{
			ntf.m_Data.set_time(m_nEndTime-m_nTime);
		}
		GetScene()->Broadcast(ntf);
	}
}

void SceneBFFight::AddItem(Role *pRole,std::vector<ItemDesc> &refItems)
{
	stBFRoleInfo* pRoleInfo =	GetBFFightRole(pRole->GetID());
	if (pRoleInfo)
	{
		pRoleInfo->AddItem(refItems);
	}
}

void SceneBFFight::GetRoleList(std::vector<stBFRoleInfo> &refList)
{
	refList = m_units;
}

void SceneBFFight::GetItem(Role *pRole, std::vector<ItemDesc> &refItems)
{
	stBFRoleInfo* pRoleInfo = GetBFFightRole(pRole->GetID());
	if (pRoleInfo==NULL)
	{
		return;
	}
	refItems = pRoleInfo->itemList;
}

bool SceneBFFight::GetFinal()
{
	return m_bFinal;
}

void SceneBFFight::SyncHangTime()
{
	for (auto iter = m_units.begin(); iter!=m_units.end(); iter++)
	{
		if (m_bLoaded&&m_nTime<=m_nEndTime &&m_nTime>=iter->nTime &&m_nTime-iter->nTime>=10)
		{
			Role* pRole = GetRole(iter->nRoleID);
			if (pRole)
			{
				stBFRoleInfo* pHangRoleInfo = GetBFFightRole(pRole->GetID());
				if (pHangRoleInfo)
				{
					pHangRoleInfo->nTime += m_nTime - iter->nTime;
				}
			}
		}
	}
	SortRoleBattle();
}


void SceneBFFight::FillBattleResult(UINT64 nRoleID,KKSG::BattleFieldBattleResult &refData)
{
	stBFRoleInfo* pRoleInfo = GetBFFightRole(nRoleID);
	if (pRoleInfo==NULL)
	{
		return;
	}
	if (m_units[0].nRoleID == nRoleID)
	{
		Role *pRole =  GetRole(nRoleID);
		if (pRole)
		{
			CBattleFieldRecord* pRecord = pRole->Get<CBattleFieldRecord>();
			pRecord->AddFirstRankCount();
		}
	}
	refData.set_killer(pRoleInfo->nKiller);
	refData.set_point(pRoleInfo->nScore);
	refData.set_roleid(pRoleInfo->nRoleID);
	refData.set_rank(pRoleInfo->nRank);
	refData.set_death(pRoleInfo->nDead);
	refData.set_svrname(pRoleInfo->svrname);
	refData.set_killstreak(pRoleInfo->nKillStreak);
	refData.set_hurt(pRoleInfo->hurt);
	refData.set_job(pRoleInfo->job);
	refData.set_name(pRoleInfo->szName);
}


void SceneBFFight::SortRoleBattle()
{
	std::sort(m_units.begin(), m_units.end(),BFRoleInfoCmp());
	UINT32 nRank = 1;
	for (auto iter = m_units.begin(); iter!=m_units.end(); iter++)
	{
		iter->nRank = nRank++;
	}

}

void SceneBFFight::OnHurt(Unit* pTargetUnit, Unit* pCastUnit, double value)
{
	if (pTargetUnit == NULL)
	{
		SSWarn << "SceneBFFight::OnHurt: enemy is NULL" << END;
		return ;
	}

	if (pCastUnit == NULL)
	{
		SSWarn << "SceneBFFight::OnHurt: castunit is NULL " << value << END;
		return ;
	}

	if(!pCastUnit->IsRole())
	{
		SSWarn << "SceneBFFight is not role" << pCastUnit << END;
		return;
	}

	Role *pRole = GetRole(pCastUnit->GetID());
	if (pRole)
	{
		stBFRoleInfo* pHurtRoleInfo = GetBFFightRole(pRole->GetID());
		if (pHurtRoleInfo)
		{
			pHurtRoleInfo->hurt += value;
		}
	}
}




void SceneBFFight::AddBuff(Enemy* pEnemy)
{
	if (!pEnemy)
	{
		return;
	}
	if (FightGroupMgr::Instance()->CanAttack(KKSG::FightRole, pEnemy->GetFightGroup()))
	{
		XAddBuffActionArgs args;
		args.singleBuff.buffID = GetGlobalConfig().BattleFieldBuff[m_nLvIdx][0];
		args.singleBuff.BuffLevel = GetGlobalConfig().BattleFieldBuff[m_nLvIdx][1];
		pEnemy->GetXObject()->DispatchAction(&args);
	}
}
