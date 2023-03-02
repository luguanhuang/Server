#ifndef __SPACTIVITYMGR_H__
#define __SPACTIVITYMGR_H__

namespace KKSG
{
	class SyncActivityEnd2MSArg;
}

struct OneActivityData
{
	UINT32 m_actid;
	UINT32 m_stage;
	time_t m_startTime;
	time_t m_endTime;
	UINT32 m_minlevel;
};

class SpActivityMgr
{
	SpActivityMgr(){m_syncTimes = 0;m_bIsCheckDuck=false;}
	~SpActivityMgr(){}
	DECLARE_SINGLETON(SpActivityMgr)

public:
	bool Init();
	void Uninit();

	void SyncActivityData(const KKSG::SyncActivityEnd2MSArg &roArg);
	const OneActivityData* GetActivityData(UINT32 actID);

private:
	void OnFlowerActivityStageChange(int oldFlowerActivityStage, int newFlowerActivityStage);

private:
	std::map<UINT32, OneActivityData> m_activityData;
	UINT32 m_syncTimes;
	bool m_bIsCheckDuck;
};


#endif