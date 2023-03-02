#ifndef __ACTIVITYRECORD_H__
#define __ACTIVITYRECORD_H__

#include "unit/roleserilizeinterface.h"

#define INVALID_ACTIVITYCOUNT (UINT32)(0)
#define INVALID_ACTIVITYCHESTID (UINT32)(-1)
#define ACTIVITY_SINGLECHEST_MASK (0x07)
#define ACTIVITY_NORMALCHEST_COUNT 3


class Role;
namespace KKSG
{
	class ActivityRecord;
}
class CActivityRecord : public RoleSerializeInterface
{
public:
	CActivityRecord(Role* role);
	void Reset();

	virtual bool Load(const KKSG::RoleAllInfo *poRoleAllInfo);
	virtual void Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged);

	void RecordToKKSG(KKSG::ActivityRecord* pRecordInfo);

	bool Update();

	void PushActivityId(UINT32 dwActivityId);
	void AddActivityIdCount(UINT32 dwActivityId, UINT32 dwValue, bool isComplete);
	UINT32 GetFinishCount(UINT32 dwActivityId);

	//void UpdateDoubleId();
	//UINT32 GetDoubleId();

	UINT32 GetChestGetInfo();
	//UINT32 GetChestIndex(UINT32 dwIndex);
	void SetChestGetInfo(UINT32 dwIndex);

	bool IsRecived(UINT32 dwIndex);

	UINT32 GetActivityAllValue();
	UINT32 GetWeekValue();

	void DoAddGuild();
	UINT32 GetGuildLadderTime() { return m_guildLadderTime; }
	void AddGuildLadderTime() { m_guildLadderTime++; }
	bool CheckHaveRandomActivityId(UINT32 activity_id);
	bool  IsGetRandomActivityId(UINT32 activity_id);
	void TestAddVaule(UINT32 value);
private:
	void _OnAvtivityAdd(UINT32 activityId, UINT32 value);
	void _AddGuildActivity(UINT32 addNum);
	void _TeamCostGift();
	void _ActivityCoin(UINT32 activityid);

private:
	Role* m_pRole;
	Switch m_bIsModify;
	UINT32 m_dwAllValue;
	UINT32 m_dwDoubleId;
	UINT32 m_dwChestGetInfo;
	time_t m_dwLastUpdateTime;
	UINT32 m_dwWeekValue; // 活跃度周累积
	UINT32 m_guildLadderTime; // 公会天梯领奖次数
	std::map<UINT32, UINT32> m_mapFinishCount;
};

#endif