#ifndef __GUILDBOSSRANK_H__
#define __GUILDBOSSRANK_H__

#include "ranklist.h"

#define GUILDBOSSRANKCOUNT 16

class GuildBossRankInfo : public CRankInfo
{
public:
	GuildBossRankInfo(UINT64 guildid);

	virtual void Reset();
	virtual void RankInfoToKKSG(KKSG::RankData* pRankData) const;
	virtual void RankInfoToDBKKSG(KKSG::RankData* pRankData) const;
	virtual void DBKKSGToRankInfo(const KKSG::RankData* pRankData);	

	double m_damage;               // 总伤害
	UINT32 m_time;                 // 击杀时间
	UINT32 m_bossIndex;           // 工会boss出场顺序id
	std::string m_guildname;       // 公会名字
	std::string m_dpsMaxrole;      // 最强输出玩家名字
	std::string m_bossName;   // 公会boss名字
};

class GuildBossRankCompare
{
public:
	bool operator()(GuildBossRankInfo* pa, GuildBossRankInfo* pb)
	{
		if (pa->m_bossIndex > pb->m_bossIndex)
		{
			return true;
		}
		else if (pa->m_bossIndex == pb->m_bossIndex)
		{
			if (pa->m_damage > pb->m_damage)
			{
				return true;
			}
			else if (pa->m_damage == pb->m_damage)
			{
				if(pa->m_time == pb->m_time)
				{
					return pa->GetUniqueId() < pb->GetUniqueId();
				}
				return pa->m_time < pb->m_time;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
};

/////////////////////////////////////////////////////////////////////////////////////
class GuildBossRankDb : public RankDbHandler
{
	virtual void PackDBData(KKSG::RankList* ranklist);
	virtual void UnpackDBData(const KKSG::RankData& rankdata, UINT32 rank);
};

class GuildBossRankList : public CRankList
{
public:
	GuildBossRankList(int type);
	virtual ~GuildBossRankList();

	void Clear();
	
	virtual inline UINT32 GetClientQueryCount() { return GUILDBOSSRANKCOUNT; }

	virtual UINT32 GetNeedRankCount();
	virtual UINT32 GetRank(UINT64 guildid);
	virtual UINT32 GetSize();
	const GuildBossRankInfo* GetInfoByRank(UINT32 rank);

	virtual void RankListToClient(KKSG::RankList* rankdatas, UINT32 count, UINT32 profession);
	virtual void RankInfoToClient(KKSG::RankData* rankdata, UINT32 rank);

	void Update(UINT64 guildid, UINT32 bossIndex, UINT32 time, double damage, std::string dpsRole, std::string bossName);
	
	void InsertKey(GuildBossRankInfo* rankinfo);

	void GetRankInfo(std::vector<GuildBossRankInfo*>& vec);

	void SaveToDb();

	void DeleteGuild(UINT64 guildid);

private:
	RankTree<GuildBossRankInfo*, GuildBossRankCompare> m_rankTree;
};
#endif