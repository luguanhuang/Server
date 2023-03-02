#include "pch.h"
#include "garden/rpcg2m_getgardeninfo.h"
#include "garden/garden_manager.h"
#include "role/rolemanager.h"
#include "garden/garden_config.h"
#include "role/rolesummarymgr.h"
// generate by ProtoGen at date: 2017/1/5 10:33:06

RPC_SERVER_IMPLEMETION(RpcG2M_GetGardenInfo, GetGardenInfoArg, GetGardenInfoRes)

void RpcG2M_GetGardenInfo::OnCall(const GetGardenInfoArg &roArg, GetGardenInfoRes &roRes)
{
	CRole* role = CRoleManager::Instance()->GetByRoleID(roArg.role_id());
	if (NULL == role)
	{
		SSWarn<<"role is NULL, role id:"<<roArg.role_id()<<END;
		return;
	}

	Garden* garden = GardenManager::Instance()->GetGarden(role->GetID(),role->GetName());		
	if (NULL == garden)
	{
		//roRes.set_result(KKSG::ERR_GARDEN_TOLOADING);
		return ;
	}
	FoodInfo food_info;
	garden->GetGardenCookingInfo(food_info);
	
	roRes.set_cook_level(food_info.cooking_level);
}

void RpcG2M_GetGardenInfo::OnDelayReplyRpc(const GetGardenInfoArg &roArg, GetGardenInfoRes &roRes, const CUserData &roUserData)
{
}
