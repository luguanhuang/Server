#ifndef __DRAGONGUILDSORTER_H__
#define __DRAGONGUILDSORTER_H__

//#define MAX_SORTER 5 

class DragonGuild;

class IDragonGuildSorter
{
public:
	virtual ~IDragonGuildSorter() {}

	virtual void AddDragonGuild(DragonGuild *pGuild) = 0;
	virtual void RemoveDragonGuild(DragonGuild *pGuild) = 0;
	virtual void Sort() = 0;
	virtual std::vector<DragonGuild *> &GetSortResult() = 0;
	virtual bool HasDragonGuild(UINT64 guildId) = 0;
};

class DragonGuildSorter
{
public:
	DragonGuildSorter();
	~DragonGuildSorter();

public:
	static const UINT32 MAX_SORTER = 6; // ÅÅÐÐ°ñ¸öÊý

	//void Init();
	//void Unint();

	void AddDragonGuild(DragonGuild *pGuild);
	void RemoveDragonGuild(DragonGuild *pGuild);

	void Sort();
	void SetChanged(int sortType);
	std::vector<DragonGuild *> &GetSortResult(int sortType);

	bool IsSorttypeSupport(int sortType);
	UINT32 GetRankNum(UINT32 sortType, UINT64 guildId);
	bool IsGuildInSort(UINT64 guildId);

private:
	std::map<UINT32, IDragonGuildSorter*> m_isorterMap;
	//ISorter *m_Sorters[MAX_SORTER];
};

#endif // __GUILDSORTER_H__