#ifndef _LEAGUE_ELE_DATA_H_
#define _LEAGUE_ELE_DATA_H_

#include "pb/enum.pb.h"

struct LBEleminationTeam
{
	LBEleminationTeam() : m_leagueId(0), m_serverId(0) {}
	UINT64 m_leagueId;
	UINT32 m_serverId;

	void Reset() { m_leagueId = 0; m_serverId = 0; }

	void LoadFromKKSG(const KKSG::LBEleTeamData& info);
	void ToKKSG(KKSG::LBEleTeamData& info);
};

struct LBEleminationRoom
{
	LBEleminationRoom(): m_roomId(0), m_state(KKSG::LBEleRoomState_Idle), m_winLeagueId(0), m_liveId(0)
						,m_sceneId(0), m_gsLine(0), m_isCross(false) {}

	UINT32 m_roomId;
	LBEleminationTeam m_team1;
	LBEleminationTeam m_team2;
	KKSG::LBEleRoomState m_state;
	UINT64 m_winLeagueId;
	UINT32 m_liveId;
	UINT32 m_sceneId;
	UINT32 m_gsLine;
	bool   m_isCross;

	void LoadFromKKSG(const KKSG::LBEleRoomData& info);
	void ToKKSG(KKSG::LBEleRoomData& info);
};

struct LBEleminationRound
{
	UINT32 m_round;
	std::vector<LBEleminationRoom> m_room;

	void LoadFromKKSG(const KKSG::LBEleRoundData& info);
	void ToKKSG(KKSG::LBEleRoundData& info);
};

#endif