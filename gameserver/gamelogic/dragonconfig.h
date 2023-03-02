#ifndef __DRAGONCONFIG_H__
#define __DRAGONCONFIG_H__
//#include "table/TeamDragonRewardTable.h"
#include "table/DragonNestTable.h"
#include "table/DragonNestType.h"

struct SceneInfo;

struct DragonSceneInfo
{
	DragonSceneInfo()
	{
		openTime = 0;
		passTime = 0;
	}
	UINT32 openTime;
	UINT32 passTime;
};

class DragonConfig
{
public:
	DragonConfig();
	~DragonConfig();
	DECLARE_SINGLETON(DragonConfig)

	bool Init();
	void Uninit();
	bool LoadFile();
	const DragonNestTable::RowData* GetDragonTableData(int expID);
	const DragonNestTable::RowData* GetDragonTableData(int type, int hard, int floor);
	int GetTopestFloor(int type, int hard);
	const std::string* GetDragonTypeName(int expID);
	const DragonNestType& GetDragonNestType(){return m_dragonTypeTable;}

	int GetHardest(int type);

	void FillGlobalInfo(const std::string& value);
	void UpdateGlobalInfo(UINT32 sceneid);
	// revive
	bool CanLocalRevive(UINT32 sceneid);
	bool CanCrossRevive(UINT32 sceneid, std::set<UINT32>& serverids);
	UINT32 GetCanReviveCD(UINT32 sceneid);

	UINT32 GetGlobalPassTimeByExpID(UINT32 expID);
	UINT32 GetGlobalOpenTimeByExpID(UINT32 expID);
	void CalcDragonWeakBuffInfo(UINT32 expID, UINT32& buffID, UINT32& buffLevel, KKSG::DragonWeakType& type, UINT32& refreshTimes);
	void CalcDragonWeakBuffInfo(UINT32 expID, UINT32 openTime, UINT32 passTime, UINT32& buffID, UINT32& buffLevel, KKSG::DragonWeakType& type, UINT32& refreshTimes);
	static void CheckDragonWeak(UINT32 sceneID, SceneInfo *pSceneInfo, void *arg);
	void SyncWorldLevel(UINT32 level);

    bool IsAllServerNotPass(UINT32 expID);

	bool IsReviveExcept(UINT32 sceneid);

private:
	static void _FillDragonGlobalInfo(const KKSG::GlobalDragonInfo& info, std::map<UINT32, DragonSceneInfo>& id2time);
	static bool _CanRevive(UINT32 sceneid, const std::map<UINT32, DragonSceneInfo>& id2time);

private:
	//std::map<UINT32, UINT32> m_id2time;
	std::map<UINT32, DragonSceneInfo> m_id2time;
	DragonNestTable m_dragonTable;
	DragonNestType m_dragonTypeTable;

};
#endif // 