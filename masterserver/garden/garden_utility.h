#ifndef __GARDEN_UTILITY_H__
#define __GARDEN_UTILITY_H__

#include <string>
#include "role/role.h"
#include "garden/garden_define.h"
#include "garden/garden.h"
#include "guild/guild_garden.h"
#include "util/gametime.h"

class GardenUtility
{
	GardenUtility();
	~GardenUtility();
	DECLARE_SINGLETON(GardenUtility);
public:
	
	// ��ȡ�����԰��
	bool GetGardenNameForGuild(
		const UINT64 role_id,
		const UINT64 garden_id,
		std::string& garden_name);

	//// ��plant��¼��ӵ�res��
	void AddPlantInfoToRes(
		const UINT32 farmland_id,
		const PlantInforRecord& record,
		KKSG::FetchPlantInfoRes& roRes);

	// ��ȡ���Ѽ�԰��
	bool GetGardenNameForFriend(
		const UINT64 garden_id,	
		std::string& garden_name);

	// ��ȡ��԰��
	void GetGardenName(
		const UINT64 role_id,
		const UINT64 garden_id,
		std::string& garden_name);

	// ��ֲ �仯֪ͨ
	void NotifyEvnetChangeNtf(
		const KKSG::GardenPlayStageChange& arg,
		const INT64 garden_id,
		const bool guild_garden = false);

	// rpc ֪ͨ ֲ��׶α仯
	void RpcPlantChangeStage(
		const UINT64 garden_id,
		const UINT32 farmland_id,
		const UINT32 sprite_id,
		const UINT32 grow_state,
		bool guild_garden = false);

	// �����ѻ�ȡ
	void FriendOrderByDegree(
		CRole* role,
		const std::vector<UINT64>& all,
		std::vector<UINT64>& guests);

	// ֪ͨ��Ὺʼ
	void NotifyBanquetQuests(
		const UINT64 garden_id,
		const UINT32 banquet_id,
		const std::vector<UINT64>& guests);

	Garden* GetMyGarden(
		const UINT64 garden_id ,
		const std::string& garden_name);

	Garden* GetFriendGarden(
		const UINT64 garden_id ,
		const std::string& garden_name);

	GuildGarden* GetGuildGarden(const UINT64 garden_id);

	void CheckGardenHintNotifyMS(
		const KKSG::GardenPlayStageChange& arg,
		const UINT64 garden_id);

	Garden* GetGardenByRoleId(const UINT64 garden_id );

	// �������˵ĵؿ���
	UINT32 CheckPlantStauts(const PlantInfoStore& store);
};

#endif // __GARDEN_UTILITY_H__