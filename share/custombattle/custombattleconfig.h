#ifndef __CUSTOMBATTLECONFIG_H__
#define __CUSTOMBATTLECONFIG_H__

#include "table/CustomBattleSystemTable.h"
#include "table/CustomBattleTable.h"
#include "table/CustomBattleTypeTable.h"
#include "table/CustomRewardTable.h"
#include "table/CustomSystemRewardTable.h"
#include "table/PkParamTable.h"
#include "table/PkPrepareTable.h"

struct ItemDesc;
class CCustomBattleConfig
{
public:
	CCustomBattleConfig();
	~CCustomBattleConfig();
	DECLARE_SINGLETON(CCustomBattleConfig);
public:
	bool Init();
	void Uninit();

	bool LoadFile();
	void ClearFile();
	bool CheckFile();

	bool IsSystem(UINT64 uid);

	bool IsLevelLimit(UINT32 config, UINT32 level);

	UINT32 WinLimit(UINT32 configid);
	UINT32 LoseLimit(UINT32 configid);

	bool IsGMCreate(UINT32 configid);
	bool IsGMJoin(UINT32 configid);
	KKSG::ErrorCode IsValidCreate(const KKSG::CustomBattleConfig& config);
	KKSG::ErrorCode FillCreate(const KKSG::CustomBattleConfig& config, UINT64 roleid, const std::string& name, KKSG::CustomBattleConfig& serverconfig, bool isgm);

	std::vector<ItemDesc> GetJoinCost(UINT32 configid);
	std::vector<ItemDesc> GetGenerateCost(UINT32 configid);

	std::vector<ItemDesc> GetClearSystemCD(UINT32 configid, UINT32 win);
	std::vector<ItemDesc> GetClearCustomCD(UINT32 configid, UINT32 rank);

	UINT32 GetSystemRewardCD(UINT32 confiGgid, UINT32 win);
	UINT32 GetCustomRewardCD(UINT32 configid, UINT32 rank);

	std::vector<ItemDesc> GetSystemReward(UINT32 configid, UINT32 win);
	std::vector<ItemDesc> GetCustomReward(UINT32 configid, UINT32 rank);

	const CustomBattleSystemTable& GetTable() { return m_table; }

	UINT32 GetParam(UINT32 point, bool iswin);
	int GetPkPoint(int basepoint, int pointspan, int result);

	KKSG::ErrorCode ValidPassword(const std::string& password);

	std::string GenerateToken(); 
	char RandomChar(); 

	bool IsPvpTwo(UINT32 configid);
	void FillPkStage(UINT32 point, KKSG::CustomBattleMatchRoleInfo& data, KKSG::PkNVNType type = KKSG::PK_1v1);

private:
	CustomBattleSystemTable::RowData* GetSystemData(UINT32 configid);
	CustomBattleTable::RowData* GetData(UINT32 configid);

private:
	CustomBattleTable m_customtable;
	CustomBattleTypeTable m_typetable;
	CustomBattleSystemTable m_table;

	CustomRewardTable m_rewardtable;
	CustomSystemRewardTable m_systemrewardtable;

	PkParamTable m_pktable;	
	PkPrepareTable m_pkpreparetable;	
};

#endif