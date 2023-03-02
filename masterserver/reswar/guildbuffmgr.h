#ifndef __GUILDBUFF_H__
#define __GUILDBUFF_H__

#include "table/GuildBuffTable.h"

#include "reswarmgrdef.h"

using namespace GuildResWar;
//设计的时候把它跟工会剥离关系、同时又把它跟资源战分离开
class GuildBuffMgr: public ITimer
{
	GuildBuffMgr();
	~GuildBuffMgr();
	DECLARE_SINGLETON(GuildBuffMgr);
public:
	bool Init();
	bool Check();
	void Uninit();
	GuildBuffInfo* GetGuildBuffInfo(UINT64 nGuildID,UINT32 nBuffID);
	UINT32 GetAddBuffTime(UINT64 nGuildID);
	bool DelBuff(GuildBuffInfo* pGuildBuff,UINT32 nID);
	bool AddBuff(UINT64 nGuildID,UINT64 nTargetGuildID,UINT32 nID);
	void ClearDeBuff(UINT64 nGuildID);
	void GetBuff(UINT64 nGuild, std::set<GuildBuffInfo *> &setGuildBuffInfo);
	bool IsUnknown(UINT64 nGuildID);
	void SendBuff(UINT64 nGuildID);
	GuildBuffTable::RowData * GetGuildBuffID(UINT32 nItemID);
	GuildBuffTable::RowData * GetGuildBuffItemID(UINT32 nItemID);
	void OnTimer(UINT32 dwID, UINT32 dwCount);
	void Clear();
	void AddRoleID(UINT64 roleID);
	void GetRoleID(std::set<UINT64> &setRole);
	void DelRoleID(UINT64 roleID);
private:
	std::set<UINT64> m_setRole;
	std::set<GuildBuffInfo*> m_setGuildBuff;
	GuildBuffTable m_guildBuffTable;
	UINT32 m_nTime;
	HTIMER m_handler;
};

#endif
