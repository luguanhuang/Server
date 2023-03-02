#ifndef __SKILLDEFINE_H__
#define __SKILLDEFINE_H__

struct skillInfo
{
	UINT32 skillhash;
	UINT32 skilllevel;
	UINT32 skillpoint;
	bool   isbasic;
	skillInfo()
	{
		skillhash = 0;
		skilllevel = 0;
		skillpoint = 0;
		isbasic = false;
	}
};

class SkillFlags
{
public:
	SkillFlags()
	:m_Flag(1)
	{
	}

	inline bool IsFlagSet(UINT32 flag) const
	{
		return (m_Flag & ((UINT64)1 << flag)) != 0;
	}

	inline void SetFlag(UINT32 flag)
	{
		m_Flag = m_Flag | ((UINT64)1 << flag);
	}

	void SetFlag(std::vector<UINT32> flags)
	{
		for (UINT32 i = 0; i < flags.size(); ++i)
		{
			SetFlag(flags[i]);
		}
	}

	void Reset() { m_Flag = 1; }

private:
	UINT64 m_Flag;
};

class Unit;
class XEntity;
class ISkillSet 
{
public:
	virtual ~ISkillSet(){}
	virtual UINT32 GetSkillLevel(UINT32 skillhash) = 0; 
	virtual const SkillFlags* GetSkillFlags() { return NULL; }
	virtual void RefreshSelfLinkedLevels(Unit* pUnit) {}
	virtual void RefreshMobLinkedLevels(Unit* pUnit, Unit* pHoster) {}
protected:
	std::map<UINT32, UINT32> m_LinkedLevels;
};

#endif // __SKILLDEFINE_H__