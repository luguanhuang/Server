#ifndef __ARENAHISTORY_H__
#define __ARENAHISTORY_H__

#define ARENA_RECORDCOUNT 6

struct SRoleArenaRecordInfo
{
	UINT32 dwRecordMask;	
	UINT32 dwRecordTime;
	UINT64 qwChallengedId;

	SRoleArenaRecordInfo()
	:dwRecordMask(0)
	,dwRecordTime(0)
	,qwChallengedId(0)
	{

	}
};

namespace KKSG
{
	class RoleArenaInfo;
}
struct SRoleArenaInfo
{
	UINT32 dwRecordCount;
	SRoleArenaRecordInfo RoleArenaRecordInfoList[ARENA_RECORDCOUNT];

	SRoleArenaInfo()
	:dwRecordCount(0)
	{

	}
	void LoadArenaInfo(const KKSG::RoleArenaInfo& stRoleArenaInfo);
	void SaveArenaInfo(KKSG::RoleArenaInfo& stRoleArenaInfo);
	void PushRecorded(UINT32 dwIsPassive, UINT32 dwIsWin, UINT64 qwChallengeId);
};

#endif