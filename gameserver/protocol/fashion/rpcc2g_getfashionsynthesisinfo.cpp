#include "pch.h"
#include "fashion/rpcc2g_getfashionsynthesisinfo.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/rolefashion.h"
#include "gamelogic/fashionmgr.h"
#include "define/tlogenum.h"
#include "gamelogic/ibshopmgr.h"

// generate by ProtoGen at date: 2017/9/6 15:24:32

RPC_SERVER_IMPLEMETION(RpcC2G_GetFashionSynthesisInfo, FashionSynthesisInfoArg, FashionSynthesisInfoRes)

	void RpcC2G_GetFashionSynthesisInfo::OnCall(const FashionSynthesisInfoArg &roArg, FashionSynthesisInfoRes &roRes)
{
	roRes.set_fashion_id(0);
	roRes.set_add_succes_rate(0);	
	roRes.set_result(KKSG::ERR_SUCCESS);

	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == pRole)
	{
		SSWarn<<"pRole is NULL, session id:"<<m_sessionID<<END;
		roRes.set_result(ERR_INVALID_REQUEST);
		return;
	}
	///> system open
	if (!pRole->Get<CRoleSystem>()->IsSystemOpened(SYS_FASHION_COMPOSE))
	{
		SSInfo<<"system close"<<END;
		roRes.set_result(ERR_SYS_NOTOPEN);
		return;
	}

	UINT32 suit_id  =  FashionMgr::Instance()->GetFashionSuitID(roArg.fashion_id());
	if (suit_id == 0)
	{
		SSWarn<<" not exist this fashion ,id :"<<roArg.fashion_id()<<END;
		roRes.set_result(ERR_INVALID_REQUEST);
		return;
	}
	auto suit_cfg = FashionMgr::Instance()->GetFashionSuitConf(suit_id);
	if (suit_cfg == NULL)
	{
		SSWarn<<" not exist this fashion ,id :"<<roArg.fashion_id()<<END;
		roRes.set_result(ERR_INVALID_REQUEST);
		return;
	}

	if ( pRole->GetLevel() < suit_cfg->ShowLevel)
	{
		SSWarn<<" not exist this fashion ,id :"<<roArg.fashion_id()<<END;
		roRes.set_result(ERR_INVALID_REQUEST);
		return;
	}

	auto config_info =  FashionMgr::Instance()->GetFashinSynthesisinfo(roArg.fashion_id());
	if (config_info == NULL)
	{
		SSWarn<<" not exist this fashion ,id :"<<roArg.fashion_id()<<END;
		roRes.set_result(ERR_INVALID_REQUEST);
		return;
	}

	roRes.set_fashion_id(config_info->FashionID);	

	UINT32 count = pRole->Get<RoleFashion>()->GetFashionSynthesisFaildCount(roArg.fashion_id());

	UINT32 add_rate = count * config_info->FashinSynthesisAddSucessRate;

	roRes.set_add_succes_rate(add_rate);
}

void RpcC2G_GetFashionSynthesisInfo::OnDelayReplyRpc(const FashionSynthesisInfoArg &roArg, FashionSynthesisInfoRes &roRes, const CUserData &roUserData)
{
}
