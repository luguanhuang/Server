#ifndef __GARDEN_EVENT_H__
#define __GARDEN_EVENT_H__
#include "unit/rolemanager.h"
#include "gamelogic/bagtransition.h"

class GardenEvent 
{
	GardenEvent();
	~GardenEvent();
	DECLARE_SINGLETON(GardenEvent);
public:
	// 家园事件的物品消耗
	INT32 ConsumeItems(
		Role& role,
		const UINT32 reson,
		const UINT32 sub_reson,
		std::vector<ItemDesc>& items);

	//void ChangeStatus();
	
	UINT32 CheckBanquetScenePlay(
		Scene& scene,
		const Role& role,
		std::vector<UINT64>& guests);

	// 
	void SetVector(Vector3& pos,float& face,const Sequence<float, 4>& seq);
	Role* SetRolePose(const UINT32 seques,Scene& scene, const UINT64 role_id,const INT32 index);
};

#endif // __GARDEN_EVENT_H__