#ifndef __CRANKLISTMGR_H__
#define __CRANKLISTMGR_H__

#include "timermgr.h"
#include "pb/project.pb.h"

class CRoleSummary;
class CRankList;
class LeagueRankList;
class CrossLeagueRankList;
class SkyCraftRankList;
class SkyCityRankList;

class CRankListMgr : public ITimer
{
public:
	CRankListMgr();
	~CRankListMgr();
	DECLARE_SINGLETON(CRankListMgr);

	bool Init();
	void Uninit();

	bool QueryDB();

	virtual void OnTimer(UINT32 id, UINT32 count);

	CRankList* GetRankList(UINT32 type, UINT32 serverid = 0);

	void SaveAllToDB();

	void OnChangeBrief(UINT64 roleid, UINT32 ppt, UINT32 ppttime, UINT32 level, UINT32 leveltime);
	void OnChangeFlowerCount(UINT64 roleid, UINT32 count, std::map<UINT32, UINT32>& flowers, UINT32 time);
	void OnChangeFlowerThisWeekCount(UINT64 roleid, UINT32 count, std::map<UINT32, UINT32>& flowers, UINT32 time);
	void OnChangeFlowerYestodayCount(UINT64 roleid, UINT32 yestodaycount, std::map<UINT32, UINT32>& flowers, UINT32 time);
	void OnChangeFlowerActivityCount(UINT64 roleid, UINT32 yestodaycount, std::map<UINT32, UINT32>& flowers, UINT32 time);
	void OnChangeFashionPower(UINT64 roleid, UINT32 ppt, UINT32 level, UINT32 time);	
	void OnChangeTowerRank(std::vector<UINT64>& roleids, UINT32 towerHardLevel, UINT32 towerFloor, UINT32 useTime, UINT32 time);
	void OnChangeFlowerTotalCount(UINT64 roleid, UINT32 total, std::map<UINT32, UINT32>& flowers, UINT32 time);
	void OnChangeSpritePowerPoint(UINT64 roleid, UINT32 powerpoint, UINT32 time);
	void OnChangePetPowerPoint(UINT64 petuid, UINT32 petid, UINT64 roleid, UINT32 powerpoint, UINT32 time);
	std::map<int,int> OnChangeFirstPassRank(std::vector<UINT64>& roleids, int firstPassID, UINT32 starLevel);

	void SendTShowTop3Rank();
	void SendTShowSelfRank(CRoleSummary* role);

	UINT32 GetFirstPassRankKey(int firstPassID);
	bool IsFirstPassRankKey(UINT32 type);

	void DeleteTheOneInRankList(UINT64 roleid, KKSG::PunishType type);

	UINT32 GetPunishType(KKSG::RankeType type);

	void GMClearFirstPass();

	LeagueRankList* GetLeagueRankList();
	CrossLeagueRankList* GetCrossLeagueRankList();
	SkyCraftRankList* GetSkyCraftRankList();
	SkyCityRankList* GetSkyCityRankList();


	void DebugSetFlowerRankNum(UINT32 num);
	void OnSartUp();
	bool OnWeekUpdate();
	bool OnWeekEvent(UINT32 nTime);
	bool OnTwoWeekEvent(UINT32 nTime);
	UINT32 DebugGetFlowerRankNum(){return m_debugFlowerRankNum;}

private:
	bool _QueryDBToRankList(CRankList* ranklist);
private:
	HTIMER m_timeHandler;
	std::map<UINT32, std::map<UINT32, CRankList*>> m_rankLists;			// key : type, serverid->ranklist
	//std::map<std::pair<UINT32, UINT32>, UINT32> m_uniqServerIDMap;	// key : type + serverid -> real serverid

	UINT32 m_debugFlowerRankNum;
	UINT32 m_weektimeHandle;
	UINT32 m_lastUpdateTime;
	std::map<KKSG::RankeType, KKSG::PunishType> m_rank2punish;
};

#endif
