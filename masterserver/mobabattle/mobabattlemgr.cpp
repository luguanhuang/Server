#include "pch.h"
#include "mobabattlemgr.h"
#include "scene/sceneswitch.h"
#include "scene/scenecreate.h"
#include "role/rolemanager.h"
#include "role/rolesummarymgr.h"
#include "util/gametime.h"
#include "team/teammgr.h"
#include "team/team.h"
#include "table/expeditionconfigmgr.h"
#include "team/teamforceop.h"
#include "other/matchhandler.h"
#include "table/globalconfig.h"
#include "network/worldlink.h"
#include "cross/crosszonestatus.h"
#include "util/timespecificmgr.h"
#include "util/XCommon.h"
#include "network/dblink.h"

#define DELAY_TIMEOUT 10

MobaMatchUnit::MobaMatchUnit(UINT64 roleid, int teamid)
{
	if (roleid == 0)
	{
		m_roleid = 0;
		m_teamid = teamid;
	}
	else
	{
		m_roleid = roleid;
		m_teamid = 0;
	}
	eloPoint = GetGlobalConfig().HeroBattleOriginELOPoint;
	belongPos = 0;
	matchExpand = 0;
	expandLastTime = 0;
}

bool MobaMatchUnit::operator == (const MobaMatchUnit& unit) const
{
	if (IsRole() != unit.IsRole()) return false;
	if (IsRole())
	{
		return m_roleid == unit.GetRoleID();
	}
	return m_teamid == unit.GetTeamID();
}

void MobaMatchUnit::GetRoles(std::vector<UINT64>& roles) const
{
	if (IsRole())
	{
		roles.push_back(m_roleid);
		return;
	}
	Team* pTeam = TeamMgr::Instance()->FindTeam(m_teamid);
	if (pTeam != NULL)
	{
		pTeam->GetRoleIDs(roles);
	}
}

UINT32 MobaMatchUnit::GetRoleCount() const
{
	if (IsRole())
	{
		return 1;
	}
	Team* pTeam = TeamMgr::Instance()->FindTeam(m_teamid);
	if (pTeam != NULL)
	{
		return pTeam->GetMemberCount();
	}
	SSError << "do not find team id = " << m_teamid << END;
	return 0;
}


MobaBattleMgr::MobaBattleMgr()
{
	m_groupID = 0;
	m_matchID = 0;
}

MobaBattleMgr::~MobaBattleMgr()
{

}

bool MobaBattleMgr::Init()
{
	m_handler = CTimerMgr::Instance()->SetTimer(this, 0, 1000, -1, __FILE__, __LINE__);	

	ExpeditionTable::RowData* conf = ExpeditionConfigMgr::Instance()->GetExpeditionConf(MOBABATTLE_TEAM_SCENE);
	UINT32 num = conf == NULL ? 4 : conf->PlayerNumber;
	if (!m_oMatchTable.LoadFile("table/HeroBattleMatch.txt"))
	{
		LogWarn("load file table/HeroBattleMatch.txt failed");
		return false;
	}

	m_allMachine.resize(m_oMatchTable.Table.size());	
	for (UINT32 i = 0; i < m_oMatchTable.Table.size(); i++)
	{
		m_allMachine[i].Init(num, MatchMachineType_MobaBattle);
	}
	m_lastMatchTime.resize(m_oMatchTable.Table.size(), time(NULL));	

	return LoadFile();
}

void MobaBattleMgr::Uninit()
{
	CTimerMgr::Instance()->KillTimer(m_handler);
}

bool MobaBattleMgr::CheckFile()
{
	MobaWeekReward tmpWeekReward;
	if (!tmpWeekReward.LoadFile("table/MobaWeekReward.txt"))
	{
		LogWarn("Load MobaWeekReward.txt failed");
		return false;
	}
	return true;
}

bool MobaBattleMgr::LoadFile()
{
	if (!CheckFile())
	{
		return false;
	}

	ClearFile();

	m_oWeekReward.LoadFile("table/MobaWeekReward.txt");

	return true;
}

void MobaBattleMgr::ClearFile()
{
	m_oWeekReward.Clear();
}

MobaWeekRewardConf* MobaBattleMgr::GetMobaWeekRewardConf(UINT32 id)
{
	return m_oWeekReward.GetByid(id);
}

void MobaBattleMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	_CheckExpandMatch();

	time_t nowTime = time(NULL);
	for (UINT32 i = 0; i < m_allMachine.size(); i++)
	{
		UINT32 diffTime;
		if (nowTime >= m_lastMatchTime[i]) diffTime = nowTime - m_lastMatchTime[i];
		else diffTime = m_lastMatchTime[i] - nowTime;
		if (diffTime >= + m_oMatchTable.Table[i]->SearchTime)
		{
			_DoMatch(m_allMachine[i]);
			m_lastMatchTime[i] = nowTime;
		}
	}

	_CheckDelayTimeout();
}

KKSG::ErrorCode MobaBattleMgr::AddMatchUnit(const MobaMatchUnit& unit)
{
	std::vector<UINT64> roles;
	unit.GetRoles(roles);
	for (UINT32 i = 0; i < roles.size(); i++)
	{
		if (_CanAddRoleIn(roles[i])) ///> 还为了判断一个玩家是否在delayRole里
		{
			LogWarn("Add MobaBattle unit failed, role [%llu] is already in", roles[i]);
			return KKSG::ERR_FAILED;
		}
	}

	if (unit.IsTeam())
	{
		Team* pTeam = TeamMgr::Instance()->FindTeam(unit.GetTeamID());
		if (pTeam == NULL || pTeam->GetConf() == NULL)
		{
			return KKSG::ERR_FAILED;
		}
	}
	else
	{
		CRole* pRole = CRoleManager::Instance()->GetByRoleID(unit.GetRoleID());
		if (pRole == NULL)
		{
			LogError("role [%llu] is offline", unit.GetRoleID());
			return KKSG::ERR_FAILED;
		}
	}

	if (_IsUnitIn(unit))
	{
		if (unit.IsTeam()) LogWarn("Add MobaBattle unit failed, team [%d] is already in", unit.GetTeamID());
		else LogWarn("Add MobaBattle unit failed, role [%llu] is already in", unit.GetRoleID());
		return KKSG::ERR_FAILED;
	}

	UINT32 roleCount = unit.GetRoleCount();
	UINT64 newMatchID = NewMatchID();
	UINT32 pos = _ELOPointBelong(unit.eloPoint, roleCount);
	LogDebug("elo point [%.2lf], role count [%u], belongPos [%u]", unit.eloPoint, roleCount, pos);
	if (!m_allMachine[pos].AddMatchUnit(newMatchID, roleCount))
	{
		return KKSG::ERR_FAILED;
	}

	_InsertMatchUnit(newMatchID, pos, unit);

	if (unit.IsTeam()) LogInfo("Team [%d] add in MobaBattle match, elo point [%.2lf], role num [%u]", unit.GetTeamID(), unit.eloPoint, roles.size());
	else LogInfo("role [%llu] add in MobaBattle match, elo point [%.2lf]", unit.GetRoleID(), unit.eloPoint);

	return KKSG::ERR_SUCCESS;
}

void MobaBattleMgr::RemoveMatchUnit(const MobaMatchUnit& unit)
{	
	MobaMatchUnit* pUnit = NULL;
	if (!_IsUnitIn(unit, &pUnit))
	{
		if (unit.IsTeam()) LogWarn("Remove MobaBattle unit failed, team:%d is not in", unit.GetTeamID());
		else LogWarn("Remove MobaBattle unit failed, role [%llu] is not in", unit.GetRoleID());
		return;
	}

	const MobaMatchUnit& matchUnit = *pUnit;

	if (matchUnit.IsTeam()) LogInfo("Team [%d] remove in MobaBattle match", matchUnit.GetTeamID());
	else LogInfo("role [%llu] remove in MobaBattle match", matchUnit.GetRoleID());

	_EraseMatchUnit(matchUnit.matchid);	
}

bool MobaBattleMgr::IsRoleIn(UINT64 roleid)
{
	return _IsUnitIn(MobaMatchUnit(roleid, 0));
}

bool MobaBattleMgr::IsTeamIn(int teamid)
{
	return _IsUnitIn(MobaMatchUnit(0, teamid));
}

void MobaBattleMgr::CreateSceneSucceed(UINT32 groupid, UINT32 sceneid)
{
	auto iter = m_allGroup.find(groupid);
	if (iter == m_allGroup.end())
	{
		LogError("groupid:%u not in allGroup map", groupid);
		return;
	}
	LogInfo("groupID:%u enter MobaBattle", groupid);

	std::vector<UINT64> roles;
	for (auto i = iter->second.fightTeam1.begin(); i != iter->second.fightTeam1.end(); i++)
	{
		(*i).GetRoles(roles);
	}
	for (auto i = iter->second.fightTeam2.begin(); i != iter->second.fightTeam2.end(); i++)
	{
		(*i).GetRoles(roles);
	}

	KKSG::SceneSwitchData oData;
	for (auto i = roles.begin(); i != roles.end(); i++)
	{
		CRole* role = CRoleManager::Instance()->GetByRoleID(*i);
		if (role != NULL) CSceneSwitch::EnterScene(role, sceneid, oData);

		m_delayRole.erase(*i);
	}

	m_allGroup.erase(groupid);
}

void MobaBattleMgr::CreateSceneFailed(UINT32 groupid)
{
	auto iter = m_allGroup.find(groupid);
	if (iter == m_allGroup.end())
	{
		LogError("groupid:%u not in allGroup map", groupid);
		return;
	}
	LogWarn("groupid:%u Create MobaBattle Scene Failed", groupid);

	std::vector<UINT64> roles;
	for (auto i = iter->second.fightTeam1.begin(); i != iter->second.fightTeam1.end(); i++)
	{
		(*i).GetRoles(roles);
	}
	for (auto i = iter->second.fightTeam2.begin(); i != iter->second.fightTeam2.end(); i++)
	{
		(*i).GetRoles(roles);
	}

	for (auto i = roles.begin(); i != roles.end(); i++)
	{
		m_delayRole.erase(*i);
	}

	m_allGroup.erase(groupid);
}

bool MobaBattleMgr::GMChangeNum(UINT32 num)
{
	LogInfo("GM Change MobaBattle match num [%u]", num);
	bool result = false;
	for (auto i = m_allMachine.begin(); i != m_allMachine.end(); i++)
	{
		result = (*i).GMChangeNum(num);
	}
	return result;
}

void MobaBattleMgr::_DoMatch(MatchMachine& machine)
{
	std::vector<std::vector<UINT64>> matchTeam;
	machine.DoMatch(matchTeam);

	for (UINT32 i = 1; i < matchTeam.size(); i += 2)
	{
		UINT32 newID = NewGroupID();
		FightGroup& group = m_allGroup[newID];

		std::string ss1, ss2;	
		group.eloPoint1 = 0;
		int unitCount = 0;
		for (UINT32 j = 0; j < matchTeam[i - 1].size(); j++)
		{
			auto iter = m_allUnit.find(matchTeam[i - 1][j]);
			if (iter == m_allUnit.end())
			{
				LogError("can't find unit in MobaBattle, it's must be some wrong code");
				continue;
			}
			const MobaMatchUnit& matchUnit = iter->second;
			group.fightTeam1.push_back(matchUnit);

			UINT32 roleCount = matchUnit.GetRoleCount();
			group.eloPoint1 += matchUnit.eloPoint * roleCount;
			unitCount += roleCount;

			if (j != 0) ss1 += ","; 
			ss1 += ToString(roleCount);

			_EraseMatchUnit(matchTeam[i - 1][j]); ///> machine里已经删除，要保持一致
		}
		if (unitCount != 0) group.eloPoint1 /= unitCount;
		else
		{
			group.eloPoint1 = GetGlobalConfig().HeroBattleOriginELOPoint;
		}

		group.eloPoint2 = 0;
		unitCount = 0;
		for (UINT32 j = 0; j < matchTeam[i].size(); j++)
		{
			auto iter = m_allUnit.find(matchTeam[i][j]);
			if (iter == m_allUnit.end())
			{
				LogError("can't find unit in MobaBattle, it's must be some wrong code");
				continue;
			}
			const MobaMatchUnit& matchUnit = iter->second;
			group.fightTeam2.push_back(matchUnit);

			UINT32 roleCount = matchUnit.GetRoleCount();
			group.eloPoint2 += matchUnit.eloPoint * roleCount;
			unitCount += roleCount;

			if (j != 0) ss2 += ",";
			ss2 += ToString(roleCount);

			_EraseMatchUnit(matchTeam[i][j]); ///> machine里已经删除，要保持一致
		}
		if (unitCount != 0) group.eloPoint2 /= unitCount;
		else
		{
			group.eloPoint2 = GetGlobalConfig().HeroBattleOriginELOPoint;
		}

		LogInfo("MobaBattle match: (%s) vs (%s), average elo point, %.0lf : %.0lf", ss1.c_str(), ss2.c_str(), group.eloPoint1, group.eloPoint2);

		_CreateBattle(newID);
	}

	if (matchTeam.size() & 1)
	{
		for (UINT32 i = 0; i < matchTeam[matchTeam.size() - 1].size(); i++)
		{
			UINT64 uid = matchTeam[matchTeam.size() - 1][i];
			if (m_allUnit.find(uid) == m_allUnit.end())
			{
				LogError("can't find unit in MobaBattle, it's must be some wrong code");
				continue;
			}
			machine.AddMatchUnit(uid, m_allUnit[uid].GetRoleCount()); ///> 把没有匹配到(单数的最后一个）的玩家加回去
		}
	}
}

void MobaBattleMgr::_InsertMatchUnit(UINT64 matchid, UINT32 belongPos, const MobaMatchUnit& unit)
{
	if (m_allUnit.find(matchid) != m_allUnit.end())
	{
		return;
	}
	MobaMatchUnit& matchunit = m_allUnit[matchid];
	matchunit = unit;
	matchunit.matchid = matchid;
	matchunit.belongPos = belongPos;
	time_t nowTime = time(NULL);
	matchunit.startMatchTime = nowTime;
	matchunit.matchExpand = 0;
	matchunit.expandLastTime = nowTime;

	LogDebug("Insert MobaMatchUnit [%llu]", matchid);
}

void MobaBattleMgr::_EraseMatchUnit(UINT64 matchid)
{
	auto iter = m_allUnit.find(matchid);
	if (iter == m_allUnit.end())
	{
		return;
	}
	m_allUnit.erase(iter);

	_DelMatchUnitInAllMachine(matchid);

	LogDebug("Erase MobaMatchUnit [%llu]", matchid);
}

void MobaBattleMgr::_DelMatchUnitInAllMachine(UINT64 matchid)
{
	for (auto i = m_allMachine.begin(); i != m_allMachine.end(); i++)
	{
		(*i).DelMatchUnit(matchid);
	}
}

UINT32 MobaBattleMgr::_ELOPointBelong(double point, UINT32 roleCount)
{
	if (!GetGlobalConfig().HeroBattleELOTeam.empty())
	{
		if (roleCount > GetGlobalConfig().HeroBattleELOTeam.size())
		{
			roleCount = GetGlobalConfig().HeroBattleELOTeam.size() - 1;
		}
		if (roleCount == 0) roleCount = 1;
		point += GetGlobalConfig().HeroBattleELOTeam[roleCount - 1];
	}
	for (UINT32 i = 0; i < m_oMatchTable.Table.size(); i++)
	{
		HeroBattleMatch::RowData* data = m_oMatchTable.Table[i];
		if ((UINT32)point >= data->PointBlock[0] && (UINT32)point <= data->PointBlock[1])
		{
			return i;
		}
	}
	LogWarn("MobaBattlePoint:%.2lf, not belong to HeroBattleMatch.txt", point);
	if (point > m_oMatchTable.Table[m_oMatchTable.Table.size() - 1]->PointBlock[1])
	{
		return m_oMatchTable.Table.size() - 1;
	}
	return 0;
}

bool MobaBattleMgr::_IsUnitIn(const MobaMatchUnit& unit, MobaMatchUnit** pUnit)
{
	for (auto i = m_allUnit.begin(); i != m_allUnit.end(); i++)
	{
		if (i->second == unit)
		{
			if (pUnit != NULL)
			{
				*pUnit = &i->second;
			}
			return true;
		}
	}
	return false;
}

bool MobaBattleMgr::_CanAddRoleIn(UINT64 roleid)
{
	return IsRoleIn(roleid) || m_delayRole.find(roleid) != m_delayRole.end();
}

void MobaBattleMgr::_CreateBattle(UINT32 groupid)
{
	auto iter = m_allGroup.find(groupid);
	if (iter == m_allGroup.end())
	{
		return;
	}

	KKSG::CreateBattleParam param;
	KKSG::MobaBattleInfo* info = param.mutable_mobabattle();
	info->set_uid(groupid);
	info->set_elopoint1(iter->second.eloPoint1);
	info->set_elopoint2(iter->second.eloPoint2);

	time_t nowTime = time(NULL);

	std::vector<int> teamids;
	std::vector<UINT64> roles;
	for (auto i = iter->second.fightTeam1.begin(); i != iter->second.fightTeam1.end(); i++)
	{
		if ((*i).IsTeam()) teamids.push_back((*i).GetTeamID());

		roles.clear();
		(*i).GetRoles(roles);
		for (auto j = roles.begin(); j != roles.end(); j++)
		{
			_FillRoleInfo(*j, 1, info);
			m_delayRole[*j] = nowTime;
		}

		_MatchSucceed(*i);
	}

	for (auto i = iter->second.fightTeam2.begin(); i != iter->second.fightTeam2.end(); i++)
	{
		if ((*i).IsTeam()) teamids.push_back((*i).GetTeamID());

		roles.clear();
		(*i).GetRoles(roles);
		for (auto j = roles.begin(); j != roles.end(); j++)
		{
			_FillRoleInfo(*j, 2, info);
			m_delayRole[*j] = nowTime;
		}

		_MatchSucceed(*i);
	}

	UINT32 sceneid = ExpeditionConfigMgr::Instance()->GetMobaBattleScene();

	bool isSucceed = CSceneCreator::Instance()->CreateBattleScene(sceneid, &ICreateMobaBattleListener::GlobalCreateMobaBattleListener, param);

	if (!isSucceed)
	{
		LogError("MobaBattle create battle failed, sceneid [%u]", sceneid);
		CreateSceneFailed(groupid);
		return;
	}

	for (UINT32 i = 0; i < teamids.size(); i++)
	{
		TeamForceOp op;
		op.SetInBattleState(teamids[i]);
	}
}

template <typename T>
void MobaBattleMgr::_FillRoleInfo(UINT64 roleid, UINT32 campid, T* info)
{
	CRole* role = CRoleManager::Instance()->GetByRoleID(roleid);
	if (role == NULL)
	{
		return;
	}
	KKSG::PvpRoleInfo* roleinfo = info->add_roleinfo();
	roleinfo->set_roleid(roleid);
	roleinfo->set_camp(campid);
	CRoleSummary* pSummary = CRoleSummaryMgr::Instance()->GetRoleSummary(roleid);
	if (pSummary)
	{
		roleinfo->set_name(pSummary->GetName());
		roleinfo->set_level(pSummary->GetLevel());
		roleinfo->set_profession(pSummary->GetProfession());
		roleinfo->set_military_rank(pSummary->GetMilitaryRank());
	}
}

void MobaBattleMgr::_CheckDelayTimeout()
{
	time_t nowTime = time(NULL);
	for (auto i = m_delayRole.begin(); i != m_delayRole.end(); )
	{
		if (i->second + DELAY_TIMEOUT <= nowTime)
		{
			m_delayRole.erase(i++);
		}
		else i++;
	}
}

void MobaBattleMgr::_CheckExpandMatch()
{
	time_t nowTime = time(NULL);
	for (auto i = m_allUnit.begin(); i != m_allUnit.end(); i++)
	{
		MobaMatchUnit& matchUnit = i->second;
		UINT32 pos = matchUnit.belongPos;
		UINT32 matchExpand = matchUnit.matchExpand;
		if (nowTime >= matchUnit.expandLastTime + m_oMatchTable.Table[pos]->ExpandTime)
		{
			matchUnit.expandLastTime = nowTime;
			if (pos >= matchExpand + 1)
			{
				m_allMachine[pos - matchExpand - 1].AddMatchUnit(matchUnit.matchid, matchUnit.GetRoleCount());
			}
			if (pos + matchExpand + 1 < m_allMachine.size())
			{
				m_allMachine[pos + matchExpand + 1].AddMatchUnit(matchUnit.matchid, matchUnit.GetRoleCount());
			}
			if (pos >= matchExpand + 1 || pos + matchExpand + 1 < m_allMachine.size())
			{
				matchUnit.matchExpand = matchExpand + 1;
				LogDebug("MobaMatchUnit [%llu] expand match [%u]", matchUnit.matchid, matchExpand + 1);
			}
		}
	}
}

void MobaBattleMgr::_MatchSucceed(const MobaMatchUnit& unit)
{
	HMatchHandler handler;
	if (unit.IsRole())
	{
		CRole* pRole = CRoleManager::Instance()->GetByRoleID(unit.GetRoleID());
		if(pRole != NULL)
		{
			handler.Done(pRole, KKSG::KMT_MOBA);
		}
	}
	else
	{
		Team* pTeam = TeamMgr::Instance()->FindTeam(unit.GetTeamID());
		if(pTeam != NULL)
		{
			handler.Done(pTeam, KKSG::KMT_MOBA);
		}
	}
}