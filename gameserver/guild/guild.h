#ifndef __GUILD_H__
#define __GUILD_H__
#include "protocol.h"

typedef void (*MemberCB)(UINT64 roleId, void *arg);

class CGuildSimple
{
public:
	enum GuildPosition
	{
		GPOS_LEADER,       //会长
		GPOS_VICELEADER,   //副会长
		GPOS_OFFICER,      //官员
		GPOS_ELITEMEMBER,  //精英会员
		GPOS_MEMBER,       //普通会员
		GPOS_MAX,
	};

public:
	CGuildSimple();
	~CGuildSimple();

public:
	inline void SetGuildId(UINT64 guildId) { m_guildId = guildId; }
	inline void SetGuildLvl(UINT32 guildLvl) { m_guildLvl = guildLvl; }
	//inline void SetGuildLeaderId(UINT64 guildLeaderId) { m_guildLeaderId = guildLeaderId; }
	inline void SetGuildName(std::string guildName) { m_guildName = guildName; }
	inline void SetGuildIcon(UINT32 guildIcon) { m_guildIcon = guildIcon; }

	inline UINT64 GetGuildId() { return m_guildId; }
	inline UINT32 GetGuildLvl() { return m_guildLvl; }
	//inline UINT64 GetGuildLeaderId() { return m_guildLeaderId; }
	inline UINT32 GetGuildIcon() { return m_guildIcon; }
	inline std::string GetGuildName() { return m_guildName; }
	inline const std::map<UINT64, UINT32>& getAllMember() { return m_allMemberMap; } 
	inline bool IsGuildLeader(UINT64 roleId) { return GetPosition(roleId) == GPOS_LEADER; }
	inline bool IsGuildVicLeader(UINT64 roleId) { return GetPosition(roleId) == GPOS_VICELEADER; }
	//inline UINT32 GetRecruitTime() { return m_recruitTime; }
	//inline void SetRecruitTime(UINT32 nowTime) { m_recruitTime = nowTime; }

	UINT32 GetPosition(UINT64 roleId);
	UINT64 GetGuildLeaderId();
	UINT64 GetGuildLeader2Id();//副会长
	void BroadCast(const CProtocol& ptcMsg);
	void AddGuildMember(UINT64 roleId, UINT32 position);
	void DeleteMember(UINT64 roleId);
	void ForeachMember(MemberCB cb, void *arg);
	void FillGuildBrief(KKSG::MyGuild* ginfo);

private:
	UINT64 m_guildId;
	UINT32 m_guildLvl;
	//UINT64 m_guildLeaderId;
	UINT32 m_guildIcon;
	//UINT32 m_recruitTime;   // 招募冷却时间
	std::string m_guildName;
	std::map<UINT64, UINT32> m_allMemberMap;

};

#endif // __GUILD_H__