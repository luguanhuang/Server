#include "pch.h"
#include "marriage/rpcm2g_checkmarriagecouple.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "tablemgr/XEntityInfoLibrary.h"
#include "scene/scene.h"
#include "gamelogic/bag.h"
#include "gamelogic/bagtransition.h"

// generate by ProtoGen at date: 2017/7/26 19:59:05

RPC_SERVER_IMPLEMETION(RpcM2G_CheckMarriageCouple, CheckMarriageCoupleArg, CheckMarriageCoupleRes)

void RpcM2G_CheckMarriageCouple::OnCall(const CheckMarriageCoupleArg &roArg, CheckMarriageCoupleRes &roRes)
{
	roRes.set_error(KKSG::ERR_SUCCESS);
	Role* role1 = RoleManager::Instance()->FindByRoleID(roArg.role1());
	Role* role2 = RoleManager::Instance()->FindByRoleID(roArg.role2());
	if (!role1 || !role2 || !role1->GetCurrScene() || !role2->GetCurrScene() || role1->GetCurrScene()->GetSceneTemplateID() != HALL_MAP_ID || role2->GetCurrScene()->GetSceneTemplateID() != HALL_MAP_ID)
	{
		roRes.set_error(KKSG::ERR_MARRIAGE_DISTANCE_TOOFAR);
		return;
	}

	if (roArg.optype() == KKSG::MarriageOpType_MarryApply || roArg.optype() == KKSG::MarriageOpType_MarryAgree)
	{
		auto data1 = XEntityInfoLibrary::Instance()->TemplateID2Data_Role(role1->GetProfession());
		auto data2 = XEntityInfoLibrary::Instance()->TemplateID2Data_Role(role2->GetProfession());
		if (!data1 || !data2 || data1->Sex == data2->Sex)
		{
			roRes.set_error(KKSG::ERR_MARRIAGE_NEED_TWO_NOTMARRY);
			return;
		}

		auto pos1 = role1->GetXRole()->GetPosition_p();
		auto pos2 = role2->GetXRole()->GetPosition_p();

		float dis = Vector3::Distance(pos1, pos2);
		if (dis > GetGlobalConfig().ApplyMarriageDistance)
		{
			roRes.set_error(KKSG::ERR_MARRIAGE_DISTANCE_TOOFAR);
			return;
		}
	}


	UINT32 costID = 0;
	UINT32 costNum = 0;
	if (roArg.optype() == KKSG::MarriageOpType_MarryApply || roArg.optype() == KKSG::MarriageOpType_MarryAgree)
	{
		if (roArg.type() == KKSG::WeddingType_Normal)
		{
			costID = GetGlobalConfig().MarriageCost.seq[0];
			costNum = GetGlobalConfig().MarriageCost.seq[1];
		}
		else if (roArg.type() == KKSG::WeddingType_Luxury)
		{
			costID = GetGlobalConfig().MarriageSplendidCost.seq[0];
			costNum = GetGlobalConfig().MarriageSplendidCost.seq[1];
		}
		else
		{
			roRes.set_error(KKSG::ERR_ITEM_NOTEXIST);
			return;
		}
	}
	else if (roArg.optype() == KKSG::MarriageOpType_Divorce)
	{
		costID = GetGlobalConfig().MarriageDivorceCost.seq[0];
		costNum = GetGlobalConfig().MarriageDivorceCost.seq[1];
	}

	UINT64 ownNum = 0;
	if (XItem::IsVirtualItem(costID))
	{
		ownNum = role1->Get<Bag>()->CountVirtualItem(costID);
	}
	else
	{
		ownNum = role1->Get<Bag>()->CountItemInBag(costID);
	}
	if (ownNum < costNum)
	{
		roRes.set_error(KKSG::ERR_ITEM_NOT_ENOUGH_COMMON);
		return;
	}

	if (roArg.optype() == KKSG::MarriageOpType_MarryAgree || roArg.optype() == KKSG::MarriageOpType_Divorce)
	{
		BagTakeItemTransition transition(role1);
		if (roArg.optype() == KKSG::MarriageOpType_MarryAgree)
		{
			transition.SetReason(ItemFlow_Marriage, ItemFlow_Marriage_Marry);
		}
		else
		{
			transition.SetReason(ItemFlow_Marriage, ItemFlow_Marriage_Divorce);
		}
		if (!transition.TakeItem(costID, costNum))
		{
			transition.RollBack();
			roRes.set_error(KKSG::ERR_ITEM_NOT_ENOUGH_COMMON);
		}
		transition.NotifyClient();
	}

	roRes.set_role1(roArg.role1());
	roRes.set_role2(roArg.role2());

	return;
}

void RpcM2G_CheckMarriageCouple::OnDelayReplyRpc(const CheckMarriageCoupleArg &roArg, CheckMarriageCoupleRes &roRes, const CUserData &roUserData)
{
}
