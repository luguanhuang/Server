#include "pch.h"
#include "garden/rpcc2m_gardenfishinfo.h"
#include "garden/garden_manager.h"
#include "role/rolemanager.h"
#include "garden/garden_utility.h"
// generate by ProtoGen at date: 2016/10/22 17:31:29

RPC_SERVER_IMPLEMETION(RpcC2M_GardenFishInfo, GardenFishInfoArg, GardenFishInfoRes)

	void RpcC2M_GardenFishInfo::OnCall(const GardenFishInfoArg &roArg, GardenFishInfoRes &roRes)
{
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == role)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		SSWarn<<"role is NULL, session id:"<<m_sessionID<<END;		
		return;
	}

	// 获取的仍然是自己家园的钓鱼等级和经验
	Garden* garden =  GardenUtility::Instance()->GetMyGarden(role->GetID(),role->GetName());
	if (NULL == garden)
	{
		LogWarn("not find the garden,role id is : %llu",role->GetID());
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return;
	}

	UINT64 garden_id = role->GetID();
	if (role->GetID() != roArg.garden_id())
	{	
		garden_id = roArg.garden_id();
	}

	const FishInfoStore& fish_info = garden->GetFishInfo();
	roRes.set_fish_level(fish_info.fish_level);	
	roRes.set_experiences(fish_info.experiences);
	roRes.set_result(KKSG::ERR_SUCCESS);

	// 通知钓鱼改变朝向 		
	KKSG::GardenPlayStageChange arg;					
	arg.set_event_type(KKSG::FISH_FACE);	
	// 借用下这个字段
	arg.set_role_id(role->GetID());
	GardenUtility::Instance()->NotifyEvnetChangeNtf(arg,garden_id);
}

void RpcC2M_GardenFishInfo::OnDelayReplyRpc(const GardenFishInfoArg &roArg, GardenFishInfoRes &roRes, const CUserData &roUserData)
{
}
