#ifndef __GUILD_GOBLIN_H__
#define __GUILD_GOBLIN_H__
#include "guild/guilddef.h"
#include "guild.h"
#include <time.h>
#include "guild/guildmgr.h"
#include "role/switch.h"

using namespace KKSG;
namespace KKSG 
{
	class GuildAllDBData;
	class KillGoblinTotalInfo;
}

class CRole;

#define MAX_ENTER_GOBLIN_COUNT 5
#define SMALL_GOBLIN_ID 10001
#define BIG_GOBLIN_ID 10002
#define GUILD_GOBLIN_SCENE 2000

struct KillGoblin
{
	UINT64 roleID;
	std::string roleName;
	int num;
	int ability;
	int roleLevel; 
	bool operator < (const KillGoblin& a)const
	{
		if (num > a.num)
		{
			return true;
		}
		return false;
	}
	void load(const KKSG::KillGoblinInfo& info);
	void save(KKSG::KillGoblinInfo& info);

};
class GuildGoblin : public IGuildSerializerInterface
{
public:
	GuildGoblin(Guild* pGuild);
	~GuildGoblin();

	virtual bool Load(const KKSG::GuildAllDBData & guilddata);
	virtual void Save(KKSG::GuildAllDBData & guilddata);
	virtual bool IsChanged();
	virtual const std::string ToBinary();

	bool OnTimer();
	void Sort();

	int getRank(UINT64 roleID);
	int getGoblinLevel();
	void getSummaryInfo(CRole* pCRole, KKSG::GuildGoblinInfoRes& res);

	void addKillNum(UINT64 roleID, int num);
	void addKillNum(int num);
	void UpdateGoblinInfo(const KKSG::KillGoblinTotalInfo* info);

protected:
	int getLeftEnterCount(CRole* role);
	void Update();

private:

	Guild *m_pGuild;
	Switch m_isModified;
	UINT32 m_timer;
	int m_totalKillNum;
	std::vector<KillGoblin> m_vecKillInfo;
	std::unordered_map<UINT64, UINT32> m_roleid2rank;
	int m_updatetime;
	
};
#endif // __GUILD_GOBLIN_H__
