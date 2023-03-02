#ifndef __REPORTDATA_MGR_H__
#define __REPORTDATA_MGR_H__
#include "role/role.h"
#include "guild/guilddef.h"
#include "guild/guild.h"
class CReportDataMgr
{
public:
	CReportDataMgr();
	~CReportDataMgr();
	DECLARE_SINGLETON(CReportDataMgr);

public:
	bool Init();
	void Uninit();
	void JoinGuild(CRole* role);
	void GuildLevelUp(CRole* role, UINT32 level);
	void CreateGuild(CRole* role, std::string name);
	void DismissGuild(UINT64 roleid);
	void LeaveGuild(UINT64 roleid);
	void GuildMemberPosChanged(UINT64 roleid, GuildPosition pos);
	void OnLogin(CRole* role);
	void GuildBindQQ(UINT64 roleid, UINT32 m_lastUpdateBindTime);
	void Common(CRole* role, INT32 type, const string& data);
	void MobaResult(UINT64 roleid, UINT32 todaynum, UINT32 realtotalnum, UINT32 realwinnum, bool iswin);

private:
	bool m_bIsQQ;
};
#endif

