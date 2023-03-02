#include "pch.h"
#include "skycraftelidata.h"
#include "skyteam/skyteammgr.h"
#include "skyteam/skyteam.h"

void SkyCraftEliRoom::ToKKSG(KKSG::SCEliRoomData& data)
{
	data.set_roomid(m_roomId);
	data.set_team1(m_stId1);
	data.set_team2(m_stId2);
	data.set_win_stid(m_winStId);
	data.set_state(m_state);
	data.set_liveid(m_liveId);
	data.set_sceneid(m_sceneId);
	data.set_gsline(m_gsLine);
}

void SkyCraftEliRoom::FromKKSG(const KKSG::SCEliRoomData& data)
{
	m_roomId = data.roomid();
	m_stId1 = data.team1();
	m_stId2 = data.team2();
	m_winStId = data.win_stid();		
	m_state = data.state();
	m_liveId = data.liveid();
	m_sceneId = data.sceneid();
	m_gsLine = data.gsline();
}

void SkyCraftEliRoom::FillRoomInfo(KKSG::SCEliRoomInfo& data)
{
	data.set_roomid(m_roomId);
	data.set_state(m_state);
	data.set_win_stid(m_winStId);
	data.set_liveid(m_liveId);

	SkyTeam* pSTeam1 = SkyTeamMgr::Instance()->GetSkyTeamById(m_stId1);
	SkyTeam* pSTeam2 = SkyTeamMgr::Instance()->GetSkyTeamById(m_stId2);
	if (pSTeam1) pSTeam1->FillEliTeamInfo(*data.mutable_team1());
	if (pSTeam2) pSTeam2->FillEliTeamInfo(*data.mutable_team2());
}

void SkyCraftEliRound::FromKKSG(const KKSG::SCEliRoundData& data)
{
	m_round = data.round();
	m_rooms.clear();
	for (int i =0; i < data.rooms_size(); ++i)
	{
		SkyCraftEliRoom room;
		room.FromKKSG(data.rooms(i));
		m_rooms.push_back(room);
	}
}

void SkyCraftEliRound::ToKKSG(KKSG::SCEliRoundData& data)
{
	data.set_round(m_round);
	data.clear_rooms();
	for (auto it = m_rooms.begin(); it != m_rooms.end(); ++it)
	{
		it->ToKKSG(*data.add_rooms());
	}
}

void SkyCraftEliRound::FillRoundInfo(KKSG::SCEliRoundInfo& data)
{
	data.set_round(m_round);
	data.clear_rooms();
	for (auto it = m_rooms.begin(); it != m_rooms.end(); ++it)
	{
		it->FillRoomInfo(*data.add_rooms());
	}
}