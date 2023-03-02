#include "pch.h"
#include "gm/rpcm2n_gmdowhitelist.h"
#include "idip/idip.h"
#include "util/jsonutil.h"
#include "rapidjson/document.h"
#include "account/whitelistmgr.h"
#include "idip/idiphandler.h"

// generate by ProtoGen at date: 2017/8/22 15:21:06

RPC_SERVER_IMPLEMETION(RpcM2N_GMDoWhiteList, GMDoWhiteListArg, GMDoWhiteListRes)

void RpcM2N_GMDoWhiteList::OnCall(const GMDoWhiteListArg &roArg, GMDoWhiteListRes &roRes)
{
	if (roArg.cmdid() == IDIP_QUERY_WHITE_LIST_REQ)
	{
		JsonBuilder oBuilder;
		bool isWhilteList = CWhiteListMgr::Instance()->IsOpenIDInWhiteList(roArg.openid());
		oBuilder.AddMember_Int("IsExist", isWhilteList?1:0);
		const std::string& str = oBuilder.GetString();
		roRes.set_result(IDIP_ERR_SUCCESS);
		roRes.set_resultstring(str);
	}
	else if (roArg.cmdid() == IDIP_DO_ADD_DEL_WHITE_LIST_REQ)
	{
		if (roArg.type() == 1)
		{
			CWhiteListMgr::Instance()->Add(roArg.openid());
		}
		else //2
		{
			CWhiteListMgr::Instance()->Del(roArg.openid());
		}
		roRes.set_result(IDIP_ERR_SUCCESS);
		roRes.set_resultstring(PackErrResponse(IDIP_ERR_SUCCESS));
	}
}

void RpcM2N_GMDoWhiteList::OnDelayReplyRpc(const GMDoWhiteListArg &roArg, GMDoWhiteListRes &roRes, const CUserData &roUserData)
{
}
