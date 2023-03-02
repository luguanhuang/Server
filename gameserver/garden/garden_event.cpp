#include "pch.h"
#include "garden/garden_event.h"
#include "gamelogic/rolesummarymgr.h"
#include "scene/scenemanager.h"
#include "scene/scene.h"
#include "unit/roleoutlook.h"
#include "rolesummary/rolesummarybase.h"

INSTANCE_SINGLETON(GardenEvent);

GardenEvent::GardenEvent()
{

}

GardenEvent::~GardenEvent()
{

}

INT32 GardenEvent::ConsumeItems(
	Role& role,
	const UINT32 reson,
	const UINT32 sub_reson,
	std::vector<ItemDesc>& items)
{
	BagTakeItemTransition transition(&role);
	transition.SetReason(ItemFlow_Garden,ItemFlow_Garden_Banquet);
	auto it =items.begin();
	auto it_end =items.end();
	for (; it != it_end; ++it )
	{
		if (!transition.TakeItem(it->itemID,(int)it->itemCount))
		{
			transition.RollBack();	
			return KKSG::ERR_ITEM_NOT_ENOUGH;				
		}

		SSInfo<<"itemid:"<<it->itemID <<" count:"<<it->itemCount<<END;
	}
	transition.NotifyClient();

	return KKSG::ERR_SUCCESS;
}

UINT32 GardenEvent::CheckBanquetScenePlay(
	Scene& scene,
	const Role& role,
	std::vector<UINT64>& guests)
{
	const std::list<Role*>& output = scene.GetAllRoles();	
	if (output.size() < GetGlobalConfig().GardenBanquetGuests)
	{
		//  不足4个人，没法继续
		return KKSG::ERR_GARDEN_QUESTS_NOENOUGH;
	}

	bool exist = false;
	auto it = output.begin();
	auto it_end = output.end();
	for (; it != it_end; ++it)
	{
		if ((*it)->GetID() == role.GetID())
		{
			// 自己在家园，才能继续
			exist = true;
		}
		guests.push_back((*it)->GetID());
	}
	if (!exist)
	{
		return KKSG::ERR_GARDEN_NOTINGARDEN;
	}

	return KKSG::ERR_SUCCESS;
}


void GardenEvent::SetVector(Vector3& pos,float& face,const Sequence<float, 4>& seq)
{
	pos.x = seq.seq[0];
	pos.y = seq.seq[1];
	pos.z = seq.seq[2];
	face = seq.seq[3];
}

Role* GardenEvent::SetRolePose(const UINT32 seques,Scene& scene, const UINT64 role_id,const INT32 index)
{
	Role* role = scene.FindSceneRole(role_id);
	if (NULL != role)
	{
		Vector3 pos;
		float face=0.0;
		if (0 == index)
		{
			if (1 == seques)
			{
				Sequence<float, 4>& seq = GetGlobalConfig().HomeFeastDes1;
				SetVector(pos,face,seq);
			}
			if (0 == seques)
			{
				Sequence<float, 4>& seq = GetGlobalConfig().HomeFeastEndDes1;
				SetVector(pos,face,seq);
			}
		}
		if (1 == index)
		{
			if (1 == seques)
			{
				Sequence<float, 4>& seq = GetGlobalConfig().HomeFeastDes2;
				SetVector(pos,face,seq);
			}
			if (0 == seques)
			{
				Sequence<float, 4>& seq = GetGlobalConfig().HomeFeastEndDes2;
				SetVector(pos,face,seq);
			}
		}
		if (2 == index)
		{
			if (1 == seques)
			{
				Sequence<float, 4>& seq = GetGlobalConfig().HomeFeastDes3;
				SetVector(pos,face,seq);
			}
			if (0 == seques)
			{
				Sequence<float, 4>& seq = GetGlobalConfig().HomeFeastEndDes3;
				SetVector(pos,face,seq);
			}
		}
		if (3 == index)
		{
			if (1 == seques)
			{
				Sequence<float, 4>& seq = GetGlobalConfig().HomeFeastDes4;
				SetVector(pos,face,seq);
			}
			if (0 == seques)
			{
				Sequence<float, 4>& seq = GetGlobalConfig().HomeFeastEndDes4;
				SetVector(pos,face,seq);
			}
		}

		XEntity* entity = dynamic_cast<XEntity*>(role->GetXObject());
		if(entity)
		{
			entity->TransferLocation(pos, face);
		}

		///> 开始
		if (1 == seques)
		{
			role->SetState(KKSG::OutLook_Sit);
			RoleOutLook outlook(role);
			outlook.Fill(SUMMARY_STATE, role);
			outlook.BroadCast();

			role->MakePuppet(true);
		}
		///> 结束
		else if (0 == seques)
		{
			if (KKSG::OutLook_Sit == role->GetState().statetype())
			{
				role->SetState(KKSG::OutLook_Normal);
				RoleOutLook outlook(role);
				outlook.Fill(SUMMARY_STATE, role);
				outlook.BroadCast();
			}
			role->MakePuppet(false);
		}

		LogDebug("TransferLocation id  is %llu  and pos is  %f,%f,%f,%f ", role->GetID(),pos.x,pos.y,pos.z,face);
	}				
	return role;
}