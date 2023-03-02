#ifndef __GUILDSORTER_H__
#define __GUILDSORTER_H__

//#define MAX_SORTER 5 

class Guild;

class ISorter
{
public:
	virtual ~ISorter() {}

	virtual void AddGuild(Guild *pGuild) = 0;
	virtual void RemoveGuild(Guild *pGuild) = 0;
	virtual void Sort() = 0;
	virtual std::vector<Guild *> &GetSortResult() = 0;
	virtual bool HasGuild(UINT64 guildId) = 0;
};

class GuildSorter
{
public:
	GuildSorter();
	~GuildSorter();

public:
	static const UINT32 MAX_SORTER = 5; // ÅÅÐÐ°ñ¸öÊý

	//void Init();
	//void Unint();

	void AddGuild(Guild *pGuild);
	void RemoveGuild(Guild *pGuild);

	void Sort();
	void SetChanged(int sortType);
	std::vector<Guild *> &GetSortResult(int sortType);

	bool IsSorttypeSupport(int sortType);
	UINT32 GetRankNum(UINT32 sortType, UINT64 guildId);
	bool IsGuildInSort(UINT64 guildId);

private:
	std::map<UINT32, ISorter*> m_isorterMap;
	//ISorter *m_Sorters[MAX_SORTER];
};

#endif // __GUILDSORTER_H__