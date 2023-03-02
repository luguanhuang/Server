#ifndef __CUSTOMBATTLERANDOM_H__
#define __CUSTOMBATTLERANDOM_H__

class CustomBattle;
struct CustomBattleRandomUnit
{
	// 31-16:friend, 15-0:guild
	UINT32 value; 
	CustomBattle* battle;	

	bool operator<(const CustomBattleRandomUnit& unit) const
	{
		if (value != unit.value)
		{
			return value < unit.value;
		}
		else
		{
			return battle < unit.battle;
		}
	}
};

class CustomBattleRandom
{
public:
	CustomBattleRandom();
	~CustomBattleRandom();

	void Add(CustomBattle* battle, UINT32 frienddegree, bool isfriend, bool isguild);
	std::vector<CustomBattle*> Get();

public:
	std::vector<CustomBattle*> m_randoms;
	std::set<CustomBattleRandomUnit> m_randomrelated;
};

#endif
