#ifndef __MATCHMACHINE_H__
#define __MATCHMACHINE_H__

/*
	问题：一个队所需人数为x，给出一些人数<=x的队伍，组合出最多的完整队伍。
	这个问题是NP的，没有多项式时间算法。
	近似的解决方案是用贪心去匹配。

	如人数为4的队伍，可以由 4，3+1，2+2，2+1+1，1+1+1+1 组成，
	如人数为5的队伍，可以由 5，4+1，3+1，3+1+1，2+2+1，2+1+1+1，1+1+1+1+1 组成。
	贪心策略就是优先使用人多的队伍，因为人少的队伍可以有更多的选择。
	在所需人数小于等于6的情况下，手工模拟没有找出反例，是最优的方案。

	若所需人数为7，反例:4+1+1+1，3+3+1，4的队伍有1个，3的队伍有6个，1的队伍有3个。
	由于是先使用4+1+1+1的情况，只组成了一组完整队伍。
	如果使用3+3+1,就可以组成3组完整队伍。
*/

enum MatchMachineType
{
	MatchMachineType_Unknown,
	MatchMachineType_HeroBattle,
	MatchMachineType_MobaBattle,
};

class MatchMachine
{
public:
	MatchMachine();
	~MatchMachine();
	void Init(UINT32 num, MatchMachineType type = MatchMachineType_Unknown);	

	void Clear();

	bool AddMatchUnit(UINT64 uid, UINT32 num);
	bool DelMatchUnit(UINT64 uid);

	///> 匹配成功在result中的就直接删除了
	void DoMatch(std::vector<std::vector<UINT64>>& result);
	
	bool GMChangeNum(UINT32 num);

	void PrintLog();

private:
	std::set<std::pair<time_t, UINT64>>* _GetUnitSet(UINT64 uid);

	static void _BuildMatchTemplate();
	static void _DfsBuild(UINT32 dep, UINT32 val, std::vector<std::vector<UINT32>>& temp, std::vector<UINT32>& vec);

private:
	UINT32 m_number;
	MatchMachineType m_type;

	std::vector<std::set<std::pair<time_t, UINT64>>> m_units;
	std::map<UINT64, time_t> m_units2time;

	static std::unordered_map<UINT32, std::vector<std::vector<UINT32>>> m_stMatchTemplate;
};



#endif