#include "pch.h"
#include "enhance/rpcc2g_getenhanceattr.h"
#include "gamelogic/enhanceequipmgr.h"

// generate by ProtoGen at date: 2017/7/6 16:17:50

RPC_SERVER_IMPLEMETION(RpcC2G_GetEnhanceAttr, GetEnhanceAttrArg, GetEnhanceAttrRes)

void RpcC2G_GetEnhanceAttr::OnCall(const GetEnhanceAttrArg &roArg, GetEnhanceAttrRes &roRes)
{
	const std::vector<Sequence<UINT32, 2>>* pAttrs = EnhanceEquipMgr::Instance()->GetEnhanceAttr(roArg.equippos(), roArg.enhancelevel(), roArg.prof());	
	if (pAttrs == NULL)
	{
		roRes.set_isend(true);
		return;
	}
	roRes.set_isend(false);
	for (auto i = pAttrs->begin(); i != pAttrs->end(); ++i)
	{
		KKSG::AttributeInfo* attr = roRes.add_attrs();
		attr->set_id((*i)[0]);
		attr->set_value((*i)[1]);
	}
}

void RpcC2G_GetEnhanceAttr::OnDelayReplyRpc(const GetEnhanceAttrArg &roArg, GetEnhanceAttrRes &roRes, const CUserData &roUserData)
{
}
