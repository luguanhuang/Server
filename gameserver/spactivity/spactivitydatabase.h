#ifndef __SPACTIVITYDATABASE_H__
#define __SPACTIVITYDATABASE_H__

#include "spactivityrecord.h"
#include "spactivitydef.h"
#include "spactivitytask.h"



class Role;

// 活动数据基类
class SpActivityDataBase
{
public:
	SpActivityDataBase(Role* pRole, UINT32 actid);
	virtual ~SpActivityDataBase();

	virtual void Load(const KKSG::SpActivityOne& spActOneData);
	virtual void Save(KKSG::SpActivityOne& spActOneData);

	virtual void UpdatePerMinute() {}
	virtual void OnFirstEnterScene() {}

	void SetModify();

	inline UINT32 GetActID() const { return m_actid; }
	inline UINT32 GetStartTime() const { return m_startTime; }
	inline UINT32 GetActStage() const { return m_actStage; }

	///> 活动开启/结束的时候会调用
	virtual void OnSpActivityOpen() {}
	virtual void OnSpActivityClose() {}

	void AddActStage();

	void UpdateSpActivityTask(UINT32 startTime, UINT32 actStage, const std::vector<SpActivityTaskConf*>* allTask);

	virtual void IncreaseProgress(const SpActivityTaskConf* conf, UINT32 count);
	virtual void DecreaseProgress(const SpActivityTaskConf* conf, UINT32 count);	

	const SpActTask* GetTask(UINT32 taskid);

	void ResetTask();

	const std::map<UINT32, SpActTask>& GetTaskMap() const { return m_mapTask; }	

	virtual KKSG::ErrorCode GetReward(UINT32 taskid, const std::vector<Sequence<UINT32, 2>>& reward, int subreason);

	void GMClear();
	void GMCompleteAll();
	void DoTxLog(UINT32 actid, UINT32 taskid, UINT32 state, UINT32 progress);

protected:
	void _UpdateFatherTaskState(UINT32 taskid);
	// Use for complete task more than once (just for father task)
	void _DecreaseSonTaskProgressOneTime(UINT32 taskid);
	
	void _TaskChangeNtf(UINT32 taskid, const SpActTask& spActTask);

protected:
	Role* m_pRole;
	UINT32 m_actid;
	UINT32 m_startTime;
	UINT32 m_actStage;
	std::map<UINT32, SpActTask> m_mapTask;
};





#endif