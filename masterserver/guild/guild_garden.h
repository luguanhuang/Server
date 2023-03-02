#ifndef __GUILD_GARDEN_H__
#define __GUILD_GARDEN_H__

#include "guild/guilddef.h"
#include "guild/guildlistener.h"
#include "garden/guild_garden_plant.h"

class Guild;
namespace KKSG
{ 
	class GuildAllDBData;
}
class GuildGarden : public IGuildSerializerInterface, public IGuildListener
{
public:
	GuildGarden(Guild* guild);
	virtual ~GuildGarden();

	virtual bool IsChanged();
	virtual bool Load(const KKSG::GuildAllDBData& guilddata);
	virtual void Save(KKSG::GuildAllDBData& guilddata);
	virtual const std::string ToBinary();

	virtual void OnAddMember(Guild *pGuild, GuildMember *pMember){};
	virtual void OnAddMemberFromDB(Guild *pGuild, GuildMember *pMember) { };
	virtual void OnRemoveMember(Guild *pGuild, UINT64 qwRoleID){};
	virtual void OnMemberPosChanged(Guild *pGuild, GuildMember *pMember) { };
	virtual void OnMembergGuildBossDps(Guild *pGuild, GuildMember *pMember) { };
public:
	GuildGardenPlant* GetGuildPlant();
	
	void AddEventLog(
		PlantInforRecord* record,
		const UINT64 role_id,
		const std::string& role_name,
		const UINT32 event_type,
		const UINT32 target,
		const bool result);

	const std::string& GetName()
	{
		return guild_plant_->GetName();
	}

	void SetGuildNameID(const std::string& garden_name,const UINT64 garden_id)
	{
		guild_plant_->SetGuildNameID(garden_name,garden_id);
	}

	void GetPlantInfo(std::vector<GardenPlantStatus>& plant_status);

	const PlantInfoStore& GetPlantInfoStore()
	{
		return guild_plant_->GetPlantInfoStore();
	}
public:
	// for test
	// plant 
	void AddGrownAmount(
		const UINT32 farmland_id,
		const UINT32 amount);
private:
	Guild* m_guild;
	GuildGardenPlant* guild_plant_ ;
	AddLogFunc add_log_func_;
	GardenStoreLogInfo garden_log_;
	UINT32 guild_level_;
};

#endif