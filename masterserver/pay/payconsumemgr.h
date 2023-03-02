#ifndef __PayConsumeMgr_H__
#define __PayConsumeMgr_H__

class CPayConsumeMgr : public ITimer
{
	CPayConsumeMgr();
	~CPayConsumeMgr();
	DECLARE_SINGLETON(CPayConsumeMgr)

public:
	bool Init();
	void Uninit();
	void OnTimer(UINT32 dwID, UINT32 dwCount);
	bool LoadFromDB();
	void CheckConfig();
	void SendToGs(UINT32 line=0);
	void BuildData(KKSG::ConsumeRebateNtfData* pData);
	void SaveData();
	void CheckAndRsync();

private:
	UINT32 m_DateType;
	UINT32 m_StartDate;
	UINT32 m_EndDate;
	UINT32 m_Rate;
	UINT32 m_DurationDays;
	bool m_IsNeedSave;

	time_t m_LastUpdateTime;
	HTIMER m_TimeHandler;
};
#endif
