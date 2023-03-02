#include "pch.h"
#include "leagueeledata.h"

void LBEleminationTeam::LoadFromKKSG(const KKSG::LBEleTeamData& info)
{
	m_leagueId = info.leagueid();
	m_serverId = info.serverid();
}

void LBEleminationTeam::ToKKSG(KKSG::LBEleTeamData& info)
{
	info.set_leagueid(m_leagueId);
	info.set_serverid(m_serverId);
}

void LBEleminationRoom::LoadFromKKSG(const KKSG::LBEleRoomData& info)
{
	m_roomId = info.roomid();
	m_team1.LoadFromKKSG(info.team1());
	m_team2.LoadFromKKSG(info.team2());
	m_state = info.state();
	m_winLeagueId = info.winleagueid();
	m_liveId = info.liveid();
	m_sceneId = info.sceneid();
	m_gsLine = info.gsline();
	m_isCross = info.iscross();
}

void LBEleminationRoom::ToKKSG(KKSG::LBEleRoomData& info)
{
	info.set_roomid(m_roomId);
	m_team1.ToKKSG(*info.mutable_team1());
	m_team2.ToKKSG(*info.mutable_team2());
	info.set_state(m_state);
	info.set_winleagueid(m_winLeagueId);
	info.set_liveid(m_liveId);
	info.set_sceneid(m_sceneId);
	info.set_gsline(m_gsLine);
	info.set_iscross(m_isCross);
}

void LBEleminationRound::LoadFromKKSG(const KKSG::LBEleRoundData& info)
{
	m_round = info.round();
	m_room.clear();
	for (int i = 0; i < info.rooms_size(); ++i)
	{
		LBEleminationRoom room;	
		room.LoadFromKKSG(info.rooms(i));
		m_room.push_back(room);
	}
}

void LBEleminationRound::ToKKSG(KKSG::LBEleRoundData& info)
{
	info.set_round(m_round);
	info.clear_rooms();
	for (size_t i = 0; i < m_room.size(); ++i)
	{
		m_room[i].ToKKSG(*info.add_rooms());
	}
}