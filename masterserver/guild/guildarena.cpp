#include "pch.h"
#include "guild.h"
#include "guildarena.h"
#include "guildmgr.h"
#include "role/rolesummarymgr.h"
#include "role/rolesummary.h"
#include "time.h"
#include "notice/noticemgr.h"
#include "role/rolemanager.h"
#include "table/guildConfig.h"
#include "gmf/guildmatchfightmgr.h"
//#include "mail/mailop.h"
#include "define/maildef.h"
//#include "gamelogic/designationMgr.h"
#include "guild/ptcm2c_synguildarenafightunitnew.h"
#include "guild/ptcm2c_synguildarenabattleinfonew.h"
#include "guild/ptcm2c_noticeguildarenabeginnew.h"
#include "guild/ptcm2c_synguildarenaroleonlinenew.h"
#include "guild/ptcm2c_noticeguildarenanexttime.h"
//#include "mail/mailsender.h"
#include "mail/maildata.h"
#include "mail/mailmgr.h"
#include "../share/util/XCommon.h"
#include "../share/util/gametime.h"
#include "../share/util/XRandom.h"
#include "global/GlobalConfigDBBuild.h"
#include "guild/ptcm2c_synguilintegralstate.h"
#include "guildauct/guildauctmgr.h"
#include "mail/mailconfig.h"

//#include "../util/XCommon.h"
#include "foreach.h"
#include "event/eventmgr.h"


/*
CGuildArenaTeam::CGuildArenaTeam()
{
	m_guildId = 0;
	m_figthRoleMap.clear();
	_InitPos();
}

CGuildArenaTeam::CGuildArenaTeam(UINT64 guildId)
{
	m_guildId = guildId;
	m_figthRoleMap.clear();
	_InitPos();
}

CGuildArenaTeam::~CGuildArenaTeam()
{
}

UINT32 CGuildArenaTeam::Deploy(UINT64 roleId, UINT64 operId, UINT32 pos)
{
	if (pos > GetGlobalConfig().GuildArenaBattleNumber)
	{
		return 0;
	}

	if (!_IsRoleFree(operId))
	{
		return 0;
	}

	if (pos != 0 && !_IsPosFree(pos))
	{
		return 0;
	}

	UINT32 operPos = _GetRolePos(operId);
	if (operPos == pos)
	{
		return 0;
	}

	Guild* pGuild = CGuildMgr::Instance()->GetGuild(m_guildId);
	if (pGuild == NULL)
	{
		return 0;
	}

	if ((operId != roleId || (pos != 0 && m_figthRoleMap[pos].roleId != 0)) && 
		!CGuildConfig::Instance()->HasPermission(pGuild->GetPosition(roleId), GPEM_ADMINISTRATOR_DISTRIBUTION))
	{
		return 0;
	}

	UINT32 warType = CGuildWarMgr::Instance()->GetWarType(m_guildId);
	CGuildArenaBattle* pBattle = CGuildWarMgr::Instance()->GetBattleByGuildId(m_guildId); 
	if (pBattle == NULL || (pBattle->GetState() == CGuildArenaBattle::BATTLE_FINISH && pBattle->GetWinId() != m_guildId))
	{
		return 0;
	}


	if (pos > 0)
	{
		if (operPos > 0)
		{
			m_figthRoleMap[operPos].roleId = m_figthRoleMap[pos].roleId;
			if (warType > 0 && pBattle->GetState() == CGuildArenaBattle::BATTLE_FIGHTING)
			{
				//GuildMatchFightMgr::Instance()->AddNewRole(warType, pBattle->GetBattleId(), m_guildId, m_figthRoleMap[pos].roleId);
			}
		}
		else if (m_figthRoleMap[pos].roleId > 0)  // 移除通知
		{
			NoticeMgr::Instance()->GuildArenaOutWar(m_figthRoleMap[pos].roleId);
		}

		m_figthRoleMap[pos].roleId = operId;
		if (warType > 0 && pBattle->GetState() == CGuildArenaBattle::BATTLE_FIGHTING)
		{
			//GuildMatchFightMgr::Instance()->AddNewRole(warType, pBattle->GetBattleId(), m_guildId, operId);
		}

		if (operPos == 0) // 上阵通知
		{
			NoticeMgr::Instance()->GuildArenaOnWar(operId);  
		}
	}
	else
	{
		if (operPos > 0 && operPos <= GetGlobalConfig().GuildArenaBattleNumber)
		{

			m_figthRoleMap[operPos].roleId = 0;
			if (warType > 0 && pBattle->GetState() == CGuildArenaBattle::BATTLE_FIGHTING)
			{
				//GuildMatchFightMgr::Instance()->AddNewRole(warType, pBattle->GetBattleId(), m_guildId, 0);
			}

			// 移除通知
			NoticeMgr::Instance()->GuildArenaOutWar(operId);
		}
	}

	return 0;
}

void CGuildArenaTeam::SimpleDeploy()
{
	if (!_IsTeamFree())
	{
		return ;
	}

	Guild* pGuild = CGuildMgr::Instance()->GetGuild(m_guildId);
	if (pGuild == NULL)
	{
		return ;
	}

	_ClearPos();  // 先清理阵位信息

	std::multimap<UINT32, UINT64> rolePowerMap;
	const std::vector<GuildMember>& guildMemberVec = pGuild->GetGuildMember();
	foreach (i in guildMemberVec)
	{
		CRoleSummary* pRoleSummary = CRoleSummaryMgr::Instance()->GetRoleSummary(i->qwRoleID);
		if (pRoleSummary != NULL)
		{
			rolePowerMap.insert(std::make_pair(pRoleSummary->GetPowerPoint(), i->qwRoleID));
		}
	}

	UINT32 index = 1;
	for (auto i = rolePowerMap.rbegin(); i != rolePowerMap.rend() && index <= GetGlobalConfig().GuildArenaBattleNumber; ++i, ++index)
	{
		fightUnit& unit = m_figthRoleMap[index];
		unit.pos = index;
		unit.roleId = i->second;
		unit.state = fightUnit::READY;

		NoticeMgr::Instance()->GuildArenaOnWar(i->second);
	}
}

void CGuildArenaTeam::AutoDeploy()
{
	if (!_IsTeamFree())
	{
		return ;
	}

	if (_IsFightRoleMapFull())
	{
		return ;
	}

	Guild* pGuild = CGuildMgr::Instance()->GetGuild(m_guildId);
	if (pGuild == NULL)
	{
		return ;
	}

	std::multimap<UINT32, UINT64> rolePowerMap;
	const std::vector<GuildMember>& guildMemberVec = pGuild->GetGuildMember();
	foreach (i in guildMemberVec)
	{
		CRoleSummary* pRoleSummary = CRoleSummaryMgr::Instance()->GetRoleSummary(i->qwRoleID);
		if (pRoleSummary != NULL && pRoleSummary->GetLastLogout() == 0)
		{
			rolePowerMap.insert(std::make_pair(pRoleSummary->GetPowerPoint(), i->qwRoleID));
		}
	}

	foreach (i in m_figthRoleMap)
	{
		if (i->second.state != fightUnit::READY || i->second.roleId != 0)
		{
			continue;
		}

		for (auto iter = rolePowerMap.rbegin(); iter != rolePowerMap.rend(); ++iter)
		{
			if (!_IsRoleInFightMap(iter->second))
			{
				i->second.roleId = iter->second;
				NoticeMgr::Instance()->GuildArenaOnWar(iter->second);
				break;
			}
		}
	}
}

void CGuildArenaTeam::RenewUnitState()
{
	foreach (i in m_figthRoleMap)
	{
		if (i->second.state != fightUnit::READY)
		{
			i->second.state = fightUnit::READY;
			CGuildWarMgr::Instance()->SynUnitFightState(m_guildId, i->first, (UINT32)fightUnit::READY);
		}
	}
}

void CGuildArenaTeam::LastRenewState()
{
	foreach (i in m_figthRoleMap)
	{
		if (i->second.state == fightUnit::FIGHTING)
		{
			i->second.state = fightUnit::READY;
			CGuildWarMgr::Instance()->SynUnitFightState(m_guildId, i->first, (UINT32)fightUnit::READY);
		}
	}
}

void CGuildArenaTeam::GetFightRole(std::vector<UINT64>& roleVec)
{
	roleVec.clear();
	foreach (i in m_figthRoleMap)
	{
		roleVec.push_back(i->second.roleId);
	}
}

UINT32 CGuildArenaTeam::GetRoleInMap()
{
	UINT32 roleNum = 0;
	foreach (i in m_figthRoleMap)
	{
		if (i->second.roleId != 0)
		{
			++roleNum;
		}
	}
	return roleNum;
}

void CGuildArenaTeam::SetState(UINT32 pos, fightUnit::FIGHT_UNIT_STATE state)
{
	auto i = m_figthRoleMap.find(pos);
	if (i != m_figthRoleMap.end())
	{
		i->second.state = state;
	}
}

bool CGuildArenaTeam::_IsFightRoleMapFull()
{
	foreach (i in m_figthRoleMap)
	{
		if (i->second.roleId == 0)
		{
			return false;
		}
	}

	return true;
}

bool CGuildArenaTeam::_IsRoleInFightMap(UINT64 roleId)
{
	foreach (i in m_figthRoleMap)
	{
		if (i->second.roleId == roleId)
		{
			return true;
		}
	}

	return false;
}

bool CGuildArenaTeam::_IsRoleFree(UINT64 roleId)
{
	foreach (i in m_figthRoleMap)
	{
		if (i->second.roleId == roleId && i->second.state != fightUnit::READY)
		{
			return false;
		}
	}
	return true;
}

bool CGuildArenaTeam::_IsPosFree(UINT32 pos)
{
	auto iter = m_figthRoleMap.find(pos);
	if (iter != m_figthRoleMap.end() && iter->second.state == fightUnit::READY)
	{
		return true;
	}
	return false;
}

bool CGuildArenaTeam::_IsTeamFree()
{
	foreach (i in m_figthRoleMap)
	{
		if (i->second.state != fightUnit::READY)
		{
			return false;
		}
	}
	return true;
}

UINT32 CGuildArenaTeam::_GetRolePos(UINT64 roleId)
{
	foreach (i in m_figthRoleMap)
	{
		if (i->second.roleId == roleId)
		{
			return i->first;
		}
	}

	return 0;
}

void CGuildArenaTeam::_InitPos()
{
	for (UINT32 i = 1; i <= GetGlobalConfig().GuildArenaBattleNumber; ++i)
	{
		fightUnit& unit = m_figthRoleMap[i];
		unit.pos = i;
		unit.roleId = 0;
		unit.state = fightUnit::READY;
	}
}

void CGuildArenaTeam::_ClearPos()
{
	foreach (i in m_figthRoleMap)
	{
		i->second.roleId = 0;
		i->second.state = fightUnit::READY;
	}
}
*/

///////////////////////////

CGuildArenaBattle::CGuildArenaBattle()
{
	m_battleId = 0;
	m_winGuildId = 0;
	m_watchId = 0;
	m_state = BATTLE_READY;
	m_teamMap.clear();
}

CGuildArenaBattle::CGuildArenaBattle(UINT32 battleId)
{
	m_battleId = battleId;
	m_winGuildId = 0;
	m_watchId = 0;
	m_state = BATTLE_READY;
	m_teamMap.clear();
}

CGuildArenaBattle::CGuildArenaBattle(UINT32 battleId, UINT32 pos, UINT64 guildOne)
{
	m_battleId = battleId;
	m_winGuildId = 0;
	m_watchId = 0;
	m_state = BATTLE_READY;
	m_teamMap[pos] = guildOne;
}
CGuildArenaBattle::CGuildArenaBattle(UINT32 battleId, UINT32 posOne, UINT64 guildOne, UINT32 posTwo, UINT64 guildTwo)
{
	m_battleId = battleId;
	m_winGuildId = 0;
	m_watchId = 0;
	m_state = BATTLE_READY;
	m_teamMap[posOne] = guildOne;
	m_teamMap[posTwo] = guildTwo;
}

CGuildArenaBattle::~CGuildArenaBattle()
{
}

UINT64 CGuildArenaBattle::GetLoserId()
{
	if (m_winGuildId == 0)
	{
		return 0;
	}

	foreach (i in m_teamMap)
	{
		if (i->second != m_winGuildId)
		{
			return i->second;
		}
	}

	return 0;
}

UINT32 CGuildArenaBattle::GetFightState()
{
	if (m_state == BATTLE_READY)
	{
		return 0;
	}
	else if (m_state == BATTLE_FINISH)
	{
		return 3;
	}
	else if (m_watchId == 0)
	{
		return 1;
	}
	else
	{
		return 2;
	}
}

bool CGuildArenaBattle::CheckEnd()
{
	if (m_state != BATTLE_FINISH)
	{
		return false;
	}
	return true;
}

/*
CGuildArenaTeam* CGuildArenaBattle::GetWiner()
{
	if (m_winGuildId > 0)
	{
		foreach (i in m_teamMap)
		{
			if (i->second.GetGuildId() == m_winGuildId)
			{
				return &(i->second);
			}
		}
	}

	if (m_teamMap.size() == 1)
	{
		m_winGuildId = m_teamMap.begin()->second.GetGuildId();
		return &(m_teamMap.begin()->second);
	}

	return NULL;
}
*/

bool CGuildArenaBattle::HasGuild(UINT64 guildId)
{
	foreach (i in m_teamMap)
	{
		if (i->second == guildId)
		{
			return true;
		}
	}
	return false;
}

/*
void CGuildArenaBattle::RenewUnitState()
{
	foreach (i in m_teamMap)
	{
		i->second.RenewUnitState();
	}
}
*/

/*
bool CGuildArenaBattle::AutoEndBattle()
{
	if (m_teamMap.size() == 2)
	{
		auto one = m_teamMap.begin();
		auto two = one++;
		if (one->second.GetRoleInMap() == 0 && two->second.GetRoleInMap() != 0)
		{
			m_state = BATTLE_FINISH;
			m_winGuildId = two->second.GetGuildId();
			// SSInfo << "CGuildArenaBattle: auto end id1:" << m_winGuildId << "id2" << one->second.GetGuildId() << "and winerid: "<< m_winGuildId << END; 
			return true;
		}
		if (one->second.GetRoleInMap() != 0 && two->second.GetRoleInMap() == 0)
		{
			m_state = BATTLE_FINISH;
			m_winGuildId = one->second.GetGuildId();
			return true;
		}
		if (one->second.GetRoleInMap() == 0 && two->second.GetRoleInMap() == 0)
		{
			m_state = BATTLE_FINISH;
			m_winGuildId = one->second.GetGuildId();
			return true;
		}
	}

	if (m_teamMap.size() == 1)
	{
		m_state = BATTLE_FINISH;
		m_winGuildId = m_teamMap.begin()->second.GetGuildId();
		return true;
	}

	if (m_teamMap.empty())
	{
		m_state = BATTLE_FINISH;
		m_winGuildId = 0;
		return true;
	}

	return false;
}
*/

void CGuildArenaBattle::AddTeam(UINT32 pos, UINT64 guildId)
{
	if (m_teamMap.size() >= 2)
	{
		return ;
	}

	//CGuildArenaTeam team(guildId);
	m_teamMap[pos] = guildId;
}

/*
void CGuildArenaBattle::AddTeam(UINT32 pos, CGuildArenaTeam& team)
{
	if (m_teamMap.size() >= 2)
	{
		return ;
	}

	m_teamMap[pos] = team;
}
*/


///////////////////////

CGuildArenaWar::CGuildArenaWar()
{
	m_warType = 0;
	m_curTurnSet.clear();
	m_battleMap.clear();
}

CGuildArenaWar::CGuildArenaWar(UINT32 warType)
{
	m_warType = warType;
	m_curTurnSet.clear();
	m_battleMap.clear();
}

CGuildArenaWar::~CGuildArenaWar()
{

}

void CGuildArenaWar::Init(std::map<UINT32, UINT64> guildMap)
{
	if (guildMap.empty())
	{
		return ;
	}

	UINT32 oneIndex = 1; 
	UINT32 twoIndex = CGuildArenaWar::BATTLE_MAX; 
	UINT32 battleIndex = 0;  // 比赛编号
	m_battleMap.clear();

	while (oneIndex < twoIndex)
	{
		if (oneIndex == 1)
		{
			battleIndex = 1;
		}
		else if (oneIndex ==2)
		{
			battleIndex = 3;
		} 
		else if (oneIndex == 3)
		{
			battleIndex = 4;
		}
		else
		{
			battleIndex = 2;
		}

		auto one = guildMap.find(oneIndex);
		auto two = guildMap.find(twoIndex);

		if (one == guildMap.end() && two == guildMap.end())
		{
			m_battleMap[battleIndex] = CGuildArenaBattle(battleIndex);
		}
		else if (one != guildMap.end() && two == guildMap.end())
		{
			m_battleMap[battleIndex] = CGuildArenaBattle(battleIndex, 1, one->second); 
		}
		else if (one == guildMap.end() && two != guildMap.end())
		{
			m_battleMap[battleIndex] = CGuildArenaBattle(battleIndex, 2, two->second); 
		}
		else if (one != guildMap.end() && two != guildMap.end())
		{
			m_battleMap[battleIndex] = CGuildArenaBattle(battleIndex, 1, one->second, 2, two->second); 
		}

		++oneIndex;
		--twoIndex;
	}
}

void CGuildArenaWar::StartNewTurn()
{
	if (_CreateNextTurn())
	{
		_StartNewtTurnBattle();
	}
}

bool CGuildArenaWar::IsWarEnd()
{
	if (m_battleMap.size() < 7)
	{
		return false;
	}
	if (m_battleMap.find(7) != m_battleMap.end() && m_battleMap.find(7)->second.GetState() == CGuildArenaBattle::BATTLE_FINISH)
	{
		return true;
	}
	return false;
}

bool CGuildArenaWar::_CreateNextTurn()
{
	if (m_curTurnSet.empty())  // 首轮初始化
	{
		foreach (i in m_battleMap)
		{
			m_curTurnSet.insert(i->first);
		}
		return true;
	}

	if (m_curTurnSet.size() >= 2)
	{
		std::set<UINT32> newSet;
		for (auto i = m_curTurnSet.begin(); i != m_curTurnSet.end(); ++i)
		{
			auto j = i++;
			auto iterOne = m_battleMap.find(*j);
			auto iterTwo = m_battleMap.end();
			if (i != m_curTurnSet.end())
			{
				iterTwo = m_battleMap.find(*i);
			}

			//CGuildArenaTeam* pWinerOne = NULL;
			//CGuildArenaTeam* pWinerTwo = NULL;
			UINT64 winOneId = 0;
			UINT64 winTwoId = 0;

			if (iterOne != m_battleMap.end())
			{
				if (!iterOne->second.CheckEnd())
				{
					CGuildWarMgr::Instance()->SynBattleInfo(m_warType, iterOne->first);
				}
				//pWinerOne = iterOne->second.GetWiner();
				winOneId = iterOne->second.GetWinId();
			}
			if (iterTwo != m_battleMap.end())
			{
				if (!iterTwo->second.CheckEnd())
				{
					CGuildWarMgr::Instance()->SynBattleInfo(m_warType, iterTwo->first);
				}
				//pWinerTwo = iterTwo->second.GetWiner();
				winTwoId = iterTwo->second.GetWinId();
			}


			UINT32 battleIndex = m_battleMap.size() + 1;
			m_battleMap[battleIndex] = CGuildArenaBattle(battleIndex);
			newSet.insert(battleIndex);

			if (winOneId != 0)
			{
				m_battleMap[battleIndex].AddTeam(1, winOneId);
			}

			if (winTwoId != 0)
			{
				m_battleMap[battleIndex].AddTeam(2, winTwoId);
			}

			if (i == m_curTurnSet.end())
			{
				break;
			}
		}

		m_curTurnSet = newSet;
		//_RenewUnitState(); //恢复战斗单元状态
		return true;
	}

	return false;
}

/*
bool CGuildArenaWar::_CheckAutoEnd(CGuildArenaBattle* pBattle, UINT32 battleId)
{
	if (pBattle == NULL)
	{
		return false;
	}

	if (pBattle->AutoEndBattle()) // 是否自动结束
	{
		UINT64 winId = pBattle->GetWinId();
		UINT64 losId = pBattle->GetLoserId();
		if (battleId < 7)
		{
			GiveReward(CGuildArenaWar::WIN, winId);
			GiveReward(CGuildArenaWar::LOSE, losId);
		}
		else
		{
			GiveReward(CGuildArenaWar::CHA_WIN, winId);
			GiveReward(CGuildArenaWar::CHA_LOSE, losId);
		}
		CGuildWarMgr::Instance()->SynBattleInfo(m_warType, battleId);
		CGuildWarMgr::Instance()->CheckEnd();
		return true;
	}
	return false;
}
*/

void CGuildArenaWar::_StartNewtTurnBattle()
{
	foreach (i in m_curTurnSet)
	{
		auto j = m_battleMap.find(*i); 
		if (j == m_battleMap.end())
		{
			continue;
		}
		
		/*std::map<UINT32, CGuildArenaTeam>& teamMap = j->second.GetTeamMap();
		foreach (k in teamMap)
		{
			k->second.AutoDeploy();  // 自动部署
		}

		if (_CheckAutoEnd(&(j->second), *i)) // 是否自动结束
		{
			continue;
		}*/

		std::map<UINT32, UINT64>& teamMap = j->second.GetTeamMap();

		if (teamMap.size() == 2)
		{
			UINT64 guildIdOne = 0;
			UINT64 guildIdTwo = 0;
			std::vector<UINT64> teamOneVec;
			std::vector<UINT64> teamTwoVec;

			auto iter = teamMap.begin();
			//iter->second.AutoDeploy(); // 
			guildIdOne = iter->second;
			//iter->second.GetFightRole(teamOneVec);

			//iter++;
			//iter->second.AutoDeploy(); // 
			guildIdTwo = (++iter)->second;
			//iter->second.GetFightRole(teamTwoVec);

			j->second.SetState(CGuildArenaBattle::BATTLE_FIGHTING); // 设置状态
			GuildMatchFightMgr::Instance()->AddOneGMFRoom(guildIdOne, guildIdTwo, m_warType, *i);
		}
		else if (teamMap.size() == 1)
		{
			j->second.SetState(CGuildArenaBattle::BATTLE_FINISH);
			j->second.SetWinId(teamMap.begin()->second);

			UINT64 winId = j->second.GetWinId();
			UINT64 losId = j->second.GetLoserId();
			if (j->first < 7)
			{
				GiveReward(CGuildArenaWar::WIN, winId);
				GiveReward(CGuildArenaWar::LOSE, losId);
			}
			else
			{
				GiveReward(CGuildArenaWar::CHA_WIN, winId);
				GiveReward(CGuildArenaWar::CHA_LOSE, losId);
			}
			//CGuildWarMgr::Instance()->SynBattleInfo(m_warType, j->first);
			CGuildWarMgr::Instance()->CheckEnd();
		}
		else
		{
			j->second.SetState(CGuildArenaBattle::BATTLE_FINISH);
			j->second.SetWinId(0);
		}
		CGuildWarMgr::Instance()->SynBattleInfo(m_warType, j->first);
	}
}

/*
void CGuildArenaWar::_RenewUnitState()
{
	foreach (i in m_curTurnSet)
	{
		if (m_battleMap.find(*i) != m_battleMap.end())
		{
			m_battleMap.find(*i)->second.RenewUnitState();
		}
	}
}
*/

void CGuildArenaWar::GiveReward(REWARD_TYPE rewardType, UINT64 guildId)
{
	Guild* pGuild = CGuildMgr::Instance()->GetGuild(guildId);
	if (pGuild == NULL)
	{
		return ;
	}

	std::map<UINT32, UINT32> guildReward;
	std::vector<ItemDesc> itemVec;
	CGuildWarMgr::Instance()->GetReward(m_warType, rewardType, itemVec, guildReward);
	pGuild->AddGuildReward(guildReward); // 增加威望
	std::string mailTitle;
	std::string mailCon;
	if (rewardType == WIN)
	{
		mailTitle = GetGlobalConfig().GuildArenaBattleWinTitle; 
		mailCon = GetGlobalConfig().GuildArenaBattleWinContent; 
	}
	else if (rewardType == LOSE)
	{
		mailTitle = GetGlobalConfig().GuildArenaBattleLoseTitle; 
		mailCon = GetGlobalConfig().GuildArenaBattleLoseContent; 
	}
	else if (rewardType == CHA_WIN)
	{
		mailTitle = GetGlobalConfig().GuildArenaBattleChampionTitle; 
		mailCon = GetGlobalConfig().GuildArenaBattleChampionContent; 
		EventMgr::Instance()->AddGuildEvent(pGuild, DESIGNATION_COM_TYPE_GUILD_GROUP_CHAMPION,m_warType);
	}
	else if (rewardType == CHA_LOSE)
	{
		mailTitle = GetGlobalConfig().GuildArenaBattleSecondTitle; 
		mailCon = GetGlobalConfig().GuildArenaBattleSecondContent; 
	}

	const std::map<UINT64, GuildMember>& guildMember = pGuild->GetGuildMember();
	foreach (i in guildMember)
	{
		MailData mail(mailTitle, mailCon, itemVec, MAIL_EXPIRE_TIME);
		mail.SetReason(ItemFlow_GuildArenaAward);
		CMailMgr::Instance()->SendMail(i->first, mail);
		/*MailSender sender;
		sender.SetContent(Mail_System, mailTitle, mailCon);
		sender.SetItems(itemVec);
		sender.Send(i->qwRoleID);*/
		SSInfo << "Send Mail To Client, MailType = guild arena, " << "RoleID=" << i->first << END; 
	}

	if (rewardType == CHA_WIN)
	{
		itemVec.clear();
		CGuildWarMgr::Instance()->GetReward(m_warType, CHA_LEA_WIN, itemVec, guildReward);
		pGuild->AddGuildReward(guildReward); // 增加威望
		MailData mail(GetGlobalConfig().GuildArenaBattleChampionLeaderTitle, GetGlobalConfig().GuildArenaBattleChampionLeaderContent, itemVec, MAIL_EXPIRE_TIME);
		mail.SetReason(ItemFlow_GuildArenaAward);
		CMailMgr::Instance()->SendMail(pGuild->GetLeaderID(), mail);

		std::set<UINT64> vicLeaderSet;
		pGuild->GetVicLeader(vicLeaderSet);
		foreach (j in vicLeaderSet)
		{
			CMailMgr::Instance()->SendMail(*j, mail);
		}
	}
	if (rewardType == CHA_LOSE)
	{
		itemVec.clear();
		CGuildWarMgr::Instance()->GetReward(m_warType, CHA_LEA_LOSE, itemVec, guildReward);
		pGuild->AddGuildReward(guildReward); // 增加威望
		MailData mail(GetGlobalConfig().GuildArenaBattleSecondLeaderTitle, GetGlobalConfig().GuildArenaBattleSecondLeaderContent, itemVec, MAIL_EXPIRE_TIME);
		mail.SetReason(ItemFlow_GuildArenaAward);
		CMailMgr::Instance()->SendMail(pGuild->GetLeaderID(), mail);

		std::set<UINT64> vicLeaderSet;
		pGuild->GetVicLeader(vicLeaderSet);
		foreach (j in vicLeaderSet)
		{
			CMailMgr::Instance()->SendMail(*j, mail);
		}
	}
}

void CGuildArenaWar::GetAllGuildId(std::set<UINT64>& allGuildSet)
{
	foreach (i in m_curTurnSet)
	{
		auto iter = m_battleMap.find(*i);
		if (iter != m_battleMap.end())
		{
			std::map<UINT32, UINT64>& guildMap = iter->second.GetTeamMap();
			foreach (j in guildMap)
			{
				allGuildSet.insert(j->second);
			}
		}
	}
}

/////////////////////

INSTANCE_SINGLETON(CGuildWarMgr)

CGuildWarMgr::CGuildWarMgr()
{
	m_noticeTimes = 0;
	m_state = TRUCE;
}

CGuildWarMgr::~CGuildWarMgr()
{

}

bool CGuildWarMgr::Init()
{
	if (!m_reward.LoadFile("table/GuildArenaBattleReward.txt"))
	{
		SSWarn<<"load file table/GuildArenaBattleReward.txt failed"<<END;
		return false;
	}
	m_timehandler = CTimerMgr::Instance()->SetTimer(this, 0, 1000, -1, __FILE__, __LINE__);
	return true;
}

void CGuildWarMgr::Uninit()
{
	CTimerMgr::Instance()->KillTimer(m_timehandler);
}

void CGuildWarMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	if (CGuildArenaIntegralMgr::Instance()->GetWeekNum() != 2) // 双周
	{
		return ;
	}

	// 公告
	if (_IsTime(NOTICE_ONE) || _IsTime(NOTICE_TWO) || _IsTime(NOTICE_THREE))
	{
		_NoticeReady();
	}

	//跑马灯
	if (_IsTime(LAMP_ONE) || _IsTime(LAMP_TWO) || _IsTime(LAMP_THREE))
	{
		_NoticeInit();
	}

	switch (m_state)
	{
	case (READY):
		{
			m_noticeTimes = 0;
			_BattleBeforeNotice(WAR_ONE_START);
			if (_IsTime(WAR_ONE_START))
			{
				m_state = WAR_ONE_START; 
				_DoNewBattleTurn();
				_NoticeWarBegin(); // icon推送
				_EnterSceneNotice(WAR_ONE_START); // 入场公告
				SSInfo << "guild arena READY !" << END;
			}
		}
		break;
	case (WAR_ONE_START):
		{
			_BattleBeforeNotice(WAR_TWO_START);
			CheckNextNotice(WAR_TWO_START);
			if (_IsTime(WAR_TWO_START))
			{
				m_state = WAR_TWO_START; 
				_DoNewBattleTurn();
				_NoticeWarBegin(); // icon推送
				_EnterSceneNotice(WAR_TWO_START); // 入场公告
				SSInfo << "guild arena WAR_ONE_START !" << END;
			}
		}
		break;
	case (WAR_TWO_START):
		{
			_BattleBeforeNotice(WAR_FINAL_START);
			CheckNextNotice(WAR_FINAL_START);	
			if (_IsTime(WAR_FINAL_START))
			{
				m_state = WAR_FINAL_START; 
				_DoNewBattleTurn();
				_NoticeWarBegin(); // icon推送
				_EnterSceneNotice(WAR_FINAL_START); // 入场公告
				SSInfo << "guild arena WAR_TWO_START !" << END;
			}
		}
		break;
	case (WAR_FINAL_START):
		{
			CheckNextNotice(WAR_FINAL_START);
			if (_IsTime(WAR_END))
			{
				_TimeOut();
				SSInfo << "guild arena WAR_FINAL_START !" << END;
			}
		}
		break;

	default:
		break;
	}
}

void CGuildWarMgr::CheckNextNotice(UINT32 type)
{
	UINT32 nowTime = GameTime::GetTime();
	UINT32 fightTime = 0;
	if (type == WAR_ONE_START)
	{
		fightTime = GetFightTime(GUILD_ARENA_BATTLE_ONE);
		if (nowTime > fightTime && (nowTime - fightTime) == GetGlobalConfig().GuildArenaDelayTime * 60)
		{
			_NoticeNextState(GUILD_ARENA_BATTLE_TWO);
		}
	}
	if (type == WAR_TWO_START)
	{
		fightTime = GetFightTime(GUILD_ARENA_BATTLE_TWO);
		if (nowTime > fightTime && (nowTime - fightTime) == GetGlobalConfig().GuildArenaDelayTime * 60)
		{
			_NoticeNextState(GUILD_ARENA_BATTLE_FINAL);
		}
	}
	if (type == WAR_FINAL_START)
	{
		fightTime = GetFightTime(GUILD_ARENA_BATTLE_FINAL);
		if (nowTime > fightTime && (nowTime - fightTime) == GetGlobalConfig().GuildArenaDelayTime * 60)
		{
			_NoticeNextState(GUILD_ARENA_END);
		}
	}


}

void CGuildWarMgr::GetAllGuildId(std::set<UINT64>& allGuildSet)
{
	foreach (i in m_arenaMap)
	{
		i->second.GetAllGuildId(allGuildSet);
	}
}

void CGuildWarMgr::_DoNewBattleTurn()
{
	foreach (i in m_arenaMap)
	{
		i->second.StartNewTurn();
	}
}

void CGuildWarMgr::BattleFinish(UINT32 warType, UINT32 battleId, UINT64 winId)
{
	_SetBattleWiner(warType, battleId, winId);
	SynBattleInfo(warType, battleId);
	_BattleEndNotice(warType, battleId);
	CheckEnd();
}

/*
CGuildArenaTeam* CGuildWarMgr::GetTeam(UINT64 guildId)
{
	foreach (i in m_arenaMap)
	{
		std::map<UINT32, CGuildArenaBattle>& battleMap = i->second.GetBattleMap(); 
		for (auto j = battleMap.rbegin(); j != battleMap.rend(); ++j)
		{
			foreach (k in j->second.GetTeamMap())
			{
				if (k->second.GetGuildId() == guildId)
				{
					return &(k->second);
				}
			}
		}
	}

	return NULL;
}
*/

void CGuildWarMgr::_InitArenaWar()
{
	m_allGuildSet.clear();
	m_arenaMap.clear();

	std::vector<Guild *>& pGuildVec = CGuildMgr::Instance()->GetSorter().GetSortResult(KKSG::GuildSortByPrestige);

	auto iter = pGuildVec.begin();
	for (UINT32 warType = 1; warType <= WAR_MAX && iter != pGuildVec.end(); ++warType)
	{
		std::map<UINT32, UINT64> guildMap;
		for (UINT32 j = 1; j <= CGuildArenaWar::BATTLE_MAX && iter != pGuildVec.end(); ++j, ++iter)
		{
			guildMap[j] = (*iter)->GetID();
			m_allGuildSet.insert((*iter)->GetID()); // 所有参赛工会id
		}

		if (guildMap.empty())
		{
			break;
		}

		CGuildArenaWar war(warType);
		war.Init(guildMap);  // 对战列表初始化
		m_arenaMap[warType] = war;
	}
}

void CGuildWarMgr::CreateBattle(std::vector<UINT64>& guildVec)
{
	m_allGuildSet.clear();
	m_arenaMap.clear();

	auto iter = guildVec.begin();
	std::map<UINT32, UINT64> guildMap;
	for (UINT32 j = 1; j <= CGuildArenaWar::BATTLE_MAX && iter != guildVec.end(); ++j, ++iter)
	{
		guildMap[j] = *iter;
		m_allGuildSet.insert(*iter); // 所有参赛工会id
	}

	if (guildMap.empty())
	{
		return ;
	}

	CGuildArenaWar war(1);
	war.Init(guildMap);  // 对战列表初始化
	m_arenaMap[1] = war;

	_NoticeInit(); // 对战列表公告
	m_state = READY;  //
}

/*
void CGuildWarMgr::FightUnitBegin(UINT64 guildId, UINT32 pos)
{
	CGuildArenaTeam* pTeam = GetTeam(guildId);
	if (pTeam != NULL)
	{
		pTeam->SetState(pos, fightUnit::FIGHTING);
		SynUnitFightState(guildId, pos, fightUnit::FIGHTING);
	}

}
*/

/*
void CGuildWarMgr::FightUnitLose(UINT64 guildId, UINT32 pos)
{
	CGuildArenaTeam* pTeam = GetTeam(guildId);
	if (pTeam != NULL)
	{
		pTeam->SetState(pos, fightUnit::DIE);
		SynUnitFightState(guildId, pos, fightUnit::DIE);
	}
}
*/

/*
void CGuildWarMgr::Deploy(UINT64 guildId, UINT64 roleId, UINT64 optId, UINT32 pos)
{
	CGuildArenaTeam* pTeam = GetTeam(guildId);
	if (pTeam != NULL)
	{
		pTeam->Deploy(roleId, optId, pos);
	}
}
*/

bool CGuildWarMgr::_IsTime(WAR_STATE stateType)
{
	time_t newTime = GameTime::GetTime();
	UINT32 weekNum = XCommon::GetWeekNum(newTime);

	//foreach (i in  GetGlobalConfig().GuildArenaTimeVec)
	//{

	if (GetGlobalConfig().GuildArenaFinalNotice.seq[0] == weekNum || GetGlobalConfig().GuildArenaFinalNotice.seq[0] == 0)
	{
	}
		if (GetGlobalConfig().GuildArenaFinal.seq[0] == weekNum || GetGlobalConfig().GuildArenaFinal.seq[0] == 0)
		{
			switch( stateType)
			{
			/*case (READY):
				{
					time_t beginTime = XCommon::MakeTime(newTime, i->seq[1], i->seq[2], 0);
					time_t warOneTime = XCommon::MakeTime(newTime, i->seq[3], i->seq[4], 0);
					if (newTime >= beginTime && newTime < warOneTime)
					{
						return true;
					}
				} break;*/
			case (NOTICE_ONE):
				{
					time_t beginTime = XCommon::MakeTime(newTime, GetGlobalConfig().GuildArenaFinalNotice.seq[1] / 100, GetGlobalConfig().GuildArenaFinalNotice.seq[1] % 100, 0);
					if (newTime == beginTime)
					{
						return true;
					}
				} break;
			case (NOTICE_TWO):
				{
					time_t beginTime = XCommon::MakeTime(newTime, GetGlobalConfig().GuildArenaFinalNotice.seq[2] / 100, GetGlobalConfig().GuildArenaFinalNotice.seq[2] % 100, 0);
					if (newTime == beginTime)
					{
						return true;
					}
				} break;
			case (NOTICE_THREE):
				{
					time_t beginTime = XCommon::MakeTime(newTime, GetGlobalConfig().GuildArenaFinalNotice.seq[3] / 100, GetGlobalConfig().GuildArenaFinalNotice.seq[3] % 100, 0);
					if (newTime == beginTime)
					{
						return true;
					}
				} break;
			case (LAMP_ONE):
				{
					time_t beginTime = XCommon::MakeTime(newTime, GetGlobalConfig().GuildArenaFinalLamp.seq[1] / 100, GetGlobalConfig().GuildArenaFinalLamp.seq[1] % 100, 0);
					if (newTime == beginTime)
					{
						return true;
					}
				} break;
			case (LAMP_TWO):
				{
					time_t beginTime = XCommon::MakeTime(newTime, GetGlobalConfig().GuildArenaFinalLamp.seq[2] / 100, GetGlobalConfig().GuildArenaFinalLamp.seq[2] % 100, 0);
					if (newTime == beginTime)
					{
						return true;
					}
				} break;
			case (LAMP_THREE):
				{
					time_t beginTime = XCommon::MakeTime(newTime, GetGlobalConfig().GuildArenaFinalLamp.seq[3] / 100, GetGlobalConfig().GuildArenaFinalLamp.seq[3] % 100, 0);
					if (newTime == beginTime)
					{
						return true;
					}
				} break;
				/*
			case (NOTICE_START):
				{
					time_t beginTime = XCommon::MakeTime(newTime, i->seq[7], i->seq[8], 0);
					time_t warOneTime = XCommon::MakeTime(newTime, i->seq[9], i->seq[10], 0);
					if (newTime >= beginTime && newTime < warOneTime)
					{
						return true;
					}
				} break;
				*/
			case (WAR_ONE_START):
				{
					time_t beginTime = XCommon::MakeTime(newTime, GetGlobalConfig().GuildArenaFinal.seq[1] / 100, GetGlobalConfig().GuildArenaFinal.seq[1] % 100, 0);
					time_t warOneTime = XCommon::MakeTime(newTime, GetGlobalConfig().GuildArenaFinal.seq[2] / 100, GetGlobalConfig().GuildArenaFinal.seq[2] % 100, 0);
					if (newTime >= beginTime && newTime < warOneTime)
					{
						return true;
					}
				} break;
			case (WAR_TWO_START):
				{
					time_t beginTime = XCommon::MakeTime(newTime, GetGlobalConfig().GuildArenaFinal.seq[2] / 100, GetGlobalConfig().GuildArenaFinal.seq[2] % 100, 0);
					time_t warOneTime = XCommon::MakeTime(newTime, GetGlobalConfig().GuildArenaFinal.seq[3] / 100, GetGlobalConfig().GuildArenaFinal.seq[3] % 100, 0);
					if (newTime >= beginTime && newTime < warOneTime)
					{
						return true;
					}
				} break;
			case (WAR_FINAL_START):
				{
					time_t beginTime = XCommon::MakeTime(newTime, GetGlobalConfig().GuildArenaFinal.seq[3] / 100, GetGlobalConfig().GuildArenaFinal.seq[3] % 100, 0);
					time_t endTime = XCommon::MakeTime(newTime, GetGlobalConfig().GuildArenaFinal.seq[4] / 100, GetGlobalConfig().GuildArenaFinal.seq[4] % 100, 0);
					if (newTime >= beginTime && newTime < endTime)
					{
						return true;
					}
				} break;
			case (WAR_END):
				{
					time_t endTime = XCommon::MakeTime(newTime, GetGlobalConfig().GuildArenaFinal.seq[4] / 100, GetGlobalConfig().GuildArenaFinal.seq[4] % 100, 0);
					if (newTime >= endTime)
					{
						return true;
					}
				}
			default:
				{

				}
			}
		}
	//}

	return false;
}

void CGuildWarMgr::_SetBattleWiner(UINT32 warType, UINT32 battleId, UINT64 winId)
{
	auto i = m_arenaMap.find(warType);
	if (i != m_arenaMap.end())
	{
		auto j = i->second.GetBattleMap().find(battleId);
		if (j != i->second.GetBattleMap().end())
		{

			if (j->second.GetState() == CGuildArenaBattle::BATTLE_FIGHTING)
			{
				j->second.SetWinId(winId);
			}
			j->second.SetState(CGuildArenaBattle::BATTLE_FINISH);
			j->second.clearWathId();

			/*
			std::map<UINT32, CGuildArenaTeam>& teamMap = j->second.GetTeamMap(); 
			foreach (k in teamMap)
			{
				if (k->second.GetGuildId() == winId)
				{
					if (battleId < 7)
					{
						k->second.LastRenewState();  // 恢复冠军状态
					}
					else
					{
						k->second.RenewUnitState();  // 战斗结束恢复状态
					}
				}
			}
			*/

			if (battleId < 7)
			{
				i->second.GiveReward(CGuildArenaWar::WIN, winId);
			}
			else
			{
				i->second.GiveReward(CGuildArenaWar::CHA_WIN, winId);
			}

			UINT64 loserId = j->second.GetLoserId();
			if (battleId < 7)
			{
				i->second.GiveReward(CGuildArenaWar::LOSE, loserId);
			}
			else
			{
				i->second.GiveReward(CGuildArenaWar::CHA_LOSE, loserId);
			}

			Guild* pGuildOne = CGuildMgr::Instance()->GetGuild(winId);
			Guild* pGuildTwo = CGuildMgr::Instance()->GetGuild(loserId); 
			if (battleId == 7 && pGuildOne != NULL && pGuildTwo != NULL)
			{
				CGuildArenaIntegralMgr::Instance()->AddHistoryRecord(pGuildOne->GetName(), pGuildTwo->GetName());
			}
			if (battleId == 7 && pGuildOne != NULL)
			{
				UINT64 first = pGuildOne->GetID();
				UINT64 second = pGuildTwo == NULL ? 0 : pGuildTwo->GetID();
				_GiveAuctReward(first, second);
			}
			if (battleId == 7)
			{
				if (pGuildOne != NULL)
				{
					EventMgr::Instance()->AddEventOnlyOnMS(pGuildOne->GetLeaderID(), BONUS_EVENT_OFFSET + GUILDBONUS_COM_TYPE_GUILDGMF_RANK,1);
				}
				if (pGuildTwo != NULL)
				{
					EventMgr::Instance()->AddEventOnlyOnMS(pGuildTwo->GetLeaderID(), BONUS_EVENT_OFFSET + GUILDBONUS_COM_TYPE_GUILDGMF_RANK,2);
				}

				foreach (i in m_allGuildSet)
				{
					if (*i != winId && *i != loserId)
					{
						Guild* tempGuild = CGuildMgr::Instance()->GetGuild(*i);
						if (!tempGuild)
						{
							continue;
						}
						EventMgr::Instance()->AddEventOnlyOnMS(tempGuild->GetLeaderID(), BONUS_EVENT_OFFSET + GUILDBONUS_COM_TYPE_GUILDGMF_RANK,3);
					}
				}
			}
		}
	}
}

void CGuildWarMgr::_NoticeInit()
{
	NoticeMgr::Instance()->NoticeGuildArenaInit();
}

void CGuildWarMgr::_NoticeReady()
{
	foreach (i in m_allGuildSet)
	{
		NoticeMgr::Instance()->NoticeGuildArenaReady(*i);
	}
}

/*
void CGuildWarMgr::_NoticeStart()
{
	NoticeMgr::Instance()->NoticeGuildArenaStart();
}
*/

bool CGuildWarMgr::CheckEnd()
{
	foreach (i in m_arenaMap)
	{
		if (!i->second.IsWarEnd())
		{
			return false;
		}
	}

	if (m_state != TRUCE)
	{
		m_state = TRUCE;
		_NoticeWarEnd();

		CGuildArenaBattle* pBattle = GetBattleByBattleId(1, 7);
		if (pBattle != NULL)
		{
			UINT64 guildId = pBattle->GetWinId();
			NoticeMgr::Instance()->NoticeGuildArenaWiner(guildId, GuildArenaWiner);
		}
		pBattle = GetBattleByBattleId(2, 7);
		if (pBattle != NULL)
		{
			UINT64 guildId = pBattle->GetWinId();
			NoticeMgr::Instance()->NoticeGuildArenaWiner(guildId, GuildArenaWinerTwo);
		}
		pBattle = GetBattleByBattleId(3, 7);
		if (pBattle != NULL)
		{
			UINT64 guildId = pBattle->GetWinId();
			NoticeMgr::Instance()->NoticeGuildArenaWiner(guildId, GuildArenaWinerThree);
		}
	}

	return true;
}

void CGuildWarMgr::GetReward(UINT32 warType, UINT32 rewardType, std::vector<ItemDesc>& itemVec, std::map<UINT32, UINT32>& guildReward)
{
	foreach (i in m_reward.Table)
	{
		if ((*i)->ID == warType && (*i)->type == rewardType)
		{
			foreach (j in (*i)->reward)
			{
				ItemDesc item;
				item.itemID = (*j)[0];
				item.itemCount = (*j)[1];
				itemVec.push_back(item);
			}

			foreach (k in (*i)->guildreward)
			{
				guildReward[k->seq[0]] = k->seq[1];
			}
		}
	}
}

/*
void CGuildWarMgr::SynUnitFightState(UINT64 guildId, UINT32 pos, UINT32 state)
{
	Guild* pGuild = CGuildMgr::Instance()->GetGuild(guildId);
	if (pGuild == NULL)
	{
		return ;
	}
	PtcM2C_SynGuildArenaFightUnitNew ntf;
	ntf.m_Data.set_pos(pos);
	ntf.m_Data.set_state(state);
	pGuild->BroadCast(ntf);
}
*/

void CGuildWarMgr::SynBattleInfo(UINT32 warType, UINT32 battleId)
{

	auto i = m_arenaMap.find(warType);
	if (i != m_arenaMap.end())
	{
		std::map<UINT32, CGuildArenaBattle>& battleMap = i->second.GetBattleMap();
		auto j = battleMap.find(battleId);
		if (j != battleMap.end())
		{
			PtcM2C_SynGuildArenaBattleInfoNew ntf;
			ntf.m_Data.set_wartype(warType);
			ntf.m_Data.set_timestate(GetTimeState());
			ntf.m_Data.set_state(j->second.GetFightState());
			KKSG::GuildArenaGroupData* pData = ntf.m_Data.add_arenabattleinfo();
			pData->set_warstate(j->second.GetState());
			pData->set_watchid(j->second.GetWatchId());
			pData->set_winerid(j->second.GetWinId());
			pData->set_battleid(battleId);
			pData->set_state(j->second.GetFightState());
			std::map<UINT32, UINT64>& teamMap = j->second.GetTeamMap();
			if (teamMap.find(1) != teamMap.end())
			{
				pData->set_guildoneid(teamMap.find(1)->second);
			}
			if (teamMap.find(2) != teamMap.end())
			{
				pData->set_guildtwoid(teamMap.find(2)->second);
			}

			for (auto iter = m_showRoleSet.begin(); iter != m_showRoleSet.end(); )
			{
				CRole* pRole = CRoleManager::Instance()->GetByRoleID(*iter);
				if (pRole == NULL)
				{
					m_showRoleSet.erase(iter++);
				}
				else
				{
					pRole->Send(ntf);
					++iter;
				}
			}
		}
	}

}

KKSG::GuildArenaState CGuildWarMgr::GetTimeState()
{
	time_t newTime = GameTime::GetTime();
	UINT32 weekNum = XCommon::GetWeekNum(newTime);

	foreach (i in  GetGlobalConfig().GuildArenaTimeVec)
	{
		if (i->seq[0] != weekNum && i->seq[0] != 0)
		{
			continue;
		}
		if (newTime < XCommon::MakeTime(newTime, i->seq[1], i->seq[2], 0))
		{
			return KKSG::GUILD_ARENA_NOT_BEGIN;
		}
		else if (newTime < XCommon::MakeTime(newTime, i->seq[7], i->seq[8], 0))
		{
			return KKSG::GUILD_ARENA_BEGIN;
		}
		else if (newTime < XCommon::MakeTime(newTime, i->seq[9], i->seq[10], 0))
		{
			if (newTime > XCommon::MakeTime(newTime, i->seq[7], i->seq[8], 0) 
				&& (newTime - XCommon::MakeTime(newTime, i->seq[7], i->seq[8], 0)) >= GetGlobalConfig().GuildArenaDelayTime * 60)
			{
				return GUILD_ARENA_BATTLE_TWO; 
			}
			return KKSG::GUILD_ARENA_BATTLE_ONE;
		}
		else if (newTime < XCommon::MakeTime(newTime, i->seq[11], i->seq[12], 0))
		{
			if (newTime > XCommon::MakeTime(newTime, i->seq[9], i->seq[10], 0) 
				&& (newTime - XCommon::MakeTime(newTime, i->seq[9], i->seq[10], 0)) >= GetGlobalConfig().GuildArenaDelayTime * 60)
			{
				return GUILD_ARENA_BATTLE_FINAL; 
			}
			return KKSG::GUILD_ARENA_BATTLE_TWO;
		}
		else
		{
			if (!CheckEnd())
			{
				if (newTime > XCommon::MakeTime(newTime, i->seq[11], i->seq[12], 0) 
					&& (newTime - XCommon::MakeTime(newTime, i->seq[11], i->seq[12], 0)) >= GetGlobalConfig().GuildArenaDelayTime * 60)
				{
					return GUILD_ARENA_END; 
				}
				return KKSG::GUILD_ARENA_BATTLE_FINAL;
			}
			else
			{
				return KKSG::GUILD_ARENA_END;
			}
		}
	}

	return KKSG::GUILD_ARENA_NOT_BEGIN;
}

void CGuildWarMgr::_TimeOut()
{
	foreach (i in m_arenaMap)
	{
		foreach (j in i->second.GetBattleMap())
		{
			j->second.SetState(CGuildArenaBattle::BATTLE_FINISH);
		}
	}

	if (m_state != TRUCE)
	{
		m_state = TRUCE;
		_NoticeWarEnd();
	}
}

UINT32 CGuildWarMgr::GetWarType(UINT64 guildId)
{
	foreach (i in m_arenaMap)
	{
		std::map<UINT32, CGuildArenaBattle>& battleMap = i->second.GetBattleMap();
		foreach (j in battleMap)
		{
			std::map<UINT32, UINT64>& teamMap = j->second.GetTeamMap();
			foreach (k in teamMap)
			{
				if (k->second == guildId)
				{
					return i->first; 
				}
			}
		}
	}

	return 0;
}

void CGuildWarMgr::_NoticeWarBegin()
{
	PtcM2C_NoticeGuildArenaBeginNew notice;
	notice.m_Data.set_isstart(true);

	std::set<UINT64> allGuildSet;
	GetAllGuildId(allGuildSet);
	foreach (i in allGuildSet)
	{
		Guild* pGuild = CGuildMgr::Instance()->GetGuild(*i);
		if (pGuild != NULL)
		{
			const std::map<UINT64, GuildMember>& memberMap = pGuild->GetGuildMember();
			foreach (j in memberMap)
			{
				CRole* pRole = CRoleManager::Instance()->GetByRoleID(j->first);
				if (pRole != NULL)
				{
					pRole->Send(notice);
				}
			}
		}
	}
}

void CGuildWarMgr::_NoticeWarEnd()
{
	PtcM2C_NoticeGuildArenaBeginNew notice;
	notice.m_Data.set_isstart(false);

	foreach (i in m_allGuildSet)
	{
		Guild* pGuild = CGuildMgr::Instance()->GetGuild(*i);
		if (pGuild != NULL)
		{
			const std::map<UINT64, GuildMember>& memberMap = pGuild->GetGuildMember();
			foreach (j in memberMap)
			{
				CRole* pRole = CRoleManager::Instance()->GetByRoleID(j->first);
				if (pRole != NULL)
				{
					pRole->Send(notice);
				}
			}
		}
	}
}

void CGuildWarMgr::SetBattleWatchId(UINT32 warType, UINT32 battleId, UINT32 watchId)
{
	CGuildArenaBattle* pBattle = GetBattleByBattleId(warType, battleId);
	if (pBattle != NULL && pBattle->GetState() == CGuildArenaBattle::BATTLE_FIGHTING)
	{
		pBattle->SetWatchId(watchId);
		SynBattleInfo(warType, battleId);
		if (battleId >= 1 && battleId <= 4 && m_noticeTimes <= 0)
		{
			NoticeMgr::Instance()->NoticeGuildArenaBattleOne();
			++m_noticeTimes;
		}
		else if (battleId >= 5 && battleId <= 6 && m_noticeTimes <= 1)
		{
			NoticeMgr::Instance()->NoticeGuildArenaBattleTwo();
			++m_noticeTimes;
		}
		else if (battleId == 7 && m_noticeTimes <= 2)
		{
			NoticeMgr::Instance()->NoticeGuildArenaBattleFin();
			++m_noticeTimes;
		}
	}
}

CGuildArenaBattle* CGuildWarMgr::GetBattleByBattleId(UINT32 warType, UINT32 battleId)
{
	auto i = m_arenaMap.find(warType);
	if (i != m_arenaMap.end())
	{
		auto j = i->second.GetBattleMap().find(battleId);
		if (j != i->second.GetBattleMap().end())
		{
			return &(j->second);
		}
	}
	return NULL;
}

CGuildArenaBattle* CGuildWarMgr::GetBattleByGuildId(UINT64 guildId)
{
	foreach (i in m_arenaMap)
	{
		std::map<UINT32, CGuildArenaBattle>& battleMap = i->second.GetBattleMap();
		for (std::map<UINT32, CGuildArenaBattle>::reverse_iterator iter = battleMap.rbegin(); iter != battleMap.rend(); ++iter)
		{
			if (iter->second.HasGuild(guildId))
			{
				return &(iter->second);
			}
		}
	}
	return NULL;
}

void CGuildWarMgr::CheckPlayerOnline(CRole* pRole)
{
	if (pRole == NULL)
	{
		return ;
	}

	Guild* pGuild = CGuildMgr::Instance()->GetGuildByRoleId(pRole->GetID());
	if (pGuild == NULL)
	{
		return ;
	}

	if (m_allGuildSet.find(pGuild->GetID()) == m_allGuildSet.end())
	{
		return ;
	}

	//KKSG::GuildArenaState state = GetTimeState();
	if (m_state == WAR_ONE_START || m_state == WAR_TWO_START || m_state == WAR_FINAL_START)
	{
		std::set<UINT64> allGuildSet;
		GetAllGuildId(allGuildSet);

		if (allGuildSet.find(pGuild->GetID()) != allGuildSet.end())
		{
			PtcM2C_NoticeGuildArenaBeginNew notice;
			notice.m_Data.set_isstart(true);
			pRole->Send(notice);
		}
	}
}

void CGuildWarMgr::_BattleEndNotice(UINT32 warType, UINT32 battleId)
{

	CGuildArenaBattle* pBattle = GetBattleByBattleId(warType, battleId);
	if (pBattle == NULL || pBattle->GetWinId() <= 0)
	{
		return ;
	}

	UINT64 winId = pBattle->GetWinId();
	UINT64 losId = pBattle->GetLoserId(); 
	if (battleId == 7)  // 决赛
	{
		NoticeMgr::Instance()->NoticeGuildArenaCom(winId, losId);
	}
	else
	{
		NoticeMgr::Instance()->NoticeGuildArenaWin(winId, losId);
	}
	NoticeMgr::Instance()->NoticeGuildArenaLos(losId, winId);
}

bool CGuildWarMgr::IsOnTime()
{
	time_t newTime = GameTime::GetTime();
	if (CGuildArenaIntegralMgr::Instance()->GetWeekNum(newTime) != 2)
	{
		return false;
	}
	UINT32 weekNum = XCommon::GetWeekNum(newTime);
	if (weekNum == GetGlobalConfig().GuildArenaFinal.seq[0])
	{
		time_t beginTime = XCommon::MakeTime(newTime, GetGlobalConfig().GuildArenaFinal.seq[1] / 100, GetGlobalConfig().GuildArenaFinal.seq[1] % 100, 0);
		time_t endTime = XCommon::MakeTime(newTime, GetGlobalConfig().GuildArenaFinal.seq[4] / 100, GetGlobalConfig().GuildArenaFinal.seq[4] % 100, 0);
		if (newTime >= beginTime && newTime <= endTime)
		{
			return true;
		}
	}
	return false;
}

void CGuildWarMgr::ClearData()
{
	m_allGuildSet.clear();
	m_arenaMap.clear();
}

bool CGuildWarMgr::IsGuildWaring(UINT64 guildId)
{
	if (m_allGuildSet.find(guildId) != m_allGuildSet.end() 
		&& (m_state == WAR_ONE_START || m_state == WAR_TWO_START || m_state == WAR_FINAL_START))
	{
		return true;
	}
	return false;
}

void CGuildWarMgr::SynRoleOnline(UINT64 roleId, bool online)
{

	if (!IsOnTime())
	{
		return;
	}

	Guild* pGuild = CGuildMgr::Instance()->GetGuildByRoleId(roleId);
	if (pGuild == NULL)
	{
		return ;
	}

	if (m_allGuildSet.find(pGuild->GetID()) == m_allGuildSet.end())
	{
		return ;
	}

	PtcM2C_SynGuildArenaRoleOnlineNew notice;
	notice.m_Data.set_roleid(roleId);
	notice.m_Data.set_online(online);
	pGuild->BroadCast(notice);
}

void CGuildWarMgr::_BattleBeforeNotice(WAR_STATE state)
{
	time_t nowTime = GameTime::GetTime();
	time_t startTime = _GetTime(state);
	if (nowTime > startTime)
	{
		return ;
	}

	//std::set<UINT64> allGuildSet;
	//GetAllGuildId(allGuildSet);
	if ((startTime - nowTime) == GetGlobalConfig().GuildArenaBeforeWar[2])
	{
		foreach (i in m_allGuildSet)
		{
			NoticeMgr::Instance()->GuildArenaBeforeNotice(*i, 1);
		}
	}
	else if ((startTime - nowTime) == GetGlobalConfig().GuildArenaBeforeWar[1])
	{
		foreach (i in m_allGuildSet)
		{
			NoticeMgr::Instance()->GuildArenaBeforeNotice(*i, 2);
		}
	}
	else if ((startTime - nowTime) == GetGlobalConfig().GuildArenaBeforeWar[0])
	{
		foreach (i in m_allGuildSet)
		{
			NoticeMgr::Instance()->GuildArenaBeforeNotice(*i, 3);
		}
	}

}

void CGuildWarMgr::_EnterSceneNotice(WAR_STATE state)
{
	foreach (i in m_allGuildSet)
	{
		Guild* pGuild = CGuildMgr::Instance()->GetGuild(*i);
		if (pGuild != NULL)
		{
			if (state == WAR_ONE_START)
			{
				NoticeMgr::Instance()->NoticeGuildArenaEnterSceneOne(pGuild->GetID());
			}
			if (state == WAR_TWO_START)
			{
				NoticeMgr::Instance()->NoticeGuildArenaEnterSceneTwo(pGuild->GetID());
			}
			if (state == WAR_FINAL_START)
			{
				NoticeMgr::Instance()->NoticeGuildArenaEnterSceneThree(pGuild->GetID());
			}
		}
	}
}

void CGuildWarMgr::_NoticeNextState(KKSG::GuildArenaState type)
{
	PtcM2C_NoticeGuildArenaNextTime msg;
	msg.m_Data.set_state(type);

	for (auto iter = m_showRoleSet.begin(); iter != m_showRoleSet.end(); )
	{
		CRole* pRole = CRoleManager::Instance()->GetByRoleID(*iter);
		if (pRole == NULL)
		{
			m_showRoleSet.erase(iter++);
		}
		else
		{
			pRole->Send(msg);
			++iter;
		}
	}
}

time_t CGuildWarMgr::_GetTime(WAR_STATE state)
{
	time_t newTime = GameTime::GetTime();
	UINT32 weekNum = XCommon::GetWeekNum(newTime);

	foreach (i in GetGlobalConfig().GuildArenaTimeVec)
	{
		if (i->seq[0] == weekNum || i->seq[0] == 0)
		{
			switch( state)
			{
			case (WAR_ONE_START):
				{
					return XCommon::MakeTime(newTime, i->seq[7], i->seq[8], 0);
				} break;
			case (WAR_TWO_START):
				{
					return XCommon::MakeTime(newTime, i->seq[9], i->seq[10], 0);
				} break;
			case (WAR_FINAL_START):
				{
					return XCommon::MakeTime(newTime, i->seq[11], i->seq[12], 0);
				} break;
			default:
				{
					return 0;
				}
			}
		}
	}

	return 0;
}

UINT32 CGuildWarMgr::GetFightTime(UINT32 type)
{
	if (type == GUILD_ARENA_BATTLE_ONE)
	{
		return _GetTime(WAR_ONE_START);
	}
	if (type == GUILD_ARENA_BATTLE_TWO)
	{
		return _GetTime(WAR_TWO_START);
	}
	if (type == GUILD_ARENA_BATTLE_FINAL)
	{
		return _GetTime(WAR_FINAL_START);
	}

	return 0;
}

void CGuildWarMgr::_GiveAuctReward(UINT64 first, UINT64 second)
{
	if (first != 0)
	{
		GuildAuctMgr::Instance()->AddGuildAuctData(first, GA_ACT_GUILD_GMF, 1);
	}
	if (second != 0)
	{
		GuildAuctMgr::Instance()->AddGuildAuctData(second, GA_ACT_GUILD_GMF, 2);
	}
	foreach (i in m_allGuildSet)
	{
		if (*i != first && *i != second && *i != 0)
		{
			GuildAuctMgr::Instance()->AddGuildAuctData(*i, GA_ACT_GUILD_GMF, 3);
		}
	}
}

/*
void CGuildWarMgr::SynDisplace(UINT32 warType, UINT32 battleId, UINT64 guildId, std::vector<KKSG::GuildDarenaUnit> units)
{
	if (units.size() != 5)
	{
		return;
	}

	CGuildArenaBattle* pBatlle = GetBattleByBattleId(warType, battleId);

	if (pBatlle == NULL)
	{
		return ;
	}
	
	CGuildArenaTeam* pTeam = GetTeam(guildId);
	if (pTeam != NULL)
	{
		//synguildarenaroleonline
		PtcM2C_synguildarenadisplaceNew msg; 
		auto i = pTeam->GetFigthRoleMap().begin();
		auto j = units.begin();
		for (; i != pTeam->GetFigthRoleMap().end() && j != units.end(); ++i, ++j)
		{
			i->second.roleId = j->roleid();
			i->second.state = (fightUnit::FIGHT_UNIT_STATE)j->state();
			KKSG::GuildDarenaUnit* pData = msg.m_Data.add_units();
			pData->set_roleid(i->second.roleId);
			pData->set_state(i->second.state);
		}

		Guild* pGuild = CGuildMgr::Instance()->GetGuild(guildId);
		if (pGuild != NULL)
		{
			pGuild->BroadCast(msg);
		}
	}
}
*/
/////////////////////////////////////////////////////////////////////////////////////
// 积分赛部分
INSTANCE_SINGLETON(CGuildArenaIntegralMgr)

CGuildArenaIntegralMgr::CGuildArenaIntegralMgr()
{
	m_beginTime         = 0;
	m_createTime        = 0;
	m_IntegralOneTime   = 0;
	m_IntegralOneEnd    = 0;
	m_IntegralTwoTime   = 0; 
	m_IntegralTwoEnd    = 0;
	m_IntegralThreeTime = 0; 
	m_IntegralThreeEnd  = 0;
	m_IntegralFourTime  = 0; 
	m_IntegralFourEnd   = 0;
	m_FinalCreateTime   = 0;
	m_UpdateCD          = 0;
}

CGuildArenaIntegralMgr::~CGuildArenaIntegralMgr()
{

}

void CGuildArenaIntegralMgr::_DoIntegralWar(UINT32 turnNum)
{
	std::map<UINT32, std::vector<IntegralBattleData> >::reverse_iterator iter = m_turnData.rbegin();
	if (iter != m_turnData.rend())
	{
		foreach (i in iter->second)
		{
			Guild* pGuildOne = CGuildMgr::Instance()->GetGuild(i->guildOne);
			Guild* pGuildTwo = CGuildMgr::Instance()->GetGuild(i->guildTwo);
			if (pGuildOne != NULL && pGuildTwo != NULL)
			{
				GuildMatchFightMgr::Instance()->AddOneGMFRoom(i->guildOne, i->guildTwo, 0, 0, GMF_RT_GPR);
				i->state = KKSG::integralenterscene;
				_SendBattleState(pGuildOne->GetID(), KKSG::integralenterscene);
				_SendBattleState(pGuildTwo->GetID(), KKSG::integralenterscene);
				_NoticeWarBegin(pGuildOne->GetID());
				_NoticeWarBegin(pGuildTwo->GetID());
				NoticeMgr::Instance()->NoticeArenaEnterScene(pGuildOne->GetID());
				NoticeMgr::Instance()->NoticeArenaEnterScene(pGuildTwo->GetID());
			}
			else if (pGuildOne != NULL && pGuildTwo == NULL)
			{
				i->guildOneScore += 30;
				i->state = KKSG::integralend;
				_GiveReward(pGuildOne->GetID(), true);
				_AddWinTime(i->guildOne, 3);
				_SendBattleState(pGuildOne->GetID(), KKSG::integralend);
				NoticeMgr::Instance()->NoticeGuildMiss(pGuildOne->GetID());
			}
			else if (pGuildTwo != NULL && pGuildOne == NULL)
			{
				i->guildTwoScore += 30;
				i->state = KKSG::integralend;
				_GiveReward(pGuildTwo->GetID(), true);
				_AddWinTime(i->guildTwo, 3);
				_SendBattleState(pGuildTwo->GetID(), KKSG::integralend);
				NoticeMgr::Instance()->NoticeGuildMiss(pGuildTwo->GetID());
			}
			else
			{
				i->state = KKSG::integralend;
			}
			SetChange();
		}
	}
	return ;
}

bool CGuildArenaIntegralMgr::Init()
{
	if (!m_rewardTable.LoadFile("table/GuildArenaPointRaceReward.txt"))
	{
		SSWarn<<"load file table/GuildArenaPointRaceReward.txt failed"<<END;
		return false;
	}

	Load();
	CheckFinalBattle();
	m_timehandler = CTimerMgr::Instance()->SetTimer(this, 0, 1000, -1, __FILE__, __LINE__);

	return true;
}

void CGuildArenaIntegralMgr::Uninit()
{
	CTimerMgr::Instance()->KillTimer(m_timehandler);
}

void CGuildArenaIntegralMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	CheckNewTurn();  // 检查是否新的一轮  并做相应工作

	UINT32 nowTime = GameTime::GetTime();
	UINT32 weekNum = GetWeekNum(nowTime);
	UINT32 weekDay = XCommon::GetWeekNum(nowTime, false);

	if (weekNum == 1) // 单周
	{
		UINT32 turnNum = 0; // 积分赛轮次数

		// 报名时间结束处理
		foreach (x in GetGlobalConfig().GuildArenaApply)
		{
			if (weekDay == x->seq[0])
			{
				UINT32 timeOne = XCommon::MakeTime(nowTime, x->seq[1] / 100, x->seq[1] % 100, 0); 
				UINT32 timeTwo = XCommon::MakeTime(nowTime, x->seq[2] / 100, x->seq[2] % 100, 0); 
				UINT32 timeThree = XCommon::MakeTime(nowTime, x->seq[3] / 100, x->seq[3] % 100, 0); 
				UINT32 timeFour = XCommon::MakeTime(nowTime, x->seq[4] / 100, x->seq[4] % 100, 0); 
				if (nowTime == timeOne || nowTime == timeTwo || nowTime == timeThree || nowTime == timeFour)
				{
					NoticeMgr::Instance()->NoticeGuildApply();
					SSInfo << " guild arena NoticeGuildApply !" << END;
				}
			}
		}

		auto iter = GetGlobalConfig().GuildArenaSignupTime.rbegin(); 
		if (iter != GetGlobalConfig().GuildArenaSignupTime.rend() && weekDay == iter->seq[0])
		{
			time_t endTime = XCommon::MakeTime(nowTime, iter->seq[2], 0, 0);
			if (nowTime >= endTime && m_createTime != endTime)
			{
				SetcreateTime(endTime);
				_CreateIntegralWar(1);
				_SendApplyEndMail();
				SSInfo << " guild arena create war1 !" << END;
			}
		}

		foreach (iter in GetGlobalConfig().GuildArenaSingleNotice)
		{
			if (iter->seq[0] == weekDay)
			{
				time_t noticeTime = XCommon::MakeTime(nowTime, iter->seq[1] / 100, iter->seq[1] % 100, 0);
				if (nowTime == noticeTime)
				{
					NoticeMgr::Instance()->NoticeArenaIntegralReady();
					SSInfo << "guild arena NoticeArenaIntegralReady !" << END;
				}
			}
		}

		// 开启积分赛处理 
		foreach (i in GetGlobalConfig().GuildArenaSingle)
		{
			++turnNum;
			if (i->seq[0] == weekDay)
			{
				time_t beginTime = XCommon::MakeTime(nowTime, i->seq[1] / 100, i->seq[1] % 100, 0);

				if (beginTime > nowTime) // 战前公告
				{
					_BattleBeforeNotice(beginTime - nowTime);
					//SSInfo << " guild arena _BattleBeforeNotice !" << END;
				}

				if (turnNum == 1 && m_IntegralOneTime != beginTime && nowTime >= beginTime)  // 第一周第一轮
				{
					_DoIntegralWar(turnNum);       // 开启积分赛
					SetIntegralOneTime(beginTime); // 设置这次开启记录时间
					//SSInfo << "guild arena start ward : " << turnNum << END;
				}
				if (turnNum == 2 && m_IntegralTwoTime != beginTime && nowTime >= beginTime) // 第一周第二轮
				{
					_DoIntegralWar(turnNum);       // 开启积分赛
					SetIntegralTwoTime(beginTime); // 设置这次开启记录时间
					//SSInfo << "guild arena start ward : " << turnNum << END;
				}

				time_t endTime = XCommon::MakeTime(nowTime, i->seq[2] / 100, i->seq[2] % 100, 0); 
				if (turnNum == 1 && m_IntegralOneEnd != endTime && nowTime >= endTime)  // 第一周第一轮结束
				{
					_DoBattleTimeEnd();
					_CreateIntegralWar(2);          // 匹配下轮对战 
					SetIntegralOneEnd(endTime);
					//SSInfo << " guild arena create war2 !" << END;
				}
				if (turnNum == 2 && m_IntegralTwoEnd != endTime && nowTime >= endTime) // 第一周第二轮结束
				{
					_DoBattleTimeEnd();
					_CreateIntegralWar(3);          // 匹配下轮对战 
					SetIntegralTwoEnd(endTime);
					//SSInfo << " guild arena create war3 !" << END;
				}
				
			}
		}
	}

	else if (weekNum == 2) // 双周
	{
		foreach (it in GetGlobalConfig().GuildArenaSingleNotice)
		{
			if (it->seq[0] == weekDay)
			{
				time_t noticeTime = XCommon::MakeTime(nowTime, it->seq[1] / 100, it->seq[1] % 100, 0);
				if (nowTime == noticeTime)
				{
					NoticeMgr::Instance()->NoticeArenaIntegralReady();
				}
			}
		}

		// 开启积分赛处理
		UINT32 turnNum = 0; // 积分赛轮次数

		foreach (i in GetGlobalConfig().GuildArenaDouble)
		{
			++turnNum;
			if (i->seq[0] == weekDay)
			{
				time_t beginTime = XCommon::MakeTime(nowTime, i->seq[1] / 100, i->seq[1] % 100, 0);

				if (beginTime > nowTime)
				{
					_BattleBeforeNotice(beginTime - nowTime);
				}

				if (turnNum == 1 && m_IntegralThreeTime != beginTime && nowTime > beginTime)  // 第二周第一轮
				{
					_DoIntegralWar(turnNum);         // 开启积分赛
					SetIntegralThreeTime(beginTime); // 设置这次开启记录时间
					//SSInfo << "guild arena start ward : " << turnNum << END;
				}
				if (turnNum == 2 && m_IntegralFourTime != beginTime && nowTime > beginTime) // 第二周第二轮
				{
					_DoIntegralWar(turnNum);        // 开启积分赛
					SetIntegralFourTime(beginTime); // 设置这次开启记录时间
					//SSInfo << "guild arena start ward : " << turnNum << END;
				}

				time_t endTime = XCommon::MakeTime(nowTime, i->seq[2] / 100, i->seq[2] % 100, 0);
				if (turnNum == 1 && m_IntegralThreeEnd != endTime && nowTime >= endTime)  // 第二周第一轮结束
				{
					_DoBattleTimeEnd();
					_CreateIntegralWar(4);          // 匹配下轮对战 
					SetIntegralThreeEnd(endTime);
					//SSInfo << " guild arena create war4 !" << END;
				}
				
				if (turnNum == 2 && m_IntegralFourEnd != endTime && nowTime >= endTime) // 第二周第二轮结束
				{
					_DoBattleTimeEnd();
					CreateFinalBattle();
					SetIntegralFourEnd(endTime);
					//SSInfo << " guild arena create final battle !" << END;
				}
			}
		}
	}

	Update();
	return ;
}

bool CGuildArenaIntegralMgr::_IsOnWarTime(UINT32 type)
{
	UINT32 nowTime = GameTime::GetTime();
	UINT32 weekNum = XCommon::GetWeekNum(nowTime);
	if (type == 1)
	{
		time_t beginTime = XCommon::MakeTime(nowTime, GetGlobalConfig().GuildArenaSingle[0].seq[1] / 100, GetGlobalConfig().GuildArenaSingle[0].seq[1] % 100, 0);
		time_t endTime = XCommon::MakeTime(nowTime, GetGlobalConfig().GuildArenaSingle[0].seq[2] / 100, GetGlobalConfig().GuildArenaSingle[0].seq[2] % 100, 0);
		if (nowTime >= beginTime && nowTime < endTime)
		{
			return true;
		}
	}
	if (type == 2)
	{
		time_t beginTime = XCommon::MakeTime(nowTime, GetGlobalConfig().GuildArenaSingle[1].seq[1] / 100, GetGlobalConfig().GuildArenaSingle[1].seq[1] % 100, 0);
		time_t endTime = XCommon::MakeTime(nowTime, GetGlobalConfig().GuildArenaSingle[1].seq[2] / 100, GetGlobalConfig().GuildArenaSingle[1].seq[2] % 100, 0);
		if (nowTime >= beginTime && nowTime < endTime)
		{
			return true;
		}
	}
	if (type == 3)
	{
		time_t beginTime = XCommon::MakeTime(nowTime, GetGlobalConfig().GuildArenaDouble[0].seq[1] / 100, GetGlobalConfig().GuildArenaDouble[0].seq[1] % 100, 0);
		time_t endTime = XCommon::MakeTime(nowTime, GetGlobalConfig().GuildArenaDouble[0].seq[2] / 100, GetGlobalConfig().GuildArenaDouble[0].seq[2] % 100, 0);
		if (nowTime >= beginTime && nowTime < endTime)
		{
			return true;
		}
	}
	if (type == 4)
	{
		time_t beginTime = XCommon::MakeTime(nowTime, GetGlobalConfig().GuildArenaDouble[1].seq[1] / 100, GetGlobalConfig().GuildArenaDouble[1].seq[1] % 100, 0);
		time_t endTime = XCommon::MakeTime(nowTime, GetGlobalConfig().GuildArenaDouble[1].seq[2] / 100, GetGlobalConfig().GuildArenaDouble[1].seq[2] % 100, 0);
		if (nowTime >= beginTime && nowTime < endTime)
		{
			return true;
		}
	}
	return false;
}

void CGuildArenaIntegralMgr::CheckNewTurn()
{
	UINT32 nowTime = GameTime::GetTime();
	if (IsApplyTime(nowTime) && nowTime > m_beginTime && (nowTime - m_beginTime) > 7 * 24 * 60 * 60)
	{
		SetbeginTime(nowTime);
		m_applyGuild.clear();
		m_turnData.clear();
		m_byeGuild.clear();
		SetChange();

		CGuildWarMgr::Instance()->ClearData();
		SSInfo << " guild arena do clear data !" << END;
	}
}

bool CGuildArenaIntegralMgr::IsChange()
{
	return m_switch.TestAndReset();
}

void CGuildArenaIntegralMgr::SetChange()
{
	m_switch.Set();
}

UINT32 CGuildArenaIntegralMgr::GetWeekNum(UINT32 nowTime)
{
	UINT32 weekTime = 7 * 24 * 60 * 60;
	nowTime = nowTime == 0 ? GameTime::GetTime() : nowTime; 
	UINT32 serverOpenTime = GameTime::GetWeekBeginTime(MSConfig::Instance()->GetGameServerOpenTimeStamp()) - 5 * 60 * 60; //开服时间的周开始时间
	if (nowTime <= serverOpenTime)
	{
		return 0;
	}

	UINT32 weekNum = (nowTime - serverOpenTime) / weekTime + 1; 
	if (weekNum < GetGlobalConfig().GuildArenaOriginal)
	{
		return 0;
	}

	return (((weekNum - GetGlobalConfig().GuildArenaOriginal) + 1) % 2) == 0 ? 2 : 1;
}

void CGuildArenaIntegralMgr::_CreateIntegralWar(UINT32 type)
{
	std::set<IntegralUnitData> applyGuild;
	_CreateByeGuild(type, applyGuild); // 查询并创建轮空公会

	if (applyGuild.empty())
	{
		SetChange();
		return ;
	}

	std::map<UINT32, std::vector<IntegralUnitData> > guildGroup;
	_CreateGroup(applyGuild, guildGroup);
	_DoCreateIntegral(type, guildGroup);
	SetChange();
}

void CGuildArenaIntegralMgr::_CreateGroup(const std::set<IntegralUnitData>& applyGuild, std::map<UINT32, std::vector<IntegralUnitData> >& guildGroup)
{

	UINT32 fourTime = applyGuild.size() / 8 * 2; // 8个一轮 4一个一组的组数
	auto iter = applyGuild.begin(); 
	for (UINT32 cnt = 0; cnt < fourTime; ++cnt)
	{
		std::vector<UINT32> groupIdVec;
		groupIdVec.push_back(1);
		groupIdVec.push_back(2);
		groupIdVec.push_back(3);
		groupIdVec.push_back(4);

		for (UINT32 i = 0; i < 4; ++i)
		{
			if (iter != applyGuild.begin() || i != 0)
			{
				++iter;
			}
			UINT32 randNum = XRandom::randInt(0, groupIdVec.size());
			UINT32 groupId = groupIdVec[randNum];
			auto iterVec = groupIdVec.begin(); 
			for (UINT32 j = 0; j < randNum; ++j)
			{
				++iterVec;
			}
			groupIdVec.erase(iterVec);


			std::vector<IntegralUnitData>& unitVec = guildGroup[groupId];
			unitVec.push_back(*iter);
		}
	}

	if (fourTime > 0 && iter != applyGuild.end())
	{
		++iter;
	}

	while(iter != applyGuild.end())
	{
		std::vector<IntegralUnitData>& unitVec = guildGroup[1];
		unitVec.push_back(*iter);
		++iter;
	}

}

void CGuildArenaIntegralMgr::Apply(UINT64 guildId)
{
	Guild* pGuild = CGuildMgr::Instance()->GetGuild(guildId);
	if (pGuild == NULL)
	{
		return ;
	}

	_CheckGuildDelete();

	IntegralUnitData unit;
	unit.guildId = pGuild->GetID();
	unit.guildScore = 0;
	m_applyGuild.insert(unit);
	SetChange();
}

bool CGuildArenaIntegralMgr::IsApplyTime(UINT32 nowTime)
{
	nowTime = nowTime == 0 ? GameTime::GetTime() : nowTime;
	UINT32 weekNum = GetWeekNum(nowTime);
	UINT32 weekDay = XCommon::GetWeekNum(nowTime, false); 
	if (weekNum == 1) // 单周报名
	{
		foreach (i in GetGlobalConfig().GuildArenaSignupTime)
		{
			if (weekDay == i->seq[0])
			{
				UINT32 beginTime = XCommon::MakeTime(nowTime, i->seq[1], 0, 0); 
				UINT32 endTime = XCommon::MakeTime(nowTime, i->seq[2], 0, 0); 
				if (nowTime >= beginTime && nowTime <= endTime)
				{
					return true;
				}
			}
		}
	}
	return false;
}

void CGuildArenaIntegralMgr::GuildWin(UINT32 type, UINT64 guildId, UINT32 winTimes)
{
	if (!_IsOnWarTime(type)) // 没在战斗时间内
	{
		return ;
	}

	foreach (i in m_applyGuild)
	{
		if (i->guildId == guildId)
		{
			IntegralUnitData unit;
			unit.guildId = guildId;
			unit.guildScore = i->guildScore + winTimes * 10;
			m_applyGuild.erase(i);

			_CheckGuildDelete();
			m_applyGuild.insert(unit);
			SetChange();
		}
	}
}

void CGuildArenaIntegralMgr::AddHistoryRecord(std::string first, std::string second)
{
	HistoryRecordData& recond = m_historyRecord[m_historyRecord.size() + 1];
	recond.first = first;
	recond.second = second;
	SetChange();
}

void CGuildArenaIntegralMgr::_CheckGuildDelete()
{
	for (auto iter = m_applyGuild.begin(); iter != m_applyGuild.end(); )
	{
		if (CGuildMgr::Instance()->GetGuild(iter->guildId) == NULL)
		{
			m_applyGuild.erase(iter++);
		}
		else
		{
			++iter;
		}
	}
}

void CGuildArenaIntegralMgr::_CreateByeGuild(UINT32 type, std::set<IntegralUnitData>& applyGuild)
{
	_CheckGuildDelete();

	if (m_applyGuild.empty())
	{
		return ;
	}

	UINT64 byeGuildId = 0;
	if ((m_applyGuild.size() % 2) == 1) // 有单的情况
	{
		byeGuildId = _CreateByeGuildId();
		if (byeGuildId != 0)
		{
			Guild* pGuild = CGuildMgr::Instance()->GetGuild(byeGuildId);
			if (pGuild != NULL)
			{
				std::vector<IntegralBattleData>& battleVec = m_turnData[type];
				IntegralBattleData battleData;
				battleData.guildOne = pGuild->GetID();
				battleData.guildOneScore = 0;
				battleData.iconOne = pGuild->GetIcon();
				battleData.nameOne = pGuild->GetName();
				battleVec.push_back(battleData);
			}
		}
	}

	foreach (i in m_applyGuild)
	{
		if (i->guildId != byeGuildId)
		{
			applyGuild.insert(*i);
		}
	}
}

void CGuildArenaIntegralMgr::_DoCreateIntegral(UINT32 type, std::map<UINT32, std::vector<IntegralUnitData> >& guildGroup)
{
	//m_turnData.clear();
	std::vector<IntegralBattleData>& battleVec = m_turnData[type]; 

	foreach (i in guildGroup)
	{
		while (!i->second.empty())
		{
			IntegralBattleData battle;
			Guild* pGuildOne = CGuildMgr::Instance()->GetGuild(i->second.begin()->guildId);
			if (pGuildOne != NULL)
			{
				battle.guildOne = pGuildOne->GetID();
				battle.iconOne = pGuildOne->GetIcon();
				battle.nameOne = pGuildOne->GetName();
			}
			i->second.erase(i->second.begin());

			UINT32 size = i->second.size() > 3 ? 3 : i->second.size();
			UINT32 randNum = XRandom::randInt(0, size);
			auto iter = i->second.begin();
			for (UINT32 cnt = 0; cnt < randNum; ++cnt)
			{
				++iter;
			}

			Guild* pGuildTwo = CGuildMgr::Instance()->GetGuild(iter->guildId);
			if (pGuildTwo != NULL)
			{
				battle.guildTwo = pGuildTwo->GetID();
				battle.iconTwo = pGuildTwo->GetIcon();
				battle.nameTwo = pGuildTwo->GetName();
			}
			i->second.erase(iter);
			battleVec.push_back(battle);
		}
	}
}

bool IntegralUnitData::operator()(const IntegralUnitData& x, const IntegralUnitData& y)
{
	if (x.guildScore < y.guildScore)
	{
		return false;
	}
	if (x.guildScore > y.guildScore)
	{
		return true;
	}

	Guild* pX = CGuildMgr::Instance()->GetGuild(x.guildId);
	Guild* pY = CGuildMgr::Instance()->GetGuild(y.guildId);
	if (pX != NULL && pY == NULL)
	{
		return false;
	}
	else if (pX == NULL)
	{
		return true;
	}

	if (pX->GetPrestige() < pY->GetPrestige())
	{
		return false;
	}
	if (pX->GetPrestige() > pY->GetPrestige())
	{
		return true;
	}

	if (pX->GetID() > pY->GetID())
	{
		return true;
	}
	else
	{
		return false;
	}

}

bool IntegralUnitData::operator< (const IntegralUnitData &x)const
{
	if (guildScore < x.guildScore)
	{
		return false;
	}
	if (guildScore > x.guildScore)
	{
		return true;
	}

	Guild* pX = CGuildMgr::Instance()->GetGuild(guildId);
	Guild* pY = CGuildMgr::Instance()->GetGuild(x.guildId);
	if (pX != NULL && pY == NULL)
	{
		return false;
	}
	else if (pX == NULL)
	{
		return true;
	}

	if (pX->GetPrestige() < pY->GetPrestige())
	{
		return false;
	}
	if (pX->GetPrestige() > pY->GetPrestige())
	{
		return true;
	}

	if (pX->GetID() > pY->GetID())
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool CGuildArenaIntegralMgr::Load()
{
	std::string strData = CGlobalConfigDBBuild::Instance()->GetConfigValue(eGuildArena);
	KKSG::GuildIntegralData data;
	if (!data.ParseFromString(strData))
	{
		return true;
	}

	m_beginTime = data.begintime();
	m_createTime = data.createtime();
	m_IntegralOneTime = data.integralonetime();
	m_IntegralOneEnd = data.integraloneend();
	m_IntegralTwoTime = data.integraltwotime();
	m_IntegralTwoEnd = data.integraltwoend();
	m_IntegralThreeTime = data.integralthreetime();
	m_IntegralTwoEnd = data.integralthreeend();
	m_IntegralFourTime = data.integralfourtime();
	m_IntegralFourEnd = data.integralfourend(); 

//	m_byeGuild.guildId = data.byeguild().guildid();
	//m_byeGuild.guildScore = data.byeguild().guildscore();

	for (UINT32 i = 0; i < data.applyguild_size(); ++i)
	{
		IntegralUnitData unitData;
		unitData.guildId = data.applyguild(i).guildid();
		unitData.guildScore = data.applyguild(i).guildscore();

		if (CGuildMgr::Instance()->GetGuild(unitData.guildId) != NULL)
		{
			m_applyGuild.insert(unitData);
		}
	}

	for (UINT32 j = 0; j < data.turndata_size(); ++j)
	{
		std::vector<IntegralBattleData>& battleVec = m_turnData[data.turndata(j).id()];
		for (UINT32 cnt = 0; cnt < data.turndata(j).battles_size(); ++cnt)
		{
			IntegralBattleData battleData;
			battleData.guildOne = data.turndata(j).battles(cnt).guildone();
			battleData.guildTwo = data.turndata(j).battles(cnt).guildtwo();
			battleData.guildOneScore = data.turndata(j).battles(cnt).guildonescore();
			battleData.guildTwoScore = data.turndata(j).battles(cnt).guildtwoscore();
			battleData.nameOne = data.turndata(j).battles(cnt).nameone();
			battleData.nameTwo = data.turndata(j).battles(cnt).nametwo();
			battleData.isDo = data.turndata(j).battles(cnt).isdo();
			battleData.iconOne = data.turndata(j).battles(cnt).iconone();
			battleData.iconTwo = data.turndata(j).battles(cnt).icontwo();
			battleData.warTime = data.turndata(j).battles(cnt).wartime(); 
			battleData.state = data.turndata(j).battles(cnt).state(); 
			battleVec.push_back(battleData);
		}
	}

	for (UINT32 k = 0; k < data.historyrecord_size(); ++k)
	{
		HistoryRecordData recordData;
		recordData.first = data.historyrecord(k).first();
		recordData.second = data.historyrecord(k).second();
		m_historyRecord[data.historyrecord(k).id()] = recordData;
	}

	for (UINT32 x = 0; x < data.byeguild_size(); ++x)
	{
		m_byeGuild.insert(data.byeguild(x));
	}
	return true;
}

std::string CGuildArenaIntegralMgr::ToBinary()
{
	KKSG::GuildIntegralData data;
	data.set_begintime(m_beginTime);
	data.set_createtime(m_createTime);
	data.set_integralonetime(m_IntegralOneTime);
	data.set_integraloneend(m_IntegralOneEnd);
	data.set_integraltwotime(m_IntegralTwoTime);
	data.set_integraltwoend(m_IntegralTwoEnd);
	data.set_integralthreetime(m_IntegralThreeTime);
	data.set_integralthreeend(m_IntegralThreeEnd);
	data.set_integralfourtime(m_IntegralFourTime);
	data.set_integralfourend(m_IntegralFourEnd);

//	KKSG::Integralunit* pUnit = data.mutable_byeguild();
	//pUnit->set_guildid(m_byeGuild.guildId);
	//pUnit->set_guildscore(m_byeGuild.guildScore); 

	foreach (i in m_applyGuild)
	{
		KKSG::Integralunit* pUnitData = data.add_applyguild();
		pUnitData->set_guildid(i->guildId);
		pUnitData->set_guildscore(i->guildScore);
	}

	foreach (j in m_turnData)
	{
		KKSG::IntegralBattleturn* pBattleTurn = data.add_turndata();
		pBattleTurn->set_id(j->first);
		for (auto iter = j->second.begin(); iter != j->second.end(); ++iter)
		{
			KKSG::IntegralBattle* pBattle = pBattleTurn->add_battles();
			pBattle->set_isdo(iter->isDo);
			pBattle->set_guildone(iter->guildOne);
			pBattle->set_guildtwo(iter->guildTwo);
			pBattle->set_guildonescore(iter->guildOneScore);
			pBattle->set_guildtwoscore(iter->guildTwoScore);
			pBattle->set_iconone(iter->iconOne);
			pBattle->set_icontwo(iter->iconTwo);
			pBattle->set_nameone(iter->nameOne);
			pBattle->set_nametwo(iter->nameTwo);
			pBattle->set_wartime(iter->warTime);
			pBattle->set_state(iter->state);
		}
	}

	foreach (k in m_historyRecord)
	{
		KKSG::HistoryRecord* pHistory = data.add_historyrecord();
		pHistory->set_id(k->first);
		pHistory->set_first(k->second.first);
		pHistory->set_second(k->second.second);
	}

	foreach (x in m_byeGuild)
	{
		data.add_byeguild(*x);
	}

	return data.SerializeAsString();
}

void CGuildArenaIntegralMgr::Update()
{
	if (IsChange() && (GameTime::GetTime() - m_UpdateCD) >= UPDATECD)
	{
		CGlobalConfigDBBuild::Instance()->SetConfigValue(eGuildArena, ToBinary());
		CGlobalConfigDBBuild::Instance()->UpdateToDb(eGuildArena);
		m_UpdateCD = GameTime::GetTime(); 
	}
}

void CGuildArenaIntegralMgr::CreateFinalBattle()
{
	UINT32 size = 0;
	std::vector<UINT64> guildVec;
	foreach (i in m_applyGuild)
	{
		if (size >= 8)
		{
			break ;
		}

		++size;
		guildVec.push_back(i->guildId);
		NoticeMgr::Instance()->NoticeGuildJoinLast(i->guildId);
	}
	CGuildWarMgr::Instance()->CreateBattle(guildVec); 
}

void CGuildArenaIntegralMgr::CheckFinalBattle()
{
	UINT32 nowTime = GameTime::GetTime();
	if (GetWeekNum(nowTime) == 2) //双周
	{
		UINT32 weekDay = XCommon::GetWeekNum(nowTime,false);
		if (weekDay == 5)
		{
			CreateFinalBattle();
		}

		time_t endTime = XCommon::MakeTime(nowTime, GetGlobalConfig().GuildArenaDouble[1].seq[2] / 100, GetGlobalConfig().GuildArenaDouble[1].seq[2] % 100, 0);
		if (weekDay == 4 && nowTime > endTime)
		{
			CreateFinalBattle();
		}

		time_t startTime = XCommon::MakeTime(nowTime, GetGlobalConfig().GuildArenaFinal.seq[1] / 100, GetGlobalConfig().GuildArenaFinal.seq[1] % 100, 0); 
		if (weekDay == 6 && nowTime < startTime)
		{
			CreateFinalBattle();
		}
	}
}

void CGuildArenaIntegralMgr::_BattleBeforeNotice(UINT32 beforeTime)
{
	//std::set<UINT64> allGuildSet;
	//GetAllGuildId(allGuildSet);
	if (beforeTime == GetGlobalConfig().GuildArenaBeforeWar[2])
	{
		foreach (i in m_applyGuild)
		{
			NoticeMgr::Instance()->GuildArenaBeforeNotice(i->guildId, 1);
		}
	}
	else if (beforeTime == GetGlobalConfig().GuildArenaBeforeWar[1])
	{
		foreach (i in m_applyGuild)
		{
			NoticeMgr::Instance()->GuildArenaBeforeNotice(i->guildId, 2);
		}
	}
	else if (beforeTime == GetGlobalConfig().GuildArenaBeforeWar[0])
	{
		foreach (i in m_applyGuild)
		{
			NoticeMgr::Instance()->GuildArenaBeforeNotice(i->guildId, 3);
		}
	}

}

void CGuildArenaIntegralMgr::GetAllStartTime(std::set<UINT32>& allStartTime)
{
	UINT32 weekTime = 7 * 24 * 60 *60;
	UINT32 dayTime = 24 * 60 * 60;

	UINT32 weekNum = GetWeekNum();
	if (weekNum == 0)
	{
		return ;
	}

	UINT32 nowTime = GameTime::GetTime();
	UINT32 weekBeginTime = GameTime::GetWeekBeginTime(nowTime) - 5 * 60 * 60; 
	UINT32 warTime = 0;

	if (weekNum == 1)
	{
		foreach (i in GetGlobalConfig().GuildArenaSingle)
		{
			warTime = dayTime * (i->seq[0] - 1) + (i->seq[1] / 100) * 60 * 60 + (i->seq[1] % 100) * 60 + weekBeginTime;
			allStartTime.insert(warTime);
		}

		foreach (j in GetGlobalConfig().GuildArenaDouble)
		{
			warTime = dayTime * (j->seq[0] - 1) + (j->seq[1] / 100) * 60 * 60 + (j->seq[1] % 100) * 60 + weekTime + weekBeginTime;
			allStartTime.insert(warTime);
		}
	}
	else if (weekNum == 2)
	{
		foreach (x in GetGlobalConfig().GuildArenaSingle)
		{
			warTime = dayTime * (x->seq[0] - 1) + (x->seq[1] / 100) * 60 * 60 + (x->seq[1] % 100) * 60 - weekTime + weekBeginTime;
			allStartTime.insert(warTime);
		}

		foreach (k in GetGlobalConfig().GuildArenaDouble)
		{
			warTime = dayTime * (k->seq[0] - 1) + (k->seq[1] / 100) * 60 * 60 + (k->seq[1] % 100) * 60 + weekBeginTime;
			allStartTime.insert(warTime);
		}

		warTime = dayTime * (GetGlobalConfig().GuildArenaFinal.seq[0]) + GetGlobalConfig().GuildArenaFinal.seq[1] / 100 * 60 * 60 + GetGlobalConfig().GuildArenaFinal.seq[1] % 100 * 60 + weekBeginTime; 
		allStartTime.insert(warTime);
	}

}

KKSG::GuildArenaType CGuildArenaIntegralMgr::GetTimeType()
{
	UINT32 weekNum = GetWeekNum();

	if (weekNum == 0)
	{
		return KKSG::notopen;
	}

	UINT32 nowTime = GameTime::GetTime();
	UINT32 weekDay = XCommon::GetWeekNum(nowTime, false);
	if (weekNum == 1)
	{
		/*
		foreach (i in GetGlobalConfig().GuildArenaSignupTime)
		{
			if (weekDay == i->seq[0])
			{
				UINT32 beginTime = XCommon::MakeTime(nowTime, i->seq[1], 0, 0); 
				UINT32 endTime = XCommon::MakeTime(nowTime, i->seq[2], 0, 0); 
				if (nowTime >= beginTime && nowTime < endTime)
				{
					return KKSG::apply;
				}
			}
		}*/

		if (weekDay == 1)
		{
			return apply;
		}

		if (weekDay == 2)
		{
			UINT32 beginTime = XCommon::MakeTime(nowTime, GetGlobalConfig().GuildArenaSignupTime[1].seq[2], 0, 0);  
			UINT32 endTime = XCommon::MakeTime(nowTime, GetGlobalConfig().GuildArenaSingle[0].seq[2] / 100, GetGlobalConfig().GuildArenaSingle[0].seq[2] % 100, 0); 
			if (nowTime < beginTime)
			{
				return apply;
			}
			else if (nowTime >= beginTime && nowTime < endTime)
			{
				return KKSG::battleone;
			}
			else
			{
				return KKSG::battletwo;
			}
		}

		if (weekDay == 3)
		{
			return KKSG::battletwo;
		}

		if (weekDay == 4)
		{
			UINT32 beginTime = XCommon::MakeTime(nowTime, GetGlobalConfig().GuildArenaSingle[1].seq[2] / 100, GetGlobalConfig().GuildArenaSingle[1].seq[2] % 100, 0);  
			if (nowTime <= beginTime)
			{
				return battletwo;
			}
		}

		return KKSG::battlethree;
		/*
		foreach (j in GetGlobalConfig().GuildArenaSingle)
		{
			if (weekDay == j->seq[0])
			{
				UINT32 beginTime = XCommon::MakeTime(nowTime, j->seq[1] / 100, j->seq[1] % 100, 0); 
				UINT32 endTime = XCommon::MakeTime(nowTime, j->seq[2] / 100, j->seq[2] % 100, 0); 
				if (nowTime >= beginTime && nowTime < endTime)
				{
					if (weekDay == 2)
					{
						return KKSG::battleone;
					}
					else
					{
						return KKSG::battletwo;
					}
				}
			}
		}
		return KKSG::resttime;
		*/
	}
	else if (weekNum == 2)
	{
		if (weekDay == 1)
		{
			return KKSG::battlethree;
		}

		if (weekDay == 2)
		{
			UINT32 beginTime = XCommon::MakeTime(nowTime, GetGlobalConfig().GuildArenaDouble[0].seq[2] / 100, GetGlobalConfig().GuildArenaDouble[0].seq[2] % 100, 0);  
			if (nowTime <= beginTime)
			{
				return battlethree;
			}
			else 
			{
				return KKSG::battlefour;
			}
		}

		if (weekDay == 3)
		{
			return KKSG::battlefour;
		}

		if (weekDay == 4)
		{
			UINT32 beginTime = XCommon::MakeTime(nowTime, GetGlobalConfig().GuildArenaDouble[1].seq[2] / 100, GetGlobalConfig().GuildArenaDouble[1].seq[2] % 100, 0);  
			if (nowTime <= beginTime)
			{
				return battlefour;
			}
			else 
			{
				return KKSG::battlefinal;
			}

		}

		if (weekDay == 5)
		{
			return KKSG::battlefinal;
		}

		if (weekDay == 7)
		{
			return KKSG::resttime; 
		}


		if (weekDay == GetGlobalConfig().GuildArenaFinal.seq[0])
		{
			//UINT32 beginTime = XCommon::MakeTime(nowTime, GetGlobalConfig().GuildArenaFinal.seq[1] / 100, GetGlobalConfig().GuildArenaFinal.seq[1] % 100, 0); 
			UINT32 endTime = XCommon::MakeTime(nowTime, GetGlobalConfig().GuildArenaFinal.seq[4] / 100, GetGlobalConfig().GuildArenaFinal.seq[4] % 100, 0);
			if (nowTime < endTime)
			{
				return KKSG::battlefinal ;
			}
			else
			{
				return KKSG::resttime; 
			}
		}
		/*
		foreach (k in GetGlobalConfig().GuildArenaDouble)
		{
			if (weekDay == k->seq[0])
			{
				UINT32 beginTime = XCommon::MakeTime(nowTime, k->seq[1] / 100, k->seq[1] % 100, 0); 
				UINT32 endTime = XCommon::MakeTime(nowTime, k->seq[2] / 100, k->seq[2] % 100, 0); 
				if (nowTime >= beginTime && nowTime < endTime)
				{
					if (weekDay == 2)
					{
						return KKSG::battlethree;
					}
					else
					{
						return KKSG::battlefour;
					}
				}
			}
		}
		return KKSG::resttime;
		*/
	}
	return KKSG::notopen;
}

bool CGuildArenaIntegralMgr::IsApplye(UINT64 guildId)
{
	foreach (i in m_applyGuild)
	{
		if (i->guildId == guildId)
		{
			return true;
		}
	}

	return false;
}

UINT32 CGuildArenaIntegralMgr::GetApplyEndTime()
{
	UINT32 weekNum = GetWeekNum();

	if (weekNum == 0 || weekNum == 2)
	{
		return 0;
	}

	UINT32 nowTime = GameTime::GetTime();
	UINT32 weekBeginTime = GameTime::GetWeekBeginTime(nowTime, false);
	UINT32 endTime = weekBeginTime + (24 + GetGlobalConfig().GuildArenaSignupTime[1].seq[2]) * 60 * 60; 
	if (nowTime >= weekBeginTime && nowTime < endTime)
	{
		return endTime - nowTime;

	}

	return 0;
}

void CGuildArenaIntegralMgr::WriteBattleResoult(UINT64 guildOne, UINT32 winOne, UINT64 guildTwo, UINT32 winTwo)
{
	if (!_IsOnWarTime(m_turnData.size())) // 没在战斗时间内
	{
		return ;
	}


	for (std::set<IntegralUnitData>::iterator iter = m_applyGuild.begin(); iter != m_applyGuild.end(); ++iter)
	{
		if (iter->guildId == guildOne)
		{
			IntegralUnitData data;
			data.guildId = guildOne;
			data.guildScore = iter->guildScore + winOne * 10;
			m_applyGuild.erase(iter);
			m_applyGuild.insert(data);
			SetChange();
			break;
		}
	}
	for (std::set<IntegralUnitData>::iterator it = m_applyGuild.begin(); it != m_applyGuild.end(); ++it)
	{
		if (it->guildId == guildTwo)
		{
			IntegralUnitData data;
			data.guildId = guildTwo;
			data.guildScore = it->guildScore + winTwo * 10;
			m_applyGuild.erase(it);
			m_applyGuild.insert(data);
			SetChange();
			break;
		}
	}

	std::map<UINT32, std::vector<IntegralBattleData> >::reverse_iterator it = m_turnData.rbegin();
	if (it != m_turnData.rend())
	{
		foreach (j in it->second)
		{
			if (j->guildOne == guildOne && j->guildTwo == guildTwo)
			{
				j->guildOneScore += winOne * 10;
				j->guildTwoScore += winTwo * 10;
				j->state = KKSG::integralend;
			}
			else if (j->guildOne == guildTwo && j->guildTwo == guildOne)
			{
				j->guildOneScore += winTwo * 10;
				j->guildTwoScore += winOne * 10;
				j->state = KKSG::integralend;
			}
		}
	}

	if (winOne > winTwo)
	{
		_GiveReward(guildOne, true);
		_GiveReward(guildTwo, false);
	}
	else
	{
		_GiveReward(guildOne, false);
		_GiveReward(guildTwo, true);
	}
	_NoticeWarEnd(guildOne);
	_NoticeWarEnd(guildTwo);
	return ;
}

void CGuildArenaIntegralMgr::_AddWinTime(UINT64 guildId, UINT32 winTimes)
{
	foreach (i in m_applyGuild)
	{
		if (i->guildId == guildId)
		{
			IntegralUnitData data;
			data.guildId = guildId;
			data.guildScore = i->guildScore + winTimes * 10;
			m_applyGuild.erase(i);
			_CheckGuildDelete();
			m_applyGuild.insert(data);
			break;
		}
	}
}

void CGuildArenaIntegralMgr::SetBattleState(UINT64 guildOne, UINT64 guildTwo, KKSG::IntegralState state)
{
	std::map<UINT32, std::vector<IntegralBattleData> >::reverse_iterator iter = m_turnData.rbegin();
	if (iter != m_turnData.rend())
	{
		foreach (i in iter->second)
		{
			if (((i->guildOne == guildOne && i->guildTwo == guildTwo) || (i->guildOne == guildTwo && i->guildTwo == guildOne)) && i->state != integralend)
			{
				i->state = state;
				SetChange();
			}
		}
	}

	_SendBattleState(guildOne, state);	
	_SendBattleState(guildTwo, state);
	return ;
}

void CGuildArenaIntegralMgr::_SendBattleState(UINT64 guildId, KKSG::IntegralState state)
{
	PtcM2C_SynGuilIntegralState msg;
	msg.m_Data.set_state(state);

	Guild* pGuildOne = CGuildMgr::Instance()->GetGuild(guildId);
	if (pGuildOne != NULL)
	{
		const std::map<UINT64, GuildMember>& membesVec = pGuildOne->GetGuildMember();
		foreach (i in membesVec)
		{
			CRole* pRole = CRoleManager::Instance()->GetByRoleID(i->first);
			if (pRole != NULL)
			{
				pRole->Send(msg);
			}
		}
	}

}

void CGuildArenaIntegralMgr::_DoBattleTimeEnd()
{
	std::map<UINT32, std::vector<IntegralBattleData> >::reverse_iterator iter = m_turnData.rbegin();
	if (iter != m_turnData.rend())
	{
		foreach (i in iter->second)
		{
			if (i->state != KKSG::integralend)
			{
				i->state = KKSG::integralend;
				_SendBattleState(i->guildOne, KKSG::integralend);
				_SendBattleState(i->guildTwo, KKSG::integralend);
				_NoticeWarEnd(i->guildOne);
				_NoticeWarEnd(i->guildTwo);
				SetChange();
			}
		}
	}
}

void CGuildArenaIntegralMgr::ClearALLData()
{
	m_beginTime = 0;
	m_createTime = 0;
	m_IntegralOneTime = 0;
	m_IntegralOneEnd = 0;
	m_IntegralTwoTime = 0;
	m_IntegralTwoEnd = 0;
	m_IntegralThreeTime = 0;
	m_IntegralThreeEnd = 0;
	m_IntegralFourTime = 0;
	m_FinalCreateTime = 0;
	m_IntegralFourEnd = 0;
	m_applyGuild.clear();
	m_turnData.clear();
	SetChange();
}

void CGuildArenaIntegralMgr::_NoticeWarBegin(UINT64 guildId)
{
	PtcM2C_NoticeGuildArenaBeginNew notice;
	notice.m_Data.set_isstart(true);

	Guild* pGuild = CGuildMgr::Instance()->GetGuild(guildId);
	if (pGuild != NULL)
	{
		const std::map<UINT64, GuildMember>& memberMap = pGuild->GetGuildMember();
		foreach (j in memberMap)
		{
			CRole* pRole = CRoleManager::Instance()->GetByRoleID(j->first);
			if (pRole != NULL)
			{
				pRole->Send(notice);
			}
		}
	}
}

void CGuildArenaIntegralMgr::_NoticeWarEnd(UINT64 guildId)
{
	PtcM2C_NoticeGuildArenaBeginNew notice;
	notice.m_Data.set_isstart(false);

	Guild* pGuild = CGuildMgr::Instance()->GetGuild(guildId);
	if (pGuild != NULL)
	{
		const std::map<UINT64, GuildMember>& memberMap = pGuild->GetGuildMember();
		foreach (j in memberMap)
		{
			CRole* pRole = CRoleManager::Instance()->GetByRoleID(j->first);
			if (pRole != NULL)
			{
				pRole->Send(notice);
			}
		}
	}
}

void CGuildArenaIntegralMgr::DoOnlineCheck(UINT64 roleId)
{
	CRole* pRole = CRoleManager::Instance()->GetByRoleID(roleId);
	if (pRole == NULL)
	{
		return ;
	}

	UINT64 guildId = CGuildMgr::Instance()->GetGuildIdByRoleId(roleId);
	if (guildId != 0)
	{
		std::map<UINT32, std::vector<IntegralBattleData> >::reverse_iterator iter = m_turnData.rbegin();
		if (iter != m_turnData.rend())
		{
			foreach (i in iter->second)
			{
				if ((i->guildOne == guildId || i->guildTwo == guildId) && (i->state == KKSG::integralenterscene || i->state == KKSG::integralwatch))
				{
					PtcM2C_NoticeGuildArenaBeginNew notice;
					notice.m_Data.set_isstart(true);
					pRole->Send(notice);
				}
			}
		}
	}
}

UINT64 CGuildArenaIntegralMgr::_CreateByeGuildId()
{
	if (m_applyGuild.empty())
	{
		return 0;
	}

	if (m_applyGuild.size() % 2 == 0)
	{
		return 0;
	}

	std::set<UINT64> guildSet;
	foreach (i in m_applyGuild)
	{
		if (m_byeGuild.find(i->guildId) == m_byeGuild.end())
		{
			guildSet.insert(i->guildId);
		}
	}

	if (guildSet.empty())
	{
		m_byeGuild.clear();
		foreach (j in m_applyGuild)
		{
			guildSet.insert(j->guildId);
		}
	}

	UINT32 randNum = XRandom::randInt(0, guildSet.size());
	std::set<UINT64>::iterator iter = guildSet.begin();
	for (UINT32 cnt = 0; cnt < randNum; ++cnt)
	{
		++iter;
	}

	m_byeGuild.insert(*iter);
	return *iter;
}

void CGuildArenaIntegralMgr::_SendApplyEndMail()
{
	const MailConf* pConf = MailConfig::Instance()->GetMailConf(24);
	if (pConf == NULL)
	{
		return ;
	}


	std::vector<ItemDesc> rewards;
	MailData maildata(pConf->m_title, pConf->m_content, rewards, MAIL_EXPIRE_TIME);
	foreach (i in m_applyGuild)
	{
		NoticeMgr::Instance()->NoticeIntegralApplyEnd(i->guildId);
		Guild* pGuild = CGuildMgr::Instance()->GetGuild(i->guildId);
		if (pGuild != NULL)
		{
			const std::map<UINT64, GuildMember>& allMember = pGuild->GetGuildMember();
			foreach (j in allMember)
			{
				CMailMgr::Instance()->SendMail(j->first, maildata);
			}
		}
	}
}

void CGuildArenaIntegralMgr::_GiveReward(UINT64 guildId, bool isWin)
{
	Guild* pGuild = CGuildMgr::Instance()->GetGuild(guildId);
	if (pGuild != NULL)
	{
		std::string title ;
		std::string content;
		GuildArenaPointReward::RowData* pData = NULL;
		foreach (i in m_rewardTable.Table)
		{
			if (isWin && (*i)->id == 1)
			{
				pData = *i;
				title = GetGlobalConfig().GuildArenaPointRaceWinTitle;
				content = GetGlobalConfig().GuildArenaPointRaceWinContent;
				break;
			}
			else if (!isWin && (*i)->id == 2)
			{
				pData = *i;
				title = GetGlobalConfig().GuildArenaPointRaceLoseTitle;
				content = GetGlobalConfig().GuildArenaPointRaceLoseContent;
				break;
			}
		}

		if (pData == NULL)
		{
			return ;
		}

		std::vector<ItemDesc> rewards;
		pGuild->AddPrestige(pData->guildprestige);
		pGuild->AddExp(pData->guildexp);
		foreach(j in pData->reward)
		{
			ItemDesc item;
			item.itemID = j->seq[0];
			item.itemCount = j->seq[1];
			rewards.push_back(item);
		}
		MailData maildata(title, content, rewards, MAIL_EXPIRE_TIME);
		maildata.SetReason(ItemFlow_GuildArenaAward);
		const std::map<UINT64, GuildMember>& allMember = pGuild->GetGuildMember();
		foreach (k in allMember)
		{
			CMailMgr::Instance()->SendMail(k->first, maildata);
		}
	}
}

UINT32 CGuildArenaIntegralMgr::GetCurTurnNum()
{
	UINT32 type = GetTimeType(); 
	if (type == KKSG::resttime)
	{
		return m_historyRecord.size();
	}
	else if (type == KKSG::battlefinal)
	{
		if (CGuildWarMgr::Instance()->GetState() == 1 && CGuildWarMgr::Instance()->IsOnTime())
		{
			return m_historyRecord.size();
		}
		else
		{
			return m_historyRecord.size() + 1;
		}
	}
	else
	{
		return m_historyRecord.size() + 1;
	}
}

bool CGuildArenaIntegralMgr::IsGuildWaring(UINT64 guildId)
{
	std::map<UINT32, std::vector<IntegralBattleData> >::reverse_iterator iter = m_turnData.rbegin();
	if (iter != m_turnData.rend())
	{
		foreach (i in iter->second)
		{
			if ((i->guildOne == guildId || i->guildTwo == guildId) 
				&& (i->state == KKSG::integralenterscene || i->state == KKSG::integralwatch))
			{
				return true;
			}
		}
	}
	return false;
}

void CGuildArenaIntegralMgr::ClearScoreByErrorReason(UINT32 turnNum)
{
	std::set<UINT64> guildSet;
	foreach (i in m_applyGuild)
	{
		guildSet.insert(i->guildId);
	}

	m_applyGuild.clear();

	foreach (j in guildSet)
	{
		IntegralUnitData data;
		data.guildId = *j;
		data.guildScore = 0;
		m_applyGuild.insert(data);
	}

	auto iter = m_turnData.find(turnNum);
	if (iter != m_turnData.end())
	{
		foreach (k in iter->second)
		{
			k->guildOneScore = 0;
			k->guildTwoScore = 0;
		}
	}

	SetChange();
}
