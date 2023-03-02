#include "pch.h"
#include "leagueeliminationmgr.h"
#include "ranklist/ranklistmgr.h"
#include "ranklist/leagueranklist.h"
#include "config.h"
#include "leagueteam/leagueteammgr.h"
#include "table/globalconfig.h"
#include "scene/scene.h"
#include "global/GlobalConfigDBBuild.h"
#include "util/gametime.h"
#include "league/ptcm2c_updateleagueeleroomstatentf.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "mail/mailconfig.h"
#include "define/itemdef.h"
#include "mail/maildata.h"
#include "mail/mailmgr.h"
#include "notice/notice.h"
#include "define/noticedef.h"
#include "role/rolesummarymgr.h"
#include "role/rolesummary.h"
#include "tablemgr/leaguematchtablemgr.h"
#include "event/eventmgr.h"
#include "arenastar/arenastarmgr.h"

LeagueEliminationMgr::LeagueEliminationMgr()
{

}

LeagueEliminationMgr::~LeagueEliminationMgr()
{

}

bool LeagueEliminationMgr::Init()
{
	LoadDataFromDB();
	// 注册定时器
	CTimerMgr::Instance()->SetTimer(this, 0, 1000, -1, __FILE__, __LINE__);	
	m_updateTime = GameTime::GetTime();
	return true;
}

void LeagueEliminationMgr::Uninit()
{
}

void LeagueEliminationMgr::SaveAll2DB()
{
	if (m_dirty)
	{
		SaveDataToDB();
	}
}

void LeagueEliminationMgr::LoadDataFromDB()
{
	KKSG::LBEleSaveData data;
	const std::string temp = CGlobalConfigDBBuild::Instance()->GetConfigValue(eLeagueBattleEliData);
	if (data.ParseFromString(temp) == false)
	{
		LogWarn("parse league elimination data failed");
		return;
	}
	LoadFromKKSG(data);
}

void LeagueEliminationMgr::SaveDataToDB()
{
	LogInfo("save lb elimination data, curround[%u] updatetime[%u]", m_curRound, m_updateTime);
	KKSG::LBEleSaveData data;
	SaveToKKSG(data);

	std::string temp;
	data.SerializeToString(&temp);
	CGlobalConfigDBBuild::Instance()->SetConfigValue(eLeagueBattleEliData, temp);
	CGlobalConfigDBBuild::Instance()->UpdateToDb(eLeagueBattleEliData);
	m_dirty = false;
}

void LeagueEliminationMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	UINT32 now = GameTime::GetTime();
	if (now > (m_updateTime + 10) && m_dirty)
	{
		m_updateTime = now;
		SaveDataToDB();
	}
}

void LeagueEliminationMgr::InitRound()
{
	// 清空上赛季数据
	ClearRoundData();

	// 取排行榜前八
	LeagueRankList* ranklist = CRankListMgr::Instance()->GetLeagueRankList();
	std::vector<LeagueRankInfo*> ranks;
	ranklist->GetTopNRankInfo(ELI_INIT_NUM, ranks);

	LogInfo("Init Round");
	std::vector<LBEleminationTeam> teams;
	for (size_t i = 0; i < ranks.size(); ++i)
	{
		LeagueRankInfo* info = ranks[i];
		if (info == NULL)
		{
			continue;
		}
		LogInfo("rank[%u], leagueteamId[%llu]", i + 1, info->GetUniqueId());
		LBEleminationTeam team;
		team.m_leagueId = info->GetUniqueId();
		team.m_serverId = MSConfig::Instance()->GetServerID();
		teams.push_back(team);
		LeagueTeam* pLTeam = LeagueTeamMgr::Instance()->GetLeagueTeamById(team.m_leagueId);
		if (pLTeam)
		{
			std::vector<LeagueTeamMember> vecMember = pLTeam->GetAllMember();
			for (UINT32 i = 0; i<vecMember.size();i++)
				EventMgr::Instance()->AddEvent(vecMember[i].roleId,DESIGNATION_COM_LEAGUELICHAMPTION,1);
		}
	}

	InitRoundWithTeams(teams);
	// 保存数据
	SaveDataToDB();
}

void LeagueEliminationMgr::CreateBattle(LBEleminationRoom& room)
{
	UINT64 teamId1 = room.m_team1.m_leagueId;
	UINT64 teamId2 = room.m_team2.m_leagueId;
	LeagueTeam* leagueTeamA = LeagueTeamMgr::Instance()->GetLeagueTeamById(teamId1);
	LeagueTeam* leagueTeamB = LeagueTeamMgr::Instance()->GetLeagueTeamById(teamId2);
	if (leagueTeamA == NULL || leagueTeamB == NULL)
	{
		LogWarn("Create Battle Failed, leagueteam null, leagueteam[%llu], leagueteam[%llu]", teamId1, teamId2);
		return;
	}
	LogInfo("Start Create Battle, leagueteam[%llu] leagueteam[%llu]", teamId1, teamId2);

	// 建立战场
	KKSG::CreateBattleParam param;
	KKSG::LeagueBattleInfo& info = *param.mutable_lbinfo();
	info.set_uid(room.m_roomId);
	info.set_type(KKSG::LeagueBattleType_Eliminate);
	leagueTeamA->FillLeagueTeamBattleData(*info.mutable_team1());
	leagueTeamB->FillLeagueTeamBattleData(*info.mutable_team2());

	CSceneCreator::Instance()->CreateBattleScene(GetGlobalConfig().LeagueBattleSceneId, this, param, INVALID_LINE_ID, 1);
}

void LeagueEliminationMgr::OnCreateReply(CScene* scene, const KKSG::CreateBattleParam& roParam, const KKSG::CreateBattleRes& res)
{
	LBEleminationRoom* room = GetRoom(roParam.lbinfo().uid());
	if (room == NULL)
	{
		LogError("Can't Found Room[%u]", roParam.lbinfo().uid());
		return;
	}
	if (scene == NULL)
	{
		LogError("Create Battle Failed, null scene");
		return;
	}

	UINT64 teamId1 = room->m_team1.m_leagueId;
	UINT64 teamId2 = room->m_team2.m_leagueId;
	LeagueTeam* leagueTeamA = LeagueTeamMgr::Instance()->GetLeagueTeamById(teamId1);
	LeagueTeam* leagueTeamB = LeagueTeamMgr::Instance()->GetLeagueTeamById(teamId2);

	if (leagueTeamA == NULL)
	{
		LogError("Create Battle Failed, null leagueteam [%llu]", teamId1);
		return;
	}

	if (leagueTeamB == NULL)
	{
		LogError("Create Battle Failed, null leagueteam [%llu]", teamId2);
		return;
	}

	LogInfo("Create Battle Success, leagueteam[%llu], leagueteam[%llu], roomId[%u] sceneid[%u]",
			teamId1, teamId2, room->m_roomId, scene->GetID());

	// 房间状态
	room->m_state = KKSG::LBEleRoomState_Fighting;
	room->m_sceneId = scene->GetID();
	room->m_gsLine = scene->GetLine();
	room->m_isCross = false;
	NotifyRoomState(room);

	// 队伍状态
	leagueTeamA->SetState(KKSG::LeagueTeamState_Battle);
	leagueTeamB->SetState(KKSG::LeagueTeamState_Battle);
	leagueTeamA->SetBattleStart(scene->GetID(), scene->GetLine(), false);
	leagueTeamB->SetBattleStart(scene->GetID(), scene->GetLine(), false);

	m_dirty = true;
}

void LeagueEliminationMgr::NotifyRoomState(LBEleminationRoom* room)
{
	if (room == NULL)
	{
		return;
	}
	PtcM2C_UpdateLeagueEleRoomStateNtf ntf;
	FillRoomInfo(room, *ntf.m_Data.mutable_room());
	BroadCast(ntf);
}

void LeagueEliminationMgr::BroadCast(const CProtocol& ntf)
{
	for (auto it = m_watchRoleIds.begin(); it != m_watchRoleIds.end(); ++it)
	{
		CRole* pRole = CRoleManager::Instance()->GetByRoleID(*it);
		if (pRole == NULL)
		{
			continue;
		}
		pRole->Send(ntf);
	}
}

void LeagueEliminationMgr::BroadCastToMember(const CProtocol& ntf)
{
	if (m_rounds.empty())
	{
		return;
	}
	LBEleminationRound& round = m_rounds.begin()->second;
	for (auto it = round.m_room.begin(); it != round.m_room.end(); ++it)
	{
		LeagueTeam* pLTeam1 = LeagueTeamMgr::Instance()->GetLeagueTeamById(it->m_team1.m_leagueId);
		LeagueTeam* pLTeam2 = LeagueTeamMgr::Instance()->GetLeagueTeamById(it->m_team2.m_leagueId);
		if (pLTeam1) pLTeam1->BroadCast(ntf);
		if (pLTeam2) pLTeam2->BroadCast(ntf);
	}
}

void LeagueEliminationMgr::AddWatcher(UINT64 roleId)
{
	m_watchRoleIds.insert(roleId);
}

void LeagueEliminationMgr::RemoveWatcher(UINT64 roleId)
{
	m_watchRoleIds.erase(roleId);
}

void LeagueEliminationMgr::FillChamptionInfo(KKSG::LeagueTeamDetail& data)
{
	if (m_chamption.m_leagueId == 0)
	{
		return;
	}
	LeagueTeam* pLTeam = LeagueTeamMgr::Instance()->GetLeagueTeamById(m_chamption.m_leagueId);
	if (pLTeam)
	{
		pLTeam->FillLeagueTeamDetail(data);
	}
}

void LeagueEliminationMgr::OnSeasonEnd()
{
	ClearRoundData();
}

void LeagueEliminationMgr::HandleRoomResult(UINT32 round, LBEleminationTeam& winTeam, LBEleminationTeam& loseTeam)
{
	// 奖励发放
	LeagueTeamMgr::Instance()->SendEliRoundReward(round, winTeam.m_leagueId, true, false);
	LeagueTeamMgr::Instance()->SendEliRoundReward(round, loseTeam.m_leagueId, false, false);

	OnTeamEndBattle(winTeam.m_leagueId);
	OnTeamEndBattle(loseTeam.m_leagueId);

	CheckChamption(round, winTeam);
}

void LeagueEliminationMgr::OnTeamEndBattle(UINT64 ltId)
{
	if (ltId == 0)	
	{
		return;
	}
	LeagueTeam* pLTeam = LeagueTeamMgr::Instance()->GetLeagueTeamById(ltId);
	if (pLTeam) pLTeam->SetState(KKSG::LeagueTeamState_Idle);
}


void LeagueEliminationMgr::OnChamption()
{
	LeagueTeam* pLTeam = LeagueTeamMgr::Instance()->GetLeagueTeamById(m_chamption.m_leagueId);
	if (pLTeam == NULL)
	{
		return;
	}

	ArenaStarMgr::Instance()->OnArenaSeasonEnd(KKSG::AST_LEAGUE, m_chamption.m_leagueId);

	//本服淘汰赛
	for(auto iter = pLTeam->GetAllMember().begin(); iter!=pLTeam->GetAllMember().end();iter++)
		EventMgr::Instance()->AddEvent(iter->roleId,DESIGNATION_COM_LEAGUELICHAMPTION,2);

	// 公告
	Notice notice(NoticeType_LeagueEliChamption);
	std::vector<LeagueTeamMember>& members = pLTeam->GetAllMember();
	std::ostringstream oss;
	UINT32 num = 0;
	for (auto it = members.begin(); it != members.end(); ++it)
	{
		CRoleSummary* su = CRoleSummaryMgr::Instance()->GetRoleSummary(it->roleId);
		if (su == NULL)
		{
			continue;
		}
		if (num != 0)
		{
			oss << " ";
		}
		oss << su->GetName();
		num++;
	}
	notice.Replace("$S", pLTeam->GetName());
	notice.Replace("$R", oss.str());
	notice.Send();
}

UINT64 LeagueEliminationMgr::GetWinIdByRank(UINT64 ltId1, UINT64 ltId2)
{
	LeagueRankList* ranklist = CRankListMgr::Instance()->GetLeagueRankList();
	if (ranklist->GetRank(ltId1) < ranklist->GetRank(ltId2))
	{
		return ltId1;
	}
	return ltId2;
}

void LeagueEliminationMgr::FillTeamInfo(LBEleminationTeam* team, KKSG::LBEleTeamInfo& info)
{
	if (team == NULL)
	{
		return;
	}
	info.set_leagueid(team->m_leagueId);
	LeagueTeam* pLTeam = LeagueTeamMgr::Instance()->GetLeagueTeamById(team->m_leagueId);
	if (pLTeam == NULL)
	{
		return;
	}
	info.set_name(pLTeam->GetName());
	info.set_serverid(MSConfig::Instance()->GetServerID());
	info.set_servername(MSConfig::Instance()->GetName());
	info.set_zonename(MSConfig::Instance()->GetZoneName());
}

void LeagueEliminationMgr::FillRoomInfo(LBEleminationRoom* room, KKSG::LBEleRoomInfo& info)
{
	if (room == NULL)
	{
		return;
	}
	info.set_roomid(room->m_roomId);
	FillTeamInfo(&room->m_team1, *info.mutable_team1());
	FillTeamInfo(&room->m_team2, *info.mutable_team2());
	info.set_state(room->m_state);
	info.set_winleagueid(room->m_winLeagueId);
	info.set_liveid(room->m_liveId);
}

void LeagueEliminationMgr::FillRoundInfo(LBEleminationRound* round, KKSG::LBEleRoundInfo& info)
{
	if (round == NULL)
	{
		return;
	}

	info.set_round(round->m_round);
	for (auto it = round->m_room.begin(); it != round->m_room.end(); ++it)
	{
		LBEleminationRoom& room = *it;
		FillRoomInfo(&room, *info.add_rooms());
	}
}