#ifndef __RANKLIST_H__
#define __RANKLIST_H__

#include "saverankpolicy.h"
#include "time.h"
#include "util/ranktree.h"

#define CLIENTQUERYCOUNT 100			///> 客户端默认查询排行榜条数
#define INVALID_RANK (UINT32)(-1)		///> 非法排名

enum UpdateInterVal
{
	ArenaUpdateInterVal			= 289,
	DEProgressUpdateInterVal	= 290,
	PetUpdateInterVal			= 292,
	PkUpdateInterVal			= 293,
	SpriteUpdateInterVal		= 294,
	TowerUpdateInterVal			= 295,
	NestWeekUpdateInterval      = 296,
	LeagueTeamRankInterval		= 297,
	CrossLeagueTeamRankInterval	= 298,
	HeroBattleUpdateInterval	= 299,
	MilitaryUpdateInterval		= 300,
	LastWeek_PkUpdateInterval			= 301,
	LastWeek_NestWeekUpdateInterval      = 302,
	LastWeek_LeagueTeamRankInterval		= 303,
	LastWeek_HeroBattleUpdateInterval	= 304,
	SkyCraftRankInterval		= 305,
	PK2v2UpdateInterval         = 306,
	BioHelllpdateInterval      =  307,
	CompeteDragonUpdateInterval			= 308,
	SurviveUpdateInterval        = 309,
	SkyCityUpdateInterval		 = 310,
};

namespace KKSG
{
	class RankData;
	class RankList;
	class RankListRawData;
}
class CRankInfo
{
public:
	CRankInfo(UINT64 uniqueid) { m_uniqueid = uniqueid; }
	virtual ~CRankInfo() { }

	inline UINT64 GetUniqueId() const { return m_uniqueid; }
	inline void SetUniqueId(UINT64 uniqueid) { m_uniqueid = uniqueid; }

	virtual void Reset() = 0;
	virtual void RankInfoToKKSG(KKSG::RankData* rankdata) const = 0;
	virtual void RankInfoToDBKKSG(KKSG::RankData* rankdata) const = 0;
	virtual void DBKKSGToRankInfo(const KKSG::RankData* rankdata) = 0;

private:
	UINT64 m_uniqueid;
};

//////////////////////////////////////////////////////////////////////////////////
class CRankList;
class IRankHandler
{
public:
	void Init(CRankList* ranklist) { m_ranklist = ranklist; }

protected:
	CRankList* m_ranklist;
};

class RankDbHandler : public IRankHandler
{
public:
	RankDbHandler();
	virtual ~RankDbHandler();

public:
	virtual void PackDBData(KKSG::RankList* ranklist) = 0;
	virtual void UnpackDBData(const KKSG::RankData& rankdata, UINT32 rank) = 0;

	virtual bool BeforeSaveToDB() { return false; }

	inline bool IsEmptyInDB() { return m_emptyindb; }
	inline bool IsFinishLoad() { return m_finishload; }
	inline void SetEmptyInDB() { m_emptyindb = true; }
	inline void SetFinishLoad() { m_finishload = true; }

	inline void SetPolicy(SaveToDBPolicy* policy) { m_savepolicy = policy; }

	bool CheckTimeToSaveDB();

	void SendToDB();
	void SendRawToDB(std::string& rawdata);

	void ReceiveFromDB(UINT32 rankType, const KKSG::RankList& rankList);
	//void ReceiveFromDB(const KKSG::RankListRawData& partrawdata);

private:
	bool m_emptyindb;
	bool m_finishload;
	SaveToDBPolicy* m_savepolicy;
	std::string m_rawdata;
};

//////////////////////////////////////////////////////////////////////////////////
class CRankList
{
public:
	CRankList(UINT32 type, UINT32 serverid = 0);
	virtual ~CRankList();

	virtual void Clear(){}			// 内存析构
	virtual void RemoveAll() {}		// 清榜并通知DB删数据
	
	virtual UINT32 GetNeedRankCount() = 0;
	virtual UINT32 GetRank(UINT64 roleid) = 0;
	virtual UINT32 GetSize() = 0;

	virtual void RankListToClient(KKSG::RankList* rankdatas, UINT32 count, UINT32 professsion = 0) = 0;
	virtual void RankInfoToClient(KKSG::RankData* rankdata, UINT32 rank) = 0;
	virtual void NewRankInfoToClient(KKSG::RankData* rankdata, UINT64 uniqeID){}

	virtual inline UINT32 GetClientQueryCount() { return CLIENTQUERYCOUNT; }

	inline UINT32 GetType() { return m_type; }
	inline UINT32 GetServerID() { return m_serverid; }
	inline UINT32 GetTimeStamp() { return m_timestamp; }
	inline void SetTimeStamp() { m_timestamp = (UINT32)time(NULL); }
	inline RankDbHandler* GetDbHandler() { return m_db; }

	void RegisterDb(RankDbHandler* db);

protected:
	UINT32 m_type;
	UINT32 m_serverid;
	UINT32 m_timestamp;

	RankDbHandler* m_db;
};

#endif
