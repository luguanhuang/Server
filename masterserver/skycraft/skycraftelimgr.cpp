#include "pch.h"
#include "skycraftelimgr.h"
#include "skyteam/skyteam.h"
#include "skyteam/skyteammgr.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "util/gametime.h"
#include "skycraft/ptcm2c_skycrafteliroomntf.h"
#include "global/GlobalConfigDBBuild.h"
#include "ranklist/ranklistmgr.h"
#include "ranklist/skycraftranklist.h"
#include "scene/scene.h"
#include "table/globalconfig.h"

SkyCraftEliMgr::SkyCraftEliMgr()
:m_dirty(false)
,m_curRound(KKSG::SCEliRound_None)
,m_updateTime(0)
,m_champtionId(0)
{

}

SkyCraftEliMgr::~SkyCraftEliMgr()
{

}

bool SkyCraftEliMgr::Init()
{
	LoadFromDB();
	m_updateTime = GameTime::GetTime();

	CTimerMgr::Instance()->SetTimer(this, 0, 1000, -1, __FILE__, __LINE__);	
	return true;
}

void SkyCraftEliMgr::Uninit()
{
}

void SkyCraftEliMgr::LoadFromDB()
{
	KKSG::SCEliSaveData data;
	const std::string temp = CGlobalConfigDBBuild::Instance()->GetConfigValue(eSkyCraftEliData);
	if (data.ParseFromString(temp) == false)
	{
		LogWarn("parse skycraft elimination data failed");
		return;
	}

	LoadFromKKSG(data);
	LogInfo("load skycraft elimination data, curround[%u] updatetime[%u]", m_curRound, m_updateTime);
}

void SkyCraftEliMgr::SaveToDB()
{
	if (!m_dirty)
	{
		return;
	}
	LogInfo("save skycraft elimination data, curround[%u] updatetime[%u]", m_curRound, m_updateTime);
	KKSG::SCEliSaveData data;
	SaveToKKSG(data);

	std::string temp;
	data.SerializeToString(&temp);
	CGlobalConfigDBBuild::Instance()->SetConfigValue(eSkyCraftEliData, temp);
	CGlobalConfigDBBuild::Instance()->UpdateToDb(eSkyCraftEliData);
	m_dirty = false;
}

void SkyCraftEliMgr::LoadFromKKSG(const KKSG::SCEliSaveData& data)
{
	m_updateTime = data.updatetime();
	m_curRound = data.curround();
	m_champtionId = data.champion();
	m_rounds.clear();
	for (int i = 0; i < data.rounds_size(); ++i)
	{
		UINT32 round = data.rounds(i).round();
		SkyCraftEliRound& roundData = m_rounds[round];
		roundData.FromKKSG(data.rounds(i));
	}
}

void SkyCraftEliMgr::SaveToKKSG(KKSG::SCEliSaveData& data)
{
	data.set_updatetime(m_updateTime);
	data.set_curround(m_curRound);
	data.set_champion(m_champtionId);
	data.clear_rounds();
	for (auto it = m_rounds.begin(); it != m_rounds.end(); ++it)
	{
		it->second.ToKKSG(*data.add_rounds());
	}
}

void SkyCraftEliMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	SaveToDB();
}

void SkyCraftEliMgr::OnCreateReply(CScene* scene, const KKSG::CreateBattleParam& roParam, const KKSG::CreateBattleRes& res)
{
	SkyCraftEliRoom* room = GetRoom(roParam.scinfo().uid());
	if (room == NULL)
	{
		LogError("Can't Found Room[%u]", roParam.scinfo().uid());
		return;
	}
	if (scene == NULL)
	{
		LogError("Create Battle Failed, null scene");
		return;
	}

	SkyTeam* pSTeam1 = SkyTeamMgr::Instance()->GetSkyTeamById(room->m_stId1);
	SkyTeam* pSTeam2 = SkyTeamMgr::Instance()->GetSkyTeamById(room->m_stId2);

	if (pSTeam1 == NULL)
	{
		LogError("Create Battle Failed, null skyteam [%llu]", room->m_stId1);
		return;
	}
	if (pSTeam2 == NULL)
	{
		LogError("Create Battle Failed, null skyteam [%llu]", room->m_stId2);
		return;
	}

	LogInfo("Create Battle Success, skyteam[%llu], skyteam[%llu], roomId[%u] sceneid[%u]",
			room->m_stId1, room->m_stId2, room->m_roomId, scene->GetID());

	// 房间状态
	room->m_state = KKSG::LBEleRoomState_Fighting;
	room->m_sceneId = scene->GetID();
	room->m_gsLine = scene->GetLine();
	NotifyRoomState(room);

	// 队伍状态
	pSTeam1->SetState(KKSG::SkyTeamState_Battle);
	pSTeam2->SetState(KKSG::SkyTeamState_Battle);
	pSTeam1->SetBattleStart(scene->GetID(), scene->GetLine());
	pSTeam2->SetBattleStart(scene->GetID(), scene->GetLine());
	m_dirty = true;
}

void SkyCraftEliMgr::InitRound()
{
	// 清空上赛季数据
	ClearRoundData();
	// 取排行榜前八
	SkyCraftRankList* rlist = CRankListMgr::Instance()->GetSkyCraftRankList();
	std::vector<SkyCraftRankInfo*> ranks;
	rlist->GetTopNRankInfo(SKYCRAFT_ELI_INIT_NUM, ranks);

	LogInfo("Init Round");
	std::vector<UINT64> teamIds;
	for (size_t i = 0; i < ranks.size(); ++i)
	{
		SkyCraftRankInfo* info = ranks[i];
		if (info == NULL)
		{
			continue;
		}
		LogInfo("rank[%u], skyteam[%llu]", i + 1, info->GetUniqueId());
		teamIds.push_back(info->GetUniqueId());
	}
	InitRoundWithTeams(teamIds);
	m_dirty = true;

	// 保存数据
	SaveToDB();
}

UINT32 SkyCraftEliMgr::GetRoomId(UINT32 round, UINT32 index)
{
	return round * 100 + index;
}

void SkyCraftEliMgr::GetRoundIndex(UINT32 roomId, UINT32& round, UINT32& index)
{
	round = roomId / 100;
	index = roomId % 100;
}

SkyCraftEliRoom* SkyCraftEliMgr::GetRoom(UINT32 round, UINT32 index)
{
	auto it = m_rounds.find(round);
	if (it == m_rounds.end())
	{
		return NULL;
	}
	SkyCraftEliRound& roundData = it->second;
	if (index >= roundData.m_rooms.size())
	{
		return NULL;
	}
	return &roundData.m_rooms[index];
}

SkyCraftEliRoom* SkyCraftEliMgr::GetRoom(UINT32 roomId)
{
	UINT32 round = 0, index = 0;	
	GetRoundIndex(roomId, round, index);
	return GetRoom(round, index);
}

void SkyCraftEliMgr::InitRoundWithTeams(std::vector<UINT64>& teamIds)
{
	// 首轮对阵关系
	static UINT32 initMap[SKYCRAFT_ELI_INIT_NUM] = {1, 8, 4, 5, 2, 7, 3, 6};

	UINT32 RoundTeamNum = SKYCRAFT_ELI_INIT_NUM;
	UINT32 RoundNum = 1;
	UINT32 RoomNum = RoundTeamNum / 2;
	while (RoomNum > 0)
	{
		SkyCraftEliRound& roundData = m_rounds[RoundNum];
		roundData.m_round = (KKSG::SCEliRoundType)RoundNum;

		//  第一轮从排行榜中初始化对阵Room
		if (RoundNum == 1)
		{
			for (int i = 0; i < RoomNum; ++i)
			{
				SkyCraftEliRoom room;
				room.m_roomId = GetRoomId(roundData.m_round, i);
				UINT32 index1 = initMap[2 * i] - 1;
				UINT32 index2 = initMap[2 * i + 1] - 1;

				if (index1 < teamIds.size())
				{
					room.m_stId1 = teamIds[index1];
				}
				if (index2 < teamIds.size())
				{
					room.m_stId2 = teamIds[index2];
				}
				roundData.m_rooms.push_back(room);
			}
		}
		// 其他轮次初始化Room
		else
		{
			for (int i = 0; i < RoomNum; ++i)
			{
				SkyCraftEliRoom room;
				room.m_roomId = GetRoomId(RoundNum, i);
				roundData.m_rooms.push_back(room);
			}
		}

		RoundNum++;

		RoundTeamNum /= 2;
		RoomNum = RoundTeamNum / 2;
	}
}

void SkyCraftEliMgr::OnRoundStart()
{
	// 先检查上一轮结果
	if (m_curRound != KKSG::SCEliRound_None)
	{
		CheckRoundEnd();
	}
	if (m_curRound == KKSG::SCEliRound_2to1)
	{
		LogInfo("All Round End");
		return;
	}
	m_curRound = (KKSG::SCEliRoundType)(m_curRound + 1);
	LogInfo("Round[%u] Start", m_curRound);

	auto it = m_rounds.find(m_curRound);
	if (it == m_rounds.end())
	{
		LogError("Invalid round[%u]", m_curRound);
		return;
	}
	SkyCraftEliRound& roundData = it->second;

	for (size_t i = 0; i < roundData.m_rooms.size(); ++i)
	{
		SkyCraftEliRoom& room = roundData.m_rooms[i];
		UINT64 stId1 = room.m_stId1;
		UINT64 stId2 = room.m_stId2;
		UINT64 winStId = 0, loseStId = 0;

		// 对阵队伍全是空
		if (stId1 == 0 && stId2 == 0)
		{
			room.m_state = KKSG::LBEleRoomState_Finish;
		}
		// 一方轮空
		else if (stId1 == 0)
		{
			room.m_state = KKSG::LBEleRoomState_Finish;
			winStId = stId2;
			loseStId = stId1;
		}
		// 一方轮空
		else if (stId2 == 0)
		{
			room.m_state = KKSG::LBEleRoomState_Finish;
			winStId = stId1;
			loseStId = stId2;
		}
		else
		{
			// 建立战场
			CreateBattle(room);
		}

		// 轮空胜利，直接填充到下一轮对战中
		if (room.m_state == KKSG::LBEleRoomState_Finish)
		{
			room.m_winStId = winStId;
			NotifyRoomState(&room);
			HandleRoomResult(m_curRound, winStId, loseStId);
			SetPromoteId(m_curRound, i, winStId);
		}
	}
	m_dirty = true;
}

void SkyCraftEliMgr::CheckRoundEnd()
{
	auto it = m_rounds.find(m_curRound);
	if (it == m_rounds.end())
	{
		return;
	}
	SkyCraftEliRound& roundData = it->second;

	// 检查每一场比赛的结果
	for (size_t i = 0; i < roundData.m_rooms.size(); ++i)
	{
		SkyCraftEliRoom& room = roundData.m_rooms[i];
		UINT64 stId1 = room.m_stId1;
		UINT64 stId2 = room.m_stId2;

		if (room.m_state == KKSG::LBEleRoomState_Finish)
		{
			continue;
		}
		// 异常情况没结束的, 战斗超时或者协议丢失, 排名靠前的胜利
		UINT64 winStId = 0, loseStId = 0;
		winStId = GetWinIdByRank(stId1, stId2);
		if (winStId == stId1)
		{
			loseStId = stId2;
		}
		else
		{
			loseStId = stId1;
		}

		room.m_winStId = winStId;
		NotifyRoomState(&room);
		HandleRoomResult(m_curRound, winStId, loseStId);

		// 设置晋级的队伍
		SetPromoteId(m_curRound, i, winStId);
		m_dirty = true;
	}
}

void SkyCraftEliMgr::SetPromoteId(UINT32 currRound, UINT32 index, UINT64 winStId)
{
	SkyCraftEliRoom* room = GetRoom(currRound + 1, index / 2);
	if (room == NULL)
	{
		return;
	}
	if (index % 2 == 0)
	{
		if (room->m_stId1)
		{
			LogWarn("round[%u] index[%u] already has promoted team", currRound, index);
			return;
		}
		room->m_stId1 = winStId;
	}
	else
	{
		if (room->m_stId2)
		{
			LogWarn("round[%u] index[%u] already has promoted team", currRound, index);
			return;
		}
		room->m_stId2 = winStId;
	}
	NotifyRoomState(room);
}

void SkyCraftEliMgr::SetLiveId(UINT32 roomId, UINT32 sceneId, UINT32 liveId)
{
	SkyCraftEliRoom* room = GetRoom(roomId);
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

void SkyCraftEliMgr::OnBattleResult(UINT32 roomId, UINT32 sceneId, UINT64 winStId)
{
	SkyCraftEliRoom* room = GetRoom(roomId);
	if (room == NULL)
	{
		return;
	}
	if (room->m_sceneId != sceneId)
	{
		LogError("room sceneid[%u] not same battle sceneid[%u]", room->m_sceneId, sceneId);
		return;
	}

	UINT64 stId1 = room->m_stId1;
	UINT64 stId2 = room->m_stId2;

	if (room->m_state != KKSG::LBEleRoomState_Fighting)
	{
		LogWarn("Battle Result But Room is not fighting, roomId[%u], skyteam[%llu], skyteam[%llu]",
				roomId, stId1, stId2);
		return;
	}

	LogInfo("battle result team1[%llu] team2[%llu] winteam[%llu]", stId1, stId2, winStId);

	// 设置当前房间状态, 清掉战场记录
	room->m_state = KKSG::LBEleRoomState_Finish;
	room->m_liveId = 0;
	room->m_sceneId = 0;
	room->m_gsLine = 0;
	room->m_winStId = winStId;
	NotifyRoomState(room);

	UINT64 loseStId;
	if (winStId == stId1)
	{
		loseStId = stId2;
	}
	else
	{
		loseStId = stId1;
	}

	// 填充下一轮队伍
	UINT32 round = 0, index = 0;
	GetRoundIndex(roomId, round, index);
	SetPromoteId(round, index, winStId);
	m_dirty = true;

	// 结算信息
	HandleRoomResult(m_curRound, winStId, loseStId);
}

void SkyCraftEliMgr::CheckChamption(UINT32 round, UINT64 winStId)
{
	if (m_curRound != KKSG::SCEliRound_2to1)
	{
		return;
	}
	m_champtionId = winStId;
	OnChamption();
	m_dirty = true;
}

void SkyCraftEliMgr::ClearRoundData()
{
	LogInfo("Clear Season Data");
	m_rounds.clear();
	m_curRound = KKSG::SCEliRound_None;
	m_champtionId = 0;
	m_dirty = true;
}

void SkyCraftEliMgr::NotifyRoomState(SkyCraftEliRoom* room)
{
	if (room == NULL)
	{
		return;
	}
	PtcM2C_SkyCraftEliRoomNtf ntf;
	room->FillRoomInfo(*ntf.m_Data.mutable_room());
	BroadCast(ntf);
}

void SkyCraftEliMgr::CreateBattle(SkyCraftEliRoom& room)
{
	SkyTeam* pSTeam1 = SkyTeamMgr::Instance()->GetSkyTeamById(room.m_stId1);
	SkyTeam* pSTeam2 = SkyTeamMgr::Instance()->GetSkyTeamById(room.m_stId2);
	if (pSTeam1 == NULL || pSTeam2 == NULL)
	{
		LogWarn("Create Battle Failed, skyteam null, skyteam[%llu], skyteam[%llu]", room.m_stId1, room.m_stId2);
		return;
	}
	LogInfo("Start Create Battle, skyteam[%llu] skyteam[%llu]", room.m_stId1, room.m_stId2);

	// 建立战场
	KKSG::CreateBattleParam param;
	KKSG::SkyCraftInfo& info = *param.mutable_scinfo();
	info.set_uid(room.m_roomId);
	info.set_type(KKSG::SCT_Eliminate);
	info.set_round(m_curRound);
	pSTeam1->FillBattleData(*info.mutable_team1());
	pSTeam2->FillBattleData(*info.mutable_team2());
	CSceneCreator::Instance()->CreateBattleScene(GetGlobalConfig().SkyCraftSceneId, this, param, INVALID_LINE_ID, 1);
}

UINT64 SkyCraftEliMgr::GetWinIdByRank(UINT64 stId1, UINT64 stId2)
{
	SkyCraftRankList* rlist = CRankListMgr::Instance()->GetSkyCraftRankList();
	if (rlist->GetRank(stId1) < rlist->GetRank(stId2))
	{
		return stId1;
	}
	return stId2;
}

void SkyCraftEliMgr::OnChamption()
{
	SkyTeam* pSTeam = SkyTeamMgr::Instance()->GetSkyTeamById(m_champtionId);
	if (pSTeam == NULL)
	{
		return;
	}
	// TODO
	/*
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
	*/
}

void SkyCraftEliMgr::HandleRoomResult(UINT32 round, UINT64 winStId, UINT64 loseStId)
{
	// 奖励发放
	SkyTeamMgr::Instance()->SendEliRoundReward(round, winStId, true);
	SkyTeamMgr::Instance()->SendEliRoundReward(round, loseStId, false);

	OnTeamEndBattle(winStId);
	OnTeamEndBattle(loseStId);

	CheckChamption(round, winStId);
}

void SkyCraftEliMgr::OnTeamEndBattle(UINT64 stId)
{
	if (stId == 0)	
	{
		return;
	}
	SkyTeam* pSTeam = SkyTeamMgr::Instance()->GetSkyTeamById(stId);
	if (pSTeam)
	{
		pSTeam->SetState(KKSG::SkyTeamState_Idle);
	}
}

void SkyCraftEliMgr::OnSeasonEnd()
{
	ClearRoundData();
}

void SkyCraftEliMgr::BroadCast(const CProtocol& ntf)
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

void SkyCraftEliMgr::AddWatcher(UINT64 roleId)
{
	m_watchRoleIds.insert(roleId);
}

void SkyCraftEliMgr::RemoveWatcher(UINT64 roleId)
{
	m_watchRoleIds.erase(roleId);
}

void SkyCraftEliMgr::FillEliInfo(KKSG::GetSkyCraftEliInfoRes& res)
{
	for (auto it = m_rounds.begin(); it != m_rounds.end(); ++it)
	{
		it->second.FillRoundInfo(*res.add_rounds());
	}

	if (m_champtionId)
	{
		SkyTeam* pSTeam = SkyTeamMgr::Instance()->GetSkyTeamById(m_champtionId);
		if (pSTeam)
		{
			pSTeam->FillTeamDetail(*res.mutable_chamption());
		}
	}	
}

