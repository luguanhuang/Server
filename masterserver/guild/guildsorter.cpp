#include "pch.h"
#include "guildsorter.h"
#include "guild.h"
#include "pb/project.pb.h"
#include "account/punishmgr.h"
#include "foreach.h"

typedef void (*AfterSortGuildCB)(std::vector<Guild *> &guilds);

template <typename T>
class TemplateSorter : public ISorter
{
public:

	TemplateSorter(AfterSortGuildCB cb = NULL)
	{
		m_cb = cb;
	}

	~TemplateSorter()
	{
	}

	virtual void AddGuild(Guild *pGuild)
	{
		if (pGuild == NULL)
		{
			return;
		}
		m_SortSet.push_back(pGuild);
		Sort();
	}

	virtual void RemoveGuild(Guild *pGuild)
	{
		std::vector<Guild *>::iterator iter = std::find(m_SortSet.begin(), m_SortSet.end(), pGuild);
		if (iter == m_SortSet.end())
		{
			return ;
		}

		m_SortSet.erase(iter);
		Sort();
	}

	virtual void Sort()
	{
		std::sort(m_SortSet.begin(), m_SortSet.end(), T());

		if (m_cb != NULL)
		{
			m_cb(m_SortSet);
		}
	}

	virtual std::vector<Guild *> & GetSortResult()
	{
		return m_SortSet; 
	}

	virtual bool HasGuild(UINT64 guildId)
	{
		foreach (i in m_SortSet)
		{
			if ((*i)->GetID() == guildId)
			{
				return true;
			}
		}
		return false;
	}

protected:

	AfterSortGuildCB m_cb;
	std::vector<Guild *> m_SortSet;
};

static void SetGuildRank(std::vector<Guild *> &guilds)
{
	for (UINT32 i = 0; i < guilds.size(); ++i)
	{
		guilds[i]->SetRank(i+1);
	}
}

GuildSorter::GuildSorter()
{
	m_isorterMap.clear();
	m_isorterMap[KKSG::GuildSortByLevel] = new TemplateSorter<Guild::GuildLevelCMP>(); 
	m_isorterMap[KKSG::GuildSortByMemberCount] = new TemplateSorter<Guild::GuildMemberCountCMP>();
	m_isorterMap[KKSG::GuildSortByName] = new TemplateSorter<Guild::GuildNameCMP>(); 
	m_isorterMap[KKSG::GuildSortByExp] = new TemplateSorter<Guild::GuildExpCMP>(); 
	m_isorterMap[KKSG::GuildSortByPrestige] = new TemplateSorter<Guild::GuildPrestigeCMP>(SetGuildRank); 

	/*
	memset(m_Sorters, 0, sizeof(m_Sorters));
	
	m_Sorters[KKSG::GuildSortByLevel] = new TemplateSorter<Guild::GuildLevelCMP>(SetGuildRank);
	m_Sorters[KKSG::GuildSortByMemberCount] = new TemplateSorter<Guild::GuildMemberCountCMP>();
	m_Sorters[KKSG::GuildSortByName] = new TemplateSorter<Guild::GuildNameCMP>();
	m_Sorters[KKSG::GuildSortByExp] = new TemplateSorter<Guild::GuildExpCMP>();
	*/
}

GuildSorter::~GuildSorter()
{
	for (int i = KKSG::GuildSortByLevel; i <= MAX_SORTER; ++i)
	{
		if (m_isorterMap[i])
		{
			delete m_isorterMap[i];
		}
	}
}

void GuildSorter::AddGuild(Guild *pGuild)
{
	for (int i = KKSG::GuildSortByLevel; i <= MAX_SORTER; ++i)
	{
		if (i == KKSG::GuildSortByPrestige) // ¼ì²éÊÇ·ñ±»½û·â
		{
			KKSG::PunishData punishData;
			if (CPunishMgr::Instance()->GetPunishInfo(pGuild->GetID(), PUNISH_USER_GUILD_RANK, punishData))
			{
				if (!punishData.has_rankdata() || punishData.rankdata().empty())
				{
					break ;
				}
			}
		}
		if (m_isorterMap[i])
		{
			m_isorterMap[i]->AddGuild(pGuild);
		}
	}
}

bool GuildSorter::IsGuildInSort(UINT64 guildId)
{
	return m_isorterMap[GuildSortByPrestige]->HasGuild(guildId);
}

void GuildSorter::RemoveGuild(Guild *pGuild)
{
	for (int i = KKSG::GuildSortByLevel; i <= MAX_SORTER; ++i)
	{
		if (m_isorterMap[i])
		{
			m_isorterMap[i]->RemoveGuild(pGuild);
		}
	}
}

void GuildSorter::SetChanged(int sortType)
{
	m_isorterMap[sortType]->Sort();
}

std::vector<Guild *> & GuildSorter::GetSortResult(int sortType)
{
	return m_isorterMap[sortType]->GetSortResult();
}

bool GuildSorter::IsSorttypeSupport(int sortType)
{
	if (sortType < 0 || sortType > MAX_SORTER)
	{
		return false;
	}

	return m_isorterMap[sortType] != NULL;
}

void GuildSorter::Sort()
{
	for (int i = KKSG::GuildSortByLevel; i <= MAX_SORTER; ++i)
	{
		if (m_isorterMap[i])
		{
			m_isorterMap[i]->Sort();
		}
	}
}

UINT32 GuildSorter::GetRankNum(UINT32 sortType, UINT64 guildId)
{
	std::vector<Guild *>& rankList = GetSortResult(sortType);
	if (rankList.empty())
	{
		return 0;
	}

	UINT32 rank = 0;
	for (auto iter = rankList.begin(); iter != rankList.end(); ++iter)
	{
		++rank;
		if ((*iter)->GetID() == guildId)
		{
			return rank;
		}
	}

	return 0;
}


