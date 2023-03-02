#ifndef __GUILDWAGE_H__
#define __GUILDWAGE_H__

#include "guild/guilddef.h"
#include "guild/guildmgr.h"
#include "role/switch.h"
#include "guild.h"



struct BaseData
{
	BaseData()
	{
		m_exp = 0;
		m_rolenum = 0;
		m_prestige = 0;
		m_activity = 0;
	}

	UINT32 m_exp;       // ���ܾ���ֵ
	UINT32 m_rolenum;   // ��������
	UINT32 m_prestige;  // ��������ֵ
	UINT32 m_activity;  // ���ܻ�Ծ��
};

struct WageState
{
	WageState()
	{
		m_position = GPOS_MAX;
		m_reward = 1; // 1������ 2�ѽ���ȡ 3��û����
	}

	UINT32 m_position; // ְλ
	UINT32 m_reward; // �콱״̬
};

class CGuildWage : public IGuildSerializerInterface
{
public:
	enum WageLvl
	{
		S = 1,
		A = 2,
		B = 3,
		C = 4,
		D = 5,
		WAGE_MAX = 6,
	};

public:
	CGuildWage();
	CGuildWage(Guild* pGuild);
	~CGuildWage();

public:
	virtual bool IsChanged();
	virtual bool Load(const KKSG::GuildAllDBData & guilddata);
	virtual void Save(KKSG::GuildAllDBData& guilddata);
	virtual const std::string ToBinary();

public:
	void DoCheckWork();
	bool GetReward(CRole* pRole);
	UINT32 IsReward(UINT64 roleId);
	UINT32 GetPosition(UINT64 roleId);
	void OnlineCheck(CRole* pRole);

	inline UINT32 GetLastScore() { return m_lastScore; }
	inline UINT32 GetWageLvl() { return m_wageLvl; }
	inline UINT32 GetGuildLvl() { return m_lastGuildLvl; }
	inline BaseData& GetBaseData() { return m_base; } 
	inline void SetPrestige(UINT32 num) { m_base.m_prestige = num; }

	void GMSetWageLvl(UINT32 lvl);

private: 
	UINT32 _CalcWageLvl();
	void _GetReward(UINT64 roleId, std::vector<ItemBrief>& itemVec);

private:
	Guild* m_guild;
	Switch m_switch;
	BaseData m_base;
	WageLvl m_wageLvl;     // ��������
	UINT32 m_lastScore;    // ���ܷ���
	UINT32 m_lastGuildLvl; // ���ܹ���ȼ�
	//UINT32 m_lastExp;      // ���ܹ��ᾭ��
	std::map<UINT64, WageState> m_rewardRoleMap;  // �����콱�ĳ�Ա  ����ʱ��Ӧ��ְλ
};


#endif
