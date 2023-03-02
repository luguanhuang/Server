#ifndef __GUILDLADDER_H__
#define __GUILDLADDER_H__

#include "timer.h"
#include "table/GuildPkRankReward.h"
#include "table/GuildPkRoleRankReward.h"

using std::make_pair;

struct ItemDesc;

struct GuildGroup  // С������
{
	GuildGroup()
	{
		m_isEnd = false;
		m_rewardTimes.clear();
		m_roleRank.clear();
		m_guildRank.clear();
	}

	GuildGroup(const std::set<UINT64>& guildSet)
	{
		m_isEnd = false;
		m_rewardTimes.clear();
		m_roleRank.clear();
		m_guildRank.clear();

		for (auto iter = guildSet.begin(); iter != guildSet.end(); ++iter)
		{
			m_guildRank.insert(make_pair(0, *iter));
		}
	}

	~GuildGroup()
	{

	}

	void AddTime(UINT64 roleId);  // ������ɴ���
	void DoEnd();                 // ��������
	void GiveRoleRankReward();
	void GiveGuildRankReward();
	bool TryReward(UINT64 roleId);
	UINT32 GetGuildRank(UINT64 guildId); 
	void GetThreeMember(UINT64 guildId, std::set<UINT64>& roleSet);


	bool m_isEnd;  // �������ű��
	std::map<UINT64, UINT32> m_rewardTimes;      // ���ӽ�����ȡ����
	std::multimap<UINT32, UINT64> m_roleRank;  // ������а�
	std::multimap<UINT32, UINT64> m_guildRank; // �������а�
};

class CGuildLadder : public ITimer
{
public:
	enum STATE
	{
		READY = 1,
		GOING = 2,
	};

private:
	CGuildLadder();
	~CGuildLadder();
	DECLARE_SINGLETON(CGuildLadder);

public:
	bool Init(); 
	void Uninit();
	virtual void OnTimer(UINT32 dwID, UINT32 dwCount); 


public:
	bool IsOpen();
	bool TryReward(UINT64 roleId);
	void GetRoleReward(UINT32 rank, std::vector<ItemDesc>& vecItem);
	void GetGuildReward(UINT32 rank, std::vector<ItemDesc>& vecItem, std::map<UINT32, UINT32>& guildReward);
	GuildGroup* GetGuildGroup(UINT64 guildId);
	void DoOnlineCheck(UINT64 roleId);
	UINT32 GetLastTime();

private:
	bool _IsReadyTime();
	bool _IsLastTen();
	bool _IsLastFive();
	void _DoStart();
	void _DoEnd();
	void _ClearData();
	void _CreateGroup(const std::set<UINT64>& guildSet);
	void _NoticeTen();
	void _NoticeFive();
	void _NoticeEnd();
    STATE _GetTimeState();

	//void _NoticeReady();

private:
	HTIMER m_timehandler;
	STATE m_state;   // ״̬���
	GuildPkRankReward m_guildRankTable;
	GuildPkRoleRankReward m_roleRankTable;
	std::set<GuildGroup*> m_groupSet;
	std::map<UINT64, GuildGroup*>  m_groupMap;  // ��ʱ� ���ݲ��洢

};




#endif