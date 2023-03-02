#include "pch.h"
#include "time.h"
#include "network/gslink.h"
#include "util/XCommon.h"
#include "worldbossmgr.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "pb/project.pb.h"
#include "worldbossline.h"
#include "common/ptcm2c_hintnotifyms.h"
#include "global/GlobalConfigDBBuild.h"
#include "worldboss/ptcm2g_worldbosssyncgs.h"
#include "tablemgr/XEntityInfoLibrary.h"
#include "mail/maildata.h"
#include "ranklist/ranklist.h"
#include "ranklist/ranklistmgr.h"
#include "ranklist/worldbossranklist.h"
#include "ranklist/worldbossguildranklist.h"
#include "mail/mailmgr.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "notice/notice.h"
#include "mail/mailconfig.h"
#include "table/globalconfig.h"
#include "define/systemiddef.h"
#include "table/guildauctconfig.h"
#include "guildauct/guildauctbase.h"
#include "guild/guildmgr.h"
#include "guild/guild.h"
#include "guildauct/guildauctmgr.h"
#include "event/eventmgr.h"
#include "define/eventdef.h"
#include "common/ptcm2c_mserrornotify.h"
#include "config.h"
#include "tablemgr/backflowtablemgr.h"
#include "loghelper/tlogr.h"
#include "goalawards/goalawardsmgr.h"
#include "task/roletaskmgr.h"
#include "reswar/ptcm2c_halliconmntf.h"

#define WAITEND_TIME (GetGlobalConfig().WorldBossGameEndCountDown)
#define WORLDBOSS_ROLELIMIT (GetGlobalConfig().WorldBossRoleLimit)
#define WORLDBOSS_PLAYTIME  (GetGlobalConfig().WorldBossPlayTime)
#define WORLDBOSS_PLAYCOUNT (GetGlobalConfig().WorldBossPlayCount)

INSTANCE_SINGLETON(WorldBossMgr)

WorldBossMgr::WorldBossMgr()
{
	ClearFile();

	m_isdead = false;
	m_dwWaitEndTime = 0;
	m_noticetime = 0;
	m_eState = eBeginPre;
	m_TimeHandler = INVALID_HTIMER;
}

WorldBossMgr::~WorldBossMgr()
{

}

bool WorldBossMgr::Init()
{
	if (!LoadFile())
	{
		return false;
	}

	UINT32 bossid = convert<UINT32>(CGlobalConfigDBBuild::Instance()->GetConfigValue(eWorldBossId));
	UINT32 enemyid = convert<UINT32>(CGlobalConfigDBBuild::Instance()->GetConfigValue(eWorldBossEnemyId));
	InitBoss(bossid, enemyid);

	m_TimeHandler = CTimerMgr::Instance()->SetTimer(this, 0, 1000, -1, __FILE__, __LINE__);

	return true;
}

void WorldBossMgr::Uninit()
{
	CTimerMgr::Instance()->KillTimer(m_TimeHandler);
}

void WorldBossMgr::ClearFile()
{
	m_oBossTable.Clear();
	m_rewardtable.Clear();
	m_leve2reward.clear();

	m_guildreward.Clear();
}

bool WorldBossMgr::LoadFile()
{
	if (!CheckFile())
	{
		return false;
	}

	ClearFile();

	if (!m_oBossTable.LoadFile("table/Boss.txt"))
	{
		SSWarn<<"load file table/Boss.txt failed"<<END;
		return false;
	}
	if (!m_rewardtable.LoadFile("table/WorldBossReward.txt"))
	{
		SSWarn<<"load file table/WorldBossReward.txt failed"<<END;
		return false;
	}
	for (auto i = m_rewardtable.Table.begin(); i != m_rewardtable.Table.end(); ++i)
	{
		UINT32 dwLevel = (*i)->Level;
		auto j = m_leve2reward.find(dwLevel);
		if (j == m_leve2reward.end())
		{
			std::vector<WorldBossRewardTable::RowData*> vecReward;
			vecReward.push_back(*i);
			m_leve2reward[dwLevel] = vecReward;
		}
		else
		{
			j->second.push_back(*i);
		}
	}
	if (!m_guildreward.LoadFile("table/WorldBossGuildReward.txt"))
	{
		SSWarn<<"load file table/WorldBossGuildReward.txt failed"<<END;
		return false;
	}

	return true;
}

bool WorldBossMgr::CheckFile()
{
	BossTable tempBossTable;
	if (!tempBossTable.LoadFile("table/Boss.txt"))
	{
		SSWarn<<"load file table/Boss.txt failed"<<END;
		return false;
	}
	WorldBossRewardTable temprewardtable;
	if (!temprewardtable.LoadFile("table/WorldBossReward.txt"))
	{
		SSWarn<<"load file table/WorldBossReward.txt failed"<<END;
		return false;
	}
	WorldBossGuildRewardTable tempguildrewardtable;
	if (!tempguildrewardtable.LoadFile("table/WorldBossGuildReward.txt"))
	{
		SSWarn<<"load file table/WorldBossGuildReward.txt failed"<<END;
		return false;
	}

	return true;
}

void WorldBossMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	time_t dwNowTime = GameTime::GetTime();
	switch(m_eState)
	{
	case eBeginPre:
		{
			time_t dwBeginTime = GetBeginTime(dwNowTime);
			time_t dwEndTime = GetEndTime(dwNowTime);
			if (dwNowTime >= dwBeginTime && dwNowTime <= dwEndTime)
			{
				m_eState = eBegin;
			}
			break;
		}
	case eBegin:
		{
			OnSceneTimeBegin();
			m_eState = eGoing;
			break;
		}
	case eGoing:
		{
			time_t dwEndTime = GetEndTime(dwNowTime);
			if (dwNowTime > dwEndTime || IsBossDead())
			{
				m_dwWaitEndTime = dwNowTime;
				m_eState = eWaitEnd;
				OnSceneTimeWaitEnd();
			}
			break;
		}
	case eWaitEnd:
		{
			if (dwNowTime >= WAITEND_TIME + m_dwWaitEndTime)
			{
				m_dwWaitEndTime = 0;
				m_eState = eEnd;
			}
			break;
		}
	case eEnd:
		{
			OnSceneTimeEnd();
			m_eState = eDummy;
			break;
		}
	case eDummy:
		{
			time_t dwEndTime = GetEndTime(dwNowTime);
			if (dwNowTime > dwEndTime)
			{
				m_eState = eBeginPre;
			}
			break;
		}
	default:
		break;
	}
}

void WorldBossMgr::DesRole(UINT64 roleid)
{
	auto i = m_role2time.find(roleid);
	if (i == m_role2time.end())
	{
		SSWarn<<"not find roleid:"<<roleid<<END;
	}
	else
	{
		m_role2time.erase(i);
	}
}

UINT32 WorldBossMgr::CanEnter(UINT64 roleid)
{
	if (IsSceneEnd() || IsBossDead())
	{
		return KKSG::ERR_WORLDBOSS_DEAD;
	}
	if (!IsSceneOpen())
	{
		return KKSG::ERR_SCENE_TIMELIMIT;
	}
	///> 距离结束还有一段时间不给进
	int timeleft = GetEndTimeLeft();
	if (timeleft <= GetGlobalConfig().WorldBossTimeEnd)
	{
		ErrorNotify(roleid, KKSG::ERR_SLOTATTR_LEVELLIMIT);
		return KKSG::ERR_SLOTATTR_LEVELLIMIT;
	}

	if (0 != roleid)
	{
		///> 人数满了
		if ((int)m_role2time.size() >= WORLDBOSS_ROLELIMIT)
		{
			ErrorNotify(roleid, KKSG::ERR_OTHER_GAOJIGU_NOTOPEN);
			return KKSG::ERR_OTHER_GAOJIGU_NOTOPEN;
		}
		///> 次数限制
		if ((int)m_role2count[roleid] >= WORLDBOSS_PLAYCOUNT)
		{
			ErrorNotify(roleid, KKSG::ERR_ARENA_COUNTLIMIT);
			return KKSG::ERR_ARENA_COUNTLIMIT;
		}

		++m_role2count[roleid];
		m_role2time[roleid] = (UINT32)GameTime::GetTime() + WORLDBOSS_PLAYTIME;
	}
	return KKSG::ERR_SUCCESS;	
}

void WorldBossMgr::OnSceneTimeBegin()
{
	// ranklist
	BossDamageRankList* ranklist = (BossDamageRankList*)CRankListMgr::Instance()->GetRankList(KKSG::WorldBossDamageRank);	
	if (NULL != ranklist)
	{
		ranklist->Clear();
	}
	WorldBossGuildRankList* guildranklist = (WorldBossGuildRankList*)CRankListMgr::Instance()->GetRankList(KKSG::WorldBossGuildRank);	
	if (NULL != guildranklist)
	{
		guildranklist->Clear();
	}

	m_isdead = false;
	
	SyncGS(m_eState);
	NoticeState(true);

	m_role2count.clear();
	m_role2time.clear();
	m_guild2attrcount.clear();

	SSInfo<<"world boss begin"<<END;
}

void WorldBossMgr::OnSceneTimeWaitEnd()
{
	///> 广播给玩家
	SyncGS(m_eState);

	SSInfo<<"world boss wait end"<<END;
}

void WorldBossMgr::GiveRankReward()
{
	// ranklist && reward
	UINT64 first = 0;
	UINT64 second = 0;
	UINT64 third = 0;
	const MailConf* conf = MailConfig::Instance()->GetMailConf(MailConf_WorldBoss);

	const MailConf* conf_kill = MailConfig::Instance()->GetMailConf(MailConf_WorldBossKill);
	std::vector<ItemDesc> items_kill;
	for (auto i = GetGlobalConfig().WorldBossKillReward.begin(); i != GetGlobalConfig().WorldBossKillReward.end(); ++i)
	{
		ItemDesc item;
		item.itemID = i->seq[0];
		item.itemCount = i->seq[1];
		items_kill.push_back(item);
	}

	BossDamageRankList* ranklist = (BossDamageRankList*)CRankListMgr::Instance()->GetRankList(KKSG::WorldBossDamageRank);	
	if (NULL != ranklist)
	{
		// save to db
		ranklist->SaveToDb();

		std::vector<BossDamageRankInfo*> damages;
		ranklist->GetRankInfo(damages);
		UINT32 rank = 0;
		for (auto i = damages.begin(); i != damages.end(); ++i)
		{
			BossDamageRankInfo* info = *i;
			UINT64 roleid = info->GetUniqueId();
			++rank;
			if (rank == 1)
			{
				first = roleid;
			}
			else if (rank == 2)
			{
				second = roleid;
			}
			else if (rank == 3)
			{
				third = roleid;
			}
			CRole* role = CRoleManager::Instance()->GetByRoleID(roleid);
			if (NULL != role)
			{
				Notice all(WorldBossAll);
				all.Add((UINT32)(*i)->m_damage);
				all.SendRole(role->GetID());
			}
			CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(roleid);
			if (NULL != summary)
			{
				std::vector<ItemDesc> items;
				GetReward(summary->GetLevel(), rank, items);
				if (!items.empty())
				{
					MailData mail(NULL == conf ? "" : conf->m_title, NULL == conf ? "" : conf->m_content, items, MAIL_EXPIRE_TIME);
					mail.SetReason(ItemFlow_WorldBossAward);
					CMailMgr::Instance()->SendMail(info->GetUniqueId(), mail);
				}
				///> boss被击杀的参与奖
				if (IsBossDead())
				{
					if (!items_kill.empty())
					{
						MailData mail(NULL == conf_kill ? "" : conf_kill->m_title, NULL == conf_kill ? "" : conf_kill->m_content, items_kill, MAIL_EXPIRE_TIME);
						mail.SetReason(ItemFlow_WorldBossKillAward);
						CMailMgr::Instance()->SendMail(info->GetUniqueId(), mail);
					}
				}

				EventMgr::Instance()->AddEventOnlyOnMS(roleid, MENTOR_EVENT_OFFSET + KKSG::MentorTask_WorldBossRank, 0, rank);
				EventMgr::Instance()->AddEventOnlyOnMS(roleid, MENTOR_EVENT_OFFSET + KKSG::MentorTask_WorldBossCount, 0, 1);

                // 目标奖励
                {
                    GoalAwardsMgr::Instance()->FreshValue(roleid, GOAL_AWARD_ID_1001, rank, 0);
                    GoalAwardsMgr::Instance()->FreshValue(roleid, GOAL_AWARD_ID_1002, info->m_damage, 0);
                    GoalAwardsMgr::Instance()->FreshValue(roleid, GOAL_AWARD_ID_1003, 1.f, 0);
                }
			}
			DoTxLog(rank, roleid, (*i)->m_damage, summary);
			RoleTaskMgr::Instance()->UpdateTask(roleid, KKSG::TaskOper_Set, KKSG::TaskConn_WorldBossRank, rank, 1);
			RoleTaskMgr::Instance()->UpdateTask(roleid, KKSG::TaskOper_Add, KKSG::TaskConn_Activity, KKSG::TaskActType_WorldBoss);
		}
	}

	// notice
	NoticeRank(1, first);
	NoticeRank(2, second);
	NoticeRank(3, third);
}

void WorldBossMgr::NoticeGuild()
{
	WorldBossGuildRankList* guildranklist = (WorldBossGuildRankList*)CRankListMgr::Instance()->GetRankList(KKSG::WorldBossGuildRank);	
	if (NULL != guildranklist)
	{
		guildranklist->SaveToDb();
		///> 工会公告
		for (auto i = guildranklist->m_guild2info.begin(); i != guildranklist->m_guild2info.end(); ++i)
		{
			std::set<std::pair<float, UINT64>> sorts;
			for (auto j = i->second.begin(); j != i->second.end(); ++j)
			{
				sorts.insert(std::make_pair(j->second, j->first));
			}
			std::string names;
			int count = 0;
			for (auto k = sorts.rbegin(); k != sorts.rend(); ++k)
			{
				if (count >= 3)
				{
					break;
				}
				if (0 != count)
				{
					names += ", ";
				}
				CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(k->second);
				if (NULL != summary)
				{
					names += summary->GetName();
					++count;
				}
			}
			Notice guildnotice(WorldBossGuild);
			guildnotice.Replace("$C", names);
			guildnotice.SendGuild(i->first);
		}
	}
}

void WorldBossMgr::OnSceneTimeEnd()
{
	InitBoss(++m_stBossInfo.dwId, 0);
	SendBossInfoToDb();

	SyncGS(m_eState);
	NoticeState(false);

	WorldBossLineMgr::Instance()->Clear();

	GiveRankReward();

	NoticeGuild();

	GiveAuctReward();	

	SSInfo<<"world boss end"<<END;
}

void WorldBossMgr::NoticeRank(UINT32 rank, UINT64 roleid)
{
	if (0 == roleid)
	{
		return;
	}
	CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(roleid);
	if (NULL == summary)
	{
		return;
	}
	if (rank == 1)
	{
		Notice all(WorldBossFirst);
		all.AddRole(summary);
		all.Send();
	}
	else if (rank == 2 || rank == 3)
	{
		Notice all(WorldBossSecond);
		all.AddRole(summary);
		all.Add(rank);
		all.Send();
	}
}

void WorldBossMgr::InitBoss(UINT32 id, UINT32 enemyid)
{
	if (MSConfig::Instance()->IsBackFlowServer())
	{
		BackFlowServerConfig::RowData* pData = BackFlowTableMgr::Instance()->GetServerConfigByLevel(MSConfig::Instance()->GetBackFlowLevel());
		if (pData && id < pData->WorldBoss)
		{
			id = pData->WorldBoss;
		}
	}
	LogInfo("bossid:%u", id);

	m_stBossInfo.dwId = id;
	BossTable::RowData* pRowData = NULL;
	pRowData = m_oBossTable.GetByBossID(id);
	if (NULL == pRowData)
	{
		SSWarn<<"init world boss empty, id:"<<id<<END;
		///> 默认刷第一个boss
		if (m_oBossTable.Table.empty())
		{
			SSWarn<<"init world boss failed, id:"<<id<<END;
			return;
		}
		pRowData = m_oBossTable.Table[0];
		m_stBossInfo.dwId = pRowData->BossID;
	}
	m_stBossInfo.dwEnemyId = (0 == enemyid) ? RandomBoss(pRowData->EnemyID) : enemyid;
	m_stBossInfo.fAttackPercent = pRowData->AttackPercent;
	m_stBossInfo.fLifePercent = pRowData->LifePercent;

	const XEntityStatistics::RowData* pEnemy= XEntityInfoLibrary::Instance()->TemplateID2Data_Enemy(m_stBossInfo.dwEnemyId);
	float fMaxHp = 0.0f;
	if (NULL != pEnemy)
	{
		fMaxHp = pEnemy->MaxHP;
	}
	m_stBossInfo.maxhp = fMaxHp * m_stBossInfo.fLifePercent;
	m_stBossInfo.currenthp = m_stBossInfo.maxhp; 
}

void WorldBossMgr::SubHp(float hp)
{
	if (m_stBossInfo.currenthp > hp)
	{
		m_stBossInfo.currenthp -= hp;
	}
	else
	{
		m_stBossInfo.currenthp = 0.0f;
	}
	float lastpercent = m_stBossInfo.percent;
	m_stBossInfo.percent = m_stBossInfo.currenthp/m_stBossInfo.maxhp;
	NoticePercent(lastpercent, m_stBossInfo.percent);
}

void WorldBossMgr::SendBossInfoToDb()
{
	CGlobalConfigDBBuild::Instance()->SetConfigValue(eWorldBossId, ToString(m_stBossInfo.dwId));
	CGlobalConfigDBBuild::Instance()->SetConfigValue(eWorldBossEnemyId, ToString(m_stBossInfo.dwEnemyId));
	CGlobalConfigDBBuild::Instance()->UpdateToDb(eWorldBossId);
	CGlobalConfigDBBuild::Instance()->UpdateToDb(eWorldBossEnemyId);
}

time_t WorldBossMgr::GetBeginTime(time_t dwNowTime)
{
	return MultActivityMgr::Instance()->GetOpenTime(MULT_ACTIVITY_WORLD_BOSS);
}

time_t WorldBossMgr::GetEndTime(time_t dwNowTime)
{
	return MultActivityMgr::Instance()->GetEndTime(MULT_ACTIVITY_WORLD_BOSS);
}

UINT32 WorldBossMgr::GetEndTimeLeft()
{
	time_t dwNowTime = GameTime::GetTime();
	return (UINT32)(dwNowTime < GetEndTime(dwNowTime) ? (GetEndTime(dwNowTime) - dwNowTime) : 0);
}

UINT32 WorldBossMgr::GetRoleEndTimeLeft(UINT64 roleid)
{
	if (0 == GetGlobalConfig().WorldBossPlayTime)
	{
		return (UINT32)(-1);
	}
	auto i = m_role2time.find(roleid);
	if (i == m_role2time.end())
	{
		return (UINT32)(-1);
	}
	else
	{
		UINT32 nowtime = (UINT32)GameTime::GetTime();
		return i->second > nowtime ? i->second - nowtime : 0; 
	}
}

UINT32 WorldBossMgr::GetBeginTimeLeft()
{
	time_t nowtime = GameTime::GetTime();
	time_t begintime = GetBeginTime(nowtime);
	time_t endtime = GetEndTime(nowtime);
	if (begintime > nowtime)
	{
		return (UINT32)(begintime - nowtime);
	}
	else
	{
		return (UINT32)(begintime + 24*3600 - nowtime);
	}
}

void WorldBossMgr::HintNotify(CRole* role)
{
	if (IsSceneOpen())
	{
		role->HintNotify(SysWorldBossId, false);
	}
}

void WorldBossMgr::NoticeState(bool startorend)
{
	PtcM2C_HintNotifyMS ntf;
	ntf.m_Data.add_systemid(SysWorldBossId);
	ntf.m_Data.set_isremove(!startorend);
	CRoleManager::Instance()->BroadCast(ntf);

	if (!startorend)
	{
		PtcM2C_HallIconMNtf hallntf;
		hallntf.m_Data.set_state(HICONS_BEGIN);
		hallntf.m_Data.set_systemid(SysRankWorldBossId);
		CRoleManager::Instance()->BroadCast(hallntf);
	}
}

void WorldBossMgr::ResetAll(UINT32 beginhour, UINT32 beginmin, UINT32 endhour, UINT32 endmin)
{
	m_eState = eBeginPre;
	MultActivityMgr::Instance()->GMAddOpenDayTime(MULT_ACTIVITY_WORLD_BOSS,
										          beginhour * 3600 + beginmin * 60,
												  endhour * 3600 + endmin * 60);
}

void WorldBossMgr::SyncGS(UINT32 state)
{
	PtcM2G_WorldBossSyncGS ntf;
	ntf.m_Data.set_state(state);
	GSLink::Instance()->SendToAllLine(ntf);
}

UINT32 WorldBossMgr::RandomBoss(std::vector<Sequence<UINT32, 2>>& bosses)
{
	UINT32 count = 0;
	for (auto i = bosses.begin(); i != bosses.end(); ++i)
	{
		count += i->seq[1];
	}
	UINT32 enemyid = 0;
	UINT32 random = XRandom::randInt(1, count + 1);
	for (auto i = bosses.begin(); i != bosses.end(); ++i)
	{
		enemyid = i->seq[0];
		if (i->seq[1] >= random)
		{
			break;		
		}
	}
	return enemyid;
}

bool WorldBossMgr::GetReward(UINT32 level, UINT32 rank, std::vector<ItemDesc>& items)
{
	for (auto i = m_leve2reward.begin(); i != m_leve2reward.end(); ++i)
	{
		if (level <= i->first)
		{
			for (auto j = i->second.begin(); j != i->second.end(); ++j)
			{
				WorldBossRewardTable::RowData* pRowData = *j;
				if (rank >= pRowData->Rank[0] && rank <= pRowData->Rank[1])
				{
					for (auto k = pRowData->ShowReward.begin(); k != pRowData->ShowReward.end(); ++k)
					{
						items.push_back(ItemDesc(k->seq[0], k->seq[1]));
					}
					return true;
				}			
			}
		}
	}
	return false;
}

void WorldBossMgr::NoticePercent(float lastper, float per)
{
	UINT32 noticeid = 0;
	UINT32 noticeidguild = 0;
	if (lastper > 0.25 && per <= 0.25)
	{
		noticeid = WorldBoss25;
		noticeidguild = WorldBossGuild25;
	}
	else if (lastper > 0.5 && per <= 0.5)
	{
		noticeid = WorldBoss50;
		noticeidguild = WorldBossGuild50;
	}
	else if (lastper > 0.75 && per <= 0.75)
	{
		noticeid = WorldBoss75;
		noticeidguild = WorldBossGuild75;
	}
	if (0 != noticeid)
	{
		BossDamageRankList* ranklist = (BossDamageRankList*)CRankListMgr::Instance()->GetRankList(KKSG::WorldBossDamageRank);
		const BossDamageRankInfo* info = ranklist->GetInfoByRank(1);
		if (NULL != info)
		{
			CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(info->GetUniqueId());
			if (NULL != summary)
			{
				Notice notice(noticeid);
				notice.AddRole(summary);
				notice.Send();
			}
		}
		WorldBossGuildRankList* guildranklist = (WorldBossGuildRankList*)CRankListMgr::Instance()->GetRankList(KKSG::WorldBossGuildRank);
		const BossGuildRankInfo* guildinfo = guildranklist->GetInfoByRank(1);
		if (NULL != guildinfo)
		{
			Guild* guild = CGuildMgr::Instance()->GetGuild(guildinfo->GetUniqueId());
			if (NULL != guild)
			{
				Notice notice(noticeidguild);
				notice.AddGuild(guild->GetID());
				notice.Send();
			}
		}
	}
}

void WorldBossMgr::GiveAuctReward()
{
	BossDamageRankList* roleranklist = (BossDamageRankList*)CRankListMgr::Instance()->GetRankList(KKSG::WorldBossDamageRank);	
	WorldBossGuildRankList* guildranklist = (WorldBossGuildRankList*)CRankListMgr::Instance()->GetRankList(KKSG::WorldBossGuildRank);	
	if(NULL == roleranklist || NULL == guildranklist)
	{
		return ;
	}
	int RankCount = GuildAuctConfig::Instance()->GetRankCount(GA_ACT_WORLD_BOSS);
	std::vector<BossGuildRankInfo*> guildvec;
	guildranklist->GetRankInfo(guildvec);
	int rank = 0;
	for(size_t i = 0; i < guildvec.size(); ++i)
	{
		++rank;
		if(rank <= RankCount)
		{
			std::vector<UINT64> joinroles;
			UINT64 guildid = guildvec[i]->GetUniqueId();
			auto pGuild = CGuildMgr::Instance()->GetGuild(guildid);
			if(pGuild)
			{
				const auto& members = pGuild->GetGuildMember();
				for (auto iter = members.begin(); iter != members.end(); ++iter)
				{
					if(INVALID_RANK != roleranklist->GetRank(iter->first))		
					{
						joinroles.push_back(iter->first);
					}
				}
				/*
				for(size_t j = 0; j < members.size(); ++j)
				{
					if(INVALID_RANK != roleranklist->GetRank(members[j].qwRoleID))		
					{
						joinroles.push_back(members[j].qwRoleID);
					}
				}
				*/
			}
			GuildAuctMgr::Instance()->AddGuildAuctData(guildid, GA_ACT_WORLD_BOSS, rank, joinroles);
		}
		UINT32 reward = GetGuildReward(rank);
		if (0 != reward)
		{
			UINT64 guildid = guildvec[i]->GetUniqueId();
			Guild* pGuild = CGuildMgr::Instance()->GetGuild(guildid);
			if (NULL != pGuild)
			{
				pGuild->AddPrestige(reward);
			}
		}
	}
}

UINT32 WorldBossMgr::GetGuildReward(UINT32 rank)
{
	for (UINT32 i = 0; i < m_guildreward.Table.size(); ++i)
	{
		WorldBossGuildRewardTable::RowData* data = m_guildreward.Table[i];
		if (rank >= data->Rank.seq[0] && rank <= data->Rank.seq[1])
		{
			return data->Reward;
		}
	}
	return 0;
}

void WorldBossMgr::ErrorNotify(UINT64 roleid, KKSG::ErrorCode errorcode)
{
	CRole* role = CRoleManager::Instance()->GetByRoleID(roleid);
	if (NULL != role)
	{
		PtcM2C_MSErrorNotify ptc;
		ptc.m_Data.set_errorno(errorcode);
		role->Send(ptc);
	}
}

void WorldBossMgr::OnRoleLeaveGuild(UINT64 guildid, UINT64 roleid)
{
	if (m_eState == eGoing)
	{
		WorldBossGuildRankList* guildranklist = (WorldBossGuildRankList*)CRankListMgr::Instance()->GetRankList(KKSG::WorldBossGuildRank);	
		if (NULL != guildranklist)
		{
			guildranklist->RemoveRole(guildid, roleid);
		}
	}
}

GuildAttr& WorldBossMgr::GetGuildAttr(UINT64 guildid)
{
	auto i = m_guild2attrcount.find(guildid);
	if (i == m_guild2attrcount.end())
	{
		m_guild2attrcount[guildid] = GuildAttr();
		return m_guild2attrcount[guildid];
	}
	else
	{
		return i->second;
	}
}

void WorldBossMgr::DoTxLog(UINT32 rank, UINT64 roleid, float damge, CRoleSummary* data)
{
	TWorldBossFlow oLog(TLogMgr::Instance()->GetTagString());
	oLog.m_uRoleID = roleid;
	oLog.m_Rank = rank;
	oLog.m_Damge = damge;
	oLog.SetData(data);
	oLog.Do();
}
