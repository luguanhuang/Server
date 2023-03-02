#include "pch.h"
#include "math.h"
#include "unit/rolemanager.h"
#include "gamelogic/jademgr.h"
#include "gamelogic/bagtransition.h"
#include "enhance/rpcc2g_decomposeequipment.h"
#include "util/XRandom.h"
#include "table/globalconfig.h"
#include "gamelogic/itemconfig.h"
#include "gamelogic/decompose.h"

// generate by ProtoGen at date: 2015/6/4 11:11:03

RPC_SERVER_IMPLEMETION(RpcC2G_DecomposeEquipment, DecomposeEquipmentArg, DecomposeEquipmentRes)

static UINT32 GetSelectEquip(const DecomposeEquipmentArg &roArg, Bag* bag, std::vector<std::pair<XItem*, UINT32>>& selectitems)
{
	for (int i = 0; i < roArg.equipuniqueid_size(); ++i)
	{
		if (i >= roArg.count_size())
		{
			break;
		}
		XItem* item = bag->FindUniqueItemInBag(roArg.equipuniqueid(i));
		if (item == NULL)
		{
			return ERR_DECOMPOSE_CANNOT;
		}
		ItemConf* conf = ItemConfig::Instance()->GetItemConf(item->itemID);
		if (conf == NULL)
		{
			LogWarn("Can't find item [%llu], itemid [%u] in ItemList.txt", item->uid, item->itemID);
			return ERR_DECOMPOSE_CANNOT;
		}
		if (conf->IsCanRecycle && roArg.count(i) != 0)
		{
			UINT32 count = roArg.count(i) > item->itemCount ? item->itemCount : roArg.count(i);
			selectitems.push_back(std::make_pair(item, count));
		}
		else
		{
			return ERR_DECOMPOSE_CANNOT;
		}
	}
	return ERR_SUCCESS;
}

void RpcC2G_DecomposeEquipment::OnCall(const DecomposeEquipmentArg &roArg, DecomposeEquipmentRes &roRes)
{
	Role* role = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == role)
	{
		SSWarn<<"role is NULL, session id:"<<m_sessionID<<END;
		roRes.set_errorcode(ERR_INVALID_REQUEST);
		return;
	}
	Bag* bag = role->Get<Bag>();

	std::vector<std::pair<XItem*, UINT32>> selectitems;
	UINT32 errorcode = ERR_SUCCESS;
	errorcode = GetSelectEquip(roArg, bag, selectitems);
	if (ERR_SUCCESS != errorcode)
	{
		roRes.set_errorcode((KKSG::ErrorCode)errorcode);
		return;
	}

	float param = XItem::GetDecomposeParam();

	DecomposeHelper helper(role);
	helper.Init(selectitems, param);
	bool istrue = helper.Fini();

	if (param > 1.0f)
	{
		roRes.add_param(param);
	}
	roRes.set_errorcode(istrue ? ERR_SUCCESS : ERR_DECOMPOSE_FAILED);

	return;
}

void RpcC2G_DecomposeEquipment::OnDelayReplyRpc(const DecomposeEquipmentArg &roArg, DecomposeEquipmentRes &roRes, const CUserData &roUserData)
{
}
