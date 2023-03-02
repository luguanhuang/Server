#include "pch.h"
#include "bigmeleefight.h"
#include "unit/enemy.h"
#include "foreach.h"
#include "scene/scene.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include <vector>
#include "util/gametime.h"
#include "network/worldlink.h"
#include "scene/battleresultlistener.h"
#include "bigmelee/ptcg2k_bmscenefightinfontf.h"
#include "bigmelee/ptcg2c_bmfighttimentf.h"
#include "bigmelee/ptcg2c_bmrolescenesyncntf.h"
#include "bigmelee/ptcg2c_bigmeleereliventf.h"
#include "tablemgr/XEntityInfoLibrary.h"
#include "bigmelee/rpcg2k_reportbmroleinfo.h"
#include "table/mayhemconfig.h"
#include "config/itemdropconfig.h"
#include "bigmelee/ptcg2k_bmleavefightscenentf.h"
#include "bigmelee/rpcg2k_bmranklistreq.h"
#include "utility/tlogger.h"
#include "component/XBuffComponent.h"
#include "gamelogic/taskrecord.h"
#include "gamelogic/goalaward.h"
#include "bigmelee/ptcg2c_bigmeleepointntf.h"
#include "bigmelee/ptcg2c_bigmeleepointoutlookntf.h"


SceneBMFight::SceneBMFight(Scene* scene):SceneVsBase(scene),m_nGames(0),m_nGroupSvr(0),m_nBattleIdx(0),m_nLastSynTime(0),m_nFightTime(0),m_nTime(0),m_nEndTime(0),m_nLastReqRankTime(0)
{
	m_scene = scene;
	m_bLoaded = false;
	m_bResult = false;
}

SceneBMFight::~SceneBMFight()
{
}

bool SceneBMFight::Init(const KKSG::CreateBattleParam& roData)
{
	if (roData.has_bmfight()==false)
	{
		SSError<<"has not bmfight data"<<END;
		return false;
	}
	m_nGroupSvr = roData.bmfight().groupsvr();
	m_nBattleIdx = roData.bmfight().battleid();
	m_nGames  = roData.bmfight().games();
	for (int i = 0; i < roData.bmfight().roles_size(); i++)
	{	
		stBMRoleInfo roleInfo;
		roleInfo.nRoleID = roData.bmfight().roles(i).roleid();
		roleInfo.szName  = roData.bmfight().roles(i).name();
		roleInfo.nScore  = roData.bmfight().roles(i).score();

		m_units.push_back(roleInfo);
		LogInfo("bigmelee roleid=%llu name=%s",roleInfo.nRoleID,roleInfo.szName.c_str());
	}
	InitState();	
	m_nFightTime = GameTime::GetTime() + GetGlobalConfig().BigMeleeFightTime;
	m_nEndTime =  GameTime::GetTime() + GetGlobalConfig().BigMeleeFightTime + GetGlobalConfig().BigMeleeRestTime;
	return true;
}

void SceneBMFight::InitState()
{
	m_statemgr.Init(this, &VsWaitLoad::GlobalVsWaitLoad);
	m_statemgr.GetCurrentState()->OnEnter(this);
}

UINT32 SceneBMFight::GetGroup(UINT64 roleid)
{
	std::vector<stBMRoleInfo>::iterator iter  = std::find_if(m_units.begin(), m_units.end(), BMRoleFind(roleid));
	return iter-m_units.begin();
}


void SceneBMFight::SyncHangTime()
{
	for (auto iter = m_units.begin(); iter!=m_units.end(); iter++)
	{
		if (m_bLoaded&&m_nTime<=m_nFightTime &&m_nTime>=iter->nTime+10)
		{
			iter->nTime = m_nTime;
			Role* pRole = GetRole(iter->nRoleID);
			if (pRole)
			{
				ReportToWorld(pRole->GetID(),0,0,0,0,pRole->GetProfession(),pRole->GetLevel());
			}
		}
	}
}


void SceneBMFight::Update()
{
	m_statemgr.Update();
	_CheckRoleRevive();
	m_nTime = GameTime::GetTime();
	SyncHangTime();
	if( m_nTime  >= m_nLastSynTime)
	{
		m_nLastSynTime = GetGlobalConfig().BigMeleeSyncTime + m_nTime;
		SyncTime();
	}

	
	if (m_nTime >= m_nFightTime && m_nGames == GetGlobalConfig().BigMeleeMaxGames)
	{
		if (m_nTime >= m_nLastReqRankTime)
		{
			m_nLastReqRankTime = GetGlobalConfig().BigMeleeReqRankTime + m_nTime;
			ReqFinalRank();
		}
	}

	if (m_nTime >= m_nEndTime && m_nGames != GetGlobalConfig().BigMeleeMaxGames)
	{
		SetEnd();
		GetScene()->SetDontDestory(false);
	}
}

void SceneBMFight::SyncTime()
{
	if (m_bLoaded==true)
	{
		PtcG2C_BMFightTimeNtf ntf;
		if(m_nTime <= m_nFightTime)
		{
			ntf.m_Data.set_type(1);
			ntf.m_Data.set_time(m_nFightTime-m_nTime);
		}else if(m_nTime <= m_nEndTime)
		{
			ntf.m_Data.set_type(2);
			ntf.m_Data.set_time(m_nEndTime-m_nTime);
		}
		GetScene()->Broadcast(ntf);
	}

}



bool SceneBMFight::OnEvent(const VsEvent& event)
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



void SceneBMFight::ResetPosition(UINT64 roleid)
{
	Unit* pUnit = GetUnit(roleid);
	if (pUnit==NULL)
		return;
	pUnit->GetXObject()->BuffComponent()->ClearBuff();
	if(pUnit && 1 == GetGlobalConfig().BigMeleeResetPos.size())
	{
		EnterPosition  pos;
		pos.face = pUnit->GetXObject()->GetFaceDegree();
		pos.position.x = GetGlobalConfig().BigMeleeResetPos[0][0] + (float)(XRandom::randDouble(-0.5, 0.5));
		pos.position.y = GetGlobalConfig().BigMeleeResetPos[0][1];
		pos.position.z = GetGlobalConfig().BigMeleeResetPos[0][2] + (float)(XRandom::randDouble(-0.5, 0.5));		
		XEntity* entity = dynamic_cast<XEntity*>(pUnit->GetXObject());
		if (pUnit->IsDead())
		{
			pUnit->Revive();
		}
		pUnit->ChangeFightGroup(1);
		if(entity)
		{
			entity->TransferLocation(pos.position, pos.face);
		}
	}
}

VsStateType SceneBMFight::IsAllLoad()
{
	for(size_t i = 0; i < m_units.size(); ++i)
	{
		if(!m_loadmgr.HaveRole(m_units[i].nRoleID))
		{
			return VS_STATE_DUMMY;
		}
		else if(m_loadmgr.LoadedTime(m_units[i].nRoleID) < GetGlobalConfig().BigMeleeAnimTime)
		{
			return VS_STATE_DUMMY;
		}
	}
	return VS_STATE_PREPARE321;
}

void SceneBMFight::Set321State()
{
	m_nFightTime = GameTime::GetTime() + GetGlobalConfig().BigMeleeFightTime;
	m_nEndTime =  GameTime::GetTime() + GetGlobalConfig().BigMeleeFightTime + GetGlobalConfig().BigMeleeRestTime;
	LogInfo("Loding finished.............. fighttime=%u endtime=%u",m_nFightTime,m_nEndTime);
	m_bLoaded = true;
}


VsStateType SceneBMFight::CheckFightEnd()
{
	if (m_nTime >= m_nFightTime)
	{
		return VS_STATE_AFTER_FIGHT;
	}
	return VS_STATE_DUMMY;
}


void SceneBMFight::OnStateEnd()
{
	std::vector<UINT64> roleIDS;
	for (size_t i = 0; i < m_units.size(); i++)
	{
		roleIDS.push_back(m_units[i].nRoleID);
	}
	EndSkills(roleIDS);
	GetScene()->SetDontDestory(false);
}


bool SceneBMFight::OnRoleEnter(Role *pRole)
{
	m_loadmgr.AddRole(pRole->GetID());
	if (m_units.size()==1)
	{
		Set321State();
	}
	if(IsInWaitLoaded())
		MakePuppet(pRole, true);

	PtcG2C_BMRoleSceneSyncNtf ntf;
	ntf.m_Data.set_games(m_nGames);
	for (auto iter = m_units.begin(); iter != m_units.end(); iter++)
	{
	   KKSG::BMRoleEnter *pRoleEnter = 	ntf.m_Data.add_roles();
	   pRoleEnter->set_roleid(iter->nRoleID);
	   pRoleEnter->set_name(iter->szName);
	   pRoleEnter->set_score(iter->nScore);
	} 
	pRole->Send(ntf);

	stBMRoleInfo* pBMRoleInfo =  GetBMFightRole(pRole->GetID());
	if (pBMRoleInfo)
	{
		pBMRoleInfo->state  = VS_ROLE_LOADED;
		pBMRoleInfo->nTime  = m_nTime;
	}
	ReportToWorld(pRole->GetID(),0,0,0,0,pRole->GetProfession(),pRole->GetLevel());
	return true;
}


void SceneBMFight::PickUpPoint(UINT64 dwRoleID,UINT32 job,UINT32 nLevel,UINT32 nPoint)
{
	if (m_nTime <= m_nFightTime)
	{
		ReportToWorld(dwRoleID,nPoint,0,0,0,job,nLevel);
	}
}

void SceneBMFight::SetRoleRankPoint(UINT64 dwRoleID, const KKSG::BMSceneRankRes& oData)
{
	stBMRoleInfo* pBMRoleInfo =  GetBMFightRole(dwRoleID);
	if(pBMRoleInfo)
	{
		if (oData.rank())
		{
			pBMRoleInfo->nRank = oData.rank();
		}
		if (oData.score())
		{
			pBMRoleInfo->nScore = oData.score();
		}
		if (oData.jointime())
		{
			pBMRoleInfo->nJoinTime = oData.jointime();
		}
		if (oData.deathcount())
		{
			pBMRoleInfo->nDead = oData.deathcount();
		}
		if (oData.killcount())
		{
			pBMRoleInfo->nKiller = oData.killcount();
		}
		LogInfo("roleid=%llu setrolerankpoint==%u,nrank=%u,jointime=%u,deathcount=%u",dwRoleID,pBMRoleInfo->nRank,pBMRoleInfo->nJoinTime,pBMRoleInfo->nDead,pBMRoleInfo->nKiller);
	}else
	{
		LogDebug("getrank roleid=%llu ",dwRoleID);
	}

}

bool SceneBMFight::OnRoleLeave(UINT64 roleid)
{
	m_loadmgr.DelRole(roleid);
	return true;
}


void SceneBMFight::OnEnemyDead(Enemy *pEnemy)
{
	UINT64 nRoleID = pEnemy->GetKillerID();
	Role *pRole =  GetRole(nRoleID);
	const XEntityStatistics::RowData* pRow = XEntityInfoLibrary::Instance()->TemplateID2Data_Enemy(pEnemy->GetTemplateID());
	if (pEnemy != NULL&&pRole!=NULL)
	{
		LogInfo("role kill boss roleid =%llu point=%u",nRoleID,pRow->BigMeleePoints);
		ReportToWorld(nRoleID,pRow->BigMeleePoints,0,0,0,pRole->GetProfession(),pRole->GetLevel());
	}
}


void SceneBMFight::OnEnterP321()
{
	Set321State();
}


stBMRoleInfo* SceneBMFight::GetBMFightRole(UINT64 roleid)
{
	auto iter = std::find_if(m_units.begin(),m_units.end(),BMRoleFind(roleid));
	if (iter == m_units.end())
	{
		return NULL;
	}
	return &(*iter);
}



bool SceneBMFight::OnRoleDie(UINT64 roleid)
{
	VsKillNotify(roleid, GetGlobalConfig().BigMeleeReviveTime, 0);
	Role* pDeadRole = GetRole(roleid);
	if (pDeadRole)
	{
		PtcG2C_BigMeleeReliveNtf ntf;
		ntf.m_Data.set_time(GetGlobalConfig().BigMeleeReviveTime);
		pDeadRole->Send(ntf);

		stBMRoleInfo* pDeadRoleInfo = GetBMFightRole(roleid);
		if (pDeadRoleInfo==NULL)
		{
			return false;
		}

		pDeadRoleInfo->nReliveTime = m_nTime + GetGlobalConfig().BigMeleeReviveTime;
		UINT64 killer =	pDeadRole->GetKillerID();
		Role* pKillerRole = GetRole(killer);
		if (pKillerRole)
		{
			UINT32 nExtraPoint = 0;
			for (size_t i = 0; i < GetGlobalConfig().BigMeleeKillerLvPoint.size(); i++)
			{
				if (pDeadRoleInfo->nRank >= GetGlobalConfig().BigMeleeKillerLvPoint[i][0] && pDeadRoleInfo->nRank <= GetGlobalConfig().BigMeleeKillerLvPoint[i][1])
				{
					nExtraPoint = GetGlobalConfig().BigMeleeKillerLvPoint[i][2];
				}
			}
			const Vector3& vec3 = pDeadRole->GetXObject()->GetPosition_p();
			UINT32 posxz = (int)(vec3.z * 100 + 0.5f);
			posxz |= ((int)(vec3.x * 100 + 0.5f) << 16);
			PtcG2C_BigMeleePointNtf ntf;
			ntf.m_Data.set_point(5+nExtraPoint);
			ntf.m_Data.set_posxz(posxz);
			pKillerRole->Send(ntf);
			ReportToWorld(killer,5+nExtraPoint,1,0,0,pKillerRole->GetProfession(),pKillerRole->GetLevel());

		}
		ReportToWorld(pDeadRole->GetID(),0,0,0,1,pDeadRole->GetProfession(),pDeadRole->GetLevel());
	}
	return true;
}


void SceneBMFight::OnEnterAfterFight()
{
	std::vector<UINT64> vecRole;
	for(size_t i = 0; i < m_units.size(); ++i)
	{
		ResetPosition(m_units[i].nRoleID);
		vecRole.push_back(m_units[i].nRoleID);
	}
	EndSkills(vecRole);
}

VsStateType SceneBMFight::CheckAfterFightEnd()
{
	return VS_STATE_DUMMY;
}

void SceneBMFight::OnRoleReconnect(Role* pRole)
{
	if (m_nTime >= m_nFightTime)
	{
		ResetPosition(pRole->GetID());
		std::vector<UINT64> vecRole;
		vecRole.push_back(pRole->GetID());
		EndSkills(vecRole);
		pRole->ChangeFightGroup(1);
	}
}

void SceneBMFight::FillBMRankList(const KKSG::BMRankListRes &ref)
{
	for (int i = 0; i < ref.roleid_size(); i++)
	{
		 stBMRoleInfo* pInfo = GetBMFightRole(ref.roleid(i));
		 if (pInfo)
		 {
			 pInfo->nRank =ref.rank(i);
		 }
	}
}


void SceneBMFight::ReqFinalRank()
{
	if (!m_units.empty())
	{
		RpcG2K_BMRankListReq  *pRpc = RpcG2K_BMRankListReq::CreateRpc();
		pRpc->m_oArg.set_sceneid(GetScene()->GetSceneID());
		for (auto iter = m_units.begin(); iter!=m_units.end(); iter++)
		{
			pRpc->m_oArg.add_roleids(iter->nRoleID);
			pRpc->m_oArg.add_svrid(m_nGroupSvr);
		}
		WorldLink::Instance()->SendTo(*pRpc);
	}
}

void SceneBMFight::ReportToWorld(UINT64 dwRole,UINT32 nScore,UINT32 nKiller,UINT32 nTime,UINT32 nDeadCount,UINT32 nJob,UINT32 nLv)
{
	RpcG2K_ReportBMRoleInfo *pRpc = RpcG2K_ReportBMRoleInfo::CreateRpc();
	pRpc->m_oArg.set_battleidx(m_nBattleIdx);
	pRpc->m_oArg.set_svrgroup(m_nGroupSvr);
	KKSG::BMRoleFightInfo  *pFight =  pRpc->m_oArg.mutable_roles();
	pFight->set_roleid(dwRole);
	pFight->set_score(nScore);
	pFight->set_killer(nKiller);
	stBMRoleInfo* pBMRole = GetBMFightRole(dwRole);
	if (pBMRole&&pBMRole->state==VS_ROLE_LOADED)
	{
		pFight->set_time(m_nTime-pBMRole->nTime);
		pBMRole->nTime = m_nTime;
		pBMRole->nScore += nScore; 
		if (nScore!=0)
		{
			PtcG2C_BigMeleePointOutLookNtf ntf;
			ntf.m_Data.set_roleid(dwRole);
			ntf.m_Data.set_point(pBMRole->nScore);
			m_scene->Broadcast(ntf);
		}

	}

	Role *pRole = GetRole(dwRole);
	if (pRole)
	{
		if (nKiller>0)
		{
			pRole->Get<TaskRecord>()->UpdateTask(KKSG::TaskOper_Add, KKSG::TaskConn_Activity, KKSG::TaskActType_BigmeleeKill,nKiller); 
		}
		if (nScore>0)
		{
			pRole->Get<TaskRecord>()->UpdateTask(KKSG::TaskOper_Add, KKSG::TaskConn_Activity, KKSG::TaskActType_BigmeleeScore,nScore);
		}
	}

	pFight->set_deadcount(nDeadCount);
	pFight->set_job(nJob);
	pFight->set_level(nLv);
	LogInfo("reportdata roleid=%llu battleidx=%d svrgroupidx=%d score=%d killer=%d deadcount=%d job=%d level=%d time=%d",dwRole,m_nBattleIdx,m_nGroupSvr,nScore,nKiller,nDeadCount,nJob,nLv,m_nTime);
	WorldLink::Instance()->SendTo(*pRpc);
}

void SceneBMFight::_CheckRoleRevive()
{
	if(m_nTime >= m_nFightTime)
		return;
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

void SceneBMFight::BattleResult()
{
	for (size_t i = 0; i < m_units.size(); i++)
	{
		Role *pRole = GetRole(m_units[i].nRoleID);
		if (pRole)
		{	
			NormalBattleResultListener listener(m_scene, true);
			listener.SetInitStatus(pRole);
			listener.SetSceneRank(RANK_SSS);
			KKSG::BigMeleeBattleResult * result = listener.GetBigMeleeBattleResult();
			result->set_kill(m_units[i].nKiller);
            GoalAwardsConfig::Instance()->SetValue(m_units[i].nRoleID, GOAL_AWARD_ID_901, m_units[i].nKiller, 0);
			result->set_score(m_units[i].nScore);
			result->set_roleid(m_units[i].nRoleID);
			result->set_rank(m_units[i].nRank);
			result->set_death(m_units[i].nDead);
			DoTxLog(pRole, m_units[i]);
			LogDebug("battlereulst roleid=%llu, rank=%d,score=%d,killer=%d",m_units[i].nRoleID,m_units[i].nRank,m_units[i].nScore,m_units[i].nKiller);
			std::vector<ItemDesc> list;
			MayhemConfigMgr::Instance()->GetRankAward(m_units[i].nRank,pRole->GetLevel(),list);
			MayhemConfigMgr::Instance()->GetPointAward(m_units[i].nScore,pRole->GetLevel(),list);
			if (m_units[i].nJoinTime>=GetGlobalConfig().BigMeleeJoinTime)
			{
				for (size_t j = 0; j < GetGlobalConfig().BigMeleeJoinAward.size(); j++ )
				{
					const ItemDesc* pItemDesc = ItemDropConfig::Instance()->GetRandomDrop(GetGlobalConfig().BigMeleeJoinAward[j],pRole->GetLevel());
					if (pItemDesc)
					{
						KKSG::ItemBrief* pItem =  result->add_items();
						pItem->set_itemid(pItemDesc->itemID);
						pItem->set_itemcount(pItemDesc->itemCount);
					}
				}
			}
			
			for (size_t i = 0; i < list.size(); i++)
			{
				KKSG::ItemBrief* pItem =  result->add_items();
				pItem->set_itemid(list[i].itemID);
				pItem->set_itemcount(list[i].itemCount);
			}

			listener.NotifyClient(pRole);
		}
	}
}

void SceneBMFight::DoTxLog(Role *pRole, const stBMRoleInfo& oData)
{
	if (NULL == pRole)
	{
		return;
	}
	TBigMeleeFightFlow oLog(pRole);
	oLog.m_nGroupSvr = m_nGroupSvr;
	oLog.m_nKiller = oData.nKiller;
	oLog.m_nScore = oData.nScore;
	oLog.m_nDead = oData.nDead;
	oLog.m_nRank = oData.nRank;
	oLog.m_nJoinTime = oData.nJoinTime;
	oLog.Do();
}

void SceneBMFight::SetResult()
{
	BattleResult();
	SetEnd();
	GetScene()->SetDontDestory(false);
}
