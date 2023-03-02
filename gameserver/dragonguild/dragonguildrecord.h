#ifndef _DRAGONGUILD_RECORD_H_
#define _DRAGONGUILD_RECORD_H_

#include "unit/roleserilizeinterface.h"

class Role;
class DragonGuildRecord : public RoleSerializeInterface
{
public:
	DragonGuildRecord(Role *pRole);
	~DragonGuildRecord();

	virtual bool Load(const KKSG::RoleAllInfo *poRoleAllInfo);
	virtual void Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged);


	bool IsChestTaked(UINT32 index);
	void SetChestTaked(UINT32 index);
	UINT32 GetChestTakeInfo() { return m_takedChest; }
	bool HasCheskCanTake(UINT32 level, UINT32 liveness);

	void OnLogin();
	void CheckRedPoint();
	bool CheckTaskChest(KKSG::DragonGuildTaskType taskType, UINT32 taskID);
	void CheckAndPushNewTasks();
	void CheckAndPushNewAchivement();
	KKSG::ErrorCode GetTaskStatus(KKSG::DragonGuildTaskType taskType, UINT32 taskID);
	void ReceiveOneTaskChest(KKSG::DragonGuildTaskType taskType, UINT32 taskID);

	void CheckChest(UINT32 partLevel, UINT32 liveness);

	KKSG::ErrorCode CheckCanMakeDragonGuild();

	void OnDayPass();
	//刷新到下一个任务刷新时间
	void UpdateTaskRefreshTime();
	void OnTaskRefresh();

private:
	Switch m_dirty;
	Role *m_role;
	UINT32 m_takedChest;
	UINT32 m_lastUpdateTime;
	bool m_hasChest;
	//奖励是否领取
	std::map<UINT32, bool>m_tasksReceived;       
	std::map<UINT32, bool>m_achiveReceived;
	UINT32 m_taskRefreshTime;
	//每周奖励领取次数
	UINT32 m_iReceiveCount;
};

#endif
