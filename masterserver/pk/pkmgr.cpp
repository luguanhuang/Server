#include "pch.h"
#include "pkmgr.h"
#include "ranklist/pkranklist.h"
#include "ranklist/ranklistmgr.h"
#include "pb/project.pb.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "table/globalconfig.h"
#include "util/XCommon.h"
#include "network/gslink.h"
#include "pk/ptcm2c_pktimeoutm2cntf.h"
#include "global/GlobalConfigDBBuild.h"
#include "mail/mailmgr.h"
#include "mail/mailconfig.h"
#include "define/maildef.h"
#include "guild/guildladder.h"
#include "table/OpenSystemMgr.h"
#include "define/systemiddef.h"
#include "event/eventmgr.h"
#include "role/rolesummarymgr.h"
#include "scene/scenecreate.h"
#include "scene/sceneswitch.h"
#include "other/matchhandler.h"
#include "pk/rpcm2k_pkmatchtoworldreq.h"
#include "network/worldlink.h"
#include "cross/crosszonestatus.h"
#include "util/gametime.h"
#include "arenastar/arenastarmgr.h"
#include "network/dblink.h"
#include "util/gametime.h"
#include "pk/ptcm2g_pkupdatesynntf.h"
#include "pk/ptcm2k_pkranklogsyn.h"

INSTANCE_SINGLETON(PkMgr);

#define PAIR_MAX 2

PkMgr::PkMgr()
{
	m_lastWeekRankRewardT = 0;
}

PkMgr::~PkMgr()
{

}

bool PkMgr::Init()
{
	m_handler = CTimerMgr::Instance()->SetTimer(this, 0, 1000, -1, __FILE__, __LINE__);	

	if (!m_table.LoadFile("table/PkPrepare.txt"))
	{
		SSWarn<<"load file table/PkPrepare.txt failed"<<END;
		return false;
	}
	if (!m_ranktable.LoadFile("table/PkRankReward.txt"))
	{
		SSWarn<<"load file table/PkRankReward.txt failed"<<END;
		return false;
	}

	ReadData();
	//SSDebug << __FUNCTION__ << " last reward time = " << m_lastWeekRankRewardT << END;

	return true;
}

void PkMgr::Uninit()
{
	CTimerMgr::Instance()->KillTimer(m_handler);
}

void PkMgr::Update()
{
	// match role
	PkInfo pkinfo11;
	PkInfo pkinfo22;
	if (MatchRole(pkinfo11, pkinfo22))	
	{
		Start(pkinfo11, pkinfo22, 0);
	}
	// match robot
	PkInfo pkinfo33;
	PkInfo pkinfo44;
	int robotlookupid = 0;
	if (MatchRobot(pkinfo33, robotlookupid))
	{
		Start(pkinfo33, pkinfo44, robotlookupid);
	}
	// timeout
	CheckTimeout();
}

void PkMgr::CheckTimeout()
{
	std::vector<UINT64> waitDel;
	for(size_t i = 0; i < m_preparelist.size(); ++i)
	{
		auto& tmp = m_preparelist[i];
		CRole* role = CRoleManager::Instance()->GetByRoleID(tmp.RoleID());
		if (NULL == role)
		{
			continue;
		}
		if (tmp.CheckTimeout())	
		{
			if (tmp.IsEmpty())
			{
				waitDel.push_back(tmp.RoleID());
			}
		}
	}

	for(size_t i = 0; i < waitDel.size(); ++i)
	{
		RemoveRole(waitDel[i], PK_RR_TIMEOUT);
	}
}

bool PkMgr::MatchRole(PkInfo& pkinfo11, PkInfo& pkinfo22)
{
	time_t now = GameTime::GetTime();
	if (m_preparelist.size() <= 1)
	{
		return false;
	}
	UINT32 first = 0;
	UINT32 second = UINT32(-1);
	while(first + 1 < m_preparelist.size())
	{
		UINT32 count = first + 1;
		PkInfo info = m_preparelist[first];
		UINT32 min = (UINT32)(-1);
		for (auto i = m_preparelist.begin() + first + 1; i != m_preparelist.end(); ++i)
		{
			UINT32 tempspan = i->GetFitSpan(info);
			if (tempspan < min)
			{
				second = count;
				min = tempspan;
			}
			++count;
		}
		if (second != (UINT32)(-1))
		{
			break;
		}
		++first;
	}
	if (second != UINT32(-1))
	{
		pkinfo11 = m_preparelist[first];
		pkinfo22 = m_preparelist[second];
		CRole* firstrole = CRoleManager::Instance()->GetByRoleID(pkinfo11.RoleID());
		CRole* secondrole = CRoleManager::Instance()->GetByRoleID(pkinfo22.RoleID());
		if (NULL != firstrole && NULL != secondrole)
		{
			RemoveRole(pkinfo11.RoleID(), PK_RR_DOWN);
			RemoveRole(pkinfo22.RoleID(), PK_RR_DOWN);
			return true;
		}
		else
		{
			if (NULL == firstrole)
			{
				RemoveRole(pkinfo11.RoleID(), PK_RR_ERR);
			}
			if (NULL == secondrole)
			{
				RemoveRole(pkinfo22.RoleID(), PK_RR_ERR);
			}
		}
	}
	return false;
}

bool PkMgr::MatchRobot(PkInfo& info, int& robotlookupid)
{
	if (m_preparerobotlist.empty())
	{
		return false;
	}
	info = m_preparerobotlist.front();
	if (info.TimeBegin() + GetGlobalConfig().PkMatchDelay >= UINT32(time(NULL)))
	{
		return false;
	}
	CRole* pRole = CRoleManager::Instance()->GetByRoleID(info.RoleID());
	RemoveRole(info.RoleID(), PK_RR_DOWN);
	robotlookupid = info.GetLookupID();
	if (pRole && robotlookupid)
	{
		return true;
	}
	return false;
}

void PkMgr::Start(PkInfo& pkinfo11, PkInfo& pkinfo22, int robotlookupid)
{
	CRole* pRole11 = CRoleManager::Instance()->GetByRoleID(pkinfo11.RoleID());
	CRole* pRole22 = CRoleManager::Instance()->GetByRoleID(pkinfo22.RoleID());
	if(NULL == pRole11)
	{
		SSError << " role11 == NULL " << END;
		return ;
	}
	if(pRole22 && robotlookupid)
	{
		SSError << " para 3 " << END;
		return ;
	}
	if(NULL == pRole22 && 0 == robotlookupid)
	{
		SSError << " para 1 " << END;
		return;
	}

	KKSG::PkBattleInfo pkinfo;

	if(!FillBattleRoleInfo(*pkinfo.add_roles(), pkinfo11))
	{
		return ;
	}
	if (CGuildLadder::Instance()->TryReward(pkinfo11.RoleID()))
	{
		pkinfo.add_guildladderid(pkinfo11.RoleID());
	}

	if(pRole22)
	{
		if(!FillBattleRoleInfo(*pkinfo.add_roles(), pkinfo22))
		{
			return ;
		}
		if (CGuildLadder::Instance()->TryReward(pkinfo22.RoleID()))
		{
			pkinfo.add_guildladderid(pkinfo22.RoleID());
		}

		m_opPairRec.AddPairRec(pRole11->GetID(), pRole22->GetID(), GameTime::GetTime());

	}
	else if(robotlookupid)
	{
		pkinfo.set_robotlookupid(robotlookupid);
	}
	else
	{
		return ;
	}
	CreateSceneBegin(pkinfo);
}

bool PkMgr::AddRole(CRole* role, bool addrobot, const KKSG::PkMatchInfo& pkmi)
{
	if(NULL == role)
	{
		return false;
	}
	PkInfo info;
	auto& tmp = info.MatchInfo();
	tmp.set_roleid(role->GetID());
	tmp.set_timebegin(UINT32(time(NULL)));
	tmp.mutable_rolerec()->CopyFrom(pkmi.rolerec());
	tmp.set_rolelevel(pkmi.rolelevel());
	GetMatchStage(pkmi.rolerec().point(), tmp);

	if (info.IsRobot() || addrobot)
	{
		m_preparerobotlist.push_back(info);
	}
	else
	{
		m_preparelist.push_back(info);
	}
	return true;
}

void PkMgr::RemoveReason(UINT64 roleid, PkRemoveReason reason)
{
	SSDebug << __FUNCTION__ << " roleid = " << roleid << " reason = " << reason << END;

	CRole* pRole = CRoleManager::Instance()->GetByRoleID(roleid);//pRole 有可能为 null
	HMatchHandler handler;

	if(PK_RR_DOWN == reason || PK_RR_TIMEOUT == reason || PK_RR_INWORLD == reason)
	{
		handler.Done(pRole, KKSG::KMT_PK);
	}

	if(PK_RR_TIMEOUT == reason)
	{
		TimeOutNtf(pRole);
	}

	if(PK_RR_INWORLD != reason)
	{
		auto it = find_if(m_preparelist.begin(), m_preparelist.end(), EqualPkInfo(roleid));
		if(it != m_preparelist.end())
		{
			it->CheckOutWorld();
		}
	}


}

bool PkMgr::RemoveRole(UINT64 roleid, PkRemoveReason reason)
{
	RemoveReason(roleid, reason);

	auto i = find_if(m_preparelist.begin(), m_preparelist.end(), EqualPkInfo(roleid));
	if (i != m_preparelist.end())
	{
		m_preparelist.erase(i);
		return true;
	}
	auto j = find_if(m_preparerobotlist.begin(), m_preparerobotlist.end(), EqualPkInfo(roleid));
	if (j != m_preparerobotlist.end()) 
	{
		m_preparerobotlist.erase(j);
		return true;
	}
	return false;
}

bool PkMgr::InPrepare(UINT64 roleid)
{
	auto i = find_if(m_preparelist.begin(), m_preparelist.end(), EqualPkInfo(roleid));
	if (i != m_preparelist.end())
	{
		return true;
	}
	auto j = find_if(m_preparerobotlist.begin(), m_preparerobotlist.end(), EqualPkInfo(roleid));
	if (j != m_preparerobotlist.end())
	{
		return true;
	}
	return false;
}

void PkMgr::HandlerRankUpdateDelay(const KKSG::QMHRPkArg& arg, KKSG::QMHRPkRes& res)
{
	PkRealTimeRankList* pRankList = (PkRealTimeRankList*)CRankListMgr::Instance()->GetRankList(PkTypeToRankType(arg.type()));
	if(NULL == pRankList)
	{
		return;
	}
	SSDebug << __FUNCTION__ << END;

	const auto& rankdata = arg.rankdata();
	UINT64 roleid = rankdata.roleid();
	UINT32 point = rankdata.point();

	res.set_roleid(roleid);
	res.set_lastrank(pRankList->GetRank(roleid));

	SSDebug << " roleid = " << roleid << " point = " << point << END;
	pRankList->OnChange(roleid, point, (UINT32)time(NULL), rankdata.extradata());

	res.set_allrank(pRankList->GetSize());
	res.set_currank(pRankList->GetRank(roleid));
}

UINT32 PkMgr::GetRank(UINT64 roleid, KKSG::PkNVNType type/* = KKSG::PK_1v1*/)
{
	PkRealTimeRankList* ranklist = (PkRealTimeRankList*)CRankListMgr::Instance()->GetRankList(PkTypeToRankType(type));
	if (NULL == ranklist)
	{
		SSError << " find not pkrealtimeranklist " << END;
		return INVALID_RANK;
	}
	return ranklist->GetRank(roleid);
}

void PkMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	//Update();
	CheckWeekRankRewrad();
	CheckMatchToWorld();
	time_t now = GameTime::GetTime();
	m_opDelay.Update(now);
	m_opPairRec.Update(now);
}

UINT32 PkMgr::RankRewardLeftTime()
{
	return GetGlobalConfig().PkGetResetLeftTime(m_lastWeekRankRewardT, GameTime::GetTime());
}

void PkMgr::CheckWeekRankRewrad()
{
	if(!DBLink::Instance()->IsConnected())
	{
		return ;
	}
	time_t now = GameTime::GetTime();
	if(0 == m_lastWeekRankRewardT)//first time
	{
		m_lastWeekRankRewardT = UINT32(GameTime::GetTime());
		WriteData();
		return;
	}
	if(GetGlobalConfig().PkGetResetLeftTime(m_lastWeekRankRewardT, now) > 0)
	{
		return;
	}

	m_lastWeekRankRewardT = now;
	WriteData();

	PkRealTimeRankList* pRank1v1 = (PkRealTimeRankList*)CRankListMgr::Instance()->GetRankList(KKSG::PkRealTimeRank);
	PkRealTimeRankList* pRank2v2 = (PkRealTimeRankList*)CRankListMgr::Instance()->GetRankList(KKSG::PkRank2v2);
	if (NULL == pRank1v1 || NULL == pRank2v2)
	{
		SSError << " find not pkrealtimeranklist " << END;
		return ;
	}

	PkRankWrite(pRank1v1, now);

	//SSDebug << __FUNCTION__ << END;

	std::vector<PkRankInfo*> allinfo1v1;
	pRank1v1->GetRankInfo(allinfo1v1);

	std::vector<PkRankInfo*> allinfo2v2;
	pRank2v2->GetRankInfo(allinfo2v2);

	std::unordered_map<UINT64, UINT32> role2rank;

	int RankIndex = 0;
	for(size_t i = 0; i < allinfo1v1.size(); ++i)
	{
		UINT64 roleid = allinfo1v1[i]->GetUniqueId();		
		UINT32 rank = (++RankIndex);
		if(0 == role2rank[roleid] || rank < role2rank[roleid])
		{
			role2rank[roleid] = rank;
		}

		if (rank == 1)
		{
			EventMgr::Instance()->AddEventOnlyOnMS(roleid, BONUS_EVENT_OFFSET + GUILDBONUS_COM_TYPE_PK_WEEK_RANK,rank);
		}
		if (rank <= 10)
		{
			EventMgr::Instance()->AddEvent(roleid, DESIGNATION_COM_TYPE_PK_WEEK_RANK,rank);
		}
	}

	RankIndex = 0;
	for(size_t i = 0; i < allinfo2v2.size(); ++i)
	{
		UINT64 roleid = allinfo2v2[i]->GetUniqueId();		
		UINT32 rank = (++RankIndex);
		if (rank <= 10)
		{
			EventMgr::Instance()->AddEvent(roleid, DESIGNATION_COM_TYPE_2V2PK_WEEK_RANK,rank);
		}

		if(0 == role2rank[roleid] || rank < role2rank[roleid])
		{
			role2rank[roleid] = rank;
		}
	}

	GiveReward(role2rank);

	ArenaStarMgr::Instance()->OnArenaSeasonEnd(KKSG::AST_PK);

	pRank1v1->Clear();//排行榜清空
	pRank2v2->Clear();//排行榜清空

}

void PkMgr::GetReward(UINT32 rank, std::vector<ItemDesc>& items)
{
	for (auto i = m_ranktable.Table.begin(); i != m_ranktable.Table.end(); ++i)
	{
		PkRankTable::RowData* rowdata = *i;
		if (rowdata->rank[0] <= rank && rowdata->rank[1] >= rank)
		{
			for (auto j = rowdata->reward.begin(); j != rowdata->reward.end(); ++j)
			{
				items.push_back(ItemDesc(j->seq[0], j->seq[1]));	
			}
		}
	}

}

void PkMgr::TestResetLastWeek()
{
	m_lastWeekRankRewardT = 0;
}

bool PkMgr::FillBattleRoleInfo(KKSG::PkRoleInfo& data, PkInfo& info)
{
	auto pSummary = CRoleSummaryMgr::Instance()->GetRoleSummary(info.RoleID());
	if(NULL == pSummary)
	{
		return false;
	}
	pSummary->FillSmallInfo(*data.mutable_rolebrief());
	data.mutable_pkrec()->CopyFrom(info.MatchInfo().rolerec());
	data.set_serverid(MSConfig::Instance()->GetServerID());
	return true;
}

void PkMgr::CreateSceneBegin(const KKSG::PkBattleInfo& pkinfo)
{
	UINT32 now = UINT32(time(NULL));
	for(int i = 0; i < pkinfo.roles_size(); ++i)
	{
		m_opDelay.AddRole(pkinfo.roles(i).rolebrief().roleid(), now + 10);
	}

	KKSG::CreateBattleParam data;
	data.mutable_pkinfo()->CopyFrom(pkinfo);
	CSceneCreator::Instance()->CreateBattleScene(GetGlobalConfig().PkSceneId, &ICreatePkListener::GlobalCreatePkListener, data);
}

void PkMgr::CreateSceneEnd(const KKSG::PkBattleInfo& pkinfo, UINT32 sceneID)
{
	std::vector<CRole*> roles;
	for(int i = 0; i < pkinfo.roles_size(); ++i)
	{
		UINT64 roleID = pkinfo.roles(i).rolebrief().roleid();
		CRole* pRole = CRoleManager::Instance()->GetByRoleID(roleID);	
		if(pRole)
		{
			roles.push_back(pRole);
		}
		else
		{
			SSWarn << " find role failed ID = " << roleID << END;
		}
	}

	if(pkinfo.roles_size() != roles.size())//低概率异常处理
	{
		for(size_t i = 0; i < roles.size(); ++i)
		{
			m_opDelay.DelRole(roles[i]->GetID());
			TimeOutNtf(roles[i]);//当作超时通知
		}
		return ;
	}

	KKSG::SceneSwitchData tmpData;
	for(size_t i = 0; i < roles.size(); ++i)
	{
		CSceneSwitch::EnterScene(roles[i], sceneID, tmpData);		
	}
}

bool PkMgr::InDelay(UINT64 roleID)
{
	return m_opDelay.HaveRole(roleID);
}

void PkMgr::TimeOutNtf(CRole* pRole)
{
	if(NULL == pRole)
	{
		return ;
	}
	PtcM2C_PkTimeoutM2CNtf ntf;
	ntf.m_Data.set_timeout(0);
	pRole->Send(ntf);
}

int PkMgr::PkTypeToRankType(KKSG::PkNVNType type)
{
	switch(type)
	{
	case KKSG::PK_1v1:
		{
			return KKSG::PkRealTimeRank;
		}
		break;
	case KKSG::PK_2v2:
		{
			return KKSG::PkRank2v2;
		}
		break;
	default:
		break;
	}
	return 0;
}

void PkMgr::TestPrint()
{
	SSWarn << __FUNCTION__ << " match_role_size = " << m_preparelist.size() << " match_robot_size = " << m_preparerobotlist.size() << END;
}

void PkMgr::GetMatchStage(UINT32 point, KKSG::PkMatchInfo& data, KKSG::PkNVNType type/* = KKSG::PK_1v1*/)
{
	int last = int(m_table.Table.size()) - 1;
	if(last < 0)
	{
		return;
	}
	for(int i = last; i >= 0; --i)
	{
		PkPrepareTable::RowData* conf = m_table.Table[i];
		if(conf->point[0] <= point && point <= conf->point[1] && conf->type == type)
		{
			auto& tmp = *data.add_stage();

			tmp.set_ctime(conf->time);
			tmp.set_span(conf->span);
			tmp.set_robotpercent(conf->percent);
			tmp.set_lookupid(conf->robotlookupid);
		}
	}

	last = data.stage_size() - 1;
	if(last < 0)
	{
		return;
	}
	UINT32 now = UINT32(time(NULL));
	for(int i = last; i >= 0; --i)
	{
		auto& tmp = *data.mutable_stage(i);
		UINT32 tt = tmp.ctime();
		if(i == last)
		{
			tmp.set_ctime(tt + now);
		}
		else
		{
			tmp.set_ctime(tt + data.stage(i+1).ctime());
		}
	}
}

void PkMgr::CheckMatchToWorld()
{
	if(!CrossZoneStatus::Instance()->IsCrossZoneUsable())
	{
		return;
	}

	for(size_t i = 0; i < m_preparelist.size(); ++i)
	{
		m_preparelist[i].CheckToWorld();	
	}
}

void PkMgr::OnMatchInWorld(const KKSG::PkMatchToWorldPara& data)
{
	UINT64 roleID = data.roleid();
	SSInfo << __FUNCTION__ << " roleid = " << roleID << END;
	RemoveRole(roleID, PK_RR_INWORLD);	

	CRole* pRole = CRoleManager::Instance()->GetByRoleID(roleID);
	if(pRole)
	{
		KKSG::SceneSwitchData tmp;
		CSceneSwitch::EnterScene(pRole, data.sceneid(), tmp, data.gsline(), true, GetGlobalConfig().PkSceneId);
	}
	else
	{
		SSWarn << " find role failed roleid = " << roleID << END;
	}
}

void PkMgr::ReadData()
{
	UINT32 tmp = convert<UINT32>(CGlobalConfigDBBuild::Instance()->GetConfigValue(ePkRankRewardLastTime));
	m_lastWeekRankRewardT = tmp;
	SSInfo << " read lastup = " << m_lastWeekRankRewardT << END;
}

void PkMgr::WriteData()
{
	SSInfo << " write lastup = " << m_lastWeekRankRewardT << END;
	CGlobalConfigDBBuild::Instance()->SetConfigValue(ePkRankRewardLastTime, ToString(m_lastWeekRankRewardT));
	CGlobalConfigDBBuild::Instance()->UpdateToDb(ePkRankRewardLastTime);
}

void PkMgr::GiveReward(std::unordered_map<UINT64,UINT32>& role2rank)
{
	std::string mailtitle = "null";
	std::string mailcontent;
	const MailConf* pMailConf = MailConfig::Instance()->GetMailConf(MailConf_PkWeekReward);
	if(pMailConf)
	{
		mailtitle = pMailConf->m_title;
		mailcontent = pMailConf->m_content;
	}
	std::vector<std::string> vec;
	vec = Split(mailcontent, "x");

	std::vector<ItemDesc> rewards;
	for(auto it = role2rank.begin(); it != role2rank.end(); ++it)
	{
		UINT64 roleID = it->first;
		UINT32 rank = it->second;
		rewards.clear();
		GetReward(rank, rewards);
		if(!rewards.empty())
		{
			std::string tmpcontent = mailcontent;
			if(2 == vec.size())
			{
				tmpcontent = vec[0] + ToString(rank) + vec[1];		
			}
			MailData maildata(mailtitle, tmpcontent, rewards, MAIL_EXPIRE_TIME);
			maildata.SetReason(ItemFlow_PkWeekRankAward);
			CMailMgr::Instance()->SendMail(roleID, maildata);
		}
	}
}

void PkMgr::OnRoleLogin(CRole* pRole)
{
	if(NULL == pRole)
	{
		return;
	}
	PtcM2G_PkUpdateSynNtf ptc;
	ptc.m_Data.set_roleid(pRole->GetID());
	ptc.m_Data.set_ctime(m_lastWeekRankRewardT);
	pRole->SendMsgToGS(ptc);
}

void PkMgr::PkRankWrite(PkRealTimeRankList* pRank, UINT32 now)
{
	if(NULL == pRank)
	{
		return;
	}
	std::vector<PkRankInfo*> allinfo;
	pRank->GetRankInfo(allinfo);
	UINT32 TopCount = 10;
	std::map<UINT32, std::vector<KKSG::PkRoleRankLog>> profData;
	for(size_t i = 0; i < allinfo.size(); ++i)
	{
		auto pInfo = allinfo[i];
		if(NULL == pInfo)
		{
			continue;
		}
		UINT64 roleID = pInfo->GetUniqueId();
		auto pSummary = CRoleSummaryMgr::Instance()->GetRoleSummary(roleID);
		if(NULL == pSummary)
		{
			continue;
		}
		UINT32 allRank = i + 1;
		UINT32 profession = pSummary->GetProfession() % 10;//取基础职业
		std::string openID = pSummary->GetAccount();
		UINT32 proRank = profData[profession].size() + 1;
		UINT32 point = pInfo->m_point;
		if(proRank > TopCount)
		{
			continue;
		}
		KKSG::PkRoleRankLog pklog;
		pklog.set_serverid(MSConfig::Instance()->GetServerID());
		pklog.set_roleid(roleID);
		pklog.set_openid(openID);
		pklog.set_point(point);
		pklog.set_profession(profession);
		pklog.set_pro_rank(proRank);
		pklog.set_all_rank(allRank);
		profData[profession].push_back(pklog);
	}
	PtcM2K_PkRankLogSyn ptc;
	for(auto i = profData.begin(); i != profData.end(); ++i)
	{
		const auto& vec = i->second;
		for(auto j = vec.begin(); j != vec.end(); ++j)
		{
			*ptc.m_Data.add_roles() = *j;
		}
	}
	SSInfo << "pk_rank_log size = " << ptc.m_Data.roles_size() << END;
	for(int i = 0; i < ptc.m_Data.roles_size(); ++i)
	{
		const auto& pklog = ptc.m_Data.roles(i);
		SSInfo << "pk_rank_log: " << pklog.serverid() << "|" << pklog.roleid() << "|" << pklog.openid() << "|"
			<< pklog.point() << "|" << pklog.profession() << "|" << pklog.pro_rank() << "|" << pklog.all_rank() << END;
	}
	WorldLink::Instance()->SendTo(ptc);
}

const KKSG::PkMatchStage* PkInfo::GetBack()
{
	int n = m_info.stage_size();
	if(0 == n)
	{
		return NULL;
	}
	return &(m_info.stage(n-1));
}

void PkInfo::PopBack()
{
	if(m_info.stage_size() > 0)
	{
		m_info.mutable_stage()->RemoveLast();
	}
}

bool PkInfo::IsEmpty()
{
	return (0 == m_info.stage_size());
}

bool PkInfo::CheckTimeout()
{
	const auto tmp = GetBack();
	if(NULL == tmp)
	{
		return true;
	}
	time_t timestamp = time_t(tmp->ctime());
	if (time(NULL) > timestamp)
	{
		PopBack();
		return true; 
	}
	return false;
}

bool PkInfo::IsRobot()
{
	const auto tmp = GetBack();
	if(NULL == tmp)
	{
		return false;
	}

	UINT32 percent = tmp->robotpercent();
	UINT32 random = XRandom::randInt(1, 101);
	if (percent >= random)
	{
		return true;	
	}
	return false;
}

UINT32 PkInfo::GetFitSpan(const PkInfo& other)
{
	if(IsInWorld() || other.IsInWorld())
	{
		return UINT32(-1);
	}
	if(PkMgr::Instance()->GetRoleOpRecMgr().IsPairLimit(RoleID(), other.RoleID(), GameTime::GetTime(), GetGlobalConfig().PkRoleMatchPairDayLimit, GetGlobalConfig().PkRoleMatchPairWeekLimit))
	{
		return UINT32(-1);
	}
	if(!GetGlobalConfig().PkInSameLevelRange(RoleLevel(), other.RoleLevel()))
	{
		return UINT32(-1);
	}

	UINT32 temppoint = other.Point();
	const auto tmp = GetBack();
	if(NULL == tmp)
	{
		return UINT32(-1);
	}
	UINT32 tmpspan = (Point() > temppoint)? (Point() - temppoint): (temppoint - Point());
	if(tmpspan <= tmp->span())
	{
		return tmpspan;
	}
	return (UINT32)(-1);
}

int PkInfo::GetLookupID()
{
	const auto tmp = GetBack();
	if(NULL == tmp)
	{
		return 0;
	}
	return tmp->lookupid();
}

void PkInfo::CheckToWorld()
{
	if(IsInWorld())
	{
		return;
	}

	UINT32 now = UINT32(time(NULL));
	UINT32 confTime = GetGlobalConfig().PkMatchToWorldTime;
	if(0 == confTime)
	{
		return;
	}
	if(TimeBegin() + confTime > now)
	{
		return;
	}

	m_inWorld = true;		

	RpcM2K_PkMatchToWorldReq* rpc = RpcM2K_PkMatchToWorldReq::CreateRpc();
	auto& data = rpc->m_oArg;
	data.set_type(KKSG::MTWT_ADD);
	FillToWorldData(data);
	WorldLink::Instance()->SendTo(*rpc);
}

void PkInfo::CheckOutWorld()
{
	if(!IsInWorld())
	{
		return;
	}

	m_inWorld = false;

	RpcM2K_PkMatchToWorldReq* rpc = RpcM2K_PkMatchToWorldReq::CreateRpc();
	auto& data = rpc->m_oArg;
	data.set_type(KKSG::MTWT_DEL);
	data.set_roleid(RoleID());
	WorldLink::Instance()->SendTo(*rpc);
}

void PkInfo::FillToWorldData(KKSG::PkMatchToWorldPara& data)
{
	data.set_curtime(UINT32(time(NULL)));
	data.mutable_pkinfo()->CopyFrom(m_info);
	PkMgr::Instance()->FillBattleRoleInfo(*data.mutable_roleinfo(), *this);
	if (CGuildLadder::Instance()->TryReward(RoleID()))
	{
		data.set_isguildladder(true);
	}
}
