#ifndef __GUILD_ACTIVITY_H__
#define __GUILD_ACTIVITY_H__

// 公会活跃度

#include "guild.h"
//#include "unit/roleserilizeinterface.h"
#include "role/switch.h"
#include "guildlistener.h"

class CGuildActivity : public IGuildSerializerInterface, public IGuildListener
{

public:
	static const UINT32 DAY_TIME = 24 * 60 * 60;
	static const UINT32 WEEK_TIME = 7 * DAY_TIME;

	struct stGuildActivity
	{
		UINT64 nRoleID;
		UINT32 nActivity;
		stGuildActivity()
		{
			nRoleID = 0;
			nActivity = 0;
		}
	};

	struct stGuildActivityCmp
	{
		bool operator()(const stGuildActivity &p1, const stGuildActivity &p2)
		{
			if(p1.nActivity != p2.nActivity)
			{
				return p1.nActivity > p2.nActivity;
			}
			if (p1.nRoleID != p2.nRoleID)
			{
				return p1.nRoleID > p2.nRoleID;
			}
			return true;
		}
	};

	struct stGuildActivityFind
	{
	public:
		stGuildActivityFind(const UINT64 nRoleID):m_nRoleID(nRoleID){}
		bool operator()(stGuildActivity &info)
		{
			return info.nRoleID == m_nRoleID;
		}
	private:
		UINT64 m_nRoleID;
	};



public:
	CGuildActivity();
	CGuildActivity(Guild* pGuild);
	~CGuildActivity();

public:
	virtual bool IsChanged();
	virtual bool Load(const KKSG::GuildAllDBData & guilddata);
	virtual void Save(KKSG::GuildAllDBData & guilddata);
	virtual const std::string ToBinary();

	virtual void OnAddMember(Guild *pGuild, GuildMember *pMember);
	virtual void OnRemoveMember(Guild *pGuild, UINT64 qwRoleID);
	virtual void OnAddMemberFromDB(Guild *pGuild, GuildMember *pMember){}
	virtual void OnMemberPosChanged(Guild *pGuild, GuildMember *pMember) {}
	virtual void OnMembergGuildBossDps(Guild *pGuild, GuildMember *pMember) {}
	virtual void OnGuildLevelUp(Guild *pGuild){}

	void Sort();

public:
	bool DoCheck();/*true解散公会 false不*/      // 活跃度评审 
	void DailyCheck();                          // 日评审
	bool WeekCheck();/*true解散公会 false不*/    // 周评审

	void AddActivity(UINT64 roleId, UINT32 addNum);
	void FillActivityRank(KKSG::AskGuildWageInfoRes &refData);
	void DelActivity(UINT64 roleId, UINT32 delNum);
	void DoRoleAdd(UINT64 roleId);
	void DoRoleLeave(UINT64 roleId);
	void GetStand(UINT32& activity, UINT32& activityOne, UINT32& activityTwo, UINT32& activityThree);
	inline void SetChang() { m_isModified.Set(); }
	inline UINT32 GetActivity() { return m_weekActivity; }
	inline UINT32 GetRoleActivity(UINT64 roleId) { return m_roleActivityMap.find(roleId) != m_roleActivityMap.end() ? m_roleActivityMap[roleId] : 0; }
	void DoTxLog();

private:
	bool _IsOutNewProtTime();
	void _CheckLeaderActivity();
	UINT64 _FindNextLeader();

private:
	UINT32 m_weekActivity;  // 周累积活跃度
	UINT32 m_losTimes;      // 累积周结算失败次数
	std::map<UINT64, UINT32> m_roleActivityMap; // 玩家活跃度记录
	std::vector<stGuildActivity> m_vecguildActivity;


	Switch m_isModified;
	Guild *m_pGuild;
};


#endif // __GUILD_ACTIVITY_H__
