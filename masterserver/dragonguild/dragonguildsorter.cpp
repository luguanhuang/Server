#include "pch.h"
#include "dragonguildsorter.h"
#include "dragonguild.h"
#include "pb/project.pb.h"
#include "account/punishmgr.h"
#include "foreach.h"

typedef void (*AfterSortDragonGuildCB)(std::vector<DragonGuild *> &guilds);

template <typename T>
class TemplateSorter : public IDragonGuildSorter
{
public:

	TemplateSorter(AfterSortDragonGuildCB cb = NULL)
	{
		m_cb = cb;
	}

	~TemplateSorter()
	{
	}

	virtual void AddDragonGuild(DragonGuild *pGuild)
	{
		if (pGuild == NULL)
		{
			return;
		}
		m_SortSet.push_back(pGuild);
		Sort();
	}

	virtual void RemoveDragonGuild(DragonGuild *pGuild)
	{
		std::vector<DragonGuild *>::iterator iter = std::find(m_SortSet.begin(), m_SortSet.end(), pGuild);
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

	virtual std::vector<DragonGuild *> & GetSortResult()
	{
		return m_SortSet; 
	}

	virtual bool HasDragonGuild(UINT64 guildId)
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

	AfterSortDragonGuildCB m_cb;
	std::vector<DragonGuild *> m_SortSet;
};

static void SetDragonGuildRank(std::vector<DragonGuild *> &guilds)
{
	for (UINT32 i = 0; i < guilds.size(); ++i)
	{
		guilds[i]->SetRank(i+1);
	}
}

DragonGuildSorter::DragonGuildSorter()
{
	m_isorterMap.clear();
	m_isorterMap[KKSG::DragonGuildSortByLevel] = new TemplateSorter<DragonGuild::DragonGuildLevelCMP>(SetDragonGuildRank);
	m_isorterMap[KKSG::DragonGuildSortByMemberCount] = new TemplateSorter<DragonGuild::DragonGuildMembersCMP>();
	m_isorterMap[KKSG::DragongGuildSortByTotalPPT] = new TemplateSorter<DragonGuild::DragonGuildPPTCMP>();
	m_isorterMap[KKSG::DragonGuildSortBySceneID] = new TemplateSorter<DragonGuild::DragonGuildSceneID>();
	m_isorterMap[KKSG::DragonGuildSortByName] = new TemplateSorter<DragonGuild::DragonGuildName>();
	m_isorterMap[KKSG::DragonGuildSortByLeaderName] = new TemplateSorter<DragonGuild::DragonGuildLeaderName>();
}

DragonGuildSorter::~DragonGuildSorter()
{
	for (int i = KKSG::GuildSortByLevel; i <= MAX_SORTER; ++i)
	{
		if (m_isorterMap[i])
		{
			delete m_isorterMap[i];
		}
	}
}

void DragonGuildSorter::AddDragonGuild(DragonGuild *pGuild)
{
	
	for (int i = KKSG::GuildSortByLevel; i <= MAX_SORTER; ++i)
	{
		if (m_isorterMap[i])
		{
			m_isorterMap[i]->AddDragonGuild(pGuild);
		}
	}
	
}

bool DragonGuildSorter::IsGuildInSort(UINT64 guildId)
{
	return m_isorterMap[KKSG::DragonGuildSortByLevel]->HasDragonGuild(guildId);
}

void DragonGuildSorter::RemoveDragonGuild(DragonGuild *pGuild)
{
	for (int i = KKSG::GuildSortByLevel; i <= MAX_SORTER; ++i)
	{
		if (m_isorterMap[i])
		{
			m_isorterMap[i]->RemoveDragonGuild(pGuild);
		}
	}
}

void DragonGuildSorter::SetChanged(int sortType)
{
	m_isorterMap[sortType]->Sort();
}

std::vector<DragonGuild *> & DragonGuildSorter::GetSortResult(int sortType)
{
	return m_isorterMap[sortType]->GetSortResult();
}

bool DragonGuildSorter::IsSorttypeSupport(int sortType)
{
	if (sortType < 0 || sortType > MAX_SORTER)
	{
		return false;
	}

	return m_isorterMap[sortType] != NULL;
}

void DragonGuildSorter::Sort()
{
	for (int i = KKSG::GuildSortByLevel; i <= MAX_SORTER; ++i)
	{
		if (m_isorterMap[i])
		{
			m_isorterMap[i]->Sort();
		}
	}
}

UINT32 DragonGuildSorter::GetRankNum(UINT32 sortType, UINT64 guildId)
{
	std::vector<DragonGuild *>& rankList = GetSortResult(sortType);
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


