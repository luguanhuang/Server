#include "pch.h"
#include "time.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "guildboss.h"
#include "notice/notice.h"
#include "notice/noticemgr.h"
#include "../share/util/XCommon.h"
#include "table/MultActivityMgr.h"
#include "network/gslink.h"
#include "commondef.h"
#include "scene/ptcm2g_noticeguildbossendtogsn.h"
#include "guild/rpcm2g_noticeguildbossdoendtogsn.h"
#include "scene/scenemanager.h"
#include "common/ptcm2c_hintnotifyms.h"
#include "table/globalconfig.h"
#include "mail/mailmgr.h"
#include "ranklist/ranklistmgr.h"
#include "guild/guildmgr.h"
#include "role/rolemanager.h"
#include "scene/sceneswitch.h"
#include "guildauct/guildauctmgr.h"
#include "../share/util/gametime.h" 
#include "tablemgr/XEntityInfoLibrary.h"
#include "table/guildauctconfig.h"
#include "guild/ptcm2g_synguildbosshp.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "guild/ptcm2g_synguildbossdie.h"
#include "guild/ptcm2g_synguildbossdoend.h"
#include "guild/ptcm2g_noticeguildbossattr.h"
#include "event/eventmgr.h"
#include "define/eventdef.h"
#include "common/ptcm2c_mserrornotify.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "foreach.h"
#include "util/gametime.h"
#include "loghelper/tlogr.h"

INSTANCE_SINGLETON(CGuildBossListener);
INSTANCE_SINGLETON(CGuildBossMgr);

void CGuildBossListener::OnCreateReply(CScene* scene, const KKSG::CreateBattleParam& roParam, const KKSG::CreateBattleRes& res)
{
	if (scene == NULL)
	{
		SSWarn << "guild boss create scene failed and guildid : " << roParam.guildid() << END; 
		return ;
	}
	UINT32 dwSceneInstanceID = NULL == scene ? INVALID_SCENE_ID : scene->GetID();
	CGuildBossMgr::Instance()->AddGuildScene(roParam.guildid(), dwSceneInstanceID);
	CGuildBossMgr::Instance()->PushRoleToScene(roParam.guildid(), scene->GetID(), scene->GetLine());
	SSInfo << "guild boss create scene sceneid : " << scene->GetID() << " guildid : " << roParam.guildid() << END; 
}

bool CGuildBossListener::Init()
{
	return true;
}

void CGuildBossListener::Uninit()
{
}


///////////////////////////
CGuildBossMgr::CGuildBossMgr()
{
	m_state = BeginPre;
}
CGuildBossMgr::~CGuildBossMgr()
{

}
bool CGuildBossMgr::Init()
{
	if (!m_bossconf.LoadFile("table/GuildBossConfig.txt"))
	{
		SSWarn<<"load file table/GuildBossConfig.txt failed"<<END;
		return false;
	}

	if (!m_guildreward.LoadFile("table/GuildBossReward.txt"))	
	{
		SSWarn<<"load file table/GuildBossReward.txt failed"<<END;
		return false;
	}

	m_ranklist = (GuildBossRankList*)CRankListMgr::Instance()->GetRankList(KKSG::GuildBossRank);
	if (m_ranklist == NULL)
	{
		return false;
	}

	m_timehandler = CTimerMgr::Instance()->SetTimer(this, 0, 1000, -1, __FILE__, __LINE__);
	return true;
}

void CGuildBossMgr::Uninit()
{
	CTimerMgr::Instance()->KillTimer(m_timehandler);
}

void CGuildBossMgr::ClearFile()
{
	m_bossconf.Clear();
}

bool CGuildBossMgr::LoadFile()
{
	if (!CheckFile())
	{
		return false;
	}

	ClearFile();

	if (!m_bossconf.LoadFile("table/GuildBossConfig.txt"))
	{
		SSWarn<<"load file table/GuildBossConfig.txt failed"<<END;
		return false;
	}
	return true;
}

bool CGuildBossMgr::CheckFile()
{
	GuildBossConfigTable tempbossconf;
	if (!tempbossconf.LoadFile("table/GuildBossConfig.txt"))
	{
		SSWarn<<"load file table/GuildBossConfig.txt failed"<<END;
		return false;
	}
	return true;
}

void CGuildBossMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	UINT32 weekNum = XCommon::GetWeekNum(GameTime::GetTime());
	if (!MultActivityMgr::Instance()->IsActivityOnDay(MULT_ACTIVITY_GUILD_BOSS, weekNum))
	{
		return ;
	}

	switch(m_state)
	{
	case (EGuildBossSceneState::BeginPre):
		{
			if (_IsOnTime(BeginPre))
			{
				_PrepareNotice();  // 开始前的公告
			}

			if (_IsOnTime(Begin))
			{
				m_state = Begin;
				SSInfo << "guild boss begin !" << END;
			}
		}
		break;
	case (EGuildBossSceneState::Begin):
		{
			_DoClearBeforeStart();    // 清理
			_NoticeGuildBossBegin();  // icon
			m_state = WaitEnd;
			SSInfo << "guild boss wait end !" << END;
		}
		break;
	case (EGuildBossSceneState::WaitEnd):
		{
			if (_IsOnTime(End))
			{
				_NoticeEndToGSN();
				m_state = End;
				SSInfo << " guild boss end !" << END;
			}
		}
		break;
	case (EGuildBossSceneState::End):
		{
			if (GameTime::GetTime() >= MultActivityMgr::Instance()->GetEndTime(MULT_ACTIVITY_GUILD_BOSS) + END_DELAY_TIME)
			{
				_NoticeDoEndToGSN();
				m_state = BeginPre;
				SSInfo << " guild boss do end !" << END;
			}
		}
		break;
	default:
		break;
	}	
}

bool CGuildBossMgr::_IsOnTime(EGuildBossSceneState state)
{
	time_t nowTime = GameTime::GetTime();
	time_t beginTime = MultActivityMgr::Instance()->GetOpenTime(MULT_ACTIVITY_GUILD_BOSS);
	time_t endTime = MultActivityMgr::Instance()->GetEndTime(MULT_ACTIVITY_GUILD_BOSS);

	switch (state)
	{
	case (EGuildBossSceneState::BeginPre):
		{
			if (nowTime < beginTime)
			{
				return true;
			}
		}
		break;
	case (EGuildBossSceneState::Begin):
		{
			if (nowTime >= beginTime && nowTime < endTime)
			{
				return true;
			}
		}
		break;
	case (EGuildBossSceneState::End):
		{
			if (nowTime >= endTime)
			{
				return true;
			}
		}
		break;
	default:
		{
			return false;
		}
		break;
	}

	return false;
}

void CGuildBossMgr::_PrepareNotice()
{
	time_t nowTime = GameTime::GetTime();
	time_t beginTime = MultActivityMgr::Instance()->GetOpenTime(MULT_ACTIVITY_GUILD_BOSS);
	if (nowTime >= beginTime)
	{
		return ;
	}

	UINT32 sec = (UINT32)(beginTime - nowTime);
	UINT32 min = (UINT32)(beginTime - nowTime) / 60;
	if (sec == 60 || sec == 120 || sec == 300 || sec == 600)
	{
		const std::unordered_map<UINT64, Guild*>& guildMap = CGuildMgr::Instance()->GetGuilds();
		foreach (i in guildMap)
		{
			if (MultActivityMgr::Instance()->IsGuildLevelOpen(MULT_ACTIVITY_GUILD_BOSS, i->second->GetLevel()))
			{
				_NoticeGuildBossStart(i->first, min);
			}
		}
	}
}

void CGuildBossMgr::DoTxLog(UINT64 guildId)
{
	auto it = m_guildDamage.find(guildId);
	if (it != m_guildDamage.end())
	{
		for (auto i = it->second.begin(); i != it->second.end(); i ++)
		{
			//记录tlog数据;
			std::string szTransTag = TLogMgr::Instance()->GetTagString();
			TGuildBossFlow oLog(szTransTag);
			oLog.m_uRoleID = i->roleid;
			oLog.m_uGuildID = guildId;
			oLog.m_Damage = UINT64(i->damage);
			oLog.Do();
		}
	}
}

void CGuildBossMgr::_NoticeEndToGSN()
{
	foreach (i in m_guildSceneMap)
	{
		if (m_finishGuild.find(i->first) == m_finishGuild.end())
		{
			_UpdateGuildRank(i->first);
		}
		DoTxLog(i->first);
	}

	std::set<UINT32> lineSet;
	_GetLine(lineSet);
	foreach (i in lineSet)
	{
		PtcM2G_noticeguildbossendtogsn msg;
		GSLink::Instance()->SendToLine(*i, msg);
	}

	const std::unordered_map<UINT64, Guild*>& allGuildMap = CGuildMgr::Instance()->GetGuilds();
	foreach (i in allGuildMap)
	{
		CGuildBossMgr::Instance()->HintNotify(i->second, true);
	}

	GetRankList()->SaveToDb();
	GiveLeaderReward();
	GiveAuctReward();
}

void CGuildBossMgr::_NoticeDoEndToGSN()
{

	std::set<UINT32> lineSet;
	_GetLine(lineSet);
	foreach (i in lineSet)
	{
		PtcM2G_SynGuildBossDoEnd msg;
		GSLink::Instance()->SendToLine(*i, msg);
	}
}

void CGuildBossMgr::_GetLine(std::set<UINT32>& lineSet)
{
	foreach (i in m_guildSceneMap)
	{
		foreach (j in i->second)
		{
			CScene* pScene = CSceneManager::Instance()->GetScene(j->first);
			if (pScene != NULL)
			{
				lineSet.insert(pScene->GetLine());
			}
		}
	}
}

void CGuildBossMgr::_GetGuildLine(std::set<UINT32>& lineSet, UINT64 guildId)
{
	auto iter = m_guildSceneMap.find(guildId);
	if (iter != m_guildSceneMap.end())
	{
		foreach (i in iter->second)
		{
			CScene* pScene = CSceneManager::Instance()->GetScene(i->first);
			if (pScene != NULL)
			{
				lineSet.insert(pScene->GetLine());
			}
		}
	}
}

void CGuildBossMgr::_NoticeGuildBossStart(UINT64 guildId, UINT32 min)
{
	switch (min)
	{
	case (1):
		{
			Notice notice(GuildBossLeft_1Min);
			notice.SendGuild(guildId);
		}
		break;
	case (2):
		{
			Notice notice(GuildBossLeft_2Min);
			notice.SendGuild(guildId);
		}
		break;
	case (5):
		{
			Notice notice(GuildBossLeft_5Min);
			notice.SendGuild(guildId);
		}
		break;
	case (10):
		{
			Notice notice(GuildBossLeft_10Min);
			notice.SendGuild(guildId);
		}
		break;
	}
}

UINT32 CGuildBossMgr::GetSceneId(UINT64 guildId, UINT64 roleId)
{
	foreach (i in m_sceneRole)
	{
		if (i->second.find(roleId) != i->second.end())
		{
			return i->first;
		}
	}

	auto i = m_guildSceneMap.find(guildId);
	if (i != m_guildSceneMap.end())
	{
		foreach (j in i->second)
		{
			if (j->second < GetGlobalConfig().GuildBossScenePlayer)
			{
				return j->first;
			}
		}
	}
	return 0;
}

void CGuildBossMgr::AddGuildScene(UINT64 guildId, UINT32 sceneId) 
{ 
	std::map<UINT32, UINT32>& sceneMap = m_guildSceneMap[guildId];
	sceneMap[sceneId] = 0;
}

void CGuildBossMgr::HintNotify(Guild* pGuild, bool isMove)
{
	if (pGuild != NULL && MultActivityMgr::Instance()->IsGuildBossOpen(pGuild->GetLevel()))
	{
		PtcM2C_HintNotifyMS msg;
		msg.m_Data.add_systemid(GUILD_BOSS_SYS_ID);
		msg.m_Data.set_isremove(isMove);

		pGuild->BroadCast(msg);
	}
}

void CGuildBossMgr::_CreateGuildBossScene(UINT64 roleId)
{
	Guild* pGuild = CGuildMgr::Instance()->GetGuildByRoleId(roleId);
	if (pGuild != NULL && MultActivityMgr::Instance()->IsGuildBossOpen(pGuild->GetLevel()))
	{
		KKSG::CreateBattleParam param;
		param.set_guildid(pGuild->GetID());
		param.set_guildname(pGuild->GetName());
		KKSG::guildbossinfo* pGuildBossInfo = param.mutable_guildbossinfo();
		pGuildBossInfo->set_bossindex(CGuildBossMgr::Instance()->getNextBossIndex(pGuild));
		pGuildBossInfo->set_curbossindex(pGuild->GetBossIndex());
		pGuildBossInfo->set_bosshp(_GetGuildBossHp(pGuild->GetID()));
		pGuildBossInfo->set_count(m_guildid2attrcount[pGuild->GetID()]);
		CSceneCreator::Instance()->CreateBattleScene(CGuildBossMgr::GUILD_BOSS_SCENE_ID, CGuildBossListener::Instance(), param); // 依次创建场
	}

	if (m_guildSceneMap.find(pGuild->GetID()) == m_guildSceneMap.end())
	{
		m_beginTime[pGuild->GetID()] = GameTime::GetTime();
	}

}

UINT32 CGuildBossMgr::getNextBossIndex(Guild* guild)
{
	UINT32 curIndex = guild->GetBossIndex(); 
	if (curIndex < m_bossconf.Table.size())
	{
		GuildBossConfigTable::RowData* pRowData = m_bossconf.Table[curIndex];
		if (pRowData != NULL)
		{
			if (pRowData->GuildLevel <= guild->GetLevel())
			{
				return pRowData->BossID;
			}
		}
	}

	return curIndex;
}


UINT32 CGuildBossMgr::getNeedGuildLvl(Guild* guild)
{
	UINT32 nextIndex = getNextBossIndex(guild);
	if (nextIndex >= m_bossconf.Table.size())
	{
		return 0;
	}

	GuildBossConfigTable::RowData* pRowData = m_bossconf.Table[nextIndex];
	if (pRowData != NULL && pRowData->GuildLevel > guild->GetLevel())
	{
		return pRowData->GuildLevel;
	}

	return 0;
}


UINT32 CGuildBossMgr::getNeedBoosIndex(Guild* guild)
{
	UINT32 nextIndex = getNextBossIndex(guild);
	if (nextIndex >= m_bossconf.Table.size())
	{
		return 0;
	}

	if (nextIndex > guild->GetBossIndex())
	{
		return nextIndex;
	}

	return 0;
}

std::map<UINT64, std::set<GuildBossRoleDamage> >& CGuildBossMgr::GetGuildDamage()
{
	return m_guildDamage;
}

void CGuildBossMgr::_GetGuildReward(UINT32 rank, std::vector<ItemDesc>& itemVec, bool isChief/*true会长 false副会长*/, std::map<UINT32, UINT32>& guildReward)
{
	GuildBossRewardTable::RowData* rowdata = m_guildreward.GetByrank(rank);
	if (NULL != rowdata)
	{
		if (isChief)
		{
			foreach(i in rowdata->reward)
			{
				ItemDesc item;
				item.itemID = i->seq[0];
				item.itemCount = i->seq[1];
				itemVec.push_back(item);
			}
			foreach (j in rowdata->guildreward)
			{
				guildReward[j->seq[0]] = j->seq[1];
			}
		}
		else
		{
			foreach(i in rowdata->rewardMin)
			{
				ItemDesc item;
				item.itemID = i->seq[0];
				item.itemCount = i->seq[1];
				itemVec.push_back(item);
			}
		}
	}
}

void CGuildBossMgr::GiveLeaderReward()
{
	if (m_ranklist == NULL)
	{
		return ;
	}

	std::vector<GuildBossRankInfo*> rankinfos;
	m_ranklist->GetRankInfo(rankinfos);
	for(UINT32 i = 0; i < rankinfos.size(); i++)
	{
		GuildBossRankInfo* rankinfo = rankinfos[i];
		Guild* pGuild = CGuildMgr::Instance()->GetGuild(rankinfo->GetUniqueId());
		if (pGuild != NULL)
		{
			UINT32 rank = i + 1;
			if (rank == 1)
			{
				NoticeMgr::Instance()->NoticeGuildBossRank(pGuild->GetName(), 1);
			}
			else if (rank == 2)
			{
				NoticeMgr::Instance()->NoticeGuildBossRank(pGuild->GetName(), 2);
			}
			else if (rank == 3)
			{
				NoticeMgr::Instance()->NoticeGuildBossRank(pGuild->GetName(), 3);
			}
			std::vector<ItemDesc> itemVec;
			std::map<UINT32, UINT32> guildReward;
			_GetGuildReward(rank, itemVec, true, guildReward);

			// 发放会长奖励
			UINT64 leader = pGuild->GetLeaderID();
			if (!itemVec.empty())
			{
				MailData mail( GetGlobalConfig().GuildBossPresidentMailTitle, GetGlobalConfig().GuildBossPresidentMailContent, itemVec, MAIL_EXPIRE_TIME);
				CMailMgr::Instance()->SendMail(leader, mail);
				SSInfo << "Send Mail To Client, MailType = GuildBossFinishMailTitle, " << "RoleID=" << leader << END; 
			}
			

			// 添加工会威望
			pGuild->AddGuildReward(guildReward);

			// 发放官员奖励
			itemVec.clear();
			_GetGuildReward(rank, itemVec, false, guildReward);
			if (!itemVec.empty())
			{
				std::vector<UINT64> rulerVec;
				pGuild->FindRuler(rulerVec);
				foreach (k in rulerVec)
				{
					MailData mail( GetGlobalConfig().GuildBossOfficialMailTitle, GetGlobalConfig().GuildBossOfficialMailContent, itemVec, MAIL_EXPIRE_TIME);
					mail.SetReason(ItemFlow_GuildBossAward);
					CMailMgr::Instance()->SendMail(*k, mail);
					SSInfo << "Send Mail To Client, MailType = GuildBossFinishMailTitle, " << "RoleID=" << *k << END; 
				}
			}
		}
	}
}

bool CGuildBossMgr::IsSceneOpen(UINT64 guildId)
{
	Guild* pGuild = CGuildMgr::Instance()->GetGuild(guildId);
	if (pGuild == NULL)
	{
		return false;
	}

	if (MultActivityMgr::Instance()->IsGuildBossOpen(pGuild->GetLevel()) && m_state == WaitEnd)
	{
		return true;
	}

	return false;
}

bool CGuildBossMgr::CanEnterScene(UINT64 guildId)
{
	if (IsSceneOpen(guildId) && !IsFinish(guildId))
	{
		return true;
	}
	return false;
}

UINT32 CGuildBossMgr::GetBeginTimeLeft()
{
	time_t nowtime = GameTime::GetTime();
	time_t begintime = MultActivityMgr::Instance()->GetGuilBossOpenTime();
	time_t endtime = MultActivityMgr::Instance()->GetGuilBossEndTime();
	if (begintime > nowtime)
	{
		return (UINT32)(begintime - nowtime);
	}
	else
	{
		return (UINT32)(begintime + 24*3600 - nowtime);
	}
}

UINT32 CGuildBossMgr::GetEndTimeLeft()
{
	time_t nowtime = GameTime::GetTime();
	time_t endTime = MultActivityMgr::Instance()->GetEndTime(MULT_ACTIVITY_GUILD_BOSS);
	return (UINT32)(nowtime < endTime ? (endTime - nowtime) : 0);
}

UINT32 CGuildBossMgr::GetAttrCount(UINT64 guildId)
{
	auto i = m_guildid2attrcount.find(guildId);
	if (i == m_guildid2attrcount.end())
	{
		return 0;
	}
	return i->second;
}

void CGuildBossMgr::DelGuild(UINT64 guildId) 
{ 
	m_guildSceneMap.erase(guildId); 
}

void CGuildBossMgr::DoGuildOpenGuildBoss(UINT64 guildId)
{
	if (m_state != WaitEnd)
	{
		return ;
	}

	Guild* pGuild = CGuildMgr::Instance()->GetGuild(guildId);
	if (pGuild == NULL)
	{
		return ;
	}

	if (MultActivityMgr::Instance()->GetGuildBossOpenLvl() != pGuild->GetLevel())
	{
		return ;
	}

	CGuildBossMgr::Instance()->HintNotify(pGuild, false);
}

void CGuildBossMgr::CheckRoleOnline(UINT64 roleId)
{
	Guild* pGuild = CGuildMgr::Instance()->GetGuildByRoleId(roleId);
	if (pGuild == NULL)
	{
		return ;
	}

	if (!MultActivityMgr::Instance()->IsGuildBossOpen(pGuild->GetLevel()))
	{
		return ;
	}
	
	if (m_state == WaitEnd && m_finishGuild.find(pGuild->GetID()) == m_finishGuild.end())
	{
		CRole* pRole = CRoleManager::Instance()->GetByRoleID(roleId);
		if (pRole != NULL)
		{
			PtcM2C_HintNotifyMS msg;
			msg.m_Data.add_systemid(GUILD_BOSS_SYS_ID);
			msg.m_Data.set_isremove(false);
			pRole->Send(msg);
		}
	}
}

void CGuildBossMgr::RemoveDamageRank(UINT64 roleId)
{
	UINT64 guildId = CGuildMgr::Instance()->GetGuildIdByRoleId(roleId);
	auto iter = m_guildDamage.find(guildId);
	if (iter != m_guildDamage.end())
	{
		for (auto i = iter->second.begin(); i != iter->second.end(); ++i)
		{
			if (i->roleid == roleId)
			{
				iter->second.erase(i);
				return ;
			}
		}
	}
}

void CGuildBossMgr::EnterScene(CRole* role, UINT32 sceneid, UINT32 gsline)
{
	KKSG::SceneSwitchData data;
	CSceneSwitch::EnterScene(role, sceneid, data);

	LogInfo("Role [%llu] begin to change gs_%u to gs_%u", role->GetID(), role->GetGsLine(), gsline);
}

void CGuildBossMgr::PushRoleToScene(UINT64 guildId, UINT32 sceneid, UINT32 gsline)
{
	auto iter = m_waitRoleMap.find(guildId);
	if (iter != m_waitRoleMap.end())
	{
		foreach (i in iter->second)
		{
			CRole* role = CRoleManager::Instance()->GetByRoleID(*i);
			if (NULL != role)
			{
				EnterScene(role, sceneid, gsline);
				SSInfo << "guild boss push role to scene roleid : " << *i << "guildid : " << guildId << END;
			}
			else
			{
				SSInfo << "guild boss push role to scene role is null roleid : " << *i << "guildid : " << guildId << END;
			}
			AddGuildBossSceneNum(sceneid, *i);
		}
		m_waitRoleMap.erase(iter);
	}
}

void CGuildBossMgr::RoleEnterDelay(UINT64 roleid)
{
	UINT64 guildId = CGuildMgr::Instance()->GetGuildIdByRoleId(roleid);
	if (guildId == 0)
	{
		return ;
	}

	std::set<UINT64>& roleSet = m_waitRoleMap[guildId];
	if (roleSet.find(roleid) != roleSet.end())
	{
		return ;
	}

	if (!roleSet.empty())
	{
		roleSet.insert(roleid);
		return ;
	}

	_CreateGuildBossScene(roleid);
	roleSet.insert(roleid);
}

void CGuildBossMgr::_NoticeGuildBossBegin()
{
	const std::unordered_map<UINT64, Guild*>& guildMap = CGuildMgr::Instance()->GetGuilds();
	foreach (i in guildMap)
	{
		if (i->second != NULL && MultActivityMgr::Instance()->IsGuildBossOpen(i->second->GetLevel()))
		{
			CGuildBossMgr::Instance()->HintNotify(i->second, false);
		}
	}

}

bool CGuildBossMgr::IsFinish(UINT64 guildId)
{
	return m_finishGuild.find(guildId) != m_finishGuild.end();
}

void CGuildBossMgr::AddGuildBossSceneNum(UINT32 sceneId, UINT64 roleId)
{
	bool isIn = false;
	auto iter = m_sceneRole.find(sceneId);
	if (iter != m_sceneRole.end() && iter->second.find(roleId) != iter->second.end())
	{
		isIn = true;
	}

	if (!isIn)
	{
		foreach (i in m_guildSceneMap)
		{
			foreach (j in i->second)
			{
				if (j->first == sceneId)
				{
					j->second += 1;
				}
			}
		}
	}
	
	std::map<UINT64, UINT64>& roleMap = m_sceneRole[sceneId];
	roleMap[roleId] = CGuildMgr::Instance()->GetGuildIdByRoleId(roleId);
}

void CGuildBossMgr::GiveAuctReward()
{
	if (m_ranklist == NULL)
	{
		return ;
	}

	std::vector<GuildBossRankInfo*> rankinfos;
	m_ranklist->GetRankInfo(rankinfos);
	std::map<UINT64, std::set<GuildBossRoleDamage> >& damageMap = CGuildBossMgr::Instance()->GetGuildDamage();

	int RankCount = GuildAuctConfig::Instance()->GetRankCount(GA_ACT_GUILD_BOSS);
	UINT32 rank = 0;
	foreach (i in rankinfos)
	{
		++rank;
		if(rank > RankCount)
		{
			break;
		}
		std::set<GuildBossRoleDamage>& damageSet = damageMap[(*i)->GetUniqueId()];
		std::vector<UINT64> roleVec;
		foreach (j in damageSet)
		{
			roleVec.push_back(j->roleid);

			EventMgr::Instance()->AddEventOnlyOnMS(j->roleid, MENTOR_EVENT_OFFSET + KKSG::MentorTask_GuildBossCount, 0, 1);
		}

		GuildAuctMgr::Instance()->AddGuildAuctData((*i)->GetUniqueId(), GA_ACT_GUILD_BOSS, rank, roleVec);
	}
}

double CGuildBossMgr::_GetGuildBossHp(UINT64 guildId)
{
	Guild* pGuild = CGuildMgr::Instance()->GetGuild(guildId);
	if (pGuild == NULL)
	{
		SSWarn << " guild not exit guildid : " << guildId << END;
		return 0;
	}

	auto iter = m_guildBossHp.find(guildId);
	if (iter != m_guildBossHp.end())
	{
		return iter->second;
	}
	else
	{
		UINT32 bossIndex = CGuildBossMgr::Instance()->getNextBossIndex(pGuild);
		if (bossIndex == 0 || bossIndex > m_bossconf.Table.size())
		{
			SSWarn << "can not add guild boss bossindex is wrong : " << bossIndex << END;
			return 0;
		}
		GuildBossConfigTable::RowData* pRowData = m_bossconf.Table[bossIndex - 1];
		if (pRowData == NULL)
		{
			SSWarn << "can not add guild boss bossindex is wrong : " << bossIndex << END;
			return 0;
		}

		const XEntityStatistics::RowData* pEnemy= XEntityInfoLibrary::Instance()->TemplateID2Data_Enemy(pRowData->EnemyID);
		m_guildBossHp[guildId] = (pEnemy->MaxHP) * pRowData->LifePercent;
		return m_guildBossHp[guildId];
	}

}

void CGuildBossMgr::DelGuildBossHp(UINT64 guildId, double delHp, UINT32 sceneid, UINT64 roleId)
{
	if (m_state != WaitEnd)
	{
		SSWarn << " guild boss state is not waitend " << END;
		return;
	}

	if (delHp == 0)
	{
		SSInfo << " guild boss hurt is 0 " << END;
		return;
	}

	auto iter = m_guildBossHp.find(guildId);
	if (iter == m_guildBossHp.end())
	{
		SSWarn << "guild boss hp is null" << END;
		return ;
	}

	if (iter->second == 0) // 已经击杀
	{
		return ;
	}

	iter->second = iter->second > delHp ? (iter->second - delHp) : 0;
	LogInfo("--DelGuildBossHp--guildid=%llu roleid=%llu delHp=%lf leftHp=%lf", guildId,roleId,delHp,iter->second);
	
	// 同步血量
	std::set<UINT32> lineSet;
	_GetGuildLine(lineSet, guildId);
	PtcM2G_SynGuildBossHp msg;
	msg.m_Data.set_sceneid(sceneid);
	msg.m_Data.set_delhp(delHp);
	msg.m_Data.set_guildid(guildId);
	foreach (i in lineSet)
	{
		GSLink::Instance()->SendToLine(*i, msg);
	}

	// 击杀处理
	if (iter->second == 0)
	{
		m_finishGuild.insert(guildId);
		m_lastHurt[guildId] = roleId; // 记录补刀玩家id
		_DoGuildBossEnd(guildId);
	}
}


void CGuildBossMgr::_DoGuildBossEnd(UINT64 guildId)
{
	LogInfo("guildboss dead guildid=%llu",guildId);
	Guild* pGuild = CGuildMgr::Instance()->GetGuild(guildId); 
	if (pGuild != NULL)
	{
		if (getNextBossIndex(pGuild) > pGuild->GetBossIndex())
		{
			UINT32 bossIndex = CGuildBossMgr::Instance()->getNextBossIndex(pGuild);
			if (bossIndex == 0 || bossIndex > m_bossconf.Table.size())
			{
				SSWarn << "can not add guild boss bossindex is wrong : " << bossIndex << END;
				return ;
			}
			GuildBossConfigTable::RowData* pRowData = m_bossconf.Table[bossIndex - 1];
			if (pRowData == NULL)
			{
				SSWarn << "can not add guild boss bossindex is wrong : " << bossIndex << END;
				return ;
			}

			const XEntityStatistics::RowData* pEnemy= XEntityInfoLibrary::Instance()->TemplateID2Data_Enemy(pRowData->EnemyID);
			NoticeMgr::Instance()->NoticeGuildBossFirstKill(pGuild->GetName(), pEnemy->Name);
		}

		CGuildBossMgr::Instance()->AddFinishGuild(guildId);
		CGuildBossMgr::Instance()->HintNotify(pGuild, true); // 取消强弹通知
		NoticeMgr::Instance()->NoticeGuildBossLvlup(guildId);

		UINT64 dpsId = _GetDpsRoleId(guildId); 
		pGuild->RecordBossDps(dpsId);

		if (dpsId != 0) // 最强输出公告
		{
			CRoleSummary* pSum = CRoleSummaryMgr::Instance()->GetRoleSummary(dpsId);
			if (pSum != NULL)
			{
				NoticeMgr::Instance()->NoticeGuildBossDps(pSum->GetName(), guildId);
			}
		}

		// 击杀通知
		std::set<UINT32> lineSet;
		_GetGuildLine(lineSet, guildId);
		foreach (i in lineSet)
		{
			PtcM2G_SynGuildBossDie msg;
			msg.m_Data.set_guildid(guildId);
			GSLink::Instance()->SendToLine(*i, msg);
		}

		auto iter = m_lastHurt.find(guildId); // 发补刀奖
		if (iter != m_lastHurt.end())
		{
			_GiveLastHurtReward(iter->second);
		}

		_UpdateGuildRank(guildId);
		pGuild->SetBossIndex(CGuildBossMgr::Instance()->getNextBossIndex(pGuild));  // 更新boss序列号

		
	}
}

void CGuildBossMgr::_GiveLastHurtReward(UINT64 roleId)
{
	Guild* pGuild = CGuildMgr::Instance()->GetGuildByRoleId(roleId);
	if (pGuild == NULL)
	{
		return ;
	}

	UINT32 bossIndex = CGuildBossMgr::Instance()->getNextBossIndex(pGuild);
	if (bossIndex >= m_bossconf.Table.size())
	{
		return ;
	}

	std::vector<ItemDesc> itemVecKill;
	GuildBossConfigTable::RowData* pRowData = m_bossconf.Table[bossIndex];
	foreach (i in pRowData->KillReward)
	{
		ItemDesc item;

		item.itemID = i->seq[0];
		item.itemCount = i->seq[1];

		itemVecKill.push_back(item);
	}

	MailData maildata(GetGlobalConfig().GuildBossFinishMailTitle, GetGlobalConfig().GuildBossFinishMailContent, itemVecKill, MAIL_EXPIRE_TIME);
	maildata.SetReason(ItemFlow_GuildBossAward);
	CMailMgr::Instance()->SendMail(roleId, maildata);
}

UINT64 CGuildBossMgr::_GetDpsRoleId(UINT64 guildId)
{
	auto iter = m_guildDamage.find(guildId);
	if (iter != m_guildDamage.end() && !iter->second.empty())
	{
		return iter->second.begin()->roleid;
	}
	return 0;
}

void CGuildBossMgr::_UpdateGuildRank(UINT64 guildId, UINT32 nowTime)
{
	if (nowTime == 0)
	{
		nowTime = GameTime::GetTime();
	}

	Guild* pGuild = CGuildMgr::Instance()->GetGuild(guildId);
	if (pGuild == NULL)
	{
		SSWarn << " guild not exit guildid : " << guildId << END;
		return ;
	}

	UINT32 useTime = nowTime > m_beginTime[guildId] ? (nowTime - m_beginTime[guildId]) : 0;
	UINT32 bossIndex = CGuildBossMgr::Instance()->getNextBossIndex(pGuild);

	if (bossIndex == 0 || bossIndex > m_bossconf.Table.size())
	{
		SSWarn << "can not add guild boss bossindex is wrong : " << bossIndex << END;
		return ;
	}
	GuildBossConfigTable::RowData* pRowData = m_bossconf.Table[bossIndex - 1];
	if (pRowData == NULL)
	{
		SSWarn << "can not add guild boss bossindex is wrong : " << bossIndex << END;
		return ;
	}
	const XEntityStatistics::RowData* pEnemy= XEntityInfoLibrary::Instance()->TemplateID2Data_Enemy(pRowData->EnemyID);
	double damage = (pEnemy->MaxHP) * pRowData->LifePercent - m_guildBossHp[guildId];

	std::string dpsName;
	auto iter = m_guildDamage.find(guildId);
	if (iter != m_guildDamage.end() && !iter->second.empty())
	{
		CRoleSummary* pRoleSummary = CRoleSummaryMgr::Instance()->GetRoleSummary(iter->second.begin()->roleid);
		if (pRoleSummary != NULL)
		{
			dpsName = pRoleSummary->GetName();
		}
	}

	std::string bossName = pEnemy->Name;
	if (bossIndex > 0 && bossIndex <= m_bossconf.Table.size())
	{
		bossName += m_bossconf.Table[bossIndex - 1]->BossName;
	}

	CGuildBossMgr::Instance()->GetRankList()->Update(guildId, bossIndex, useTime, damage, dpsName, bossName);
}

void CGuildBossMgr::AddAttrCount(UINT64 guildId, UINT32 sceneId)
{
	++m_guildid2attrcount[guildId];

	PtcM2G_NoticeGuildBossAttr msg;
	msg.m_Data.set_guildid(guildId);
	msg.m_Data.set_sceneid(sceneId);
	msg.m_Data.set_count(m_guildid2attrcount[guildId]);

	std::set<UINT32> lineSet;
	_GetGuildLine(lineSet, guildId);
	foreach (i in lineSet)
	{
		GSLink::Instance()->SendToLine(*i, msg);
	}
}

bool CGuildBossMgr::IsJoinOtherBefoer(UINT64 roleId)
{
	UINT64 guildId = CGuildMgr::Instance()->GetGuildIdByRoleId(roleId);
	CRole* pRole = CRoleManager::Instance()->GetByRoleID(roleId);
	if (pRole == NULL)
	{
		return true;
	}

	foreach (i in m_sceneRole)
	{
		auto iter = i->second.find(roleId);
		if (iter != i->second.end() && iter->second != guildId)
		{
			PtcM2C_MSErrorNotify msg;
			msg.m_Data.set_errorno(KKSG::ERR_HAS_JOIN_OTHER_GUILD_BOSS);
			pRole->Send(msg);
			return true;
		}
	}

	return false;
}

void CGuildBossMgr::OnGsClose(UINT32 nSceneID)
{
	for (auto iter = m_guildSceneMap.begin(); iter!=m_guildSceneMap.end();iter++)
	{
		auto &refMap =  iter->second;
		auto siter = refMap.find(nSceneID);
		if (siter!=refMap.end())
		{
			LogInfo("game onclose del sceneid=%d players=%d",nSceneID,siter->second);
			refMap.erase(siter);
			break;
		}
	}
	auto iter = m_sceneRole.find(nSceneID);
	if (iter!=m_sceneRole.end())
	{
		m_sceneRole.erase(iter);
	}
}
