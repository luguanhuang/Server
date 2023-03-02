#include "pch.h"
#include "ms_commondef_handler.h"
#include "util/jsonutil.h"
#include "role/bag.h"
#include "role/rolesummarymgr.h"

bool CMSIdipQueryReverseRoleHandler::ReadDataQequest() 
{
	RpcM2D_ReadRoleData* rpc = RpcM2D_ReadRoleData::CreateRpc();
	rpc->m_oArg.set_index(GetID());
	rpc->m_oArg.set_param(ReadOnly);
	rpc->m_oArg.set_roleid(GetRoleID());
	rpc->m_oArg.set_readtype(KKSG::ROLE_DATA_READ_IDIP);
	DBLink::Instance()->SendTo(*rpc);
	return true;
}
INT32 CMSIdipQueryReverseRoleHandler::HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	std::string strName = JsonUtil::GetJsonString(roBodyJson, "NickName");
	std::string  strNickID = JsonUtil::GetJsonString(roBodyJson, "UId");
	UINT64 NickID = strNickID.empty()?0:convert<UINT64>(strNickID);
	CRoleSummary* pRole = NULL;
	if (strName.size())
	{
		pRole = CRoleSummaryMgr::Instance()->GetRoleSummary(strName);
	}
	
	else if (NickID)
	{
		pRole = CRoleSummaryMgr::Instance()->GetRoleSummaryByNickId(NickID);
		if (NULL == pRole)
		{
			pRole = CRoleSummaryMgr::Instance()->GetRoleSummary(NickID);
		}
	}
	
	if (NULL == pRole || (strName.size() && NickID && (pRole->GetNickID() != NickID || pRole->GetID() != NickID)))
	{
		roReply.m_nErrCode = IDIP_ERR_PLAYER_NOT_EXIST;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	SetRoleID(pRole->GetID());
	return IDIP_SERVER_MS;
}
bool CMSIdipQueryReverseRoleHandler::OnAsyncReply(const CUserData& roUserData)
{
	if(roUserData.m_pUserPtr == NULL)
	{
		//timeout
		CRpc::ReplyDelayRpc(GetRpcID(), roUserData);
	}
	else
	{		
		rapidjson::Document doc;
		std::string s = GetStrJson();
		if (doc.Parse<0>(GetStrJson().c_str()).HasParseError() || roUserData.m_dwUserData != KKSG::ERR_SUCCESS)
		{
			std::string str;
			CIdipReply oReply(str);
			oReply.m_nErrCode = IDIP_ERR_UNKNOWN;
			oReply.m_strResult = PackErrResponse(oReply.m_nErrCode);
			CUserData data;
			data.m_pUserPtr = &oReply;
			CRpc::ReplyDelayRpc(GetRpcID(), data);
			return true;
		}
		UINT32 AreaID = JsonUtil::GetJsonUint(doc["body"], "AreaId");
		UINT32 PlatId = JsonUtil::GetJsonUint(doc["body"], "PlatId");
		RoleAllInfo* pInfo = (RoleAllInfo*)roUserData.m_pUserPtr;
		JsonBuilder oBuilder;
		rapidjson::Value oRole(rapidjson::kArrayType);
		UINT32 uRoleCount = 0;
		{
		rapidjson::Value oInfo(rapidjson::kObjectType);
		oInfo.AddMember("AreaId", AreaID, oBuilder.Allocator());
		oInfo.AddMember("PlatId", PlatId, oBuilder.Allocator());
		oInfo.AddMember("OpenId", pInfo->brief().accountid(), oBuilder.Allocator());
		oInfo.AddMember("NickName", UrlEncode(pInfo->brief().name()), oBuilder.Allocator());
		oInfo.AddMember("GoldNum", GetVirtualItem(*pInfo, GOLD), oBuilder.Allocator());
		oInfo.AddMember("Exp", pInfo->brief().exp(), oBuilder.Allocator());
		oInfo.AddMember("Level", pInfo->brief().level(), oBuilder.Allocator());
		oInfo.AddMember("UId", ToString(pInfo->brief().nickid()), oBuilder.Allocator());
		oInfo.AddMember("RoleId", ToString(GetRoleID()), oBuilder.Allocator());
		oRole.PushBack(oInfo, oBuilder.Allocator());
		uRoleCount ++;
		}

		oBuilder.AddMember_Int("Result", IDIP_ERR_SUCCESS);
		oBuilder.AddMember_Int("RoleReInfoList_count", uRoleCount);
		oBuilder.AddMember_Value("RoleReInfoList", oRole);

		std::string str = oBuilder.GetString();
		CIdipReply oReply(str);
		oReply.m_nErrCode = IDIP_ERR_SUCCESS;
		CUserData data;
		data.m_pUserPtr = &oReply;
		CRpc::ReplyDelayRpc(GetRpcID(), data);
	}
	return true;
}

