#ifndef __GUILDCHECKIN_H__
#define __GUILDCHECKIN_H__

#include "guild/guilddef.h"
//#include "unit/roleserilizeinterface.h"
#include "role/switch.h"
#include "table/GuildCheckinTable.h"
#include "table/GuildCheckinBoxTable.h"
#include "define/itemdef.h"
#include "role/role.h"


struct GuildCheckinRecord
{
	UINT64 roleid;
	UINT32 type;
	UINT32 timestamp;

	GuildCheckinRecord()
	{
		roleid = 0;
		type = 0;
		timestamp = 0;
	}
};

class CGuildCheckin : public IGuildSerializerInterface
{
public:
	CGuildCheckin();
	virtual ~CGuildCheckin();

	virtual bool IsChanged();
	virtual bool Load(const KKSG::GuildAllDBData & guilddata);
	virtual void Save(KKSG::GuildAllDBData & guilddata);
	virtual const std::string ToBinary();

	void Update();

	inline UINT32 GetPorcess() { return m_process; }
	inline UINT32 GetCount() { return m_count; }
	inline void DebugSetCount(int num){m_count = num;}

	void Checkin(UINT32 process, UINT32 type, UINT64 roleid);
	void HintNotify(CRole* role);

	const std::vector<GuildCheckinRecord>& GetRecords();

private:
	Switch m_switch;
	UINT32 m_updateday;
	UINT32 m_process;
	UINT32 m_count;
	std::vector<GuildCheckinRecord> m_records;
};

class Role;
class Guild;
struct ItemDesc;
class GuildCheckinMgr
{
	GuildCheckinMgr();
	~GuildCheckinMgr();
	DECLARE_SINGLETON(GuildCheckinMgr);
public:
	bool Init();
	void Uninit();

	bool OnTimer();

	UINT32 GetPorcess(UINT32 type);
	ItemDesc GetConsume(UINT32 type);
	ItemDesc GetReward(UINT32 type);
	void GetGuildReward(UINT32 type, std::map<UINT32, UINT32>& guildReward);
	std::vector<ItemDesc> GetBoxReward(UINT32 index, UINT32 process);

	void NotifyProcess(CRole* pRole, Guild* guild);

private:
	UINT32 m_timer;
	GuildCheckinTable m_table;
	GuildCheckinBoxTable m_boxtable;
};

#endif