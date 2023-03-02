#ifndef __LEVELSEAL_MGR_H__
#define __LEVELSEAL_MGR_H__

#include "table/LevelSeal.h"
#include "table/LevelSealTypeTable.h"
#include "table/LevelSealBossBuffTable.h"
#include "pb/project.pb.h"
#include "table/TeamCostScale.h"

#define SendLevelSealInfoInterval	3

struct stServerLevelSealInfo
{
	UINT32 m_uType;
	UINT32 m_uLevel;
	UINT32 m_uStartTime;
	UINT32 m_uEndTime;
	UINT32 m_uTotalCollectCount;//当前封印等级收集的封印货币个数;

	stServerLevelSealInfo()
	{
		m_uType = 0;
		m_uLevel = 0;
		m_uStartTime = 0;
		m_uEndTime = 0;
		m_uTotalCollectCount = 0;
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
	void Reset();
	bool LoadFile();
	bool CheckFile();
	void ClearFile();
	void InitTypeInfoFromDB();
	void InitTypeInfoFromCfg(UINT32 uType);
	const stServerLevelSealInfo& GetLevelSealInfo();
	UINT32 OperTime(UINT32 uTime);
	void SendLevelSealInfoToDB();
	bool CheckLevelSealInfo();
	void SetCollectCount(UINT32 uCount);
	void SetType(UINT32 uType);
	void SetTime(UINT32 uTime);
	void SetStartServerSaveFlag(bool bFlag);
	bool GetStartServerSaveFlag();
	void NotifyLevelSealInfo(bool isGmSet=false);
	void SetUnLockTime(UINT32 uType, UINT32 uTime);
	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);
	const LevelSealTypeTable::RowData* GetLevelSealTypeData(UINT32 uType);
	UINT32 GetLevelSealMaxLevelByType(UINT32 uType);
	void GetSealInfoByOpenTime();
	UINT32 GetSealInitType();

	bool CanGetTeamCost(UINT32 sealtype, UINT32 worldlevel, UINT32 rolelevel);

private:
	TeamCostScale m_teamcosttable;

	LevelSealTypeTable m_oTypeTable;
	stServerLevelSealInfo m_SealInfo;
	bool m_bStartServerSave;//启动服务器的时候是否需要回写数据;
	std::map<UINT32, UINT32> m_vUnlockInfo;
	HTIMER m_TimeHandler;
};

#endif
