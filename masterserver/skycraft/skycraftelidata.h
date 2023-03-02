#ifndef _SKYCRAFT_ELI_DATA_H_
#define _SKYCRAFT_ELI_DATA_H_

struct SkyCraftEliRoom
{
	SkyCraftEliRoom(): m_roomId(0), m_stId1(0), m_stId2(0), m_state(KKSG::LBEleRoomState_Idle), m_winStId(0), m_liveId(0), m_sceneId(0), m_gsLine(0) {}

	UINT32 m_roomId;
	UINT64 m_stId1;
	UINT64 m_stId2;
	KKSG::LBEleRoomState m_state;
	UINT64 m_winStId;
	UINT32 m_liveId;
	UINT32 m_sceneId;
	UINT32 m_gsLine;

	void ToKKSG(KKSG::SCEliRoomData& data);
	void FromKKSG(const KKSG::SCEliRoomData& data);

	void FillRoomInfo(KKSG::SCEliRoomInfo& data);
};

struct SkyCraftEliRound
{
	SkyCraftEliRound(): m_round(KKSG::SCEliRound_None) {}

	KKSG::SCEliRoundType m_round;
	std::vector<SkyCraftEliRoom> m_rooms;

	void FromKKSG(const KKSG::SCEliRoundData& data);
	void ToKKSG(KKSG::SCEliRoundData& data);

	void FillRoundInfo(KKSG::SCEliRoundInfo& data);
};

#endif