#ifndef __SPACTIVITYTASK_H__
#define __SPACTIVITYTASK_H__

#include "spactivitydef.h"
#include "spactivityconfig.h"

struct SpActTask
{
	SpActTask()
	{
		state = SpActTaskState_NotComplete;
		progress = 0;
	}
	SpActTask(UINT32 _state, UINT32 _progress):state(_state), progress(_progress){}
	UINT32 state; // 0未完成，1完成，2已领取
	UINT32 progress;

	bool operator==(const SpActTask& task) const
	{
		return state == task.state && progress == task.progress;
	}

	void Reset()
	{
		state = SpActTaskState_NotComplete;
		progress = 0;
	}
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Role;
class SpActTaskHandler
{
public:
	static SpActTaskHandler* GetTaskHandler(UINT32 baseTask);

	SpActTaskHandler(UINT32 baseTask):m_baseTask(baseTask) {}
	~SpActTaskHandler() {}

	virtual bool CheckTaskConfValid(const SpActivityTaskConf* conf) = 0;
	virtual bool IncreaseProgress(Role* pRole, SpActTask& spActTask, const SpActivityTaskConf* conf, UINT32 count) = 0;
	virtual bool DecreaseProgress(Role* pRole, SpActTask& spActTask, const SpActivityTaskConf* conf, UINT32 count) = 0;
	virtual bool DecreaseSonTaskProgressOneTime(Role* pRole, SpActTask& spActTask, const SpActivityTaskConf* conf) = 0;

private:
	static void InitHandlers();

protected:
	static std::map<UINT32, SpActTaskHandler*> m_handlers;
	UINT32 m_baseTask;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class SpActTaskHandlerArg : public SpActTaskHandler
{
public:
	SpActTaskHandlerArg(UINT32 baseTask, UINT32 argsSize, UINT32 judgePos);
	virtual bool CheckTaskConfValid(const SpActivityTaskConf* conf);
	virtual bool IncreaseProgress(Role* pRole, SpActTask& spActTask, const SpActivityTaskConf* conf, UINT32 count);
	virtual bool DecreaseProgress(Role* pRole, SpActTask& spActTask, const SpActivityTaskConf* conf, UINT32 count);
	virtual bool DecreaseSonTaskProgressOneTime(Role* pRole, SpActTask& spActTask, const SpActivityTaskConf* conf);

private:
	UINT32 m_argsSize;
	UINT32 m_judgePos;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class SpActTaskHandlerCollect : public SpActTaskHandler
{
public:
	SpActTaskHandlerCollect(UINT32 baseTask);
	virtual bool CheckTaskConfValid(const SpActivityTaskConf* conf);
	virtual bool IncreaseProgress(Role* pRole, SpActTask& spActTask, const SpActivityTaskConf* conf, UINT32 count);
	virtual bool DecreaseProgress(Role* pRole, SpActTask& spActTask, const SpActivityTaskConf* conf, UINT32 count);
	virtual bool DecreaseSonTaskProgressOneTime(Role* pRole, SpActTask& spActTask, const SpActivityTaskConf* conf);
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#endif