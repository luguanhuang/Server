#ifndef __GUILD_CAMP_H__
#define __GUILD_CAMP_H__
#include "guild/guilddef.h"
#include "guild.h"
#include <time.h>
#include "guild/guildmgr.h"
#include "role/switch.h"
#include "guild/guildcamprank.h"

namespace KKSG 
{
	class GuildAllDBData;
	class GuildCampInfoRes;
	class GuildCampResult;
};

class CRole;
class GuildCampRankList;

struct CampInfo
{

};

class CGuildCamp : public IGuildSerializerInterface
{
public:
	CGuildCamp(Guild* pGuild);
	~CGuildCamp();

	virtual bool Load(const KKSG::GuildAllDBData & guilddata);
	virtual void Save(KKSG::GuildAllDBData & guilddata);
	virtual bool IsChanged();
	virtual const std::string ToBinary();

	GuildCampRankList* GetGuildCampRankList();
	void FillGuildCampInfo(KKSG::GuildCampInfoRes& roRes);

	void UpdateRankInfo(const KKSG::GuildCampResult& result);

	void SetChanged();
	bool OnTimer();
	void Sort();

	void Reset();

	void Update();

	UINT32 GetCurGuildCampID(){return m_curCampID;}

	void DebugReset(UINT32 day);

protected:

private:

	Guild *m_pGuild;
	Switch m_isModified;
	UINT32 m_timer;

	int m_updatetime;
	UINT32 m_curCampID;
	UINT32 m_nextCampID;
	GuildCampRankList m_rankList;
	
};
#endif // 
