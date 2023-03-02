#ifndef __ARENASTARMGR_H__
#define __ARENASTARMGR_H__

#include "timer.h"
#include "arenastardb.h"
#include "arenastardata.h"
#include "arenastarbase.h"

class CRankList;
class CRole;

class ArenaStarMgr : public ITimer
{
	ArenaStarMgr();
	~ArenaStarMgr();
	DECLARE_SINGLETON(ArenaStarMgr)

public:
	bool Init();
	void Uninit();
	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);
	void SaveAll2DB();

	ASRoleData* GetRoleData(UINT64 roleID);

	void OnArenaSeasonEnd(KKSG::ArenaStarType type, UINT64 ID = 0);
    void OnLogin(CRole* pRole);

    void AddDataFromDB(KKSG::ArenaStarRoleData& dbData);

	void HandleDianZan(CRole* pRole, KKSG::ArenaStarType type, KKSG::ArenaStarReqRes& roRes);
	void HandleTopRoleData(KKSG::ArenaStarType type, KKSG::ArenaStarReqRes& roRes);

	ArenaStarRankP GetTypeRankP(KKSG::ArenaStarType type);

	void AddChangeRole(UINT64 roleID);

	void GiveDianZanRewardGs(CRole* pRole, KKSG::ArenaStarType type);
	void GiveDianZanRewardMail(const KKSG::MsGiveItemArg& roArg);

	void InfoPrint();
	void ClearAll();
	void Data2Zero();

private:
    void CheckAddRoleAndSeason(UINT64 roleID);
    void AddNewData(UINT64 roleID);
	bool Add(const ASRoleData& data);

    void UpdateTypeSeason(KKSG::ArenaStarType type, UINT32 season);
    UINT32 GetTypeSeason(KKSG::ArenaStarType type);

	bool ReadSeasonGlobalData();
	void WriteSeasonGlobalData();
	UINT32 GetSeasonEndTime(KKSG::ArenaStarType type, UINT32 season);
	void SetSeasonEndTime(KKSG::ArenaStarType type, UINT32 season, UINT32 ctime);

	void FillSeasonTime(KKSG::ArenaStarType type, KKSG::ArenaStarReqRes& roRes);

	void SeasonPrint();
	void RolePrint();
	void GlobalDataPrint();

	void CheckRoleNotice(CRole* pRole);

	std::unordered_map<UINT64, ASRoleData> m_roleData;//唯一数据
    std::unordered_map<int, UINT32> m_typeSeason;//根据m_roleData生成
	KKSG::ASSeasonGlobalData m_seasonTime;//只做时间存储和展示

	ArenaStarDbH m_dbHander;

	HTIMER m_handler;	
};

#endif
