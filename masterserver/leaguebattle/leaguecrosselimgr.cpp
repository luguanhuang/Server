#include "pch.h"
#include "leaguecrosselimgr.h"
#include "leagueeliminationmgr.h"
#include "leagueteam/crossleagueteammgr.h"
#include "other/serverinfomgr.h"
#include "league/ptcm2c_updateleagueeleroomstatentf.h"
#include "leagueteam/leagueteammgr.h"
#include "notice/notice.h"
#include "role/crossrolesummarymgr.h"
#include "other/serverinfomgr.h"
#include "leaguebattle/leagueseasonmgr.h"
#include "event/eventmgr.h"
#include "config.h"

LeagueCrossEliMgr::LeagueCrossEliMgr()
{

}

LeagueCrossEliMgr::~LeagueCrossEliMgr()
{

}

bool LeagueCrossEliMgr::Init()
{
	return true;
}

void LeagueCrossEliMgr::Uninit()
{

}

void LeagueCrossEliMgr::UpdateAllData(const KKSG::LBEleSaveData& data)
{
	LoadFromKKSG(data);
}

void LeagueCrossEliMgr::UpdateOneRoom(const KKSG::LBEleRoomData& data)
{
	LBEleminationRoom* pRoom = GetRoom(data.roomid());
	if (pRoom == NULL)
	{
		return;
	}
	pRoom->LoadFromKKSG(data);
	// 推送房间状态给客户端
	NotifyRoomState(pRoom);
	// 设置队伍状态
	TrySetLTeamBattleStart(pRoom);
	TrySetLTeamBattleFinish(pRoom);
}

void LeagueCrossEliMgr::UpdateChamption(const KKSG::LBEleTeamData& champ)
{
	m_chamption.LoadFromKKSG(champ);
	if (!LeagueSeasonMgr::Instance()->IsSeasonOpen())
	{
		return;
	}
	// 跨服冠军公告等
	CrossLeagueTeam* pLTeam = CrossLeagueTeamMgr::Instance()->GetTeam(m_chamption.m_leagueId);
	if (pLTeam == NULL)
	{
		return;
	}

	// 跨服跨服淘汰赛
	if (champ.serverid()==MSConfig::Instance()->GetServerID())
		for(auto iter = pLTeam->m_memberIds.begin(); iter!=pLTeam->m_memberIds.end();iter++)
			EventMgr::Instance()->AddEvent(*iter,DESIGNATION_COM_LEAGUELICHAMPTION,3);

	// 公告
	Notice notice(NoticeType_LeagueCrossEliChamption);
	std::ostringstream oss;
	UINT32 num = 0;
	for (auto it = pLTeam->m_memberIds.begin(); it != pLTeam->m_memberIds.end(); ++it)
	{
		CRoleSummary* su = CrossRoleSummaryMgr::Instance()->GetRoleSummary(*it);
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
	notice.Replace("$M", ZServerInfoMgr::Instance()->GetServerName(pLTeam->m_serverId));
	notice.Replace("$R", oss.str());
	notice.Replace("$S", pLTeam->m_name);
	notice.Send();
}

void LeagueCrossEliMgr::NotifyRoomState(LBEleminationRoom* room)
{
	if (room == NULL)
	{
		return;
	}
	PtcM2C_UpdateLeagueEleRoomStateNtf ntf;
	FillRoomInfo(room, *ntf.m_Data.mutable_room());
	LeagueEliminationMgr::Instance()->BroadCast(ntf);
}

void LeagueCrossEliMgr::TrySetLTeamBattleStart(LBEleminationRoom* pRoom)
{
	if (pRoom == NULL)
	{
		return;
	}
	if (pRoom->m_state != KKSG::LBEleRoomState_Fighting)
	{
		return;
	}

	if (pRoom->m_team1.m_leagueId != 0)
	{
		// 本服战队
		LeagueTeam* pLTeam = LeagueTeamMgr::Instance()->GetLeagueTeamById(pRoom->m_team1.m_leagueId);		
		if (pLTeam)
		{
			pLTeam->SetState(KKSG::LeagueTeamState_Battle);
			pLTeam->SetBattleStart(pRoom->m_sceneId, pRoom->m_gsLine, true);
		}
	}
	if (pRoom->m_team2.m_leagueId != 0)
	{
		// 本服战队
		LeagueTeam* pLTeam = LeagueTeamMgr::Instance()->GetLeagueTeamById(pRoom->m_team2.m_leagueId);		
		if (pLTeam)
		{
			pLTeam->SetState(KKSG::LeagueTeamState_Battle);
			pLTeam->SetBattleStart(pRoom->m_sceneId, pRoom->m_gsLine, true);
		}
	}
}

void LeagueCrossEliMgr::TrySetLTeamBattleFinish(LBEleminationRoom* pRoom)
{
	if (pRoom == NULL)
	{
		return;
	}
	if (pRoom->m_state != KKSG::LBEleRoomState_Finish)
	{
		return;
	}

	if (pRoom->m_team1.m_leagueId != 0)
	{
		// 本服战队
		LeagueTeam* pLTeam = LeagueTeamMgr::Instance()->GetLeagueTeamById(pRoom->m_team1.m_leagueId);		
		if (pLTeam)
		{
			pLTeam->SetState(KKSG::LeagueTeamState_Idle);
		}
	}
	if (pRoom->m_team2.m_leagueId != 0)
	{
		// 本服战队
		LeagueTeam* pLTeam = LeagueTeamMgr::Instance()->GetLeagueTeamById(pRoom->m_team2.m_leagueId);		
		if (pLTeam)
		{
			pLTeam->SetState(KKSG::LeagueTeamState_Idle);
		}
	}
}

void LeagueCrossEliMgr::FillEliInfo(KKSG::GetLeagueEleInfoRes& data)
{
	for (auto it = m_rounds.begin(); it != m_rounds.end(); ++it)
	{
		FillRoundInfo(&it->second, *data.add_rounds());
	}
	FillChamptionInfo(*data.mutable_chamption());
}

void LeagueCrossEliMgr::FillTeamInfo(LBEleminationTeam* team, KKSG::LBEleTeamInfo& info)
{
	if (team == NULL)
	{
		return;
	}
	info.set_leagueid(team->m_leagueId);
	CrossLeagueTeam* pLTeam = CrossLeagueTeamMgr::Instance()->GetTeam(team->m_leagueId);
	if (pLTeam == NULL)
	{
		return;
	}
	info.set_name(pLTeam->m_name);
	info.set_serverid(pLTeam->m_serverId);
	info.set_servername(ZServerInfoMgr::Instance()->GetServerName(pLTeam->m_serverId));
	info.set_zonename(ZServerInfoMgr::Instance()->GetZoneName(pLTeam->m_serverId));
}

void LeagueCrossEliMgr::FillRoomInfo(LBEleminationRoom* room, KKSG::LBEleRoomInfo& info)
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

void LeagueCrossEliMgr::FillRoundInfo(LBEleminationRound* round, KKSG::LBEleRoundInfo& info)
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

void LeagueCrossEliMgr::FillChamptionInfo(KKSG::LeagueTeamDetail& data)
{
	if (m_chamption.m_leagueId == 0)
	{
		return;
	}
	CrossLeagueTeam* pLTeam = CrossLeagueTeamMgr::Instance()->GetTeam(m_chamption.m_leagueId);
	if (pLTeam)
	{
		CrossLeagueTeamMgr::Instance()->FillLeagueTeamDetail(pLTeam, data);
	}
}

void LeagueCrossEliMgr::BroadCastToMember(const CProtocol& ntf)
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