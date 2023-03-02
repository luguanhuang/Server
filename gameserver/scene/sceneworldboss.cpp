#include "pch.h"
#include "pb/enum.pb.h"
#include "sceneworldboss.h"
#include "network/mslink.h"
#include "unit/role.h"
#include "scene/scene.h"
#include "unit/enemymanager.h"
#include "table/PowerPointMgr.h"
#include "tablemgr/XEntityInfoLibrary.h"
#include "unit/combatattrdef.h"
#include "pb/project.pb.h"
#include "table/globalconfig.h"
#include "scene/scenemanager.h"
#include "worldboss/ptcg2c_worldbossstatentf.h"
#include "worldboss/ptcg2c_worldbossattrntf.h"
#include "worldboss/ptcg2m_worldbosssyncms.h"
#include "common/ptcg2c_countdownntf.h"
#include "gamelogic/reportdatarecord.h"
#include "guild/guildmgr.h"
#include "guild/guild.h"
#include "gamelogic/payv2Record.h"
#include "worldboss/ptcg2c_worldbossguildaddattrsyncclientntf.h"

#define WAITEND_TIME 5 
#define UPDATE_TIME 2
#define COUNT_DOWN GetGlobalConfig().WorldBossCountDown

WorldBossHandler* WorldBossHandler::CreateHandler(Scene* scene)
{
	if (scene->GetSceneType() == KKSG::SCENE_WORLDBOSS)
	{
		return new WorldBossHandler(scene);
	}
	else
	{
		return NULL;
	}
}

WorldBossHandler::WorldBossHandler(Scene* scene)
{
	m_scene = scene;
	m_boss = NULL;
	m_eBossState = eBorning;
	m_eState = eBeginPre;
	m_timestamp = 0;

	m_timer = CTimerMgr::Instance()->SetTimer(this, 0,  1000, -1, __FILE__, __LINE__);
}

WorldBossHandler::~WorldBossHandler()
{
	if (INVALID_HTIMER != m_timer)
	{
		CTimerMgr::Instance()->KillTimer(m_timer);
	}
}

void WorldBossHandler::OnHurt(Enemy* pEnemy, Unit* pCastUnit, double value)
{
	if (NULL != pEnemy && NULL != pCastUnit && pCastUnit->IsRole())
	{        
		Role* pRole = (Role*)pCastUnit;
		auto i = m_mapRoleInfo.find(pRole->GetID());
		if (i == m_mapRoleInfo.end())
		{
			WorldBossRoleInfo stRoleInfo;
			stRoleInfo.nDamage = (float)value;
			m_mapRoleInfo[pRole->GetID()] = stRoleInfo;
			pRole->OnHitWorldBoss();

			//SSDebug<<"sceneid:"<<m_scene->GetSceneID()<<" roleid:"<<pCastUnit->GetID()<<" damage:"<<value<<END;
		}
		else
		{
			i->second.nDamage += (float)value;

			//SSDebug<<"sceneid:"<<m_scene->GetSceneID()<<" roleid:"<<pCastUnit->GetID()<<" damage:"<<value<<END;
		}
	}
	else
	{
		if (NULL == pEnemy)
		{
			SSWarn<<"enemy is NULL"<<END;
		}
		if (NULL == pCastUnit)
		{
			SSWarn<<"caster is NULL"<<END;
		}
		else
		{
			if (!pCastUnit->IsRole())
			{
				SSWarn<<"caster is not role, id:"<<pCastUnit->GetID()<<END;
			}
		}
	}
}

void WorldBossHandler::OnDied(Enemy* pEnemy)
{
	m_boss = NULL;
	m_eBossState = eDead;

	PtcG2M_WorldBossSyncMS ntf;
	ntf.m_Data.set_isbossdead(true);
	MSLink::Instance()->SendTo(ntf);
}

void WorldBossHandler::Update()
{
	if (m_eState == eGoing)
	{
		time_t nowtime = time(NULL);
		if (nowtime > m_timestamp + UPDATE_TIME)
		{
			OnRank();
			m_timestamp = nowtime;
		}
	}
}

void WorldBossHandler::OnRank()
{
	PtcG2M_WorldBossSyncMS sync;
	sync.m_Data.set_sceneid(m_scene->GetSceneID());
	KKSG::WorldBossRoleDamage* damages = sync.m_Data.mutable_damage();

	bool ischange = false;
	for (auto i = m_mapRoleInfo.begin(); i != m_mapRoleInfo.end(); ++i)
	{
		int nDamage = (int)i->second.nDamage;
		if (0 == nDamage)
		{
			continue;
		}
		damages->add_roleid(i->first);
		damages->add_damage(i->second.nDamage);

		//SSDebug<<"sceneid:"<<m_scene->GetSceneID()<<" roleid:"<<i->first<<" damage:"<<i->second.nDamage<<END;

		i->second.nDamage = 0.0f;
		ischange = true;
	}
	if (ischange)
	{
		MSLink::Instance()->SendTo(sync);
	}
}

void WorldBossHandler::BroadCast(UINT32 state, UINT32 time)
{
	PtcG2C_WorldBossStateNtf ntf;
	ntf.m_Data.set_state(KKSG::WorldBossState(state));
	ntf.m_Data.set_time(time);
	ntf.m_Data.set_iswin(NULL == m_boss || m_boss->IsDead());

	m_scene->Broadcast(ntf);
}

UINT32 WorldBossHandler::GetAddAttrCount(UINT64 qwRoleId)
{
	auto i = m_mapRoleInfo.find(qwRoleId);
	if (i == m_mapRoleInfo.end())
	{
		return 0;
	}
	return i->second.dwAddAttrCount;
}

UINT32 WorldBossHandler::PushAddAttrCount(Role* role)
{
	auto i = m_mapRoleInfo.find(role->GetID());
	if (i == m_mapRoleInfo.end())
	{
		WorldBossRoleInfo stRoleInfo;
		stRoleInfo.dwAddAttrCount = 1;
		m_mapRoleInfo[role->GetID()] = stRoleInfo;

		role->OnHitWorldBoss();

		return 1;
	}
	return ++(i->second.dwAddAttrCount);
}

void WorldBossHandler::OnEnterSceneFinally(Scene* scene, Role* pRole)
{
	pRole->Get<CReportDataRecord>()->UpdateWeekData(KKSG::WeekReportData_WorldBoss);
	///> 开始计时
	m_roleid2time[pRole->GetID()].time = (UINT32)time(NULL) + GetGlobalConfig().WorldBossPlayTime;

	///> 还原增加属性
	UINT32 dwCount = GetAddAttrCount(pRole->GetID());
	if (0 != dwCount)
	{
		for (auto i =  GetGlobalConfig().WorldBossAddAttr.begin(); i !=  GetGlobalConfig().WorldBossAddAttr.end(); ++i)
		{
			PowerPointMgr::Instance()->AdjustOneAttributeByCoeffcient(pRole, i->seq[0], (float)(i->seq[1]*dwCount)/100.0f);
		}
	}

	pRole->Get<CPayV2Record>()->GiveBuff(KKSG::SCENE_WORLDBOSS);

	PtcG2C_WorldBossAttrNtf ntf;
	ntf.m_Data.set_count(dwCount);
	pRole->Send(ntf);

	UINT64 guildid = pRole->getGuildId();
	UINT32 count = GuildSimpleMgr::Instance()->GetGuildAttr(guildid);
	if (0 != count)
	{
		// add attr
		for (auto i = GetGlobalConfig().WorldBossGuildAddAttr.begin(); i != GetGlobalConfig().WorldBossGuildAddAttr.end(); ++i)
		{
			PowerPointMgr::Instance()->AdjustOneAttributeByCoeffcient(pRole, i->seq[0], (float)(i->seq[1]*count)/100.0f);
		}
	}
	// notify client
	PtcG2C_WorldBossGuildAddAttrSyncClientNtf guildntf;
	ntf.m_Data.set_count(count);
	pRole->Send(guildntf);
}

void WorldBossHandler::OnRemoveUnit(Scene *pScene, Unit* unit)
{
	if (!unit->IsRole())
	{
		return;
	}
	auto i = m_roleid2time.find(unit->GetID());
	if (i != m_roleid2time.end())
	{
		m_roleid2time.erase(i);
		SSInfo<<"remove role:"<<unit->GetID()<<END;
	}
}

bool WorldBossHandler::CreateWorldBoss()
{
	if (NULL != m_boss)
	{
		return false;
	}
	m_boss= EnemyManager::Instance()->CreateEnemy(m_bossinfo.dwEnemyId, m_bossinfo.stPos, m_bossinfo.fRotate);
	if(NULL == m_boss)
	{
		SSWarn<<"create enemy failed"<<END;
		return false;
	}
	m_boss->SetCycleSync(GetGlobalConfig().WorldBossSyncTime);

	m_boss->SetAttr(BASIC_MaxHP, m_bossinfo.fMaxHp);
	m_boss->SetAttr(BASIC_CurrentHP, m_bossinfo.fCurrentHp);
	m_boss->SetAttr(PERCENT_PhysicalAtk, m_bossinfo.fAttackPercent);

	m_boss->AddListener(this);

	m_scene->AddUnitAndNotify(m_boss);

	m_eBossState = eActive;

	SSInfo<<"create world boss:"<<m_bossinfo.dwId<<" enemy id:"<<m_bossinfo.dwEnemyId<<" hp:"<<m_bossinfo.fCurrentHp<<END;
	return true;
}

void WorldBossHandler::RemoveWorldBoss()
{
	if (NULL != m_boss && !m_boss->IsDead())
	{
		m_boss->TriggerDeath();
	}
	m_boss = NULL;
}

void WorldBossHandler::InitBoss(UINT32 enemyid, float attackpercent, float currenthp, float maxhp)
{
	m_bossinfo.dwEnemyId = enemyid;
	m_bossinfo.fAttackPercent = attackpercent;
	m_bossinfo.fCurrentHp = currenthp;
	m_bossinfo.fMaxHp = maxhp;

	m_eBossState = eBorning;

	OnSceneTimeBegin();
}

void WorldBossHandler::KickOutAllRole()
{
	if (NULL != m_scene)
	{
		m_scene->KickAllRoleOut();
	}
}

void WorldBossHandler::OnSceneTimeBegin()
{
	// handle in world boss gs
	///> 清空其他数据
	m_mapRoleInfo.clear();

	CreateWorldBoss();

	m_eState = eGoing;
}

void WorldBossHandler::OnSceneTimeWaitEnd()
{
	BroadCast(eWaitEnd, WAITEND_TIME);

	if (!IsBossActive())
	{
		return;
	}
	// handle in world boss gs
	///> 停止AI
	EnableWorldBoss(false);
	///> 加无敌buff
	InvincibleWorldBoss(true);

	m_eBossState = eAIDisable;
	m_eState = eWaitEnd;
}

void WorldBossHandler::OnSceneTimeEnd()
{
	///> 踢人
	//KickOutAllRole();
	///> 清数据
	m_mapRoleInfo.clear();

	RemoveWorldBoss();

	m_eBossState= eDead;
	m_eState = eEnd;

	m_scene->SetDontDestory(false);
}

void WorldBossHandler::EnableWorldBoss(bool bIsEnable)
{
	if (NULL == m_boss)
	{
		return;
	}
	m_boss->EnableAI(bIsEnable);
}

void WorldBossHandler::InvincibleWorldBoss(bool bIsInvincible)
{
	if (NULL == m_boss)
	{
		return;
	}
	m_boss->Invincible(bIsInvincible);
}

void WorldBossHandler::SubHp(float hp)
{
	if (NULL == m_boss)
	{
		return;
	}
	m_boss->SubHp(NULL, hp);
}

void WorldBossHandler::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	if (m_eState == eGoing)
	{
		///> 不限制时间
		if (0 == GetGlobalConfig().WorldBossPlayTime)
		{
			return;
		}
		UINT32 nowtime = (UINT32)time(NULL);
		for (auto i = m_roleid2time.begin(); i != m_roleid2time.end(); ++i)
		{
			if (nowtime + COUNT_DOWN >= i->second.time)
			{
				if (!i->second.isnotify)
				{
					i->second.isnotify = true;

					PtcG2C_CountDownNtf ntf;
					ntf.m_Data.set_time(COUNT_DOWN - 1);
					Role* role = m_scene->FindSceneRole(i->first);
					if (NULL != role)
					{
						role->Send(ntf);
					}
				}
			}
		}
		for (auto i = m_roleid2time.begin(); i != m_roleid2time.end(); )
		{
			if (nowtime > i->second.time)
			{
				///> 拉回主城，默认成功
				Role* role = m_scene->FindSceneRole(i->first);
				if (NULL != role)
				{
					SceneManager::Instance()->ChangeScene(role, HALL_MAP_ID);
				}
				m_roleid2time.erase(i++);
				///> 每秒只拉一个人，分摊压力
				break;
			}
			else
			{
				++i;
			}
		}
	}
}

void WorldBossHandler::OnRoleReconnect(Role* role)
{
	UINT64 guildid = role->getGuildId();
	UINT32 count = GuildSimpleMgr::Instance()->GetGuildAttr(guildid);
	if (0 != count)
	{
		PtcG2C_WorldBossGuildAddAttrSyncClientNtf ntf;
		ntf.m_Data.set_count(count);
		role->Send(ntf);
	}
}
