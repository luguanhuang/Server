#include "pch.h"
#include "arenahistory.h"
#include "time.h"
#include "pb/project.pb.h"

void SRoleArenaInfo::LoadArenaInfo(const KKSG::RoleArenaInfo& stRoleArenaInfo)
{
	dwRecordCount = stRoleArenaInfo.recordcount();
	for (int i = 0; i < stRoleArenaInfo.rolearenarecordinfo_size(); ++i)
	{
		if (i >= ARENA_RECORDCOUNT)
		{
			break;
		}
		RoleArenaRecordInfoList[i].dwRecordMask = stRoleArenaInfo.rolearenarecordinfo(i).recordmask();
		RoleArenaRecordInfoList[i].dwRecordTime = stRoleArenaInfo.rolearenarecordinfo(i).recordtime();
		RoleArenaRecordInfoList[i].qwChallengedId = stRoleArenaInfo.rolearenarecordinfo(i).challengedid();
	}
}

void SRoleArenaInfo::SaveArenaInfo(KKSG::RoleArenaInfo& stRoleArenaInfo)
{
	stRoleArenaInfo.Clear();
	stRoleArenaInfo.set_recordcount(dwRecordCount);
	for (UINT32 i = 0; i < ARENA_RECORDCOUNT; ++i)
	{
		KKSG::RoleArenaRecordInfo* pRoleArenaRecordInfo = stRoleArenaInfo.add_rolearenarecordinfo();
		pRoleArenaRecordInfo->set_recordmask(RoleArenaRecordInfoList[i].dwRecordMask);
		pRoleArenaRecordInfo->set_recordtime(RoleArenaRecordInfoList[i].dwRecordTime);
		pRoleArenaRecordInfo->set_challengedid(RoleArenaRecordInfoList[i].qwChallengedId);
	}
}

void SRoleArenaInfo::PushRecorded(UINT32 dwIsPassive, UINT32 dwIsWin, UINT64 qwChallengeId)
{
	++dwRecordCount;
	SRoleArenaRecordInfo stRoleArenaRecordInfo;
	stRoleArenaRecordInfo.dwRecordMask = (dwIsPassive<<16) + dwIsWin;
	stRoleArenaRecordInfo.qwChallengedId = qwChallengeId;
	stRoleArenaRecordInfo.dwRecordTime = (UINT32)time(NULL);
	RoleArenaRecordInfoList[(dwRecordCount-1)%ARENA_RECORDCOUNT] = stRoleArenaRecordInfo;
}

