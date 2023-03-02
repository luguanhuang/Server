#include "pch.h"
#include "leagueelemgrbase.h"

LeagueEleMgrBase::LeagueEleMgrBase()
:m_dirty(false)
,m_curRound(0)
,m_updateTime(0)
{

}

LeagueEleMgrBase::~LeagueEleMgrBase()
{

}

void LeagueEleMgrBase::LoadFromKKSG(const KKSG::LBEleSaveData& data)
{
	m_updateTime = data.updatetime();
	m_curRound = data.curround();

	for (int i = 0; i < data.rounds_size(); ++i)
	{
		LBEleminationRound& round = m_rounds[data.rounds(i).round()];
		round.LoadFromKKSG(data.rounds(i));
	}
	m_chamption.LoadFromKKSG(data.chamption());
	
	LogInfo("load lb elimination data, curround[%u] updatetime[%u]", m_curRound, m_updateTime);
}

void LeagueEleMgrBase::SaveToKKSG(KKSG::LBEleSaveData& data)
{
	data.set_updatetime(m_updateTime);
	data.set_curround(m_curRound);

	for (auto it = m_rounds.begin(); it != m_rounds.end(); ++it)
	{
		it->second.ToKKSG(*data.add_rounds());
	}
	m_chamption.ToKKSG(*data.mutable_chamption());
}

UINT32 LeagueEleMgrBase::GetRoomId(UINT32 round, UINT32 index)
{
	return round * 100 + index;
}

void LeagueEleMgrBase::GetRoundIndex(UINT32 roomId, UINT32& round, UINT32& index)
{
	round = roomId / 100;
	index = roomId % 100;
}

LBEleminationRoom* LeagueEleMgrBase::GetRoom(UINT32 round, UINT32 index)
{
	auto it = m_rounds.find(round);
	if (it == m_rounds.end())
	{
		return NULL;
	}
	LBEleminationRound& roundData = it->second;
	if (index >= roundData.m_room.size())	
	{
		return NULL;
	}
	return &roundData.m_room[index];
}

LBEleminationRoom* LeagueEleMgrBase::GetRoom(UINT32 roomId)
{
	UINT32 round = 0, index = 0;
	GetRoundIndex(roomId, round, index);
	return GetRoom(round, index);
}

void LeagueEleMgrBase::InitRoundWithTeams(std::vector<LBEleminationTeam>& teams)
{
	// 首轮对阵关系
	static UINT32 initMap[ELI_INIT_NUM] = {1, 8, 4, 5, 2, 7, 3, 6};

	UINT32 RoundTeamNum = ELI_INIT_NUM;
	UINT32 RoundNum = 1;
	UINT32 RoomNum = RoundTeamNum / 2;
	while (RoomNum > 0)
	{
		LBEleminationRound& roundData = m_rounds[RoundNum];
		roundData.m_round = RoundNum;

		//  第一轮从排行榜中初始化对阵Room
		if (RoundNum == 1)
		{
			for (int i = 0; i < RoomNum; ++i)
			{
				LBEleminationRoom room;
				room.m_roomId = GetRoomId(roundData.m_round, i);
				UINT32 index1 = initMap[2 * i] - 1;
				UINT32 index2 = initMap[2 * i + 1] - 1;

				if (index1 < teams.size())
				{
					room.m_team1 = teams[index1];
				}
				if (index2 < teams.size())
				{
					room.m_team2 = teams[index2];
				}
				roundData.m_room.push_back(room);
			}
		}
		// 其他轮次初始化Room
		else
		{
			for (int i = 0; i < RoomNum; ++i)
			{
				LBEleminationRoom room;
				room.m_roomId = GetRoomId(RoundNum, i);
				roundData.m_room.push_back(room);
			}
		}

		RoundNum++;

		RoundTeamNum /= 2;
		RoomNum = RoundTeamNum / 2;
	}
}

void LeagueEleMgrBase::OnRoundStart()
{
	// 先检查上一轮结果
	if (m_curRound != 0)
	{
		CheckRoundEnd();
	}
	if (m_curRound == 3)
	{
		LogInfo("All Round End");
		return;
	}
	m_curRound++;
	LogInfo("Round[%u] Start", m_curRound);

	auto it = m_rounds.find(m_curRound);
	if (it == m_rounds.end())
	{
		LogError("Invalid round[%u]", m_curRound);
		return;
	}
	LBEleminationRound& roundData = it->second;

	for (size_t i = 0; i < roundData.m_room.size(); ++i)
	{
		LBEleminationRoom& room = roundData.m_room[i];
		LBEleminationTeam& team1 = room.m_team1;
		LBEleminationTeam& team2 = room.m_team2;

		LBEleminationTeam winTeam;
		LBEleminationTeam loseTeam;
		// 对阵队伍全是空
		if (team1.m_leagueId == 0 && team2.m_leagueId == 0)
		{
			room.m_state = KKSG::LBEleRoomState_Finish;
		}
		// 一方轮空
		else if (team1.m_leagueId == 0)
		{
			room.m_state = KKSG::LBEleRoomState_Finish;
			winTeam = team2;
			loseTeam = team1;
		}
		// 一方轮空
		else if (team2.m_leagueId == 0)
		{
			room.m_state = KKSG::LBEleRoomState_Finish;
			winTeam = team1;
			loseTeam = team2;
		}
		else
		{
			// 建立战场
			CreateBattle(room);
		}

		// 轮空胜利，直接填充到下一轮对战中
		if (room.m_state == KKSG::LBEleRoomState_Finish)
		{
			room.m_winLeagueId = winTeam.m_leagueId;
			NotifyRoomState(&room);
			HandleRoomResult(m_curRound, winTeam, loseTeam);
			LBEleminationRoom* nextRoundRoom = GetRoom(m_curRound+1, i / 2);
			if (nextRoundRoom)
			{
				if (i % 2 == 0)
				{
					nextRoundRoom->m_team1 = winTeam;
				}
				else
				{
					nextRoundRoom->m_team2 = winTeam;
				}
				NotifyRoomState(nextRoundRoom);
			}
		}
	}
	m_dirty = true;
}

void LeagueEleMgrBase::CheckRoundEnd()
{
	auto it = m_rounds.find(m_curRound);
	if (it == m_rounds.end())
	{
		return;
	}
	LBEleminationRound& roundData = it->second;

	// 检查每一场比赛的结果
	for (size_t i = 0; i < roundData.m_room.size(); ++i)
	{
		LBEleminationRoom& room = roundData.m_room[i];
		LBEleminationTeam& team1 = room.m_team1;
		LBEleminationTeam& team2 = room.m_team2;

		if (room.m_state == KKSG::LBEleRoomState_Finish)
		{
			continue;
		}
		// 异常情况没结束的, 战斗超时或者协议丢失, 排名靠前的胜利
		LBEleminationTeam winTeam;
		LBEleminationTeam loseTeam;
		UINT64 winId = GetWinIdByRank(team1.m_leagueId, team2.m_leagueId);
		if (winId == team1.m_leagueId)
		{
			winTeam = team1;
			loseTeam = team2;
		}
		else
		{
			winTeam = team2;
			loseTeam = team2;
		}

		room.m_winLeagueId = winId;
		NotifyRoomState(&room);

		// 设置晋级的队伍
		LBEleminationRoom* nextRoundRoom = GetRoom(m_curRound+1, i / 2);
		if (nextRoundRoom)
		{
			if (i % 2 == 0)
			{
				nextRoundRoom->m_team1 = winTeam;
			}
			else
			{
				nextRoundRoom->m_team2 = winTeam;
			}
			NotifyRoomState(nextRoundRoom);
		}
		m_dirty = true;
		HandleRoomResult(m_curRound, winTeam, loseTeam);
	}
}

void LeagueEleMgrBase::SetLiveId(UINT32 roomId, UINT32 sceneId, UINT32 liveId)
{
	LBEleminationRoom* room = GetRoom(roomId);
	if (room == NULL)
	{
		return;
	}
	if (room->m_sceneId != sceneId)
	{
		LogError("room sceneid[%u] not same battle sceneid[%u]", room->m_sceneId, sceneId);
		return;
	}
	room->m_liveId = liveId;

	m_dirty = true;
	NotifyRoomState(room);
}

void LeagueEleMgrBase::OnBattleResult(UINT32 roomId, UINT32 sceneId, UINT64 winTeamId)
{
	LBEleminationRoom* room = GetRoom(roomId);
	if (room == NULL)
	{
		return;
	}
	if (room->m_sceneId != sceneId)
	{
		LogError("room sceneid[%u] not same battle sceneid[%u]", room->m_sceneId, sceneId);
		return;
	}

	UINT64 teamId1 = room->m_team1.m_leagueId;
	UINT64 teamId2 = room->m_team2.m_leagueId;
	if (room->m_state != KKSG::LBEleRoomState_Fighting)
	{
		LogWarn("Battle Result But Room is not fighting, roomId[%u], leagueteam[%llu], leagueteam[%llu]",
				roomId, teamId1, teamId2);
		return;
	}

	LogInfo("battle result team1[%llu] team2[%llu] winteam[%llu]", teamId1, teamId2, winTeamId);

	// 设置当前房间状态, 清掉战场记录
	room->m_state = KKSG::LBEleRoomState_Finish;
	room->m_liveId = 0;
	room->m_sceneId = 0;
	room->m_gsLine = 0;
	room->m_isCross = false;
	room->m_winLeagueId = winTeamId;
	NotifyRoomState(room);

	LBEleminationTeam winTeam;
	LBEleminationTeam loseTeam;
	if (winTeamId == teamId1)
	{
		winTeam = room->m_team1;
		loseTeam = room->m_team2;
	}
	else
	{
		winTeam = room->m_team2;
		loseTeam = room->m_team1;
	}

	// 填充下一轮队伍
	UINT32 round = 0, index = 0;
	GetRoundIndex(roomId, round, index);
	LBEleminationRoom* nextRoom = GetRoom(round + 1, index / 2);
	if (nextRoom)
	{
		if (index % 2 == 0)
		{
			nextRoom->m_team1 = winTeam;
		}
		else
		{
			nextRoom->m_team2 = winTeam;
		}
		NotifyRoomState(nextRoom);
	}
	m_dirty = true;

	// 结算信息
	HandleRoomResult(m_curRound, winTeam, loseTeam);
}

void LeagueEleMgrBase::CheckChamption(UINT32 round, LBEleminationTeam& winTeam)
{
	if (m_curRound != 3)
	{
		return;
	}
	m_chamption = winTeam;
	OnChamption();
	m_dirty = true;
}

void LeagueEleMgrBase::ClearRoundData()
{
	LogInfo("Clear Season Data");
	m_rounds.clear();
	m_curRound = 0;
	m_chamption.Reset();
	m_dirty = true;
}

