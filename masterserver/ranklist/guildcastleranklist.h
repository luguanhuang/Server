#ifndef __GUILDCASTLERANKLIST_H__
#define __GUILDCASTLERANKLIST_H__

#include "ranklist.h"

class GuildCastleRankInfo 
{
public:
	GuildCastleRankInfo();
	~GuildCastleRankInfo();

	void DoTxLog();
	void SetData(UINT64 roleID, UINT32 killCount, UINT32 zlCount);
	void FillData(KKSG::GCFRoleBrief& data, UINT32 rank);

	inline UINT64 RoleID() const { return m_roleID; }
	inline UINT32 KillCount() const { return m_killCount; }
	inline UINT32 ZLCount() const { return m_zlCount; }
	inline UINT32 GetFeats() const {return m_feats; }

	void TestPrint();

private:
	UINT64 m_roleID;
	UINT32 m_killCount;
	UINT32 m_zlCount;
	UINT32 m_feats;
};

struct GuildCastleCompare
{
	bool operator () (const GuildCastleRankInfo* info1, const GuildCastleRankInfo* info2)
	{
		if (info1->GetFeats() != info2->GetFeats())
		{
			return  (info1->GetFeats() > info2->GetFeats());
		}
		if (info1->KillCount() != info2->KillCount())
		{
			return (info1->KillCount() > info2->KillCount());
		}
		if (info1->ZLCount() != info2->ZLCount())
		{
			return (info1->ZLCount() > info2->ZLCount());
		}
		if(info1->RoleID() != info2->RoleID())
		{
			return  (info1->RoleID() > info2->RoleID());
		}
		return false;
	}
};

class GuildCastleRankList 
{
public:
	GuildCastleRankList();
	~GuildCastleRankList();

	void Clear();
	
	void OnChange(UINT64 roleID, UINT32 killCount, UINT32 zlCount);
	void GetAllInfo(std::vector<GuildCastleRankInfo*>& vec);
	UINT32 GetRank(UINT64 roleID);
	UINT32 GetFeats(UINT64 roleID);
	void FillRankData(KKSG::GCFCommonRes& data, UINT64 roleID);
	void FillMyRank(KKSG::GCFRoleBrief& myInfo, UINT64 roleID);

	UINT32 AddKill(UINT64 roleID);
	void AddOccupy(UINT64 roleID);

	void TestPrint();

private:
	void GetOldCount(UINT64 roleID, UINT32& killCount, UINT32& occupyCount);


	RankTree<GuildCastleRankInfo*, GuildCastleCompare> m_rankTree;
};

#endif
