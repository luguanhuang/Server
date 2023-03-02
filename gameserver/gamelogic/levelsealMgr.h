#ifndef __LEVELSEAL_MGR_H__
#define __LEVELSEAL_MGR_H__

#include "table/LevelSeal.h"
#include "table/LevelSealTypeTable.h"
#include "pb/project.pb.h"
#include "unit/role.h"
#define SendLevelSealBossCountInterval 3

struct stServerLevelSealInfo
{
	UINT32 m_uType;
	UINT32 m_uLevel;
	UINT32 m_uStartTime;
	UINT32 m_uEndTime;
	UINT32 m_uTotalCollectCount;//服务器总激活数;
	UINT32 m_uIncCollectCount;
	UINT32 m_uLastUpdateTime;

	stServerLevelSealInfo()
	{
		m_uType = 0;
		m_uLevel = 0;
		m_uStartTime = 0;
		m_uEndTime = 0;
		m_uTotalCollectCount = 0;
		m_uIncCollectCount = 0;
		m_uLastUpdateTime = 0;
	}
};
class CLevelSealMgr : public ITimer
{
	CLevelSealMgr();
	~CLevelSealMgr();
	DECLARE_SINGLETON(CLevelSealMgr);
public:
	bool Init();
	void Uninit();
	bool LoadFile();
	bool CheckFile();
	void ClearFile();
	float GetPlayerExtraBuff(UINT32 uLevel, UINT64 roleid);
	const stServerLevelSealInfo& GetLevelSealInfo();
	void SetCollectCount(UINT32 uCount);
	void NotifyLevelSealInfo(Role* role);
	void SetSealInfo(KKSG::MsLevelSealInfo m_oData);
	INT32 ExchangeGift(Role* pRole);
	UINT32 GetUnLockTimeByType(UINT32 uType);
	void GetUnLockInfo(std::map<UINT32, UINT32>& oUnlockInfoList);
	INT32 GetSelfGift(Role* pRole, INT32 iIndex);
	void BackAward(Role* pRole, INT32 iType);//检查是否需要补发奖励;
	UINT32 GetUpdateTime();
	void NotifyClient(Role* pRole);

	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);
	LevelSealTypeTable::RowData* GetSealTypeTableData(UINT32 type);

	UINT32 GetThisTimeLevelSealType(time_t thisTime);

private:
	LevelSeal m_oTable;
	LevelSealTypeTable m_oTypeTable;
	stServerLevelSealInfo m_SealInfo;
	KKSG::LevelSealUnlockInfo m_oUnlockInfo;
	HTIMER m_TimeHandler;
};

#endif
