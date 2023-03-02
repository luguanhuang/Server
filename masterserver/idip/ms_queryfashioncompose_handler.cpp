#include "pch.h"
#include "ms_commondef_handler.h"
#include "util/jsonutil.h"

IDIP_HANDLER_READ_IMPLEMETION(QueryFashionCompose)
INT32 CMSIdipQueryFashionComposeHandler::HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	UINT64 roleid = JsonUtil::GetJsonUint64(roBodyJson, "RoleId");
	szOpenId = JsonUtil::GetJsonString(roBodyJson, "OpenId");
	if (!CRoleSummaryMgr::Instance()->IsValidAcountRole(szOpenId, roleid))
	{
		roReply.m_nErrCode = IDIP_ERR_PLAYER_NOT_EXIST;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	SetRoleID(roleid);

	CRole* pRole = CRoleManager::Instance()->GetByRoleID(roleid);
	if (NULL != pRole)//Íæ¼ÒÔÚÏß
	{
		return IDIP_SERVER_GS;
	}
	else
	{
		return IDIP_SERVER_MS;
	}
}

bool CMSIdipQueryFashionComposeHandler::OnAsyncReply(const CUserData& roUserData)
{
	if(roUserData.m_pUserPtr == NULL)
	{
		//timeout
		CRpc::ReplyDelayRpc(GetRpcID(), roUserData);
	}
	else
	{		
		std::string str;
		CIdipReply oReply(str);
		CUserData data;
		data.m_pUserPtr = &oReply;
		if(roUserData.m_dwUserData != KKSG::ERR_SUCCESS)
		{
			oReply.m_nErrCode = IDIP_ERR_UNKNOWN;
			oReply.m_strResult = PackErrResponse(oReply.m_nErrCode);
			CRpc::ReplyDelayRpc(GetRpcID(), data);
			return true;
		}
		RoleAllInfo* pInfo = (RoleAllInfo*)roUserData.m_pUserPtr;
		const KKSG::FashionRecord &roFashion = pInfo->fashionrecord();
		JsonBuilder oBuilder;

		oBuilder.AddMember_Int("Num", roFashion.compose_success_num() + roFashion.compose_failed_num());
		oBuilder.AddMember_Int("SuccessNum", roFashion.compose_success_num());
		oBuilder.AddMember_Int("FailNum", roFashion.compose_failed_num());
		INT32 sNum = 0;
		INT32 aNum = 0;
		for (INT32 i = 0; i < roFashion.quality_num_list_size(); i ++)
		{
			if (roFashion.quality_num_list(i).key() == FashionSQuality)
			{
				sNum = roFashion.quality_num_list(i).value();
			}
			else if (roFashion.quality_num_list(i).key() == FashionAQuality)
			{
				aNum = roFashion.quality_num_list(i).value();
			}
		}
		oBuilder.AddMember_Int("SNum", sNum);
		oBuilder.AddMember_Int("ANum", aNum);


		oReply.m_nErrCode = IDIP_ERR_SUCCESS;
		oReply.m_strResult = oBuilder.GetString();
		CRpc::ReplyDelayRpc(GetRpcID(), data);
	}
	return true;
}
