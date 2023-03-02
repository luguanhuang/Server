#ifndef __GUILDCAMPRANK_H__
#define __GUILDCAMPRANK_H__


#define CAMPRANKCOUNT 100

namespace KKSG
{
	class GuildCampInfoRes;
	class GuildCampRankInfo;
};

class CGuildCamp;

class CGuildCampRankInfo
{
public:
	CGuildCampRankInfo(std::vector<UINT64>& roleids, bool isAscend);
	~CGuildCampRankInfo();

	std::set<UINT64>& GetRoleIDs(){return m_roleIDs;}
	bool Convert2KKSGGuildCampRankInfo(KKSG::GuildCampRankInfo& info) const;

public:
	bool m_isAscend;
	int m_var;
	UINT32 m_timestamp;
	std::set<UINT64> m_roleIDs;
};

struct GuildCampRankInfoCompare
{
	bool operator () (const CGuildCampRankInfo a, const CGuildCampRankInfo b)
	{
		const CGuildCampRankInfo* rankinfoa = &a;
		const CGuildCampRankInfo* rankinfob = &b;
		if (rankinfoa->m_roleIDs.empty() || rankinfob->m_roleIDs.empty())
		{
			return false;
		}
		if (a.m_isAscend != b.m_isAscend)
		{
			return false;
		}
		if (rankinfoa->m_roleIDs.size() == rankinfob->m_roleIDs.size())
		{
			bool isequal = true;
			for (auto iter = rankinfoa->m_roleIDs.begin(), iterb = rankinfob->m_roleIDs.begin();\
				iter != rankinfoa->m_roleIDs.end() && iterb != rankinfob->m_roleIDs.end(); ++iter, ++iterb)
			{
				if ((*iter) != (*iterb))
				{
					isequal = false;
					break;
				}
			}
			if (isequal)
			{
				return false;
			}
		}
		if (rankinfoa->m_isAscend && rankinfoa->m_var < rankinfob->m_var)
		{
			return true;
		}
		else if (!rankinfoa->m_isAscend && rankinfoa->m_var > rankinfob->m_var)
		{
			return true;
		}
		else if (rankinfoa->m_var == rankinfob->m_var)
		{
			if (rankinfoa->m_timestamp < rankinfob->m_timestamp)
			{
				return true;
			}
			else if (rankinfoa->m_timestamp == rankinfob->m_timestamp)
			{
				if (rankinfoa->m_roleIDs.size() < rankinfob->m_roleIDs.size())
				{
					return true;
				}
				else if (rankinfoa->m_roleIDs.size() == rankinfob->m_roleIDs.size())
				{
					if (*rankinfoa->m_roleIDs.begin() < *rankinfob->m_roleIDs.begin())
					{
						return true;
					}
				}
			}
		}
			
		return false;
	}
};

class GuildCampRankList
{
public:
	GuildCampRankList(CGuildCamp* camp);
	virtual ~GuildCampRankList();

	virtual void Clear();

	void LoadFromDb(const KKSG::GuildCampSave& guildCampInfo);

	void SaveDb(KKSG::GuildCampSave& guildCampInfo);

	void OnChange(std::vector<UINT64>& roleids, int var, UINT32 timestamp);	

	void FillRankInfo2Client(KKSG::GuildCampInfoRes& roRes);

	void SetRankType(UINT32 type);

	void OnDayPass();

private:
	void SetChanged();

private:
	CGuildCamp* m_guildCamp;
	bool m_isAscend;
	std::set<CGuildCampRankInfo, GuildCampRankInfoCompare> m_rankinfos;
};

#endif
