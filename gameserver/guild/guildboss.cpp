#include "pch.h"
#include "guildboss.h"
#include "util/XCommon.h"
#include "time.h"
#include "guild/guildmgr.h"
#include "guild/guild.h"
#include "unit/enemymanager.h"
#include "scene/scene.h"
#include "tablemgr/XEntityInfoLibrary.h"
#include "unit/role.h"
#include "table/PowerPointMgr.h"
#include "gamelogic/item.h"
#include "gamelogic/noticemgr.h"
#include "guild/guildconfig.h"
#include "gamelogic/rewardmgr.h"
#include "common/ptcg2c_hintnotify.h"
#include "define/systemiddef.h"
#include "gamelogic/rolesummarymgr.h"
#include "gamelogic/rolesummary.h"
#include "guild/ptcg2c_noticeguildbossend.h"
#include "guild/ptcg2c_guildbosstimeout.h"
#include "mail/mailop.h"
#include "define/maildef.h"
#include "gamelogic/designationMgr.h"
#include "gamelogic/noticemgr.h"
#include "mail/mailsender.h"
#include "scene/scenemanager.h"
#include "guild/ptcg2m_noticeguildbossdie.h"
#include "guild/ptcg2m_synguildbosshurt.h"
#include "network/mslink.h"
#include "config.h"
#include "define/tlogenum.h"
#include "util/gametime.h"
#include "gamelogic/reportdatarecord.h"
#include "gamelogic/payv2Record.h"
#include "gamelogic/taskhelper.h"

#include "foreach.h"

#define WAITEND_TIME 5

bool GuildBossInfo::Init()
{
	m_timehandler = CTimerMgr::Instance()->SetTimer(this, 0, 2000, -1, __FILE__, __LINE__);
	m_dosakaHurt  = 0;
	return true;
}

void GuildBossInfo::Uninit()
{
	CTimerMgr::Instance()->KillTimer(m_timehandler);
}

void GuildBossInfo::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	if (!m_tempDamage.empty())
	{
		PtcG2M_SynGuildBossHurt msg;
		msg.m_Data.set_sceneid(m_sceneId);
		msg.m_Data.set_guildid(m_guildid);
		msg.m_Data.set_hurt(m_dosakaHurt);
		foreach (i in m_tempDamage)
		{
			KKSG::GuildBossRoleHurt* pBossHurt = msg.m_Data.add_rolehurt();
			pBossHurt->set_roleid(i->first);
			pBossHurt->set_hurt(i->second);
		}

		MSLink::Instance()->Send(MSLink::Instance()->GetConnID(), msg);
		m_tempDamage.clear();
		m_dosakaHurt = 0;
	}
}

GuildBossInfo::GuildBossInfo(UINT64 guildid, UINT32 bossIndex, Enemy* enemy, bool isFirstKill, UINT32 sceneId, std::string guildName, double bossHp)
{
	m_boss = enemy;
	m_begintime = (UINT32)GameTime::GetTime();
	m_guildid = guildid;
	m_bossIndex = bossIndex;
	m_isFirstKill = isFirstKill;
	m_sceneId = sceneId;
	m_guildName = guildName;
	m_bossHp = bossHp;
	if (enemy != NULL)
	{
		m_bossName = enemy->GetName();
	}
	Init();
}

GuildBossInfo::~GuildBossInfo()
{
	//  释放公会boss场景
	Uninit();
	Scene* pScene = SceneManager::Instance()->FindBySceneID(m_sceneId);
	if (pScene != NULL)
	{
		pScene->SetDontDestory(false);
	}
}

void GuildBossInfo::SetEnd()
{
	if (IsDead())
	{
		return;
	}
	m_boss->EnableAI(false);
	m_boss->Invincible(true);
	PtcG2C_NoticeGuildBossEnd notice;
	Scene* pScene = SceneManager::Instance()->FindBySceneID(m_sceneId);
	if (pScene != NULL)
	{
		pScene->Broadcast(notice);
	}
}

void GuildBossInfo::RemoveBoss()
{
	if (NULL != m_boss && !m_boss->IsDead())
	{
		//不能调用TriggerDeath，否则会表现异常
		Scene* scene = m_boss->GetCurrScene();
		if (scene == NULL)
		{
			return;
		}
		scene->RemoveUnitAndNotify(m_boss);
		EnemyManager::Instance()->RemoveEnemy(m_boss->GetID());
	}
	m_boss = NULL;
}

void GuildBossInfo::OnHurt(Enemy* enemy, Unit* castunit, double value)
{
	if (enemy == NULL)
	{
		SSWarn << "GuildBossInfo::OnHurt: enemy is NULL" << END;
		return ;
	}

	if (castunit == NULL)
	{
		SSWarn << "GuildBossInfo::OnHurt: castunit is NULL " << value << END;
		m_dosakaHurt += value;
		return ;
	}

	if(!castunit->IsRole())
	{
		SSWarn << "castunit is not role" << castunit << END;
		m_dosakaHurt += value;
		return;
	}

	if (m_roleid2damage.find(castunit->GetID()) == m_roleid2damage.end()) // 第一次攻击
	{
		Role* pRole = (Role*)castunit;
		pRole->OnHitGuildBoss();
		TaskHelper::UpdateTask(castunit->GetID(),KKSG::TaskOper_Add, KKSG::TaskConn_Activity,KKSG::TaskActType_GuildBoss,1);
	}

	m_roleid2damage[castunit->GetID()] += value; 
	m_tempDamage[castunit->GetID()] += value; 
}

void GuildBossInfo::OnDied(Enemy* enemy)
{
	LogInfo("boss died=guild=%llu sceneid=%u",m_guildid,m_sceneId);
	m_boss = NULL;
}

void GuildBossInfo::DoBossDie()
{
	if (NULL != m_boss)
	{
		m_boss->ClearListener();
		m_boss->TriggerDeath();
	}
	m_boss = NULL;
	NoticeBossEnd(); // 通知场景中的玩家  boss被杀死
	_GiveReward();   // 发放奖励
}

void GuildBossInfo::ClearListener()
{
	if (m_boss != NULL)
	{
		m_boss->ClearListener();
	}
}

void GuildBossInfo::_SendKillMsg()
{
	PtcG2M_NoticeGuildBossDie msg;
	msg.m_Data.set_id(m_guildid);
	msg.m_Data.set_bossindex(m_bossIndex);
	msg.m_Data.set_isfirstkill(IsFirstKill());
	foreach (i in m_roleid2damage)
	{
		KKSG::guildBossDamage* pData = msg.m_Data.add_roledamages();
		pData->set_roleid(i->first);
		pData->set_damage(i->second);
	}

	MSLink::Instance()->Send(MSLink::Instance()->GetConnID(), msg);
}

void GuildBossInfo::_GiveReward()
{
	if (!IsDead())
	{
		return ;
	}

	Scene* pScene = SceneManager::Instance()->FindBySceneID(m_sceneId);
	if (pScene == NULL)
	{
		return ;
	}

	// 参与奖
	std::vector<ItemDesc> itemVecJoin;
	GuildBossMgr::Instance()->GetRoleJoinReward(m_guildid, itemVecJoin);
	// 首杀奖
	std::vector<ItemDesc> itemVecFirst;
	GuildBossMgr::Instance()->GetRoleFirstKillReward(m_guildid, itemVecFirst);
	const std::list<Role*>& roleVec = pScene->GetAllRoles();
	foreach (i in roleVec)
	{
		if ((*i) == NULL)
		{
			continue;
		}

		UINT64 roleId = (*i)->GetID(); 
		if (m_roleid2damage.find(roleId) == m_roleid2damage.end()) // 没有伤害输出 不给奖励
		{
			continue;
		}

		// 参与奖
		if (!itemVecJoin.empty())
		{
			MailSender sender;
			sender.SetContent(Mail_System,  GetGlobalConfig().GuildBossJoinMailTitle,  GetGlobalConfig().GuildBossJoinMailContent);
			sender.SetItems(itemVecJoin);
			sender.SetReason(ItemFlow_GuildBossAward);
			sender.Send(roleId);
			SSInfo << "Send Mail To Client, MailType = GuildBossFinishMailTitle, " << "RoleID=" << roleId << END; 
		}

		// 首杀奖励
		if (IsFirstKill() && !itemVecFirst.empty())
		{
			MailSender sender;
			sender.SetContent(Mail_System,  GetGlobalConfig().GuildBossFirstkillMailTitle,  GetGlobalConfig().GuildBossFirstkillMailContent);
			sender.SetItems(itemVecFirst);
			sender.SetReason(ItemFlow_GuildBossAward);
			sender.Send(roleId);
			SSInfo << "Send Mail To Client, MailType = GuildBossFinishMailTitle, " << "RoleID=" << roleId << END; 
		}
	}

	///>首杀称号：                                                                                       
	if (IsFirstKillFinished())                                                        
	{                                                                           
		foreach (  i in m_roleid2damage )
		{                                                          
			UINT64 innerRoleID = i->first;                                       
			UINT32 innerBossIndex = this->GetBossIndex(); 
			DesignationMgr::Instance()->OnFirstTimeKillGuildBoss( innerRoleID, innerBossIndex );
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
INSTANCE_SINGLETON(GuildBossMgr);

GuildBossMgr::GuildBossMgr()
{
	m_pos.set(float( GetGlobalConfig().GuildBossPoint[0]), float( GetGlobalConfig().GuildBossPoint[1])
		, float( GetGlobalConfig().GuildBossPoint[2]));
	m_rotate = float( GetGlobalConfig().GuildBossPoint[3]);
}

GuildBossMgr::~GuildBossMgr()
{
	ClearAfterEnd();
}

bool GuildBossMgr::Init()
{
	if (!m_bossconf.LoadFile("table/GuildBossConfig.txt"))
	{
		SSWarn<<"load file table/GuildBossConfig.txt failed"<<END;
		return false;
	}

	return true;
}

void GuildBossMgr::ClearFile()
{
	m_bossconf.Clear();
}

bool GuildBossMgr::LoadFile()
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

bool GuildBossMgr::CheckFile()
{
	GuildBossConfigTable tempbossconf;
	if (!tempbossconf.LoadFile("table/GuildBossConfig.txt"))
	{
		SSWarn<<"load file table/GuildBossConfig.txt failed"<<END;
		return false;
	}
	return true;
}

time_t GuildBossMgr::GetBeginTime(time_t nowtime)
{
	return MultActivityMgr::Instance()->GetGuilBossOpenTime();
}

time_t GuildBossMgr::GetEndTime(time_t nowtime)
{
	return MultActivityMgr::Instance()->GetGuilBossEndTime(); 
}

UINT32 GuildBossMgr::GetBeginTimeLeft()
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

UINT32 GuildBossMgr::GetEndTimeLeft()
{
	time_t nowtime = GameTime::GetTime();
	return (UINT32)(nowtime < GetEndTime(nowtime) ? (GetEndTime(nowtime) - nowtime) : 0);
}

void GuildBossMgr::AddBoss(Scene* pScene, UINT64 guildId, UINT32 bossIndex, UINT32 curIndex,const std::string &guildName, double bossHp,UINT32 count)
{
	if (bossIndex == 0 || bossIndex > m_bossconf.Table.size())
	{
		SSWarn << "can not add guild boss bossindex is wrong : " << bossIndex <<END;
		return;
	}

	GuildBossConfigTable::RowData* pRowData = m_bossconf.Table[bossIndex - 1];
	if (pRowData == NULL)
	{
		SSWarn << "can not add guild boss bossindex is wrong : " << bossIndex <<END;
		return;
	}

	Enemy* boss = EnemyManager::Instance()->CreateEnemy(pRowData->EnemyID, m_pos, m_rotate);
	if(boss == NULL)
	{
		SSWarn << "GuildBossMgr::BossBegin: create enemy failed" << END;
		return ;
	}

	const XEntityStatistics::RowData* enemy = XEntityInfoLibrary::Instance()->TemplateID2Data_Enemy(pRowData->EnemyID);
	if (enemy == NULL)
	{
		SSWarn << "GuildBossMgr::BossBegin: enemy is null" << END;
		return ;
	}

	boss->SetAttr(BASIC_MaxHP, (enemy->MaxHP) * pRowData->LifePercent);
	boss->SetAttr(BASIC_CurrentHP, bossHp);
	boss->SetAttr(PERCENT_PhysicalAtk, pRowData->AttackPercent);

	pScene->AddUnitAndNotify(boss);

	bool isFirstKill = bossIndex > curIndex;
	GuildBossInfo* info = new GuildBossInfo(guildId, pRowData->BossID, boss, isFirstKill, pScene->GetSceneID(), guildName, bossHp);
	boss->AddListener(info);
	vector<GuildBossInfo*>& infoVec = m_guildid2bossinfo[guildId];
	infoVec.push_back(info);
	SetAttrCount(guildId,count);
}

std::string GuildBossMgr::GetBossAllName(UINT64 guildId)
{
	std::string bossName;
	auto i = m_guildid2bossinfo.find(guildId);
	if (i != m_guildid2bossinfo.end() && !i->second.empty())
	{
		bossName = (*(i->second.begin()))->GetBossName();
		UINT32 bossIndex = (*(i->second.begin()))->GetBossIndex();
		if (bossIndex > 0 && bossIndex <= m_bossconf.Table.size())
		{
			bossName += m_bossconf.Table[bossIndex - 1]->BossName;
		}
	}
	return bossName;
}

GuildBossInfo* GuildBossMgr::GetGuildBossInfo(UINT64 guild)
{
	auto i = m_guildid2bossinfo.find(guild);
	if (i != m_guildid2bossinfo.end() && !i->second.empty())
	{
		return *(i->second.begin());
	}
	return NULL;
}

UINT32 GuildBossMgr::GetAttrCount(UINT64 guildId)
{
	auto i = m_guildid2attrcount.find(guildId);
	if (i == m_guildid2attrcount.end())
	{
		return 0;
	}
	return i->second;
}

UINT32 GuildBossMgr::AddAttrCount(UINT64 guildId)
{
	auto i = m_guildid2attrcount.find(guildId);
	if (i == m_guildid2attrcount.end())
	{
		return m_guildid2attrcount[guildId] = 1;
	}
	++(i->second);
	LogInfo("addattrcount guildid=%llu count=%u",guildId,m_guildid2attrcount[guildId]);
	return m_guildid2attrcount[guildId];
}

void GuildBossMgr::SetAttrCount(UINT64 guildId,UINT32 nCount)
{
	m_guildid2attrcount[guildId] = nCount;
}


UINT32 GuildBossMgr::IsInCDtime(UINT64 guildId)
{
	UINT32 nowTime = time(NULL);

	auto iter = m_cdTime.find(guildId);
	if (iter == m_cdTime.end())
	{
		m_cdTime[guildId] = nowTime;
		return KKSG::ERR_SUCCESS;
	}
	else
	{
		if ((nowTime - iter->second) >= 2)
		{
			m_cdTime[guildId] = nowTime;
			return KKSG::ERR_SUCCESS;
		}
		else
		{
			return KKSG::ERR_INSPIRE_COOLDOWN;
		}
	}

}

void GuildBossMgr::OnEnterScene(Role* role)
{
	auto iter = m_guildid2bossinfo.find(role->getGuildId());
	UINT64 guildId = role->getGuildId(); 
	if (iter == m_guildid2bossinfo.end())
	{
		PtcG2C_NoticeGuildBossEnd notice;
		notice.m_Data.set_iswin(true);
		role->Send(notice);
	}
	else if (!iter->second.empty() && (*(iter->second.begin()))->IsDead())
	{
		PtcG2C_NoticeGuildBossEnd notice;
		notice.m_Data.set_iswin(true);
		role->Send(notice);
	}

	role->Get<CPayV2Record>()->GiveBuff(KKSG::SCENE_GUILD_BOSS);

	role->Get<CReportDataRecord>()->UpdateWeekData(KKSG::WeekReportData_GuildBoss);
	UINT32 count = GetAttrCount(role->getGuildId());
	if (0 == count)
	{
		return;
	}
	for (auto i =  GetGlobalConfig().GuildBossAddAttr.begin(); i !=  GetGlobalConfig().GuildBossAddAttr.end(); ++i)
	{
		PowerPointMgr::Instance()->AdjustOneAttributeByCoeffcient(role, i->seq[0], (float)(i->seq[1]*count)/100.0f);
	}
}

void GuildBossMgr::GetRoleJoinReward(UINT64 guildId, std::vector<ItemDesc>& itemVec)
{

	GuildBossInfo* pGuildBossInfo = GetGuildBossInfo(guildId);
	if (pGuildBossInfo == NULL)
	{
		return ;
	}

	GuildBossConfigTable::RowData* pRowData = m_bossconf.Table[pGuildBossInfo->GetBossIndex() - 1];
	if (pRowData != NULL)
	{
		// 参与奖励
		foreach (i in pRowData->JoinReward)
		{
			ItemDesc item;
			item.itemID = i->seq[0];
			item.itemCount = i->seq[1];
			itemVec.push_back(item);
		}
	}

}
void GuildBossMgr::GetRoleFirstKillReward(UINT64 guildId, std::vector<ItemDesc>& itemVec)
{

	GuildBossInfo* pGuildBossInfo = GetGuildBossInfo(guildId);
	if (pGuildBossInfo == NULL)
	{
		return ;
	}

	GuildBossConfigTable::RowData* pRowData = m_bossconf.Table[pGuildBossInfo->GetBossIndex() - 1];
	if (pGuildBossInfo->IsFirstKillFinished())
	{
		foreach (i in pRowData->FirsttKillReward)
		{
			ItemDesc item;
			item.itemID = i->seq[0];
			item.itemCount = i->seq[1];
			itemVec.push_back(item);
		}
	}
}

void GuildBossMgr::GetRoleLastHurtReward(UINT32 bossIndex, std::vector<ItemDesc>& itemVec)
{
	if (bossIndex >= m_bossconf.Table.size())
	{
		return ;
	}

	GuildBossConfigTable::RowData* pRowData = m_bossconf.Table[bossIndex];
	foreach (i in pRowData->KillReward)
	{
		ItemDesc item;

		item.itemID = i->seq[0];
		item.itemCount = i->seq[1];

		itemVec.push_back(item);
	}
}

void GuildBossMgr::DoGiveReward()
{
	foreach (i in m_guildid2bossinfo)
	{
		if (i->second.empty())
		{
			continue;
		}
		foreach (k in i->second)
		{
			if ((*k)->IsDead())  // kill  boss 奖励已经发放
			{
				continue ;
			}

			foreach (j in (*k)->m_roleid2damage)
			{
				std::vector<ItemDesc> itemVec;

				// 参与奖
				itemVec.clear();
				GetRoleJoinReward(i->first, itemVec);
				if (!itemVec.empty())
				{
					MailSender sender;
					sender.SetContent(Mail_System,  GetGlobalConfig().GuildBossJoinMailTitle,  GetGlobalConfig().GuildBossJoinMailContent);
					sender.SetItems(itemVec);
					sender.SetReason(ItemFlow_GuildBossAward);
					sender.Send(j->first);
					SSInfo << "Send Mail To Client, MailType = GuildBossFinishMailTitle, " << "RoleID=" << i->first << END; 
				}

			}

		}
		
	}
}

void GuildBossMgr::BroadCastHint(bool isremove)
{
	PtcG2C_HintNotify ntf;
	ntf.m_Data.add_systemid(GuildBossId);
	ntf.m_Data.set_isremove(isremove);

	std::unordered_map<UINT64, CGuildSimple>& guildSimpleMap = GuildSimpleMgr::Instance()->GetAllGuild();
	for (auto i = guildSimpleMap.begin(); i != guildSimpleMap.end(); ++i)
	{
		if (MultActivityMgr::Instance()->IsGuildBossOpen(i->second.GetGuildLvl()))
		{
			i->second.BroadCast(ntf);
		}
	}
}

void GuildBossMgr::NoticeCastHint(UINT64 guildId)
{
	CGuildSimple* pGuild = GuildSimpleMgr::Instance()->GetGuildSimple(guildId);
	if (pGuild != NULL && MultActivityMgr::Instance()->IsGuildBossOpen(pGuild->GetGuildLvl()))
	{
		PtcG2C_HintNotify ntf;
		ntf.m_Data.add_systemid(GuildBossId);
		ntf.m_Data.set_isremove(false);

		pGuild->BroadCast(ntf);
	}
}


void GuildBossInfo::NoticeBossEnd()
{
	Scene* pScene = SceneManager::Instance()->FindBySceneID(m_sceneId);
	if (pScene != NULL)
	{
		PtcG2C_NoticeGuildBossEnd notice;
		notice.m_Data.set_iswin(true);
		pScene->Broadcast(notice);
	}
}


void GuildBossMgr::ClearAfterEnd()
{
	for (auto i = m_guildid2bossinfo.begin(); i != m_guildid2bossinfo.end(); ++i)
	{
		foreach (j in i->second)
		{
			(*j)->ClearListener();
			(*j)->RemoveBoss();
			delete (*j);
			(*j) = NULL;
		}
	}
	m_guildid2bossinfo.clear();
	m_guildid2attrcount.clear();
	m_cdTime.clear();
}
