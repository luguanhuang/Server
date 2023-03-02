#include "pch.h"
#include "ms_commondef_handler.h"
#include "util/jsonutil.h"
#include "gm/rpcm2n_gmdowhitelist.h"
#include "network/controllink.h"

IDIP_HANDLER_MODIFY_IMPLEMETION_EXCEPT_GSREPLAY(WhiteList)
IDIP_REPLY_FROMGS_IMPLEMETION(WhiteList)
INT32 CMSIdipWhiteListHandler::HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	szOpenId = JsonUtil::GetJsonString(roBodyJson, "OpenId");
	m_uItemID = JsonUtil::GetJsonInt(roBodyJson, "Type");
	if (GetCmd() == IDIP_DO_ADD_DEL_WHITE_LIST_REQ && m_uItemID != 1 && m_uItemID != 2)
	{
		roReply.m_nErrCode = IDIP_ERR_INVALID_PARA;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	return IDIP_SERVER_OTHER;
}

bool CMSIdipWhiteListHandler::ModifyMsData(CUserData& roUserData)
{
	return true;
}

void CMSIdipWhiteListHandler::IdipLog()
{

}

bool CMSIdipWhiteListHandler::DoOther(CIdipReply& roReply)
{
	RpcM2N_GMDoWhiteList* rpc = RpcM2N_GMDoWhiteList::CreateRpc();
	rpc->m_oArg.set_cmdid(GetCmd());
	rpc->m_oArg.set_openid(szOpenId);
	rpc->m_oArg.set_type(m_uItemID);
	rpc->m_oArg.set_httpid(GetID());
	ControlLink::Instance()->SendTo(*rpc);
	return true;
}

bool CMSIdipWhiteListHandler::OnAsyncReplyFromOther(const CUserData& roUserData)
{
	CRpc::ReplyDelayRpc(GetRpcID(), roUserData);
	return true;
}
