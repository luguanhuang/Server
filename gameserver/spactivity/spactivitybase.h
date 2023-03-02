#ifndef __SPACTIVITYBASE_H__
#define __SPACTIVITYBASE_H__

#include "spactivityconfig.h"

class Role;

// 活动管理基类
class SpActivityBase
{
	friend class SpActivityMgr;
public:
	SpActivityBase(UINT32 actid);
	virtual ~SpActivityBase();

	virtual bool Init() { return true; }
	virtual bool LoadFile() { return true; }

	virtual void OnStart();
	virtual void OnNextStage();
	virtual void OnEnd();

	virtual void Reset();

	virtual bool CheckOpenCondition(Role* pRole);
	virtual void UpdateSpActivityData(Role* pRole, bool updateTask = false);	

	virtual KKSG::ErrorCode GetReward(Role* pRole, UINT32 taskid);

	UINT32 GetStage() const { return m_stage; }
	time_t GetStartTime() const { return m_startTime; }

	static void OpenSpActivitySystem(Role* pRole, UINT32 systemid);
	static void CloseSpActivitySystem(Role* pRole, UINT32 systemid);

protected:	
	void StartOnlinePlayer(const std::vector<SpActivityTaskConf*>& task);

protected:
	UINT32 m_actid;
	UINT32 m_stage;
	UINT32 m_handler;
	time_t m_startTime;
};


#endif