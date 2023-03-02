#ifndef __DRAGON_GUILD_H__
#define __DRAGON_GUILD_H__
#include "protocol.h"

typedef void (*MemberCB)(UINT64 roleId, void *arg);

class CDragonGuildSimple
{
public:
	enum DragonGuildPosition
	{
		GPOS_LEADER,       //会长
		GPOS_VICELEADER,   //副会长
		GPOS_MEMBER,       //普通会员
		GPOS_MAX,
	};

public:
	CDragonGuildSimple();
	~CDragonGuildSimple();

public:
	inline void SetDragonGuildId(UINT64 guildId) { m_dragonGuildId = guildId; }
	inline void SetDragonGuildLvl(UINT32 guildLvl) { m_dragonGuildLvl = guildLvl; }

	inline void SetDragonGuildName(std::string guildName) { m_dragonGuildName = guildName; }

	inline UINT64 GetDragonGuildId() { return m_dragonGuildId; }
	inline UINT32 GetDragonGuildLvl() { return m_dragonGuildLvl; }

	inline std::string GetDragonGuildName() { return m_dragonGuildName; }
	inline const std::map<UINT64, UINT32>& getAllMember() { return m_allMemberMap; } 
	inline bool IsDragonGuildLeader(UINT64 roleId) { return GetPosition(roleId) == GPOS_LEADER; }
	inline bool IsDragonGuildVicLeader(UINT64 roleId) { return GetPosition(roleId) == GPOS_VICELEADER; }
	

	UINT32 GetPosition(UINT64 roleId);
	UINT64 GetDragonGuildLeaderId();
	UINT64 GetDragonGuildLeader2Id();//副会长
	void BroadCast(const CProtocol& ptcMsg);
	void AddDragonGuildMember(UINT64 roleId, UINT32 position);
	void DeleteMember(UINT64 roleId);
	void ForeachMember(MemberCB cb, void *arg);
	void FillDragonGuildBrief(KKSG::MyGuild* ginfo);

private:
	UINT64 m_dragonGuildId;
	UINT32 m_dragonGuildLvl;
	
	std::string m_dragonGuildName;
	std::map<UINT64, UINT32> m_allMemberMap;

};

#endif