#include "pch.h"
#include "guildactivity.h"
#include "pb/project.pb.h"
#include "table/globalconfig.h"
#include "util/XCommon.h"
#include "mail/maildata.h"
#include "mail/mailmgr.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "role/rolemanager.h"
#include "guild/rpcm2g_reqguildactivity.h"
#include "notice/noticemgr.h"
#include <time.h>
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"


#include "foreach.h"
#include "loghelper/tlogr.h"
#include "util/gametime.h"
using std::make_pair;

CGuildActivity::CGuildActivity()
{
	m_weekActivity = 0;
	m_losTimes = 0;
	m_pGuild = NULL;
}

CGuildActivity::CGuildActivity(Guild* pGuild)
{
	m_weekActivity = 0;
	m_losTimes = 0;
	m_pGuild = pGuild;
}

CGuildActivity::~CGuildActivity()
{
	m_pGuild = NULL;
}

bool CGuildActivity::IsChanged()
{
	return m_isModified.TestAndReset();
}

bool CGuildActivity::Load(const KKSG::GuildAllDBData & guilddata)
{
	KKSG::GuildActivitySave activity;
	if (!guilddata.has_guildactivity())
	{
		return true;
	}

	if (guilddata.guildactivity().empty())
	{
		return true;
	}

	if (!activity.ParseFromString(guilddata.guildactivity()))
	{
		return true;
	}
	
	m_weekActivity = activity.weekactivity();
	m_losTimes = activity.losetimes();
	for (UINT32 cnt = 0; cnt < activity.roleacts_size(); ++cnt)
	{
		m_roleActivityMap[activity.roleacts(cnt).roleid()] = activity.roleacts(cnt).activity();
	}

	for (UINT32 i = 0; i < activity.lastweek_size(); ++i)
	{
		stGuildActivity act; 
		act.nRoleID = activity.lastweek(i).roleid();
		act.nActivity = activity.lastweek(i).activity();
		m_vecguildActivity.push_back(act);
	}
	Sort();

	return true;
}




void CGuildActivity::Save(KKSG::GuildAllDBData & guilddata)
{
	guilddata.mutable_guildactivity()->assign(ToBinary());
}

const std::string CGuildActivity::ToBinary()
{
	KKSG::GuildActivitySave save;
	save.set_weekactivity(m_weekActivity);
	save.set_losetimes(m_losTimes);
	foreach (i in m_roleActivityMap)
	{
		KKSG::roleActivity* pActivity = save.add_roleacts();
		pActivity->set_roleid(i->first);
		pActivity->set_activity(i->second);
	}

	foreach (i in m_vecguildActivity)
	{
		KKSG::roleActivity* pActivity = save.add_lastweek();
		pActivity->set_roleid(i->nRoleID);
		pActivity->set_activity(i->nActivity);
	}

	return save.SerializeAsString();
}

void CGuildActivity::Sort()
{
	std::sort(m_vecguildActivity.begin(), m_vecguildActivity.end(),stGuildActivityCmp());
}

bool CGuildActivity::DoCheck()
{
	DoTxLog();
	if (XCommon::GetWeekNum() == 1)
	{
		bool needDel = WeekCheck();
		m_weekActivity = 0;
		m_vecguildActivity.clear();
		for (auto iter = m_roleActivityMap.begin(); iter != m_roleActivityMap.end(); iter++)
		{
			stGuildActivity act;
			act.nRoleID = iter->first;
			act.nActivity = iter->second;
			m_vecguildActivity.push_back(act);
		}
		Sort();

		foreach (i in m_roleActivityMap)
		{
			i->second = 0;
		}
		
		SetChang();
		return needDel;
	}
	else
	{
		DailyCheck();
		return false;
	}
}




void CGuildActivity::DailyCheck()
{
	if ( GetGlobalConfig().GuildActLvl.empty())
	{
		return ;
	}

	if (XCommon::IsWeekSame(m_pGuild->GetOpenTime(), time(NULL)))
	{
		return;
	}

	if (m_weekActivity < GetGlobalConfig().GuildActLvl[0] * XCommon::GetWeekNum() * m_pGuild->GetMemberCount())
	{
		std::vector<ItemDesc> itemVec;
		MailData mail( GetGlobalConfig().GuildActDailyTitle, GetGlobalConfig().GuildActDailyCon, itemVec, MAIL_EXPIRE_TIME);
		CMailMgr::Instance()->SendMail(m_pGuild->GetLeaderID(), mail);

		std::set<UINT64> vicLeaderSet;
		m_pGuild->GetVicLeader(vicLeaderSet);
		foreach (i in vicLeaderSet)
		{
			CMailMgr::Instance()->SendMail(*i, mail);
		}
	}

	_CheckLeaderActivity();
}

bool CGuildActivity::WeekCheck()
{
	if ( GetGlobalConfig().GuildActLvl.empty())
	{
		return false;
	}
	
	if (!_IsOutNewProtTime())
	{
		return false;
	}

	if (m_weekActivity < GetGlobalConfig().GuildActLvl[0] * 7 * m_pGuild->GetMemberCount())
	{
		++m_losTimes;

		if (m_losTimes >= GetGlobalConfig().GuildActDelMax)
		{
			std::vector<ItemDesc> itemVec;
			MailData mail( GetGlobalConfig().GuildActDelTitle, GetGlobalConfig().GuildActDelCon, itemVec, MAIL_EXPIRE_TIME);
			CMailMgr::Instance()->SendGuildMail(m_pGuild->GetID(), mail);
			return true;
		}
		else
		{
			std::vector<ItemDesc> itemVec;
			MailData mail( GetGlobalConfig().GuildActWeekTitle, GetGlobalConfig().GuildActWeekCon, itemVec, MAIL_EXPIRE_TIME);
			CMailMgr::Instance()->SendGuildMail(m_pGuild->GetID(), mail);
		}
		SetChang();
	}
	else
	{
		if (m_losTimes != 0)
		{
			m_losTimes = 0;
			SetChang();
		}
	}

	_CheckLeaderActivity();
	return false;
}

void CGuildActivity::DoRoleAdd(UINT64 roleId)
{
	CRole* pRole = CRoleManager::Instance()->GetByRoleID(roleId);
	if (pRole == NULL)
	{
		return ;
	}
	RpcM2G_ReqGuildActivity* pMsg = RpcM2G_ReqGuildActivity::CreateRpc();
	pMsg->m_oArg.set_roleid(roleId);
	pRole->SendMsgToGS(*pMsg);
}

void CGuildActivity::DoRoleLeave(UINT64 roleId)
{
	auto i = m_roleActivityMap.find(roleId);
	if (i == m_roleActivityMap.end())
	{
		return ;
	}

	m_weekActivity = m_weekActivity > i->second ? (m_weekActivity - i->second) : 0;
	m_roleActivityMap.erase(roleId);

	SetChang();
}

void CGuildActivity::_CheckLeaderActivity()
{
	CRoleSummary* pRoleSumOld = CRoleSummaryMgr::Instance()->GetRoleSummary(m_pGuild->GetLeaderID()); 
	if (pRoleSumOld != NULL && pRoleSumOld->GetLastLogout() >= GetGlobalConfig().GuildActLeaDay * CGuildActivity::DAY_TIME)
	{
		UINT64 nextLeaderId = _FindNextLeader();
		CRoleSummary* pRoleSumNew = CRoleSummaryMgr::Instance()->GetRoleSummary(nextLeaderId); 
		if (pRoleSumNew != NULL)
		{
			
			std::vector<ItemDesc> itemVec;
			MailData mail( GetGlobalConfig().GuildActLeaDownTitle, GetGlobalConfig().GuildActLeaDownCon, itemVec, MAIL_EXPIRE_TIME);
			CMailMgr::Instance()->SendMail(pRoleSumOld->GetID(), mail);

			MailData mailTwo( GetGlobalConfig().GuildActLeaUpTitle, GetGlobalConfig().GuildActLeaUpCon, itemVec, MAIL_EXPIRE_TIME);
			CMailMgr::Instance()->SendMail(pRoleSumNew->GetID(), mailTwo);

			m_pGuild->SetLeader(nextLeaderId);
			NoticeMgr::Instance()->NoticeGuildActivityLeader(pRoleSumOld->GetName(), pRoleSumNew->GetName(), m_pGuild->GetID());
			m_pGuild->OnLeaderChange(pRoleSumOld->GetID(), nextLeaderId);
		}
	}
}

UINT64 CGuildActivity::_FindNextLeader()
{
	const std::map<UINT64, GuildMember>& memVec = m_pGuild->GetGuildMember();
	std::map<UINT32, std::multimap<UINT32, UINT64> > memMap;
	foreach (i in memVec)
	{
		if (i->second.position != GPOS_LEADER)
		{
			std::multimap<UINT32, UINT64>& data = memMap[i->second.position];
			data.insert(make_pair(i->second.contribute, i->first));
		}
	}

	foreach (j in memMap)
	{
		for (std::multimap<UINT32, UINT64>::reverse_iterator iter = j->second.rbegin(); iter != j->second.rend(); ++iter)
		{
			CRoleSummary* pRoleSum = CRoleSummaryMgr::Instance()->GetRoleSummary(iter->second); 
			if (pRoleSum != NULL && pRoleSum->GetLastLogout() < GetGlobalConfig().GuildActLeaDay * CGuildActivity::DAY_TIME)
			{
				return iter->second;
			}
		}
	}

	return 0;
}

bool CGuildActivity::_IsOutNewProtTime()
{
	return (GameTime::GetTime() - m_pGuild->GetOpenTime()) > CGuildActivity::WEEK_TIME;
}

void CGuildActivity::AddActivity(UINT64 roleId, UINT32 addNum) 
{
	if (addNum == 0)
	{
		return ;
	}

	m_weekActivity += addNum;
	m_roleActivityMap[roleId] += addNum;
	SetChang();
}




void CGuildActivity::FillActivityRank(KKSG::AskGuildWageInfoRes &refData)
{
	UINT32 nTotal = 0;
	for (size_t i = 0; i < m_vecguildActivity.size(); i++)
	{
		CRoleSummary* pSum	 = CRoleSummaryMgr::Instance()->GetRoleSummary(m_vecguildActivity[i].nRoleID);
		if (pSum)
		{
			KKSG::GuildActivityRole *pActRole = refData.add_roles();
			pActRole->set_name(pSum->GetName());
			pActRole->set_roleid(m_vecguildActivity[i].nRoleID);
			pActRole->set_score(m_vecguildActivity[i].nActivity);
			nTotal++;
			LogDebug("FillActivityRank rolename=%s roleid=%llu", pSum->GetName().c_str(), m_vecguildActivity[i].nRoleID);
			if (nTotal>=GetGlobalConfig().GuildActRank)
			{
				break;
			}
		}

	}
}


void CGuildActivity::DelActivity(UINT64 roleId, UINT32 delNum) 
{ 
	if (delNum == 0)
	{
		return ;
	}

	m_weekActivity = m_weekActivity <= delNum ? 0 : m_weekActivity - delNum ;

	UINT32& activity = m_roleActivityMap[roleId];
	activity = activity > delNum ? (activity - delNum) : 0;
	SetChang();
}

void CGuildActivity::OnAddMember(Guild *pGuild, GuildMember *pMember)
{
	if (pMember != NULL)
	{
		DoRoleAdd(pMember->qwRoleID);
	}
}

void CGuildActivity::OnRemoveMember(Guild *pGuild, UINT64 qwRoleID)
{
	DoRoleLeave(qwRoleID);
}

void CGuildActivity::GetStand(UINT32& activity, UINT32& activityOne, UINT32& activityTwo, UINT32& activityThree)
{
	activity = m_weekActivity;
	activityOne = GetGlobalConfig().GuildActLvl[0] * XCommon::GetWeekNum() * m_pGuild->GetMemberCount();
	activityTwo = GetGlobalConfig().GuildActLvl[1] * XCommon::GetWeekNum() * m_pGuild->GetMemberCount();
	activityThree = GetGlobalConfig().GuildActLvl[2] * XCommon::GetWeekNum() * m_pGuild->GetMemberCount();
}

void CGuildActivity::DoTxLog()
{
	TGuildFlowLog oLog(TLogMgr::Instance()->GetTagString());
	m_pGuild->DoTxLog(oLog);
	oLog.m_iWeekActivity = m_weekActivity;
	oLog.m_iActType = TX_GUILD_ACT_TYPE_9;
	oLog.Do();
}
