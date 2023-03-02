#include "pch.h"
#include "teaminvitemgr.h"
#include "team/ptcg2c_teaminvitenotify.h"
#include "team.h"
#include "unit/role.h"
#include "teamrequest.h"
#include "common/ptcg2c_gserrornotify.h"
#include "unit/rolemanager.h"
#include "table/expeditionconfigmgr.h"
#include "guild/guildmgr.h"
#include "guild/guild.h"
#include "teamhandler.h"
#include "stagemgr.h"
#include "payv2Record.h"
#include "teammgr.h"
#include "teamrecord.h"
#include "define/teamdef.h"
#include "dragonguild/dragonguildmgr.h"

INSTANCE_SINGLETON(TeamInviteMgr)

TeamInviteMgr::TeamInviteMgr()
{

}

TeamInviteMgr::~TeamInviteMgr()
{

}

bool TeamInviteMgr::Init()
{
	m_handler = CTimerMgr::Instance()->SetTimer(this, 0, 2000, -1, __FILE__, __LINE__);
	return true;
}

void TeamInviteMgr::Uninit()
{
	CTimerMgr::Instance()->KillTimer(m_handler);
}

void TeamInviteMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	time_t now = time(NULL);

	for(auto it = m_invToHistory.begin(); it != m_invToHistory.end();)
	{
		(it->second).Update();
		if(0 == (it->second).GetHistoryCount())
		{
			m_invToHistory.erase(it++);
		}
		else
		{
			it++;
		}
	}
}

void TeamInviteMgr::FillInviteList(Role* pRole, int expid, KKSG::TeamInviteRes &roRes)
{
	//这里非常耗

	ExpeditionTable::RowData *pRow = ExpeditionConfigMgr::Instance()->GetExpeditionConf(expid);

	if(NULL == pRow)
	{
		return ;
	}
	// TODO 战队联赛推荐过滤

	std::unordered_set<UINT64> recRole;
	FillInviteFriend(pRole, pRow, roRes, recRole);
	if (pRow->Type != TEAM_TYPE_MARRIAGE)
	{
		FillInviteGuild(pRole, pRow, roRes, recRole);
		FillInviteRec(pRole, pRow, roRes, recRole);
	}
}

void TeamInviteMgr::TestPrint()
{
	SSWarn << " team invite history " << END;
	for(auto it = m_invToHistory.begin(); it != m_invToHistory.end(); ++it)
	{
		SSWarn << "inver ID = " << (it->first) << END;
		(it->second).TestPrint();
	}
}

bool TeamInviteMgr::IsInInvToHistory(UINT64 invFromID, UINT64 invToID)
{
	auto it = m_invToHistory.find(invFromID);
	if(it == m_invToHistory.end())
	{
		return false;
	}
	return (it->second).InInvHistory(invToID);
}

void TeamInviteMgr::AddInvToHistory(UINT64 invFromID, UINT64 invToID)
{
	auto it = m_invToHistory.find(invFromID);
	if(it == m_invToHistory.end())
	{
		InviteHistory invhis( GetGlobalConfig().TeamInviteHistoryTime);
		invhis.AddOneInvHistory(invToID);
		m_invToHistory.insert(std::make_pair(invFromID, invhis));
	}
	else
	{
		(it->second).AddOneInvHistory(invToID);
	}
}

void TeamInviteMgr::FillInviteFriend(Role* pRole, ExpeditionTable::RowData *pRow, KKSG::TeamInviteRes &roRes, std::unordered_set<UINT64>& recRole)
{
	int recNum = 0;

	TeamCopy* pTeam = TeamCopyMgr::Instance()->GetTeamByRoleID(pRole->GetID());
	if(NULL == pTeam)
	{
		return;
	}
	auto pConf = pTeam->GetConf();
	if(NULL == pConf)
	{
		return;
	}
	UINT64 teamgid = pTeam->GetGuildID();

	const std::set<UINT64>* flist = pRole->GetFriendList();
	for(auto it = flist->begin(); it != flist->end(); ++it)
	{
		UINT64 roleID = (*it);
		Role* frole = RoleManager::Instance()->FindByRoleID(roleID);
		if(NULL == frole)
		{
			continue;
		}
		if (pTeam->IsPPTLimit((UINT32)frole->GetAttr(TOTAL_POWERPOINT)))
		{
			continue;
		}
		if(teamgid && teamgid != GuildSimpleMgr::Instance()->GetGuildId(roleID))
		{
			continue;//公会关卡 且不同公会
		}

		KKSG::TeamInvRoleInfo* rinfo = roRes.add_friend_();

		frole->FillTeamInviteRoleInfo(rinfo, pConf->Type);
		rinfo->set_teamguildid(teamgid);
		KKSG::TeamInvRoleState rstate = GetInviteRoleState(pRole, frole, pRow);
		rinfo->set_state(rstate);

		if((KKSG::TIRS_NORMAL == rstate)
			&& recNum < 3 && recRole.find(frole->GetID()) == recRole.end())	
		{
			recRole.insert(frole->GetID());
			++recNum;
		}

		if(roRes.friend__size() >=  GetGlobalConfig().TeamInviteFriendCount)
		{
			break;
		}
	}
}

void TeamInviteMgr::FillInviteGuild(Role* pRole, ExpeditionTable::RowData *pRow, KKSG::TeamInviteRes &roRes, std::unordered_set<UINT64>& recRole)
{
	int recNum = 0;
	CGuildSimple* guild = GuildSimpleMgr::Instance()->GetGuildByRoleId(pRole->GetID());
	if(NULL == guild)
	{
		return ;
	}

	TeamCopy* pTeam = TeamCopyMgr::Instance()->GetTeamByRoleID(pRole->GetID());
	if(NULL == pTeam)
	{
		return;
	}
	auto pConf = pTeam->GetConf();
	if(NULL == pConf)
	{
		return;
	}

	const auto& gmems = guild->getAllMember();
	for(auto it = gmems.begin(); it != gmems.end(); ++it)
	{
		UINT64 memqwRoleID = (it->first);
		if(memqwRoleID == pRole->GetID())
		{
			continue;
		}
		Role* grole = RoleManager::Instance()->FindByRoleID(memqwRoleID);
		if(NULL == grole)
		{
			continue;
		}
		if (pTeam->IsPPTLimit((UINT32)grole->GetAttr(TOTAL_POWERPOINT)))
		{
			continue;
		}

		KKSG::TeamInvRoleInfo* rinfo = roRes.add_guild();
		grole->FillTeamInviteRoleInfo(rinfo, pConf->Type);
		rinfo->set_teamguildid(pTeam->GetGuildID());
		KKSG::TeamInvRoleState rstate = GetInviteRoleState(pRole, grole, pRow);
		rinfo->set_state(rstate);

		if((KKSG::TIRS_NORMAL == rstate)
			&& recNum < 3 && recRole.find(grole->GetID()) == recRole.end())	
		{
			recRole.insert(grole->GetID());
			++recNum;
		}
		if(roRes.guild_size() >=  GetGlobalConfig().TeamInviteGuildCount)
		{
			break;
		}
	}

}

struct TeamInvRolePrio
{
	TeamInvRolePrio()
	{
		pRole = NULL;
		prio = 0;
	}
	Role* pRole;
	int prio;

	bool operator < (const TeamInvRolePrio& other) const
	{
		return prio > other.prio;
	}
};

void TeamInviteMgr::FillInviteRec(Role* pRole, ExpeditionTable::RowData *pRow, KKSG::TeamInviteRes &roRes, std::unordered_set<UINT64>& recRole)
{
	int needcount =  GetGlobalConfig().TeamInviteRecCount - (int)recRole.size();
	if(needcount <= 0)
	{
		return ;
	}
	UINT32 sceneid = GetSampleSceneID(pRow);	
	if(0 == sceneid)
	{
		return ;
	}

	TeamCopy* pTeam = TeamCopyMgr::Instance()->GetTeamByRoleID(pRole->GetID());
	if(NULL == pTeam)
	{
		return;
	}
	auto pConf = pTeam->GetConf();
	if(NULL == pConf)
	{
		return;
	}
	UINT64 teamgid = pTeam->GetGuildID();
	std::priority_queue<TeamInvRolePrio> prioQue;

	int ForLimit = 1000;
	auto itend = RoleManager::Instance()->End();
	for(auto it = RoleManager::Instance()->Begin(); it != itend; ++it)
	{
		if(--ForLimit < 0)
		{
			break;
		}
		Role* rrole = (*it);
		if(rrole->GetID() == pRole->GetID())
		{
			continue;
		}
		if(pRole->IsFamiliar(rrole->GetID()))
		{
			continue;
		}
		if(rrole->IsInBattle())
		{
			continue;
		}
		///> 战力限制
		if (pTeam->IsPPTLimit((UINT32)rrole->GetAttr(TOTAL_POWERPOINT)))
		{
			continue;
		}

		if(teamgid && teamgid != GuildSimpleMgr::Instance()->GetGuildId(rrole->GetID()))
		{
			continue;
		}

		KKSG::TeamInvRoleState rstate = GetInviteRoleState(pRole, rrole, pRow);
		if(KKSG::TIRS_NORMAL != rstate)
		{
			continue;
		}
		if(IsInInvToHistory(pRole->GetID(), rrole->GetID()))
		{
			continue;
		}
		TeamInvRolePrio prio;
		prio.pRole = rrole;
		if(!rrole->Get<StageMgr>()->IsPassed(sceneid))
		{
			prio.prio = 2;
		}
		else
		{
			prio.prio = 1;
		}
		if(rrole->Get<CTeamRecord>()->IsWantHelp(pRow->Type))
		{
			prio.prio += 2;
		}
		if (DragonGuildSimpleMgr::Instance()->IsInSameDragonGuild(rrole->GetID(), pRole->GetID()))
		{
			prio.prio += 4;
		}
		prioQue.push(prio);
		if(int(prioQue.size()) > needcount)
		{
			prioQue.pop();
		}
	}

	while(!prioQue.empty())
	{
		TeamInvRolePrio prio = prioQue.top();
		prioQue.pop();

		Role* rrole = prio.pRole;
		if(rrole)
		{
			KKSG::TeamInvRoleInfo* info = roRes.add_rec();
			rrole->FillTeamInviteRoleInfo(info, pRow->Type);
			info->set_teamguildid(teamgid);
			info->set_state(KKSG::TIRS_NORMAL);
			SSDebug << " team inv role = " << rrole->GetName() << END;
		}
	}
}

KKSG::TeamInvRoleState  TeamInviteMgr::GetInviteRoleState(Role* pRole, Role* inRole, ExpeditionTable::RowData *pRow)
{
	if(NULL == pRole || NULL == inRole || NULL == pRow)
	{
		return KKSG::TIRS_NORMAL;
	}
	KKSG::TeamMemberType inType = KKSG::TMT_NORMAL;
	if(inRole->Get<CTeamRecord>()->IsWantHelp(pRow->Type))
	{
		inType = KKSG::TMT_HELPER;
	}
	TeamEnterData enterData(inType);
	TeamRequest req(inRole);
	int ret = req.CheckEnterCondtion(inRole, pRow->DNExpeditionID, &enterData);
	if(0 != ret)
	{
		if(KKSG::ERR_SCENE_TODYCOUNTLIMIT == ret)
		{
			return KKSG::TIRS_COUNT_LESS;
		}
		if(KKSG::ERR_SCENE_NOFATIGUE == ret)
		{
			return KKSG::TIRS_FATIGUE_LESS;
		}
		//有可能少判断一些条件
		return KKSG::TIRS_NOT_OPEN;
	}
	if(inRole->IsInBattle())
	{
		return KKSG::TIRS_IN_BATTLE;
	}
	int inRoleTeamID = TeamCopyMgr::Instance()->GetTeamIDByRoleID(inRole->GetID());
	int pRoleTeamID = TeamCopyMgr::Instance()->GetTeamIDByRoleID(pRole->GetID());
	if(inRoleTeamID && pRoleTeamID == inRoleTeamID)
	{
		return KKSG::TIRS_IN_MY_TEAM;
	}
	if(inRoleTeamID && pRoleTeamID != inRoleTeamID)
	{
		return KKSG::TIRS_IN_OTHER_TEAM;
	}
	return KKSG::TIRS_NORMAL;
}

InviteHistory::InviteHistory(UINT32 delTime)
{
	m_delTime = delTime;
}

void InviteHistory::Update()
{
	time_t now = time(NULL);
	for(auto it = m_history.begin(); it != m_history.end(); )
	{
		if(it->second + m_delTime <= now)
		{
			m_history.erase(it++);
		}
		else
		{
			it++;
		}
	}
}

void InviteHistory::AddOneInvHistory(UINT64 roleID)
{
	m_history[roleID] = time(NULL);	
	
	if((int)m_history.size() >  GetGlobalConfig().TeamInviteHistoryCount)
	{
		m_history.erase(m_history.begin());
	}
}

bool InviteHistory::InInvHistory(UINT64 roleID)
{
	if(m_history.find(roleID) == m_history.end())
	{
		return false;
	}
	return true;
}

void InviteHistory::TestPrint()
{
	SSWarn << "Invite history " << END;
	for(auto it = m_history.begin(); it != m_history.end(); ++it)
	{
		SSWarn << it->first << " " << it->second << END;
	}
}
